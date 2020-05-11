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
 *     and register accessor functions for the etrans block
 *****************************************************************************/
#ifndef _ETRANS_IO_INLINE_H
#define _ETRANS_IO_INLINE_H

#include "etrans_loc.h"
#include "etrans_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ETRANS_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for etrans
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
    etrans_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} etrans_buffer_t;
static INLINE void etrans_buffer_init( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void etrans_buffer_init( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "etrans_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void etrans_buffer_flush( etrans_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void etrans_buffer_flush( etrans_buffer_t *b_ptr )
{
    IOLOG( "etrans_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 etrans_reg_read( etrans_buffer_t *b_ptr,
                                      etrans_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_read( etrans_buffer_t *b_ptr,
                                      etrans_handle_t *h_ptr,
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
static INLINE void etrans_reg_write( etrans_buffer_t *b_ptr,
                                     etrans_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_write( etrans_buffer_t *b_ptr,
                                     etrans_handle_t *h_ptr,
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

static INLINE void etrans_field_set( etrans_buffer_t *b_ptr,
                                     etrans_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 mask,
                                     UINT32 unused_mask,
                                     UINT32 ofs,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_set( etrans_buffer_t *b_ptr,
                                     etrans_handle_t *h_ptr,
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

static INLINE void etrans_action_on_write_field_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 mem_type,
                                                     UINT32 reg,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_action_on_write_field_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
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

static INLINE void etrans_burst_read( etrans_buffer_t *b_ptr,
                                      etrans_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void etrans_burst_read( etrans_buffer_t *b_ptr,
                                      etrans_handle_t *h_ptr,
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

static INLINE void etrans_burst_write( etrans_buffer_t *b_ptr,
                                       etrans_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void etrans_burst_write( etrans_buffer_t *b_ptr,
                                       etrans_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE etrans_poll( etrans_buffer_t *b_ptr,
                                                etrans_handle_t *h_ptr,
                                                UINT32 mem_type,
                                                UINT32 reg,
                                                UINT32 mask,
                                                UINT32 value,
                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                UINT32 max_count,
                                                UINT32 *num_failed_polls,
                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_poll( etrans_buffer_t *b_ptr,
                                                etrans_handle_t *h_ptr,
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
 *  register access functions for etrans
 * ==================================================================================
 */

static INLINE void etrans_reg_GLOBAL_SOFT_RST_CFG_write( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_GLOBAL_SOFT_RST_CFG_write( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_GLOBAL_SOFT_RST_CFG_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_GLOBAL_SOFT_RST_CFG,
                      value);
}

static INLINE void etrans_reg_GLOBAL_SOFT_RST_CFG_field_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_GLOBAL_SOFT_RST_CFG_field_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_GLOBAL_SOFT_RST_CFG_field_set", mask, ofs, value );
    etrans_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_GLOBAL_SOFT_RST_CFG,
                      mask,
                      PMC_ETRANS120_REG_GLOBAL_SOFT_RST_CFG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 etrans_reg_GLOBAL_SOFT_RST_CFG_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_GLOBAL_SOFT_RST_CFG_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ETRANS120_REG_GLOBAL_SOFT_RST_CFG);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_GLOBAL_SOFT_RST_CFG_read", reg_value);
    return reg_value;
}

static INLINE void etrans_reg_GLOBAL_LPBK_CFG_write( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_GLOBAL_LPBK_CFG_write( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_GLOBAL_LPBK_CFG_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_GLOBAL_LPBK_CFG,
                      value);
}

static INLINE void etrans_reg_GLOBAL_LPBK_CFG_field_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_GLOBAL_LPBK_CFG_field_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_GLOBAL_LPBK_CFG_field_set", mask, ofs, value );
    etrans_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_GLOBAL_LPBK_CFG,
                      mask,
                      PMC_ETRANS120_REG_GLOBAL_LPBK_CFG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 etrans_reg_GLOBAL_LPBK_CFG_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_GLOBAL_LPBK_CFG_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ETRANS120_REG_GLOBAL_LPBK_CFG);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_GLOBAL_LPBK_CFG_read", reg_value);
    return reg_value;
}

static INLINE void etrans_reg_RX_100G_LINK_CFG_REG_write( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_RX_100G_LINK_CFG_REG_write( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_RX_100G_LINK_CFG_REG_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_RX_100G_LINK_CFG_REG,
                      value);
}

static INLINE void etrans_reg_RX_100G_LINK_CFG_REG_field_set( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_RX_100G_LINK_CFG_REG_field_set( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_RX_100G_LINK_CFG_REG_field_set", mask, ofs, value );
    etrans_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_RX_100G_LINK_CFG_REG,
                      mask,
                      PMC_ETRANS120_REG_RX_100G_LINK_CFG_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 etrans_reg_RX_100G_LINK_CFG_REG_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_RX_100G_LINK_CFG_REG_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ETRANS120_REG_RX_100G_LINK_CFG_REG);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_RX_100G_LINK_CFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void etrans_reg_TX_100G_LINK_CFG_write( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_100G_LINK_CFG_write( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_TX_100G_LINK_CFG_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_TX_100G_LINK_CFG,
                      value);
}

static INLINE void etrans_reg_TX_100G_LINK_CFG_field_set( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_100G_LINK_CFG_field_set( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_TX_100G_LINK_CFG_field_set", mask, ofs, value );
    etrans_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_TX_100G_LINK_CFG,
                      mask,
                      PMC_ETRANS120_REG_TX_100G_LINK_CFG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 etrans_reg_TX_100G_LINK_CFG_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_TX_100G_LINK_CFG_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ETRANS120_REG_TX_100G_LINK_CFG);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_TX_100G_LINK_CFG_read", reg_value);
    return reg_value;
}

static INLINE void etrans_reg_TX_CHID_LOOKUP_CFG_REG0_write( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_CHID_LOOKUP_CFG_REG0_write( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_TX_CHID_LOOKUP_CFG_REG0_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0,
                      value);
}

static INLINE void etrans_reg_TX_CHID_LOOKUP_CFG_REG0_field_set( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_CHID_LOOKUP_CFG_REG0_field_set( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_TX_CHID_LOOKUP_CFG_REG0_field_set", mask, ofs, value );
    etrans_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0,
                      mask,
                      PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 etrans_reg_TX_CHID_LOOKUP_CFG_REG0_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_TX_CHID_LOOKUP_CFG_REG0_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_TX_CHID_LOOKUP_CFG_REG0_read", reg_value);
    return reg_value;
}

static INLINE void etrans_reg_TX_CHID_LOOKUP_CFG_REG1_write( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_CHID_LOOKUP_CFG_REG1_write( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_TX_CHID_LOOKUP_CFG_REG1_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1,
                      value);
}

static INLINE void etrans_reg_TX_CHID_LOOKUP_CFG_REG1_field_set( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_CHID_LOOKUP_CFG_REG1_field_set( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_TX_CHID_LOOKUP_CFG_REG1_field_set", mask, ofs, value );
    etrans_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1,
                      mask,
                      PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 etrans_reg_TX_CHID_LOOKUP_CFG_REG1_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_TX_CHID_LOOKUP_CFG_REG1_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_TX_CHID_LOOKUP_CFG_REG1_read", reg_value);
    return reg_value;
}

static INLINE void etrans_reg_TX_CHID_LOOKUP_CFG_REG2_write( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_CHID_LOOKUP_CFG_REG2_write( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_TX_CHID_LOOKUP_CFG_REG2_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2,
                      value);
}

static INLINE void etrans_reg_TX_CHID_LOOKUP_CFG_REG2_field_set( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_CHID_LOOKUP_CFG_REG2_field_set( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_TX_CHID_LOOKUP_CFG_REG2_field_set", mask, ofs, value );
    etrans_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2,
                      mask,
                      PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 etrans_reg_TX_CHID_LOOKUP_CFG_REG2_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_TX_CHID_LOOKUP_CFG_REG2_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_TX_CHID_LOOKUP_CFG_REG2_read", reg_value);
    return reg_value;
}

static INLINE void etrans_reg_TX_OP_SCH_CAL_CFG0_write( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_OP_SCH_CAL_CFG0_write( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_TX_OP_SCH_CAL_CFG0_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0,
                      value);
}

static INLINE void etrans_reg_TX_OP_SCH_CAL_CFG0_field_set( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_OP_SCH_CAL_CFG0_field_set( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_TX_OP_SCH_CAL_CFG0_field_set", mask, ofs, value );
    etrans_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0,
                      mask,
                      PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 etrans_reg_TX_OP_SCH_CAL_CFG0_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_TX_OP_SCH_CAL_CFG0_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_TX_OP_SCH_CAL_CFG0_read", reg_value);
    return reg_value;
}

static INLINE void etrans_reg_TX_OP_SCH_CAL_CFG1_write( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_OP_SCH_CAL_CFG1_write( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_TX_OP_SCH_CAL_CFG1_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1,
                      value);
}

static INLINE void etrans_reg_TX_OP_SCH_CAL_CFG1_field_set( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_OP_SCH_CAL_CFG1_field_set( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_TX_OP_SCH_CAL_CFG1_field_set", mask, ofs, value );
    etrans_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1,
                      mask,
                      PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 etrans_reg_TX_OP_SCH_CAL_CFG1_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_TX_OP_SCH_CAL_CFG1_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_TX_OP_SCH_CAL_CFG1_read", reg_value);
    return reg_value;
}

static INLINE void etrans_reg_LINKID_MAPPING_array_write( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_LINKID_MAPPING_array_write( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_LINKID_MAPPING_array_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_LINKID_MAPPING(N),
                      value);
}

static INLINE void etrans_reg_LINKID_MAPPING_array_field_set( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_LINKID_MAPPING_array_field_set( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_LINKID_MAPPING_array_field_set", N, mask, ofs, value );
    etrans_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_LINKID_MAPPING(N),
                      mask,
                      PMC_ETRANS120_REG_LINKID_MAPPING_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 etrans_reg_LINKID_MAPPING_array_read( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_LINKID_MAPPING_array_read( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ETRANS120_REG_LINKID_MAPPING(N));

    IOLOG( "%s -> 0x%08x; N=%d", "etrans_reg_LINKID_MAPPING_array_read", reg_value, N);
    return reg_value;
}

static INLINE void etrans_reg_RX_10G_LINK_CFG_REG_array_write( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_RX_10G_LINK_CFG_REG_array_write( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_RX_10G_LINK_CFG_REG_array_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_RX_10G_LINK_CFG_REG(N),
                      value);
}

static INLINE void etrans_reg_RX_10G_LINK_CFG_REG_array_field_set( etrans_buffer_t *b_ptr,
                                                                   etrans_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_RX_10G_LINK_CFG_REG_array_field_set( etrans_buffer_t *b_ptr,
                                                                   etrans_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_RX_10G_LINK_CFG_REG_array_field_set", N, mask, ofs, value );
    etrans_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_RX_10G_LINK_CFG_REG(N),
                      mask,
                      PMC_ETRANS120_REG_RX_10G_LINK_CFG_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 etrans_reg_RX_10G_LINK_CFG_REG_array_read( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_RX_10G_LINK_CFG_REG_array_read( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ETRANS120_REG_RX_10G_LINK_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "etrans_reg_RX_10G_LINK_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void etrans_reg_TX_10G_LINK_CFG_array_write( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_10G_LINK_CFG_array_write( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_TX_10G_LINK_CFG_array_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_TX_10G_LINK_CFG(N),
                      value);
}

static INLINE void etrans_reg_TX_10G_LINK_CFG_array_field_set( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_10G_LINK_CFG_array_field_set( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_TX_10G_LINK_CFG_array_field_set", N, mask, ofs, value );
    etrans_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_TX_10G_LINK_CFG(N),
                      mask,
                      PMC_ETRANS120_REG_TX_10G_LINK_CFG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 etrans_reg_TX_10G_LINK_CFG_array_read( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_TX_10G_LINK_CFG_array_read( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ETRANS120_REG_TX_10G_LINK_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "etrans_reg_TX_10G_LINK_CFG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void etrans_reg_RX_40G_LINK_CFG_REG_array_write( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_RX_40G_LINK_CFG_REG_array_write( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_RX_40G_LINK_CFG_REG_array_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_RX_40G_LINK_CFG_REG(N),
                      value);
}

static INLINE void etrans_reg_RX_40G_LINK_CFG_REG_array_field_set( etrans_buffer_t *b_ptr,
                                                                   etrans_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_RX_40G_LINK_CFG_REG_array_field_set( etrans_buffer_t *b_ptr,
                                                                   etrans_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_RX_40G_LINK_CFG_REG_array_field_set", N, mask, ofs, value );
    etrans_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_RX_40G_LINK_CFG_REG(N),
                      mask,
                      PMC_ETRANS120_REG_RX_40G_LINK_CFG_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 etrans_reg_RX_40G_LINK_CFG_REG_array_read( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_RX_40G_LINK_CFG_REG_array_read( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ETRANS120_REG_RX_40G_LINK_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "etrans_reg_RX_40G_LINK_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void etrans_reg_TX_40G_LINK_CFG_array_write( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_40G_LINK_CFG_array_write( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_TX_40G_LINK_CFG_array_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_TX_40G_LINK_CFG(N),
                      value);
}

static INLINE void etrans_reg_TX_40G_LINK_CFG_array_field_set( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_40G_LINK_CFG_array_field_set( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_TX_40G_LINK_CFG_array_field_set", N, mask, ofs, value );
    etrans_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ETRANS120_REG_TX_40G_LINK_CFG(N),
                      mask,
                      PMC_ETRANS120_REG_TX_40G_LINK_CFG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 etrans_reg_TX_40G_LINK_CFG_array_read( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_TX_40G_LINK_CFG_array_read( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ETRANS120_REG_TX_40G_LINK_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "etrans_reg_TX_40G_LINK_CFG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void etrans_reg_RXFIFO_OVERFLOW_INT_EN_write( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_RXFIFO_OVERFLOW_INT_EN_write( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_RXFIFO_OVERFLOW_INT_EN_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN,
                      value);
}

static INLINE void etrans_reg_RXFIFO_OVERFLOW_INT_EN_field_set( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_RXFIFO_OVERFLOW_INT_EN_field_set( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_RXFIFO_OVERFLOW_INT_EN_field_set", mask, ofs, value );
    etrans_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN,
                      mask,
                      PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 etrans_reg_RXFIFO_OVERFLOW_INT_EN_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_RXFIFO_OVERFLOW_INT_EN_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_RXFIFO_OVERFLOW_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void etrans_reg_RXFIFO_UNDERRUN_INT_EN_write( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_RXFIFO_UNDERRUN_INT_EN_write( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_RXFIFO_UNDERRUN_INT_EN_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN,
                      value);
}

static INLINE void etrans_reg_RXFIFO_UNDERRUN_INT_EN_field_set( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_RXFIFO_UNDERRUN_INT_EN_field_set( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_RXFIFO_UNDERRUN_INT_EN_field_set", mask, ofs, value );
    etrans_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN,
                      mask,
                      PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 etrans_reg_RXFIFO_UNDERRUN_INT_EN_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_RXFIFO_UNDERRUN_INT_EN_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_RXFIFO_UNDERRUN_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void etrans_reg_TX_FIFO_OVF_INT_EN_write( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_FIFO_OVF_INT_EN_write( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_TX_FIFO_OVF_INT_EN_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN,
                      value);
}

static INLINE void etrans_reg_TX_FIFO_OVF_INT_EN_field_set( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_FIFO_OVF_INT_EN_field_set( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_TX_FIFO_OVF_INT_EN_field_set", mask, ofs, value );
    etrans_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN,
                      mask,
                      PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 etrans_reg_TX_FIFO_OVF_INT_EN_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_TX_FIFO_OVF_INT_EN_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_TX_FIFO_OVF_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void etrans_reg_TX_FIFO_UNDERRUN_INT_EN_write( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_FIFO_UNDERRUN_INT_EN_write( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_TX_FIFO_UNDERRUN_INT_EN_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN,
                      value);
}

static INLINE void etrans_reg_TX_FIFO_UNDERRUN_INT_EN_field_set( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_FIFO_UNDERRUN_INT_EN_field_set( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_TX_FIFO_UNDERRUN_INT_EN_field_set", mask, ofs, value );
    etrans_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN,
                      mask,
                      PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 etrans_reg_TX_FIFO_UNDERRUN_INT_EN_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_TX_FIFO_UNDERRUN_INT_EN_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_TX_FIFO_UNDERRUN_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void etrans_reg_RXFIFO_OVERFLOW_INT_STAT_write( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_RXFIFO_OVERFLOW_INT_STAT_write( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_RXFIFO_OVERFLOW_INT_STAT_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT,
                      value);
}

static INLINE void etrans_reg_RXFIFO_OVERFLOW_INT_STAT_action_on_write_field_set( etrans_buffer_t *b_ptr,
                                                                                  etrans_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_RXFIFO_OVERFLOW_INT_STAT_action_on_write_field_set( etrans_buffer_t *b_ptr,
                                                                                  etrans_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_RXFIFO_OVERFLOW_INT_STAT_action_on_write_field_set", mask, ofs, value );
    etrans_action_on_write_field_set( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT,
                                      mask,
                                      ofs,
                                      value);

}

static INLINE UINT32 etrans_reg_RXFIFO_OVERFLOW_INT_STAT_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_RXFIFO_OVERFLOW_INT_STAT_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_RXFIFO_OVERFLOW_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_reg_RXFIFO_OVERFLOW_INT_STAT_poll( etrans_buffer_t *b_ptr,
                                                                             etrans_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_reg_RXFIFO_OVERFLOW_INT_STAT_poll( etrans_buffer_t *b_ptr,
                                                                             etrans_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "etrans_reg_RXFIFO_OVERFLOW_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return etrans_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE void etrans_reg_RXFIFO_UNDERRUN_INT_STAT_write( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_RXFIFO_UNDERRUN_INT_STAT_write( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_RXFIFO_UNDERRUN_INT_STAT_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT,
                      value);
}

static INLINE void etrans_reg_RXFIFO_UNDERRUN_INT_STAT_action_on_write_field_set( etrans_buffer_t *b_ptr,
                                                                                  etrans_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_RXFIFO_UNDERRUN_INT_STAT_action_on_write_field_set( etrans_buffer_t *b_ptr,
                                                                                  etrans_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_RXFIFO_UNDERRUN_INT_STAT_action_on_write_field_set", mask, ofs, value );
    etrans_action_on_write_field_set( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT,
                                      mask,
                                      ofs,
                                      value);

}

static INLINE UINT32 etrans_reg_RXFIFO_UNDERRUN_INT_STAT_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_RXFIFO_UNDERRUN_INT_STAT_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_RXFIFO_UNDERRUN_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_reg_RXFIFO_UNDERRUN_INT_STAT_poll( etrans_buffer_t *b_ptr,
                                                                             etrans_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_reg_RXFIFO_UNDERRUN_INT_STAT_poll( etrans_buffer_t *b_ptr,
                                                                             etrans_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "etrans_reg_RXFIFO_UNDERRUN_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return etrans_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE void etrans_reg_TX_FIFO_OVF_INT_STAT_write( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_FIFO_OVF_INT_STAT_write( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_TX_FIFO_OVF_INT_STAT_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT,
                      value);
}

static INLINE void etrans_reg_TX_FIFO_OVF_INT_STAT_action_on_write_field_set( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_FIFO_OVF_INT_STAT_action_on_write_field_set( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_TX_FIFO_OVF_INT_STAT_action_on_write_field_set", mask, ofs, value );
    etrans_action_on_write_field_set( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT,
                                      mask,
                                      ofs,
                                      value);

}

static INLINE UINT32 etrans_reg_TX_FIFO_OVF_INT_STAT_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_TX_FIFO_OVF_INT_STAT_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_TX_FIFO_OVF_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_reg_TX_FIFO_OVF_INT_STAT_poll( etrans_buffer_t *b_ptr,
                                                                         etrans_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_reg_TX_FIFO_OVF_INT_STAT_poll( etrans_buffer_t *b_ptr,
                                                                         etrans_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "etrans_reg_TX_FIFO_OVF_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return etrans_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE void etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_write( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_write( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_write", value );
    etrans_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT,
                      value);
}

static INLINE void etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_action_on_write_field_set( etrans_buffer_t *b_ptr,
                                                                                   etrans_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_action_on_write_field_set( etrans_buffer_t *b_ptr,
                                                                                   etrans_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_action_on_write_field_set", mask, ofs, value );
    etrans_action_on_write_field_set( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT,
                                      mask,
                                      ofs,
                                      value);

}

static INLINE UINT32 etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return etrans_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE UINT32 etrans_reg_RXFIFO_OVERFLOW_INT_VAL_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_RXFIFO_OVERFLOW_INT_VAL_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_RXFIFO_OVERFLOW_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_reg_RXFIFO_OVERFLOW_INT_VAL_poll( etrans_buffer_t *b_ptr,
                                                                            etrans_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_reg_RXFIFO_OVERFLOW_INT_VAL_poll( etrans_buffer_t *b_ptr,
                                                                            etrans_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "etrans_reg_RXFIFO_OVERFLOW_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return etrans_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE UINT32 etrans_reg_RXFIFO_UNDERRUN_INT_VAL_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_RXFIFO_UNDERRUN_INT_VAL_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_RXFIFO_UNDERRUN_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_reg_RXFIFO_UNDERRUN_INT_VAL_poll( etrans_buffer_t *b_ptr,
                                                                            etrans_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_reg_RXFIFO_UNDERRUN_INT_VAL_poll( etrans_buffer_t *b_ptr,
                                                                            etrans_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "etrans_reg_RXFIFO_UNDERRUN_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return etrans_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE UINT32 etrans_reg_TX_FIFO_OVF_INT_VAL_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_TX_FIFO_OVF_INT_VAL_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_TX_FIFO_OVF_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_reg_TX_FIFO_OVF_INT_VAL_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_reg_TX_FIFO_OVF_INT_VAL_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "etrans_reg_TX_FIFO_OVF_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return etrans_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE UINT32 etrans_reg_TX_FIFO_UNDERRUN_INT_VAL_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_reg_TX_FIFO_UNDERRUN_INT_VAL_read( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = etrans_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "etrans_reg_TX_FIFO_UNDERRUN_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_reg_TX_FIFO_UNDERRUN_INT_VAL_poll( etrans_buffer_t *b_ptr,
                                                                             etrans_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_reg_TX_FIFO_UNDERRUN_INT_VAL_poll( etrans_buffer_t *b_ptr,
                                                                             etrans_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "etrans_reg_TX_FIFO_UNDERRUN_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return etrans_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL,
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
static INLINE void etrans_field_SW_100G_RST_set( etrans_buffer_t *b_ptr,
                                                 etrans_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_SW_100G_RST_set( etrans_buffer_t *b_ptr,
                                                 etrans_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_SW_100G_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_SW_100G_RST_set", value );

    /* (0x0000003c bits 2) field SW_100G_RST of register PMC_ETRANS120_REG_GLOBAL_SOFT_RST_CFG */
    etrans_reg_GLOBAL_SOFT_RST_CFG_field_set( b_ptr,
                                              h_ptr,
                                              ETRANS120_REG_GLOBAL_SOFT_RST_CFG_BIT_SW_100G_RST_MSK,
                                              ETRANS120_REG_GLOBAL_SOFT_RST_CFG_BIT_SW_100G_RST_OFF,
                                              value);
}

static INLINE UINT32 etrans_field_SW_100G_RST_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_SW_100G_RST_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 2) field SW_100G_RST of register PMC_ETRANS120_REG_GLOBAL_SOFT_RST_CFG */
    reg_value = etrans_reg_GLOBAL_SOFT_RST_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_GLOBAL_SOFT_RST_CFG_BIT_SW_100G_RST_MSK) >> ETRANS120_REG_GLOBAL_SOFT_RST_CFG_BIT_SW_100G_RST_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_SW_100G_RST_get", value );

    return value;
}
static INLINE void etrans_field_SW_40G_RST_set( etrans_buffer_t *b_ptr,
                                                etrans_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_SW_40G_RST_set( etrans_buffer_t *b_ptr,
                                                etrans_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_SW_40G_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_SW_40G_RST_set", value );

    /* (0x0000003c bits 1) field SW_40G_RST of register PMC_ETRANS120_REG_GLOBAL_SOFT_RST_CFG */
    etrans_reg_GLOBAL_SOFT_RST_CFG_field_set( b_ptr,
                                              h_ptr,
                                              ETRANS120_REG_GLOBAL_SOFT_RST_CFG_BIT_SW_40G_RST_MSK,
                                              ETRANS120_REG_GLOBAL_SOFT_RST_CFG_BIT_SW_40G_RST_OFF,
                                              value);
}

static INLINE UINT32 etrans_field_SW_40G_RST_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_SW_40G_RST_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 1) field SW_40G_RST of register PMC_ETRANS120_REG_GLOBAL_SOFT_RST_CFG */
    reg_value = etrans_reg_GLOBAL_SOFT_RST_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_GLOBAL_SOFT_RST_CFG_BIT_SW_40G_RST_MSK) >> ETRANS120_REG_GLOBAL_SOFT_RST_CFG_BIT_SW_40G_RST_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_SW_40G_RST_get", value );

    return value;
}
static INLINE void etrans_field_SW_10G_RST_set( etrans_buffer_t *b_ptr,
                                                etrans_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_SW_10G_RST_set( etrans_buffer_t *b_ptr,
                                                etrans_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_SW_10G_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_SW_10G_RST_set", value );

    /* (0x0000003c bits 0) field SW_10G_RST of register PMC_ETRANS120_REG_GLOBAL_SOFT_RST_CFG */
    etrans_reg_GLOBAL_SOFT_RST_CFG_field_set( b_ptr,
                                              h_ptr,
                                              ETRANS120_REG_GLOBAL_SOFT_RST_CFG_BIT_SW_10G_RST_MSK,
                                              ETRANS120_REG_GLOBAL_SOFT_RST_CFG_BIT_SW_10G_RST_OFF,
                                              value);
}

static INLINE UINT32 etrans_field_SW_10G_RST_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_SW_10G_RST_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 0) field SW_10G_RST of register PMC_ETRANS120_REG_GLOBAL_SOFT_RST_CFG */
    reg_value = etrans_reg_GLOBAL_SOFT_RST_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_GLOBAL_SOFT_RST_CFG_BIT_SW_10G_RST_MSK) >> ETRANS120_REG_GLOBAL_SOFT_RST_CFG_BIT_SW_10G_RST_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_SW_10G_RST_get", value );

    return value;
}
static INLINE void etrans_field_GBL_ETRANS_LB_MCAST_EN_set( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_GBL_ETRANS_LB_MCAST_EN_set( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_GBL_ETRANS_LB_MCAST_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_GBL_ETRANS_LB_MCAST_EN_set", value );

    /* (0x00000048 bits 31) field GBL_ETRANS_LB_MCAST_EN of register PMC_ETRANS120_REG_GLOBAL_LPBK_CFG */
    etrans_reg_GLOBAL_LPBK_CFG_field_set( b_ptr,
                                          h_ptr,
                                          ETRANS120_REG_GLOBAL_LPBK_CFG_BIT_GBL_ETRANS_LB_MCAST_EN_MSK,
                                          ETRANS120_REG_GLOBAL_LPBK_CFG_BIT_GBL_ETRANS_LB_MCAST_EN_OFF,
                                          value);
}

static INLINE UINT32 etrans_field_GBL_ETRANS_LB_MCAST_EN_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_GBL_ETRANS_LB_MCAST_EN_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000048 bits 31) field GBL_ETRANS_LB_MCAST_EN of register PMC_ETRANS120_REG_GLOBAL_LPBK_CFG */
    reg_value = etrans_reg_GLOBAL_LPBK_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_GLOBAL_LPBK_CFG_BIT_GBL_ETRANS_LB_MCAST_EN_MSK) >> ETRANS120_REG_GLOBAL_LPBK_CFG_BIT_GBL_ETRANS_LB_MCAST_EN_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_GBL_ETRANS_LB_MCAST_EN_get", value );

    return value;
}
static INLINE void etrans_field_GBL_ETRANS_LB_EN_set( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_GBL_ETRANS_LB_EN_set( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_GBL_ETRANS_LB_EN_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "etrans_field_GBL_ETRANS_LB_EN_set", value );

    /* (0x00000048 bits 11:0) field GBL_ETRANS_LB_EN of register PMC_ETRANS120_REG_GLOBAL_LPBK_CFG */
    etrans_reg_GLOBAL_LPBK_CFG_field_set( b_ptr,
                                          h_ptr,
                                          ETRANS120_REG_GLOBAL_LPBK_CFG_BIT_GBL_ETRANS_LB_EN_MSK,
                                          ETRANS120_REG_GLOBAL_LPBK_CFG_BIT_GBL_ETRANS_LB_EN_OFF,
                                          value);
}

static INLINE UINT32 etrans_field_GBL_ETRANS_LB_EN_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_GBL_ETRANS_LB_EN_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000048 bits 11:0) field GBL_ETRANS_LB_EN of register PMC_ETRANS120_REG_GLOBAL_LPBK_CFG */
    reg_value = etrans_reg_GLOBAL_LPBK_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_GLOBAL_LPBK_CFG_BIT_GBL_ETRANS_LB_EN_MSK) >> ETRANS120_REG_GLOBAL_LPBK_CFG_BIT_GBL_ETRANS_LB_EN_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_GBL_ETRANS_LB_EN_get", value );

    return value;
}
static INLINE void etrans_field_range_GBL_ETRANS_LB_EN_set( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_GBL_ETRANS_LB_EN_set( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_GBL_ETRANS_LB_EN_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_GBL_ETRANS_LB_EN_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_GBL_ETRANS_LB_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000048 bits 11:0) field GBL_ETRANS_LB_EN of register PMC_ETRANS120_REG_GLOBAL_LPBK_CFG */
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
        /* (0x00000048 bits 11:0) field GBL_ETRANS_LB_EN of register PMC_ETRANS120_REG_GLOBAL_LPBK_CFG */
        etrans_reg_GLOBAL_LPBK_CFG_field_set( b_ptr,
                                              h_ptr,
                                              subfield_mask << (ETRANS120_REG_GLOBAL_LPBK_CFG_BIT_GBL_ETRANS_LB_EN_OFF + subfield_offset),
                                              ETRANS120_REG_GLOBAL_LPBK_CFG_BIT_GBL_ETRANS_LB_EN_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_GBL_ETRANS_LB_EN_get( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_GBL_ETRANS_LB_EN_get( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_GBL_ETRANS_LB_EN_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_GBL_ETRANS_LB_EN_get", stop_bit, 11 );
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
    /* (0x00000048 bits 11:0) field GBL_ETRANS_LB_EN of register PMC_ETRANS120_REG_GLOBAL_LPBK_CFG */
    reg_value = etrans_reg_GLOBAL_LPBK_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ETRANS120_REG_GLOBAL_LPBK_CFG_BIT_GBL_ETRANS_LB_EN_MSK)
                  >> ETRANS120_REG_GLOBAL_LPBK_CFG_BIT_GBL_ETRANS_LB_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_GLOBAL_LPBK_CFG_BIT_GBL_ETRANS_LB_EN_MSK, ETRANS120_REG_GLOBAL_LPBK_CFG_BIT_GBL_ETRANS_LB_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_GBL_ETRANS_LB_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_RX_100G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_100G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_100G_LINK_DP_SEL_set", value, 3);
    IOLOG( "%s <= 0x%08x", "etrans_field_RX_100G_LINK_DP_SEL_set", value );

    /* (0x00000090 bits 3:2) field RX_100G_LINK_DP_SEL of register PMC_ETRANS120_REG_RX_100G_LINK_CFG_REG */
    etrans_reg_RX_100G_LINK_CFG_REG_field_set( b_ptr,
                                               h_ptr,
                                               ETRANS120_REG_RX_100G_LINK_CFG_REG_BIT_RX_100G_LINK_DP_SEL_MSK,
                                               ETRANS120_REG_RX_100G_LINK_CFG_REG_BIT_RX_100G_LINK_DP_SEL_OFF,
                                               value);
}

static INLINE UINT32 etrans_field_RX_100G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_100G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000090 bits 3:2) field RX_100G_LINK_DP_SEL of register PMC_ETRANS120_REG_RX_100G_LINK_CFG_REG */
    reg_value = etrans_reg_RX_100G_LINK_CFG_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ETRANS120_REG_RX_100G_LINK_CFG_REG_BIT_RX_100G_LINK_DP_SEL_MSK) >> ETRANS120_REG_RX_100G_LINK_CFG_REG_BIT_RX_100G_LINK_DP_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_100G_LINK_DP_SEL_get", value );

    return value;
}
static INLINE void etrans_field_range_RX_100G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_RX_100G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_100G_LINK_DP_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_100G_LINK_DP_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_100G_LINK_DP_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000090 bits 3:2) field RX_100G_LINK_DP_SEL of register PMC_ETRANS120_REG_RX_100G_LINK_CFG_REG */
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
        /* (0x00000090 bits 3:2) field RX_100G_LINK_DP_SEL of register PMC_ETRANS120_REG_RX_100G_LINK_CFG_REG */
        etrans_reg_RX_100G_LINK_CFG_REG_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (ETRANS120_REG_RX_100G_LINK_CFG_REG_BIT_RX_100G_LINK_DP_SEL_OFF + subfield_offset),
                                                   ETRANS120_REG_RX_100G_LINK_CFG_REG_BIT_RX_100G_LINK_DP_SEL_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_RX_100G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_RX_100G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_100G_LINK_DP_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_100G_LINK_DP_SEL_get", stop_bit, 1 );
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
    /* (0x00000090 bits 3:2) field RX_100G_LINK_DP_SEL of register PMC_ETRANS120_REG_RX_100G_LINK_CFG_REG */
    reg_value = etrans_reg_RX_100G_LINK_CFG_REG_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & ETRANS120_REG_RX_100G_LINK_CFG_REG_BIT_RX_100G_LINK_DP_SEL_MSK)
                  >> ETRANS120_REG_RX_100G_LINK_CFG_REG_BIT_RX_100G_LINK_DP_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_RX_100G_LINK_CFG_REG_BIT_RX_100G_LINK_DP_SEL_MSK, ETRANS120_REG_RX_100G_LINK_CFG_REG_BIT_RX_100G_LINK_DP_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_100G_LINK_DP_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_RX_100G_LINK_EN_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_100G_LINK_EN_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_100G_LINK_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_RX_100G_LINK_EN_set", value );

    /* (0x00000090 bits 1) field RX_100G_LINK_EN of register PMC_ETRANS120_REG_RX_100G_LINK_CFG_REG */
    etrans_reg_RX_100G_LINK_CFG_REG_field_set( b_ptr,
                                               h_ptr,
                                               ETRANS120_REG_RX_100G_LINK_CFG_REG_BIT_RX_100G_LINK_EN_MSK,
                                               ETRANS120_REG_RX_100G_LINK_CFG_REG_BIT_RX_100G_LINK_EN_OFF,
                                               value);
}

static INLINE UINT32 etrans_field_RX_100G_LINK_EN_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_100G_LINK_EN_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000090 bits 1) field RX_100G_LINK_EN of register PMC_ETRANS120_REG_RX_100G_LINK_CFG_REG */
    reg_value = etrans_reg_RX_100G_LINK_CFG_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ETRANS120_REG_RX_100G_LINK_CFG_REG_BIT_RX_100G_LINK_EN_MSK) >> ETRANS120_REG_RX_100G_LINK_CFG_REG_BIT_RX_100G_LINK_EN_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_100G_LINK_EN_get", value );

    return value;
}
static INLINE void etrans_field_RX_100G_LINK_RST_set( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_100G_LINK_RST_set( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_100G_LINK_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_RX_100G_LINK_RST_set", value );

    /* (0x00000090 bits 0) field RX_100G_LINK_RST of register PMC_ETRANS120_REG_RX_100G_LINK_CFG_REG */
    etrans_reg_RX_100G_LINK_CFG_REG_field_set( b_ptr,
                                               h_ptr,
                                               ETRANS120_REG_RX_100G_LINK_CFG_REG_BIT_RX_100G_LINK_RST_MSK,
                                               ETRANS120_REG_RX_100G_LINK_CFG_REG_BIT_RX_100G_LINK_RST_OFF,
                                               value);
}

static INLINE UINT32 etrans_field_RX_100G_LINK_RST_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_100G_LINK_RST_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000090 bits 0) field RX_100G_LINK_RST of register PMC_ETRANS120_REG_RX_100G_LINK_CFG_REG */
    reg_value = etrans_reg_RX_100G_LINK_CFG_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ETRANS120_REG_RX_100G_LINK_CFG_REG_BIT_RX_100G_LINK_RST_MSK) >> ETRANS120_REG_RX_100G_LINK_CFG_REG_BIT_RX_100G_LINK_RST_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_100G_LINK_RST_get", value );

    return value;
}
static INLINE void etrans_field_TX_100G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_100G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_100G_LINK_DP_SEL_set", value, 3);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_100G_LINK_DP_SEL_set", value );

    /* (0x00000100 bits 3:2) field TX_100G_LINK_DP_SEL of register PMC_ETRANS120_REG_TX_100G_LINK_CFG */
    etrans_reg_TX_100G_LINK_CFG_field_set( b_ptr,
                                           h_ptr,
                                           ETRANS120_REG_TX_100G_LINK_CFG_BIT_TX_100G_LINK_DP_SEL_MSK,
                                           ETRANS120_REG_TX_100G_LINK_CFG_BIT_TX_100G_LINK_DP_SEL_OFF,
                                           value);
}

static INLINE UINT32 etrans_field_TX_100G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_100G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 3:2) field TX_100G_LINK_DP_SEL of register PMC_ETRANS120_REG_TX_100G_LINK_CFG */
    reg_value = etrans_reg_TX_100G_LINK_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_100G_LINK_CFG_BIT_TX_100G_LINK_DP_SEL_MSK) >> ETRANS120_REG_TX_100G_LINK_CFG_BIT_TX_100G_LINK_DP_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_100G_LINK_DP_SEL_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_100G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_100G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_100G_LINK_DP_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_100G_LINK_DP_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_100G_LINK_DP_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000100 bits 3:2) field TX_100G_LINK_DP_SEL of register PMC_ETRANS120_REG_TX_100G_LINK_CFG */
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
        /* (0x00000100 bits 3:2) field TX_100G_LINK_DP_SEL of register PMC_ETRANS120_REG_TX_100G_LINK_CFG */
        etrans_reg_TX_100G_LINK_CFG_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (ETRANS120_REG_TX_100G_LINK_CFG_BIT_TX_100G_LINK_DP_SEL_OFF + subfield_offset),
                                               ETRANS120_REG_TX_100G_LINK_CFG_BIT_TX_100G_LINK_DP_SEL_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_100G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_100G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_100G_LINK_DP_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_100G_LINK_DP_SEL_get", stop_bit, 1 );
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
    /* (0x00000100 bits 3:2) field TX_100G_LINK_DP_SEL of register PMC_ETRANS120_REG_TX_100G_LINK_CFG */
    reg_value = etrans_reg_TX_100G_LINK_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_100G_LINK_CFG_BIT_TX_100G_LINK_DP_SEL_MSK)
                  >> ETRANS120_REG_TX_100G_LINK_CFG_BIT_TX_100G_LINK_DP_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_100G_LINK_CFG_BIT_TX_100G_LINK_DP_SEL_MSK, ETRANS120_REG_TX_100G_LINK_CFG_BIT_TX_100G_LINK_DP_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_100G_LINK_DP_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_100G_LINK_EN_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_100G_LINK_EN_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_100G_LINK_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_100G_LINK_EN_set", value );

    /* (0x00000100 bits 1) field TX_100G_LINK_EN of register PMC_ETRANS120_REG_TX_100G_LINK_CFG */
    etrans_reg_TX_100G_LINK_CFG_field_set( b_ptr,
                                           h_ptr,
                                           ETRANS120_REG_TX_100G_LINK_CFG_BIT_TX_100G_LINK_EN_MSK,
                                           ETRANS120_REG_TX_100G_LINK_CFG_BIT_TX_100G_LINK_EN_OFF,
                                           value);
}

static INLINE UINT32 etrans_field_TX_100G_LINK_EN_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_100G_LINK_EN_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 1) field TX_100G_LINK_EN of register PMC_ETRANS120_REG_TX_100G_LINK_CFG */
    reg_value = etrans_reg_TX_100G_LINK_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_100G_LINK_CFG_BIT_TX_100G_LINK_EN_MSK) >> ETRANS120_REG_TX_100G_LINK_CFG_BIT_TX_100G_LINK_EN_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_100G_LINK_EN_get", value );

    return value;
}
static INLINE void etrans_field_TX_100G_LINK_RST_set( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_100G_LINK_RST_set( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_100G_LINK_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_100G_LINK_RST_set", value );

    /* (0x00000100 bits 0) field TX_100G_LINK_RST of register PMC_ETRANS120_REG_TX_100G_LINK_CFG */
    etrans_reg_TX_100G_LINK_CFG_field_set( b_ptr,
                                           h_ptr,
                                           ETRANS120_REG_TX_100G_LINK_CFG_BIT_TX_100G_LINK_RST_MSK,
                                           ETRANS120_REG_TX_100G_LINK_CFG_BIT_TX_100G_LINK_RST_OFF,
                                           value);
}

static INLINE UINT32 etrans_field_TX_100G_LINK_RST_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_100G_LINK_RST_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 0) field TX_100G_LINK_RST of register PMC_ETRANS120_REG_TX_100G_LINK_CFG */
    reg_value = etrans_reg_TX_100G_LINK_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_100G_LINK_CFG_BIT_TX_100G_LINK_RST_MSK) >> ETRANS120_REG_TX_100G_LINK_CFG_BIT_TX_100G_LINK_RST_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_100G_LINK_RST_get", value );

    return value;
}
static INLINE void etrans_field_TX_CHID_LINK3_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_CHID_LINK3_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_CHID_LINK3_set", value, 127);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_CHID_LINK3_set", value );

    /* (0x00000134 bits 30:24) field TX_CHID_LINK3 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
    etrans_reg_TX_CHID_LOOKUP_CFG_REG0_field_set( b_ptr,
                                                  h_ptr,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK3_MSK,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK3_OFF,
                                                  value);
}

static INLINE UINT32 etrans_field_TX_CHID_LINK3_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_CHID_LINK3_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000134 bits 30:24) field TX_CHID_LINK3 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG0_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK3_MSK) >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK3_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_CHID_LINK3_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_CHID_LINK3_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_CHID_LINK3_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK3_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK3_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000134 bits 30:24) field TX_CHID_LINK3 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
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
        /* (0x00000134 bits 30:24) field TX_CHID_LINK3 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
        etrans_reg_TX_CHID_LOOKUP_CFG_REG0_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK3_OFF + subfield_offset),
                                                      ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK3_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_CHID_LINK3_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_CHID_LINK3_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK3_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK3_get", stop_bit, 6 );
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
    /* (0x00000134 bits 30:24) field TX_CHID_LINK3 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG0_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK3_MSK)
                  >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK3_MSK, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_CHID_LINK2_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_CHID_LINK2_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_CHID_LINK2_set", value, 127);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_CHID_LINK2_set", value );

    /* (0x00000134 bits 22:16) field TX_CHID_LINK2 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
    etrans_reg_TX_CHID_LOOKUP_CFG_REG0_field_set( b_ptr,
                                                  h_ptr,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK2_MSK,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK2_OFF,
                                                  value);
}

static INLINE UINT32 etrans_field_TX_CHID_LINK2_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_CHID_LINK2_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000134 bits 22:16) field TX_CHID_LINK2 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG0_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK2_MSK) >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK2_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_CHID_LINK2_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_CHID_LINK2_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_CHID_LINK2_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK2_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK2_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK2_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000134 bits 22:16) field TX_CHID_LINK2 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
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
        /* (0x00000134 bits 22:16) field TX_CHID_LINK2 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
        etrans_reg_TX_CHID_LOOKUP_CFG_REG0_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK2_OFF + subfield_offset),
                                                      ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK2_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_CHID_LINK2_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_CHID_LINK2_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK2_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK2_get", stop_bit, 6 );
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
    /* (0x00000134 bits 22:16) field TX_CHID_LINK2 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG0_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK2_MSK)
                  >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK2_MSK, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_CHID_LINK1_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_CHID_LINK1_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_CHID_LINK1_set", value, 127);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_CHID_LINK1_set", value );

    /* (0x00000134 bits 14:8) field TX_CHID_LINK1 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
    etrans_reg_TX_CHID_LOOKUP_CFG_REG0_field_set( b_ptr,
                                                  h_ptr,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK1_MSK,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK1_OFF,
                                                  value);
}

static INLINE UINT32 etrans_field_TX_CHID_LINK1_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_CHID_LINK1_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000134 bits 14:8) field TX_CHID_LINK1 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG0_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK1_MSK) >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK1_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_CHID_LINK1_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_CHID_LINK1_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_CHID_LINK1_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK1_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK1_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK1_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000134 bits 14:8) field TX_CHID_LINK1 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
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
        /* (0x00000134 bits 14:8) field TX_CHID_LINK1 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
        etrans_reg_TX_CHID_LOOKUP_CFG_REG0_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK1_OFF + subfield_offset),
                                                      ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK1_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_CHID_LINK1_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_CHID_LINK1_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK1_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK1_get", stop_bit, 6 );
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
    /* (0x00000134 bits 14:8) field TX_CHID_LINK1 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG0_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK1_MSK)
                  >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK1_MSK, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_CHID_LINK0_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_CHID_LINK0_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_CHID_LINK0_set", value, 127);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_CHID_LINK0_set", value );

    /* (0x00000134 bits 6:0) field TX_CHID_LINK0 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
    etrans_reg_TX_CHID_LOOKUP_CFG_REG0_field_set( b_ptr,
                                                  h_ptr,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK0_MSK,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK0_OFF,
                                                  value);
}

static INLINE UINT32 etrans_field_TX_CHID_LINK0_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_CHID_LINK0_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000134 bits 6:0) field TX_CHID_LINK0 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG0_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK0_MSK) >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK0_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_CHID_LINK0_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_CHID_LINK0_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_CHID_LINK0_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK0_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK0_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK0_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000134 bits 6:0) field TX_CHID_LINK0 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
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
        /* (0x00000134 bits 6:0) field TX_CHID_LINK0 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
        etrans_reg_TX_CHID_LOOKUP_CFG_REG0_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK0_OFF + subfield_offset),
                                                      ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK0_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_CHID_LINK0_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_CHID_LINK0_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK0_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK0_get", stop_bit, 6 );
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
    /* (0x00000134 bits 6:0) field TX_CHID_LINK0 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG0_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK0_MSK)
                  >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK0_MSK, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG0_BIT_TX_CHID_LINK0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_CHID_LINK7_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_CHID_LINK7_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_CHID_LINK7_set", value, 127);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_CHID_LINK7_set", value );

    /* (0x00000138 bits 30:24) field TX_CHID_LINK7 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
    etrans_reg_TX_CHID_LOOKUP_CFG_REG1_field_set( b_ptr,
                                                  h_ptr,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK7_MSK,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK7_OFF,
                                                  value);
}

static INLINE UINT32 etrans_field_TX_CHID_LINK7_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_CHID_LINK7_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000138 bits 30:24) field TX_CHID_LINK7 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG1_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK7_MSK) >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK7_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_CHID_LINK7_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_CHID_LINK7_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_CHID_LINK7_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK7_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK7_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK7_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000138 bits 30:24) field TX_CHID_LINK7 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
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
        /* (0x00000138 bits 30:24) field TX_CHID_LINK7 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
        etrans_reg_TX_CHID_LOOKUP_CFG_REG1_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK7_OFF + subfield_offset),
                                                      ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK7_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_CHID_LINK7_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_CHID_LINK7_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK7_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK7_get", stop_bit, 6 );
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
    /* (0x00000138 bits 30:24) field TX_CHID_LINK7 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG1_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK7_MSK)
                  >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK7_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK7_MSK, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK7_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK7_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_CHID_LINK6_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_CHID_LINK6_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_CHID_LINK6_set", value, 127);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_CHID_LINK6_set", value );

    /* (0x00000138 bits 22:16) field TX_CHID_LINK6 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
    etrans_reg_TX_CHID_LOOKUP_CFG_REG1_field_set( b_ptr,
                                                  h_ptr,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK6_MSK,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK6_OFF,
                                                  value);
}

static INLINE UINT32 etrans_field_TX_CHID_LINK6_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_CHID_LINK6_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000138 bits 22:16) field TX_CHID_LINK6 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG1_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK6_MSK) >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK6_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_CHID_LINK6_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_CHID_LINK6_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_CHID_LINK6_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK6_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK6_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK6_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000138 bits 22:16) field TX_CHID_LINK6 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
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
        /* (0x00000138 bits 22:16) field TX_CHID_LINK6 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
        etrans_reg_TX_CHID_LOOKUP_CFG_REG1_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK6_OFF + subfield_offset),
                                                      ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK6_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_CHID_LINK6_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_CHID_LINK6_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK6_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK6_get", stop_bit, 6 );
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
    /* (0x00000138 bits 22:16) field TX_CHID_LINK6 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG1_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK6_MSK)
                  >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK6_MSK, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK6_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_CHID_LINK5_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_CHID_LINK5_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_CHID_LINK5_set", value, 127);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_CHID_LINK5_set", value );

    /* (0x00000138 bits 14:8) field TX_CHID_LINK5 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
    etrans_reg_TX_CHID_LOOKUP_CFG_REG1_field_set( b_ptr,
                                                  h_ptr,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK5_MSK,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK5_OFF,
                                                  value);
}

static INLINE UINT32 etrans_field_TX_CHID_LINK5_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_CHID_LINK5_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000138 bits 14:8) field TX_CHID_LINK5 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG1_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK5_MSK) >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK5_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_CHID_LINK5_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_CHID_LINK5_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_CHID_LINK5_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK5_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK5_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK5_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000138 bits 14:8) field TX_CHID_LINK5 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
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
        /* (0x00000138 bits 14:8) field TX_CHID_LINK5 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
        etrans_reg_TX_CHID_LOOKUP_CFG_REG1_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK5_OFF + subfield_offset),
                                                      ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK5_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_CHID_LINK5_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_CHID_LINK5_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK5_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK5_get", stop_bit, 6 );
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
    /* (0x00000138 bits 14:8) field TX_CHID_LINK5 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG1_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK5_MSK)
                  >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK5_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK5_MSK, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK5_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK5_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_CHID_LINK4_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_CHID_LINK4_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_CHID_LINK4_set", value, 127);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_CHID_LINK4_set", value );

    /* (0x00000138 bits 6:0) field TX_CHID_LINK4 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
    etrans_reg_TX_CHID_LOOKUP_CFG_REG1_field_set( b_ptr,
                                                  h_ptr,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK4_MSK,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK4_OFF,
                                                  value);
}

static INLINE UINT32 etrans_field_TX_CHID_LINK4_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_CHID_LINK4_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000138 bits 6:0) field TX_CHID_LINK4 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG1_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK4_MSK) >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK4_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_CHID_LINK4_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_CHID_LINK4_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_CHID_LINK4_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK4_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK4_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK4_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000138 bits 6:0) field TX_CHID_LINK4 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
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
        /* (0x00000138 bits 6:0) field TX_CHID_LINK4 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
        etrans_reg_TX_CHID_LOOKUP_CFG_REG1_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK4_OFF + subfield_offset),
                                                      ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK4_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_CHID_LINK4_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_CHID_LINK4_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK4_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK4_get", stop_bit, 6 );
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
    /* (0x00000138 bits 6:0) field TX_CHID_LINK4 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG1_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK4_MSK)
                  >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK4_MSK, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG1_BIT_TX_CHID_LINK4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK4_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_CHID_LINK11_set( etrans_buffer_t *b_ptr,
                                                    etrans_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_CHID_LINK11_set( etrans_buffer_t *b_ptr,
                                                    etrans_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_CHID_LINK11_set", value, 127);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_CHID_LINK11_set", value );

    /* (0x0000013c bits 30:24) field TX_CHID_LINK11 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
    etrans_reg_TX_CHID_LOOKUP_CFG_REG2_field_set( b_ptr,
                                                  h_ptr,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK11_MSK,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK11_OFF,
                                                  value);
}

static INLINE UINT32 etrans_field_TX_CHID_LINK11_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_CHID_LINK11_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000013c bits 30:24) field TX_CHID_LINK11 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG2_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK11_MSK) >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK11_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_CHID_LINK11_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_CHID_LINK11_set( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_CHID_LINK11_set( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK11_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK11_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK11_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000013c bits 30:24) field TX_CHID_LINK11 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
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
        /* (0x0000013c bits 30:24) field TX_CHID_LINK11 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
        etrans_reg_TX_CHID_LOOKUP_CFG_REG2_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK11_OFF + subfield_offset),
                                                      ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK11_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_CHID_LINK11_get( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_CHID_LINK11_get( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK11_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK11_get", stop_bit, 6 );
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
    /* (0x0000013c bits 30:24) field TX_CHID_LINK11 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG2_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK11_MSK)
                  >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK11_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK11_MSK, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK11_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK11_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_CHID_LINK10_set( etrans_buffer_t *b_ptr,
                                                    etrans_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_CHID_LINK10_set( etrans_buffer_t *b_ptr,
                                                    etrans_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_CHID_LINK10_set", value, 127);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_CHID_LINK10_set", value );

    /* (0x0000013c bits 22:16) field TX_CHID_LINK10 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
    etrans_reg_TX_CHID_LOOKUP_CFG_REG2_field_set( b_ptr,
                                                  h_ptr,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK10_MSK,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK10_OFF,
                                                  value);
}

static INLINE UINT32 etrans_field_TX_CHID_LINK10_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_CHID_LINK10_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000013c bits 22:16) field TX_CHID_LINK10 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG2_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK10_MSK) >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK10_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_CHID_LINK10_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_CHID_LINK10_set( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_CHID_LINK10_set( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK10_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK10_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK10_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000013c bits 22:16) field TX_CHID_LINK10 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
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
        /* (0x0000013c bits 22:16) field TX_CHID_LINK10 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
        etrans_reg_TX_CHID_LOOKUP_CFG_REG2_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK10_OFF + subfield_offset),
                                                      ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK10_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_CHID_LINK10_get( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_CHID_LINK10_get( etrans_buffer_t *b_ptr,
                                                            etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK10_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK10_get", stop_bit, 6 );
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
    /* (0x0000013c bits 22:16) field TX_CHID_LINK10 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG2_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK10_MSK)
                  >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK10_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK10_MSK, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK10_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK10_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_CHID_LINK9_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_CHID_LINK9_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_CHID_LINK9_set", value, 127);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_CHID_LINK9_set", value );

    /* (0x0000013c bits 14:8) field TX_CHID_LINK9 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
    etrans_reg_TX_CHID_LOOKUP_CFG_REG2_field_set( b_ptr,
                                                  h_ptr,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK9_MSK,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK9_OFF,
                                                  value);
}

static INLINE UINT32 etrans_field_TX_CHID_LINK9_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_CHID_LINK9_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000013c bits 14:8) field TX_CHID_LINK9 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG2_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK9_MSK) >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK9_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_CHID_LINK9_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_CHID_LINK9_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_CHID_LINK9_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK9_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK9_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK9_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000013c bits 14:8) field TX_CHID_LINK9 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
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
        /* (0x0000013c bits 14:8) field TX_CHID_LINK9 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
        etrans_reg_TX_CHID_LOOKUP_CFG_REG2_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK9_OFF + subfield_offset),
                                                      ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK9_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_CHID_LINK9_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_CHID_LINK9_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK9_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK9_get", stop_bit, 6 );
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
    /* (0x0000013c bits 14:8) field TX_CHID_LINK9 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG2_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK9_MSK)
                  >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK9_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK9_MSK, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK9_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK9_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_CHID_LINK8_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_CHID_LINK8_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_CHID_LINK8_set", value, 127);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_CHID_LINK8_set", value );

    /* (0x0000013c bits 6:0) field TX_CHID_LINK8 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
    etrans_reg_TX_CHID_LOOKUP_CFG_REG2_field_set( b_ptr,
                                                  h_ptr,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK8_MSK,
                                                  ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK8_OFF,
                                                  value);
}

static INLINE UINT32 etrans_field_TX_CHID_LINK8_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_CHID_LINK8_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000013c bits 6:0) field TX_CHID_LINK8 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG2_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK8_MSK) >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK8_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_CHID_LINK8_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_CHID_LINK8_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_CHID_LINK8_set( etrans_buffer_t *b_ptr,
                                                         etrans_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK8_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK8_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK8_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000013c bits 6:0) field TX_CHID_LINK8 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
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
        /* (0x0000013c bits 6:0) field TX_CHID_LINK8 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
        etrans_reg_TX_CHID_LOOKUP_CFG_REG2_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK8_OFF + subfield_offset),
                                                      ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK8_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_CHID_LINK8_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_CHID_LINK8_get( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_CHID_LINK8_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_CHID_LINK8_get", stop_bit, 6 );
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
    /* (0x0000013c bits 6:0) field TX_CHID_LINK8 of register PMC_ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2 */
    reg_value = etrans_reg_TX_CHID_LOOKUP_CFG_REG2_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK8_MSK)
                  >> ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK8_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK8_MSK, ETRANS120_REG_TX_CHID_LOOKUP_CFG_REG2_BIT_TX_CHID_LINK8_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_CHID_LINK8_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_SLOT_7_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_SLOT_7_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_SLOT_7_set", value, 15);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_SLOT_7_set", value );

    /* (0x00000140 bits 31:28) field TX_SLOT_7 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    etrans_reg_TX_OP_SCH_CAL_CFG0_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_7_MSK,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_7_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_SLOT_7_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_SLOT_7_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000140 bits 31:28) field TX_SLOT_7 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG0_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_7_MSK) >> ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_7_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_SLOT_7_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_SLOT_7_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_SLOT_7_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_7_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_7_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_7_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000140 bits 31:28) field TX_SLOT_7 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
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
        /* (0x00000140 bits 31:28) field TX_SLOT_7 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
        etrans_reg_TX_OP_SCH_CAL_CFG0_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_7_OFF + subfield_offset),
                                                 ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_7_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_SLOT_7_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_SLOT_7_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_7_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_7_get", stop_bit, 3 );
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
    /* (0x00000140 bits 31:28) field TX_SLOT_7 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG0_read(  b_ptr, h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_7_MSK)
                  >> ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_7_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_7_MSK, ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_7_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_7_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_SLOT_6_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_SLOT_6_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_SLOT_6_set", value, 15);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_SLOT_6_set", value );

    /* (0x00000140 bits 27:24) field TX_SLOT_6 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    etrans_reg_TX_OP_SCH_CAL_CFG0_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_6_MSK,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_6_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_SLOT_6_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_SLOT_6_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000140 bits 27:24) field TX_SLOT_6 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG0_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_6_MSK) >> ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_6_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_SLOT_6_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_SLOT_6_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_SLOT_6_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_6_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_6_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_6_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000140 bits 27:24) field TX_SLOT_6 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
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
        /* (0x00000140 bits 27:24) field TX_SLOT_6 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
        etrans_reg_TX_OP_SCH_CAL_CFG0_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_6_OFF + subfield_offset),
                                                 ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_6_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_SLOT_6_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_SLOT_6_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_6_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_6_get", stop_bit, 3 );
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
    /* (0x00000140 bits 27:24) field TX_SLOT_6 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG0_read(  b_ptr, h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_6_MSK)
                  >> ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_6_MSK, ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_6_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_SLOT_5_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_SLOT_5_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_SLOT_5_set", value, 15);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_SLOT_5_set", value );

    /* (0x00000140 bits 23:20) field TX_SLOT_5 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    etrans_reg_TX_OP_SCH_CAL_CFG0_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_5_MSK,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_5_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_SLOT_5_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_SLOT_5_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000140 bits 23:20) field TX_SLOT_5 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG0_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_5_MSK) >> ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_5_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_SLOT_5_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_SLOT_5_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_SLOT_5_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_5_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_5_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_5_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000140 bits 23:20) field TX_SLOT_5 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
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
        /* (0x00000140 bits 23:20) field TX_SLOT_5 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
        etrans_reg_TX_OP_SCH_CAL_CFG0_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_5_OFF + subfield_offset),
                                                 ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_5_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_SLOT_5_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_SLOT_5_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_5_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_5_get", stop_bit, 3 );
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
    /* (0x00000140 bits 23:20) field TX_SLOT_5 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG0_read(  b_ptr, h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_5_MSK)
                  >> ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_5_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_5_MSK, ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_5_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_5_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_SLOT_4_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_SLOT_4_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_SLOT_4_set", value, 15);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_SLOT_4_set", value );

    /* (0x00000140 bits 19:16) field TX_SLOT_4 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    etrans_reg_TX_OP_SCH_CAL_CFG0_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_4_MSK,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_4_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_SLOT_4_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_SLOT_4_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000140 bits 19:16) field TX_SLOT_4 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG0_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_4_MSK) >> ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_4_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_SLOT_4_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_SLOT_4_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_SLOT_4_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_4_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_4_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_4_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000140 bits 19:16) field TX_SLOT_4 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
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
        /* (0x00000140 bits 19:16) field TX_SLOT_4 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
        etrans_reg_TX_OP_SCH_CAL_CFG0_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_4_OFF + subfield_offset),
                                                 ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_4_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_SLOT_4_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_SLOT_4_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_4_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_4_get", stop_bit, 3 );
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
    /* (0x00000140 bits 19:16) field TX_SLOT_4 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG0_read(  b_ptr, h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_4_MSK)
                  >> ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_4_MSK, ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_4_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_SLOT_3_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_SLOT_3_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_SLOT_3_set", value, 15);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_SLOT_3_set", value );

    /* (0x00000140 bits 15:12) field TX_SLOT_3 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    etrans_reg_TX_OP_SCH_CAL_CFG0_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_3_MSK,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_3_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_SLOT_3_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_SLOT_3_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000140 bits 15:12) field TX_SLOT_3 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG0_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_3_MSK) >> ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_3_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_SLOT_3_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_SLOT_3_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_SLOT_3_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_3_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_3_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000140 bits 15:12) field TX_SLOT_3 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
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
        /* (0x00000140 bits 15:12) field TX_SLOT_3 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
        etrans_reg_TX_OP_SCH_CAL_CFG0_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_3_OFF + subfield_offset),
                                                 ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_3_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_SLOT_3_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_SLOT_3_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_3_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_3_get", stop_bit, 3 );
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
    /* (0x00000140 bits 15:12) field TX_SLOT_3 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG0_read(  b_ptr, h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_3_MSK)
                  >> ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_3_MSK, ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_SLOT_2_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_SLOT_2_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_SLOT_2_set", value, 15);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_SLOT_2_set", value );

    /* (0x00000140 bits 11:8) field TX_SLOT_2 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    etrans_reg_TX_OP_SCH_CAL_CFG0_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_2_MSK,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_2_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_SLOT_2_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_SLOT_2_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000140 bits 11:8) field TX_SLOT_2 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG0_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_2_MSK) >> ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_2_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_SLOT_2_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_SLOT_2_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_SLOT_2_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_2_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_2_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_2_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000140 bits 11:8) field TX_SLOT_2 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
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
        /* (0x00000140 bits 11:8) field TX_SLOT_2 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
        etrans_reg_TX_OP_SCH_CAL_CFG0_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_2_OFF + subfield_offset),
                                                 ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_2_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_SLOT_2_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_SLOT_2_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_2_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_2_get", stop_bit, 3 );
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
    /* (0x00000140 bits 11:8) field TX_SLOT_2 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG0_read(  b_ptr, h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_2_MSK)
                  >> ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_2_MSK, ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_SLOT_1_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_SLOT_1_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_SLOT_1_set", value, 15);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_SLOT_1_set", value );

    /* (0x00000140 bits 7:4) field TX_SLOT_1 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    etrans_reg_TX_OP_SCH_CAL_CFG0_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_1_MSK,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_1_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_SLOT_1_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_SLOT_1_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000140 bits 7:4) field TX_SLOT_1 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG0_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_1_MSK) >> ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_1_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_SLOT_1_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_SLOT_1_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_SLOT_1_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_1_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_1_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_1_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000140 bits 7:4) field TX_SLOT_1 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
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
        /* (0x00000140 bits 7:4) field TX_SLOT_1 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
        etrans_reg_TX_OP_SCH_CAL_CFG0_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_1_OFF + subfield_offset),
                                                 ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_1_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_SLOT_1_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_SLOT_1_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_1_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_1_get", stop_bit, 3 );
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
    /* (0x00000140 bits 7:4) field TX_SLOT_1 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG0_read(  b_ptr, h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_1_MSK)
                  >> ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_1_MSK, ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_SLOT_0_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_SLOT_0_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_SLOT_0_set", value, 15);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_SLOT_0_set", value );

    /* (0x00000140 bits 3:0) field TX_SLOT_0 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    etrans_reg_TX_OP_SCH_CAL_CFG0_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_0_MSK,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_0_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_SLOT_0_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_SLOT_0_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000140 bits 3:0) field TX_SLOT_0 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG0_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_0_MSK) >> ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_0_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_SLOT_0_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_SLOT_0_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_SLOT_0_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_0_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_0_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_0_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000140 bits 3:0) field TX_SLOT_0 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
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
        /* (0x00000140 bits 3:0) field TX_SLOT_0 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
        etrans_reg_TX_OP_SCH_CAL_CFG0_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_0_OFF + subfield_offset),
                                                 ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_0_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_SLOT_0_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_SLOT_0_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_0_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_0_get", stop_bit, 3 );
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
    /* (0x00000140 bits 3:0) field TX_SLOT_0 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG0 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG0_read(  b_ptr, h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_0_MSK)
                  >> ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_0_MSK, ETRANS120_REG_TX_OP_SCH_CAL_CFG0_BIT_TX_SLOT_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_LINK_SCH_OFFSET_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK_SCH_OFFSET_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK_SCH_OFFSET_set", value, 15);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK_SCH_OFFSET_set", value );

    /* (0x00000144 bits 27:24) field TX_LINK_SCH_OFFSET of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    etrans_reg_TX_OP_SCH_CAL_CFG1_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_OFFSET_MSK,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_OFFSET_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_LINK_SCH_OFFSET_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK_SCH_OFFSET_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000144 bits 27:24) field TX_LINK_SCH_OFFSET of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG1_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_OFFSET_MSK) >> ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_OFFSET_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK_SCH_OFFSET_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_LINK_SCH_OFFSET_set( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_LINK_SCH_OFFSET_set( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_LINK_SCH_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_LINK_SCH_OFFSET_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_LINK_SCH_OFFSET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000144 bits 27:24) field TX_LINK_SCH_OFFSET of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
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
        /* (0x00000144 bits 27:24) field TX_LINK_SCH_OFFSET of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
        etrans_reg_TX_OP_SCH_CAL_CFG1_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_OFFSET_OFF + subfield_offset),
                                                 ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_OFFSET_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_LINK_SCH_OFFSET_get( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_LINK_SCH_OFFSET_get( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_LINK_SCH_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_LINK_SCH_OFFSET_get", stop_bit, 3 );
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
    /* (0x00000144 bits 27:24) field TX_LINK_SCH_OFFSET of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG1_read(  b_ptr, h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_OFFSET_MSK)
                  >> ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_OFFSET_MSK, ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_LINK_SCH_OFFSET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_LINK_SCH_LAST_SLOT_set( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK_SCH_LAST_SLOT_set( etrans_buffer_t *b_ptr,
                                                           etrans_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK_SCH_LAST_SLOT_set", value, 15);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK_SCH_LAST_SLOT_set", value );

    /* (0x00000144 bits 23:20) field TX_LINK_SCH_LAST_SLOT of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    etrans_reg_TX_OP_SCH_CAL_CFG1_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_LAST_SLOT_MSK,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_LAST_SLOT_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_LINK_SCH_LAST_SLOT_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK_SCH_LAST_SLOT_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000144 bits 23:20) field TX_LINK_SCH_LAST_SLOT of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG1_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_LAST_SLOT_MSK) >> ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_LAST_SLOT_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK_SCH_LAST_SLOT_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_LINK_SCH_LAST_SLOT_set( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_LINK_SCH_LAST_SLOT_set( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_LINK_SCH_LAST_SLOT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_LINK_SCH_LAST_SLOT_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_LINK_SCH_LAST_SLOT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000144 bits 23:20) field TX_LINK_SCH_LAST_SLOT of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
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
        /* (0x00000144 bits 23:20) field TX_LINK_SCH_LAST_SLOT of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
        etrans_reg_TX_OP_SCH_CAL_CFG1_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_LAST_SLOT_OFF + subfield_offset),
                                                 ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_LAST_SLOT_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_LINK_SCH_LAST_SLOT_get( etrans_buffer_t *b_ptr,
                                                                   etrans_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_LINK_SCH_LAST_SLOT_get( etrans_buffer_t *b_ptr,
                                                                   etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_LINK_SCH_LAST_SLOT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_LINK_SCH_LAST_SLOT_get", stop_bit, 3 );
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
    /* (0x00000144 bits 23:20) field TX_LINK_SCH_LAST_SLOT of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG1_read(  b_ptr, h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_LAST_SLOT_MSK)
                  >> ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_LAST_SLOT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_LAST_SLOT_MSK, ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_LAST_SLOT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_LINK_SCH_LAST_SLOT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_SLOT_11_set( etrans_buffer_t *b_ptr,
                                                etrans_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_SLOT_11_set( etrans_buffer_t *b_ptr,
                                                etrans_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_SLOT_11_set", value, 15);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_SLOT_11_set", value );

    /* (0x00000144 bits 15:12) field TX_SLOT_11 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    etrans_reg_TX_OP_SCH_CAL_CFG1_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_11_MSK,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_11_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_SLOT_11_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_SLOT_11_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000144 bits 15:12) field TX_SLOT_11 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG1_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_11_MSK) >> ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_11_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_SLOT_11_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_SLOT_11_set( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_SLOT_11_set( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_11_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_11_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_11_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000144 bits 15:12) field TX_SLOT_11 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
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
        /* (0x00000144 bits 15:12) field TX_SLOT_11 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
        etrans_reg_TX_OP_SCH_CAL_CFG1_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_11_OFF + subfield_offset),
                                                 ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_11_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_SLOT_11_get( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_SLOT_11_get( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_11_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_11_get", stop_bit, 3 );
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
    /* (0x00000144 bits 15:12) field TX_SLOT_11 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG1_read(  b_ptr, h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_11_MSK)
                  >> ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_11_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_11_MSK, ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_11_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_11_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_SLOT_10_set( etrans_buffer_t *b_ptr,
                                                etrans_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_SLOT_10_set( etrans_buffer_t *b_ptr,
                                                etrans_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_SLOT_10_set", value, 15);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_SLOT_10_set", value );

    /* (0x00000144 bits 11:8) field TX_SLOT_10 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    etrans_reg_TX_OP_SCH_CAL_CFG1_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_10_MSK,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_10_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_SLOT_10_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_SLOT_10_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000144 bits 11:8) field TX_SLOT_10 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG1_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_10_MSK) >> ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_10_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_SLOT_10_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_SLOT_10_set( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_SLOT_10_set( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_10_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_10_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_10_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000144 bits 11:8) field TX_SLOT_10 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
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
        /* (0x00000144 bits 11:8) field TX_SLOT_10 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
        etrans_reg_TX_OP_SCH_CAL_CFG1_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_10_OFF + subfield_offset),
                                                 ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_10_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_SLOT_10_get( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_SLOT_10_get( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_10_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_10_get", stop_bit, 3 );
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
    /* (0x00000144 bits 11:8) field TX_SLOT_10 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG1_read(  b_ptr, h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_10_MSK)
                  >> ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_10_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_10_MSK, ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_10_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_10_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_SLOT_9_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_SLOT_9_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_SLOT_9_set", value, 15);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_SLOT_9_set", value );

    /* (0x00000144 bits 7:4) field TX_SLOT_9 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    etrans_reg_TX_OP_SCH_CAL_CFG1_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_9_MSK,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_9_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_SLOT_9_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_SLOT_9_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000144 bits 7:4) field TX_SLOT_9 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG1_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_9_MSK) >> ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_9_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_SLOT_9_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_SLOT_9_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_SLOT_9_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_9_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_9_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_9_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000144 bits 7:4) field TX_SLOT_9 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
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
        /* (0x00000144 bits 7:4) field TX_SLOT_9 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
        etrans_reg_TX_OP_SCH_CAL_CFG1_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_9_OFF + subfield_offset),
                                                 ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_9_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_SLOT_9_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_SLOT_9_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_9_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_9_get", stop_bit, 3 );
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
    /* (0x00000144 bits 7:4) field TX_SLOT_9 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG1_read(  b_ptr, h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_9_MSK)
                  >> ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_9_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_9_MSK, ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_9_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_9_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_SLOT_8_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_SLOT_8_set( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_SLOT_8_set", value, 15);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_SLOT_8_set", value );

    /* (0x00000144 bits 3:0) field TX_SLOT_8 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    etrans_reg_TX_OP_SCH_CAL_CFG1_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_8_MSK,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_8_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_SLOT_8_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_SLOT_8_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000144 bits 3:0) field TX_SLOT_8 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG1_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_8_MSK) >> ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_8_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_SLOT_8_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_SLOT_8_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_SLOT_8_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_8_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_8_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_8_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000144 bits 3:0) field TX_SLOT_8 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
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
        /* (0x00000144 bits 3:0) field TX_SLOT_8 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
        etrans_reg_TX_OP_SCH_CAL_CFG1_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_8_OFF + subfield_offset),
                                                 ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_8_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_SLOT_8_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_SLOT_8_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_SLOT_8_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_SLOT_8_get", stop_bit, 3 );
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
    /* (0x00000144 bits 3:0) field TX_SLOT_8 of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG1_read(  b_ptr, h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_8_MSK)
                  >> ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_8_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_8_MSK, ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_SLOT_8_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_SLOT_8_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size12
 * ==================================================================================
 */
static INLINE void etrans_field_RX_CHID_set( etrans_buffer_t *b_ptr,
                                             etrans_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_CHID_set( etrans_buffer_t *b_ptr,
                                             etrans_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_RX_CHID_set", N, 11);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_CHID_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "etrans_field_RX_CHID_set", N, value );

    /* ((0x00000000 + (N) * 0x4) bits 6:0) field RX_CHID of register PMC_ETRANS120_REG_LINKID_MAPPING index N=0..11 */
    etrans_reg_LINKID_MAPPING_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ETRANS120_REG_LINKID_MAPPING_BIT_RX_CHID_MSK,
                                               ETRANS120_REG_LINKID_MAPPING_BIT_RX_CHID_OFF,
                                               value);
}

static INLINE UINT32 etrans_field_RX_CHID_get( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_CHID_get( etrans_buffer_t *b_ptr,
                                               etrans_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_RX_CHID_get", N, 11);
    /* ((0x00000000 + (N) * 0x4) bits 6:0) field RX_CHID of register PMC_ETRANS120_REG_LINKID_MAPPING index N=0..11 */
    reg_value = etrans_reg_LINKID_MAPPING_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ETRANS120_REG_LINKID_MAPPING_BIT_RX_CHID_MSK) >> ETRANS120_REG_LINKID_MAPPING_BIT_RX_CHID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "etrans_field_RX_CHID_get", N, value );

    return value;
}
static INLINE void etrans_field_range_RX_CHID_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_RX_CHID_set( etrans_buffer_t *b_ptr,
                                                   etrans_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_range_RX_CHID_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_CHID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_CHID_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_CHID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000000 + (N) * 0x4) bits 6:0) field RX_CHID of register PMC_ETRANS120_REG_LINKID_MAPPING index N=0..11 */
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
        /* ((0x00000000 + (N) * 0x4) bits 6:0) field RX_CHID of register PMC_ETRANS120_REG_LINKID_MAPPING index N=0..11 */
        etrans_reg_LINKID_MAPPING_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (ETRANS120_REG_LINKID_MAPPING_BIT_RX_CHID_OFF + subfield_offset),
                                                   ETRANS120_REG_LINKID_MAPPING_BIT_RX_CHID_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_RX_CHID_get( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_RX_CHID_get( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_range_RX_CHID_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_CHID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_CHID_get", stop_bit, 6 );
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
    /* ((0x00000000 + (N) * 0x4) bits 6:0) field RX_CHID of register PMC_ETRANS120_REG_LINKID_MAPPING index N=0..11 */
    reg_value = etrans_reg_LINKID_MAPPING_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & ETRANS120_REG_LINKID_MAPPING_BIT_RX_CHID_MSK)
                  >> ETRANS120_REG_LINKID_MAPPING_BIT_RX_CHID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_LINKID_MAPPING_BIT_RX_CHID_MSK, ETRANS120_REG_LINKID_MAPPING_BIT_RX_CHID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_CHID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_RX_10G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_10G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_RX_10G_LINK_DP_SEL_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_10G_LINK_DP_SEL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "etrans_field_RX_10G_LINK_DP_SEL_set", N, value );

    /* ((0x00000054 + (N) * 0x04) bits 3:2) field RX_10G_LINK_DP_SEL of register PMC_ETRANS120_REG_RX_10G_LINK_CFG_REG index N=0..11 */
    etrans_reg_RX_10G_LINK_CFG_REG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    ETRANS120_REG_RX_10G_LINK_CFG_REG_BIT_RX_10G_LINK_DP_SEL_MSK,
                                                    ETRANS120_REG_RX_10G_LINK_CFG_REG_BIT_RX_10G_LINK_DP_SEL_OFF,
                                                    value);
}

static INLINE UINT32 etrans_field_RX_10G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_10G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_RX_10G_LINK_DP_SEL_get", N, 11);
    /* ((0x00000054 + (N) * 0x04) bits 3:2) field RX_10G_LINK_DP_SEL of register PMC_ETRANS120_REG_RX_10G_LINK_CFG_REG index N=0..11 */
    reg_value = etrans_reg_RX_10G_LINK_CFG_REG_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & ETRANS120_REG_RX_10G_LINK_CFG_REG_BIT_RX_10G_LINK_DP_SEL_MSK) >> ETRANS120_REG_RX_10G_LINK_CFG_REG_BIT_RX_10G_LINK_DP_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "etrans_field_RX_10G_LINK_DP_SEL_get", N, value );

    return value;
}
static INLINE void etrans_field_range_RX_10G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_RX_10G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_range_RX_10G_LINK_DP_SEL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_10G_LINK_DP_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_10G_LINK_DP_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_10G_LINK_DP_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000054 + (N) * 0x04) bits 3:2) field RX_10G_LINK_DP_SEL of register PMC_ETRANS120_REG_RX_10G_LINK_CFG_REG index N=0..11 */
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
        /* ((0x00000054 + (N) * 0x04) bits 3:2) field RX_10G_LINK_DP_SEL of register PMC_ETRANS120_REG_RX_10G_LINK_CFG_REG index N=0..11 */
        etrans_reg_RX_10G_LINK_CFG_REG_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        subfield_mask << (ETRANS120_REG_RX_10G_LINK_CFG_REG_BIT_RX_10G_LINK_DP_SEL_OFF + subfield_offset),
                                                        ETRANS120_REG_RX_10G_LINK_CFG_REG_BIT_RX_10G_LINK_DP_SEL_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_RX_10G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_RX_10G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_range_RX_10G_LINK_DP_SEL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_10G_LINK_DP_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_10G_LINK_DP_SEL_get", stop_bit, 1 );
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
    /* ((0x00000054 + (N) * 0x04) bits 3:2) field RX_10G_LINK_DP_SEL of register PMC_ETRANS120_REG_RX_10G_LINK_CFG_REG index N=0..11 */
    reg_value = etrans_reg_RX_10G_LINK_CFG_REG_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & ETRANS120_REG_RX_10G_LINK_CFG_REG_BIT_RX_10G_LINK_DP_SEL_MSK)
                  >> ETRANS120_REG_RX_10G_LINK_CFG_REG_BIT_RX_10G_LINK_DP_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_RX_10G_LINK_CFG_REG_BIT_RX_10G_LINK_DP_SEL_MSK, ETRANS120_REG_RX_10G_LINK_CFG_REG_BIT_RX_10G_LINK_DP_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_10G_LINK_DP_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_RX_10G_LINK_EN_set( etrans_buffer_t *b_ptr,
                                                    etrans_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_10G_LINK_EN_set( etrans_buffer_t *b_ptr,
                                                    etrans_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_RX_10G_LINK_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_10G_LINK_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "etrans_field_RX_10G_LINK_EN_set", N, value );

    /* ((0x00000054 + (N) * 0x04) bits 1) field RX_10G_LINK_EN of register PMC_ETRANS120_REG_RX_10G_LINK_CFG_REG index N=0..11 */
    etrans_reg_RX_10G_LINK_CFG_REG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    ETRANS120_REG_RX_10G_LINK_CFG_REG_BIT_RX_10G_LINK_EN_MSK,
                                                    ETRANS120_REG_RX_10G_LINK_CFG_REG_BIT_RX_10G_LINK_EN_OFF,
                                                    value);
}

static INLINE UINT32 etrans_field_RX_10G_LINK_EN_get( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_10G_LINK_EN_get( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_RX_10G_LINK_EN_get", N, 11);
    /* ((0x00000054 + (N) * 0x04) bits 1) field RX_10G_LINK_EN of register PMC_ETRANS120_REG_RX_10G_LINK_CFG_REG index N=0..11 */
    reg_value = etrans_reg_RX_10G_LINK_CFG_REG_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & ETRANS120_REG_RX_10G_LINK_CFG_REG_BIT_RX_10G_LINK_EN_MSK) >> ETRANS120_REG_RX_10G_LINK_CFG_REG_BIT_RX_10G_LINK_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "etrans_field_RX_10G_LINK_EN_get", N, value );

    return value;
}
static INLINE void etrans_field_RX_10G_LINK_RST_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_10G_LINK_RST_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_RX_10G_LINK_RST_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_10G_LINK_RST_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "etrans_field_RX_10G_LINK_RST_set", N, value );

    /* ((0x00000054 + (N) * 0x04) bits 0) field RX_10G_LINK_RST of register PMC_ETRANS120_REG_RX_10G_LINK_CFG_REG index N=0..11 */
    etrans_reg_RX_10G_LINK_CFG_REG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    ETRANS120_REG_RX_10G_LINK_CFG_REG_BIT_RX_10G_LINK_RST_MSK,
                                                    ETRANS120_REG_RX_10G_LINK_CFG_REG_BIT_RX_10G_LINK_RST_OFF,
                                                    value);
}

static INLINE UINT32 etrans_field_RX_10G_LINK_RST_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_10G_LINK_RST_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_RX_10G_LINK_RST_get", N, 11);
    /* ((0x00000054 + (N) * 0x04) bits 0) field RX_10G_LINK_RST of register PMC_ETRANS120_REG_RX_10G_LINK_CFG_REG index N=0..11 */
    reg_value = etrans_reg_RX_10G_LINK_CFG_REG_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & ETRANS120_REG_RX_10G_LINK_CFG_REG_BIT_RX_10G_LINK_RST_MSK) >> ETRANS120_REG_RX_10G_LINK_CFG_REG_BIT_RX_10G_LINK_RST_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "etrans_field_RX_10G_LINK_RST_get", N, value );

    return value;
}
static INLINE void etrans_field_TX_10G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_10G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_TX_10G_LINK_DP_SEL_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_10G_LINK_DP_SEL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "etrans_field_TX_10G_LINK_DP_SEL_set", N, value );

    /* ((0x000000c4 + (N) * 0x04) bits 3:2) field TX_10G_LINK_DP_SEL of register PMC_ETRANS120_REG_TX_10G_LINK_CFG index N=0..11 */
    etrans_reg_TX_10G_LINK_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ETRANS120_REG_TX_10G_LINK_CFG_BIT_TX_10G_LINK_DP_SEL_MSK,
                                                ETRANS120_REG_TX_10G_LINK_CFG_BIT_TX_10G_LINK_DP_SEL_OFF,
                                                value);
}

static INLINE UINT32 etrans_field_TX_10G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_10G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_TX_10G_LINK_DP_SEL_get", N, 11);
    /* ((0x000000c4 + (N) * 0x04) bits 3:2) field TX_10G_LINK_DP_SEL of register PMC_ETRANS120_REG_TX_10G_LINK_CFG index N=0..11 */
    reg_value = etrans_reg_TX_10G_LINK_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ETRANS120_REG_TX_10G_LINK_CFG_BIT_TX_10G_LINK_DP_SEL_MSK) >> ETRANS120_REG_TX_10G_LINK_CFG_BIT_TX_10G_LINK_DP_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "etrans_field_TX_10G_LINK_DP_SEL_get", N, value );

    return value;
}
static INLINE void etrans_field_range_TX_10G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_10G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_range_TX_10G_LINK_DP_SEL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_10G_LINK_DP_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_10G_LINK_DP_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_10G_LINK_DP_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000000c4 + (N) * 0x04) bits 3:2) field TX_10G_LINK_DP_SEL of register PMC_ETRANS120_REG_TX_10G_LINK_CFG index N=0..11 */
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
        /* ((0x000000c4 + (N) * 0x04) bits 3:2) field TX_10G_LINK_DP_SEL of register PMC_ETRANS120_REG_TX_10G_LINK_CFG index N=0..11 */
        etrans_reg_TX_10G_LINK_CFG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (ETRANS120_REG_TX_10G_LINK_CFG_BIT_TX_10G_LINK_DP_SEL_OFF + subfield_offset),
                                                    ETRANS120_REG_TX_10G_LINK_CFG_BIT_TX_10G_LINK_DP_SEL_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_10G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_10G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_range_TX_10G_LINK_DP_SEL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_10G_LINK_DP_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_10G_LINK_DP_SEL_get", stop_bit, 1 );
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
    /* ((0x000000c4 + (N) * 0x04) bits 3:2) field TX_10G_LINK_DP_SEL of register PMC_ETRANS120_REG_TX_10G_LINK_CFG index N=0..11 */
    reg_value = etrans_reg_TX_10G_LINK_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & ETRANS120_REG_TX_10G_LINK_CFG_BIT_TX_10G_LINK_DP_SEL_MSK)
                  >> ETRANS120_REG_TX_10G_LINK_CFG_BIT_TX_10G_LINK_DP_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_10G_LINK_CFG_BIT_TX_10G_LINK_DP_SEL_MSK, ETRANS120_REG_TX_10G_LINK_CFG_BIT_TX_10G_LINK_DP_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_10G_LINK_DP_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_10G_LINK_EN_set( etrans_buffer_t *b_ptr,
                                                    etrans_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_10G_LINK_EN_set( etrans_buffer_t *b_ptr,
                                                    etrans_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_TX_10G_LINK_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_10G_LINK_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "etrans_field_TX_10G_LINK_EN_set", N, value );

    /* ((0x000000c4 + (N) * 0x04) bits 1) field TX_10G_LINK_EN of register PMC_ETRANS120_REG_TX_10G_LINK_CFG index N=0..11 */
    etrans_reg_TX_10G_LINK_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ETRANS120_REG_TX_10G_LINK_CFG_BIT_TX_10G_LINK_EN_MSK,
                                                ETRANS120_REG_TX_10G_LINK_CFG_BIT_TX_10G_LINK_EN_OFF,
                                                value);
}

static INLINE UINT32 etrans_field_TX_10G_LINK_EN_get( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_10G_LINK_EN_get( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_TX_10G_LINK_EN_get", N, 11);
    /* ((0x000000c4 + (N) * 0x04) bits 1) field TX_10G_LINK_EN of register PMC_ETRANS120_REG_TX_10G_LINK_CFG index N=0..11 */
    reg_value = etrans_reg_TX_10G_LINK_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ETRANS120_REG_TX_10G_LINK_CFG_BIT_TX_10G_LINK_EN_MSK) >> ETRANS120_REG_TX_10G_LINK_CFG_BIT_TX_10G_LINK_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "etrans_field_TX_10G_LINK_EN_get", N, value );

    return value;
}
static INLINE void etrans_field_TX_10G_LINK_RST_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_10G_LINK_RST_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_TX_10G_LINK_RST_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_10G_LINK_RST_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "etrans_field_TX_10G_LINK_RST_set", N, value );

    /* ((0x000000c4 + (N) * 0x04) bits 0) field TX_10G_LINK_RST of register PMC_ETRANS120_REG_TX_10G_LINK_CFG index N=0..11 */
    etrans_reg_TX_10G_LINK_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ETRANS120_REG_TX_10G_LINK_CFG_BIT_TX_10G_LINK_RST_MSK,
                                                ETRANS120_REG_TX_10G_LINK_CFG_BIT_TX_10G_LINK_RST_OFF,
                                                value);
}

static INLINE UINT32 etrans_field_TX_10G_LINK_RST_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_10G_LINK_RST_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_TX_10G_LINK_RST_get", N, 11);
    /* ((0x000000c4 + (N) * 0x04) bits 0) field TX_10G_LINK_RST of register PMC_ETRANS120_REG_TX_10G_LINK_CFG index N=0..11 */
    reg_value = etrans_reg_TX_10G_LINK_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ETRANS120_REG_TX_10G_LINK_CFG_BIT_TX_10G_LINK_RST_MSK) >> ETRANS120_REG_TX_10G_LINK_CFG_BIT_TX_10G_LINK_RST_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "etrans_field_TX_10G_LINK_RST_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size3
 * ==================================================================================
 */
static INLINE void etrans_field_RX_40G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_40G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_RX_40G_LINK_DP_SEL_set", N, 2);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_40G_LINK_DP_SEL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "etrans_field_RX_40G_LINK_DP_SEL_set", N, value );

    /* ((0x00000084 + (N) * 0x04) bits 3:2) field RX_40G_LINK_DP_SEL of register PMC_ETRANS120_REG_RX_40G_LINK_CFG_REG index N=0..2 */
    etrans_reg_RX_40G_LINK_CFG_REG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    ETRANS120_REG_RX_40G_LINK_CFG_REG_BIT_RX_40G_LINK_DP_SEL_MSK,
                                                    ETRANS120_REG_RX_40G_LINK_CFG_REG_BIT_RX_40G_LINK_DP_SEL_OFF,
                                                    value);
}

static INLINE UINT32 etrans_field_RX_40G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_40G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_RX_40G_LINK_DP_SEL_get", N, 2);
    /* ((0x00000084 + (N) * 0x04) bits 3:2) field RX_40G_LINK_DP_SEL of register PMC_ETRANS120_REG_RX_40G_LINK_CFG_REG index N=0..2 */
    reg_value = etrans_reg_RX_40G_LINK_CFG_REG_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & ETRANS120_REG_RX_40G_LINK_CFG_REG_BIT_RX_40G_LINK_DP_SEL_MSK) >> ETRANS120_REG_RX_40G_LINK_CFG_REG_BIT_RX_40G_LINK_DP_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "etrans_field_RX_40G_LINK_DP_SEL_get", N, value );

    return value;
}
static INLINE void etrans_field_range_RX_40G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_RX_40G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_range_RX_40G_LINK_DP_SEL_set", N, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_40G_LINK_DP_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_40G_LINK_DP_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_40G_LINK_DP_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000084 + (N) * 0x04) bits 3:2) field RX_40G_LINK_DP_SEL of register PMC_ETRANS120_REG_RX_40G_LINK_CFG_REG index N=0..2 */
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
        /* ((0x00000084 + (N) * 0x04) bits 3:2) field RX_40G_LINK_DP_SEL of register PMC_ETRANS120_REG_RX_40G_LINK_CFG_REG index N=0..2 */
        etrans_reg_RX_40G_LINK_CFG_REG_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        subfield_mask << (ETRANS120_REG_RX_40G_LINK_CFG_REG_BIT_RX_40G_LINK_DP_SEL_OFF + subfield_offset),
                                                        ETRANS120_REG_RX_40G_LINK_CFG_REG_BIT_RX_40G_LINK_DP_SEL_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_RX_40G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_RX_40G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
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

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_range_RX_40G_LINK_DP_SEL_get", N, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_40G_LINK_DP_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_40G_LINK_DP_SEL_get", stop_bit, 1 );
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
    /* ((0x00000084 + (N) * 0x04) bits 3:2) field RX_40G_LINK_DP_SEL of register PMC_ETRANS120_REG_RX_40G_LINK_CFG_REG index N=0..2 */
    reg_value = etrans_reg_RX_40G_LINK_CFG_REG_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & ETRANS120_REG_RX_40G_LINK_CFG_REG_BIT_RX_40G_LINK_DP_SEL_MSK)
                  >> ETRANS120_REG_RX_40G_LINK_CFG_REG_BIT_RX_40G_LINK_DP_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_RX_40G_LINK_CFG_REG_BIT_RX_40G_LINK_DP_SEL_MSK, ETRANS120_REG_RX_40G_LINK_CFG_REG_BIT_RX_40G_LINK_DP_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_40G_LINK_DP_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_RX_40G_LINK_EN_set( etrans_buffer_t *b_ptr,
                                                    etrans_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_40G_LINK_EN_set( etrans_buffer_t *b_ptr,
                                                    etrans_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_RX_40G_LINK_EN_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_40G_LINK_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "etrans_field_RX_40G_LINK_EN_set", N, value );

    /* ((0x00000084 + (N) * 0x04) bits 1) field RX_40G_LINK_EN of register PMC_ETRANS120_REG_RX_40G_LINK_CFG_REG index N=0..2 */
    etrans_reg_RX_40G_LINK_CFG_REG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    ETRANS120_REG_RX_40G_LINK_CFG_REG_BIT_RX_40G_LINK_EN_MSK,
                                                    ETRANS120_REG_RX_40G_LINK_CFG_REG_BIT_RX_40G_LINK_EN_OFF,
                                                    value);
}

static INLINE UINT32 etrans_field_RX_40G_LINK_EN_get( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_40G_LINK_EN_get( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_RX_40G_LINK_EN_get", N, 2);
    /* ((0x00000084 + (N) * 0x04) bits 1) field RX_40G_LINK_EN of register PMC_ETRANS120_REG_RX_40G_LINK_CFG_REG index N=0..2 */
    reg_value = etrans_reg_RX_40G_LINK_CFG_REG_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & ETRANS120_REG_RX_40G_LINK_CFG_REG_BIT_RX_40G_LINK_EN_MSK) >> ETRANS120_REG_RX_40G_LINK_CFG_REG_BIT_RX_40G_LINK_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "etrans_field_RX_40G_LINK_EN_get", N, value );

    return value;
}
static INLINE void etrans_field_RX_40G_LINK_RST_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_40G_LINK_RST_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_RX_40G_LINK_RST_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_40G_LINK_RST_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "etrans_field_RX_40G_LINK_RST_set", N, value );

    /* ((0x00000084 + (N) * 0x04) bits 0) field RX_40G_LINK_RST of register PMC_ETRANS120_REG_RX_40G_LINK_CFG_REG index N=0..2 */
    etrans_reg_RX_40G_LINK_CFG_REG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    ETRANS120_REG_RX_40G_LINK_CFG_REG_BIT_RX_40G_LINK_RST_MSK,
                                                    ETRANS120_REG_RX_40G_LINK_CFG_REG_BIT_RX_40G_LINK_RST_OFF,
                                                    value);
}

static INLINE UINT32 etrans_field_RX_40G_LINK_RST_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_40G_LINK_RST_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_RX_40G_LINK_RST_get", N, 2);
    /* ((0x00000084 + (N) * 0x04) bits 0) field RX_40G_LINK_RST of register PMC_ETRANS120_REG_RX_40G_LINK_CFG_REG index N=0..2 */
    reg_value = etrans_reg_RX_40G_LINK_CFG_REG_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & ETRANS120_REG_RX_40G_LINK_CFG_REG_BIT_RX_40G_LINK_RST_MSK) >> ETRANS120_REG_RX_40G_LINK_CFG_REG_BIT_RX_40G_LINK_RST_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "etrans_field_RX_40G_LINK_RST_get", N, value );

    return value;
}
static INLINE void etrans_field_TX_40G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_40G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_TX_40G_LINK_DP_SEL_set", N, 2);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_40G_LINK_DP_SEL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "etrans_field_TX_40G_LINK_DP_SEL_set", N, value );

    /* ((0x000000f4 + (N) * 0x04) bits 3:2) field TX_40G_LINK_DP_SEL of register PMC_ETRANS120_REG_TX_40G_LINK_CFG index N=0..2 */
    etrans_reg_TX_40G_LINK_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ETRANS120_REG_TX_40G_LINK_CFG_BIT_TX_40G_LINK_DP_SEL_MSK,
                                                ETRANS120_REG_TX_40G_LINK_CFG_BIT_TX_40G_LINK_DP_SEL_OFF,
                                                value);
}

static INLINE UINT32 etrans_field_TX_40G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_40G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                          etrans_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_TX_40G_LINK_DP_SEL_get", N, 2);
    /* ((0x000000f4 + (N) * 0x04) bits 3:2) field TX_40G_LINK_DP_SEL of register PMC_ETRANS120_REG_TX_40G_LINK_CFG index N=0..2 */
    reg_value = etrans_reg_TX_40G_LINK_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ETRANS120_REG_TX_40G_LINK_CFG_BIT_TX_40G_LINK_DP_SEL_MSK) >> ETRANS120_REG_TX_40G_LINK_CFG_BIT_TX_40G_LINK_DP_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "etrans_field_TX_40G_LINK_DP_SEL_get", N, value );

    return value;
}
static INLINE void etrans_field_range_TX_40G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_40G_LINK_DP_SEL_set( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_range_TX_40G_LINK_DP_SEL_set", N, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_40G_LINK_DP_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_40G_LINK_DP_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_40G_LINK_DP_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000000f4 + (N) * 0x04) bits 3:2) field TX_40G_LINK_DP_SEL of register PMC_ETRANS120_REG_TX_40G_LINK_CFG index N=0..2 */
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
        /* ((0x000000f4 + (N) * 0x04) bits 3:2) field TX_40G_LINK_DP_SEL of register PMC_ETRANS120_REG_TX_40G_LINK_CFG index N=0..2 */
        etrans_reg_TX_40G_LINK_CFG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (ETRANS120_REG_TX_40G_LINK_CFG_BIT_TX_40G_LINK_DP_SEL_OFF + subfield_offset),
                                                    ETRANS120_REG_TX_40G_LINK_CFG_BIT_TX_40G_LINK_DP_SEL_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_40G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_40G_LINK_DP_SEL_get( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
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

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_range_TX_40G_LINK_DP_SEL_get", N, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_40G_LINK_DP_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_40G_LINK_DP_SEL_get", stop_bit, 1 );
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
    /* ((0x000000f4 + (N) * 0x04) bits 3:2) field TX_40G_LINK_DP_SEL of register PMC_ETRANS120_REG_TX_40G_LINK_CFG index N=0..2 */
    reg_value = etrans_reg_TX_40G_LINK_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & ETRANS120_REG_TX_40G_LINK_CFG_BIT_TX_40G_LINK_DP_SEL_MSK)
                  >> ETRANS120_REG_TX_40G_LINK_CFG_BIT_TX_40G_LINK_DP_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_40G_LINK_CFG_BIT_TX_40G_LINK_DP_SEL_MSK, ETRANS120_REG_TX_40G_LINK_CFG_BIT_TX_40G_LINK_DP_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_40G_LINK_DP_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_40G_LINK_EN_set( etrans_buffer_t *b_ptr,
                                                    etrans_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_40G_LINK_EN_set( etrans_buffer_t *b_ptr,
                                                    etrans_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_TX_40G_LINK_EN_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_40G_LINK_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "etrans_field_TX_40G_LINK_EN_set", N, value );

    /* ((0x000000f4 + (N) * 0x04) bits 1) field TX_40G_LINK_EN of register PMC_ETRANS120_REG_TX_40G_LINK_CFG index N=0..2 */
    etrans_reg_TX_40G_LINK_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ETRANS120_REG_TX_40G_LINK_CFG_BIT_TX_40G_LINK_EN_MSK,
                                                ETRANS120_REG_TX_40G_LINK_CFG_BIT_TX_40G_LINK_EN_OFF,
                                                value);
}

static INLINE UINT32 etrans_field_TX_40G_LINK_EN_get( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_40G_LINK_EN_get( etrans_buffer_t *b_ptr,
                                                      etrans_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_TX_40G_LINK_EN_get", N, 2);
    /* ((0x000000f4 + (N) * 0x04) bits 1) field TX_40G_LINK_EN of register PMC_ETRANS120_REG_TX_40G_LINK_CFG index N=0..2 */
    reg_value = etrans_reg_TX_40G_LINK_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ETRANS120_REG_TX_40G_LINK_CFG_BIT_TX_40G_LINK_EN_MSK) >> ETRANS120_REG_TX_40G_LINK_CFG_BIT_TX_40G_LINK_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "etrans_field_TX_40G_LINK_EN_get", N, value );

    return value;
}
static INLINE void etrans_field_TX_40G_LINK_RST_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_40G_LINK_RST_set( etrans_buffer_t *b_ptr,
                                                     etrans_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_TX_40G_LINK_RST_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_40G_LINK_RST_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "etrans_field_TX_40G_LINK_RST_set", N, value );

    /* ((0x000000f4 + (N) * 0x04) bits 0) field TX_40G_LINK_RST of register PMC_ETRANS120_REG_TX_40G_LINK_CFG index N=0..2 */
    etrans_reg_TX_40G_LINK_CFG_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ETRANS120_REG_TX_40G_LINK_CFG_BIT_TX_40G_LINK_RST_MSK,
                                                ETRANS120_REG_TX_40G_LINK_CFG_BIT_TX_40G_LINK_RST_OFF,
                                                value);
}

static INLINE UINT32 etrans_field_TX_40G_LINK_RST_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_40G_LINK_RST_get( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "etrans_field_TX_40G_LINK_RST_get", N, 2);
    /* ((0x000000f4 + (N) * 0x04) bits 0) field TX_40G_LINK_RST of register PMC_ETRANS120_REG_TX_40G_LINK_CFG index N=0..2 */
    reg_value = etrans_reg_TX_40G_LINK_CFG_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ETRANS120_REG_TX_40G_LINK_CFG_BIT_TX_40G_LINK_RST_MSK) >> ETRANS120_REG_TX_40G_LINK_CFG_BIT_TX_40G_LINK_RST_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "etrans_field_TX_40G_LINK_RST_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void etrans_field_TX_LINK_SCH_UPDATE_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK_SCH_UPDATE_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK_SCH_UPDATE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK_SCH_UPDATE_set", value );

    /* (0x00000144 bits 31) field TX_LINK_SCH_UPDATE of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    etrans_reg_TX_OP_SCH_CAL_CFG1_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_UPDATE_MSK,
                                             ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_UPDATE_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_LINK_SCH_UPDATE_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK_SCH_UPDATE_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000144 bits 31) field TX_LINK_SCH_UPDATE of register PMC_ETRANS120_REG_TX_OP_SCH_CAL_CFG1 */
    reg_value = etrans_reg_TX_OP_SCH_CAL_CFG1_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_UPDATE_MSK) >> ETRANS120_REG_TX_OP_SCH_CAL_CFG1_BIT_TX_LINK_SCH_UPDATE_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK_SCH_UPDATE_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void etrans_field_RX_100G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_100G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_100G_FIFO_OVF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_RX_100G_FIFO_OVF_E_set", value );

    /* (0x00000094 bits 15) field RX_100G_FIFO_OVF_E of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN */
    etrans_reg_RXFIFO_OVERFLOW_INT_EN_field_set( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_100G_FIFO_OVF_E_MSK,
                                                 ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_100G_FIFO_OVF_E_OFF,
                                                 value);
}

static INLINE UINT32 etrans_field_RX_100G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_100G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 15) field RX_100G_FIFO_OVF_E of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN */
    reg_value = etrans_reg_RXFIFO_OVERFLOW_INT_EN_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_100G_FIFO_OVF_E_MSK) >> ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_100G_FIFO_OVF_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_100G_FIFO_OVF_E_get", value );

    return value;
}
static INLINE void etrans_field_RX_40G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_40G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_40G_FIFO_OVF_E_set", value, 7);
    IOLOG( "%s <= 0x%08x", "etrans_field_RX_40G_FIFO_OVF_E_set", value );

    /* (0x00000094 bits 14:12) field RX_40G_FIFO_OVF_E of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN */
    etrans_reg_RXFIFO_OVERFLOW_INT_EN_field_set( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_40G_FIFO_OVF_E_MSK,
                                                 ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_40G_FIFO_OVF_E_OFF,
                                                 value);
}

static INLINE UINT32 etrans_field_RX_40G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_40G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 14:12) field RX_40G_FIFO_OVF_E of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN */
    reg_value = etrans_reg_RXFIFO_OVERFLOW_INT_EN_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_40G_FIFO_OVF_E_MSK) >> ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_40G_FIFO_OVF_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_40G_FIFO_OVF_E_get", value );

    return value;
}
static INLINE void etrans_field_range_RX_40G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_RX_40G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_40G_FIFO_OVF_E_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_40G_FIFO_OVF_E_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_40G_FIFO_OVF_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000094 bits 14:12) field RX_40G_FIFO_OVF_E of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN */
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
        /* (0x00000094 bits 14:12) field RX_40G_FIFO_OVF_E of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN */
        etrans_reg_RXFIFO_OVERFLOW_INT_EN_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_40G_FIFO_OVF_E_OFF + subfield_offset),
                                                     ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_40G_FIFO_OVF_E_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_RX_40G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_RX_40G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_40G_FIFO_OVF_E_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_40G_FIFO_OVF_E_get", stop_bit, 2 );
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
    /* (0x00000094 bits 14:12) field RX_40G_FIFO_OVF_E of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN */
    reg_value = etrans_reg_RXFIFO_OVERFLOW_INT_EN_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_40G_FIFO_OVF_E_MSK)
                  >> ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_40G_FIFO_OVF_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_40G_FIFO_OVF_E_MSK, ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_40G_FIFO_OVF_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_40G_FIFO_OVF_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_RX_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_10G_FIFO_OVF_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "etrans_field_RX_10G_FIFO_OVF_E_set", value );

    /* (0x00000094 bits 11:0) field RX_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN */
    etrans_reg_RXFIFO_OVERFLOW_INT_EN_field_set( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_10G_FIFO_OVF_E_MSK,
                                                 ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_10G_FIFO_OVF_E_OFF,
                                                 value);
}

static INLINE UINT32 etrans_field_RX_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000094 bits 11:0) field RX_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN */
    reg_value = etrans_reg_RXFIFO_OVERFLOW_INT_EN_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_10G_FIFO_OVF_E_MSK) >> ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_10G_FIFO_OVF_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_10G_FIFO_OVF_E_get", value );

    return value;
}
static INLINE void etrans_field_range_RX_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_RX_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_10G_FIFO_OVF_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_10G_FIFO_OVF_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_10G_FIFO_OVF_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000094 bits 11:0) field RX_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN */
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
        /* (0x00000094 bits 11:0) field RX_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN */
        etrans_reg_RXFIFO_OVERFLOW_INT_EN_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_10G_FIFO_OVF_E_OFF + subfield_offset),
                                                     ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_10G_FIFO_OVF_E_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_RX_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_RX_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_10G_FIFO_OVF_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_10G_FIFO_OVF_E_get", stop_bit, 11 );
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
    /* (0x00000094 bits 11:0) field RX_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN */
    reg_value = etrans_reg_RXFIFO_OVERFLOW_INT_EN_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_10G_FIFO_OVF_E_MSK)
                  >> ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_10G_FIFO_OVF_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_10G_FIFO_OVF_E_MSK, ETRANS120_REG_RXFIFO_OVERFLOW_INT_EN_BIT_RX_10G_FIFO_OVF_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_10G_FIFO_OVF_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_RX_100G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_100G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_100G_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_RX_100G_FIFO_UDR_E_set", value );

    /* (0x00000098 bits 15) field RX_100G_FIFO_UDR_E of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN */
    etrans_reg_RXFIFO_UNDERRUN_INT_EN_field_set( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_100G_FIFO_UDR_E_MSK,
                                                 ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_100G_FIFO_UDR_E_OFF,
                                                 value);
}

static INLINE UINT32 etrans_field_RX_100G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_100G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000098 bits 15) field RX_100G_FIFO_UDR_E of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN */
    reg_value = etrans_reg_RXFIFO_UNDERRUN_INT_EN_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_100G_FIFO_UDR_E_MSK) >> ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_100G_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_100G_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void etrans_field_RX_40G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_40G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_40G_FIFO_UDR_E_set", value, 7);
    IOLOG( "%s <= 0x%08x", "etrans_field_RX_40G_FIFO_UDR_E_set", value );

    /* (0x00000098 bits 14:12) field RX_40G_FIFO_UDR_E of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN */
    etrans_reg_RXFIFO_UNDERRUN_INT_EN_field_set( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_40G_FIFO_UDR_E_MSK,
                                                 ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_40G_FIFO_UDR_E_OFF,
                                                 value);
}

static INLINE UINT32 etrans_field_RX_40G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_40G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000098 bits 14:12) field RX_40G_FIFO_UDR_E of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN */
    reg_value = etrans_reg_RXFIFO_UNDERRUN_INT_EN_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_40G_FIFO_UDR_E_MSK) >> ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_40G_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_40G_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void etrans_field_range_RX_40G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_RX_40G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_40G_FIFO_UDR_E_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_40G_FIFO_UDR_E_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_40G_FIFO_UDR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000098 bits 14:12) field RX_40G_FIFO_UDR_E of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN */
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
        /* (0x00000098 bits 14:12) field RX_40G_FIFO_UDR_E of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN */
        etrans_reg_RXFIFO_UNDERRUN_INT_EN_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_40G_FIFO_UDR_E_OFF + subfield_offset),
                                                     ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_40G_FIFO_UDR_E_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_RX_40G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_RX_40G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_40G_FIFO_UDR_E_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_40G_FIFO_UDR_E_get", stop_bit, 2 );
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
    /* (0x00000098 bits 14:12) field RX_40G_FIFO_UDR_E of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN */
    reg_value = etrans_reg_RXFIFO_UNDERRUN_INT_EN_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_40G_FIFO_UDR_E_MSK)
                  >> ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_40G_FIFO_UDR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_40G_FIFO_UDR_E_MSK, ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_40G_FIFO_UDR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_40G_FIFO_UDR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_RX_10G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_10G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_10G_FIFO_UDR_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "etrans_field_RX_10G_FIFO_UDR_E_set", value );

    /* (0x00000098 bits 11:0) field RX_10G_FIFO_UDR_E of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN */
    etrans_reg_RXFIFO_UNDERRUN_INT_EN_field_set( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_10G_FIFO_UDR_E_MSK,
                                                 ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_10G_FIFO_UDR_E_OFF,
                                                 value);
}

static INLINE UINT32 etrans_field_RX_10G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_10G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000098 bits 11:0) field RX_10G_FIFO_UDR_E of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN */
    reg_value = etrans_reg_RXFIFO_UNDERRUN_INT_EN_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_10G_FIFO_UDR_E_MSK) >> ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_10G_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_10G_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void etrans_field_range_RX_10G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_RX_10G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_10G_FIFO_UDR_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_10G_FIFO_UDR_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_10G_FIFO_UDR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000098 bits 11:0) field RX_10G_FIFO_UDR_E of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN */
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
        /* (0x00000098 bits 11:0) field RX_10G_FIFO_UDR_E of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN */
        etrans_reg_RXFIFO_UNDERRUN_INT_EN_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_10G_FIFO_UDR_E_OFF + subfield_offset),
                                                     ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_10G_FIFO_UDR_E_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_RX_10G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_RX_10G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_10G_FIFO_UDR_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_10G_FIFO_UDR_E_get", stop_bit, 11 );
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
    /* (0x00000098 bits 11:0) field RX_10G_FIFO_UDR_E of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN */
    reg_value = etrans_reg_RXFIFO_UNDERRUN_INT_EN_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_10G_FIFO_UDR_E_MSK)
                  >> ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_10G_FIFO_UDR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_10G_FIFO_UDR_E_MSK, ETRANS120_REG_RXFIFO_UNDERRUN_INT_EN_BIT_RX_10G_FIFO_UDR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_10G_FIFO_UDR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_100G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_100G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_100G_FIFO_OVF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_100G_FIFO_OVF_E_set", value );

    /* (0x00000104 bits 15) field TX_100G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    etrans_reg_TX_FIFO_OVF_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_100G_FIFO_OVF_E_MSK,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_100G_FIFO_OVF_E_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_100G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_100G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 15) field TX_100G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_100G_FIFO_OVF_E_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_100G_FIFO_OVF_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_100G_FIFO_OVF_E_get", value );

    return value;
}
static INLINE void etrans_field_TX_40G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_40G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_40G_FIFO_OVF_E_set", value, 7);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_40G_FIFO_OVF_E_set", value );

    /* (0x00000104 bits 14:12) field TX_40G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    etrans_reg_TX_FIFO_OVF_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_40G_FIFO_OVF_E_MSK,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_40G_FIFO_OVF_E_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_40G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_40G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 14:12) field TX_40G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_40G_FIFO_OVF_E_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_40G_FIFO_OVF_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_40G_FIFO_OVF_E_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_40G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_40G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_40G_FIFO_OVF_E_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_40G_FIFO_OVF_E_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_40G_FIFO_OVF_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000104 bits 14:12) field TX_40G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
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
        /* (0x00000104 bits 14:12) field TX_40G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
        etrans_reg_TX_FIFO_OVF_INT_EN_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_40G_FIFO_OVF_E_OFF + subfield_offset),
                                                 ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_40G_FIFO_OVF_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_40G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_40G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_40G_FIFO_OVF_E_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_40G_FIFO_OVF_E_get", stop_bit, 2 );
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
    /* (0x00000104 bits 14:12) field TX_40G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_40G_FIFO_OVF_E_MSK)
                  >> ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_40G_FIFO_OVF_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_40G_FIFO_OVF_E_MSK, ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_40G_FIFO_OVF_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_40G_FIFO_OVF_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_LINK11_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK11_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK11_10G_FIFO_OVF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK11_10G_FIFO_OVF_E_set", value );

    /* (0x00000104 bits 11) field TX_LINK11_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    etrans_reg_TX_FIFO_OVF_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK11_10G_FIFO_OVF_E_MSK,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK11_10G_FIFO_OVF_E_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_LINK11_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK11_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 11) field TX_LINK11_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK11_10G_FIFO_OVF_E_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK11_10G_FIFO_OVF_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK11_10G_FIFO_OVF_E_get", value );

    return value;
}
static INLINE void etrans_field_TX_LINK10_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK10_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                              etrans_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK10_10G_FIFO_OVF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK10_10G_FIFO_OVF_E_set", value );

    /* (0x00000104 bits 10) field TX_LINK10_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    etrans_reg_TX_FIFO_OVF_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK10_10G_FIFO_OVF_E_MSK,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK10_10G_FIFO_OVF_E_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_LINK10_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK10_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 10) field TX_LINK10_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK10_10G_FIFO_OVF_E_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK10_10G_FIFO_OVF_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK10_10G_FIFO_OVF_E_get", value );

    return value;
}
static INLINE void etrans_field_TX_LINK9_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK9_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK9_10G_FIFO_OVF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK9_10G_FIFO_OVF_E_set", value );

    /* (0x00000104 bits 9) field TX_LINK9_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    etrans_reg_TX_FIFO_OVF_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK9_10G_FIFO_OVF_E_MSK,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK9_10G_FIFO_OVF_E_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_LINK9_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK9_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 9) field TX_LINK9_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK9_10G_FIFO_OVF_E_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK9_10G_FIFO_OVF_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK9_10G_FIFO_OVF_E_get", value );

    return value;
}
static INLINE void etrans_field_TX_LINK8_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK8_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK8_10G_FIFO_OVF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK8_10G_FIFO_OVF_E_set", value );

    /* (0x00000104 bits 8) field TX_LINK8_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    etrans_reg_TX_FIFO_OVF_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK8_10G_FIFO_OVF_E_MSK,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK8_10G_FIFO_OVF_E_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_LINK8_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK8_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 8) field TX_LINK8_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK8_10G_FIFO_OVF_E_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK8_10G_FIFO_OVF_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK8_10G_FIFO_OVF_E_get", value );

    return value;
}
static INLINE void etrans_field_TX_LINK7_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK7_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK7_10G_FIFO_OVF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK7_10G_FIFO_OVF_E_set", value );

    /* (0x00000104 bits 7) field TX_LINK7_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    etrans_reg_TX_FIFO_OVF_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK7_10G_FIFO_OVF_E_MSK,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK7_10G_FIFO_OVF_E_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_LINK7_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK7_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 7) field TX_LINK7_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK7_10G_FIFO_OVF_E_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK7_10G_FIFO_OVF_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK7_10G_FIFO_OVF_E_get", value );

    return value;
}
static INLINE void etrans_field_TX_LINK6_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK6_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK6_10G_FIFO_OVF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK6_10G_FIFO_OVF_E_set", value );

    /* (0x00000104 bits 6) field TX_LINK6_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    etrans_reg_TX_FIFO_OVF_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK6_10G_FIFO_OVF_E_MSK,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK6_10G_FIFO_OVF_E_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_LINK6_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK6_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 6) field TX_LINK6_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK6_10G_FIFO_OVF_E_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK6_10G_FIFO_OVF_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK6_10G_FIFO_OVF_E_get", value );

    return value;
}
static INLINE void etrans_field_TX_LINK5_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK5_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK5_10G_FIFO_OVF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK5_10G_FIFO_OVF_E_set", value );

    /* (0x00000104 bits 5) field TX_LINK5_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    etrans_reg_TX_FIFO_OVF_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK5_10G_FIFO_OVF_E_MSK,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK5_10G_FIFO_OVF_E_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_LINK5_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK5_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 5) field TX_LINK5_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK5_10G_FIFO_OVF_E_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK5_10G_FIFO_OVF_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK5_10G_FIFO_OVF_E_get", value );

    return value;
}
static INLINE void etrans_field_TX_LINK4_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK4_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK4_10G_FIFO_OVF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK4_10G_FIFO_OVF_E_set", value );

    /* (0x00000104 bits 4) field TX_LINK4_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    etrans_reg_TX_FIFO_OVF_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK4_10G_FIFO_OVF_E_MSK,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK4_10G_FIFO_OVF_E_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_LINK4_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK4_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 4) field TX_LINK4_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK4_10G_FIFO_OVF_E_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK4_10G_FIFO_OVF_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK4_10G_FIFO_OVF_E_get", value );

    return value;
}
static INLINE void etrans_field_TX_LINK3_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK3_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK3_10G_FIFO_OVF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK3_10G_FIFO_OVF_E_set", value );

    /* (0x00000104 bits 3) field TX_LINK3_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    etrans_reg_TX_FIFO_OVF_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK3_10G_FIFO_OVF_E_MSK,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK3_10G_FIFO_OVF_E_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_LINK3_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK3_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 3) field TX_LINK3_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK3_10G_FIFO_OVF_E_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK3_10G_FIFO_OVF_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK3_10G_FIFO_OVF_E_get", value );

    return value;
}
static INLINE void etrans_field_TX_LINK2_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK2_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK2_10G_FIFO_OVF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK2_10G_FIFO_OVF_E_set", value );

    /* (0x00000104 bits 2) field TX_LINK2_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    etrans_reg_TX_FIFO_OVF_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK2_10G_FIFO_OVF_E_MSK,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK2_10G_FIFO_OVF_E_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_LINK2_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK2_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 2) field TX_LINK2_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK2_10G_FIFO_OVF_E_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK2_10G_FIFO_OVF_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK2_10G_FIFO_OVF_E_get", value );

    return value;
}
static INLINE void etrans_field_TX_LINK1_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK1_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK1_10G_FIFO_OVF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK1_10G_FIFO_OVF_E_set", value );

    /* (0x00000104 bits 1) field TX_LINK1_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    etrans_reg_TX_FIFO_OVF_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK1_10G_FIFO_OVF_E_MSK,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK1_10G_FIFO_OVF_E_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_LINK1_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK1_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 1) field TX_LINK1_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK1_10G_FIFO_OVF_E_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK1_10G_FIFO_OVF_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK1_10G_FIFO_OVF_E_get", value );

    return value;
}
static INLINE void etrans_field_TX_LINK0_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK0_10G_FIFO_OVF_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK0_10G_FIFO_OVF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK0_10G_FIFO_OVF_E_set", value );

    /* (0x00000104 bits 0) field TX_LINK0_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    etrans_reg_TX_FIFO_OVF_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK0_10G_FIFO_OVF_E_MSK,
                                             ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK0_10G_FIFO_OVF_E_OFF,
                                             value);
}

static INLINE UINT32 etrans_field_TX_LINK0_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK0_10G_FIFO_OVF_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 0) field TX_LINK0_10G_FIFO_OVF_E of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_EN */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK0_10G_FIFO_OVF_E_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_EN_BIT_TX_LINK0_10G_FIFO_OVF_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK0_10G_FIFO_OVF_E_get", value );

    return value;
}
static INLINE void etrans_field_TX_100G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_100G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                        etrans_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_100G_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_100G_FIFO_UDR_E_set", value );

    /* (0x00000108 bits 15) field TX_100G_FIFO_UDR_E of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN */
    etrans_reg_TX_FIFO_UNDERRUN_INT_EN_field_set( b_ptr,
                                                  h_ptr,
                                                  ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_100G_FIFO_UDR_E_MSK,
                                                  ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_100G_FIFO_UDR_E_OFF,
                                                  value);
}

static INLINE UINT32 etrans_field_TX_100G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_100G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000108 bits 15) field TX_100G_FIFO_UDR_E of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN */
    reg_value = etrans_reg_TX_FIFO_UNDERRUN_INT_EN_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_100G_FIFO_UDR_E_MSK) >> ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_100G_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_100G_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void etrans_field_TX_40G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_40G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_40G_FIFO_UDR_E_set", value, 7);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_40G_FIFO_UDR_E_set", value );

    /* (0x00000108 bits 14:12) field TX_40G_FIFO_UDR_E of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN */
    etrans_reg_TX_FIFO_UNDERRUN_INT_EN_field_set( b_ptr,
                                                  h_ptr,
                                                  ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_40G_FIFO_UDR_E_MSK,
                                                  ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_40G_FIFO_UDR_E_OFF,
                                                  value);
}

static INLINE UINT32 etrans_field_TX_40G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_40G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000108 bits 14:12) field TX_40G_FIFO_UDR_E of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN */
    reg_value = etrans_reg_TX_FIFO_UNDERRUN_INT_EN_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_40G_FIFO_UDR_E_MSK) >> ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_40G_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_40G_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_40G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_40G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_40G_FIFO_UDR_E_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_40G_FIFO_UDR_E_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_40G_FIFO_UDR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000108 bits 14:12) field TX_40G_FIFO_UDR_E of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN */
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
        /* (0x00000108 bits 14:12) field TX_40G_FIFO_UDR_E of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN */
        etrans_reg_TX_FIFO_UNDERRUN_INT_EN_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_40G_FIFO_UDR_E_OFF + subfield_offset),
                                                      ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_40G_FIFO_UDR_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_40G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_40G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_40G_FIFO_UDR_E_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_40G_FIFO_UDR_E_get", stop_bit, 2 );
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
    /* (0x00000108 bits 14:12) field TX_40G_FIFO_UDR_E of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN */
    reg_value = etrans_reg_TX_FIFO_UNDERRUN_INT_EN_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_40G_FIFO_UDR_E_MSK)
                  >> ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_40G_FIFO_UDR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_40G_FIFO_UDR_E_MSK, ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_40G_FIFO_UDR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_40G_FIFO_UDR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void etrans_field_TX_10G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_10G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                       etrans_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_10G_FIFO_UDR_E_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_10G_FIFO_UDR_E_set", value );

    /* (0x00000108 bits 11:0) field TX_10G_FIFO_UDR_E of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN */
    etrans_reg_TX_FIFO_UNDERRUN_INT_EN_field_set( b_ptr,
                                                  h_ptr,
                                                  ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_10G_FIFO_UDR_E_MSK,
                                                  ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_10G_FIFO_UDR_E_OFF,
                                                  value);
}

static INLINE UINT32 etrans_field_TX_10G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_10G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000108 bits 11:0) field TX_10G_FIFO_UDR_E of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN */
    reg_value = etrans_reg_TX_FIFO_UNDERRUN_INT_EN_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_10G_FIFO_UDR_E_MSK) >> ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_10G_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_10G_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_10G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_10G_FIFO_UDR_E_set( etrans_buffer_t *b_ptr,
                                                             etrans_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_10G_FIFO_UDR_E_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_10G_FIFO_UDR_E_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_10G_FIFO_UDR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000108 bits 11:0) field TX_10G_FIFO_UDR_E of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN */
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
        /* (0x00000108 bits 11:0) field TX_10G_FIFO_UDR_E of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN */
        etrans_reg_TX_FIFO_UNDERRUN_INT_EN_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_10G_FIFO_UDR_E_OFF + subfield_offset),
                                                      ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_10G_FIFO_UDR_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_10G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_10G_FIFO_UDR_E_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_10G_FIFO_UDR_E_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_10G_FIFO_UDR_E_get", stop_bit, 11 );
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
    /* (0x00000108 bits 11:0) field TX_10G_FIFO_UDR_E of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN */
    reg_value = etrans_reg_TX_FIFO_UNDERRUN_INT_EN_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_10G_FIFO_UDR_E_MSK)
                  >> ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_10G_FIFO_UDR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_10G_FIFO_UDR_E_MSK, ETRANS120_REG_TX_FIFO_UNDERRUN_INT_EN_BIT_TX_10G_FIFO_UDR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_10G_FIFO_UDR_E_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void etrans_field_RX_100G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_100G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_100G_FIFO_OVF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_RX_100G_FIFO_OVF_I_set_to_clear", value );

    /* (0x000000a4 bits 15) field RX_100G_FIFO_OVF_I of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT */
    etrans_reg_RXFIFO_OVERFLOW_INT_STAT_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_100G_FIFO_OVF_I_MSK,
                                                                   ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_100G_FIFO_OVF_I_OFF,
                                                                   value);
}

static INLINE UINT32 etrans_field_RX_100G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_100G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a4 bits 15) field RX_100G_FIFO_OVF_I of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT */
    reg_value = etrans_reg_RXFIFO_OVERFLOW_INT_STAT_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_100G_FIFO_OVF_I_MSK) >> ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_100G_FIFO_OVF_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_100G_FIFO_OVF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_100G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                         etrans_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_100G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                         etrans_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_RX_100G_FIFO_OVF_I_poll", value );

    /* (0x000000a4 bits 15) field RX_100G_FIFO_OVF_I of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT */
    return etrans_reg_RXFIFO_OVERFLOW_INT_STAT_poll( b_ptr,
                                                     h_ptr,
                                                     ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_100G_FIFO_OVF_I_MSK,
                                                     (value<<ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_100G_FIFO_OVF_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_RX_40G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_40G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_40G_FIFO_OVF_I_set_to_clear", value, 7);
    IOLOG( "%s <= 0x%08x", "etrans_field_RX_40G_FIFO_OVF_I_set_to_clear", value );

    /* (0x000000a4 bits 14:12) field RX_40G_FIFO_OVF_I of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT */
    etrans_reg_RXFIFO_OVERFLOW_INT_STAT_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_40G_FIFO_OVF_I_MSK,
                                                                   ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_40G_FIFO_OVF_I_OFF,
                                                                   value);
}

static INLINE UINT32 etrans_field_RX_40G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_40G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a4 bits 14:12) field RX_40G_FIFO_OVF_I of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT */
    reg_value = etrans_reg_RXFIFO_OVERFLOW_INT_STAT_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_40G_FIFO_OVF_I_MSK) >> ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_40G_FIFO_OVF_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_40G_FIFO_OVF_I_get", value );

    return value;
}
static INLINE void etrans_field_range_RX_40G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_RX_40G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_40G_FIFO_OVF_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_40G_FIFO_OVF_I_set_to_clear", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_40G_FIFO_OVF_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000a4 bits 14:12) field RX_40G_FIFO_OVF_I of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT */
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
        /* (0x000000a4 bits 14:12) field RX_40G_FIFO_OVF_I of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT */
        etrans_reg_RXFIFO_OVERFLOW_INT_STAT_action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       subfield_mask << (ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_40G_FIFO_OVF_I_OFF + subfield_offset),
                                                                       ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_40G_FIFO_OVF_I_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_RX_40G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_RX_40G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_40G_FIFO_OVF_I_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_40G_FIFO_OVF_I_get", stop_bit, 2 );
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
    /* (0x000000a4 bits 14:12) field RX_40G_FIFO_OVF_I of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT */
    reg_value = etrans_reg_RXFIFO_OVERFLOW_INT_STAT_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_40G_FIFO_OVF_I_MSK)
                  >> ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_40G_FIFO_OVF_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_40G_FIFO_OVF_I_MSK, ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_40G_FIFO_OVF_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_40G_FIFO_OVF_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_RX_40G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_RX_40G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_40G_FIFO_OVF_I_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_40G_FIFO_OVF_I_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_40G_FIFO_OVF_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000a4 bits 14:12) field RX_40G_FIFO_OVF_I of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000000a4 bits 14:12) field RX_40G_FIFO_OVF_I of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT */
        return etrans_reg_RXFIFO_OVERFLOW_INT_STAT_poll( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_40G_FIFO_OVF_I_OFF + subfield_offset),
                                                         value << (ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_40G_FIFO_OVF_I_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_40G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_40G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_RX_40G_FIFO_OVF_I_poll", value );

    /* (0x000000a4 bits 14:12) field RX_40G_FIFO_OVF_I of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT */
    return etrans_reg_RXFIFO_OVERFLOW_INT_STAT_poll( b_ptr,
                                                     h_ptr,
                                                     ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_40G_FIFO_OVF_I_MSK,
                                                     (value<<ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_40G_FIFO_OVF_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_RX_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_10G_FIFO_OVF_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "etrans_field_RX_10G_FIFO_OVF_I_set_to_clear", value );

    /* (0x000000a4 bits 11:0) field RX_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT */
    etrans_reg_RXFIFO_OVERFLOW_INT_STAT_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_10G_FIFO_OVF_I_MSK,
                                                                   ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_10G_FIFO_OVF_I_OFF,
                                                                   value);
}

static INLINE UINT32 etrans_field_RX_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a4 bits 11:0) field RX_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT */
    reg_value = etrans_reg_RXFIFO_OVERFLOW_INT_STAT_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_10G_FIFO_OVF_I_MSK) >> ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_10G_FIFO_OVF_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_10G_FIFO_OVF_I_get", value );

    return value;
}
static INLINE void etrans_field_range_RX_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_RX_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_10G_FIFO_OVF_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_10G_FIFO_OVF_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_10G_FIFO_OVF_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000a4 bits 11:0) field RX_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT */
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
        /* (0x000000a4 bits 11:0) field RX_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT */
        etrans_reg_RXFIFO_OVERFLOW_INT_STAT_action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       subfield_mask << (ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_10G_FIFO_OVF_I_OFF + subfield_offset),
                                                                       ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_10G_FIFO_OVF_I_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_RX_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_RX_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_10G_FIFO_OVF_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_10G_FIFO_OVF_I_get", stop_bit, 11 );
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
    /* (0x000000a4 bits 11:0) field RX_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT */
    reg_value = etrans_reg_RXFIFO_OVERFLOW_INT_STAT_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_10G_FIFO_OVF_I_MSK)
                  >> ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_10G_FIFO_OVF_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_10G_FIFO_OVF_I_MSK, ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_10G_FIFO_OVF_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_10G_FIFO_OVF_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_RX_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_RX_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_10G_FIFO_OVF_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_10G_FIFO_OVF_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_10G_FIFO_OVF_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000a4 bits 11:0) field RX_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT */
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
        /* (0x000000a4 bits 11:0) field RX_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT */
        return etrans_reg_RXFIFO_OVERFLOW_INT_STAT_poll( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_10G_FIFO_OVF_I_OFF + subfield_offset),
                                                         value << (ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_10G_FIFO_OVF_I_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_RX_10G_FIFO_OVF_I_poll", value );

    /* (0x000000a4 bits 11:0) field RX_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT */
    return etrans_reg_RXFIFO_OVERFLOW_INT_STAT_poll( b_ptr,
                                                     h_ptr,
                                                     ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_10G_FIFO_OVF_I_MSK,
                                                     (value<<ETRANS120_REG_RXFIFO_OVERFLOW_INT_STAT_BIT_RX_10G_FIFO_OVF_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_RX_100G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_100G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_100G_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_RX_100G_FIFO_UDR_I_set_to_clear", value );

    /* (0x000000a8 bits 15) field RX_100G_FIFO_UDR_I of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT */
    etrans_reg_RXFIFO_UNDERRUN_INT_STAT_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_100G_FIFO_UDR_I_MSK,
                                                                   ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_100G_FIFO_UDR_I_OFF,
                                                                   value);
}

static INLINE UINT32 etrans_field_RX_100G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_100G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a8 bits 15) field RX_100G_FIFO_UDR_I of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT */
    reg_value = etrans_reg_RXFIFO_UNDERRUN_INT_STAT_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_100G_FIFO_UDR_I_MSK) >> ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_100G_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_100G_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_100G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                         etrans_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_100G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                         etrans_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_RX_100G_FIFO_UDR_I_poll", value );

    /* (0x000000a8 bits 15) field RX_100G_FIFO_UDR_I of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT */
    return etrans_reg_RXFIFO_UNDERRUN_INT_STAT_poll( b_ptr,
                                                     h_ptr,
                                                     ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_100G_FIFO_UDR_I_MSK,
                                                     (value<<ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_100G_FIFO_UDR_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_RX_40G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_40G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_40G_FIFO_UDR_I_set_to_clear", value, 7);
    IOLOG( "%s <= 0x%08x", "etrans_field_RX_40G_FIFO_UDR_I_set_to_clear", value );

    /* (0x000000a8 bits 14:12) field RX_40G_FIFO_UDR_I of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT */
    etrans_reg_RXFIFO_UNDERRUN_INT_STAT_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_40G_FIFO_UDR_I_MSK,
                                                                   ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_40G_FIFO_UDR_I_OFF,
                                                                   value);
}

static INLINE UINT32 etrans_field_RX_40G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_40G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a8 bits 14:12) field RX_40G_FIFO_UDR_I of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT */
    reg_value = etrans_reg_RXFIFO_UNDERRUN_INT_STAT_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_40G_FIFO_UDR_I_MSK) >> ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_40G_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_40G_FIFO_UDR_I_get", value );

    return value;
}
static INLINE void etrans_field_range_RX_40G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_RX_40G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_40G_FIFO_UDR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_40G_FIFO_UDR_I_set_to_clear", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_40G_FIFO_UDR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000a8 bits 14:12) field RX_40G_FIFO_UDR_I of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT */
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
        /* (0x000000a8 bits 14:12) field RX_40G_FIFO_UDR_I of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT */
        etrans_reg_RXFIFO_UNDERRUN_INT_STAT_action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       subfield_mask << (ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_40G_FIFO_UDR_I_OFF + subfield_offset),
                                                                       ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_40G_FIFO_UDR_I_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_RX_40G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_RX_40G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_40G_FIFO_UDR_I_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_40G_FIFO_UDR_I_get", stop_bit, 2 );
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
    /* (0x000000a8 bits 14:12) field RX_40G_FIFO_UDR_I of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT */
    reg_value = etrans_reg_RXFIFO_UNDERRUN_INT_STAT_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_40G_FIFO_UDR_I_MSK)
                  >> ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_40G_FIFO_UDR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_40G_FIFO_UDR_I_MSK, ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_40G_FIFO_UDR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_40G_FIFO_UDR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_RX_40G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_RX_40G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_40G_FIFO_UDR_I_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_40G_FIFO_UDR_I_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_40G_FIFO_UDR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000a8 bits 14:12) field RX_40G_FIFO_UDR_I of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000000a8 bits 14:12) field RX_40G_FIFO_UDR_I of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT */
        return etrans_reg_RXFIFO_UNDERRUN_INT_STAT_poll( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_40G_FIFO_UDR_I_OFF + subfield_offset),
                                                         value << (ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_40G_FIFO_UDR_I_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_40G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_40G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_RX_40G_FIFO_UDR_I_poll", value );

    /* (0x000000a8 bits 14:12) field RX_40G_FIFO_UDR_I of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT */
    return etrans_reg_RXFIFO_UNDERRUN_INT_STAT_poll( b_ptr,
                                                     h_ptr,
                                                     ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_40G_FIFO_UDR_I_MSK,
                                                     (value<<ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_40G_FIFO_UDR_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_RX_10G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_RX_10G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_RX_10G_FIFO_UDR_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "etrans_field_RX_10G_FIFO_UDR_I_set_to_clear", value );

    /* (0x000000a8 bits 11:0) field RX_10G_FIFO_UDR_I of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT */
    etrans_reg_RXFIFO_UNDERRUN_INT_STAT_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_10G_FIFO_UDR_I_MSK,
                                                                   ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_10G_FIFO_UDR_I_OFF,
                                                                   value);
}

static INLINE UINT32 etrans_field_RX_10G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_10G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000a8 bits 11:0) field RX_10G_FIFO_UDR_I of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT */
    reg_value = etrans_reg_RXFIFO_UNDERRUN_INT_STAT_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_10G_FIFO_UDR_I_MSK) >> ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_10G_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_10G_FIFO_UDR_I_get", value );

    return value;
}
static INLINE void etrans_field_range_RX_10G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_RX_10G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_10G_FIFO_UDR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_10G_FIFO_UDR_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_10G_FIFO_UDR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000a8 bits 11:0) field RX_10G_FIFO_UDR_I of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT */
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
        /* (0x000000a8 bits 11:0) field RX_10G_FIFO_UDR_I of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT */
        etrans_reg_RXFIFO_UNDERRUN_INT_STAT_action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       subfield_mask << (ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_10G_FIFO_UDR_I_OFF + subfield_offset),
                                                                       ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_10G_FIFO_UDR_I_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_RX_10G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_RX_10G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_10G_FIFO_UDR_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_10G_FIFO_UDR_I_get", stop_bit, 11 );
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
    /* (0x000000a8 bits 11:0) field RX_10G_FIFO_UDR_I of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT */
    reg_value = etrans_reg_RXFIFO_UNDERRUN_INT_STAT_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_10G_FIFO_UDR_I_MSK)
                  >> ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_10G_FIFO_UDR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_10G_FIFO_UDR_I_MSK, ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_10G_FIFO_UDR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_10G_FIFO_UDR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_RX_10G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_RX_10G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_10G_FIFO_UDR_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_10G_FIFO_UDR_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_10G_FIFO_UDR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000a8 bits 11:0) field RX_10G_FIFO_UDR_I of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT */
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
        /* (0x000000a8 bits 11:0) field RX_10G_FIFO_UDR_I of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT */
        return etrans_reg_RXFIFO_UNDERRUN_INT_STAT_poll( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_10G_FIFO_UDR_I_OFF + subfield_offset),
                                                         value << (ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_10G_FIFO_UDR_I_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_10G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_10G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_RX_10G_FIFO_UDR_I_poll", value );

    /* (0x000000a8 bits 11:0) field RX_10G_FIFO_UDR_I of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT */
    return etrans_reg_RXFIFO_UNDERRUN_INT_STAT_poll( b_ptr,
                                                     h_ptr,
                                                     ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_10G_FIFO_UDR_I_MSK,
                                                     (value<<ETRANS120_REG_RXFIFO_UNDERRUN_INT_STAT_BIT_RX_10G_FIFO_UDR_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_TX_100G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_100G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_100G_FIFO_OVF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_100G_FIFO_OVF_I_set_to_clear", value );

    /* (0x00000114 bits 15) field TX_100G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    etrans_reg_TX_FIFO_OVF_INT_STAT_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_100G_FIFO_OVF_I_MSK,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_100G_FIFO_OVF_I_OFF,
                                                               value);
}

static INLINE UINT32 etrans_field_TX_100G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_100G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000114 bits 15) field TX_100G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_STAT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_100G_FIFO_OVF_I_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_100G_FIFO_OVF_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_100G_FIFO_OVF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_100G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                         etrans_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_100G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                         etrans_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_100G_FIFO_OVF_I_poll", value );

    /* (0x00000114 bits 15) field TX_100G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    return etrans_reg_TX_FIFO_OVF_INT_STAT_poll( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_100G_FIFO_OVF_I_MSK,
                                                 (value<<ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_100G_FIFO_OVF_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_TX_40G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_40G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_40G_FIFO_OVF_I_set_to_clear", value, 7);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_40G_FIFO_OVF_I_set_to_clear", value );

    /* (0x00000114 bits 14:12) field TX_40G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    etrans_reg_TX_FIFO_OVF_INT_STAT_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_40G_FIFO_OVF_I_MSK,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_40G_FIFO_OVF_I_OFF,
                                                               value);
}

static INLINE UINT32 etrans_field_TX_40G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_40G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000114 bits 14:12) field TX_40G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_STAT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_40G_FIFO_OVF_I_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_40G_FIFO_OVF_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_40G_FIFO_OVF_I_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_40G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_40G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_40G_FIFO_OVF_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_40G_FIFO_OVF_I_set_to_clear", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_40G_FIFO_OVF_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000114 bits 14:12) field TX_40G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
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
        /* (0x00000114 bits 14:12) field TX_40G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
        etrans_reg_TX_FIFO_OVF_INT_STAT_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   subfield_mask << (ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_40G_FIFO_OVF_I_OFF + subfield_offset),
                                                                   ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_40G_FIFO_OVF_I_OFF + subfield_offset,
                                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_40G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_40G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_40G_FIFO_OVF_I_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_40G_FIFO_OVF_I_get", stop_bit, 2 );
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
    /* (0x00000114 bits 14:12) field TX_40G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_STAT_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_40G_FIFO_OVF_I_MSK)
                  >> ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_40G_FIFO_OVF_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_40G_FIFO_OVF_I_MSK, ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_40G_FIFO_OVF_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_40G_FIFO_OVF_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_TX_40G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_TX_40G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_40G_FIFO_OVF_I_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_40G_FIFO_OVF_I_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_40G_FIFO_OVF_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000114 bits 14:12) field TX_40G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000114 bits 14:12) field TX_40G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
        return etrans_reg_TX_FIFO_OVF_INT_STAT_poll( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_40G_FIFO_OVF_I_OFF + subfield_offset),
                                                     value << (ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_40G_FIFO_OVF_I_OFF + subfield_offset),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_40G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_40G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_40G_FIFO_OVF_I_poll", value );

    /* (0x00000114 bits 14:12) field TX_40G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    return etrans_reg_TX_FIFO_OVF_INT_STAT_poll( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_40G_FIFO_OVF_I_MSK,
                                                 (value<<ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_40G_FIFO_OVF_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_TX_LINK11_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                       etrans_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK11_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                       etrans_handle_t *h_ptr,
                                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK11_10G_FIFO_OVF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK11_10G_FIFO_OVF_I_set_to_clear", value );

    /* (0x00000114 bits 11) field TX_LINK11_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    etrans_reg_TX_FIFO_OVF_INT_STAT_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK11_10G_FIFO_OVF_I_MSK,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK11_10G_FIFO_OVF_I_OFF,
                                                               value);
}

static INLINE UINT32 etrans_field_TX_LINK11_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK11_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000114 bits 11) field TX_LINK11_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_STAT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK11_10G_FIFO_OVF_I_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK11_10G_FIFO_OVF_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK11_10G_FIFO_OVF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK11_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                               etrans_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK11_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                               etrans_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK11_10G_FIFO_OVF_I_poll", value );

    /* (0x00000114 bits 11) field TX_LINK11_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    return etrans_reg_TX_FIFO_OVF_INT_STAT_poll( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK11_10G_FIFO_OVF_I_MSK,
                                                 (value<<ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK11_10G_FIFO_OVF_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_TX_LINK10_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                       etrans_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK10_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                       etrans_handle_t *h_ptr,
                                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK10_10G_FIFO_OVF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK10_10G_FIFO_OVF_I_set_to_clear", value );

    /* (0x00000114 bits 10) field TX_LINK10_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    etrans_reg_TX_FIFO_OVF_INT_STAT_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK10_10G_FIFO_OVF_I_MSK,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK10_10G_FIFO_OVF_I_OFF,
                                                               value);
}

static INLINE UINT32 etrans_field_TX_LINK10_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK10_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000114 bits 10) field TX_LINK10_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_STAT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK10_10G_FIFO_OVF_I_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK10_10G_FIFO_OVF_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK10_10G_FIFO_OVF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK10_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                               etrans_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK10_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                               etrans_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK10_10G_FIFO_OVF_I_poll", value );

    /* (0x00000114 bits 10) field TX_LINK10_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    return etrans_reg_TX_FIFO_OVF_INT_STAT_poll( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK10_10G_FIFO_OVF_I_MSK,
                                                 (value<<ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK10_10G_FIFO_OVF_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_TX_LINK9_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK9_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK9_10G_FIFO_OVF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK9_10G_FIFO_OVF_I_set_to_clear", value );

    /* (0x00000114 bits 9) field TX_LINK9_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    etrans_reg_TX_FIFO_OVF_INT_STAT_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK9_10G_FIFO_OVF_I_MSK,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK9_10G_FIFO_OVF_I_OFF,
                                                               value);
}

static INLINE UINT32 etrans_field_TX_LINK9_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK9_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000114 bits 9) field TX_LINK9_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_STAT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK9_10G_FIFO_OVF_I_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK9_10G_FIFO_OVF_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK9_10G_FIFO_OVF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK9_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK9_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK9_10G_FIFO_OVF_I_poll", value );

    /* (0x00000114 bits 9) field TX_LINK9_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    return etrans_reg_TX_FIFO_OVF_INT_STAT_poll( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK9_10G_FIFO_OVF_I_MSK,
                                                 (value<<ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK9_10G_FIFO_OVF_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_TX_LINK8_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK8_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK8_10G_FIFO_OVF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK8_10G_FIFO_OVF_I_set_to_clear", value );

    /* (0x00000114 bits 8) field TX_LINK8_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    etrans_reg_TX_FIFO_OVF_INT_STAT_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK8_10G_FIFO_OVF_I_MSK,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK8_10G_FIFO_OVF_I_OFF,
                                                               value);
}

static INLINE UINT32 etrans_field_TX_LINK8_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK8_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000114 bits 8) field TX_LINK8_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_STAT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK8_10G_FIFO_OVF_I_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK8_10G_FIFO_OVF_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK8_10G_FIFO_OVF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK8_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK8_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK8_10G_FIFO_OVF_I_poll", value );

    /* (0x00000114 bits 8) field TX_LINK8_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    return etrans_reg_TX_FIFO_OVF_INT_STAT_poll( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK8_10G_FIFO_OVF_I_MSK,
                                                 (value<<ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK8_10G_FIFO_OVF_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_TX_LINK7_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK7_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK7_10G_FIFO_OVF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK7_10G_FIFO_OVF_I_set_to_clear", value );

    /* (0x00000114 bits 7) field TX_LINK7_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    etrans_reg_TX_FIFO_OVF_INT_STAT_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK7_10G_FIFO_OVF_I_MSK,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK7_10G_FIFO_OVF_I_OFF,
                                                               value);
}

static INLINE UINT32 etrans_field_TX_LINK7_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK7_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000114 bits 7) field TX_LINK7_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_STAT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK7_10G_FIFO_OVF_I_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK7_10G_FIFO_OVF_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK7_10G_FIFO_OVF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK7_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK7_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK7_10G_FIFO_OVF_I_poll", value );

    /* (0x00000114 bits 7) field TX_LINK7_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    return etrans_reg_TX_FIFO_OVF_INT_STAT_poll( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK7_10G_FIFO_OVF_I_MSK,
                                                 (value<<ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK7_10G_FIFO_OVF_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_TX_LINK6_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK6_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK6_10G_FIFO_OVF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK6_10G_FIFO_OVF_I_set_to_clear", value );

    /* (0x00000114 bits 6) field TX_LINK6_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    etrans_reg_TX_FIFO_OVF_INT_STAT_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK6_10G_FIFO_OVF_I_MSK,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK6_10G_FIFO_OVF_I_OFF,
                                                               value);
}

static INLINE UINT32 etrans_field_TX_LINK6_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK6_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000114 bits 6) field TX_LINK6_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_STAT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK6_10G_FIFO_OVF_I_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK6_10G_FIFO_OVF_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK6_10G_FIFO_OVF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK6_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK6_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK6_10G_FIFO_OVF_I_poll", value );

    /* (0x00000114 bits 6) field TX_LINK6_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    return etrans_reg_TX_FIFO_OVF_INT_STAT_poll( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK6_10G_FIFO_OVF_I_MSK,
                                                 (value<<ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK6_10G_FIFO_OVF_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_TX_LINK5_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK5_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK5_10G_FIFO_OVF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK5_10G_FIFO_OVF_I_set_to_clear", value );

    /* (0x00000114 bits 5) field TX_LINK5_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    etrans_reg_TX_FIFO_OVF_INT_STAT_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK5_10G_FIFO_OVF_I_MSK,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK5_10G_FIFO_OVF_I_OFF,
                                                               value);
}

static INLINE UINT32 etrans_field_TX_LINK5_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK5_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000114 bits 5) field TX_LINK5_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_STAT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK5_10G_FIFO_OVF_I_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK5_10G_FIFO_OVF_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK5_10G_FIFO_OVF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK5_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK5_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK5_10G_FIFO_OVF_I_poll", value );

    /* (0x00000114 bits 5) field TX_LINK5_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    return etrans_reg_TX_FIFO_OVF_INT_STAT_poll( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK5_10G_FIFO_OVF_I_MSK,
                                                 (value<<ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK5_10G_FIFO_OVF_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_TX_LINK4_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK4_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK4_10G_FIFO_OVF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK4_10G_FIFO_OVF_I_set_to_clear", value );

    /* (0x00000114 bits 4) field TX_LINK4_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    etrans_reg_TX_FIFO_OVF_INT_STAT_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK4_10G_FIFO_OVF_I_MSK,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK4_10G_FIFO_OVF_I_OFF,
                                                               value);
}

static INLINE UINT32 etrans_field_TX_LINK4_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK4_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000114 bits 4) field TX_LINK4_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_STAT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK4_10G_FIFO_OVF_I_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK4_10G_FIFO_OVF_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK4_10G_FIFO_OVF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK4_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK4_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK4_10G_FIFO_OVF_I_poll", value );

    /* (0x00000114 bits 4) field TX_LINK4_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    return etrans_reg_TX_FIFO_OVF_INT_STAT_poll( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK4_10G_FIFO_OVF_I_MSK,
                                                 (value<<ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK4_10G_FIFO_OVF_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_TX_LINK3_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK3_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK3_10G_FIFO_OVF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK3_10G_FIFO_OVF_I_set_to_clear", value );

    /* (0x00000114 bits 3) field TX_LINK3_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    etrans_reg_TX_FIFO_OVF_INT_STAT_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK3_10G_FIFO_OVF_I_MSK,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK3_10G_FIFO_OVF_I_OFF,
                                                               value);
}

static INLINE UINT32 etrans_field_TX_LINK3_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK3_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000114 bits 3) field TX_LINK3_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_STAT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK3_10G_FIFO_OVF_I_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK3_10G_FIFO_OVF_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK3_10G_FIFO_OVF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK3_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK3_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK3_10G_FIFO_OVF_I_poll", value );

    /* (0x00000114 bits 3) field TX_LINK3_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    return etrans_reg_TX_FIFO_OVF_INT_STAT_poll( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK3_10G_FIFO_OVF_I_MSK,
                                                 (value<<ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK3_10G_FIFO_OVF_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_TX_LINK2_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK2_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK2_10G_FIFO_OVF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK2_10G_FIFO_OVF_I_set_to_clear", value );

    /* (0x00000114 bits 2) field TX_LINK2_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    etrans_reg_TX_FIFO_OVF_INT_STAT_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK2_10G_FIFO_OVF_I_MSK,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK2_10G_FIFO_OVF_I_OFF,
                                                               value);
}

static INLINE UINT32 etrans_field_TX_LINK2_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK2_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000114 bits 2) field TX_LINK2_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_STAT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK2_10G_FIFO_OVF_I_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK2_10G_FIFO_OVF_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK2_10G_FIFO_OVF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK2_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK2_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK2_10G_FIFO_OVF_I_poll", value );

    /* (0x00000114 bits 2) field TX_LINK2_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    return etrans_reg_TX_FIFO_OVF_INT_STAT_poll( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK2_10G_FIFO_OVF_I_MSK,
                                                 (value<<ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK2_10G_FIFO_OVF_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_TX_LINK1_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK1_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK1_10G_FIFO_OVF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK1_10G_FIFO_OVF_I_set_to_clear", value );

    /* (0x00000114 bits 1) field TX_LINK1_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    etrans_reg_TX_FIFO_OVF_INT_STAT_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK1_10G_FIFO_OVF_I_MSK,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK1_10G_FIFO_OVF_I_OFF,
                                                               value);
}

static INLINE UINT32 etrans_field_TX_LINK1_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK1_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000114 bits 1) field TX_LINK1_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_STAT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK1_10G_FIFO_OVF_I_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK1_10G_FIFO_OVF_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK1_10G_FIFO_OVF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK1_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK1_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK1_10G_FIFO_OVF_I_poll", value );

    /* (0x00000114 bits 1) field TX_LINK1_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    return etrans_reg_TX_FIFO_OVF_INT_STAT_poll( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK1_10G_FIFO_OVF_I_MSK,
                                                 (value<<ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK1_10G_FIFO_OVF_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_TX_LINK0_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_LINK0_10G_FIFO_OVF_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_LINK0_10G_FIFO_OVF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_LINK0_10G_FIFO_OVF_I_set_to_clear", value );

    /* (0x00000114 bits 0) field TX_LINK0_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    etrans_reg_TX_FIFO_OVF_INT_STAT_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK0_10G_FIFO_OVF_I_MSK,
                                                               ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK0_10G_FIFO_OVF_I_OFF,
                                                               value);
}

static INLINE UINT32 etrans_field_TX_LINK0_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK0_10G_FIFO_OVF_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000114 bits 0) field TX_LINK0_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_STAT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK0_10G_FIFO_OVF_I_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK0_10G_FIFO_OVF_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK0_10G_FIFO_OVF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK0_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK0_10G_FIFO_OVF_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK0_10G_FIFO_OVF_I_poll", value );

    /* (0x00000114 bits 0) field TX_LINK0_10G_FIFO_OVF_I of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_STAT */
    return etrans_reg_TX_FIFO_OVF_INT_STAT_poll( b_ptr,
                                                 h_ptr,
                                                 ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK0_10G_FIFO_OVF_I_MSK,
                                                 (value<<ETRANS120_REG_TX_FIFO_OVF_INT_STAT_BIT_TX_LINK0_10G_FIFO_OVF_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_TX_100G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_100G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                 etrans_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_100G_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_100G_FIFO_UDR_I_set_to_clear", value );

    /* (0x00000118 bits 15) field TX_100G_FIFO_UDR_I of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT */
    etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_100G_FIFO_UDR_I_MSK,
                                                                    ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_100G_FIFO_UDR_I_OFF,
                                                                    value);
}

static INLINE UINT32 etrans_field_TX_100G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_100G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000118 bits 15) field TX_100G_FIFO_UDR_I of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_100G_FIFO_UDR_I_MSK) >> ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_100G_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_100G_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_100G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                         etrans_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_100G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                         etrans_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_100G_FIFO_UDR_I_poll", value );

    /* (0x00000118 bits 15) field TX_100G_FIFO_UDR_I of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT */
    return etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_poll( b_ptr,
                                                      h_ptr,
                                                      ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_100G_FIFO_UDR_I_MSK,
                                                      (value<<ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_100G_FIFO_UDR_I_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_TX_40G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_40G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_40G_FIFO_UDR_I_set_to_clear", value, 7);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_40G_FIFO_UDR_I_set_to_clear", value );

    /* (0x00000118 bits 14:12) field TX_40G_FIFO_UDR_I of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT */
    etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_40G_FIFO_UDR_I_MSK,
                                                                    ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_40G_FIFO_UDR_I_OFF,
                                                                    value);
}

static INLINE UINT32 etrans_field_TX_40G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_40G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000118 bits 14:12) field TX_40G_FIFO_UDR_I of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_40G_FIFO_UDR_I_MSK) >> ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_40G_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_40G_FIFO_UDR_I_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_40G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_40G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_40G_FIFO_UDR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_40G_FIFO_UDR_I_set_to_clear", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_40G_FIFO_UDR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000118 bits 14:12) field TX_40G_FIFO_UDR_I of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT */
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
        /* (0x00000118 bits 14:12) field TX_40G_FIFO_UDR_I of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT */
        etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_action_on_write_field_set( b_ptr,
                                                                        h_ptr,
                                                                        subfield_mask << (ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_40G_FIFO_UDR_I_OFF + subfield_offset),
                                                                        ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_40G_FIFO_UDR_I_OFF + subfield_offset,
                                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_40G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_40G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_40G_FIFO_UDR_I_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_40G_FIFO_UDR_I_get", stop_bit, 2 );
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
    /* (0x00000118 bits 14:12) field TX_40G_FIFO_UDR_I of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_40G_FIFO_UDR_I_MSK)
                  >> ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_40G_FIFO_UDR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_40G_FIFO_UDR_I_MSK, ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_40G_FIFO_UDR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_40G_FIFO_UDR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_TX_40G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_TX_40G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_40G_FIFO_UDR_I_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_40G_FIFO_UDR_I_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_40G_FIFO_UDR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000118 bits 14:12) field TX_40G_FIFO_UDR_I of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000118 bits 14:12) field TX_40G_FIFO_UDR_I of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT */
        return etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_poll( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_40G_FIFO_UDR_I_OFF + subfield_offset),
                                                          value << (ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_40G_FIFO_UDR_I_OFF + subfield_offset),
                                                          cmp,
                                                          max_count,
                                                          num_failed_polls,
                                                          delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_40G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_40G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_40G_FIFO_UDR_I_poll", value );

    /* (0x00000118 bits 14:12) field TX_40G_FIFO_UDR_I of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT */
    return etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_poll( b_ptr,
                                                      h_ptr,
                                                      ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_40G_FIFO_UDR_I_MSK,
                                                      (value<<ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_40G_FIFO_UDR_I_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE void etrans_field_TX_10G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_TX_10G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                etrans_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "etrans_field_TX_10G_FIFO_UDR_I_set_to_clear", value, 4095);
    IOLOG( "%s <= 0x%08x", "etrans_field_TX_10G_FIFO_UDR_I_set_to_clear", value );

    /* (0x00000118 bits 11:0) field TX_10G_FIFO_UDR_I of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT */
    etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_10G_FIFO_UDR_I_MSK,
                                                                    ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_10G_FIFO_UDR_I_OFF,
                                                                    value);
}

static INLINE UINT32 etrans_field_TX_10G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_10G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000118 bits 11:0) field TX_10G_FIFO_UDR_I of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_10G_FIFO_UDR_I_MSK) >> ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_10G_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_10G_FIFO_UDR_I_get", value );

    return value;
}
static INLINE void etrans_field_range_TX_10G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void etrans_field_range_TX_10G_FIFO_UDR_I_set_to_clear( etrans_buffer_t *b_ptr,
                                                                      etrans_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_10G_FIFO_UDR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_10G_FIFO_UDR_I_set_to_clear", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_10G_FIFO_UDR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000118 bits 11:0) field TX_10G_FIFO_UDR_I of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT */
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
        /* (0x00000118 bits 11:0) field TX_10G_FIFO_UDR_I of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT */
        etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_action_on_write_field_set( b_ptr,
                                                                        h_ptr,
                                                                        subfield_mask << (ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_10G_FIFO_UDR_I_OFF + subfield_offset),
                                                                        ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_10G_FIFO_UDR_I_OFF + subfield_offset,
                                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 etrans_field_range_TX_10G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_10G_FIFO_UDR_I_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_10G_FIFO_UDR_I_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_10G_FIFO_UDR_I_get", stop_bit, 11 );
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
    /* (0x00000118 bits 11:0) field TX_10G_FIFO_UDR_I of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT */
    reg_value = etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_10G_FIFO_UDR_I_MSK)
                  >> ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_10G_FIFO_UDR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_10G_FIFO_UDR_I_MSK, ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_10G_FIFO_UDR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_10G_FIFO_UDR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_TX_10G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_TX_10G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_10G_FIFO_UDR_I_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_10G_FIFO_UDR_I_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_10G_FIFO_UDR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000118 bits 11:0) field TX_10G_FIFO_UDR_I of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT */
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
        /* (0x00000118 bits 11:0) field TX_10G_FIFO_UDR_I of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT */
        return etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_poll( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_10G_FIFO_UDR_I_OFF + subfield_offset),
                                                          value << (ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_10G_FIFO_UDR_I_OFF + subfield_offset),
                                                          cmp,
                                                          max_count,
                                                          num_failed_polls,
                                                          delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_10G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_10G_FIFO_UDR_I_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_10G_FIFO_UDR_I_poll", value );

    /* (0x00000118 bits 11:0) field TX_10G_FIFO_UDR_I of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT */
    return etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_poll( b_ptr,
                                                      h_ptr,
                                                      ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_10G_FIFO_UDR_I_MSK,
                                                      (value<<ETRANS120_REG_TX_FIFO_UNDERRUN_INT_STAT_BIT_TX_10G_FIFO_UDR_I_OFF),
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
static INLINE UINT32 etrans_field_RX_100G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_100G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b4 bits 15) field RX_100G_FIFO_OVF_V of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL */
    reg_value = etrans_reg_RXFIFO_OVERFLOW_INT_VAL_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_100G_FIFO_OVF_V_MSK) >> ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_100G_FIFO_OVF_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_100G_FIFO_OVF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_100G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                         etrans_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_100G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                         etrans_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_RX_100G_FIFO_OVF_V_poll", value );

    /* (0x000000b4 bits 15) field RX_100G_FIFO_OVF_V of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL */
    return etrans_reg_RXFIFO_OVERFLOW_INT_VAL_poll( b_ptr,
                                                    h_ptr,
                                                    ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_100G_FIFO_OVF_V_MSK,
                                                    (value<<ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_100G_FIFO_OVF_V_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_RX_40G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_40G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b4 bits 14:12) field RX_40G_FIFO_OVF_V of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL */
    reg_value = etrans_reg_RXFIFO_OVERFLOW_INT_VAL_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_40G_FIFO_OVF_V_MSK) >> ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_40G_FIFO_OVF_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_40G_FIFO_OVF_V_get", value );

    return value;
}
static INLINE UINT32 etrans_field_range_RX_40G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_RX_40G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_40G_FIFO_OVF_V_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_40G_FIFO_OVF_V_get", stop_bit, 2 );
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
    /* (0x000000b4 bits 14:12) field RX_40G_FIFO_OVF_V of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL */
    reg_value = etrans_reg_RXFIFO_OVERFLOW_INT_VAL_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_40G_FIFO_OVF_V_MSK)
                  >> ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_40G_FIFO_OVF_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_40G_FIFO_OVF_V_MSK, ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_40G_FIFO_OVF_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_40G_FIFO_OVF_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_RX_40G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_RX_40G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_40G_FIFO_OVF_V_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_40G_FIFO_OVF_V_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_40G_FIFO_OVF_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000b4 bits 14:12) field RX_40G_FIFO_OVF_V of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000000b4 bits 14:12) field RX_40G_FIFO_OVF_V of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL */
        return etrans_reg_RXFIFO_OVERFLOW_INT_VAL_poll( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_40G_FIFO_OVF_V_OFF + subfield_offset),
                                                        value << (ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_40G_FIFO_OVF_V_OFF + subfield_offset),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_40G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_40G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_RX_40G_FIFO_OVF_V_poll", value );

    /* (0x000000b4 bits 14:12) field RX_40G_FIFO_OVF_V of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL */
    return etrans_reg_RXFIFO_OVERFLOW_INT_VAL_poll( b_ptr,
                                                    h_ptr,
                                                    ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_40G_FIFO_OVF_V_MSK,
                                                    (value<<ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_40G_FIFO_OVF_V_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_RX_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b4 bits 11:0) field RX_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL */
    reg_value = etrans_reg_RXFIFO_OVERFLOW_INT_VAL_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_10G_FIFO_OVF_V_MSK) >> ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_10G_FIFO_OVF_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_10G_FIFO_OVF_V_get", value );

    return value;
}
static INLINE UINT32 etrans_field_range_RX_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_RX_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_10G_FIFO_OVF_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_10G_FIFO_OVF_V_get", stop_bit, 11 );
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
    /* (0x000000b4 bits 11:0) field RX_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL */
    reg_value = etrans_reg_RXFIFO_OVERFLOW_INT_VAL_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_10G_FIFO_OVF_V_MSK)
                  >> ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_10G_FIFO_OVF_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_10G_FIFO_OVF_V_MSK, ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_10G_FIFO_OVF_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_10G_FIFO_OVF_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_RX_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_RX_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_10G_FIFO_OVF_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_10G_FIFO_OVF_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_10G_FIFO_OVF_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000b4 bits 11:0) field RX_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL */
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
        /* (0x000000b4 bits 11:0) field RX_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL */
        return etrans_reg_RXFIFO_OVERFLOW_INT_VAL_poll( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_10G_FIFO_OVF_V_OFF + subfield_offset),
                                                        value << (ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_10G_FIFO_OVF_V_OFF + subfield_offset),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_RX_10G_FIFO_OVF_V_poll", value );

    /* (0x000000b4 bits 11:0) field RX_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL */
    return etrans_reg_RXFIFO_OVERFLOW_INT_VAL_poll( b_ptr,
                                                    h_ptr,
                                                    ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_10G_FIFO_OVF_V_MSK,
                                                    (value<<ETRANS120_REG_RXFIFO_OVERFLOW_INT_VAL_BIT_RX_10G_FIFO_OVF_V_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_RX_100G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_100G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b8 bits 15) field RX_100G_FIFO_UDR_V of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL */
    reg_value = etrans_reg_RXFIFO_UNDERRUN_INT_VAL_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_100G_FIFO_UDR_V_MSK) >> ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_100G_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_100G_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_100G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                         etrans_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_100G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                         etrans_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_RX_100G_FIFO_UDR_V_poll", value );

    /* (0x000000b8 bits 15) field RX_100G_FIFO_UDR_V of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL */
    return etrans_reg_RXFIFO_UNDERRUN_INT_VAL_poll( b_ptr,
                                                    h_ptr,
                                                    ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_100G_FIFO_UDR_V_MSK,
                                                    (value<<ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_100G_FIFO_UDR_V_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_RX_40G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_40G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b8 bits 14:12) field RX_40G_FIFO_UDR_V of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL */
    reg_value = etrans_reg_RXFIFO_UNDERRUN_INT_VAL_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_40G_FIFO_UDR_V_MSK) >> ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_40G_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_40G_FIFO_UDR_V_get", value );

    return value;
}
static INLINE UINT32 etrans_field_range_RX_40G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_RX_40G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_40G_FIFO_UDR_V_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_40G_FIFO_UDR_V_get", stop_bit, 2 );
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
    /* (0x000000b8 bits 14:12) field RX_40G_FIFO_UDR_V of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL */
    reg_value = etrans_reg_RXFIFO_UNDERRUN_INT_VAL_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_40G_FIFO_UDR_V_MSK)
                  >> ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_40G_FIFO_UDR_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_40G_FIFO_UDR_V_MSK, ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_40G_FIFO_UDR_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_40G_FIFO_UDR_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_RX_40G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_RX_40G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_40G_FIFO_UDR_V_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_40G_FIFO_UDR_V_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_40G_FIFO_UDR_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000b8 bits 14:12) field RX_40G_FIFO_UDR_V of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000000b8 bits 14:12) field RX_40G_FIFO_UDR_V of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL */
        return etrans_reg_RXFIFO_UNDERRUN_INT_VAL_poll( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_40G_FIFO_UDR_V_OFF + subfield_offset),
                                                        value << (ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_40G_FIFO_UDR_V_OFF + subfield_offset),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_40G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_40G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_RX_40G_FIFO_UDR_V_poll", value );

    /* (0x000000b8 bits 14:12) field RX_40G_FIFO_UDR_V of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL */
    return etrans_reg_RXFIFO_UNDERRUN_INT_VAL_poll( b_ptr,
                                                    h_ptr,
                                                    ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_40G_FIFO_UDR_V_MSK,
                                                    (value<<ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_40G_FIFO_UDR_V_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_RX_10G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_RX_10G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000000b8 bits 11:0) field RX_10G_FIFO_UDR_V of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL */
    reg_value = etrans_reg_RXFIFO_UNDERRUN_INT_VAL_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_10G_FIFO_UDR_V_MSK) >> ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_10G_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_RX_10G_FIFO_UDR_V_get", value );

    return value;
}
static INLINE UINT32 etrans_field_range_RX_10G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_RX_10G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_10G_FIFO_UDR_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_10G_FIFO_UDR_V_get", stop_bit, 11 );
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
    /* (0x000000b8 bits 11:0) field RX_10G_FIFO_UDR_V of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL */
    reg_value = etrans_reg_RXFIFO_UNDERRUN_INT_VAL_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_10G_FIFO_UDR_V_MSK)
                  >> ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_10G_FIFO_UDR_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_10G_FIFO_UDR_V_MSK, ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_10G_FIFO_UDR_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_10G_FIFO_UDR_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_RX_10G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_RX_10G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_RX_10G_FIFO_UDR_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_RX_10G_FIFO_UDR_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_RX_10G_FIFO_UDR_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000b8 bits 11:0) field RX_10G_FIFO_UDR_V of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL */
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
        /* (0x000000b8 bits 11:0) field RX_10G_FIFO_UDR_V of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL */
        return etrans_reg_RXFIFO_UNDERRUN_INT_VAL_poll( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_10G_FIFO_UDR_V_OFF + subfield_offset),
                                                        value << (ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_10G_FIFO_UDR_V_OFF + subfield_offset),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_10G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_RX_10G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_RX_10G_FIFO_UDR_V_poll", value );

    /* (0x000000b8 bits 11:0) field RX_10G_FIFO_UDR_V of register PMC_ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL */
    return etrans_reg_RXFIFO_UNDERRUN_INT_VAL_poll( b_ptr,
                                                    h_ptr,
                                                    ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_10G_FIFO_UDR_V_MSK,
                                                    (value<<ETRANS120_REG_RXFIFO_UNDERRUN_INT_VAL_BIT_RX_10G_FIFO_UDR_V_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_TX_100G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_100G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000124 bits 15) field TX_100G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_100G_FIFO_OVF_V_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_100G_FIFO_OVF_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_100G_FIFO_OVF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_100G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                         etrans_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_100G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                         etrans_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_100G_FIFO_OVF_V_poll", value );

    /* (0x00000124 bits 15) field TX_100G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    return etrans_reg_TX_FIFO_OVF_INT_VAL_poll( b_ptr,
                                                h_ptr,
                                                ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_100G_FIFO_OVF_V_MSK,
                                                (value<<ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_100G_FIFO_OVF_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_TX_40G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_40G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000124 bits 14:12) field TX_40G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_40G_FIFO_OVF_V_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_40G_FIFO_OVF_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_40G_FIFO_OVF_V_get", value );

    return value;
}
static INLINE UINT32 etrans_field_range_TX_40G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_40G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_40G_FIFO_OVF_V_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_40G_FIFO_OVF_V_get", stop_bit, 2 );
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
    /* (0x00000124 bits 14:12) field TX_40G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_VAL_read(  b_ptr, h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_40G_FIFO_OVF_V_MSK)
                  >> ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_40G_FIFO_OVF_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_40G_FIFO_OVF_V_MSK, ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_40G_FIFO_OVF_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_40G_FIFO_OVF_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_TX_40G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_TX_40G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_40G_FIFO_OVF_V_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_40G_FIFO_OVF_V_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_40G_FIFO_OVF_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000124 bits 14:12) field TX_40G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000124 bits 14:12) field TX_40G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
        return etrans_reg_TX_FIFO_OVF_INT_VAL_poll( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_40G_FIFO_OVF_V_OFF + subfield_offset),
                                                    value << (ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_40G_FIFO_OVF_V_OFF + subfield_offset),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_40G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_40G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_40G_FIFO_OVF_V_poll", value );

    /* (0x00000124 bits 14:12) field TX_40G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    return etrans_reg_TX_FIFO_OVF_INT_VAL_poll( b_ptr,
                                                h_ptr,
                                                ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_40G_FIFO_OVF_V_MSK,
                                                (value<<ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_40G_FIFO_OVF_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_TX_LINK11_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK11_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000124 bits 11) field TX_LINK11_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK11_10G_FIFO_OVF_V_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK11_10G_FIFO_OVF_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK11_10G_FIFO_OVF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK11_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                               etrans_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK11_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                               etrans_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK11_10G_FIFO_OVF_V_poll", value );

    /* (0x00000124 bits 11) field TX_LINK11_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    return etrans_reg_TX_FIFO_OVF_INT_VAL_poll( b_ptr,
                                                h_ptr,
                                                ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK11_10G_FIFO_OVF_V_MSK,
                                                (value<<ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK11_10G_FIFO_OVF_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_TX_LINK10_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK10_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000124 bits 10) field TX_LINK10_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK10_10G_FIFO_OVF_V_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK10_10G_FIFO_OVF_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK10_10G_FIFO_OVF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK10_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                               etrans_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK10_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                               etrans_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK10_10G_FIFO_OVF_V_poll", value );

    /* (0x00000124 bits 10) field TX_LINK10_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    return etrans_reg_TX_FIFO_OVF_INT_VAL_poll( b_ptr,
                                                h_ptr,
                                                ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK10_10G_FIFO_OVF_V_MSK,
                                                (value<<ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK10_10G_FIFO_OVF_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_TX_LINK9_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK9_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000124 bits 9) field TX_LINK9_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK9_10G_FIFO_OVF_V_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK9_10G_FIFO_OVF_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK9_10G_FIFO_OVF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK9_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK9_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK9_10G_FIFO_OVF_V_poll", value );

    /* (0x00000124 bits 9) field TX_LINK9_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    return etrans_reg_TX_FIFO_OVF_INT_VAL_poll( b_ptr,
                                                h_ptr,
                                                ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK9_10G_FIFO_OVF_V_MSK,
                                                (value<<ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK9_10G_FIFO_OVF_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_TX_LINK8_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK8_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000124 bits 8) field TX_LINK8_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK8_10G_FIFO_OVF_V_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK8_10G_FIFO_OVF_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK8_10G_FIFO_OVF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK8_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK8_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK8_10G_FIFO_OVF_V_poll", value );

    /* (0x00000124 bits 8) field TX_LINK8_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    return etrans_reg_TX_FIFO_OVF_INT_VAL_poll( b_ptr,
                                                h_ptr,
                                                ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK8_10G_FIFO_OVF_V_MSK,
                                                (value<<ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK8_10G_FIFO_OVF_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_TX_LINK7_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK7_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000124 bits 7) field TX_LINK7_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK7_10G_FIFO_OVF_V_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK7_10G_FIFO_OVF_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK7_10G_FIFO_OVF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK7_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK7_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK7_10G_FIFO_OVF_V_poll", value );

    /* (0x00000124 bits 7) field TX_LINK7_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    return etrans_reg_TX_FIFO_OVF_INT_VAL_poll( b_ptr,
                                                h_ptr,
                                                ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK7_10G_FIFO_OVF_V_MSK,
                                                (value<<ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK7_10G_FIFO_OVF_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_TX_LINK6_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK6_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000124 bits 6) field TX_LINK6_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK6_10G_FIFO_OVF_V_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK6_10G_FIFO_OVF_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK6_10G_FIFO_OVF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK6_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK6_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK6_10G_FIFO_OVF_V_poll", value );

    /* (0x00000124 bits 6) field TX_LINK6_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    return etrans_reg_TX_FIFO_OVF_INT_VAL_poll( b_ptr,
                                                h_ptr,
                                                ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK6_10G_FIFO_OVF_V_MSK,
                                                (value<<ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK6_10G_FIFO_OVF_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_TX_LINK5_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK5_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000124 bits 5) field TX_LINK5_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK5_10G_FIFO_OVF_V_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK5_10G_FIFO_OVF_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK5_10G_FIFO_OVF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK5_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK5_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK5_10G_FIFO_OVF_V_poll", value );

    /* (0x00000124 bits 5) field TX_LINK5_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    return etrans_reg_TX_FIFO_OVF_INT_VAL_poll( b_ptr,
                                                h_ptr,
                                                ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK5_10G_FIFO_OVF_V_MSK,
                                                (value<<ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK5_10G_FIFO_OVF_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_TX_LINK4_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK4_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000124 bits 4) field TX_LINK4_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK4_10G_FIFO_OVF_V_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK4_10G_FIFO_OVF_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK4_10G_FIFO_OVF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK4_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK4_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK4_10G_FIFO_OVF_V_poll", value );

    /* (0x00000124 bits 4) field TX_LINK4_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    return etrans_reg_TX_FIFO_OVF_INT_VAL_poll( b_ptr,
                                                h_ptr,
                                                ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK4_10G_FIFO_OVF_V_MSK,
                                                (value<<ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK4_10G_FIFO_OVF_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_TX_LINK3_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK3_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000124 bits 3) field TX_LINK3_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK3_10G_FIFO_OVF_V_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK3_10G_FIFO_OVF_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK3_10G_FIFO_OVF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK3_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK3_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK3_10G_FIFO_OVF_V_poll", value );

    /* (0x00000124 bits 3) field TX_LINK3_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    return etrans_reg_TX_FIFO_OVF_INT_VAL_poll( b_ptr,
                                                h_ptr,
                                                ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK3_10G_FIFO_OVF_V_MSK,
                                                (value<<ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK3_10G_FIFO_OVF_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_TX_LINK2_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK2_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000124 bits 2) field TX_LINK2_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK2_10G_FIFO_OVF_V_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK2_10G_FIFO_OVF_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK2_10G_FIFO_OVF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK2_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK2_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK2_10G_FIFO_OVF_V_poll", value );

    /* (0x00000124 bits 2) field TX_LINK2_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    return etrans_reg_TX_FIFO_OVF_INT_VAL_poll( b_ptr,
                                                h_ptr,
                                                ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK2_10G_FIFO_OVF_V_MSK,
                                                (value<<ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK2_10G_FIFO_OVF_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_TX_LINK1_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK1_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000124 bits 1) field TX_LINK1_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK1_10G_FIFO_OVF_V_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK1_10G_FIFO_OVF_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK1_10G_FIFO_OVF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK1_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK1_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK1_10G_FIFO_OVF_V_poll", value );

    /* (0x00000124 bits 1) field TX_LINK1_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    return etrans_reg_TX_FIFO_OVF_INT_VAL_poll( b_ptr,
                                                h_ptr,
                                                ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK1_10G_FIFO_OVF_V_MSK,
                                                (value<<ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK1_10G_FIFO_OVF_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_TX_LINK0_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_LINK0_10G_FIFO_OVF_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000124 bits 0) field TX_LINK0_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_OVF_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK0_10G_FIFO_OVF_V_MSK) >> ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK0_10G_FIFO_OVF_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_LINK0_10G_FIFO_OVF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK0_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_LINK0_10G_FIFO_OVF_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_LINK0_10G_FIFO_OVF_V_poll", value );

    /* (0x00000124 bits 0) field TX_LINK0_10G_FIFO_OVF_V of register PMC_ETRANS120_REG_TX_FIFO_OVF_INT_VAL */
    return etrans_reg_TX_FIFO_OVF_INT_VAL_poll( b_ptr,
                                                h_ptr,
                                                ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK0_10G_FIFO_OVF_V_MSK,
                                                (value<<ETRANS120_REG_TX_FIFO_OVF_INT_VAL_BIT_TX_LINK0_10G_FIFO_OVF_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_TX_100G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_100G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000128 bits 15) field TX_100G_FIFO_UDR_V of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_UNDERRUN_INT_VAL_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_100G_FIFO_UDR_V_MSK) >> ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_100G_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_100G_FIFO_UDR_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_100G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                         etrans_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_100G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                         etrans_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_100G_FIFO_UDR_V_poll", value );

    /* (0x00000128 bits 15) field TX_100G_FIFO_UDR_V of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL */
    return etrans_reg_TX_FIFO_UNDERRUN_INT_VAL_poll( b_ptr,
                                                     h_ptr,
                                                     ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_100G_FIFO_UDR_V_MSK,
                                                     (value<<ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_100G_FIFO_UDR_V_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_TX_40G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_40G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000128 bits 14:12) field TX_40G_FIFO_UDR_V of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_UNDERRUN_INT_VAL_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_40G_FIFO_UDR_V_MSK) >> ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_40G_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_40G_FIFO_UDR_V_get", value );

    return value;
}
static INLINE UINT32 etrans_field_range_TX_40G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_40G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_40G_FIFO_UDR_V_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_40G_FIFO_UDR_V_get", stop_bit, 2 );
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
    /* (0x00000128 bits 14:12) field TX_40G_FIFO_UDR_V of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_UNDERRUN_INT_VAL_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_40G_FIFO_UDR_V_MSK)
                  >> ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_40G_FIFO_UDR_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_40G_FIFO_UDR_V_MSK, ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_40G_FIFO_UDR_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_40G_FIFO_UDR_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_TX_40G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_TX_40G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_40G_FIFO_UDR_V_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_40G_FIFO_UDR_V_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_40G_FIFO_UDR_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000128 bits 14:12) field TX_40G_FIFO_UDR_V of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL */
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
        if (stop_bit < 2) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 2;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000128 bits 14:12) field TX_40G_FIFO_UDR_V of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL */
        return etrans_reg_TX_FIFO_UNDERRUN_INT_VAL_poll( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_40G_FIFO_UDR_V_OFF + subfield_offset),
                                                         value << (ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_40G_FIFO_UDR_V_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_40G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_40G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_40G_FIFO_UDR_V_poll", value );

    /* (0x00000128 bits 14:12) field TX_40G_FIFO_UDR_V of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL */
    return etrans_reg_TX_FIFO_UNDERRUN_INT_VAL_poll( b_ptr,
                                                     h_ptr,
                                                     ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_40G_FIFO_UDR_V_MSK,
                                                     (value<<ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_40G_FIFO_UDR_V_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE UINT32 etrans_field_TX_10G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_TX_10G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000128 bits 11:0) field TX_10G_FIFO_UDR_V of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_UNDERRUN_INT_VAL_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_10G_FIFO_UDR_V_MSK) >> ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_10G_FIFO_UDR_V_OFF;
    IOLOG( "%s -> 0x%08x", "etrans_field_TX_10G_FIFO_UDR_V_get", value );

    return value;
}
static INLINE UINT32 etrans_field_range_TX_10G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 etrans_field_range_TX_10G_FIFO_UDR_V_get( etrans_buffer_t *b_ptr,
                                                               etrans_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_10G_FIFO_UDR_V_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_10G_FIFO_UDR_V_get", stop_bit, 11 );
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
    /* (0x00000128 bits 11:0) field TX_10G_FIFO_UDR_V of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL */
    reg_value = etrans_reg_TX_FIFO_UNDERRUN_INT_VAL_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_10G_FIFO_UDR_V_MSK)
                  >> ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_10G_FIFO_UDR_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_10G_FIFO_UDR_V_MSK, ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_10G_FIFO_UDR_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_10G_FIFO_UDR_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_TX_10G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_range_TX_10G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                              etrans_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "etrans_field_range_TX_10G_FIFO_UDR_V_poll", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "etrans_field_range_TX_10G_FIFO_UDR_V_poll", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "etrans_field_range_TX_10G_FIFO_UDR_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000128 bits 11:0) field TX_10G_FIFO_UDR_V of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL */
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
        /* (0x00000128 bits 11:0) field TX_10G_FIFO_UDR_V of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL */
        return etrans_reg_TX_FIFO_UNDERRUN_INT_VAL_poll( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_10G_FIFO_UDR_V_OFF + subfield_offset),
                                                         value << (ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_10G_FIFO_UDR_V_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_10G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE etrans_field_TX_10G_FIFO_UDR_V_poll( etrans_buffer_t *b_ptr,
                                                                        etrans_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "etrans_field_TX_10G_FIFO_UDR_V_poll", value );

    /* (0x00000128 bits 11:0) field TX_10G_FIFO_UDR_V of register PMC_ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL */
    return etrans_reg_TX_FIFO_UNDERRUN_INT_VAL_poll( b_ptr,
                                                     h_ptr,
                                                     ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_10G_FIFO_UDR_V_MSK,
                                                     (value<<ETRANS120_REG_TX_FIFO_UNDERRUN_INT_VAL_BIT_TX_10G_FIFO_UDR_V_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ETRANS_IO_INLINE_H */
