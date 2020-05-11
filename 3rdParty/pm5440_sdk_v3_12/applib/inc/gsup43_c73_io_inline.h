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
 *     and register accessor functions for the gsup43_c73 block
 *****************************************************************************/
#ifndef _GSUP43_C73_IO_INLINE_H
#define _GSUP43_C73_IO_INLINE_H

#include "gsup43_c73_loc.h"
#include "gsup43_c73_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GSUP43_C73_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for gsup43_c73
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
    gsup43_c73_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} gsup43_c73_buffer_t;
static INLINE void gsup43_c73_buffer_init( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void gsup43_c73_buffer_init( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "gsup43_c73_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void gsup43_c73_buffer_flush( gsup43_c73_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void gsup43_c73_buffer_flush( gsup43_c73_buffer_t *b_ptr )
{
    IOLOG( "gsup43_c73_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 gsup43_c73_reg_read( gsup43_c73_buffer_t *b_ptr,
                                          gsup43_c73_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_read( gsup43_c73_buffer_t *b_ptr,
                                          gsup43_c73_handle_t *h_ptr,
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
static INLINE void gsup43_c73_reg_write( gsup43_c73_buffer_t *b_ptr,
                                         gsup43_c73_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_write( gsup43_c73_buffer_t *b_ptr,
                                         gsup43_c73_handle_t *h_ptr,
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

static INLINE void gsup43_c73_field_set( gsup43_c73_buffer_t *b_ptr,
                                         gsup43_c73_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 mask,
                                         UINT32 unused_mask,
                                         UINT32 ofs,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_set( gsup43_c73_buffer_t *b_ptr,
                                         gsup43_c73_handle_t *h_ptr,
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

static INLINE void gsup43_c73_action_on_write_field_set( gsup43_c73_buffer_t *b_ptr,
                                                         gsup43_c73_handle_t *h_ptr,
                                                         UINT32 mem_type,
                                                         UINT32 reg,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_action_on_write_field_set( gsup43_c73_buffer_t *b_ptr,
                                                         gsup43_c73_handle_t *h_ptr,
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

static INLINE void gsup43_c73_burst_read( gsup43_c73_buffer_t *b_ptr,
                                          gsup43_c73_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_burst_read( gsup43_c73_buffer_t *b_ptr,
                                          gsup43_c73_handle_t *h_ptr,
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

static INLINE void gsup43_c73_burst_write( gsup43_c73_buffer_t *b_ptr,
                                           gsup43_c73_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 len,
                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_burst_write( gsup43_c73_buffer_t *b_ptr,
                                           gsup43_c73_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_poll( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 mem_type,
                                                    UINT32 reg,
                                                    UINT32 mask,
                                                    UINT32 value,
                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                    UINT32 max_count,
                                                    UINT32 *num_failed_polls,
                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_poll( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
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
 *  register access functions for gsup43_c73
 * ==================================================================================
 */

static INLINE void gsup43_c73_reg_CFG_write( gsup43_c73_buffer_t *b_ptr,
                                             gsup43_c73_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_CFG_write( gsup43_c73_buffer_t *b_ptr,
                                             gsup43_c73_handle_t *h_ptr,
                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "gsup43_c73_reg_CFG_write", value );
    gsup43_c73_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_GSUP43_C73_REG_CFG,
                          value);
}

static INLINE void gsup43_c73_reg_CFG_field_set( gsup43_c73_buffer_t *b_ptr,
                                                 gsup43_c73_handle_t *h_ptr,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_CFG_field_set( gsup43_c73_buffer_t *b_ptr,
                                                 gsup43_c73_handle_t *h_ptr,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "gsup43_c73_reg_CFG_field_set", mask, ofs, value );
    gsup43_c73_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_GSUP43_C73_REG_CFG,
                          mask,
                          PMC_GSUP43_C73_REG_CFG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 gsup43_c73_reg_CFG_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_CFG_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_GSUP43_C73_REG_CFG);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_CFG_read", reg_value);
    return reg_value;
}

static INLINE void gsup43_c73_reg_RX_CFG_write( gsup43_c73_buffer_t *b_ptr,
                                                gsup43_c73_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_RX_CFG_write( gsup43_c73_buffer_t *b_ptr,
                                                gsup43_c73_handle_t *h_ptr,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "gsup43_c73_reg_RX_CFG_write", value );
    gsup43_c73_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_GSUP43_C73_REG_RX_CFG,
                          value);
}

static INLINE void gsup43_c73_reg_RX_CFG_field_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_RX_CFG_field_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "gsup43_c73_reg_RX_CFG_field_set", mask, ofs, value );
    gsup43_c73_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_GSUP43_C73_REG_RX_CFG,
                          mask,
                          PMC_GSUP43_C73_REG_RX_CFG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 gsup43_c73_reg_RX_CFG_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_RX_CFG_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_GSUP43_C73_REG_RX_CFG);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_RX_CFG_read", reg_value);
    return reg_value;
}

static INLINE void gsup43_c73_reg_RX_GSUP43_CFG_write( gsup43_c73_buffer_t *b_ptr,
                                                       gsup43_c73_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_RX_GSUP43_CFG_write( gsup43_c73_buffer_t *b_ptr,
                                                       gsup43_c73_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "gsup43_c73_reg_RX_GSUP43_CFG_write", value );
    gsup43_c73_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_GSUP43_C73_REG_RX_GSUP43_CFG,
                          value);
}

static INLINE void gsup43_c73_reg_RX_GSUP43_CFG_field_set( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_RX_GSUP43_CFG_field_set( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "gsup43_c73_reg_RX_GSUP43_CFG_field_set", mask, ofs, value );
    gsup43_c73_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_GSUP43_C73_REG_RX_GSUP43_CFG,
                          mask,
                          PMC_GSUP43_C73_REG_RX_GSUP43_CFG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 gsup43_c73_reg_RX_GSUP43_CFG_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_RX_GSUP43_CFG_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_GSUP43_C73_REG_RX_GSUP43_CFG);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_RX_GSUP43_CFG_read", reg_value);
    return reg_value;
}

static INLINE void gsup43_c73_reg_RX_125US_CNT_write( gsup43_c73_buffer_t *b_ptr,
                                                      gsup43_c73_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_RX_125US_CNT_write( gsup43_c73_buffer_t *b_ptr,
                                                      gsup43_c73_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "gsup43_c73_reg_RX_125US_CNT_write", value );
    gsup43_c73_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_GSUP43_C73_REG_RX_125US_CNT,
                          value);
}

static INLINE void gsup43_c73_reg_RX_125US_CNT_field_set( gsup43_c73_buffer_t *b_ptr,
                                                          gsup43_c73_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_RX_125US_CNT_field_set( gsup43_c73_buffer_t *b_ptr,
                                                          gsup43_c73_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "gsup43_c73_reg_RX_125US_CNT_field_set", mask, ofs, value );
    gsup43_c73_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_GSUP43_C73_REG_RX_125US_CNT,
                          mask,
                          PMC_GSUP43_C73_REG_RX_125US_CNT_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 gsup43_c73_reg_RX_125US_CNT_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_RX_125US_CNT_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_GSUP43_C73_REG_RX_125US_CNT);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_RX_125US_CNT_read", reg_value);
    return reg_value;
}

static INLINE void gsup43_c73_reg_TX_CFG_1_write( gsup43_c73_buffer_t *b_ptr,
                                                  gsup43_c73_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_TX_CFG_1_write( gsup43_c73_buffer_t *b_ptr,
                                                  gsup43_c73_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "gsup43_c73_reg_TX_CFG_1_write", value );
    gsup43_c73_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_GSUP43_C73_REG_TX_CFG_1,
                          value);
}

static INLINE void gsup43_c73_reg_TX_CFG_1_field_set( gsup43_c73_buffer_t *b_ptr,
                                                      gsup43_c73_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_TX_CFG_1_field_set( gsup43_c73_buffer_t *b_ptr,
                                                      gsup43_c73_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "gsup43_c73_reg_TX_CFG_1_field_set", mask, ofs, value );
    gsup43_c73_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_GSUP43_C73_REG_TX_CFG_1,
                          mask,
                          PMC_GSUP43_C73_REG_TX_CFG_1_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 gsup43_c73_reg_TX_CFG_1_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_TX_CFG_1_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_GSUP43_C73_REG_TX_CFG_1);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_TX_CFG_1_read", reg_value);
    return reg_value;
}

static INLINE void gsup43_c73_reg_TX_CFG_2_write( gsup43_c73_buffer_t *b_ptr,
                                                  gsup43_c73_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_TX_CFG_2_write( gsup43_c73_buffer_t *b_ptr,
                                                  gsup43_c73_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "gsup43_c73_reg_TX_CFG_2_write", value );
    gsup43_c73_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_GSUP43_C73_REG_TX_CFG_2,
                          value);
}

static INLINE void gsup43_c73_reg_TX_CFG_2_field_set( gsup43_c73_buffer_t *b_ptr,
                                                      gsup43_c73_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_TX_CFG_2_field_set( gsup43_c73_buffer_t *b_ptr,
                                                      gsup43_c73_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "gsup43_c73_reg_TX_CFG_2_field_set", mask, ofs, value );
    gsup43_c73_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_GSUP43_C73_REG_TX_CFG_2,
                          mask,
                          PMC_GSUP43_C73_REG_TX_CFG_2_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 gsup43_c73_reg_TX_CFG_2_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_TX_CFG_2_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_GSUP43_C73_REG_TX_CFG_2);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_TX_CFG_2_read", reg_value);
    return reg_value;
}

static INLINE void gsup43_c73_reg_RX_INT_INDICATION_write( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_RX_INT_INDICATION_write( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "gsup43_c73_reg_RX_INT_INDICATION_write", value );
    gsup43_c73_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_GSUP43_C73_REG_RX_INT_INDICATION,
                          value);
}

static INLINE void gsup43_c73_reg_RX_INT_INDICATION_action_on_write_field_set( gsup43_c73_buffer_t *b_ptr,
                                                                               gsup43_c73_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_RX_INT_INDICATION_action_on_write_field_set( gsup43_c73_buffer_t *b_ptr,
                                                                               gsup43_c73_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "gsup43_c73_reg_RX_INT_INDICATION_action_on_write_field_set", mask, ofs, value );
    gsup43_c73_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_GSUP43_C73_REG_RX_INT_INDICATION,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 gsup43_c73_reg_RX_INT_INDICATION_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_RX_INT_INDICATION_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_RX_INT_INDICATION);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_RX_INT_INDICATION_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_reg_RX_INT_INDICATION_poll( gsup43_c73_buffer_t *b_ptr,
                                                                          gsup43_c73_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_reg_RX_INT_INDICATION_poll( gsup43_c73_buffer_t *b_ptr,
                                                                          gsup43_c73_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "gsup43_c73_reg_RX_INT_INDICATION_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return gsup43_c73_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_GSUP43_C73_REG_RX_INT_INDICATION,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE void gsup43_c73_reg_TX_INT_INDICATION_write( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_TX_INT_INDICATION_write( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "gsup43_c73_reg_TX_INT_INDICATION_write", value );
    gsup43_c73_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_GSUP43_C73_REG_TX_INT_INDICATION,
                          value);
}

static INLINE void gsup43_c73_reg_TX_INT_INDICATION_action_on_write_field_set( gsup43_c73_buffer_t *b_ptr,
                                                                               gsup43_c73_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_TX_INT_INDICATION_action_on_write_field_set( gsup43_c73_buffer_t *b_ptr,
                                                                               gsup43_c73_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "gsup43_c73_reg_TX_INT_INDICATION_action_on_write_field_set", mask, ofs, value );
    gsup43_c73_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_GSUP43_C73_REG_TX_INT_INDICATION,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 gsup43_c73_reg_TX_INT_INDICATION_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_TX_INT_INDICATION_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_TX_INT_INDICATION);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_TX_INT_INDICATION_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_reg_TX_INT_INDICATION_poll( gsup43_c73_buffer_t *b_ptr,
                                                                          gsup43_c73_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_reg_TX_INT_INDICATION_poll( gsup43_c73_buffer_t *b_ptr,
                                                                          gsup43_c73_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "gsup43_c73_reg_TX_INT_INDICATION_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return gsup43_c73_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_GSUP43_C73_REG_TX_INT_INDICATION,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE void gsup43_c73_reg_RX_INT_EN_write( gsup43_c73_buffer_t *b_ptr,
                                                   gsup43_c73_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_RX_INT_EN_write( gsup43_c73_buffer_t *b_ptr,
                                                   gsup43_c73_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "gsup43_c73_reg_RX_INT_EN_write", value );
    gsup43_c73_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_GSUP43_C73_REG_RX_INT_EN,
                          value);
}

static INLINE void gsup43_c73_reg_RX_INT_EN_field_set( gsup43_c73_buffer_t *b_ptr,
                                                       gsup43_c73_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_RX_INT_EN_field_set( gsup43_c73_buffer_t *b_ptr,
                                                       gsup43_c73_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "gsup43_c73_reg_RX_INT_EN_field_set", mask, ofs, value );
    gsup43_c73_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_GSUP43_C73_REG_RX_INT_EN,
                          mask,
                          PMC_GSUP43_C73_REG_RX_INT_EN_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 gsup43_c73_reg_RX_INT_EN_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_RX_INT_EN_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_RX_INT_EN);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_RX_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void gsup43_c73_reg_TX_INT_EN_write( gsup43_c73_buffer_t *b_ptr,
                                                   gsup43_c73_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_TX_INT_EN_write( gsup43_c73_buffer_t *b_ptr,
                                                   gsup43_c73_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "gsup43_c73_reg_TX_INT_EN_write", value );
    gsup43_c73_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_GSUP43_C73_REG_TX_INT_EN,
                          value);
}

static INLINE void gsup43_c73_reg_TX_INT_EN_field_set( gsup43_c73_buffer_t *b_ptr,
                                                       gsup43_c73_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_reg_TX_INT_EN_field_set( gsup43_c73_buffer_t *b_ptr,
                                                       gsup43_c73_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "gsup43_c73_reg_TX_INT_EN_field_set", mask, ofs, value );
    gsup43_c73_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_GSUP43_C73_REG_TX_INT_EN,
                          mask,
                          PMC_GSUP43_C73_REG_TX_INT_EN_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 gsup43_c73_reg_TX_INT_EN_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_TX_INT_EN_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_TX_INT_EN);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_TX_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE UINT32 gsup43_c73_reg_RX_STAT_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_RX_STAT_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_RX_STAT);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_RX_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_reg_RX_STAT_poll( gsup43_c73_buffer_t *b_ptr,
                                                                gsup43_c73_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_reg_RX_STAT_poll( gsup43_c73_buffer_t *b_ptr,
                                                                gsup43_c73_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "gsup43_c73_reg_RX_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return gsup43_c73_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_GSUP43_C73_REG_RX_STAT,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 gsup43_c73_reg_TX_STAT_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_TX_STAT_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_TX_STAT);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_TX_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_reg_TX_STAT_poll( gsup43_c73_buffer_t *b_ptr,
                                                                gsup43_c73_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_reg_TX_STAT_poll( gsup43_c73_buffer_t *b_ptr,
                                                                gsup43_c73_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "gsup43_c73_reg_TX_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return gsup43_c73_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_GSUP43_C73_REG_TX_STAT,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 gsup43_c73_reg_RX_FRAME_ERR_CNT_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_RX_FRAME_ERR_CNT_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_RX_FRAME_ERR_CNT);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_RX_FRAME_ERR_CNT_read", reg_value);
    return reg_value;
}

static INLINE UINT32 gsup43_c73_reg_RX_BLOCK_ERR_CNTS_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_RX_BLOCK_ERR_CNTS_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_RX_BLOCK_ERR_CNTS);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_RX_BLOCK_ERR_CNTS_read", reg_value);
    return reg_value;
}

static INLINE UINT32 gsup43_c73_reg_RX_DISCARDED_ORDERED_SET_CNT_LSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_RX_DISCARDED_ORDERED_SET_CNT_LSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_RX_DISCARDED_ORDERED_SET_CNT_LSB);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_RX_DISCARDED_ORDERED_SET_CNT_LSB_read", reg_value);
    return reg_value;
}

static INLINE UINT32 gsup43_c73_reg_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_read", reg_value);
    return reg_value;
}

static INLINE UINT32 gsup43_c73_reg_RX_DATA_BYTE_CNT_LSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_RX_DATA_BYTE_CNT_LSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_RX_DATA_BYTE_CNT_LSB);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_RX_DATA_BYTE_CNT_LSB_read", reg_value);
    return reg_value;
}

static INLINE UINT32 gsup43_c73_reg_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_read", reg_value);
    return reg_value;
}

static INLINE UINT32 gsup43_c73_reg_RX_ORDERED_SET_CNT_LSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_RX_ORDERED_SET_CNT_LSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_RX_ORDERED_SET_CNT_LSB);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_RX_ORDERED_SET_CNT_LSB_read", reg_value);
    return reg_value;
}

static INLINE UINT32 gsup43_c73_reg_RX_PACKET_CNT_LSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_RX_PACKET_CNT_LSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_RX_PACKET_CNT_LSB);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_RX_PACKET_CNT_LSB_read", reg_value);
    return reg_value;
}

static INLINE UINT32 gsup43_c73_reg_RX_PACKET_CNT_MSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_RX_PACKET_CNT_MSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_RX_PACKET_CNT_MSB);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_RX_PACKET_CNT_MSB_read", reg_value);
    return reg_value;
}

static INLINE UINT32 gsup43_c73_reg_TX_PACKET_CNT_LSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_TX_PACKET_CNT_LSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_TX_PACKET_CNT_LSB);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_TX_PACKET_CNT_LSB_read", reg_value);
    return reg_value;
}

static INLINE UINT32 gsup43_c73_reg_TX_PACKET_CNT_MSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_TX_PACKET_CNT_MSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_TX_PACKET_CNT_MSB);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_TX_PACKET_CNT_MSB_read", reg_value);
    return reg_value;
}

static INLINE UINT32 gsup43_c73_reg_TX_DATA_BYTE_CNT_LSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_TX_DATA_BYTE_CNT_LSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_TX_DATA_BYTE_CNT_LSB);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_TX_DATA_BYTE_CNT_LSB_read", reg_value);
    return reg_value;
}

static INLINE UINT32 gsup43_c73_reg_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_read", reg_value);
    return reg_value;
}

static INLINE UINT32 gsup43_c73_reg_TX_ORDERED_SET_CNT_LSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_reg_TX_ORDERED_SET_CNT_LSB_read( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gsup43_c73_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_GSUP43_C73_REG_TX_ORDERED_SET_CNT_LSB);

    IOLOG( "%s -> 0x%08x;", "gsup43_c73_reg_TX_ORDERED_SET_CNT_LSB_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void gsup43_c73_field_SW_RST_set( gsup43_c73_buffer_t *b_ptr,
                                                gsup43_c73_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_SW_RST_set( gsup43_c73_buffer_t *b_ptr,
                                                gsup43_c73_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_SW_RST_set", value );

    /* (0x00000000 bits 0) field SW_RST of register PMC_GSUP43_C73_REG_CFG */
    gsup43_c73_reg_CFG_field_set( b_ptr,
                                  h_ptr,
                                  GSUP43_C73_REG_CFG_BIT_SW_RST_MSK,
                                  GSUP43_C73_REG_CFG_BIT_SW_RST_OFF,
                                  value);
}

static INLINE UINT32 gsup43_c73_field_SW_RST_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_SW_RST_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field SW_RST of register PMC_GSUP43_C73_REG_CFG */
    reg_value = gsup43_c73_reg_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_CFG_BIT_SW_RST_MSK) >> GSUP43_C73_REG_CFG_BIT_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_SW_RST_get", value );

    return value;
}
static INLINE void gsup43_c73_field_RX_SW_RST_set( gsup43_c73_buffer_t *b_ptr,
                                                   gsup43_c73_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_RX_SW_RST_set( gsup43_c73_buffer_t *b_ptr,
                                                   gsup43_c73_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_RX_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_RX_SW_RST_set", value );

    /* (0x00000004 bits 31) field RX_SW_RST of register PMC_GSUP43_C73_REG_RX_CFG */
    gsup43_c73_reg_RX_CFG_field_set( b_ptr,
                                     h_ptr,
                                     GSUP43_C73_REG_RX_CFG_BIT_RX_SW_RST_MSK,
                                     GSUP43_C73_REG_RX_CFG_BIT_RX_SW_RST_OFF,
                                     value);
}

static INLINE UINT32 gsup43_c73_field_RX_SW_RST_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_SW_RST_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 31) field RX_SW_RST of register PMC_GSUP43_C73_REG_RX_CFG */
    reg_value = gsup43_c73_reg_RX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_CFG_BIT_RX_SW_RST_MSK) >> GSUP43_C73_REG_RX_CFG_BIT_RX_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_SW_RST_get", value );

    return value;
}
static INLINE void gsup43_c73_field_LOS_INV_set( gsup43_c73_buffer_t *b_ptr,
                                                 gsup43_c73_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_LOS_INV_set( gsup43_c73_buffer_t *b_ptr,
                                                 gsup43_c73_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_LOS_INV_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_LOS_INV_set", value );

    /* (0x00000004 bits 0) field LOS_INV of register PMC_GSUP43_C73_REG_RX_CFG */
    gsup43_c73_reg_RX_CFG_field_set( b_ptr,
                                     h_ptr,
                                     GSUP43_C73_REG_RX_CFG_BIT_LOS_INV_MSK,
                                     GSUP43_C73_REG_RX_CFG_BIT_LOS_INV_OFF,
                                     value);
}

static INLINE UINT32 gsup43_c73_field_LOS_INV_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_LOS_INV_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 0) field LOS_INV of register PMC_GSUP43_C73_REG_RX_CFG */
    reg_value = gsup43_c73_reg_RX_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_CFG_BIT_LOS_INV_MSK) >> GSUP43_C73_REG_RX_CFG_BIT_LOS_INV_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_LOS_INV_get", value );

    return value;
}
static INLINE void gsup43_c73_field_RX_GSUP43_C73_EN_set( gsup43_c73_buffer_t *b_ptr,
                                                          gsup43_c73_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_RX_GSUP43_C73_EN_set( gsup43_c73_buffer_t *b_ptr,
                                                          gsup43_c73_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_RX_GSUP43_C73_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_RX_GSUP43_C73_EN_set", value );

    /* (0x00000008 bits 31) field RX_GSUP43_C73_EN of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    gsup43_c73_reg_RX_GSUP43_CFG_field_set( b_ptr,
                                            h_ptr,
                                            GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_GSUP43_C73_EN_MSK,
                                            GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_GSUP43_C73_EN_OFF,
                                            value);
}

static INLINE UINT32 gsup43_c73_field_RX_GSUP43_C73_EN_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_GSUP43_C73_EN_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 31) field RX_GSUP43_C73_EN of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    reg_value = gsup43_c73_reg_RX_GSUP43_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_GSUP43_C73_EN_MSK) >> GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_GSUP43_C73_EN_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_GSUP43_C73_EN_get", value );

    return value;
}
static INLINE void gsup43_c73_field_DSCRD_ZONE_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_DSCRD_ZONE_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_DSCRD_ZONE_set", value, 3);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_DSCRD_ZONE_set", value );

    /* (0x00000008 bits 25:24) bits 0:1 use field DSCRD_ZONE of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    gsup43_c73_reg_RX_GSUP43_CFG_field_set( b_ptr,
                                            h_ptr,
                                            GSUP43_C73_REG_RX_GSUP43_CFG_BIT_DSCRD_ZONE_MSK,
                                            GSUP43_C73_REG_RX_GSUP43_CFG_BIT_DSCRD_ZONE_OFF,
                                            value);
}

static INLINE UINT32 gsup43_c73_field_DSCRD_ZONE_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_DSCRD_ZONE_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 25:24) bits 0:1 use field DSCRD_ZONE of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    reg_value = gsup43_c73_reg_RX_GSUP43_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_GSUP43_CFG_BIT_DSCRD_ZONE_MSK) >> GSUP43_C73_REG_RX_GSUP43_CFG_BIT_DSCRD_ZONE_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_DSCRD_ZONE_get", value );

    return value;
}
static INLINE void gsup43_c73_field_range_DSCRD_ZONE_set( gsup43_c73_buffer_t *b_ptr,
                                                          gsup43_c73_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_range_DSCRD_ZONE_set( gsup43_c73_buffer_t *b_ptr,
                                                          gsup43_c73_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_DSCRD_ZONE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_DSCRD_ZONE_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_DSCRD_ZONE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000008 bits 25:24) bits 0:1 use field DSCRD_ZONE of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
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
        /* (0x00000008 bits 25:24) bits 0:1 use field DSCRD_ZONE of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
        gsup43_c73_reg_RX_GSUP43_CFG_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (GSUP43_C73_REG_RX_GSUP43_CFG_BIT_DSCRD_ZONE_OFF + subfield_offset),
                                                GSUP43_C73_REG_RX_GSUP43_CFG_BIT_DSCRD_ZONE_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 gsup43_c73_field_range_DSCRD_ZONE_get( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_range_DSCRD_ZONE_get( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_DSCRD_ZONE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_DSCRD_ZONE_get", stop_bit, 1 );
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
    /* (0x00000008 bits 25:24) bits 0:1 use field DSCRD_ZONE of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    reg_value = gsup43_c73_reg_RX_GSUP43_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_RX_GSUP43_CFG_BIT_DSCRD_ZONE_MSK)
                  >> GSUP43_C73_REG_RX_GSUP43_CFG_BIT_DSCRD_ZONE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_RX_GSUP43_CFG_BIT_DSCRD_ZONE_MSK, GSUP43_C73_REG_RX_GSUP43_CFG_BIT_DSCRD_ZONE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_DSCRD_ZONE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void gsup43_c73_field_RX_SIG_OS_DSCRD_EN_set( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_RX_SIG_OS_DSCRD_EN_set( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_RX_SIG_OS_DSCRD_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_RX_SIG_OS_DSCRD_EN_set", value );

    /* (0x00000008 bits 23) field RX_SIG_OS_DSCRD_EN of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    gsup43_c73_reg_RX_GSUP43_CFG_field_set( b_ptr,
                                            h_ptr,
                                            GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_SIG_OS_DSCRD_EN_MSK,
                                            GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_SIG_OS_DSCRD_EN_OFF,
                                            value);
}

static INLINE UINT32 gsup43_c73_field_RX_SIG_OS_DSCRD_EN_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_SIG_OS_DSCRD_EN_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 23) field RX_SIG_OS_DSCRD_EN of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    reg_value = gsup43_c73_reg_RX_GSUP43_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_SIG_OS_DSCRD_EN_MSK) >> GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_SIG_OS_DSCRD_EN_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_SIG_OS_DSCRD_EN_get", value );

    return value;
}
static INLINE void gsup43_c73_field_RX_SEQ_OS_DSCRD_EN_set( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_RX_SEQ_OS_DSCRD_EN_set( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_RX_SEQ_OS_DSCRD_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_RX_SEQ_OS_DSCRD_EN_set", value );

    /* (0x00000008 bits 22) field RX_SEQ_OS_DSCRD_EN of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    gsup43_c73_reg_RX_GSUP43_CFG_field_set( b_ptr,
                                            h_ptr,
                                            GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_SEQ_OS_DSCRD_EN_MSK,
                                            GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_SEQ_OS_DSCRD_EN_OFF,
                                            value);
}

static INLINE UINT32 gsup43_c73_field_RX_SEQ_OS_DSCRD_EN_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_SEQ_OS_DSCRD_EN_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 22) field RX_SEQ_OS_DSCRD_EN of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    reg_value = gsup43_c73_reg_RX_GSUP43_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_SEQ_OS_DSCRD_EN_MSK) >> GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_SEQ_OS_DSCRD_EN_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_SEQ_OS_DSCRD_EN_get", value );

    return value;
}
static INLINE void gsup43_c73_field_RX_OS_ALL_DSCRD_set( gsup43_c73_buffer_t *b_ptr,
                                                         gsup43_c73_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_RX_OS_ALL_DSCRD_set( gsup43_c73_buffer_t *b_ptr,
                                                         gsup43_c73_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_RX_OS_ALL_DSCRD_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_RX_OS_ALL_DSCRD_set", value );

    /* (0x00000008 bits 21) field RX_OS_ALL_DSCRD of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    gsup43_c73_reg_RX_GSUP43_CFG_field_set( b_ptr,
                                            h_ptr,
                                            GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_OS_ALL_DSCRD_MSK,
                                            GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_OS_ALL_DSCRD_OFF,
                                            value);
}

static INLINE UINT32 gsup43_c73_field_RX_OS_ALL_DSCRD_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_OS_ALL_DSCRD_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 21) field RX_OS_ALL_DSCRD of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    reg_value = gsup43_c73_reg_RX_GSUP43_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_OS_ALL_DSCRD_MSK) >> GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_OS_ALL_DSCRD_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_OS_ALL_DSCRD_get", value );

    return value;
}
static INLINE void gsup43_c73_field_RX_FULL_OS_RPT_DIS_set( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_RX_FULL_OS_RPT_DIS_set( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_RX_FULL_OS_RPT_DIS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_RX_FULL_OS_RPT_DIS_set", value );

    /* (0x00000008 bits 20) field RX_FULL_OS_RPT_DIS of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    gsup43_c73_reg_RX_GSUP43_CFG_field_set( b_ptr,
                                            h_ptr,
                                            GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_FULL_OS_RPT_DIS_MSK,
                                            GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_FULL_OS_RPT_DIS_OFF,
                                            value);
}

static INLINE UINT32 gsup43_c73_field_RX_FULL_OS_RPT_DIS_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_FULL_OS_RPT_DIS_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 20) field RX_FULL_OS_RPT_DIS of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    reg_value = gsup43_c73_reg_RX_GSUP43_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_FULL_OS_RPT_DIS_MSK) >> GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_FULL_OS_RPT_DIS_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_FULL_OS_RPT_DIS_get", value );

    return value;
}
static INLINE void gsup43_c73_field_RX_MIN_OS_RPT_set( gsup43_c73_buffer_t *b_ptr,
                                                       gsup43_c73_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_RX_MIN_OS_RPT_set( gsup43_c73_buffer_t *b_ptr,
                                                       gsup43_c73_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_RX_MIN_OS_RPT_set", value, 15);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_RX_MIN_OS_RPT_set", value );

    /* (0x00000008 bits 19:16) bits 0:3 use field RX_MIN_OS_RPT of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    gsup43_c73_reg_RX_GSUP43_CFG_field_set( b_ptr,
                                            h_ptr,
                                            GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_MIN_OS_RPT_MSK,
                                            GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_MIN_OS_RPT_OFF,
                                            value);
}

static INLINE UINT32 gsup43_c73_field_RX_MIN_OS_RPT_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_MIN_OS_RPT_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 19:16) bits 0:3 use field RX_MIN_OS_RPT of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    reg_value = gsup43_c73_reg_RX_GSUP43_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_MIN_OS_RPT_MSK) >> GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_MIN_OS_RPT_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_MIN_OS_RPT_get", value );

    return value;
}
static INLINE void gsup43_c73_field_range_RX_MIN_OS_RPT_set( gsup43_c73_buffer_t *b_ptr,
                                                             gsup43_c73_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_range_RX_MIN_OS_RPT_set( gsup43_c73_buffer_t *b_ptr,
                                                             gsup43_c73_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_RX_MIN_OS_RPT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_RX_MIN_OS_RPT_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_RX_MIN_OS_RPT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000008 bits 19:16) bits 0:3 use field RX_MIN_OS_RPT of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
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
        /* (0x00000008 bits 19:16) bits 0:3 use field RX_MIN_OS_RPT of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
        gsup43_c73_reg_RX_GSUP43_CFG_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_MIN_OS_RPT_OFF + subfield_offset),
                                                GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_MIN_OS_RPT_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 gsup43_c73_field_range_RX_MIN_OS_RPT_get( gsup43_c73_buffer_t *b_ptr,
                                                               gsup43_c73_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_range_RX_MIN_OS_RPT_get( gsup43_c73_buffer_t *b_ptr,
                                                               gsup43_c73_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_RX_MIN_OS_RPT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_RX_MIN_OS_RPT_get", stop_bit, 3 );
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
    /* (0x00000008 bits 19:16) bits 0:3 use field RX_MIN_OS_RPT of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    reg_value = gsup43_c73_reg_RX_GSUP43_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_MIN_OS_RPT_MSK)
                  >> GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_MIN_OS_RPT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_MIN_OS_RPT_MSK, GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_MIN_OS_RPT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_RX_MIN_OS_RPT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void gsup43_c73_field_RX_PKT_TYPE_set( gsup43_c73_buffer_t *b_ptr,
                                                     gsup43_c73_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_RX_PKT_TYPE_set( gsup43_c73_buffer_t *b_ptr,
                                                     gsup43_c73_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_RX_PKT_TYPE_set", value, 255);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_RX_PKT_TYPE_set", value );

    /* (0x00000008 bits 15:8) bits 0:7 use field RX_PKT_TYPE of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    gsup43_c73_reg_RX_GSUP43_CFG_field_set( b_ptr,
                                            h_ptr,
                                            GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_PKT_TYPE_MSK,
                                            GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_PKT_TYPE_OFF,
                                            value);
}

static INLINE UINT32 gsup43_c73_field_RX_PKT_TYPE_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_PKT_TYPE_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 15:8) bits 0:7 use field RX_PKT_TYPE of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    reg_value = gsup43_c73_reg_RX_GSUP43_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_PKT_TYPE_MSK) >> GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_PKT_TYPE_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_PKT_TYPE_get", value );

    return value;
}
static INLINE void gsup43_c73_field_range_RX_PKT_TYPE_set( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_range_RX_PKT_TYPE_set( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_RX_PKT_TYPE_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_RX_PKT_TYPE_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_RX_PKT_TYPE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000008 bits 15:8) bits 0:7 use field RX_PKT_TYPE of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
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
        /* (0x00000008 bits 15:8) bits 0:7 use field RX_PKT_TYPE of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
        gsup43_c73_reg_RX_GSUP43_CFG_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_PKT_TYPE_OFF + subfield_offset),
                                                GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_PKT_TYPE_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 gsup43_c73_field_range_RX_PKT_TYPE_get( gsup43_c73_buffer_t *b_ptr,
                                                             gsup43_c73_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_range_RX_PKT_TYPE_get( gsup43_c73_buffer_t *b_ptr,
                                                             gsup43_c73_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_RX_PKT_TYPE_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_RX_PKT_TYPE_get", stop_bit, 7 );
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
    /* (0x00000008 bits 15:8) bits 0:7 use field RX_PKT_TYPE of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    reg_value = gsup43_c73_reg_RX_GSUP43_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_PKT_TYPE_MSK)
                  >> GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_PKT_TYPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_PKT_TYPE_MSK, GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_PKT_TYPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_RX_PKT_TYPE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void gsup43_c73_field_RX_OS_TYPE_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_RX_OS_TYPE_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_RX_OS_TYPE_set", value, 255);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_RX_OS_TYPE_set", value );

    /* (0x00000008 bits 7:0) bits 0:7 use field RX_OS_TYPE of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    gsup43_c73_reg_RX_GSUP43_CFG_field_set( b_ptr,
                                            h_ptr,
                                            GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_OS_TYPE_MSK,
                                            GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_OS_TYPE_OFF,
                                            value);
}

static INLINE UINT32 gsup43_c73_field_RX_OS_TYPE_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_OS_TYPE_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 7:0) bits 0:7 use field RX_OS_TYPE of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    reg_value = gsup43_c73_reg_RX_GSUP43_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_OS_TYPE_MSK) >> GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_OS_TYPE_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_OS_TYPE_get", value );

    return value;
}
static INLINE void gsup43_c73_field_range_RX_OS_TYPE_set( gsup43_c73_buffer_t *b_ptr,
                                                          gsup43_c73_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_range_RX_OS_TYPE_set( gsup43_c73_buffer_t *b_ptr,
                                                          gsup43_c73_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_RX_OS_TYPE_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_RX_OS_TYPE_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_RX_OS_TYPE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000008 bits 7:0) bits 0:7 use field RX_OS_TYPE of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
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
        /* (0x00000008 bits 7:0) bits 0:7 use field RX_OS_TYPE of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
        gsup43_c73_reg_RX_GSUP43_CFG_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_OS_TYPE_OFF + subfield_offset),
                                                GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_OS_TYPE_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 gsup43_c73_field_range_RX_OS_TYPE_get( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_range_RX_OS_TYPE_get( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_RX_OS_TYPE_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_RX_OS_TYPE_get", stop_bit, 7 );
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
    /* (0x00000008 bits 7:0) bits 0:7 use field RX_OS_TYPE of register PMC_GSUP43_C73_REG_RX_GSUP43_CFG */
    reg_value = gsup43_c73_reg_RX_GSUP43_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_OS_TYPE_MSK)
                  >> GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_OS_TYPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_OS_TYPE_MSK, GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_OS_TYPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_RX_OS_TYPE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void gsup43_c73_field_RX_125US_CNT_set( gsup43_c73_buffer_t *b_ptr,
                                                      gsup43_c73_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_RX_125US_CNT_set( gsup43_c73_buffer_t *b_ptr,
                                                      gsup43_c73_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_RX_125US_CNT_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_RX_125US_CNT_set", value );

    /* (0x0000000c bits 15:0) bits 0:15 use field RX_125US_CNT of register PMC_GSUP43_C73_REG_RX_125US_CNT */
    gsup43_c73_reg_RX_125US_CNT_field_set( b_ptr,
                                           h_ptr,
                                           GSUP43_C73_REG_RX_125US_CNT_BIT_RX_125US_CNT_MSK,
                                           GSUP43_C73_REG_RX_125US_CNT_BIT_RX_125US_CNT_OFF,
                                           value);
}

static INLINE UINT32 gsup43_c73_field_RX_125US_CNT_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_125US_CNT_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 15:0) bits 0:15 use field RX_125US_CNT of register PMC_GSUP43_C73_REG_RX_125US_CNT */
    reg_value = gsup43_c73_reg_RX_125US_CNT_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_125US_CNT_BIT_RX_125US_CNT_MSK) >> GSUP43_C73_REG_RX_125US_CNT_BIT_RX_125US_CNT_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_125US_CNT_get", value );

    return value;
}
static INLINE void gsup43_c73_field_range_RX_125US_CNT_set( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_range_RX_125US_CNT_set( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_RX_125US_CNT_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_RX_125US_CNT_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_RX_125US_CNT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000000c bits 15:0) bits 0:15 use field RX_125US_CNT of register PMC_GSUP43_C73_REG_RX_125US_CNT */
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
        /* (0x0000000c bits 15:0) bits 0:15 use field RX_125US_CNT of register PMC_GSUP43_C73_REG_RX_125US_CNT */
        gsup43_c73_reg_RX_125US_CNT_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (GSUP43_C73_REG_RX_125US_CNT_BIT_RX_125US_CNT_OFF + subfield_offset),
                                               GSUP43_C73_REG_RX_125US_CNT_BIT_RX_125US_CNT_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 gsup43_c73_field_range_RX_125US_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                              gsup43_c73_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_range_RX_125US_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                              gsup43_c73_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_RX_125US_CNT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_RX_125US_CNT_get", stop_bit, 15 );
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
    /* (0x0000000c bits 15:0) bits 0:15 use field RX_125US_CNT of register PMC_GSUP43_C73_REG_RX_125US_CNT */
    reg_value = gsup43_c73_reg_RX_125US_CNT_read(  b_ptr, h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_RX_125US_CNT_BIT_RX_125US_CNT_MSK)
                  >> GSUP43_C73_REG_RX_125US_CNT_BIT_RX_125US_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_RX_125US_CNT_BIT_RX_125US_CNT_MSK, GSUP43_C73_REG_RX_125US_CNT_BIT_RX_125US_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_RX_125US_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void gsup43_c73_field_TX_SW_RST_set( gsup43_c73_buffer_t *b_ptr,
                                                   gsup43_c73_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_TX_SW_RST_set( gsup43_c73_buffer_t *b_ptr,
                                                   gsup43_c73_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_TX_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_TX_SW_RST_set", value );

    /* (0x00000040 bits 31) field TX_SW_RST of register PMC_GSUP43_C73_REG_TX_CFG_1 */
    gsup43_c73_reg_TX_CFG_1_field_set( b_ptr,
                                       h_ptr,
                                       GSUP43_C73_REG_TX_CFG_1_BIT_TX_SW_RST_MSK,
                                       GSUP43_C73_REG_TX_CFG_1_BIT_TX_SW_RST_OFF,
                                       value);
}

static INLINE UINT32 gsup43_c73_field_TX_SW_RST_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_TX_SW_RST_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000040 bits 31) field TX_SW_RST of register PMC_GSUP43_C73_REG_TX_CFG_1 */
    reg_value = gsup43_c73_reg_TX_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_CFG_1_BIT_TX_SW_RST_MSK) >> GSUP43_C73_REG_TX_CFG_1_BIT_TX_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_TX_SW_RST_get", value );

    return value;
}
static INLINE void gsup43_c73_field_TX_ERR_DROP_set( gsup43_c73_buffer_t *b_ptr,
                                                     gsup43_c73_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_TX_ERR_DROP_set( gsup43_c73_buffer_t *b_ptr,
                                                     gsup43_c73_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_TX_ERR_DROP_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_TX_ERR_DROP_set", value );

    /* (0x00000040 bits 16) field TX_ERR_DROP of register PMC_GSUP43_C73_REG_TX_CFG_1 */
    gsup43_c73_reg_TX_CFG_1_field_set( b_ptr,
                                       h_ptr,
                                       GSUP43_C73_REG_TX_CFG_1_BIT_TX_ERR_DROP_MSK,
                                       GSUP43_C73_REG_TX_CFG_1_BIT_TX_ERR_DROP_OFF,
                                       value);
}

static INLINE UINT32 gsup43_c73_field_TX_ERR_DROP_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_TX_ERR_DROP_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000040 bits 16) field TX_ERR_DROP of register PMC_GSUP43_C73_REG_TX_CFG_1 */
    reg_value = gsup43_c73_reg_TX_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_CFG_1_BIT_TX_ERR_DROP_MSK) >> GSUP43_C73_REG_TX_CFG_1_BIT_TX_ERR_DROP_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_TX_ERR_DROP_get", value );

    return value;
}
static INLINE void gsup43_c73_field_TX_PKT_TYPE_set( gsup43_c73_buffer_t *b_ptr,
                                                     gsup43_c73_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_TX_PKT_TYPE_set( gsup43_c73_buffer_t *b_ptr,
                                                     gsup43_c73_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_TX_PKT_TYPE_set", value, 255);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_TX_PKT_TYPE_set", value );

    /* (0x00000040 bits 15:8) bits 0:7 use field TX_PKT_TYPE of register PMC_GSUP43_C73_REG_TX_CFG_1 */
    gsup43_c73_reg_TX_CFG_1_field_set( b_ptr,
                                       h_ptr,
                                       GSUP43_C73_REG_TX_CFG_1_BIT_TX_PKT_TYPE_MSK,
                                       GSUP43_C73_REG_TX_CFG_1_BIT_TX_PKT_TYPE_OFF,
                                       value);
}

static INLINE UINT32 gsup43_c73_field_TX_PKT_TYPE_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_TX_PKT_TYPE_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000040 bits 15:8) bits 0:7 use field TX_PKT_TYPE of register PMC_GSUP43_C73_REG_TX_CFG_1 */
    reg_value = gsup43_c73_reg_TX_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_CFG_1_BIT_TX_PKT_TYPE_MSK) >> GSUP43_C73_REG_TX_CFG_1_BIT_TX_PKT_TYPE_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_TX_PKT_TYPE_get", value );

    return value;
}
static INLINE void gsup43_c73_field_range_TX_PKT_TYPE_set( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_range_TX_PKT_TYPE_set( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_TX_PKT_TYPE_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_TX_PKT_TYPE_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_TX_PKT_TYPE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000040 bits 15:8) bits 0:7 use field TX_PKT_TYPE of register PMC_GSUP43_C73_REG_TX_CFG_1 */
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
        /* (0x00000040 bits 15:8) bits 0:7 use field TX_PKT_TYPE of register PMC_GSUP43_C73_REG_TX_CFG_1 */
        gsup43_c73_reg_TX_CFG_1_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (GSUP43_C73_REG_TX_CFG_1_BIT_TX_PKT_TYPE_OFF + subfield_offset),
                                           GSUP43_C73_REG_TX_CFG_1_BIT_TX_PKT_TYPE_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 gsup43_c73_field_range_TX_PKT_TYPE_get( gsup43_c73_buffer_t *b_ptr,
                                                             gsup43_c73_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_range_TX_PKT_TYPE_get( gsup43_c73_buffer_t *b_ptr,
                                                             gsup43_c73_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_TX_PKT_TYPE_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_TX_PKT_TYPE_get", stop_bit, 7 );
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
    /* (0x00000040 bits 15:8) bits 0:7 use field TX_PKT_TYPE of register PMC_GSUP43_C73_REG_TX_CFG_1 */
    reg_value = gsup43_c73_reg_TX_CFG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_TX_CFG_1_BIT_TX_PKT_TYPE_MSK)
                  >> GSUP43_C73_REG_TX_CFG_1_BIT_TX_PKT_TYPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_TX_CFG_1_BIT_TX_PKT_TYPE_MSK, GSUP43_C73_REG_TX_CFG_1_BIT_TX_PKT_TYPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_TX_PKT_TYPE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void gsup43_c73_field_TX_OS_TYPE_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_TX_OS_TYPE_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_TX_OS_TYPE_set", value, 255);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_TX_OS_TYPE_set", value );

    /* (0x00000040 bits 7:0) bits 0:7 use field TX_OS_TYPE of register PMC_GSUP43_C73_REG_TX_CFG_1 */
    gsup43_c73_reg_TX_CFG_1_field_set( b_ptr,
                                       h_ptr,
                                       GSUP43_C73_REG_TX_CFG_1_BIT_TX_OS_TYPE_MSK,
                                       GSUP43_C73_REG_TX_CFG_1_BIT_TX_OS_TYPE_OFF,
                                       value);
}

static INLINE UINT32 gsup43_c73_field_TX_OS_TYPE_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_TX_OS_TYPE_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000040 bits 7:0) bits 0:7 use field TX_OS_TYPE of register PMC_GSUP43_C73_REG_TX_CFG_1 */
    reg_value = gsup43_c73_reg_TX_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_CFG_1_BIT_TX_OS_TYPE_MSK) >> GSUP43_C73_REG_TX_CFG_1_BIT_TX_OS_TYPE_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_TX_OS_TYPE_get", value );

    return value;
}
static INLINE void gsup43_c73_field_range_TX_OS_TYPE_set( gsup43_c73_buffer_t *b_ptr,
                                                          gsup43_c73_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_range_TX_OS_TYPE_set( gsup43_c73_buffer_t *b_ptr,
                                                          gsup43_c73_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_TX_OS_TYPE_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_TX_OS_TYPE_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_TX_OS_TYPE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000040 bits 7:0) bits 0:7 use field TX_OS_TYPE of register PMC_GSUP43_C73_REG_TX_CFG_1 */
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
        /* (0x00000040 bits 7:0) bits 0:7 use field TX_OS_TYPE of register PMC_GSUP43_C73_REG_TX_CFG_1 */
        gsup43_c73_reg_TX_CFG_1_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (GSUP43_C73_REG_TX_CFG_1_BIT_TX_OS_TYPE_OFF + subfield_offset),
                                           GSUP43_C73_REG_TX_CFG_1_BIT_TX_OS_TYPE_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 gsup43_c73_field_range_TX_OS_TYPE_get( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_range_TX_OS_TYPE_get( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_TX_OS_TYPE_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_TX_OS_TYPE_get", stop_bit, 7 );
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
    /* (0x00000040 bits 7:0) bits 0:7 use field TX_OS_TYPE of register PMC_GSUP43_C73_REG_TX_CFG_1 */
    reg_value = gsup43_c73_reg_TX_CFG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_TX_CFG_1_BIT_TX_OS_TYPE_MSK)
                  >> GSUP43_C73_REG_TX_CFG_1_BIT_TX_OS_TYPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_TX_CFG_1_BIT_TX_OS_TYPE_MSK, GSUP43_C73_REG_TX_CFG_1_BIT_TX_OS_TYPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_TX_OS_TYPE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void gsup43_c73_field_TX_GSUP43_C73_EN_set( gsup43_c73_buffer_t *b_ptr,
                                                          gsup43_c73_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_TX_GSUP43_C73_EN_set( gsup43_c73_buffer_t *b_ptr,
                                                          gsup43_c73_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_TX_GSUP43_C73_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_TX_GSUP43_C73_EN_set", value );

    /* (0x00000044 bits 31) field TX_GSUP43_C73_EN of register PMC_GSUP43_C73_REG_TX_CFG_2 */
    gsup43_c73_reg_TX_CFG_2_field_set( b_ptr,
                                       h_ptr,
                                       GSUP43_C73_REG_TX_CFG_2_BIT_TX_GSUP43_C73_EN_MSK,
                                       GSUP43_C73_REG_TX_CFG_2_BIT_TX_GSUP43_C73_EN_OFF,
                                       value);
}

static INLINE UINT32 gsup43_c73_field_TX_GSUP43_C73_EN_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_TX_GSUP43_C73_EN_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000044 bits 31) field TX_GSUP43_C73_EN of register PMC_GSUP43_C73_REG_TX_CFG_2 */
    reg_value = gsup43_c73_reg_TX_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_CFG_2_BIT_TX_GSUP43_C73_EN_MSK) >> GSUP43_C73_REG_TX_CFG_2_BIT_TX_GSUP43_C73_EN_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_TX_GSUP43_C73_EN_get", value );

    return value;
}
static INLINE void gsup43_c73_field_FORCE_MIN_IPG_set( gsup43_c73_buffer_t *b_ptr,
                                                       gsup43_c73_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_FORCE_MIN_IPG_set( gsup43_c73_buffer_t *b_ptr,
                                                       gsup43_c73_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_FORCE_MIN_IPG_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_FORCE_MIN_IPG_set", value );

    /* (0x00000044 bits 10) field FORCE_MIN_IPG of register PMC_GSUP43_C73_REG_TX_CFG_2 */
    gsup43_c73_reg_TX_CFG_2_field_set( b_ptr,
                                       h_ptr,
                                       GSUP43_C73_REG_TX_CFG_2_BIT_FORCE_MIN_IPG_MSK,
                                       GSUP43_C73_REG_TX_CFG_2_BIT_FORCE_MIN_IPG_OFF,
                                       value);
}

static INLINE UINT32 gsup43_c73_field_FORCE_MIN_IPG_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_FORCE_MIN_IPG_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000044 bits 10) field FORCE_MIN_IPG of register PMC_GSUP43_C73_REG_TX_CFG_2 */
    reg_value = gsup43_c73_reg_TX_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_CFG_2_BIT_FORCE_MIN_IPG_MSK) >> GSUP43_C73_REG_TX_CFG_2_BIT_FORCE_MIN_IPG_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_FORCE_MIN_IPG_get", value );

    return value;
}
static INLINE void gsup43_c73_field_UNIDIR_EN_set( gsup43_c73_buffer_t *b_ptr,
                                                   gsup43_c73_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_UNIDIR_EN_set( gsup43_c73_buffer_t *b_ptr,
                                                   gsup43_c73_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_UNIDIR_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_UNIDIR_EN_set", value );

    /* (0x00000044 bits 9) field UNIDIR_EN of register PMC_GSUP43_C73_REG_TX_CFG_2 */
    gsup43_c73_reg_TX_CFG_2_field_set( b_ptr,
                                       h_ptr,
                                       GSUP43_C73_REG_TX_CFG_2_BIT_UNIDIR_EN_MSK,
                                       GSUP43_C73_REG_TX_CFG_2_BIT_UNIDIR_EN_OFF,
                                       value);
}

static INLINE UINT32 gsup43_c73_field_UNIDIR_EN_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_UNIDIR_EN_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000044 bits 9) field UNIDIR_EN of register PMC_GSUP43_C73_REG_TX_CFG_2 */
    reg_value = gsup43_c73_reg_TX_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_CFG_2_BIT_UNIDIR_EN_MSK) >> GSUP43_C73_REG_TX_CFG_2_BIT_UNIDIR_EN_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_UNIDIR_EN_get", value );

    return value;
}
static INLINE void gsup43_c73_field_SEND_IDLE_EN_set( gsup43_c73_buffer_t *b_ptr,
                                                      gsup43_c73_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_SEND_IDLE_EN_set( gsup43_c73_buffer_t *b_ptr,
                                                      gsup43_c73_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_SEND_IDLE_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_SEND_IDLE_EN_set", value );

    /* (0x00000044 bits 8) field SEND_IDLE_EN of register PMC_GSUP43_C73_REG_TX_CFG_2 */
    gsup43_c73_reg_TX_CFG_2_field_set( b_ptr,
                                       h_ptr,
                                       GSUP43_C73_REG_TX_CFG_2_BIT_SEND_IDLE_EN_MSK,
                                       GSUP43_C73_REG_TX_CFG_2_BIT_SEND_IDLE_EN_OFF,
                                       value);
}

static INLINE UINT32 gsup43_c73_field_SEND_IDLE_EN_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_SEND_IDLE_EN_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000044 bits 8) field SEND_IDLE_EN of register PMC_GSUP43_C73_REG_TX_CFG_2 */
    reg_value = gsup43_c73_reg_TX_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_CFG_2_BIT_SEND_IDLE_EN_MSK) >> GSUP43_C73_REG_TX_CFG_2_BIT_SEND_IDLE_EN_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_SEND_IDLE_EN_get", value );

    return value;
}
static INLINE void gsup43_c73_field_SEND_LF_EN_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_SEND_LF_EN_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_SEND_LF_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_SEND_LF_EN_set", value );

    /* (0x00000044 bits 7) field SEND_LF_EN of register PMC_GSUP43_C73_REG_TX_CFG_2 */
    gsup43_c73_reg_TX_CFG_2_field_set( b_ptr,
                                       h_ptr,
                                       GSUP43_C73_REG_TX_CFG_2_BIT_SEND_LF_EN_MSK,
                                       GSUP43_C73_REG_TX_CFG_2_BIT_SEND_LF_EN_OFF,
                                       value);
}

static INLINE UINT32 gsup43_c73_field_SEND_LF_EN_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_SEND_LF_EN_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000044 bits 7) field SEND_LF_EN of register PMC_GSUP43_C73_REG_TX_CFG_2 */
    reg_value = gsup43_c73_reg_TX_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_CFG_2_BIT_SEND_LF_EN_MSK) >> GSUP43_C73_REG_TX_CFG_2_BIT_SEND_LF_EN_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_SEND_LF_EN_get", value );

    return value;
}
static INLINE void gsup43_c73_field_SEND_RF_EN_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_SEND_RF_EN_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_SEND_RF_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_SEND_RF_EN_set", value );

    /* (0x00000044 bits 6) field SEND_RF_EN of register PMC_GSUP43_C73_REG_TX_CFG_2 */
    gsup43_c73_reg_TX_CFG_2_field_set( b_ptr,
                                       h_ptr,
                                       GSUP43_C73_REG_TX_CFG_2_BIT_SEND_RF_EN_MSK,
                                       GSUP43_C73_REG_TX_CFG_2_BIT_SEND_RF_EN_OFF,
                                       value);
}

static INLINE UINT32 gsup43_c73_field_SEND_RF_EN_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_SEND_RF_EN_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000044 bits 6) field SEND_RF_EN of register PMC_GSUP43_C73_REG_TX_CFG_2 */
    reg_value = gsup43_c73_reg_TX_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_CFG_2_BIT_SEND_RF_EN_MSK) >> GSUP43_C73_REG_TX_CFG_2_BIT_SEND_RF_EN_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_SEND_RF_EN_get", value );

    return value;
}
static INLINE void gsup43_c73_field_PRBS31_EN_set( gsup43_c73_buffer_t *b_ptr,
                                                   gsup43_c73_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_PRBS31_EN_set( gsup43_c73_buffer_t *b_ptr,
                                                   gsup43_c73_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_PRBS31_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_PRBS31_EN_set", value );

    /* (0x00000044 bits 5) field PRBS31_EN of register PMC_GSUP43_C73_REG_TX_CFG_2 */
    gsup43_c73_reg_TX_CFG_2_field_set( b_ptr,
                                       h_ptr,
                                       GSUP43_C73_REG_TX_CFG_2_BIT_PRBS31_EN_MSK,
                                       GSUP43_C73_REG_TX_CFG_2_BIT_PRBS31_EN_OFF,
                                       value);
}

static INLINE UINT32 gsup43_c73_field_PRBS31_EN_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_PRBS31_EN_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000044 bits 5) field PRBS31_EN of register PMC_GSUP43_C73_REG_TX_CFG_2 */
    reg_value = gsup43_c73_reg_TX_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_CFG_2_BIT_PRBS31_EN_MSK) >> GSUP43_C73_REG_TX_CFG_2_BIT_PRBS31_EN_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_PRBS31_EN_get", value );

    return value;
}
static INLINE void gsup43_c73_field_FIFO_HMARK_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_FIFO_HMARK_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_FIFO_HMARK_set", value, 31);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_FIFO_HMARK_set", value );

    /* (0x00000044 bits 4:0) bits 0:4 use field FIFO_HMARK of register PMC_GSUP43_C73_REG_TX_CFG_2 */
    gsup43_c73_reg_TX_CFG_2_field_set( b_ptr,
                                       h_ptr,
                                       GSUP43_C73_REG_TX_CFG_2_BIT_FIFO_HMARK_MSK,
                                       GSUP43_C73_REG_TX_CFG_2_BIT_FIFO_HMARK_OFF,
                                       value);
}

static INLINE UINT32 gsup43_c73_field_FIFO_HMARK_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_FIFO_HMARK_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000044 bits 4:0) bits 0:4 use field FIFO_HMARK of register PMC_GSUP43_C73_REG_TX_CFG_2 */
    reg_value = gsup43_c73_reg_TX_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_CFG_2_BIT_FIFO_HMARK_MSK) >> GSUP43_C73_REG_TX_CFG_2_BIT_FIFO_HMARK_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_FIFO_HMARK_get", value );

    return value;
}
static INLINE void gsup43_c73_field_range_FIFO_HMARK_set( gsup43_c73_buffer_t *b_ptr,
                                                          gsup43_c73_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_range_FIFO_HMARK_set( gsup43_c73_buffer_t *b_ptr,
                                                          gsup43_c73_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_FIFO_HMARK_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_FIFO_HMARK_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_FIFO_HMARK_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000044 bits 4:0) bits 0:4 use field FIFO_HMARK of register PMC_GSUP43_C73_REG_TX_CFG_2 */
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
        if (stop_bit < 4) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 4;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000044 bits 4:0) bits 0:4 use field FIFO_HMARK of register PMC_GSUP43_C73_REG_TX_CFG_2 */
        gsup43_c73_reg_TX_CFG_2_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (GSUP43_C73_REG_TX_CFG_2_BIT_FIFO_HMARK_OFF + subfield_offset),
                                           GSUP43_C73_REG_TX_CFG_2_BIT_FIFO_HMARK_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 gsup43_c73_field_range_FIFO_HMARK_get( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_range_FIFO_HMARK_get( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_FIFO_HMARK_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_FIFO_HMARK_get", stop_bit, 4 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 4) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 4;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000044 bits 4:0) bits 0:4 use field FIFO_HMARK of register PMC_GSUP43_C73_REG_TX_CFG_2 */
    reg_value = gsup43_c73_reg_TX_CFG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_TX_CFG_2_BIT_FIFO_HMARK_MSK)
                  >> GSUP43_C73_REG_TX_CFG_2_BIT_FIFO_HMARK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_TX_CFG_2_BIT_FIFO_HMARK_MSK, GSUP43_C73_REG_TX_CFG_2_BIT_FIFO_HMARK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_FIFO_HMARK_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void gsup43_c73_field_PMON_UPDATE_set( gsup43_c73_buffer_t *b_ptr,
                                                     gsup43_c73_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_PMON_UPDATE_set( gsup43_c73_buffer_t *b_ptr,
                                                     gsup43_c73_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_PMON_UPDATE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_PMON_UPDATE_set", value );

    /* (0x00000000 bits 1) field PMON_UPDATE of register PMC_GSUP43_C73_REG_CFG */
    gsup43_c73_reg_CFG_field_set( b_ptr,
                                  h_ptr,
                                  GSUP43_C73_REG_CFG_BIT_PMON_UPDATE_MSK,
                                  GSUP43_C73_REG_CFG_BIT_PMON_UPDATE_OFF,
                                  value);
}

static INLINE UINT32 gsup43_c73_field_PMON_UPDATE_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_PMON_UPDATE_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 1) field PMON_UPDATE of register PMC_GSUP43_C73_REG_CFG */
    reg_value = gsup43_c73_reg_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_CFG_BIT_PMON_UPDATE_MSK) >> GSUP43_C73_REG_CFG_BIT_PMON_UPDATE_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_PMON_UPDATE_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_both
 * ==================================================================================
 */
static INLINE void gsup43_c73_field_RX_LF_IC_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_RX_LF_IC_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_RX_LF_IC_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_RX_LF_IC_set_to_clear", value );

    /* (0x00000014 bits 9) field RX_LF_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    gsup43_c73_reg_RX_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_LF_IC_MSK,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_LF_IC_OFF,
                                                                value);
}

static INLINE UINT32 gsup43_c73_field_RX_LF_IC_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_LF_IC_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 9) field RX_LF_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    reg_value = gsup43_c73_reg_RX_INT_INDICATION_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_LF_IC_MSK) >> GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_LF_IC_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_LF_IC_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_LF_IC_poll( gsup43_c73_buffer_t *b_ptr,
                                                                   gsup43_c73_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_LF_IC_poll( gsup43_c73_buffer_t *b_ptr,
                                                                   gsup43_c73_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_RX_LF_IC_poll", value );

    /* (0x00000014 bits 9) field RX_LF_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    return gsup43_c73_reg_RX_INT_INDICATION_poll( b_ptr,
                                                  h_ptr,
                                                  GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_LF_IC_MSK,
                                                  (value<<GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_LF_IC_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void gsup43_c73_field_RX_RF_IC_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_RX_RF_IC_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_RX_RF_IC_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_RX_RF_IC_set_to_clear", value );

    /* (0x00000014 bits 8) field RX_RF_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    gsup43_c73_reg_RX_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_RF_IC_MSK,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_RF_IC_OFF,
                                                                value);
}

static INLINE UINT32 gsup43_c73_field_RX_RF_IC_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_RF_IC_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 8) field RX_RF_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    reg_value = gsup43_c73_reg_RX_INT_INDICATION_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_RF_IC_MSK) >> GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_RF_IC_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_RF_IC_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_RF_IC_poll( gsup43_c73_buffer_t *b_ptr,
                                                                   gsup43_c73_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_RF_IC_poll( gsup43_c73_buffer_t *b_ptr,
                                                                   gsup43_c73_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_RX_RF_IC_poll", value );

    /* (0x00000014 bits 8) field RX_RF_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    return gsup43_c73_reg_RX_INT_INDICATION_poll( b_ptr,
                                                  h_ptr,
                                                  GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_RF_IC_MSK,
                                                  (value<<GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_RF_IC_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void gsup43_c73_field_ALL_ONES_OR_ZEROS_IC_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                                       gsup43_c73_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_ALL_ONES_OR_ZEROS_IC_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                                       gsup43_c73_handle_t *h_ptr,
                                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_ALL_ONES_OR_ZEROS_IC_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_ALL_ONES_OR_ZEROS_IC_set_to_clear", value );

    /* (0x00000014 bits 7) field ALL_ONES_OR_ZEROS_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    gsup43_c73_reg_RX_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_ALL_ONES_OR_ZEROS_IC_MSK,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_ALL_ONES_OR_ZEROS_IC_OFF,
                                                                value);
}

static INLINE UINT32 gsup43_c73_field_ALL_ONES_OR_ZEROS_IC_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_ALL_ONES_OR_ZEROS_IC_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 7) field ALL_ONES_OR_ZEROS_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    reg_value = gsup43_c73_reg_RX_INT_INDICATION_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_INDICATION_BIT_ALL_ONES_OR_ZEROS_IC_MSK) >> GSUP43_C73_REG_RX_INT_INDICATION_BIT_ALL_ONES_OR_ZEROS_IC_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_ALL_ONES_OR_ZEROS_IC_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_ALL_ONES_OR_ZEROS_IC_poll( gsup43_c73_buffer_t *b_ptr,
                                                                               gsup43_c73_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_ALL_ONES_OR_ZEROS_IC_poll( gsup43_c73_buffer_t *b_ptr,
                                                                               gsup43_c73_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_ALL_ONES_OR_ZEROS_IC_poll", value );

    /* (0x00000014 bits 7) field ALL_ONES_OR_ZEROS_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    return gsup43_c73_reg_RX_INT_INDICATION_poll( b_ptr,
                                                  h_ptr,
                                                  GSUP43_C73_REG_RX_INT_INDICATION_BIT_ALL_ONES_OR_ZEROS_IC_MSK,
                                                  (value<<GSUP43_C73_REG_RX_INT_INDICATION_BIT_ALL_ONES_OR_ZEROS_IC_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void gsup43_c73_field_RX_FAULT_IC_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                              gsup43_c73_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_RX_FAULT_IC_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                              gsup43_c73_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_RX_FAULT_IC_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_RX_FAULT_IC_set_to_clear", value );

    /* (0x00000014 bits 4) field RX_FAULT_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    gsup43_c73_reg_RX_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_FAULT_IC_MSK,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_FAULT_IC_OFF,
                                                                value);
}

static INLINE UINT32 gsup43_c73_field_RX_FAULT_IC_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_FAULT_IC_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 4) field RX_FAULT_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    reg_value = gsup43_c73_reg_RX_INT_INDICATION_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_FAULT_IC_MSK) >> GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_FAULT_IC_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_FAULT_IC_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_FAULT_IC_poll( gsup43_c73_buffer_t *b_ptr,
                                                                      gsup43_c73_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_FAULT_IC_poll( gsup43_c73_buffer_t *b_ptr,
                                                                      gsup43_c73_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_RX_FAULT_IC_poll", value );

    /* (0x00000014 bits 4) field RX_FAULT_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    return gsup43_c73_reg_RX_INT_INDICATION_poll( b_ptr,
                                                  h_ptr,
                                                  GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_FAULT_IC_MSK,
                                                  (value<<GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_FAULT_IC_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void gsup43_c73_field_LINK_FAIL_IC_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                               gsup43_c73_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_LINK_FAIL_IC_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                               gsup43_c73_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_LINK_FAIL_IC_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_LINK_FAIL_IC_set_to_clear", value );

    /* (0x00000014 bits 3) field LINK_FAIL_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    gsup43_c73_reg_RX_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_LINK_FAIL_IC_MSK,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_LINK_FAIL_IC_OFF,
                                                                value);
}

static INLINE UINT32 gsup43_c73_field_LINK_FAIL_IC_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_LINK_FAIL_IC_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 3) field LINK_FAIL_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    reg_value = gsup43_c73_reg_RX_INT_INDICATION_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_INDICATION_BIT_LINK_FAIL_IC_MSK) >> GSUP43_C73_REG_RX_INT_INDICATION_BIT_LINK_FAIL_IC_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_LINK_FAIL_IC_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_LINK_FAIL_IC_poll( gsup43_c73_buffer_t *b_ptr,
                                                                       gsup43_c73_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_LINK_FAIL_IC_poll( gsup43_c73_buffer_t *b_ptr,
                                                                       gsup43_c73_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_LINK_FAIL_IC_poll", value );

    /* (0x00000014 bits 3) field LINK_FAIL_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    return gsup43_c73_reg_RX_INT_INDICATION_poll( b_ptr,
                                                  h_ptr,
                                                  GSUP43_C73_REG_RX_INT_INDICATION_BIT_LINK_FAIL_IC_MSK,
                                                  (value<<GSUP43_C73_REG_RX_INT_INDICATION_BIT_LINK_FAIL_IC_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void gsup43_c73_field_HI_BER_IC_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_HI_BER_IC_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_HI_BER_IC_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_HI_BER_IC_set_to_clear", value );

    /* (0x00000014 bits 2) field HI_BER_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    gsup43_c73_reg_RX_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_HI_BER_IC_MSK,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_HI_BER_IC_OFF,
                                                                value);
}

static INLINE UINT32 gsup43_c73_field_HI_BER_IC_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_HI_BER_IC_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 2) field HI_BER_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    reg_value = gsup43_c73_reg_RX_INT_INDICATION_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_INDICATION_BIT_HI_BER_IC_MSK) >> GSUP43_C73_REG_RX_INT_INDICATION_BIT_HI_BER_IC_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_HI_BER_IC_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_HI_BER_IC_poll( gsup43_c73_buffer_t *b_ptr,
                                                                    gsup43_c73_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_HI_BER_IC_poll( gsup43_c73_buffer_t *b_ptr,
                                                                    gsup43_c73_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_HI_BER_IC_poll", value );

    /* (0x00000014 bits 2) field HI_BER_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    return gsup43_c73_reg_RX_INT_INDICATION_poll( b_ptr,
                                                  h_ptr,
                                                  GSUP43_C73_REG_RX_INT_INDICATION_BIT_HI_BER_IC_MSK,
                                                  (value<<GSUP43_C73_REG_RX_INT_INDICATION_BIT_HI_BER_IC_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void gsup43_c73_field_LOS_SYNC_IC_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                              gsup43_c73_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_LOS_SYNC_IC_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                              gsup43_c73_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_LOS_SYNC_IC_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_LOS_SYNC_IC_set_to_clear", value );

    /* (0x00000014 bits 1) field LOS_SYNC_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    gsup43_c73_reg_RX_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_LOS_SYNC_IC_MSK,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_LOS_SYNC_IC_OFF,
                                                                value);
}

static INLINE UINT32 gsup43_c73_field_LOS_SYNC_IC_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_LOS_SYNC_IC_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 1) field LOS_SYNC_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    reg_value = gsup43_c73_reg_RX_INT_INDICATION_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_INDICATION_BIT_LOS_SYNC_IC_MSK) >> GSUP43_C73_REG_RX_INT_INDICATION_BIT_LOS_SYNC_IC_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_LOS_SYNC_IC_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_LOS_SYNC_IC_poll( gsup43_c73_buffer_t *b_ptr,
                                                                      gsup43_c73_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_LOS_SYNC_IC_poll( gsup43_c73_buffer_t *b_ptr,
                                                                      gsup43_c73_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_LOS_SYNC_IC_poll", value );

    /* (0x00000014 bits 1) field LOS_SYNC_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    return gsup43_c73_reg_RX_INT_INDICATION_poll( b_ptr,
                                                  h_ptr,
                                                  GSUP43_C73_REG_RX_INT_INDICATION_BIT_LOS_SYNC_IC_MSK,
                                                  (value<<GSUP43_C73_REG_RX_INT_INDICATION_BIT_LOS_SYNC_IC_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void gsup43_c73_field_LOS_IC_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                         gsup43_c73_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_LOS_IC_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                         gsup43_c73_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_LOS_IC_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_LOS_IC_set_to_clear", value );

    /* (0x00000014 bits 0) field LOS_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    gsup43_c73_reg_RX_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_LOS_IC_MSK,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_LOS_IC_OFF,
                                                                value);
}

static INLINE UINT32 gsup43_c73_field_LOS_IC_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_LOS_IC_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 0) field LOS_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    reg_value = gsup43_c73_reg_RX_INT_INDICATION_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_INDICATION_BIT_LOS_IC_MSK) >> GSUP43_C73_REG_RX_INT_INDICATION_BIT_LOS_IC_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_LOS_IC_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_LOS_IC_poll( gsup43_c73_buffer_t *b_ptr,
                                                                 gsup43_c73_handle_t *h_ptr,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_LOS_IC_poll( gsup43_c73_buffer_t *b_ptr,
                                                                 gsup43_c73_handle_t *h_ptr,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_LOS_IC_poll", value );

    /* (0x00000014 bits 0) field LOS_IC of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    return gsup43_c73_reg_RX_INT_INDICATION_poll( b_ptr,
                                                  h_ptr,
                                                  GSUP43_C73_REG_RX_INT_INDICATION_BIT_LOS_IC_MSK,
                                                  (value<<GSUP43_C73_REG_RX_INT_INDICATION_BIT_LOS_IC_OFF),
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
static INLINE void gsup43_c73_field_RX_PRBS31_ERR_I_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                                  gsup43_c73_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_RX_PRBS31_ERR_I_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                                  gsup43_c73_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_RX_PRBS31_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_RX_PRBS31_ERR_I_set_to_clear", value );

    /* (0x00000014 bits 6) field RX_PRBS31_ERR_I of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    gsup43_c73_reg_RX_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_PRBS31_ERR_I_MSK,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_PRBS31_ERR_I_OFF,
                                                                value);
}

static INLINE UINT32 gsup43_c73_field_RX_PRBS31_ERR_I_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_PRBS31_ERR_I_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 6) field RX_PRBS31_ERR_I of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    reg_value = gsup43_c73_reg_RX_INT_INDICATION_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_PRBS31_ERR_I_MSK) >> GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_PRBS31_ERR_I_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_PRBS31_ERR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_PRBS31_ERR_I_poll( gsup43_c73_buffer_t *b_ptr,
                                                                          gsup43_c73_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_PRBS31_ERR_I_poll( gsup43_c73_buffer_t *b_ptr,
                                                                          gsup43_c73_handle_t *h_ptr,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_RX_PRBS31_ERR_I_poll", value );

    /* (0x00000014 bits 6) field RX_PRBS31_ERR_I of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    return gsup43_c73_reg_RX_INT_INDICATION_poll( b_ptr,
                                                  h_ptr,
                                                  GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_PRBS31_ERR_I_MSK,
                                                  (value<<GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_PRBS31_ERR_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void gsup43_c73_field_BLK_TYPE_ERR_I_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                                 gsup43_c73_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_BLK_TYPE_ERR_I_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                                 gsup43_c73_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_BLK_TYPE_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_BLK_TYPE_ERR_I_set_to_clear", value );

    /* (0x00000014 bits 5) field BLK_TYPE_ERR_I of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    gsup43_c73_reg_RX_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_BLK_TYPE_ERR_I_MSK,
                                                                GSUP43_C73_REG_RX_INT_INDICATION_BIT_BLK_TYPE_ERR_I_OFF,
                                                                value);
}

static INLINE UINT32 gsup43_c73_field_BLK_TYPE_ERR_I_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_BLK_TYPE_ERR_I_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 5) field BLK_TYPE_ERR_I of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    reg_value = gsup43_c73_reg_RX_INT_INDICATION_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_INDICATION_BIT_BLK_TYPE_ERR_I_MSK) >> GSUP43_C73_REG_RX_INT_INDICATION_BIT_BLK_TYPE_ERR_I_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_BLK_TYPE_ERR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_BLK_TYPE_ERR_I_poll( gsup43_c73_buffer_t *b_ptr,
                                                                         gsup43_c73_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_BLK_TYPE_ERR_I_poll( gsup43_c73_buffer_t *b_ptr,
                                                                         gsup43_c73_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_BLK_TYPE_ERR_I_poll", value );

    /* (0x00000014 bits 5) field BLK_TYPE_ERR_I of register PMC_GSUP43_C73_REG_RX_INT_INDICATION */
    return gsup43_c73_reg_RX_INT_INDICATION_poll( b_ptr,
                                                  h_ptr,
                                                  GSUP43_C73_REG_RX_INT_INDICATION_BIT_BLK_TYPE_ERR_I_MSK,
                                                  (value<<GSUP43_C73_REG_RX_INT_INDICATION_BIT_BLK_TYPE_ERR_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void gsup43_c73_field_TYPE_ERR_I_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                             gsup43_c73_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_TYPE_ERR_I_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                             gsup43_c73_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_TYPE_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_TYPE_ERR_I_set_to_clear", value );

    /* (0x00000060 bits 5) field TYPE_ERR_I of register PMC_GSUP43_C73_REG_TX_INT_INDICATION */
    gsup43_c73_reg_TX_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                GSUP43_C73_REG_TX_INT_INDICATION_BIT_TYPE_ERR_I_MSK,
                                                                GSUP43_C73_REG_TX_INT_INDICATION_BIT_TYPE_ERR_I_OFF,
                                                                value);
}

static INLINE UINT32 gsup43_c73_field_TYPE_ERR_I_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_TYPE_ERR_I_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000060 bits 5) field TYPE_ERR_I of register PMC_GSUP43_C73_REG_TX_INT_INDICATION */
    reg_value = gsup43_c73_reg_TX_INT_INDICATION_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_INT_INDICATION_BIT_TYPE_ERR_I_MSK) >> GSUP43_C73_REG_TX_INT_INDICATION_BIT_TYPE_ERR_I_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_TYPE_ERR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_TYPE_ERR_I_poll( gsup43_c73_buffer_t *b_ptr,
                                                                     gsup43_c73_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_TYPE_ERR_I_poll( gsup43_c73_buffer_t *b_ptr,
                                                                     gsup43_c73_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_TYPE_ERR_I_poll", value );

    /* (0x00000060 bits 5) field TYPE_ERR_I of register PMC_GSUP43_C73_REG_TX_INT_INDICATION */
    return gsup43_c73_reg_TX_INT_INDICATION_poll( b_ptr,
                                                  h_ptr,
                                                  GSUP43_C73_REG_TX_INT_INDICATION_BIT_TYPE_ERR_I_MSK,
                                                  (value<<GSUP43_C73_REG_TX_INT_INDICATION_BIT_TYPE_ERR_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void gsup43_c73_field_SEND_IDLE_I_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                              gsup43_c73_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_SEND_IDLE_I_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                              gsup43_c73_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_SEND_IDLE_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_SEND_IDLE_I_set_to_clear", value );

    /* (0x00000060 bits 4) field SEND_IDLE_I of register PMC_GSUP43_C73_REG_TX_INT_INDICATION */
    gsup43_c73_reg_TX_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_IDLE_I_MSK,
                                                                GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_IDLE_I_OFF,
                                                                value);
}

static INLINE UINT32 gsup43_c73_field_SEND_IDLE_I_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_SEND_IDLE_I_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000060 bits 4) field SEND_IDLE_I of register PMC_GSUP43_C73_REG_TX_INT_INDICATION */
    reg_value = gsup43_c73_reg_TX_INT_INDICATION_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_IDLE_I_MSK) >> GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_IDLE_I_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_SEND_IDLE_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_SEND_IDLE_I_poll( gsup43_c73_buffer_t *b_ptr,
                                                                      gsup43_c73_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_SEND_IDLE_I_poll( gsup43_c73_buffer_t *b_ptr,
                                                                      gsup43_c73_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_SEND_IDLE_I_poll", value );

    /* (0x00000060 bits 4) field SEND_IDLE_I of register PMC_GSUP43_C73_REG_TX_INT_INDICATION */
    return gsup43_c73_reg_TX_INT_INDICATION_poll( b_ptr,
                                                  h_ptr,
                                                  GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_IDLE_I_MSK,
                                                  (value<<GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_IDLE_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void gsup43_c73_field_SEND_LF_I_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_SEND_LF_I_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_SEND_LF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_SEND_LF_I_set_to_clear", value );

    /* (0x00000060 bits 3) field SEND_LF_I of register PMC_GSUP43_C73_REG_TX_INT_INDICATION */
    gsup43_c73_reg_TX_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_LF_I_MSK,
                                                                GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_LF_I_OFF,
                                                                value);
}

static INLINE UINT32 gsup43_c73_field_SEND_LF_I_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_SEND_LF_I_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000060 bits 3) field SEND_LF_I of register PMC_GSUP43_C73_REG_TX_INT_INDICATION */
    reg_value = gsup43_c73_reg_TX_INT_INDICATION_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_LF_I_MSK) >> GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_LF_I_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_SEND_LF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_SEND_LF_I_poll( gsup43_c73_buffer_t *b_ptr,
                                                                    gsup43_c73_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_SEND_LF_I_poll( gsup43_c73_buffer_t *b_ptr,
                                                                    gsup43_c73_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_SEND_LF_I_poll", value );

    /* (0x00000060 bits 3) field SEND_LF_I of register PMC_GSUP43_C73_REG_TX_INT_INDICATION */
    return gsup43_c73_reg_TX_INT_INDICATION_poll( b_ptr,
                                                  h_ptr,
                                                  GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_LF_I_MSK,
                                                  (value<<GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_LF_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void gsup43_c73_field_SEND_RF_I_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_SEND_RF_I_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_SEND_RF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_SEND_RF_I_set_to_clear", value );

    /* (0x00000060 bits 2) field SEND_RF_I of register PMC_GSUP43_C73_REG_TX_INT_INDICATION */
    gsup43_c73_reg_TX_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_RF_I_MSK,
                                                                GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_RF_I_OFF,
                                                                value);
}

static INLINE UINT32 gsup43_c73_field_SEND_RF_I_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_SEND_RF_I_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000060 bits 2) field SEND_RF_I of register PMC_GSUP43_C73_REG_TX_INT_INDICATION */
    reg_value = gsup43_c73_reg_TX_INT_INDICATION_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_RF_I_MSK) >> GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_RF_I_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_SEND_RF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_SEND_RF_I_poll( gsup43_c73_buffer_t *b_ptr,
                                                                    gsup43_c73_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_SEND_RF_I_poll( gsup43_c73_buffer_t *b_ptr,
                                                                    gsup43_c73_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_SEND_RF_I_poll", value );

    /* (0x00000060 bits 2) field SEND_RF_I of register PMC_GSUP43_C73_REG_TX_INT_INDICATION */
    return gsup43_c73_reg_TX_INT_INDICATION_poll( b_ptr,
                                                  h_ptr,
                                                  GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_RF_I_MSK,
                                                  (value<<GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_RF_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void gsup43_c73_field_FIFO_OVRUN_I_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                               gsup43_c73_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_FIFO_OVRUN_I_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                               gsup43_c73_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_FIFO_OVRUN_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_FIFO_OVRUN_I_set_to_clear", value );

    /* (0x00000060 bits 1) field FIFO_OVRUN_I of register PMC_GSUP43_C73_REG_TX_INT_INDICATION */
    gsup43_c73_reg_TX_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                GSUP43_C73_REG_TX_INT_INDICATION_BIT_FIFO_OVRUN_I_MSK,
                                                                GSUP43_C73_REG_TX_INT_INDICATION_BIT_FIFO_OVRUN_I_OFF,
                                                                value);
}

static INLINE UINT32 gsup43_c73_field_FIFO_OVRUN_I_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_FIFO_OVRUN_I_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000060 bits 1) field FIFO_OVRUN_I of register PMC_GSUP43_C73_REG_TX_INT_INDICATION */
    reg_value = gsup43_c73_reg_TX_INT_INDICATION_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_INT_INDICATION_BIT_FIFO_OVRUN_I_MSK) >> GSUP43_C73_REG_TX_INT_INDICATION_BIT_FIFO_OVRUN_I_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_FIFO_OVRUN_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_FIFO_OVRUN_I_poll( gsup43_c73_buffer_t *b_ptr,
                                                                       gsup43_c73_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_FIFO_OVRUN_I_poll( gsup43_c73_buffer_t *b_ptr,
                                                                       gsup43_c73_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_FIFO_OVRUN_I_poll", value );

    /* (0x00000060 bits 1) field FIFO_OVRUN_I of register PMC_GSUP43_C73_REG_TX_INT_INDICATION */
    return gsup43_c73_reg_TX_INT_INDICATION_poll( b_ptr,
                                                  h_ptr,
                                                  GSUP43_C73_REG_TX_INT_INDICATION_BIT_FIFO_OVRUN_I_MSK,
                                                  (value<<GSUP43_C73_REG_TX_INT_INDICATION_BIT_FIFO_OVRUN_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void gsup43_c73_field_FIFO_UNDRUN_I_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                                gsup43_c73_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_FIFO_UNDRUN_I_set_to_clear( gsup43_c73_buffer_t *b_ptr,
                                                                gsup43_c73_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_FIFO_UNDRUN_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_FIFO_UNDRUN_I_set_to_clear", value );

    /* (0x00000060 bits 0) field FIFO_UNDRUN_I of register PMC_GSUP43_C73_REG_TX_INT_INDICATION */
    gsup43_c73_reg_TX_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                GSUP43_C73_REG_TX_INT_INDICATION_BIT_FIFO_UNDRUN_I_MSK,
                                                                GSUP43_C73_REG_TX_INT_INDICATION_BIT_FIFO_UNDRUN_I_OFF,
                                                                value);
}

static INLINE UINT32 gsup43_c73_field_FIFO_UNDRUN_I_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_FIFO_UNDRUN_I_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000060 bits 0) field FIFO_UNDRUN_I of register PMC_GSUP43_C73_REG_TX_INT_INDICATION */
    reg_value = gsup43_c73_reg_TX_INT_INDICATION_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_INT_INDICATION_BIT_FIFO_UNDRUN_I_MSK) >> GSUP43_C73_REG_TX_INT_INDICATION_BIT_FIFO_UNDRUN_I_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_FIFO_UNDRUN_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_FIFO_UNDRUN_I_poll( gsup43_c73_buffer_t *b_ptr,
                                                                        gsup43_c73_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_FIFO_UNDRUN_I_poll( gsup43_c73_buffer_t *b_ptr,
                                                                        gsup43_c73_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_FIFO_UNDRUN_I_poll", value );

    /* (0x00000060 bits 0) field FIFO_UNDRUN_I of register PMC_GSUP43_C73_REG_TX_INT_INDICATION */
    return gsup43_c73_reg_TX_INT_INDICATION_poll( b_ptr,
                                                  h_ptr,
                                                  GSUP43_C73_REG_TX_INT_INDICATION_BIT_FIFO_UNDRUN_I_MSK,
                                                  (value<<GSUP43_C73_REG_TX_INT_INDICATION_BIT_FIFO_UNDRUN_I_OFF),
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
static INLINE void gsup43_c73_field_RX_LF_E_set( gsup43_c73_buffer_t *b_ptr,
                                                 gsup43_c73_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_RX_LF_E_set( gsup43_c73_buffer_t *b_ptr,
                                                 gsup43_c73_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_RX_LF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_RX_LF_E_set", value );

    /* (0x00000010 bits 9) field RX_LF_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    gsup43_c73_reg_RX_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_RX_LF_E_MSK,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_RX_LF_E_OFF,
                                        value);
}

static INLINE UINT32 gsup43_c73_field_RX_LF_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_LF_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 9) field RX_LF_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    reg_value = gsup43_c73_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_EN_BIT_RX_LF_E_MSK) >> GSUP43_C73_REG_RX_INT_EN_BIT_RX_LF_E_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_LF_E_get", value );

    return value;
}
static INLINE void gsup43_c73_field_RX_RF_E_set( gsup43_c73_buffer_t *b_ptr,
                                                 gsup43_c73_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_RX_RF_E_set( gsup43_c73_buffer_t *b_ptr,
                                                 gsup43_c73_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_RX_RF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_RX_RF_E_set", value );

    /* (0x00000010 bits 8) field RX_RF_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    gsup43_c73_reg_RX_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_RX_RF_E_MSK,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_RX_RF_E_OFF,
                                        value);
}

static INLINE UINT32 gsup43_c73_field_RX_RF_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_RF_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 8) field RX_RF_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    reg_value = gsup43_c73_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_EN_BIT_RX_RF_E_MSK) >> GSUP43_C73_REG_RX_INT_EN_BIT_RX_RF_E_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_RF_E_get", value );

    return value;
}
static INLINE void gsup43_c73_field_ALL_ONES_OR_ZEROS_E_set( gsup43_c73_buffer_t *b_ptr,
                                                             gsup43_c73_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_ALL_ONES_OR_ZEROS_E_set( gsup43_c73_buffer_t *b_ptr,
                                                             gsup43_c73_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_ALL_ONES_OR_ZEROS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_ALL_ONES_OR_ZEROS_E_set", value );

    /* (0x00000010 bits 7) field ALL_ONES_OR_ZEROS_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    gsup43_c73_reg_RX_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_ALL_ONES_OR_ZEROS_E_MSK,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_ALL_ONES_OR_ZEROS_E_OFF,
                                        value);
}

static INLINE UINT32 gsup43_c73_field_ALL_ONES_OR_ZEROS_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_ALL_ONES_OR_ZEROS_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 7) field ALL_ONES_OR_ZEROS_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    reg_value = gsup43_c73_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_EN_BIT_ALL_ONES_OR_ZEROS_E_MSK) >> GSUP43_C73_REG_RX_INT_EN_BIT_ALL_ONES_OR_ZEROS_E_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_ALL_ONES_OR_ZEROS_E_get", value );

    return value;
}
static INLINE void gsup43_c73_field_RX_PRBS31_ERR_E_set( gsup43_c73_buffer_t *b_ptr,
                                                         gsup43_c73_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_RX_PRBS31_ERR_E_set( gsup43_c73_buffer_t *b_ptr,
                                                         gsup43_c73_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_RX_PRBS31_ERR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_RX_PRBS31_ERR_E_set", value );

    /* (0x00000010 bits 6) field RX_PRBS31_ERR_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    gsup43_c73_reg_RX_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_RX_PRBS31_ERR_E_MSK,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_RX_PRBS31_ERR_E_OFF,
                                        value);
}

static INLINE UINT32 gsup43_c73_field_RX_PRBS31_ERR_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_PRBS31_ERR_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 6) field RX_PRBS31_ERR_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    reg_value = gsup43_c73_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_EN_BIT_RX_PRBS31_ERR_E_MSK) >> GSUP43_C73_REG_RX_INT_EN_BIT_RX_PRBS31_ERR_E_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_PRBS31_ERR_E_get", value );

    return value;
}
static INLINE void gsup43_c73_field_BLK_TYPE_ERR_E_set( gsup43_c73_buffer_t *b_ptr,
                                                        gsup43_c73_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_BLK_TYPE_ERR_E_set( gsup43_c73_buffer_t *b_ptr,
                                                        gsup43_c73_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_BLK_TYPE_ERR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_BLK_TYPE_ERR_E_set", value );

    /* (0x00000010 bits 5) field BLK_TYPE_ERR_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    gsup43_c73_reg_RX_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_BLK_TYPE_ERR_E_MSK,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_BLK_TYPE_ERR_E_OFF,
                                        value);
}

static INLINE UINT32 gsup43_c73_field_BLK_TYPE_ERR_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_BLK_TYPE_ERR_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 5) field BLK_TYPE_ERR_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    reg_value = gsup43_c73_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_EN_BIT_BLK_TYPE_ERR_E_MSK) >> GSUP43_C73_REG_RX_INT_EN_BIT_BLK_TYPE_ERR_E_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_BLK_TYPE_ERR_E_get", value );

    return value;
}
static INLINE void gsup43_c73_field_RX_FAULT_E_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_RX_FAULT_E_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_RX_FAULT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_RX_FAULT_E_set", value );

    /* (0x00000010 bits 4) field RX_FAULT_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    gsup43_c73_reg_RX_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_RX_FAULT_E_MSK,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_RX_FAULT_E_OFF,
                                        value);
}

static INLINE UINT32 gsup43_c73_field_RX_FAULT_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_FAULT_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 4) field RX_FAULT_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    reg_value = gsup43_c73_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_EN_BIT_RX_FAULT_E_MSK) >> GSUP43_C73_REG_RX_INT_EN_BIT_RX_FAULT_E_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_FAULT_E_get", value );

    return value;
}
static INLINE void gsup43_c73_field_LINK_FAIL_E_set( gsup43_c73_buffer_t *b_ptr,
                                                     gsup43_c73_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_LINK_FAIL_E_set( gsup43_c73_buffer_t *b_ptr,
                                                     gsup43_c73_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_LINK_FAIL_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_LINK_FAIL_E_set", value );

    /* (0x00000010 bits 3) field LINK_FAIL_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    gsup43_c73_reg_RX_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_LINK_FAIL_E_MSK,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_LINK_FAIL_E_OFF,
                                        value);
}

static INLINE UINT32 gsup43_c73_field_LINK_FAIL_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_LINK_FAIL_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 3) field LINK_FAIL_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    reg_value = gsup43_c73_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_EN_BIT_LINK_FAIL_E_MSK) >> GSUP43_C73_REG_RX_INT_EN_BIT_LINK_FAIL_E_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_LINK_FAIL_E_get", value );

    return value;
}
static INLINE void gsup43_c73_field_HI_BER_E_set( gsup43_c73_buffer_t *b_ptr,
                                                  gsup43_c73_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_HI_BER_E_set( gsup43_c73_buffer_t *b_ptr,
                                                  gsup43_c73_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_HI_BER_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_HI_BER_E_set", value );

    /* (0x00000010 bits 2) field HI_BER_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    gsup43_c73_reg_RX_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_HI_BER_E_MSK,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_HI_BER_E_OFF,
                                        value);
}

static INLINE UINT32 gsup43_c73_field_HI_BER_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_HI_BER_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 2) field HI_BER_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    reg_value = gsup43_c73_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_EN_BIT_HI_BER_E_MSK) >> GSUP43_C73_REG_RX_INT_EN_BIT_HI_BER_E_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_HI_BER_E_get", value );

    return value;
}
static INLINE void gsup43_c73_field_LOS_SYNC_E_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_LOS_SYNC_E_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_LOS_SYNC_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_LOS_SYNC_E_set", value );

    /* (0x00000010 bits 1) field LOS_SYNC_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    gsup43_c73_reg_RX_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_LOS_SYNC_E_MSK,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_LOS_SYNC_E_OFF,
                                        value);
}

static INLINE UINT32 gsup43_c73_field_LOS_SYNC_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_LOS_SYNC_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 1) field LOS_SYNC_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    reg_value = gsup43_c73_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_EN_BIT_LOS_SYNC_E_MSK) >> GSUP43_C73_REG_RX_INT_EN_BIT_LOS_SYNC_E_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_LOS_SYNC_E_get", value );

    return value;
}
static INLINE void gsup43_c73_field_LOS_E_set( gsup43_c73_buffer_t *b_ptr,
                                               gsup43_c73_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_LOS_E_set( gsup43_c73_buffer_t *b_ptr,
                                               gsup43_c73_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_LOS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_LOS_E_set", value );

    /* (0x00000010 bits 0) field LOS_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    gsup43_c73_reg_RX_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_LOS_E_MSK,
                                        GSUP43_C73_REG_RX_INT_EN_BIT_LOS_E_OFF,
                                        value);
}

static INLINE UINT32 gsup43_c73_field_LOS_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_LOS_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 0) field LOS_E of register PMC_GSUP43_C73_REG_RX_INT_EN */
    reg_value = gsup43_c73_reg_RX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_INT_EN_BIT_LOS_E_MSK) >> GSUP43_C73_REG_RX_INT_EN_BIT_LOS_E_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_LOS_E_get", value );

    return value;
}
static INLINE void gsup43_c73_field_TYPE_ERR_E_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_TYPE_ERR_E_set( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_TYPE_ERR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_TYPE_ERR_E_set", value );

    /* (0x0000005c bits 5) field TYPE_ERR_E of register PMC_GSUP43_C73_REG_TX_INT_EN */
    gsup43_c73_reg_TX_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_TX_INT_EN_BIT_TYPE_ERR_E_MSK,
                                        GSUP43_C73_REG_TX_INT_EN_BIT_TYPE_ERR_E_OFF,
                                        value);
}

static INLINE UINT32 gsup43_c73_field_TYPE_ERR_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_TYPE_ERR_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000005c bits 5) field TYPE_ERR_E of register PMC_GSUP43_C73_REG_TX_INT_EN */
    reg_value = gsup43_c73_reg_TX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_INT_EN_BIT_TYPE_ERR_E_MSK) >> GSUP43_C73_REG_TX_INT_EN_BIT_TYPE_ERR_E_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_TYPE_ERR_E_get", value );

    return value;
}
static INLINE void gsup43_c73_field_SEND_IDLE_E_set( gsup43_c73_buffer_t *b_ptr,
                                                     gsup43_c73_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_SEND_IDLE_E_set( gsup43_c73_buffer_t *b_ptr,
                                                     gsup43_c73_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_SEND_IDLE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_SEND_IDLE_E_set", value );

    /* (0x0000005c bits 4) field SEND_IDLE_E of register PMC_GSUP43_C73_REG_TX_INT_EN */
    gsup43_c73_reg_TX_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_TX_INT_EN_BIT_SEND_IDLE_E_MSK,
                                        GSUP43_C73_REG_TX_INT_EN_BIT_SEND_IDLE_E_OFF,
                                        value);
}

static INLINE UINT32 gsup43_c73_field_SEND_IDLE_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_SEND_IDLE_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000005c bits 4) field SEND_IDLE_E of register PMC_GSUP43_C73_REG_TX_INT_EN */
    reg_value = gsup43_c73_reg_TX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_INT_EN_BIT_SEND_IDLE_E_MSK) >> GSUP43_C73_REG_TX_INT_EN_BIT_SEND_IDLE_E_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_SEND_IDLE_E_get", value );

    return value;
}
static INLINE void gsup43_c73_field_SEND_LF_E_set( gsup43_c73_buffer_t *b_ptr,
                                                   gsup43_c73_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_SEND_LF_E_set( gsup43_c73_buffer_t *b_ptr,
                                                   gsup43_c73_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_SEND_LF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_SEND_LF_E_set", value );

    /* (0x0000005c bits 3) field SEND_LF_E of register PMC_GSUP43_C73_REG_TX_INT_EN */
    gsup43_c73_reg_TX_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_TX_INT_EN_BIT_SEND_LF_E_MSK,
                                        GSUP43_C73_REG_TX_INT_EN_BIT_SEND_LF_E_OFF,
                                        value);
}

static INLINE UINT32 gsup43_c73_field_SEND_LF_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_SEND_LF_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000005c bits 3) field SEND_LF_E of register PMC_GSUP43_C73_REG_TX_INT_EN */
    reg_value = gsup43_c73_reg_TX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_INT_EN_BIT_SEND_LF_E_MSK) >> GSUP43_C73_REG_TX_INT_EN_BIT_SEND_LF_E_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_SEND_LF_E_get", value );

    return value;
}
static INLINE void gsup43_c73_field_SEND_RF_E_set( gsup43_c73_buffer_t *b_ptr,
                                                   gsup43_c73_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_SEND_RF_E_set( gsup43_c73_buffer_t *b_ptr,
                                                   gsup43_c73_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_SEND_RF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_SEND_RF_E_set", value );

    /* (0x0000005c bits 2) field SEND_RF_E of register PMC_GSUP43_C73_REG_TX_INT_EN */
    gsup43_c73_reg_TX_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_TX_INT_EN_BIT_SEND_RF_E_MSK,
                                        GSUP43_C73_REG_TX_INT_EN_BIT_SEND_RF_E_OFF,
                                        value);
}

static INLINE UINT32 gsup43_c73_field_SEND_RF_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_SEND_RF_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000005c bits 2) field SEND_RF_E of register PMC_GSUP43_C73_REG_TX_INT_EN */
    reg_value = gsup43_c73_reg_TX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_INT_EN_BIT_SEND_RF_E_MSK) >> GSUP43_C73_REG_TX_INT_EN_BIT_SEND_RF_E_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_SEND_RF_E_get", value );

    return value;
}
static INLINE void gsup43_c73_field_FIFO_OVRUN_E_set( gsup43_c73_buffer_t *b_ptr,
                                                      gsup43_c73_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_FIFO_OVRUN_E_set( gsup43_c73_buffer_t *b_ptr,
                                                      gsup43_c73_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_FIFO_OVRUN_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_FIFO_OVRUN_E_set", value );

    /* (0x0000005c bits 1) field FIFO_OVRUN_E of register PMC_GSUP43_C73_REG_TX_INT_EN */
    gsup43_c73_reg_TX_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_TX_INT_EN_BIT_FIFO_OVRUN_E_MSK,
                                        GSUP43_C73_REG_TX_INT_EN_BIT_FIFO_OVRUN_E_OFF,
                                        value);
}

static INLINE UINT32 gsup43_c73_field_FIFO_OVRUN_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_FIFO_OVRUN_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000005c bits 1) field FIFO_OVRUN_E of register PMC_GSUP43_C73_REG_TX_INT_EN */
    reg_value = gsup43_c73_reg_TX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_INT_EN_BIT_FIFO_OVRUN_E_MSK) >> GSUP43_C73_REG_TX_INT_EN_BIT_FIFO_OVRUN_E_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_FIFO_OVRUN_E_get", value );

    return value;
}
static INLINE void gsup43_c73_field_FIFO_UNDRUN_E_set( gsup43_c73_buffer_t *b_ptr,
                                                       gsup43_c73_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void gsup43_c73_field_FIFO_UNDRUN_E_set( gsup43_c73_buffer_t *b_ptr,
                                                       gsup43_c73_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gsup43_c73_field_FIFO_UNDRUN_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gsup43_c73_field_FIFO_UNDRUN_E_set", value );

    /* (0x0000005c bits 0) field FIFO_UNDRUN_E of register PMC_GSUP43_C73_REG_TX_INT_EN */
    gsup43_c73_reg_TX_INT_EN_field_set( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_TX_INT_EN_BIT_FIFO_UNDRUN_E_MSK,
                                        GSUP43_C73_REG_TX_INT_EN_BIT_FIFO_UNDRUN_E_OFF,
                                        value);
}

static INLINE UINT32 gsup43_c73_field_FIFO_UNDRUN_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_FIFO_UNDRUN_E_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000005c bits 0) field FIFO_UNDRUN_E of register PMC_GSUP43_C73_REG_TX_INT_EN */
    reg_value = gsup43_c73_reg_TX_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_INT_EN_BIT_FIFO_UNDRUN_E_MSK) >> GSUP43_C73_REG_TX_INT_EN_BIT_FIFO_UNDRUN_E_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_FIFO_UNDRUN_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value
 * ==================================================================================
 */
static INLINE UINT32 gsup43_c73_field_RX_LF_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_LF_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 9) field RX_LF_V of register PMC_GSUP43_C73_REG_RX_STAT */
    reg_value = gsup43_c73_reg_RX_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_STAT_BIT_RX_LF_V_MSK) >> GSUP43_C73_REG_RX_STAT_BIT_RX_LF_V_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_LF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_LF_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                  gsup43_c73_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_LF_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                  gsup43_c73_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_RX_LF_V_poll", value );

    /* (0x00000018 bits 9) field RX_LF_V of register PMC_GSUP43_C73_REG_RX_STAT */
    return gsup43_c73_reg_RX_STAT_poll( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_RX_STAT_BIT_RX_LF_V_MSK,
                                        (value<<GSUP43_C73_REG_RX_STAT_BIT_RX_LF_V_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 gsup43_c73_field_RX_RF_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_RF_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 8) field RX_RF_V of register PMC_GSUP43_C73_REG_RX_STAT */
    reg_value = gsup43_c73_reg_RX_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_STAT_BIT_RX_RF_V_MSK) >> GSUP43_C73_REG_RX_STAT_BIT_RX_RF_V_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_RF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_RF_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                  gsup43_c73_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_RF_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                  gsup43_c73_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_RX_RF_V_poll", value );

    /* (0x00000018 bits 8) field RX_RF_V of register PMC_GSUP43_C73_REG_RX_STAT */
    return gsup43_c73_reg_RX_STAT_poll( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_RX_STAT_BIT_RX_RF_V_MSK,
                                        (value<<GSUP43_C73_REG_RX_STAT_BIT_RX_RF_V_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 gsup43_c73_field_ALL_ONES_OR_ZEROS_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_ALL_ONES_OR_ZEROS_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 7) field ALL_ONES_OR_ZEROS_V of register PMC_GSUP43_C73_REG_RX_STAT */
    reg_value = gsup43_c73_reg_RX_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_STAT_BIT_ALL_ONES_OR_ZEROS_V_MSK) >> GSUP43_C73_REG_RX_STAT_BIT_ALL_ONES_OR_ZEROS_V_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_ALL_ONES_OR_ZEROS_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_ALL_ONES_OR_ZEROS_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                              gsup43_c73_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_ALL_ONES_OR_ZEROS_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                              gsup43_c73_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_ALL_ONES_OR_ZEROS_V_poll", value );

    /* (0x00000018 bits 7) field ALL_ONES_OR_ZEROS_V of register PMC_GSUP43_C73_REG_RX_STAT */
    return gsup43_c73_reg_RX_STAT_poll( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_RX_STAT_BIT_ALL_ONES_OR_ZEROS_V_MSK,
                                        (value<<GSUP43_C73_REG_RX_STAT_BIT_ALL_ONES_OR_ZEROS_V_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 gsup43_c73_field_RX_FAULT_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_FAULT_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 4) field RX_FAULT_V of register PMC_GSUP43_C73_REG_RX_STAT */
    reg_value = gsup43_c73_reg_RX_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_STAT_BIT_RX_FAULT_V_MSK) >> GSUP43_C73_REG_RX_STAT_BIT_RX_FAULT_V_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_FAULT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_FAULT_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                     gsup43_c73_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_FAULT_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                     gsup43_c73_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_RX_FAULT_V_poll", value );

    /* (0x00000018 bits 4) field RX_FAULT_V of register PMC_GSUP43_C73_REG_RX_STAT */
    return gsup43_c73_reg_RX_STAT_poll( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_RX_STAT_BIT_RX_FAULT_V_MSK,
                                        (value<<GSUP43_C73_REG_RX_STAT_BIT_RX_FAULT_V_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 gsup43_c73_field_LINK_FAIL_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_LINK_FAIL_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 3) field LINK_FAIL_V of register PMC_GSUP43_C73_REG_RX_STAT */
    reg_value = gsup43_c73_reg_RX_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_STAT_BIT_LINK_FAIL_V_MSK) >> GSUP43_C73_REG_RX_STAT_BIT_LINK_FAIL_V_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_LINK_FAIL_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_LINK_FAIL_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                      gsup43_c73_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_LINK_FAIL_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                      gsup43_c73_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_LINK_FAIL_V_poll", value );

    /* (0x00000018 bits 3) field LINK_FAIL_V of register PMC_GSUP43_C73_REG_RX_STAT */
    return gsup43_c73_reg_RX_STAT_poll( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_RX_STAT_BIT_LINK_FAIL_V_MSK,
                                        (value<<GSUP43_C73_REG_RX_STAT_BIT_LINK_FAIL_V_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 gsup43_c73_field_HI_BER_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_HI_BER_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 2) field HI_BER_V of register PMC_GSUP43_C73_REG_RX_STAT */
    reg_value = gsup43_c73_reg_RX_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_STAT_BIT_HI_BER_V_MSK) >> GSUP43_C73_REG_RX_STAT_BIT_HI_BER_V_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_HI_BER_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_HI_BER_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                   gsup43_c73_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_HI_BER_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                   gsup43_c73_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_HI_BER_V_poll", value );

    /* (0x00000018 bits 2) field HI_BER_V of register PMC_GSUP43_C73_REG_RX_STAT */
    return gsup43_c73_reg_RX_STAT_poll( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_RX_STAT_BIT_HI_BER_V_MSK,
                                        (value<<GSUP43_C73_REG_RX_STAT_BIT_HI_BER_V_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 gsup43_c73_field_LOS_SYNC_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_LOS_SYNC_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 1) field LOS_SYNC_V of register PMC_GSUP43_C73_REG_RX_STAT */
    reg_value = gsup43_c73_reg_RX_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_STAT_BIT_LOS_SYNC_V_MSK) >> GSUP43_C73_REG_RX_STAT_BIT_LOS_SYNC_V_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_LOS_SYNC_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_LOS_SYNC_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                     gsup43_c73_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_LOS_SYNC_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                     gsup43_c73_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_LOS_SYNC_V_poll", value );

    /* (0x00000018 bits 1) field LOS_SYNC_V of register PMC_GSUP43_C73_REG_RX_STAT */
    return gsup43_c73_reg_RX_STAT_poll( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_RX_STAT_BIT_LOS_SYNC_V_MSK,
                                        (value<<GSUP43_C73_REG_RX_STAT_BIT_LOS_SYNC_V_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 gsup43_c73_field_LOS_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_LOS_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 0) field LOS_V of register PMC_GSUP43_C73_REG_RX_STAT */
    reg_value = gsup43_c73_reg_RX_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_STAT_BIT_LOS_V_MSK) >> GSUP43_C73_REG_RX_STAT_BIT_LOS_V_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_LOS_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_LOS_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                gsup43_c73_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_LOS_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                gsup43_c73_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_LOS_V_poll", value );

    /* (0x00000018 bits 0) field LOS_V of register PMC_GSUP43_C73_REG_RX_STAT */
    return gsup43_c73_reg_RX_STAT_poll( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_RX_STAT_BIT_LOS_V_MSK,
                                        (value<<GSUP43_C73_REG_RX_STAT_BIT_LOS_V_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 gsup43_c73_field_SEND_IDLE_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_SEND_IDLE_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000064 bits 4) field SEND_IDLE_V of register PMC_GSUP43_C73_REG_TX_STAT */
    reg_value = gsup43_c73_reg_TX_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_STAT_BIT_SEND_IDLE_V_MSK) >> GSUP43_C73_REG_TX_STAT_BIT_SEND_IDLE_V_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_SEND_IDLE_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_SEND_IDLE_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                      gsup43_c73_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_SEND_IDLE_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                      gsup43_c73_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_SEND_IDLE_V_poll", value );

    /* (0x00000064 bits 4) field SEND_IDLE_V of register PMC_GSUP43_C73_REG_TX_STAT */
    return gsup43_c73_reg_TX_STAT_poll( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_TX_STAT_BIT_SEND_IDLE_V_MSK,
                                        (value<<GSUP43_C73_REG_TX_STAT_BIT_SEND_IDLE_V_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 gsup43_c73_field_SEND_LF_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_SEND_LF_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000064 bits 3) field SEND_LF_V of register PMC_GSUP43_C73_REG_TX_STAT */
    reg_value = gsup43_c73_reg_TX_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_STAT_BIT_SEND_LF_V_MSK) >> GSUP43_C73_REG_TX_STAT_BIT_SEND_LF_V_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_SEND_LF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_SEND_LF_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                    gsup43_c73_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_SEND_LF_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                    gsup43_c73_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_SEND_LF_V_poll", value );

    /* (0x00000064 bits 3) field SEND_LF_V of register PMC_GSUP43_C73_REG_TX_STAT */
    return gsup43_c73_reg_TX_STAT_poll( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_TX_STAT_BIT_SEND_LF_V_MSK,
                                        (value<<GSUP43_C73_REG_TX_STAT_BIT_SEND_LF_V_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 gsup43_c73_field_SEND_RF_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_SEND_RF_V_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000064 bits 2) field SEND_RF_V of register PMC_GSUP43_C73_REG_TX_STAT */
    reg_value = gsup43_c73_reg_TX_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_TX_STAT_BIT_SEND_RF_V_MSK) >> GSUP43_C73_REG_TX_STAT_BIT_SEND_RF_V_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_SEND_RF_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_SEND_RF_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                    gsup43_c73_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gsup43_c73_field_SEND_RF_V_poll( gsup43_c73_buffer_t *b_ptr,
                                                                    gsup43_c73_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "gsup43_c73_field_SEND_RF_V_poll", value );

    /* (0x00000064 bits 2) field SEND_RF_V of register PMC_GSUP43_C73_REG_TX_STAT */
    return gsup43_c73_reg_TX_STAT_poll( b_ptr,
                                        h_ptr,
                                        GSUP43_C73_REG_TX_STAT_BIT_SEND_RF_V_MSK,
                                        (value<<GSUP43_C73_REG_TX_STAT_BIT_SEND_RF_V_OFF),
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
static INLINE UINT32 gsup43_c73_field_BLK_TYPE_ERR_CNT_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_BLK_TYPE_ERR_CNT_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 15:8) bits 0:7 use field BLK_TYPE_ERR_CNT of register PMC_GSUP43_C73_REG_RX_FRAME_ERR_CNT */
    reg_value = gsup43_c73_reg_RX_FRAME_ERR_CNT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_FRAME_ERR_CNT_BIT_BLK_TYPE_ERR_CNT_MSK) >> GSUP43_C73_REG_RX_FRAME_ERR_CNT_BIT_BLK_TYPE_ERR_CNT_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_BLK_TYPE_ERR_CNT_get", value );

    return value;
}
static INLINE UINT32 gsup43_c73_field_range_BLK_TYPE_ERR_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                                  gsup43_c73_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_range_BLK_TYPE_ERR_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                                  gsup43_c73_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_BLK_TYPE_ERR_CNT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_BLK_TYPE_ERR_CNT_get", stop_bit, 7 );
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
    /* (0x0000001c bits 15:8) bits 0:7 use field BLK_TYPE_ERR_CNT of register PMC_GSUP43_C73_REG_RX_FRAME_ERR_CNT */
    reg_value = gsup43_c73_reg_RX_FRAME_ERR_CNT_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_RX_FRAME_ERR_CNT_BIT_BLK_TYPE_ERR_CNT_MSK)
                  >> GSUP43_C73_REG_RX_FRAME_ERR_CNT_BIT_BLK_TYPE_ERR_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_RX_FRAME_ERR_CNT_BIT_BLK_TYPE_ERR_CNT_MSK, GSUP43_C73_REG_RX_FRAME_ERR_CNT_BIT_BLK_TYPE_ERR_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_BLK_TYPE_ERR_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 gsup43_c73_field_HI_BER_CNT_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_HI_BER_CNT_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 7:0) bits 0:7 use field HI_BER_CNT of register PMC_GSUP43_C73_REG_RX_FRAME_ERR_CNT */
    reg_value = gsup43_c73_reg_RX_FRAME_ERR_CNT_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_FRAME_ERR_CNT_BIT_HI_BER_CNT_MSK) >> GSUP43_C73_REG_RX_FRAME_ERR_CNT_BIT_HI_BER_CNT_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_HI_BER_CNT_get", value );

    return value;
}
static INLINE UINT32 gsup43_c73_field_range_HI_BER_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_range_HI_BER_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_HI_BER_CNT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_HI_BER_CNT_get", stop_bit, 7 );
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
    /* (0x0000001c bits 7:0) bits 0:7 use field HI_BER_CNT of register PMC_GSUP43_C73_REG_RX_FRAME_ERR_CNT */
    reg_value = gsup43_c73_reg_RX_FRAME_ERR_CNT_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_RX_FRAME_ERR_CNT_BIT_HI_BER_CNT_MSK)
                  >> GSUP43_C73_REG_RX_FRAME_ERR_CNT_BIT_HI_BER_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_RX_FRAME_ERR_CNT_BIT_HI_BER_CNT_MSK, GSUP43_C73_REG_RX_FRAME_ERR_CNT_BIT_HI_BER_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_HI_BER_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 gsup43_c73_field_RX_BLK_ERR_CNT_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RX_BLK_ERR_CNT_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 31:0) bits 0:31 use field RX_BLK_ERR_CNT of register PMC_GSUP43_C73_REG_RX_BLOCK_ERR_CNTS */
    reg_value = gsup43_c73_reg_RX_BLOCK_ERR_CNTS_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_BLOCK_ERR_CNTS_BIT_RX_BLK_ERR_CNT_MSK) >> GSUP43_C73_REG_RX_BLOCK_ERR_CNTS_BIT_RX_BLK_ERR_CNT_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RX_BLK_ERR_CNT_get", value );

    return value;
}
static INLINE UINT32 gsup43_c73_field_range_RX_BLK_ERR_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                                gsup43_c73_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_range_RX_BLK_ERR_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                                gsup43_c73_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_RX_BLK_ERR_CNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_RX_BLK_ERR_CNT_get", stop_bit, 31 );
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
    /* (0x00000020 bits 31:0) bits 0:31 use field RX_BLK_ERR_CNT of register PMC_GSUP43_C73_REG_RX_BLOCK_ERR_CNTS */
    reg_value = gsup43_c73_reg_RX_BLOCK_ERR_CNTS_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_RX_BLOCK_ERR_CNTS_BIT_RX_BLK_ERR_CNT_MSK)
                  >> GSUP43_C73_REG_RX_BLOCK_ERR_CNTS_BIT_RX_BLK_ERR_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_RX_BLOCK_ERR_CNTS_BIT_RX_BLK_ERR_CNT_MSK, GSUP43_C73_REG_RX_BLOCK_ERR_CNTS_BIT_RX_BLK_ERR_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_RX_BLK_ERR_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void gsup43_c73_lfield_RX_OS_DSCRCD_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void gsup43_c73_lfield_RX_OS_DSCRCD_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x00000024 bits 31:0) bits 0:31 use field RX_OS_DSCRCD_CNT of register PMC_GSUP43_C73_REG_RX_DISCARDED_ORDERED_SET_CNT_LSB */
    reg_value = gsup43_c73_reg_RX_DISCARDED_ORDERED_SET_CNT_LSB_read( b_ptr,
                                                                      h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_RX_DISCARDED_ORDERED_SET_CNT_LSB_BIT_RX_OS_DSCRCD_CNT_MSK) >> GSUP43_C73_REG_RX_DISCARDED_ORDERED_SET_CNT_LSB_BIT_RX_OS_DSCRCD_CNT_OFF;
    value[0] |= field_value;

    /* (0x00000028 bits 7:0) bits 32:39 use field RX_OS_DSCRCD_CNT of register PMC_GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT */
    reg_value = gsup43_c73_reg_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_read( b_ptr,
                                                                                 h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_BIT_RX_OS_DSCRCD_CNT_MSK) >> GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_BIT_RX_OS_DSCRCD_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "gsup43_c73_lfield_RX_OS_DSCRCD_CNT_get", value[1] , value[0] );

}
static INLINE UINT32 gsup43_c73_lfield_range_RX_OS_DSCRCD_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                                   gsup43_c73_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_lfield_range_RX_OS_DSCRCD_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                                   gsup43_c73_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_lfield_range_RX_OS_DSCRCD_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_lfield_range_RX_OS_DSCRCD_CNT_get", stop_bit, 39 );
    if (start_bit <= 31) {
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
    /* (0x00000024 bits 31:0) bits 0:31 use field RX_OS_DSCRCD_CNT of register PMC_GSUP43_C73_REG_RX_DISCARDED_ORDERED_SET_CNT_LSB */
        reg_value = gsup43_c73_reg_RX_DISCARDED_ORDERED_SET_CNT_LSB_read( b_ptr,
                                                                          h_ptr);
        field_value = (reg_value & GSUP43_C73_REG_RX_DISCARDED_ORDERED_SET_CNT_LSB_BIT_RX_OS_DSCRCD_CNT_MSK)
                      >> GSUP43_C73_REG_RX_DISCARDED_ORDERED_SET_CNT_LSB_BIT_RX_OS_DSCRCD_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_RX_DISCARDED_ORDERED_SET_CNT_LSB_BIT_RX_OS_DSCRCD_CNT_MSK, GSUP43_C73_REG_RX_DISCARDED_ORDERED_SET_CNT_LSB_BIT_RX_OS_DSCRCD_CNT_OFF, field_value );
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
    /* (0x00000028 bits 7:0) bits 32:39 use field RX_OS_DSCRCD_CNT of register PMC_GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT */
        reg_value = gsup43_c73_reg_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_read( b_ptr,
                                                                                     h_ptr);
        field_value = (reg_value & GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_BIT_RX_OS_DSCRCD_CNT_MSK)
                      >> GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_BIT_RX_OS_DSCRCD_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_BIT_RX_OS_DSCRCD_CNT_MSK, GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_BIT_RX_OS_DSCRCD_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_lfield_range_RX_OS_DSCRCD_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 gsup43_c73_field_PRBS31_ERR_CNT_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_PRBS31_ERR_CNT_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 31:16) bits 0:15 use field PRBS31_ERR_CNT of register PMC_GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT */
    reg_value = gsup43_c73_reg_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_read( b_ptr,
                                                                                 h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_BIT_PRBS31_ERR_CNT_MSK) >> GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_BIT_PRBS31_ERR_CNT_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_PRBS31_ERR_CNT_get", value );

    return value;
}
static INLINE UINT32 gsup43_c73_field_range_PRBS31_ERR_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                                gsup43_c73_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_range_PRBS31_ERR_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                                gsup43_c73_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_field_range_PRBS31_ERR_CNT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_field_range_PRBS31_ERR_CNT_get", stop_bit, 15 );
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
    /* (0x00000028 bits 31:16) bits 0:15 use field PRBS31_ERR_CNT of register PMC_GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT */
    reg_value = gsup43_c73_reg_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_read( b_ptr,
                                                                                 h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_BIT_PRBS31_ERR_CNT_MSK)
                  >> GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_BIT_PRBS31_ERR_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_BIT_PRBS31_ERR_CNT_MSK, GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_BIT_PRBS31_ERR_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_field_range_PRBS31_ERR_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void gsup43_c73_lfield_RX_DATA_BYTE_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void gsup43_c73_lfield_RX_DATA_BYTE_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x0000002c bits 31:0) bits 0:31 use field RX_DATA_BYTE_CNT of register PMC_GSUP43_C73_REG_RX_DATA_BYTE_CNT_LSB */
    reg_value = gsup43_c73_reg_RX_DATA_BYTE_CNT_LSB_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_RX_DATA_BYTE_CNT_LSB_BIT_RX_DATA_BYTE_CNT_MSK) >> GSUP43_C73_REG_RX_DATA_BYTE_CNT_LSB_BIT_RX_DATA_BYTE_CNT_OFF;
    value[0] |= field_value;

    /* (0x00000034 bits 31:24) bits 32:39 use field RX_DATA_BYTE_CNT of register PMC_GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB */
    reg_value = gsup43_c73_reg_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_read( b_ptr,
                                                                          h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_RX_DATA_BYTE_CNT_MSK) >> GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_RX_DATA_BYTE_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "gsup43_c73_lfield_RX_DATA_BYTE_CNT_get", value[1] , value[0] );

}
static INLINE UINT32 gsup43_c73_lfield_range_RX_DATA_BYTE_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                                   gsup43_c73_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_lfield_range_RX_DATA_BYTE_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                                   gsup43_c73_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_lfield_range_RX_DATA_BYTE_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_lfield_range_RX_DATA_BYTE_CNT_get", stop_bit, 39 );
    if (start_bit <= 31) {
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
    /* (0x0000002c bits 31:0) bits 0:31 use field RX_DATA_BYTE_CNT of register PMC_GSUP43_C73_REG_RX_DATA_BYTE_CNT_LSB */
        reg_value = gsup43_c73_reg_RX_DATA_BYTE_CNT_LSB_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & GSUP43_C73_REG_RX_DATA_BYTE_CNT_LSB_BIT_RX_DATA_BYTE_CNT_MSK)
                      >> GSUP43_C73_REG_RX_DATA_BYTE_CNT_LSB_BIT_RX_DATA_BYTE_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_RX_DATA_BYTE_CNT_LSB_BIT_RX_DATA_BYTE_CNT_MSK, GSUP43_C73_REG_RX_DATA_BYTE_CNT_LSB_BIT_RX_DATA_BYTE_CNT_OFF, field_value );
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
    /* (0x00000034 bits 31:24) bits 32:39 use field RX_DATA_BYTE_CNT of register PMC_GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB */
        reg_value = gsup43_c73_reg_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_read( b_ptr,
                                                                              h_ptr);
        field_value = (reg_value & GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_RX_DATA_BYTE_CNT_MSK)
                      >> GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_RX_DATA_BYTE_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_RX_DATA_BYTE_CNT_MSK, GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_RX_DATA_BYTE_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_lfield_range_RX_DATA_BYTE_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void gsup43_c73_lfield_RX_OS_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void gsup43_c73_lfield_RX_OS_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x00000030 bits 31:0) bits 0:31 use field RX_OS_CNT of register PMC_GSUP43_C73_REG_RX_ORDERED_SET_CNT_LSB */
    reg_value = gsup43_c73_reg_RX_ORDERED_SET_CNT_LSB_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_RX_ORDERED_SET_CNT_LSB_BIT_RX_OS_CNT_MSK) >> GSUP43_C73_REG_RX_ORDERED_SET_CNT_LSB_BIT_RX_OS_CNT_OFF;
    value[0] |= field_value;

    /* (0x00000034 bits 7:0) bits 32:39 use field RX_OS_CNT of register PMC_GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB */
    reg_value = gsup43_c73_reg_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_read( b_ptr,
                                                                          h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_RX_OS_CNT_MSK) >> GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_RX_OS_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "gsup43_c73_lfield_RX_OS_CNT_get", value[1] , value[0] );

}
static INLINE UINT32 gsup43_c73_lfield_range_RX_OS_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_lfield_range_RX_OS_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_lfield_range_RX_OS_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_lfield_range_RX_OS_CNT_get", stop_bit, 39 );
    if (start_bit <= 31) {
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
    /* (0x00000030 bits 31:0) bits 0:31 use field RX_OS_CNT of register PMC_GSUP43_C73_REG_RX_ORDERED_SET_CNT_LSB */
        reg_value = gsup43_c73_reg_RX_ORDERED_SET_CNT_LSB_read( b_ptr,
                                                                h_ptr);
        field_value = (reg_value & GSUP43_C73_REG_RX_ORDERED_SET_CNT_LSB_BIT_RX_OS_CNT_MSK)
                      >> GSUP43_C73_REG_RX_ORDERED_SET_CNT_LSB_BIT_RX_OS_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_RX_ORDERED_SET_CNT_LSB_BIT_RX_OS_CNT_MSK, GSUP43_C73_REG_RX_ORDERED_SET_CNT_LSB_BIT_RX_OS_CNT_OFF, field_value );
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
    /* (0x00000034 bits 7:0) bits 32:39 use field RX_OS_CNT of register PMC_GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB */
        reg_value = gsup43_c73_reg_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_read( b_ptr,
                                                                              h_ptr);
        field_value = (reg_value & GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_RX_OS_CNT_MSK)
                      >> GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_RX_OS_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_RX_OS_CNT_MSK, GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_RX_OS_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_lfield_range_RX_OS_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void gsup43_c73_lfield_RX_PKT_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                     gsup43_c73_handle_t *h_ptr,
                                                     UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void gsup43_c73_lfield_RX_PKT_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                     gsup43_c73_handle_t *h_ptr,
                                                     UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x00000038 bits 31:0) bits 0:31 use field RX_PKT_CNT of register PMC_GSUP43_C73_REG_RX_PACKET_CNT_LSB */
    reg_value = gsup43_c73_reg_RX_PACKET_CNT_LSB_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_RX_PACKET_CNT_LSB_BIT_RX_PKT_CNT_MSK) >> GSUP43_C73_REG_RX_PACKET_CNT_LSB_BIT_RX_PKT_CNT_OFF;
    value[0] |= field_value;

    /* (0x0000003c bits 7:0) bits 32:39 use field RX_PKT_CNT of register PMC_GSUP43_C73_REG_RX_PACKET_CNT_MSB */
    reg_value = gsup43_c73_reg_RX_PACKET_CNT_MSB_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_RX_PACKET_CNT_MSB_BIT_RX_PKT_CNT_MSK) >> GSUP43_C73_REG_RX_PACKET_CNT_MSB_BIT_RX_PKT_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "gsup43_c73_lfield_RX_PKT_CNT_get", value[1] , value[0] );

}
static INLINE UINT32 gsup43_c73_lfield_range_RX_PKT_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                             gsup43_c73_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_lfield_range_RX_PKT_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                             gsup43_c73_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_lfield_range_RX_PKT_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_lfield_range_RX_PKT_CNT_get", stop_bit, 39 );
    if (start_bit <= 31) {
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
    /* (0x00000038 bits 31:0) bits 0:31 use field RX_PKT_CNT of register PMC_GSUP43_C73_REG_RX_PACKET_CNT_LSB */
        reg_value = gsup43_c73_reg_RX_PACKET_CNT_LSB_read( b_ptr,
                                                           h_ptr);
        field_value = (reg_value & GSUP43_C73_REG_RX_PACKET_CNT_LSB_BIT_RX_PKT_CNT_MSK)
                      >> GSUP43_C73_REG_RX_PACKET_CNT_LSB_BIT_RX_PKT_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_RX_PACKET_CNT_LSB_BIT_RX_PKT_CNT_MSK, GSUP43_C73_REG_RX_PACKET_CNT_LSB_BIT_RX_PKT_CNT_OFF, field_value );
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
    /* (0x0000003c bits 7:0) bits 32:39 use field RX_PKT_CNT of register PMC_GSUP43_C73_REG_RX_PACKET_CNT_MSB */
        reg_value = gsup43_c73_reg_RX_PACKET_CNT_MSB_read( b_ptr,
                                                           h_ptr);
        field_value = (reg_value & GSUP43_C73_REG_RX_PACKET_CNT_MSB_BIT_RX_PKT_CNT_MSK)
                      >> GSUP43_C73_REG_RX_PACKET_CNT_MSB_BIT_RX_PKT_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_RX_PACKET_CNT_MSB_BIT_RX_PKT_CNT_MSK, GSUP43_C73_REG_RX_PACKET_CNT_MSB_BIT_RX_PKT_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_lfield_range_RX_PKT_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void gsup43_c73_lfield_TX_PKT_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                     gsup43_c73_handle_t *h_ptr,
                                                     UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void gsup43_c73_lfield_TX_PKT_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                     gsup43_c73_handle_t *h_ptr,
                                                     UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x00000048 bits 31:0) bits 0:31 use field TX_PKT_CNT of register PMC_GSUP43_C73_REG_TX_PACKET_CNT_LSB */
    reg_value = gsup43_c73_reg_TX_PACKET_CNT_LSB_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_TX_PACKET_CNT_LSB_BIT_TX_PKT_CNT_MSK) >> GSUP43_C73_REG_TX_PACKET_CNT_LSB_BIT_TX_PKT_CNT_OFF;
    value[0] |= field_value;

    /* (0x0000004c bits 7:0) bits 32:39 use field TX_PKT_CNT of register PMC_GSUP43_C73_REG_TX_PACKET_CNT_MSB */
    reg_value = gsup43_c73_reg_TX_PACKET_CNT_MSB_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_TX_PACKET_CNT_MSB_BIT_TX_PKT_CNT_MSK) >> GSUP43_C73_REG_TX_PACKET_CNT_MSB_BIT_TX_PKT_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "gsup43_c73_lfield_TX_PKT_CNT_get", value[1] , value[0] );

}
static INLINE UINT32 gsup43_c73_lfield_range_TX_PKT_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                             gsup43_c73_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_lfield_range_TX_PKT_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                             gsup43_c73_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_lfield_range_TX_PKT_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_lfield_range_TX_PKT_CNT_get", stop_bit, 39 );
    if (start_bit <= 31) {
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
    /* (0x00000048 bits 31:0) bits 0:31 use field TX_PKT_CNT of register PMC_GSUP43_C73_REG_TX_PACKET_CNT_LSB */
        reg_value = gsup43_c73_reg_TX_PACKET_CNT_LSB_read( b_ptr,
                                                           h_ptr);
        field_value = (reg_value & GSUP43_C73_REG_TX_PACKET_CNT_LSB_BIT_TX_PKT_CNT_MSK)
                      >> GSUP43_C73_REG_TX_PACKET_CNT_LSB_BIT_TX_PKT_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_TX_PACKET_CNT_LSB_BIT_TX_PKT_CNT_MSK, GSUP43_C73_REG_TX_PACKET_CNT_LSB_BIT_TX_PKT_CNT_OFF, field_value );
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
    /* (0x0000004c bits 7:0) bits 32:39 use field TX_PKT_CNT of register PMC_GSUP43_C73_REG_TX_PACKET_CNT_MSB */
        reg_value = gsup43_c73_reg_TX_PACKET_CNT_MSB_read( b_ptr,
                                                           h_ptr);
        field_value = (reg_value & GSUP43_C73_REG_TX_PACKET_CNT_MSB_BIT_TX_PKT_CNT_MSK)
                      >> GSUP43_C73_REG_TX_PACKET_CNT_MSB_BIT_TX_PKT_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_TX_PACKET_CNT_MSB_BIT_TX_PKT_CNT_MSK, GSUP43_C73_REG_TX_PACKET_CNT_MSB_BIT_TX_PKT_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_lfield_range_TX_PKT_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void gsup43_c73_lfield_TX_DATA_BYTE_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void gsup43_c73_lfield_TX_DATA_BYTE_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                           gsup43_c73_handle_t *h_ptr,
                                                           UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x00000050 bits 31:0) bits 0:31 use field TX_DATA_BYTE_CNT of register PMC_GSUP43_C73_REG_TX_DATA_BYTE_CNT_LSB */
    reg_value = gsup43_c73_reg_TX_DATA_BYTE_CNT_LSB_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_TX_DATA_BYTE_CNT_LSB_BIT_TX_DATA_BYTE_CNT_MSK) >> GSUP43_C73_REG_TX_DATA_BYTE_CNT_LSB_BIT_TX_DATA_BYTE_CNT_OFF;
    value[0] |= field_value;

    /* (0x00000058 bits 15:8) bits 32:39 use field TX_DATA_BYTE_CNT of register PMC_GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB */
    reg_value = gsup43_c73_reg_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_read( b_ptr,
                                                                          h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_TX_DATA_BYTE_CNT_MSK) >> GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_TX_DATA_BYTE_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "gsup43_c73_lfield_TX_DATA_BYTE_CNT_get", value[1] , value[0] );

}
static INLINE UINT32 gsup43_c73_lfield_range_TX_DATA_BYTE_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                                   gsup43_c73_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_lfield_range_TX_DATA_BYTE_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                                   gsup43_c73_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_lfield_range_TX_DATA_BYTE_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_lfield_range_TX_DATA_BYTE_CNT_get", stop_bit, 39 );
    if (start_bit <= 31) {
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
    /* (0x00000050 bits 31:0) bits 0:31 use field TX_DATA_BYTE_CNT of register PMC_GSUP43_C73_REG_TX_DATA_BYTE_CNT_LSB */
        reg_value = gsup43_c73_reg_TX_DATA_BYTE_CNT_LSB_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & GSUP43_C73_REG_TX_DATA_BYTE_CNT_LSB_BIT_TX_DATA_BYTE_CNT_MSK)
                      >> GSUP43_C73_REG_TX_DATA_BYTE_CNT_LSB_BIT_TX_DATA_BYTE_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_TX_DATA_BYTE_CNT_LSB_BIT_TX_DATA_BYTE_CNT_MSK, GSUP43_C73_REG_TX_DATA_BYTE_CNT_LSB_BIT_TX_DATA_BYTE_CNT_OFF, field_value );
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
    /* (0x00000058 bits 15:8) bits 32:39 use field TX_DATA_BYTE_CNT of register PMC_GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB */
        reg_value = gsup43_c73_reg_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_read( b_ptr,
                                                                              h_ptr);
        field_value = (reg_value & GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_TX_DATA_BYTE_CNT_MSK)
                      >> GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_TX_DATA_BYTE_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_TX_DATA_BYTE_CNT_MSK, GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_TX_DATA_BYTE_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_lfield_range_TX_DATA_BYTE_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void gsup43_c73_lfield_TX_OS_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void gsup43_c73_lfield_TX_OS_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                    gsup43_c73_handle_t *h_ptr,
                                                    UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x00000054 bits 31:0) bits 0:31 use field TX_OS_CNT of register PMC_GSUP43_C73_REG_TX_ORDERED_SET_CNT_LSB */
    reg_value = gsup43_c73_reg_TX_ORDERED_SET_CNT_LSB_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_TX_ORDERED_SET_CNT_LSB_BIT_TX_OS_CNT_MSK) >> GSUP43_C73_REG_TX_ORDERED_SET_CNT_LSB_BIT_TX_OS_CNT_OFF;
    value[0] |= field_value;

    /* (0x00000058 bits 7:0) bits 32:39 use field TX_OS_CNT of register PMC_GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB */
    reg_value = gsup43_c73_reg_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_read( b_ptr,
                                                                          h_ptr);
    field_value = (reg_value & GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_TX_OS_CNT_MSK) >> GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_TX_OS_CNT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "gsup43_c73_lfield_TX_OS_CNT_get", value[1] , value[0] );

}
static INLINE UINT32 gsup43_c73_lfield_range_TX_OS_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_lfield_range_TX_OS_CNT_get( gsup43_c73_buffer_t *b_ptr,
                                                            gsup43_c73_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gsup43_c73_lfield_range_TX_OS_CNT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gsup43_c73_lfield_range_TX_OS_CNT_get", stop_bit, 39 );
    if (start_bit <= 31) {
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
    /* (0x00000054 bits 31:0) bits 0:31 use field TX_OS_CNT of register PMC_GSUP43_C73_REG_TX_ORDERED_SET_CNT_LSB */
        reg_value = gsup43_c73_reg_TX_ORDERED_SET_CNT_LSB_read( b_ptr,
                                                                h_ptr);
        field_value = (reg_value & GSUP43_C73_REG_TX_ORDERED_SET_CNT_LSB_BIT_TX_OS_CNT_MSK)
                      >> GSUP43_C73_REG_TX_ORDERED_SET_CNT_LSB_BIT_TX_OS_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_TX_ORDERED_SET_CNT_LSB_BIT_TX_OS_CNT_MSK, GSUP43_C73_REG_TX_ORDERED_SET_CNT_LSB_BIT_TX_OS_CNT_OFF, field_value );
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
    /* (0x00000058 bits 7:0) bits 32:39 use field TX_OS_CNT of register PMC_GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB */
        reg_value = gsup43_c73_reg_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_read( b_ptr,
                                                                              h_ptr);
        field_value = (reg_value & GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_TX_OS_CNT_MSK)
                      >> GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_TX_OS_CNT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_TX_OS_CNT_MSK, GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_TX_OS_CNT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gsup43_c73_lfield_range_TX_OS_CNT_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 gsup43_c73_field_RF_FAULT_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_RF_FAULT_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 31) field RF_FAULT of register PMC_GSUP43_C73_REG_RX_STAT */
    reg_value = gsup43_c73_reg_RX_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_STAT_BIT_RF_FAULT_MSK) >> GSUP43_C73_REG_RX_STAT_BIT_RF_FAULT_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_RF_FAULT_get", value );

    return value;
}
static INLINE UINT32 gsup43_c73_field_LF_FAULT_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gsup43_c73_field_LF_FAULT_get( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 30) field LF_FAULT of register PMC_GSUP43_C73_REG_RX_STAT */
    reg_value = gsup43_c73_reg_RX_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & GSUP43_C73_REG_RX_STAT_BIT_LF_FAULT_MSK) >> GSUP43_C73_REG_RX_STAT_BIT_LF_FAULT_OFF;
    IOLOG( "%s -> 0x%08x", "gsup43_c73_field_LF_FAULT_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _GSUP43_C73_IO_INLINE_H */
