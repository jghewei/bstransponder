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
 *     and register accessor functions for the scpb_reg block
 *****************************************************************************/
#ifndef _SCPB_REG_IO_INLINE_H
#define _SCPB_REG_IO_INLINE_H

#include "cpb_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "scpb_reg_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SCPB_REG_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for scpb_reg
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
#  define IOLOG(...) vaLog( LOG_CALLER_ARGS __VA_ARGS__)
# endif
#endif
#ifndef IO_RANGE_CHECK
# ifdef NO_IO_LOGGING
#  define IO_RANGE_CHECK(...)
# else
#  define IO_RANGE_CHECK(...) vaLog( LOG_CALLER_ARGS "** ERROR: range check ** " __VA_ARGS__)
# endif
#endif
#ifndef IOLOG_DATA
# ifdef NO_IO_LOGGING
#  define IOLOG_DATA(...)
# else
#  define IOLOG_DATA(...) vaLogData( LOG_CALLER_ARGS __VA_ARGS__)
# endif
#endif
typedef struct {
    coalesce_buffer_t coalesce_handle[1]; /* only used if register coalescing is enabled */
    cpb_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} scpb_reg_buffer_t;

#ifndef scpb_reg_buffer_init
#define scpb_reg_buffer_init( b_ptr, h_ptr ) \
       _scpb_reg_buffer_init( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE void _scpb_reg_buffer_init( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "scpb_reg_buffer_init");
    
}

#ifndef scpb_reg_buffer_flush
#define scpb_reg_buffer_flush( b_ptr ) \
       _scpb_reg_buffer_flush( LOG_CALLER b_ptr )
#endif

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void _scpb_reg_buffer_flush( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr )
{
    IOLOG( "scpb_reg_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 scpb_reg_reg_read( scpb_reg_buffer_t *b_ptr,
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
static INLINE void scpb_reg_reg_write( scpb_reg_buffer_t *b_ptr,
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

static INLINE void scpb_reg_field_set( scpb_reg_buffer_t *b_ptr,
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

static INLINE void scpb_reg_action_on_write_field_set( scpb_reg_buffer_t *b_ptr,
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

static INLINE void scpb_reg_burst_read( scpb_reg_buffer_t *b_ptr,
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

static INLINE void scpb_reg_burst_write( scpb_reg_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE scpb_reg_poll( scpb_reg_buffer_t *b_ptr,
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
 *  register access functions for scpb_reg
 * ==================================================================================
 */

/**
 * accessor functions for register PMC_SCPB_REG_REG_REMAP_OLD_ADDR
 *
 * Register: Remap Block Request Old Address
 * Offset: 0x00104204 REMAP_OLD_ADDR
 *
 * bits 15:0 default 0000 0000 0000 0000 REMAP_OLD_BLOCK_ADDR
 */

#ifndef scpb_reg_reg_REMAP_OLD_ADDR_write
#define scpb_reg_reg_REMAP_OLD_ADDR_write( b_ptr, h_ptr, value ) \
       _scpb_reg_reg_REMAP_OLD_ADDR_write( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_reg_REMAP_OLD_ADDR_write( LOG_CALLER_DEFN
                                                       scpb_reg_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_REMAP_OLD_ADDR_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_REMAP_OLD_ADDR,
                        value);
}


#ifndef scpb_reg_reg_REMAP_OLD_ADDR_field_set
#define scpb_reg_reg_REMAP_OLD_ADDR_field_set( b_ptr, h_ptr, mask, ofs, value ) \
       _scpb_reg_reg_REMAP_OLD_ADDR_field_set( LOG_CALLER b_ptr, h_ptr, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_REMAP_OLD_ADDR_field_set( LOG_CALLER_DEFN
                                                           scpb_reg_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_REMAP_OLD_ADDR_field_set", mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_REMAP_OLD_ADDR,
                        mask,
                        PMC_SCPB_REG_REG_REMAP_OLD_ADDR_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_REMAP_OLD_ADDR_read
#define scpb_reg_reg_REMAP_OLD_ADDR_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_REMAP_OLD_ADDR_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_REMAP_OLD_ADDR_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SCPB_REG_REG_REMAP_OLD_ADDR);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_REMAP_OLD_ADDR_read", reg_value);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_REMAP_NEW_ADDR
 *
 * Register: Remap Block Request New Address
 * Offset: 0x00104208 REMAP_NEW_ADDR
 *
 * bits 15:0 default 0000 0000 0000 0000 REMAP_NEW_BLOCK_ADDR
 */

#ifndef scpb_reg_reg_REMAP_NEW_ADDR_write
#define scpb_reg_reg_REMAP_NEW_ADDR_write( b_ptr, h_ptr, value ) \
       _scpb_reg_reg_REMAP_NEW_ADDR_write( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_reg_REMAP_NEW_ADDR_write( LOG_CALLER_DEFN
                                                       scpb_reg_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_REMAP_NEW_ADDR_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_REMAP_NEW_ADDR,
                        value);
}


#ifndef scpb_reg_reg_REMAP_NEW_ADDR_field_set
#define scpb_reg_reg_REMAP_NEW_ADDR_field_set( b_ptr, h_ptr, mask, ofs, value ) \
       _scpb_reg_reg_REMAP_NEW_ADDR_field_set( LOG_CALLER b_ptr, h_ptr, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_REMAP_NEW_ADDR_field_set( LOG_CALLER_DEFN
                                                           scpb_reg_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_REMAP_NEW_ADDR_field_set", mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_REMAP_NEW_ADDR,
                        mask,
                        PMC_SCPB_REG_REG_REMAP_NEW_ADDR_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_REMAP_NEW_ADDR_read
#define scpb_reg_reg_REMAP_NEW_ADDR_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_REMAP_NEW_ADDR_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_REMAP_NEW_ADDR_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SCPB_REG_REG_REMAP_NEW_ADDR);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_REMAP_NEW_ADDR_read", reg_value);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_REMAP_MARGIN
 *
 * Register: Remap Margin
 * Offset: 0x00104210 REMAP_MARGIN
 *
 * bits 7:0 default 0000 0111 REMAP_WPTR_MARGIN
 */

#ifndef scpb_reg_reg_REMAP_MARGIN_write
#define scpb_reg_reg_REMAP_MARGIN_write( b_ptr, h_ptr, value ) \
       _scpb_reg_reg_REMAP_MARGIN_write( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_reg_REMAP_MARGIN_write( LOG_CALLER_DEFN
                                                     scpb_reg_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_REMAP_MARGIN_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_REMAP_MARGIN,
                        value);
}


#ifndef scpb_reg_reg_REMAP_MARGIN_field_set
#define scpb_reg_reg_REMAP_MARGIN_field_set( b_ptr, h_ptr, mask, ofs, value ) \
       _scpb_reg_reg_REMAP_MARGIN_field_set( LOG_CALLER b_ptr, h_ptr, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_REMAP_MARGIN_field_set( LOG_CALLER_DEFN
                                                         scpb_reg_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_REMAP_MARGIN_field_set", mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_REMAP_MARGIN,
                        mask,
                        PMC_SCPB_REG_REG_REMAP_MARGIN_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_REMAP_MARGIN_read
#define scpb_reg_reg_REMAP_MARGIN_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_REMAP_MARGIN_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_REMAP_MARGIN_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SCPB_REG_REG_REMAP_MARGIN);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_REMAP_MARGIN_read", reg_value);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_REMAP_OLD_BLK_CFG
 *
 * Register: Remap Old Block Config
 * Offset: 0x00104214 REMAP_OLD_BLK_CFG
 *
 * bits 23:16 default 0x00        REMAP_WQID
 *
 * bits 8:0   default 0 0000 0000 REMAP_LBA
 */

#ifndef scpb_reg_reg_REMAP_OLD_BLK_CFG_write
#define scpb_reg_reg_REMAP_OLD_BLK_CFG_write( b_ptr, h_ptr, value ) \
       _scpb_reg_reg_REMAP_OLD_BLK_CFG_write( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_reg_REMAP_OLD_BLK_CFG_write( LOG_CALLER_DEFN
                                                          scpb_reg_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_REMAP_OLD_BLK_CFG_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_REMAP_OLD_BLK_CFG,
                        value);
}


#ifndef scpb_reg_reg_REMAP_OLD_BLK_CFG_field_set
#define scpb_reg_reg_REMAP_OLD_BLK_CFG_field_set( b_ptr, h_ptr, mask, ofs, value ) \
       _scpb_reg_reg_REMAP_OLD_BLK_CFG_field_set( LOG_CALLER b_ptr, h_ptr, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_REMAP_OLD_BLK_CFG_field_set( LOG_CALLER_DEFN
                                                              scpb_reg_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_REMAP_OLD_BLK_CFG_field_set", mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_REMAP_OLD_BLK_CFG,
                        mask,
                        PMC_SCPB_REG_REG_REMAP_OLD_BLK_CFG_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_REMAP_OLD_BLK_CFG_read
#define scpb_reg_reg_REMAP_OLD_BLK_CFG_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_REMAP_OLD_BLK_CFG_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_REMAP_OLD_BLK_CFG_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SCPB_REG_REG_REMAP_OLD_BLK_CFG);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_REMAP_OLD_BLK_CFG_read", reg_value);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_REMAP_OLD_PAGE_CFG
 *
 * Register: Remap Old Page Config
 * Offset: 0x00104218 REMAP_OLD_PAGE_CFG
 *
 * bits 17:16 default 00  REMAP_QS_ID
 *
 * bits 9:8   default 00  REMAP_WP_ID
 */

#ifndef scpb_reg_reg_REMAP_OLD_PAGE_CFG_write
#define scpb_reg_reg_REMAP_OLD_PAGE_CFG_write( b_ptr, h_ptr, value ) \
       _scpb_reg_reg_REMAP_OLD_PAGE_CFG_write( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_reg_REMAP_OLD_PAGE_CFG_write( LOG_CALLER_DEFN
                                                           scpb_reg_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_REMAP_OLD_PAGE_CFG_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_REMAP_OLD_PAGE_CFG,
                        value);
}


#ifndef scpb_reg_reg_REMAP_OLD_PAGE_CFG_field_set
#define scpb_reg_reg_REMAP_OLD_PAGE_CFG_field_set( b_ptr, h_ptr, mask, ofs, value ) \
       _scpb_reg_reg_REMAP_OLD_PAGE_CFG_field_set( LOG_CALLER b_ptr, h_ptr, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_REMAP_OLD_PAGE_CFG_field_set( LOG_CALLER_DEFN
                                                               scpb_reg_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_REMAP_OLD_PAGE_CFG_field_set", mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_REMAP_OLD_PAGE_CFG,
                        mask,
                        PMC_SCPB_REG_REG_REMAP_OLD_PAGE_CFG_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_REMAP_OLD_PAGE_CFG_read
#define scpb_reg_reg_REMAP_OLD_PAGE_CFG_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_REMAP_OLD_PAGE_CFG_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_REMAP_OLD_PAGE_CFG_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SCPB_REG_REG_REMAP_OLD_PAGE_CFG);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_REMAP_OLD_PAGE_CFG_read", reg_value);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_FIFO_IPT_BYPASS_REG
 *
 * Register: IPT Bypass
 * Offset: 0x00103808 FIFO_IPT_BYPASS_REG
 *
 * bits 0 default 0  IPT_BYPASS
 */

#ifndef scpb_reg_reg_FIFO_IPT_BYPASS_REG_write
#define scpb_reg_reg_FIFO_IPT_BYPASS_REG_write( b_ptr, h_ptr, value ) \
       _scpb_reg_reg_FIFO_IPT_BYPASS_REG_write( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_reg_FIFO_IPT_BYPASS_REG_write( LOG_CALLER_DEFN
                                                            scpb_reg_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_FIFO_IPT_BYPASS_REG_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_FIFO_IPT_BYPASS_REG,
                        value);
}


#ifndef scpb_reg_reg_FIFO_IPT_BYPASS_REG_field_set
#define scpb_reg_reg_FIFO_IPT_BYPASS_REG_field_set( b_ptr, h_ptr, mask, ofs, value ) \
       _scpb_reg_reg_FIFO_IPT_BYPASS_REG_field_set( LOG_CALLER b_ptr, h_ptr, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_FIFO_IPT_BYPASS_REG_field_set( LOG_CALLER_DEFN
                                                                scpb_reg_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_FIFO_IPT_BYPASS_REG_field_set", mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_FIFO_IPT_BYPASS_REG,
                        mask,
                        PMC_SCPB_REG_REG_FIFO_IPT_BYPASS_REG_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_FIFO_IPT_BYPASS_REG_read
#define scpb_reg_reg_FIFO_IPT_BYPASS_REG_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_FIFO_IPT_BYPASS_REG_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_FIFO_IPT_BYPASS_REG_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SCPB_REG_REG_FIFO_IPT_BYPASS_REG);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_FIFO_IPT_BYPASS_REG_read", reg_value);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_FIFO_CFG
 *
 * Register: FIFO End Address Configuration
 * Offset: 0x00103d00 FIFO_CFG
 *
 * bits 28   default 0                     FIFO_END_ADDR_CHG_ABORT
 *
 * bits 24   default 0                     FIFO_END_ADDR_CHG_REQ
 *
 * bits 16:0 default 0 0000 0000 0000 0000 FIFO_END_ADDR
 */

#ifndef scpb_reg_reg_FIFO_CFG_write
#define scpb_reg_reg_FIFO_CFG_write( b_ptr, h_ptr, value ) \
       _scpb_reg_reg_FIFO_CFG_write( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_reg_FIFO_CFG_write( LOG_CALLER_DEFN
                                                 scpb_reg_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_FIFO_CFG_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_FIFO_CFG,
                        value);
}


#ifndef scpb_reg_reg_FIFO_CFG_field_set
#define scpb_reg_reg_FIFO_CFG_field_set( b_ptr, h_ptr, mask, ofs, value ) \
       _scpb_reg_reg_FIFO_CFG_field_set( LOG_CALLER b_ptr, h_ptr, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_FIFO_CFG_field_set( LOG_CALLER_DEFN
                                                     scpb_reg_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_FIFO_CFG_field_set", mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_FIFO_CFG,
                        mask,
                        PMC_SCPB_REG_REG_FIFO_CFG_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_FIFO_CFG_read
#define scpb_reg_reg_FIFO_CFG_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_FIFO_CFG_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_FIFO_CFG_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SCPB_REG_REG_FIFO_CFG);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_FIFO_CFG_read", reg_value);
    return reg_value;
}

#ifndef scpb_reg_reg_FIFO_CFG_poll
#define scpb_reg_reg_FIFO_CFG_poll( b_ptr, h_ptr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_reg_FIFO_CFG_poll( LOG_CALLER b_ptr, h_ptr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_reg_FIFO_CFG_poll( LOG_CALLER_DEFN
                                                                scpb_reg_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "scpb_reg_reg_FIFO_CFG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return scpb_reg_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_SCPB_REG_REG_FIFO_CFG,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


/**
 * accessor functions for register PMC_SCPB_REG_REG_FIFO_CFG_RID
 *
 * Register: FIFO End Address RID
 * Offset: 0x00103d04 FIFO_CFG_RID
 *
 * bits 7:0 default 0000 0000 FIFO_END_ADDR_CHG_RID
 */

#ifndef scpb_reg_reg_FIFO_CFG_RID_write
#define scpb_reg_reg_FIFO_CFG_RID_write( b_ptr, h_ptr, value ) \
       _scpb_reg_reg_FIFO_CFG_RID_write( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_reg_FIFO_CFG_RID_write( LOG_CALLER_DEFN
                                                     scpb_reg_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_FIFO_CFG_RID_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_FIFO_CFG_RID,
                        value);
}


#ifndef scpb_reg_reg_FIFO_CFG_RID_field_set
#define scpb_reg_reg_FIFO_CFG_RID_field_set( b_ptr, h_ptr, mask, ofs, value ) \
       _scpb_reg_reg_FIFO_CFG_RID_field_set( LOG_CALLER b_ptr, h_ptr, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_FIFO_CFG_RID_field_set( LOG_CALLER_DEFN
                                                         scpb_reg_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_FIFO_CFG_RID_field_set", mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_FIFO_CFG_RID,
                        mask,
                        PMC_SCPB_REG_REG_FIFO_CFG_RID_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_FIFO_CFG_RID_read
#define scpb_reg_reg_FIFO_CFG_RID_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_FIFO_CFG_RID_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_FIFO_CFG_RID_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SCPB_REG_REG_FIFO_CFG_RID);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_FIFO_CFG_RID_read", reg_value);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_FIFO_CFG_WID
 *
 * Register: FIFO End Address WID
 * Offset: 0x00103d08 FIFO_CFG_WID
 *
 * bits 9:0 default 00 0000 0000 FIFO_END_ADDR_CHG_WID
 */

#ifndef scpb_reg_reg_FIFO_CFG_WID_write
#define scpb_reg_reg_FIFO_CFG_WID_write( b_ptr, h_ptr, value ) \
       _scpb_reg_reg_FIFO_CFG_WID_write( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_reg_FIFO_CFG_WID_write( LOG_CALLER_DEFN
                                                     scpb_reg_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_FIFO_CFG_WID_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_FIFO_CFG_WID,
                        value);
}


#ifndef scpb_reg_reg_FIFO_CFG_WID_field_set
#define scpb_reg_reg_FIFO_CFG_WID_field_set( b_ptr, h_ptr, mask, ofs, value ) \
       _scpb_reg_reg_FIFO_CFG_WID_field_set( LOG_CALLER b_ptr, h_ptr, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_FIFO_CFG_WID_field_set( LOG_CALLER_DEFN
                                                         scpb_reg_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_FIFO_CFG_WID_field_set", mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_FIFO_CFG_WID,
                        mask,
                        PMC_SCPB_REG_REG_FIFO_CFG_WID_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_FIFO_CFG_WID_read
#define scpb_reg_reg_FIFO_CFG_WID_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_FIFO_CFG_WID_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_FIFO_CFG_WID_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SCPB_REG_REG_FIFO_CFG_WID);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_FIFO_CFG_WID_read", reg_value);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_DBG_CTRL
 *
 * Register: Debug Interface Control
 * Offset: 0x00103e00 DBG_CTRL
 *
 * bits 16   default 0      DBG_TRIG
 *
 * bits 10:0 default 0x0000 DBG_RADDR
 */

#ifndef scpb_reg_reg_DBG_CTRL_write
#define scpb_reg_reg_DBG_CTRL_write( b_ptr, h_ptr, value ) \
       _scpb_reg_reg_DBG_CTRL_write( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_reg_DBG_CTRL_write( LOG_CALLER_DEFN
                                                 scpb_reg_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_DBG_CTRL_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_DBG_CTRL,
                        value);
}


#ifndef scpb_reg_reg_DBG_CTRL_field_set
#define scpb_reg_reg_DBG_CTRL_field_set( b_ptr, h_ptr, mask, ofs, value ) \
       _scpb_reg_reg_DBG_CTRL_field_set( LOG_CALLER b_ptr, h_ptr, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_DBG_CTRL_field_set( LOG_CALLER_DEFN
                                                     scpb_reg_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_DBG_CTRL_field_set", mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_DBG_CTRL,
                        mask,
                        PMC_SCPB_REG_REG_DBG_CTRL_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_DBG_CTRL_read
#define scpb_reg_reg_DBG_CTRL_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_DBG_CTRL_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_DBG_CTRL_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SCPB_REG_REG_DBG_CTRL);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_DBG_CTRL_read", reg_value);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_GPO
 *
 * Register: General Purpose Output
 * Offset: (0x00104140 + (N) * 0x4) GPO
 *
 * bits 31:0 default 0x00000000 GPO
 */

#ifndef scpb_reg_reg_GPO_array_burst_write
#define scpb_reg_reg_GPO_array_burst_write( b_ptr, h_ptr, ofs, len, value ) \
       _scpb_reg_reg_GPO_array_burst_write( LOG_CALLER b_ptr, h_ptr, ofs, len, value )
#endif
static INLINE void _scpb_reg_reg_GPO_array_burst_write( LOG_CALLER_DEFN
                                                        scpb_reg_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 ofs,
                                                        UINT32 len,
                                                        UINT32 *value )
{
    if (len + ofs > 4)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "scpb_reg_reg_GPO_array_burst_write", ofs, len, 4 );
    IOLOG( "%s of %d words ofs=%d len=%d", "scpb_reg_reg_GPO_array_burst_write", 4, ofs, len);
    scpb_reg_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_SCPB_REG_REG_GPO(ofs),
                          len,
                          value);

}


#ifndef scpb_reg_reg_GPO_array_field_set
#define scpb_reg_reg_GPO_array_field_set( b_ptr, h_ptr, N, mask, ofs, value ) \
       _scpb_reg_reg_GPO_array_field_set( LOG_CALLER b_ptr, h_ptr, N, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_GPO_array_field_set( LOG_CALLER_DEFN
                                                      scpb_reg_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 N,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_GPO_array_field_set", mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_GPO(N),
                        mask,
                        PMC_SCPB_REG_REG_GPO_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_GPO_array_burst_read
#define scpb_reg_reg_GPO_array_burst_read( b_ptr, h_ptr, ofs, len, value ) \
       _scpb_reg_reg_GPO_array_burst_read( LOG_CALLER b_ptr, h_ptr, ofs, len, value )
#endif
static INLINE void _scpb_reg_reg_GPO_array_burst_read( LOG_CALLER_DEFN
                                                       scpb_reg_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 ofs,
                                                       UINT32 len,
                                                       UINT32 *value )
{
    if (len + ofs > 4)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "scpb_reg_reg_GPO_array_burst_read", ofs, len, 4 );
    scpb_reg_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_CONFIG,
                         PMC_SCPB_REG_REG_GPO(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "scpb_reg_reg_GPO_array_burst_read", 4, ofs, len);
}


#ifndef scpb_reg_reg_GPO_array_read
#define scpb_reg_reg_GPO_array_read( b_ptr, h_ptr, N ) \
       _scpb_reg_reg_GPO_array_read( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_reg_GPO_array_read( LOG_CALLER_DEFN
                                                   scpb_reg_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 N )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SCPB_REG_REG_GPO(N));

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_GPO_array_read", reg_value);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_BLOCK_TABLE_REG
 *
 * Register: Block Table Configuration
 * Offset: (0x00100000 + (N) * 0x100 + (M) * 0x4) BLOCK_TABLE_REG
 *
 * bits 31    default 0           IPT_BLOCK_VALID
 * attribute: cumulative=exposed_enables
 *
 * bits 30    default 0           IPT_BLOCK_ACTIVE
 *
 * bits 23:16 default 0x00        IPT_WQID
 *
 * bits 0:10  default 0 0000 0000 IPT_LBA
 *
 * bits 0     default 0x1         WPTR_RST_PUSH_P0
 */

#ifndef scpb_reg_reg_BLOCK_TABLE_REG_array_write
#define scpb_reg_reg_BLOCK_TABLE_REG_array_write( b_ptr, h_ptr, N, M, value ) \
       _scpb_reg_reg_BLOCK_TABLE_REG_array_write( LOG_CALLER b_ptr, h_ptr, N, M, value )
#endif
static INLINE void _scpb_reg_reg_BLOCK_TABLE_REG_array_write( LOG_CALLER_DEFN
                                                              scpb_reg_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32  M,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_BLOCK_TABLE_REG_array_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_BLOCK_TABLE_REG(N,M),
                        value);
}


#ifndef scpb_reg_reg_BLOCK_TABLE_REG_array_field_set
#define scpb_reg_reg_BLOCK_TABLE_REG_array_field_set( b_ptr, h_ptr, N, M, mask, ofs, value ) \
       _scpb_reg_reg_BLOCK_TABLE_REG_array_field_set( LOG_CALLER b_ptr, h_ptr, N, M, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_BLOCK_TABLE_REG_array_field_set( LOG_CALLER_DEFN
                                                                  scpb_reg_buffer_t *b_ptr,
                                                                  cpb_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32  M,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d M=%d mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_BLOCK_TABLE_REG_array_field_set", N, M, mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_BLOCK_TABLE_REG(N,M),
                        mask,
                        PMC_SCPB_REG_REG_BLOCK_TABLE_REG_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_BLOCK_TABLE_REG_array_read
#define scpb_reg_reg_BLOCK_TABLE_REG_array_read( b_ptr, h_ptr, N, M ) \
       _scpb_reg_reg_BLOCK_TABLE_REG_array_read( LOG_CALLER b_ptr, h_ptr, N, M )
#endif
static INLINE UINT32 _scpb_reg_reg_BLOCK_TABLE_REG_array_read( LOG_CALLER_DEFN
                                                               scpb_reg_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32  M )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SCPB_REG_REG_BLOCK_TABLE_REG(N,M));

    IOLOG( "%s -> 0x%08x; N=%d M=%d", "scpb_reg_reg_BLOCK_TABLE_REG_array_read", reg_value, N, M);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_PAGE_TABLE_REG
 *
 * Register: Page Table Configuration
 * Offset: (0x00104000 + (N) * 0x4) PAGE_TABLE_REG
 *
 * bits 24    default 0   PAGE_VALID
 *
 * bits 17:16 default 00  IPT_QS_ID
 *
 * bits 9:8   default 00  IPT_WP_ID
 */

#ifndef scpb_reg_reg_PAGE_TABLE_REG_array_write
#define scpb_reg_reg_PAGE_TABLE_REG_array_write( b_ptr, h_ptr, N, value ) \
       _scpb_reg_reg_PAGE_TABLE_REG_array_write( LOG_CALLER b_ptr, h_ptr, N, value )
#endif
static INLINE void _scpb_reg_reg_PAGE_TABLE_REG_array_write( LOG_CALLER_DEFN
                                                             scpb_reg_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_PAGE_TABLE_REG_array_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_PAGE_TABLE_REG(N),
                        value);
}


#ifndef scpb_reg_reg_PAGE_TABLE_REG_array_field_set
#define scpb_reg_reg_PAGE_TABLE_REG_array_field_set( b_ptr, h_ptr, N, mask, ofs, value ) \
       _scpb_reg_reg_PAGE_TABLE_REG_array_field_set( LOG_CALLER b_ptr, h_ptr, N, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_PAGE_TABLE_REG_array_field_set( LOG_CALLER_DEFN
                                                                 scpb_reg_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_PAGE_TABLE_REG_array_field_set", N, mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SCPB_REG_REG_PAGE_TABLE_REG(N),
                        mask,
                        PMC_SCPB_REG_REG_PAGE_TABLE_REG_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_PAGE_TABLE_REG_array_read
#define scpb_reg_reg_PAGE_TABLE_REG_array_read( b_ptr, h_ptr, N ) \
       _scpb_reg_reg_PAGE_TABLE_REG_array_read( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_reg_PAGE_TABLE_REG_array_read( LOG_CALLER_DEFN
                                                              scpb_reg_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SCPB_REG_REG_PAGE_TABLE_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "scpb_reg_reg_PAGE_TABLE_REG_array_read", reg_value, N);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_REMAP_REQ
 *
 * Register: Remap Block Request
 * Offset: 0x00104200 REMAP_REQ
 *
 * bits 28 default 0  REMAP_ABORT
 *
 * bits 24 default 0  REMAP_REQ
 */

#ifndef scpb_reg_reg_REMAP_REQ_write
#define scpb_reg_reg_REMAP_REQ_write( b_ptr, h_ptr, value ) \
       _scpb_reg_reg_REMAP_REQ_write( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_reg_REMAP_REQ_write( LOG_CALLER_DEFN
                                                  scpb_reg_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_REMAP_REQ_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_REMAP_REQ,
                        value);
}


#ifndef scpb_reg_reg_REMAP_REQ_field_set
#define scpb_reg_reg_REMAP_REQ_field_set( b_ptr, h_ptr, mask, ofs, value ) \
       _scpb_reg_reg_REMAP_REQ_field_set( LOG_CALLER b_ptr, h_ptr, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_REMAP_REQ_field_set( LOG_CALLER_DEFN
                                                      scpb_reg_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_REMAP_REQ_field_set", mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_REMAP_REQ,
                        mask,
                        PMC_SCPB_REG_REG_REMAP_REQ_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_REMAP_REQ_read
#define scpb_reg_reg_REMAP_REQ_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_REMAP_REQ_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_REMAP_REQ_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_REMAP_REQ);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_REMAP_REQ_read", reg_value);
    return reg_value;
}

#ifndef scpb_reg_reg_REMAP_REQ_poll
#define scpb_reg_reg_REMAP_REQ_poll( b_ptr, h_ptr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_reg_REMAP_REQ_poll( LOG_CALLER b_ptr, h_ptr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_reg_REMAP_REQ_poll( LOG_CALLER_DEFN
                                                                 scpb_reg_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "scpb_reg_reg_REMAP_REQ_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return scpb_reg_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_SCPB_REG_REG_REMAP_REQ,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


/**
 * accessor functions for register PMC_SCPB_REG_REG_PUSH_PORT_P3_RST
 *
 * Register: Push Port 3 Write Pointer Reset (Write Only)
 * Offset: (0x00101800 + (N) * 0x4) PUSH_PORT_P3_RST
 *
 * bits 0 default 0x1 WPTR_RST_PUSH_P3
 */

#ifndef scpb_reg_reg_PUSH_PORT_P3_RST_array_write
#define scpb_reg_reg_PUSH_PORT_P3_RST_array_write( b_ptr, h_ptr, N, value ) \
       _scpb_reg_reg_PUSH_PORT_P3_RST_array_write( LOG_CALLER b_ptr, h_ptr, N, value )
#endif
static INLINE void _scpb_reg_reg_PUSH_PORT_P3_RST_array_write( LOG_CALLER_DEFN
                                                               scpb_reg_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_PUSH_PORT_P3_RST_array_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_PUSH_PORT_P3_RST(N),
                        value);
}


#ifndef scpb_reg_reg_PUSH_PORT_P3_RST_array_field_set
#define scpb_reg_reg_PUSH_PORT_P3_RST_array_field_set( b_ptr, h_ptr, N, mask, ofs, value ) \
       _scpb_reg_reg_PUSH_PORT_P3_RST_array_field_set( LOG_CALLER b_ptr, h_ptr, N, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_PUSH_PORT_P3_RST_array_field_set( LOG_CALLER_DEFN
                                                                   scpb_reg_buffer_t *b_ptr,
                                                                   cpb_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_PUSH_PORT_P3_RST_array_field_set", N, mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_PUSH_PORT_P3_RST(N),
                        mask,
                        PMC_SCPB_REG_REG_PUSH_PORT_P3_RST_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_PUSH_PORT_P3_RST_array_read
#define scpb_reg_reg_PUSH_PORT_P3_RST_array_read( b_ptr, h_ptr, N ) \
       _scpb_reg_reg_PUSH_PORT_P3_RST_array_read( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_reg_PUSH_PORT_P3_RST_array_read( LOG_CALLER_DEFN
                                                                scpb_reg_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_PUSH_PORT_P3_RST(N));

    IOLOG( "%s -> 0x%08x; N=%d", "scpb_reg_reg_PUSH_PORT_P3_RST_array_read", reg_value, N);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_PUSH_PORT_P2_RST
 *
 * Register: Push Port 2 Write Pointer Reset (Write Only)
 * Offset: (0x00101000 + (N) * 0x4) PUSH_PORT_P2_RST
 *
 * bits 0 default 0x1 WPTR_RST_PUSH_P2
 */

#ifndef scpb_reg_reg_PUSH_PORT_P2_RST_array_write
#define scpb_reg_reg_PUSH_PORT_P2_RST_array_write( b_ptr, h_ptr, N, value ) \
       _scpb_reg_reg_PUSH_PORT_P2_RST_array_write( LOG_CALLER b_ptr, h_ptr, N, value )
#endif
static INLINE void _scpb_reg_reg_PUSH_PORT_P2_RST_array_write( LOG_CALLER_DEFN
                                                               scpb_reg_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_PUSH_PORT_P2_RST_array_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_PUSH_PORT_P2_RST(N),
                        value);
}


#ifndef scpb_reg_reg_PUSH_PORT_P2_RST_array_field_set
#define scpb_reg_reg_PUSH_PORT_P2_RST_array_field_set( b_ptr, h_ptr, N, mask, ofs, value ) \
       _scpb_reg_reg_PUSH_PORT_P2_RST_array_field_set( LOG_CALLER b_ptr, h_ptr, N, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_PUSH_PORT_P2_RST_array_field_set( LOG_CALLER_DEFN
                                                                   scpb_reg_buffer_t *b_ptr,
                                                                   cpb_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_PUSH_PORT_P2_RST_array_field_set", N, mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_PUSH_PORT_P2_RST(N),
                        mask,
                        PMC_SCPB_REG_REG_PUSH_PORT_P2_RST_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_PUSH_PORT_P2_RST_array_read
#define scpb_reg_reg_PUSH_PORT_P2_RST_array_read( b_ptr, h_ptr, N ) \
       _scpb_reg_reg_PUSH_PORT_P2_RST_array_read( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_reg_PUSH_PORT_P2_RST_array_read( LOG_CALLER_DEFN
                                                                scpb_reg_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_PUSH_PORT_P2_RST(N));

    IOLOG( "%s -> 0x%08x; N=%d", "scpb_reg_reg_PUSH_PORT_P2_RST_array_read", reg_value, N);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_PUSH_PORT_P1_RST
 *
 * Register: Push Port 1 Write Pointer Reset (Write Only)
 * Offset: (0x00100800 + (N) * 0x4) PUSH_PORT_P1_RST
 *
 * bits 0 default 0x1 WPTR_RST_PUSH_P1
 */

#ifndef scpb_reg_reg_PUSH_PORT_P1_RST_array_write
#define scpb_reg_reg_PUSH_PORT_P1_RST_array_write( b_ptr, h_ptr, N, value ) \
       _scpb_reg_reg_PUSH_PORT_P1_RST_array_write( LOG_CALLER b_ptr, h_ptr, N, value )
#endif
static INLINE void _scpb_reg_reg_PUSH_PORT_P1_RST_array_write( LOG_CALLER_DEFN
                                                               scpb_reg_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_PUSH_PORT_P1_RST_array_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_PUSH_PORT_P1_RST(N),
                        value);
}


#ifndef scpb_reg_reg_PUSH_PORT_P1_RST_array_field_set
#define scpb_reg_reg_PUSH_PORT_P1_RST_array_field_set( b_ptr, h_ptr, N, mask, ofs, value ) \
       _scpb_reg_reg_PUSH_PORT_P1_RST_array_field_set( LOG_CALLER b_ptr, h_ptr, N, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_PUSH_PORT_P1_RST_array_field_set( LOG_CALLER_DEFN
                                                                   scpb_reg_buffer_t *b_ptr,
                                                                   cpb_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_PUSH_PORT_P1_RST_array_field_set", N, mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_PUSH_PORT_P1_RST(N),
                        mask,
                        PMC_SCPB_REG_REG_PUSH_PORT_P1_RST_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_PUSH_PORT_P1_RST_array_read
#define scpb_reg_reg_PUSH_PORT_P1_RST_array_read( b_ptr, h_ptr, N ) \
       _scpb_reg_reg_PUSH_PORT_P1_RST_array_read( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_reg_PUSH_PORT_P1_RST_array_read( LOG_CALLER_DEFN
                                                                scpb_reg_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_PUSH_PORT_P1_RST(N));

    IOLOG( "%s -> 0x%08x; N=%d", "scpb_reg_reg_PUSH_PORT_P1_RST_array_read", reg_value, N);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_POP_PORT_RST
 *
 * Register: Pop Port Read Pointer Reset (Write Only)
 * Offset: (0x00102000 + (N) * 0x4) POP_PORT_RST
 *
 * bits 0 default 0x1 RPTR_RST_POP
 */

#ifndef scpb_reg_reg_POP_PORT_RST_array_write
#define scpb_reg_reg_POP_PORT_RST_array_write( b_ptr, h_ptr, N, value ) \
       _scpb_reg_reg_POP_PORT_RST_array_write( LOG_CALLER b_ptr, h_ptr, N, value )
#endif
static INLINE void _scpb_reg_reg_POP_PORT_RST_array_write( LOG_CALLER_DEFN
                                                           scpb_reg_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_POP_PORT_RST_array_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_POP_PORT_RST(N),
                        value);
}


#ifndef scpb_reg_reg_POP_PORT_RST_array_field_set
#define scpb_reg_reg_POP_PORT_RST_array_field_set( b_ptr, h_ptr, N, mask, ofs, value ) \
       _scpb_reg_reg_POP_PORT_RST_array_field_set( LOG_CALLER b_ptr, h_ptr, N, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_POP_PORT_RST_array_field_set( LOG_CALLER_DEFN
                                                               scpb_reg_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_POP_PORT_RST_array_field_set", N, mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_POP_PORT_RST(N),
                        mask,
                        PMC_SCPB_REG_REG_POP_PORT_RST_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_POP_PORT_RST_array_read
#define scpb_reg_reg_POP_PORT_RST_array_read( b_ptr, h_ptr, N ) \
       _scpb_reg_reg_POP_PORT_RST_array_read( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_reg_POP_PORT_RST_array_read( LOG_CALLER_DEFN
                                                            scpb_reg_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_POP_PORT_RST(N));

    IOLOG( "%s -> 0x%08x; N=%d", "scpb_reg_reg_POP_PORT_RST_array_read", reg_value, N);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_PAGE_IPT_ERR_INT_REG
 *
 * Register: Page IPT Error Interrupt
 * Offset: 0x00104100 PAGE_IPT_ERR_INT_REG
 *
 * bits 0 default 0  PAGE_IPT_ERR_I
 */

#ifndef scpb_reg_reg_PAGE_IPT_ERR_INT_REG_write
#define scpb_reg_reg_PAGE_IPT_ERR_INT_REG_write( b_ptr, h_ptr, value ) \
       _scpb_reg_reg_PAGE_IPT_ERR_INT_REG_write( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_reg_PAGE_IPT_ERR_INT_REG_write( LOG_CALLER_DEFN
                                                             scpb_reg_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_PAGE_IPT_ERR_INT_REG_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_PAGE_IPT_ERR_INT_REG,
                        value);
}


#ifndef scpb_reg_reg_PAGE_IPT_ERR_INT_REG_action_on_write_field_set
#define scpb_reg_reg_PAGE_IPT_ERR_INT_REG_action_on_write_field_set( b_ptr, h_ptr, mask, ofs, value ) \
       _scpb_reg_reg_PAGE_IPT_ERR_INT_REG_action_on_write_field_set( LOG_CALLER b_ptr, h_ptr, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_PAGE_IPT_ERR_INT_REG_action_on_write_field_set( LOG_CALLER_DEFN
                                                                                 scpb_reg_buffer_t *b_ptr,
                                                                                 cpb_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_PAGE_IPT_ERR_INT_REG_action_on_write_field_set", mask, ofs, value );
    scpb_reg_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_SCPB_REG_REG_PAGE_IPT_ERR_INT_REG,
                                        mask,
                                        ofs,
                                        value);

}


#ifndef scpb_reg_reg_PAGE_IPT_ERR_INT_REG_read
#define scpb_reg_reg_PAGE_IPT_ERR_INT_REG_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_PAGE_IPT_ERR_INT_REG_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_PAGE_IPT_ERR_INT_REG_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_PAGE_IPT_ERR_INT_REG);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_PAGE_IPT_ERR_INT_REG_read", reg_value);
    return reg_value;
}

#ifndef scpb_reg_reg_PAGE_IPT_ERR_INT_REG_poll
#define scpb_reg_reg_PAGE_IPT_ERR_INT_REG_poll( b_ptr, h_ptr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_reg_PAGE_IPT_ERR_INT_REG_poll( LOG_CALLER b_ptr, h_ptr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_reg_PAGE_IPT_ERR_INT_REG_poll( LOG_CALLER_DEFN
                                                                            scpb_reg_buffer_t *b_ptr,
                                                                            cpb_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "scpb_reg_reg_PAGE_IPT_ERR_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return scpb_reg_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_SCPB_REG_REG_PAGE_IPT_ERR_INT_REG,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


/**
 * accessor functions for register PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG
 *
 * Register: Block IPT Error Interrupt
 * Offset: 0x00104104 BLOCK_IPT_ERR_INT_REG
 *
 * bits 31:0 default 0x00000000 BLOCK_IPT_ERR_I
 */

#ifndef scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_write
#define scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_write( b_ptr, h_ptr, value ) \
       _scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_write( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_write( LOG_CALLER_DEFN
                                                              scpb_reg_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG,
                        value);
}


#ifndef scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_action_on_write_field_set
#define scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_action_on_write_field_set( b_ptr, h_ptr, mask, ofs, value ) \
       _scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_action_on_write_field_set( LOG_CALLER b_ptr, h_ptr, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_action_on_write_field_set( LOG_CALLER_DEFN
                                                                                  scpb_reg_buffer_t *b_ptr,
                                                                                  cpb_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_action_on_write_field_set", mask, ofs, value );
    scpb_reg_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG,
                                        mask,
                                        ofs,
                                        value);

}


#ifndef scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_read
#define scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_read", reg_value);
    return reg_value;
}

#ifndef scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_poll
#define scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_poll( b_ptr, h_ptr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_poll( LOG_CALLER b_ptr, h_ptr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_poll( LOG_CALLER_DEFN
                                                                             scpb_reg_buffer_t *b_ptr,
                                                                             cpb_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return scpb_reg_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


/**
 * accessor functions for register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG
 *
 * Register: Prefetch FIFO Interrupt
 * Offset: 0x00104108 PREFETCH_FIFO_INT_REG
 *
 * bits        19:16 default 0x0 HOQ_DLY_OVERFLOW_I
 * field_bits  3:0
 *
 * bits        11:8  default 0x0 HOQ_CFC_UNDERFLOW_I
 * field_bits  3:0
 *
 * bits        3:0   default 0x0 HOQ_CFC_OVERFLOW_I
 * field_bits  3:0
 */

#ifndef scpb_reg_reg_PREFETCH_FIFO_INT_REG_write
#define scpb_reg_reg_PREFETCH_FIFO_INT_REG_write( b_ptr, h_ptr, value ) \
       _scpb_reg_reg_PREFETCH_FIFO_INT_REG_write( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_reg_PREFETCH_FIFO_INT_REG_write( LOG_CALLER_DEFN
                                                              scpb_reg_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_PREFETCH_FIFO_INT_REG_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG,
                        value);
}


#ifndef scpb_reg_reg_PREFETCH_FIFO_INT_REG_action_on_write_field_set
#define scpb_reg_reg_PREFETCH_FIFO_INT_REG_action_on_write_field_set( b_ptr, h_ptr, mask, ofs, value ) \
       _scpb_reg_reg_PREFETCH_FIFO_INT_REG_action_on_write_field_set( LOG_CALLER b_ptr, h_ptr, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_PREFETCH_FIFO_INT_REG_action_on_write_field_set( LOG_CALLER_DEFN
                                                                                  scpb_reg_buffer_t *b_ptr,
                                                                                  cpb_handle_t *h_ptr,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_PREFETCH_FIFO_INT_REG_action_on_write_field_set", mask, ofs, value );
    scpb_reg_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG,
                                        mask,
                                        ofs,
                                        value);

}


#ifndef scpb_reg_reg_PREFETCH_FIFO_INT_REG_read
#define scpb_reg_reg_PREFETCH_FIFO_INT_REG_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_PREFETCH_FIFO_INT_REG_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_PREFETCH_FIFO_INT_REG_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_PREFETCH_FIFO_INT_REG_read", reg_value);
    return reg_value;
}

#ifndef scpb_reg_reg_PREFETCH_FIFO_INT_REG_poll
#define scpb_reg_reg_PREFETCH_FIFO_INT_REG_poll( b_ptr, h_ptr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_reg_PREFETCH_FIFO_INT_REG_poll( LOG_CALLER b_ptr, h_ptr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_reg_PREFETCH_FIFO_INT_REG_poll( LOG_CALLER_DEFN
                                                                             scpb_reg_buffer_t *b_ptr,
                                                                             cpb_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "scpb_reg_reg_PREFETCH_FIFO_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return scpb_reg_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


/**
 * accessor functions for register PMC_SCPB_REG_REG_FIFO_INT_REG
 *
 * Register: FIFO Overflow and Underflow Interrupts
 * Offset: 0x00102800 FIFO_INT_REG
 *
 * bits 8   default 0    FIFO_UDR_I
 *
 * bits 3:0 default 0000 FIFO_OVR_PUSH_I
 */

#ifndef scpb_reg_reg_FIFO_INT_REG_write
#define scpb_reg_reg_FIFO_INT_REG_write( b_ptr, h_ptr, value ) \
       _scpb_reg_reg_FIFO_INT_REG_write( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_reg_FIFO_INT_REG_write( LOG_CALLER_DEFN
                                                     scpb_reg_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_FIFO_INT_REG_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_FIFO_INT_REG,
                        value);
}


#ifndef scpb_reg_reg_FIFO_INT_REG_action_on_write_field_set
#define scpb_reg_reg_FIFO_INT_REG_action_on_write_field_set( b_ptr, h_ptr, mask, ofs, value ) \
       _scpb_reg_reg_FIFO_INT_REG_action_on_write_field_set( LOG_CALLER b_ptr, h_ptr, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_FIFO_INT_REG_action_on_write_field_set( LOG_CALLER_DEFN
                                                                         scpb_reg_buffer_t *b_ptr,
                                                                         cpb_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_FIFO_INT_REG_action_on_write_field_set", mask, ofs, value );
    scpb_reg_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_SCPB_REG_REG_FIFO_INT_REG,
                                        mask,
                                        ofs,
                                        value);

}


#ifndef scpb_reg_reg_FIFO_INT_REG_read
#define scpb_reg_reg_FIFO_INT_REG_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_FIFO_INT_REG_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_FIFO_INT_REG_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_FIFO_INT_REG);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_FIFO_INT_REG_read", reg_value);
    return reg_value;
}

#ifndef scpb_reg_reg_FIFO_INT_REG_poll
#define scpb_reg_reg_FIFO_INT_REG_poll( b_ptr, h_ptr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_reg_FIFO_INT_REG_poll( LOG_CALLER b_ptr, h_ptr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_reg_FIFO_INT_REG_poll( LOG_CALLER_DEFN
                                                                    scpb_reg_buffer_t *b_ptr,
                                                                    cpb_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "scpb_reg_reg_FIFO_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return scpb_reg_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_SCPB_REG_REG_FIFO_INT_REG,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


/**
 * accessor functions for register PMC_SCPB_REG_REG_PAGE_IPT_ERR_INT_EN_REG
 *
 * Register: Page IPT Error Interrupt Enable
 * Offset: 0x00104110 PAGE_IPT_ERR_INT_EN_REG
 *
 * bits 0 default 0  PAGE_IPT_ERR_E
 */

#ifndef scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_write
#define scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_write( b_ptr, h_ptr, value ) \
       _scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_write( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_write( LOG_CALLER_DEFN
                                                                scpb_reg_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_PAGE_IPT_ERR_INT_EN_REG,
                        value);
}


#ifndef scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_field_set
#define scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_field_set( b_ptr, h_ptr, mask, ofs, value ) \
       _scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_field_set( LOG_CALLER b_ptr, h_ptr, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_field_set( LOG_CALLER_DEFN
                                                                    scpb_reg_buffer_t *b_ptr,
                                                                    cpb_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_field_set", mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_PAGE_IPT_ERR_INT_EN_REG,
                        mask,
                        PMC_SCPB_REG_REG_PAGE_IPT_ERR_INT_EN_REG_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_read
#define scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_PAGE_IPT_ERR_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_read", reg_value);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG
 *
 * Register: Block IPT Error Interrupt Enable
 * Offset: 0x00104114 BLOCK_IPT_ERR_INT_EN_REG
 *
 * bits 31:0 default 0x00000000 BLOCK_IPT_ERR_E
 */

#ifndef scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_write
#define scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_write( b_ptr, h_ptr, value ) \
       _scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_write( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_write( LOG_CALLER_DEFN
                                                                 scpb_reg_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG,
                        value);
}


#ifndef scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_field_set
#define scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_field_set( b_ptr, h_ptr, mask, ofs, value ) \
       _scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_field_set( LOG_CALLER b_ptr, h_ptr, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_field_set( LOG_CALLER_DEFN
                                                                     scpb_reg_buffer_t *b_ptr,
                                                                     cpb_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_field_set", mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG,
                        mask,
                        PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_read
#define scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_read", reg_value);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG
 *
 * Register: Prefetch FIFO Interrupt Enable
 * Offset: 0x00104118 PREFETCH_FIFO_INT_EN_REG
 *
 * bits        19:16 default 0x0 HOQ_DLY_OVERFLOW_E
 * field_bits  3:0
 *
 * bits        11:8  default 0x0 HOQ_CFC_UNDERFLOW_E
 * field_bits  3:0
 *
 * bits        3:0   default 0x0 HOQ_CFC_OVERFLOW_E
 * field_bits  3:0
 */

#ifndef scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_write
#define scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_write( b_ptr, h_ptr, value ) \
       _scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_write( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_write( LOG_CALLER_DEFN
                                                                 scpb_reg_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG,
                        value);
}


#ifndef scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_field_set
#define scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_field_set( b_ptr, h_ptr, mask, ofs, value ) \
       _scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_field_set( LOG_CALLER b_ptr, h_ptr, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_field_set( LOG_CALLER_DEFN
                                                                     scpb_reg_buffer_t *b_ptr,
                                                                     cpb_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_field_set", mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG,
                        mask,
                        PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_read
#define scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_read", reg_value);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_FIFO_INT_EN_REG
 *
 * Register: FIFO Overflow and Underflow Interrupt Enables
 * Offset: 0x00102804 FIFO_INT_EN_REG
 *
 * bits 8   default 0    FIFO_UDR_E
 *
 * bits 3:0 default 0000 FIFO_OVR_PUSH_E
 */

#ifndef scpb_reg_reg_FIFO_INT_EN_REG_write
#define scpb_reg_reg_FIFO_INT_EN_REG_write( b_ptr, h_ptr, value ) \
       _scpb_reg_reg_FIFO_INT_EN_REG_write( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_reg_FIFO_INT_EN_REG_write( LOG_CALLER_DEFN
                                                        scpb_reg_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "scpb_reg_reg_FIFO_INT_EN_REG_write", value );
    scpb_reg_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_FIFO_INT_EN_REG,
                        value);
}


#ifndef scpb_reg_reg_FIFO_INT_EN_REG_field_set
#define scpb_reg_reg_FIFO_INT_EN_REG_field_set( b_ptr, h_ptr, mask, ofs, value ) \
       _scpb_reg_reg_FIFO_INT_EN_REG_field_set( LOG_CALLER b_ptr, h_ptr, mask, ofs, value )
#endif
static INLINE void _scpb_reg_reg_FIFO_INT_EN_REG_field_set( LOG_CALLER_DEFN
                                                            scpb_reg_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "scpb_reg_reg_FIFO_INT_EN_REG_field_set", mask, ofs, value );
    scpb_reg_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_SCPB_REG_REG_FIFO_INT_EN_REG,
                        mask,
                        PMC_SCPB_REG_REG_FIFO_INT_EN_REG_UNUSED_MASK,
                        ofs,
                        value);

}


#ifndef scpb_reg_reg_FIFO_INT_EN_REG_read
#define scpb_reg_reg_FIFO_INT_EN_REG_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_FIFO_INT_EN_REG_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_FIFO_INT_EN_REG_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_FIFO_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_FIFO_INT_EN_REG_read", reg_value);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_REMAP_STAT
 *
 * Register: Remap Block Request Status
 * Offset: 0x0010420c REMAP_STAT
 *
 * bits 0 default 0  REMAP_SUCCESS
 */

#ifndef scpb_reg_reg_REMAP_STAT_read
#define scpb_reg_reg_REMAP_STAT_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_REMAP_STAT_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_REMAP_STAT_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_REMAP_STAT);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_REMAP_STAT_read", reg_value);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_FIFO_CFG_STAT
 *
 * Register: FIFO End Address Change Status
 * Offset: 0x00103d0c FIFO_CFG_STAT
 *
 * bits 0 default 0  FIFO_END_ADDR_CHG_SUCCESS
 */

#ifndef scpb_reg_reg_FIFO_CFG_STAT_read
#define scpb_reg_reg_FIFO_CFG_STAT_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_FIFO_CFG_STAT_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_FIFO_CFG_STAT_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_FIFO_CFG_STAT);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_FIFO_CFG_STAT_read", reg_value);
    return reg_value;
}

#ifndef scpb_reg_reg_FIFO_CFG_STAT_poll
#define scpb_reg_reg_FIFO_CFG_STAT_poll( b_ptr, h_ptr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_reg_FIFO_CFG_STAT_poll( LOG_CALLER b_ptr, h_ptr, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_reg_FIFO_CFG_STAT_poll( LOG_CALLER_DEFN
                                                                     scpb_reg_buffer_t *b_ptr,
                                                                     cpb_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "scpb_reg_reg_FIFO_CFG_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return scpb_reg_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_SCPB_REG_REG_FIFO_CFG_STAT,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


/**
 * accessor functions for register PMC_SCPB_REG_REG_DBG_RDATA
 *
 * Register: Debug Interface Read Data
 * Offset: 0x00103e04 DBG_RDATA
 *
 * bits 31:0 default 0x0000 DBG_RDATA
 */

#ifndef scpb_reg_reg_DBG_RDATA_read
#define scpb_reg_reg_DBG_RDATA_read( b_ptr, h_ptr ) \
       _scpb_reg_reg_DBG_RDATA_read( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_reg_DBG_RDATA_read( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_DBG_RDATA);

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_DBG_RDATA_read", reg_value);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_GPI
 *
 * Register: General Purpose Input
 * Offset: (0x00104120 + (N) * 0x4) GPI
 *
 * bits 31:0 default 0x00000000 GPI
 */

#ifndef scpb_reg_reg_GPI_array_burst_read
#define scpb_reg_reg_GPI_array_burst_read( b_ptr, h_ptr, ofs, len, value ) \
       _scpb_reg_reg_GPI_array_burst_read( LOG_CALLER b_ptr, h_ptr, ofs, len, value )
#endif
static INLINE void _scpb_reg_reg_GPI_array_burst_read( LOG_CALLER_DEFN
                                                       scpb_reg_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 ofs,
                                                       UINT32 len,
                                                       UINT32 *value )
{
    if (len + ofs > 4)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "scpb_reg_reg_GPI_array_burst_read", ofs, len, 4 );
    scpb_reg_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_SCPB_REG_REG_GPI(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "scpb_reg_reg_GPI_array_burst_read", 4, ofs, len);
}


#ifndef scpb_reg_reg_GPI_array_read
#define scpb_reg_reg_GPI_array_read( b_ptr, h_ptr, N ) \
       _scpb_reg_reg_GPI_array_read( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_reg_GPI_array_read( LOG_CALLER_DEFN
                                                   scpb_reg_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 N )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_GPI(N));

    IOLOG( "%s -> 0x%08x;", "scpb_reg_reg_GPI_array_read", reg_value);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_PUSH_PORT_P0_RST_VAL
 *
 * Register: Push Port 0 Write Pointer Reset Value (Read Only)
 * Offset: (0x00100400 + (N) * 0x4) PUSH_PORT_P0_RST_VAL
 *
 * bits 31:0 default 0xFFFFFFFF WPTR_RST_PUSH_P0_VAL
 */

#ifndef scpb_reg_reg_PUSH_PORT_P0_RST_VAL_array_read
#define scpb_reg_reg_PUSH_PORT_P0_RST_VAL_array_read( b_ptr, h_ptr, N ) \
       _scpb_reg_reg_PUSH_PORT_P0_RST_VAL_array_read( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_reg_PUSH_PORT_P0_RST_VAL_array_read( LOG_CALLER_DEFN
                                                                    scpb_reg_buffer_t *b_ptr,
                                                                    cpb_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_PUSH_PORT_P0_RST_VAL(N));

    IOLOG( "%s -> 0x%08x; N=%d", "scpb_reg_reg_PUSH_PORT_P0_RST_VAL_array_read", reg_value, N);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_PUSH_PORT_P1_RST_VAL
 *
 * Register: Push Port 1 Write Pointer Reset Value (Read Only)
 * Offset: (0x00100c00 + (N) * 0x4) PUSH_PORT_P1_RST_VAL
 *
 * bits 31:0 default 0xFFFFFFFF WPTR_RST_PUSH_P1_VAL
 */

#ifndef scpb_reg_reg_PUSH_PORT_P1_RST_VAL_array_read
#define scpb_reg_reg_PUSH_PORT_P1_RST_VAL_array_read( b_ptr, h_ptr, N ) \
       _scpb_reg_reg_PUSH_PORT_P1_RST_VAL_array_read( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_reg_PUSH_PORT_P1_RST_VAL_array_read( LOG_CALLER_DEFN
                                                                    scpb_reg_buffer_t *b_ptr,
                                                                    cpb_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_PUSH_PORT_P1_RST_VAL(N));

    IOLOG( "%s -> 0x%08x; N=%d", "scpb_reg_reg_PUSH_PORT_P1_RST_VAL_array_read", reg_value, N);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_PUSH_PORT_P2_RST_VAL
 *
 * Register: Push Port 2 Write Pointer Reset Value (Read Only)
 * Offset: (0x00101400 + (N) * 0x4) PUSH_PORT_P2_RST_VAL
 *
 * bits 31:0 default 0xFFFFFFFF WPTR_RST_PUSH_P2_VAL
 */

#ifndef scpb_reg_reg_PUSH_PORT_P2_RST_VAL_array_read
#define scpb_reg_reg_PUSH_PORT_P2_RST_VAL_array_read( b_ptr, h_ptr, N ) \
       _scpb_reg_reg_PUSH_PORT_P2_RST_VAL_array_read( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_reg_PUSH_PORT_P2_RST_VAL_array_read( LOG_CALLER_DEFN
                                                                    scpb_reg_buffer_t *b_ptr,
                                                                    cpb_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_PUSH_PORT_P2_RST_VAL(N));

    IOLOG( "%s -> 0x%08x; N=%d", "scpb_reg_reg_PUSH_PORT_P2_RST_VAL_array_read", reg_value, N);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_PUSH_PORT_P3_RST_VAL
 *
 * Register: Push Port 3 Write Pointer Reset Value (Read Only)
 * Offset: (0x00101c00 + (N) * 0x4) PUSH_PORT_P3_RST_VAL
 *
 * bits 31:0 default 0xFFFFFFFF WPTR_RST_PUSH_P3_VAL
 */

#ifndef scpb_reg_reg_PUSH_PORT_P3_RST_VAL_array_read
#define scpb_reg_reg_PUSH_PORT_P3_RST_VAL_array_read( b_ptr, h_ptr, N ) \
       _scpb_reg_reg_PUSH_PORT_P3_RST_VAL_array_read( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_reg_PUSH_PORT_P3_RST_VAL_array_read( LOG_CALLER_DEFN
                                                                    scpb_reg_buffer_t *b_ptr,
                                                                    cpb_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_PUSH_PORT_P3_RST_VAL(N));

    IOLOG( "%s -> 0x%08x; N=%d", "scpb_reg_reg_PUSH_PORT_P3_RST_VAL_array_read", reg_value, N);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_RPTR_RST_POP_VAL_REG
 *
 * Register: Read Pointer Reset Value (Read Only)
 * Offset: (0x00102400 + (N) * 0x4) RPTR_RST_POP_VAL_REG
 *
 * bits 31:0 default 0xFFFFFFFF RPTR_RST_POP_VAL
 */

#ifndef scpb_reg_reg_RPTR_RST_POP_VAL_REG_array_read
#define scpb_reg_reg_RPTR_RST_POP_VAL_REG_array_read( b_ptr, h_ptr, N ) \
       _scpb_reg_reg_RPTR_RST_POP_VAL_REG_array_read( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_reg_RPTR_RST_POP_VAL_REG_array_read( LOG_CALLER_DEFN
                                                                    scpb_reg_buffer_t *b_ptr,
                                                                    cpb_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_RPTR_RST_POP_VAL_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "scpb_reg_reg_RPTR_RST_POP_VAL_REG_array_read", reg_value, N);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_FIFO_PTR_EMPTY_REG
 *
 * Register: Pop Port Packet Empty Status
 * Offset: (0x00102600 + (N) * 0x4) FIFO_PTR_EMPTY_REG
 *
 * bits 31:0 default 0xFFFFFFFF FIFO_PKT_EMPTY
 */

#ifndef scpb_reg_reg_FIFO_PTR_EMPTY_REG_array_read
#define scpb_reg_reg_FIFO_PTR_EMPTY_REG_array_read( b_ptr, h_ptr, N ) \
       _scpb_reg_reg_FIFO_PTR_EMPTY_REG_array_read( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_reg_FIFO_PTR_EMPTY_REG_array_read( LOG_CALLER_DEFN
                                                                  scpb_reg_buffer_t *b_ptr,
                                                                  cpb_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_FIFO_PTR_EMPTY_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "scpb_reg_reg_FIFO_PTR_EMPTY_REG_array_read", reg_value, N);
    return reg_value;
}

#ifndef scpb_reg_reg_FIFO_PTR_EMPTY_REG_array_poll
#define scpb_reg_reg_FIFO_PTR_EMPTY_REG_array_poll( b_ptr, h_ptr, N, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_reg_FIFO_PTR_EMPTY_REG_array_poll( LOG_CALLER b_ptr, h_ptr, N, mask, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_reg_FIFO_PTR_EMPTY_REG_array_poll( LOG_CALLER_DEFN
                                                                                scpb_reg_buffer_t *b_ptr,
                                                                                cpb_handle_t *h_ptr,
                                                                                UINT32  N,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "scpb_reg_reg_FIFO_PTR_EMPTY_REG_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return scpb_reg_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_SCPB_REG_REG_FIFO_PTR_EMPTY_REG(N),
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


/**
 * accessor functions for register PMC_SCPB_REG_REG_FIFO_UDR_STAT_REG_0
 *
 * Register: Pop Port FIFO Underrun Status
 * Offset: (0x00103000 + (N) * 0x4) FIFO_UDR_STAT_REG_0
 *
 * bits        31:0 default 0x00000000 FIFO_UDR_STATUS
 * field_bits  31:0
 */

#ifndef scpb_reg_reg_FIFO_UDR_STAT_REG_0_array_read
#define scpb_reg_reg_FIFO_UDR_STAT_REG_0_array_read( b_ptr, h_ptr, N ) \
       _scpb_reg_reg_FIFO_UDR_STAT_REG_0_array_read( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_reg_FIFO_UDR_STAT_REG_0_array_read( LOG_CALLER_DEFN
                                                                   scpb_reg_buffer_t *b_ptr,
                                                                   cpb_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_FIFO_UDR_STAT_REG_0(N));

    IOLOG( "%s -> 0x%08x; N=%d", "scpb_reg_reg_FIFO_UDR_STAT_REG_0_array_read", reg_value, N);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_P0_FIFO_OVR_STAT_REG
 *
 * Register: Push Port 0 FIFO Overflow Status
 * Offset: (0x00103100 + (N) * 0x4) P0_FIFO_OVR_STAT_REG
 *
 * bits 31:0 default 0x00000000 P0_FIFO_OVR_STATUS
 */

#ifndef scpb_reg_reg_P0_FIFO_OVR_STAT_REG_array_read
#define scpb_reg_reg_P0_FIFO_OVR_STAT_REG_array_read( b_ptr, h_ptr, N ) \
       _scpb_reg_reg_P0_FIFO_OVR_STAT_REG_array_read( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_reg_P0_FIFO_OVR_STAT_REG_array_read( LOG_CALLER_DEFN
                                                                    scpb_reg_buffer_t *b_ptr,
                                                                    cpb_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_P0_FIFO_OVR_STAT_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "scpb_reg_reg_P0_FIFO_OVR_STAT_REG_array_read", reg_value, N);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_P1_FIFO_OVR_STAT_REG
 *
 * Register: Push Port 1 FIFO Overflow Status
 * Offset: (0x00103200 + (N) * 0x4) P1_FIFO_OVR_STAT_REG
 *
 * bits 31:0 default 0x00000000 P1_FIFO_OVR_STATUS
 */

#ifndef scpb_reg_reg_P1_FIFO_OVR_STAT_REG_array_read
#define scpb_reg_reg_P1_FIFO_OVR_STAT_REG_array_read( b_ptr, h_ptr, N ) \
       _scpb_reg_reg_P1_FIFO_OVR_STAT_REG_array_read( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_reg_P1_FIFO_OVR_STAT_REG_array_read( LOG_CALLER_DEFN
                                                                    scpb_reg_buffer_t *b_ptr,
                                                                    cpb_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_P1_FIFO_OVR_STAT_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "scpb_reg_reg_P1_FIFO_OVR_STAT_REG_array_read", reg_value, N);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_P2_FIFO_OVR_STAT_REG
 *
 * Register: Push Port 2 FIFO Overflow Status
 * Offset: (0x00103400 + (N) * 0x4) P2_FIFO_OVR_STAT_REG
 *
 * bits 31:0 default 0x00000000 P2_FIFO_OVR_STATUS
 */

#ifndef scpb_reg_reg_P2_FIFO_OVR_STAT_REG_array_read
#define scpb_reg_reg_P2_FIFO_OVR_STAT_REG_array_read( b_ptr, h_ptr, N ) \
       _scpb_reg_reg_P2_FIFO_OVR_STAT_REG_array_read( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_reg_P2_FIFO_OVR_STAT_REG_array_read( LOG_CALLER_DEFN
                                                                    scpb_reg_buffer_t *b_ptr,
                                                                    cpb_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_P2_FIFO_OVR_STAT_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "scpb_reg_reg_P2_FIFO_OVR_STAT_REG_array_read", reg_value, N);
    return reg_value;
}

/**
 * accessor functions for register PMC_SCPB_REG_REG_P3_FIFO_OVR_STAT_REG
 *
 * Register: Push Port 3 FIFO Overflow Status
 * Offset: (0x00103500 + (N) * 0x4) P3_FIFO_OVR_STAT_REG
 *
 * bits 31:0 default 0x00000000 P3_FIFO_OVR_STATUS
 */

#ifndef scpb_reg_reg_P3_FIFO_OVR_STAT_REG_array_read
#define scpb_reg_reg_P3_FIFO_OVR_STAT_REG_array_read( b_ptr, h_ptr, N ) \
       _scpb_reg_reg_P3_FIFO_OVR_STAT_REG_array_read( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_reg_P3_FIFO_OVR_STAT_REG_array_read( LOG_CALLER_DEFN
                                                                    scpb_reg_buffer_t *b_ptr,
                                                                    cpb_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = scpb_reg_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SCPB_REG_REG_P3_FIFO_OVR_STAT_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "scpb_reg_reg_P3_FIFO_OVR_STAT_REG_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */

#ifndef scpb_reg_field_REMAP_OLD_BLOCK_ADDR_set
#define scpb_reg_field_REMAP_OLD_BLOCK_ADDR_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_REMAP_OLD_BLOCK_ADDR_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_REMAP_OLD_BLOCK_ADDR_set( LOG_CALLER_DEFN
                                                             scpb_reg_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_REMAP_OLD_BLOCK_ADDR_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_REMAP_OLD_BLOCK_ADDR_set", value );

    /* (0x00104204 bits 15:0) field REMAP_OLD_BLOCK_ADDR of register PMC_SCPB_REG_REG_REMAP_OLD_ADDR */
    _scpb_reg_reg_REMAP_OLD_ADDR_field_set( LOG_CALLER_ARGS
                                            b_ptr,
                                            h_ptr,
                                            SCPB_REG_REG_REMAP_OLD_ADDR_BIT_REMAP_OLD_BLOCK_ADDR_MSK,
                                            SCPB_REG_REG_REMAP_OLD_ADDR_BIT_REMAP_OLD_BLOCK_ADDR_OFF,
                                            value);
}


#ifndef scpb_reg_field_REMAP_OLD_BLOCK_ADDR_get
#define scpb_reg_field_REMAP_OLD_BLOCK_ADDR_get( b_ptr, h_ptr ) \
       _scpb_reg_field_REMAP_OLD_BLOCK_ADDR_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_REMAP_OLD_BLOCK_ADDR_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00104204 bits 15:0) field REMAP_OLD_BLOCK_ADDR of register PMC_SCPB_REG_REG_REMAP_OLD_ADDR */
    reg_value = _scpb_reg_reg_REMAP_OLD_ADDR_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    value = (reg_value & SCPB_REG_REG_REMAP_OLD_ADDR_BIT_REMAP_OLD_BLOCK_ADDR_MSK) >> SCPB_REG_REG_REMAP_OLD_ADDR_BIT_REMAP_OLD_BLOCK_ADDR_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_REMAP_OLD_BLOCK_ADDR_get", value );

    return value;
}

#ifndef scpb_reg_field_range_REMAP_OLD_BLOCK_ADDR_set
#define scpb_reg_field_range_REMAP_OLD_BLOCK_ADDR_set( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_REMAP_OLD_BLOCK_ADDR_set( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_REMAP_OLD_BLOCK_ADDR_set( LOG_CALLER_DEFN
                                                                   scpb_reg_buffer_t *b_ptr,
                                                                   cpb_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_REMAP_OLD_BLOCK_ADDR_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_REMAP_OLD_BLOCK_ADDR_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_REMAP_OLD_BLOCK_ADDR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00104204 bits 15:0) field REMAP_OLD_BLOCK_ADDR of register PMC_SCPB_REG_REG_REMAP_OLD_ADDR */
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
        /* (0x00104204 bits 15:0) field REMAP_OLD_BLOCK_ADDR of register PMC_SCPB_REG_REG_REMAP_OLD_ADDR */
        _scpb_reg_reg_REMAP_OLD_ADDR_field_set( LOG_CALLER_ARGS
                                                b_ptr,
                                                h_ptr,
                                                subfield_mask << (SCPB_REG_REG_REMAP_OLD_ADDR_BIT_REMAP_OLD_BLOCK_ADDR_OFF + subfield_offset),
                                                SCPB_REG_REG_REMAP_OLD_ADDR_BIT_REMAP_OLD_BLOCK_ADDR_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_REMAP_OLD_BLOCK_ADDR_get
#define scpb_reg_field_range_REMAP_OLD_BLOCK_ADDR_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_REMAP_OLD_BLOCK_ADDR_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_REMAP_OLD_BLOCK_ADDR_get( LOG_CALLER_DEFN
                                                                     scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_REMAP_OLD_BLOCK_ADDR_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_REMAP_OLD_BLOCK_ADDR_get", stop_bit, 15 );
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
    /* (0x00104204 bits 15:0) field REMAP_OLD_BLOCK_ADDR of register PMC_SCPB_REG_REG_REMAP_OLD_ADDR */
    reg_value = _scpb_reg_reg_REMAP_OLD_ADDR_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    field_value = (reg_value & SCPB_REG_REG_REMAP_OLD_ADDR_BIT_REMAP_OLD_BLOCK_ADDR_MSK)
                  >> SCPB_REG_REG_REMAP_OLD_ADDR_BIT_REMAP_OLD_BLOCK_ADDR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_REMAP_OLD_ADDR_BIT_REMAP_OLD_BLOCK_ADDR_MSK, SCPB_REG_REG_REMAP_OLD_ADDR_BIT_REMAP_OLD_BLOCK_ADDR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_REMAP_OLD_BLOCK_ADDR_get", start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_REMAP_NEW_BLOCK_ADDR_set
#define scpb_reg_field_REMAP_NEW_BLOCK_ADDR_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_REMAP_NEW_BLOCK_ADDR_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_REMAP_NEW_BLOCK_ADDR_set( LOG_CALLER_DEFN
                                                             scpb_reg_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_REMAP_NEW_BLOCK_ADDR_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_REMAP_NEW_BLOCK_ADDR_set", value );

    /* (0x00104208 bits 15:0) field REMAP_NEW_BLOCK_ADDR of register PMC_SCPB_REG_REG_REMAP_NEW_ADDR */
    _scpb_reg_reg_REMAP_NEW_ADDR_field_set( LOG_CALLER_ARGS
                                            b_ptr,
                                            h_ptr,
                                            SCPB_REG_REG_REMAP_NEW_ADDR_BIT_REMAP_NEW_BLOCK_ADDR_MSK,
                                            SCPB_REG_REG_REMAP_NEW_ADDR_BIT_REMAP_NEW_BLOCK_ADDR_OFF,
                                            value);
}


#ifndef scpb_reg_field_REMAP_NEW_BLOCK_ADDR_get
#define scpb_reg_field_REMAP_NEW_BLOCK_ADDR_get( b_ptr, h_ptr ) \
       _scpb_reg_field_REMAP_NEW_BLOCK_ADDR_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_REMAP_NEW_BLOCK_ADDR_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00104208 bits 15:0) field REMAP_NEW_BLOCK_ADDR of register PMC_SCPB_REG_REG_REMAP_NEW_ADDR */
    reg_value = _scpb_reg_reg_REMAP_NEW_ADDR_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    value = (reg_value & SCPB_REG_REG_REMAP_NEW_ADDR_BIT_REMAP_NEW_BLOCK_ADDR_MSK) >> SCPB_REG_REG_REMAP_NEW_ADDR_BIT_REMAP_NEW_BLOCK_ADDR_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_REMAP_NEW_BLOCK_ADDR_get", value );

    return value;
}

#ifndef scpb_reg_field_range_REMAP_NEW_BLOCK_ADDR_set
#define scpb_reg_field_range_REMAP_NEW_BLOCK_ADDR_set( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_REMAP_NEW_BLOCK_ADDR_set( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_REMAP_NEW_BLOCK_ADDR_set( LOG_CALLER_DEFN
                                                                   scpb_reg_buffer_t *b_ptr,
                                                                   cpb_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_REMAP_NEW_BLOCK_ADDR_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_REMAP_NEW_BLOCK_ADDR_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_REMAP_NEW_BLOCK_ADDR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00104208 bits 15:0) field REMAP_NEW_BLOCK_ADDR of register PMC_SCPB_REG_REG_REMAP_NEW_ADDR */
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
        /* (0x00104208 bits 15:0) field REMAP_NEW_BLOCK_ADDR of register PMC_SCPB_REG_REG_REMAP_NEW_ADDR */
        _scpb_reg_reg_REMAP_NEW_ADDR_field_set( LOG_CALLER_ARGS
                                                b_ptr,
                                                h_ptr,
                                                subfield_mask << (SCPB_REG_REG_REMAP_NEW_ADDR_BIT_REMAP_NEW_BLOCK_ADDR_OFF + subfield_offset),
                                                SCPB_REG_REG_REMAP_NEW_ADDR_BIT_REMAP_NEW_BLOCK_ADDR_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_REMAP_NEW_BLOCK_ADDR_get
#define scpb_reg_field_range_REMAP_NEW_BLOCK_ADDR_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_REMAP_NEW_BLOCK_ADDR_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_REMAP_NEW_BLOCK_ADDR_get( LOG_CALLER_DEFN
                                                                     scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_REMAP_NEW_BLOCK_ADDR_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_REMAP_NEW_BLOCK_ADDR_get", stop_bit, 15 );
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
    /* (0x00104208 bits 15:0) field REMAP_NEW_BLOCK_ADDR of register PMC_SCPB_REG_REG_REMAP_NEW_ADDR */
    reg_value = _scpb_reg_reg_REMAP_NEW_ADDR_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    field_value = (reg_value & SCPB_REG_REG_REMAP_NEW_ADDR_BIT_REMAP_NEW_BLOCK_ADDR_MSK)
                  >> SCPB_REG_REG_REMAP_NEW_ADDR_BIT_REMAP_NEW_BLOCK_ADDR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_REMAP_NEW_ADDR_BIT_REMAP_NEW_BLOCK_ADDR_MSK, SCPB_REG_REG_REMAP_NEW_ADDR_BIT_REMAP_NEW_BLOCK_ADDR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_REMAP_NEW_BLOCK_ADDR_get", start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_REMAP_WPTR_MARGIN_set
#define scpb_reg_field_REMAP_WPTR_MARGIN_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_REMAP_WPTR_MARGIN_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_REMAP_WPTR_MARGIN_set( LOG_CALLER_DEFN
                                                          scpb_reg_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_REMAP_WPTR_MARGIN_set", value, 255);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_REMAP_WPTR_MARGIN_set", value );

    /* (0x00104210 bits 7:0) field REMAP_WPTR_MARGIN of register PMC_SCPB_REG_REG_REMAP_MARGIN */
    _scpb_reg_reg_REMAP_MARGIN_field_set( LOG_CALLER_ARGS
                                          b_ptr,
                                          h_ptr,
                                          SCPB_REG_REG_REMAP_MARGIN_BIT_REMAP_WPTR_MARGIN_MSK,
                                          SCPB_REG_REG_REMAP_MARGIN_BIT_REMAP_WPTR_MARGIN_OFF,
                                          value);
}


#ifndef scpb_reg_field_REMAP_WPTR_MARGIN_get
#define scpb_reg_field_REMAP_WPTR_MARGIN_get( b_ptr, h_ptr ) \
       _scpb_reg_field_REMAP_WPTR_MARGIN_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_REMAP_WPTR_MARGIN_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00104210 bits 7:0) field REMAP_WPTR_MARGIN of register PMC_SCPB_REG_REG_REMAP_MARGIN */
    reg_value = _scpb_reg_reg_REMAP_MARGIN_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    value = (reg_value & SCPB_REG_REG_REMAP_MARGIN_BIT_REMAP_WPTR_MARGIN_MSK) >> SCPB_REG_REG_REMAP_MARGIN_BIT_REMAP_WPTR_MARGIN_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_REMAP_WPTR_MARGIN_get", value );

    return value;
}

#ifndef scpb_reg_field_range_REMAP_WPTR_MARGIN_set
#define scpb_reg_field_range_REMAP_WPTR_MARGIN_set( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_REMAP_WPTR_MARGIN_set( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_REMAP_WPTR_MARGIN_set( LOG_CALLER_DEFN
                                                                scpb_reg_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_REMAP_WPTR_MARGIN_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_REMAP_WPTR_MARGIN_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_REMAP_WPTR_MARGIN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00104210 bits 7:0) field REMAP_WPTR_MARGIN of register PMC_SCPB_REG_REG_REMAP_MARGIN */
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
        /* (0x00104210 bits 7:0) field REMAP_WPTR_MARGIN of register PMC_SCPB_REG_REG_REMAP_MARGIN */
        _scpb_reg_reg_REMAP_MARGIN_field_set( LOG_CALLER_ARGS
                                              b_ptr,
                                              h_ptr,
                                              subfield_mask << (SCPB_REG_REG_REMAP_MARGIN_BIT_REMAP_WPTR_MARGIN_OFF + subfield_offset),
                                              SCPB_REG_REG_REMAP_MARGIN_BIT_REMAP_WPTR_MARGIN_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_REMAP_WPTR_MARGIN_get
#define scpb_reg_field_range_REMAP_WPTR_MARGIN_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_REMAP_WPTR_MARGIN_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_REMAP_WPTR_MARGIN_get( LOG_CALLER_DEFN
                                                                  scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_REMAP_WPTR_MARGIN_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_REMAP_WPTR_MARGIN_get", stop_bit, 7 );
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
    /* (0x00104210 bits 7:0) field REMAP_WPTR_MARGIN of register PMC_SCPB_REG_REG_REMAP_MARGIN */
    reg_value = _scpb_reg_reg_REMAP_MARGIN_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    field_value = (reg_value & SCPB_REG_REG_REMAP_MARGIN_BIT_REMAP_WPTR_MARGIN_MSK)
                  >> SCPB_REG_REG_REMAP_MARGIN_BIT_REMAP_WPTR_MARGIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_REMAP_MARGIN_BIT_REMAP_WPTR_MARGIN_MSK, SCPB_REG_REG_REMAP_MARGIN_BIT_REMAP_WPTR_MARGIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_REMAP_WPTR_MARGIN_get", start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_REMAP_WQID_set
#define scpb_reg_field_REMAP_WQID_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_REMAP_WQID_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_REMAP_WQID_set( LOG_CALLER_DEFN
                                                   scpb_reg_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_REMAP_WQID_set", value, 255);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_REMAP_WQID_set", value );

    /* (0x00104214 bits 23:16) field REMAP_WQID of register PMC_SCPB_REG_REG_REMAP_OLD_BLK_CFG */
    _scpb_reg_reg_REMAP_OLD_BLK_CFG_field_set( LOG_CALLER_ARGS
                                               b_ptr,
                                               h_ptr,
                                               SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_WQID_MSK,
                                               SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_WQID_OFF,
                                               value);
}


#ifndef scpb_reg_field_REMAP_WQID_get
#define scpb_reg_field_REMAP_WQID_get( b_ptr, h_ptr ) \
       _scpb_reg_field_REMAP_WQID_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_REMAP_WQID_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00104214 bits 23:16) field REMAP_WQID of register PMC_SCPB_REG_REG_REMAP_OLD_BLK_CFG */
    reg_value = _scpb_reg_reg_REMAP_OLD_BLK_CFG_read( LOG_CALLER_ARGS
                                                      b_ptr,
                                                      h_ptr);
    value = (reg_value & SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_WQID_MSK) >> SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_WQID_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_REMAP_WQID_get", value );

    return value;
}

#ifndef scpb_reg_field_range_REMAP_WQID_set
#define scpb_reg_field_range_REMAP_WQID_set( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_REMAP_WQID_set( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_REMAP_WQID_set( LOG_CALLER_DEFN
                                                         scpb_reg_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_REMAP_WQID_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_REMAP_WQID_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_REMAP_WQID_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00104214 bits 23:16) field REMAP_WQID of register PMC_SCPB_REG_REG_REMAP_OLD_BLK_CFG */
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
        /* (0x00104214 bits 23:16) field REMAP_WQID of register PMC_SCPB_REG_REG_REMAP_OLD_BLK_CFG */
        _scpb_reg_reg_REMAP_OLD_BLK_CFG_field_set( LOG_CALLER_ARGS
                                                   b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_WQID_OFF + subfield_offset),
                                                   SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_WQID_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_REMAP_WQID_get
#define scpb_reg_field_range_REMAP_WQID_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_REMAP_WQID_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_REMAP_WQID_get( LOG_CALLER_DEFN
                                                           scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_REMAP_WQID_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_REMAP_WQID_get", stop_bit, 7 );
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
    /* (0x00104214 bits 23:16) field REMAP_WQID of register PMC_SCPB_REG_REG_REMAP_OLD_BLK_CFG */
    reg_value = _scpb_reg_reg_REMAP_OLD_BLK_CFG_read( LOG_CALLER_ARGS
                                                      b_ptr,
                                                      h_ptr);
    field_value = (reg_value & SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_WQID_MSK)
                  >> SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_WQID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_WQID_MSK, SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_WQID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_REMAP_WQID_get", start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_REMAP_LBA_set
#define scpb_reg_field_REMAP_LBA_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_REMAP_LBA_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_REMAP_LBA_set( LOG_CALLER_DEFN
                                                  scpb_reg_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_REMAP_LBA_set", value, 511);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_REMAP_LBA_set", value );

    /* (0x00104214 bits 8:0) field REMAP_LBA of register PMC_SCPB_REG_REG_REMAP_OLD_BLK_CFG */
    _scpb_reg_reg_REMAP_OLD_BLK_CFG_field_set( LOG_CALLER_ARGS
                                               b_ptr,
                                               h_ptr,
                                               SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_LBA_MSK,
                                               SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_LBA_OFF,
                                               value);
}


#ifndef scpb_reg_field_REMAP_LBA_get
#define scpb_reg_field_REMAP_LBA_get( b_ptr, h_ptr ) \
       _scpb_reg_field_REMAP_LBA_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_REMAP_LBA_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00104214 bits 8:0) field REMAP_LBA of register PMC_SCPB_REG_REG_REMAP_OLD_BLK_CFG */
    reg_value = _scpb_reg_reg_REMAP_OLD_BLK_CFG_read( LOG_CALLER_ARGS
                                                      b_ptr,
                                                      h_ptr);
    value = (reg_value & SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_LBA_MSK) >> SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_LBA_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_REMAP_LBA_get", value );

    return value;
}

#ifndef scpb_reg_field_range_REMAP_LBA_set
#define scpb_reg_field_range_REMAP_LBA_set( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_REMAP_LBA_set( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_REMAP_LBA_set( LOG_CALLER_DEFN
                                                        scpb_reg_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_REMAP_LBA_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_REMAP_LBA_set", stop_bit, 8 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_REMAP_LBA_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00104214 bits 8:0) field REMAP_LBA of register PMC_SCPB_REG_REG_REMAP_OLD_BLK_CFG */
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
        /* (0x00104214 bits 8:0) field REMAP_LBA of register PMC_SCPB_REG_REG_REMAP_OLD_BLK_CFG */
        _scpb_reg_reg_REMAP_OLD_BLK_CFG_field_set( LOG_CALLER_ARGS
                                                   b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_LBA_OFF + subfield_offset),
                                                   SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_LBA_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_REMAP_LBA_get
#define scpb_reg_field_range_REMAP_LBA_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_REMAP_LBA_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_REMAP_LBA_get( LOG_CALLER_DEFN
                                                          scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_REMAP_LBA_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_REMAP_LBA_get", stop_bit, 8 );
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
    /* (0x00104214 bits 8:0) field REMAP_LBA of register PMC_SCPB_REG_REG_REMAP_OLD_BLK_CFG */
    reg_value = _scpb_reg_reg_REMAP_OLD_BLK_CFG_read( LOG_CALLER_ARGS
                                                      b_ptr,
                                                      h_ptr);
    field_value = (reg_value & SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_LBA_MSK)
                  >> SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_LBA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_LBA_MSK, SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_LBA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_REMAP_LBA_get", start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_REMAP_QS_ID_set
#define scpb_reg_field_REMAP_QS_ID_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_REMAP_QS_ID_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_REMAP_QS_ID_set( LOG_CALLER_DEFN
                                                    scpb_reg_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_REMAP_QS_ID_set", value, 3);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_REMAP_QS_ID_set", value );

    /* (0x00104218 bits 17:16) field REMAP_QS_ID of register PMC_SCPB_REG_REG_REMAP_OLD_PAGE_CFG */
    _scpb_reg_reg_REMAP_OLD_PAGE_CFG_field_set( LOG_CALLER_ARGS
                                                b_ptr,
                                                h_ptr,
                                                SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_QS_ID_MSK,
                                                SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_QS_ID_OFF,
                                                value);
}


#ifndef scpb_reg_field_REMAP_QS_ID_get
#define scpb_reg_field_REMAP_QS_ID_get( b_ptr, h_ptr ) \
       _scpb_reg_field_REMAP_QS_ID_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_REMAP_QS_ID_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00104218 bits 17:16) field REMAP_QS_ID of register PMC_SCPB_REG_REG_REMAP_OLD_PAGE_CFG */
    reg_value = _scpb_reg_reg_REMAP_OLD_PAGE_CFG_read( LOG_CALLER_ARGS
                                                       b_ptr,
                                                       h_ptr);
    value = (reg_value & SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_QS_ID_MSK) >> SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_QS_ID_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_REMAP_QS_ID_get", value );

    return value;
}

#ifndef scpb_reg_field_range_REMAP_QS_ID_set
#define scpb_reg_field_range_REMAP_QS_ID_set( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_REMAP_QS_ID_set( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_REMAP_QS_ID_set( LOG_CALLER_DEFN
                                                          scpb_reg_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_REMAP_QS_ID_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_REMAP_QS_ID_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_REMAP_QS_ID_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00104218 bits 17:16) field REMAP_QS_ID of register PMC_SCPB_REG_REG_REMAP_OLD_PAGE_CFG */
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
        /* (0x00104218 bits 17:16) field REMAP_QS_ID of register PMC_SCPB_REG_REG_REMAP_OLD_PAGE_CFG */
        _scpb_reg_reg_REMAP_OLD_PAGE_CFG_field_set( LOG_CALLER_ARGS
                                                    b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_QS_ID_OFF + subfield_offset),
                                                    SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_QS_ID_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_REMAP_QS_ID_get
#define scpb_reg_field_range_REMAP_QS_ID_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_REMAP_QS_ID_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_REMAP_QS_ID_get( LOG_CALLER_DEFN
                                                            scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_REMAP_QS_ID_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_REMAP_QS_ID_get", stop_bit, 1 );
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
    /* (0x00104218 bits 17:16) field REMAP_QS_ID of register PMC_SCPB_REG_REG_REMAP_OLD_PAGE_CFG */
    reg_value = _scpb_reg_reg_REMAP_OLD_PAGE_CFG_read( LOG_CALLER_ARGS
                                                       b_ptr,
                                                       h_ptr);
    field_value = (reg_value & SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_QS_ID_MSK)
                  >> SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_QS_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_QS_ID_MSK, SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_QS_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_REMAP_QS_ID_get", start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_REMAP_WP_ID_set
#define scpb_reg_field_REMAP_WP_ID_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_REMAP_WP_ID_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_REMAP_WP_ID_set( LOG_CALLER_DEFN
                                                    scpb_reg_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_REMAP_WP_ID_set", value, 3);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_REMAP_WP_ID_set", value );

    /* (0x00104218 bits 9:8) field REMAP_WP_ID of register PMC_SCPB_REG_REG_REMAP_OLD_PAGE_CFG */
    _scpb_reg_reg_REMAP_OLD_PAGE_CFG_field_set( LOG_CALLER_ARGS
                                                b_ptr,
                                                h_ptr,
                                                SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_WP_ID_MSK,
                                                SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_WP_ID_OFF,
                                                value);
}


#ifndef scpb_reg_field_REMAP_WP_ID_get
#define scpb_reg_field_REMAP_WP_ID_get( b_ptr, h_ptr ) \
       _scpb_reg_field_REMAP_WP_ID_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_REMAP_WP_ID_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00104218 bits 9:8) field REMAP_WP_ID of register PMC_SCPB_REG_REG_REMAP_OLD_PAGE_CFG */
    reg_value = _scpb_reg_reg_REMAP_OLD_PAGE_CFG_read( LOG_CALLER_ARGS
                                                       b_ptr,
                                                       h_ptr);
    value = (reg_value & SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_WP_ID_MSK) >> SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_WP_ID_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_REMAP_WP_ID_get", value );

    return value;
}

#ifndef scpb_reg_field_range_REMAP_WP_ID_set
#define scpb_reg_field_range_REMAP_WP_ID_set( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_REMAP_WP_ID_set( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_REMAP_WP_ID_set( LOG_CALLER_DEFN
                                                          scpb_reg_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_REMAP_WP_ID_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_REMAP_WP_ID_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_REMAP_WP_ID_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00104218 bits 9:8) field REMAP_WP_ID of register PMC_SCPB_REG_REG_REMAP_OLD_PAGE_CFG */
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
        /* (0x00104218 bits 9:8) field REMAP_WP_ID of register PMC_SCPB_REG_REG_REMAP_OLD_PAGE_CFG */
        _scpb_reg_reg_REMAP_OLD_PAGE_CFG_field_set( LOG_CALLER_ARGS
                                                    b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_WP_ID_OFF + subfield_offset),
                                                    SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_WP_ID_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_REMAP_WP_ID_get
#define scpb_reg_field_range_REMAP_WP_ID_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_REMAP_WP_ID_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_REMAP_WP_ID_get( LOG_CALLER_DEFN
                                                            scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_REMAP_WP_ID_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_REMAP_WP_ID_get", stop_bit, 1 );
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
    /* (0x00104218 bits 9:8) field REMAP_WP_ID of register PMC_SCPB_REG_REG_REMAP_OLD_PAGE_CFG */
    reg_value = _scpb_reg_reg_REMAP_OLD_PAGE_CFG_read( LOG_CALLER_ARGS
                                                       b_ptr,
                                                       h_ptr);
    field_value = (reg_value & SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_WP_ID_MSK)
                  >> SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_WP_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_WP_ID_MSK, SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_WP_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_REMAP_WP_ID_get", start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_IPT_BYPASS_set
#define scpb_reg_field_IPT_BYPASS_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_IPT_BYPASS_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_IPT_BYPASS_set( LOG_CALLER_DEFN
                                                   scpb_reg_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_IPT_BYPASS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_IPT_BYPASS_set", value );

    /* (0x00103808 bits 0) field IPT_BYPASS of register PMC_SCPB_REG_REG_FIFO_IPT_BYPASS_REG */
    _scpb_reg_reg_FIFO_IPT_BYPASS_REG_field_set( LOG_CALLER_ARGS
                                                 b_ptr,
                                                 h_ptr,
                                                 SCPB_REG_REG_FIFO_IPT_BYPASS_REG_BIT_IPT_BYPASS_MSK,
                                                 SCPB_REG_REG_FIFO_IPT_BYPASS_REG_BIT_IPT_BYPASS_OFF,
                                                 value);
}


#ifndef scpb_reg_field_IPT_BYPASS_get
#define scpb_reg_field_IPT_BYPASS_get( b_ptr, h_ptr ) \
       _scpb_reg_field_IPT_BYPASS_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_IPT_BYPASS_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00103808 bits 0) field IPT_BYPASS of register PMC_SCPB_REG_REG_FIFO_IPT_BYPASS_REG */
    reg_value = _scpb_reg_reg_FIFO_IPT_BYPASS_REG_read( LOG_CALLER_ARGS
                                                        b_ptr,
                                                        h_ptr);
    value = (reg_value & SCPB_REG_REG_FIFO_IPT_BYPASS_REG_BIT_IPT_BYPASS_MSK) >> SCPB_REG_REG_FIFO_IPT_BYPASS_REG_BIT_IPT_BYPASS_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_IPT_BYPASS_get", value );

    return value;
}

#ifndef scpb_reg_field_FIFO_END_ADDR_set
#define scpb_reg_field_FIFO_END_ADDR_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_FIFO_END_ADDR_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_FIFO_END_ADDR_set( LOG_CALLER_DEFN
                                                      scpb_reg_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 131071)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_FIFO_END_ADDR_set", value, 131071);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_FIFO_END_ADDR_set", value );

    /* (0x00103d00 bits 16:0) field FIFO_END_ADDR of register PMC_SCPB_REG_REG_FIFO_CFG */
    _scpb_reg_reg_FIFO_CFG_field_set( LOG_CALLER_ARGS
                                      b_ptr,
                                      h_ptr,
                                      SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_MSK,
                                      SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_OFF,
                                      value);
}


#ifndef scpb_reg_field_FIFO_END_ADDR_get
#define scpb_reg_field_FIFO_END_ADDR_get( b_ptr, h_ptr ) \
       _scpb_reg_field_FIFO_END_ADDR_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_FIFO_END_ADDR_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00103d00 bits 16:0) field FIFO_END_ADDR of register PMC_SCPB_REG_REG_FIFO_CFG */
    reg_value = _scpb_reg_reg_FIFO_CFG_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    value = (reg_value & SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_MSK) >> SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_FIFO_END_ADDR_get", value );

    return value;
}

#ifndef scpb_reg_field_range_FIFO_END_ADDR_set
#define scpb_reg_field_range_FIFO_END_ADDR_set( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_FIFO_END_ADDR_set( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_FIFO_END_ADDR_set( LOG_CALLER_DEFN
                                                            scpb_reg_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_FIFO_END_ADDR_set", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_FIFO_END_ADDR_set", stop_bit, 16 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_FIFO_END_ADDR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00103d00 bits 16:0) field FIFO_END_ADDR of register PMC_SCPB_REG_REG_FIFO_CFG */
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
        if (stop_bit < 16) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 16;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00103d00 bits 16:0) field FIFO_END_ADDR of register PMC_SCPB_REG_REG_FIFO_CFG */
        _scpb_reg_reg_FIFO_CFG_field_set( LOG_CALLER_ARGS
                                          b_ptr,
                                          h_ptr,
                                          subfield_mask << (SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_OFF + subfield_offset),
                                          SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_FIFO_END_ADDR_get
#define scpb_reg_field_range_FIFO_END_ADDR_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_FIFO_END_ADDR_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_FIFO_END_ADDR_get( LOG_CALLER_DEFN
                                                              scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_FIFO_END_ADDR_get", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_FIFO_END_ADDR_get", stop_bit, 16 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 16) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 16;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00103d00 bits 16:0) field FIFO_END_ADDR of register PMC_SCPB_REG_REG_FIFO_CFG */
    reg_value = _scpb_reg_reg_FIFO_CFG_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    field_value = (reg_value & SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_MSK)
                  >> SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_MSK, SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_FIFO_END_ADDR_get", start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_FIFO_END_ADDR_CHG_RID_set
#define scpb_reg_field_FIFO_END_ADDR_CHG_RID_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_FIFO_END_ADDR_CHG_RID_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_FIFO_END_ADDR_CHG_RID_set( LOG_CALLER_DEFN
                                                              scpb_reg_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_FIFO_END_ADDR_CHG_RID_set", value, 255);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_FIFO_END_ADDR_CHG_RID_set", value );

    /* (0x00103d04 bits 7:0) field FIFO_END_ADDR_CHG_RID of register PMC_SCPB_REG_REG_FIFO_CFG_RID */
    _scpb_reg_reg_FIFO_CFG_RID_field_set( LOG_CALLER_ARGS
                                          b_ptr,
                                          h_ptr,
                                          SCPB_REG_REG_FIFO_CFG_RID_BIT_FIFO_END_ADDR_CHG_RID_MSK,
                                          SCPB_REG_REG_FIFO_CFG_RID_BIT_FIFO_END_ADDR_CHG_RID_OFF,
                                          value);
}


#ifndef scpb_reg_field_FIFO_END_ADDR_CHG_RID_get
#define scpb_reg_field_FIFO_END_ADDR_CHG_RID_get( b_ptr, h_ptr ) \
       _scpb_reg_field_FIFO_END_ADDR_CHG_RID_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_FIFO_END_ADDR_CHG_RID_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00103d04 bits 7:0) field FIFO_END_ADDR_CHG_RID of register PMC_SCPB_REG_REG_FIFO_CFG_RID */
    reg_value = _scpb_reg_reg_FIFO_CFG_RID_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    value = (reg_value & SCPB_REG_REG_FIFO_CFG_RID_BIT_FIFO_END_ADDR_CHG_RID_MSK) >> SCPB_REG_REG_FIFO_CFG_RID_BIT_FIFO_END_ADDR_CHG_RID_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_FIFO_END_ADDR_CHG_RID_get", value );

    return value;
}

#ifndef scpb_reg_field_range_FIFO_END_ADDR_CHG_RID_set
#define scpb_reg_field_range_FIFO_END_ADDR_CHG_RID_set( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_FIFO_END_ADDR_CHG_RID_set( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_FIFO_END_ADDR_CHG_RID_set( LOG_CALLER_DEFN
                                                                    scpb_reg_buffer_t *b_ptr,
                                                                    cpb_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_FIFO_END_ADDR_CHG_RID_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_FIFO_END_ADDR_CHG_RID_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_FIFO_END_ADDR_CHG_RID_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00103d04 bits 7:0) field FIFO_END_ADDR_CHG_RID of register PMC_SCPB_REG_REG_FIFO_CFG_RID */
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
        /* (0x00103d04 bits 7:0) field FIFO_END_ADDR_CHG_RID of register PMC_SCPB_REG_REG_FIFO_CFG_RID */
        _scpb_reg_reg_FIFO_CFG_RID_field_set( LOG_CALLER_ARGS
                                              b_ptr,
                                              h_ptr,
                                              subfield_mask << (SCPB_REG_REG_FIFO_CFG_RID_BIT_FIFO_END_ADDR_CHG_RID_OFF + subfield_offset),
                                              SCPB_REG_REG_FIFO_CFG_RID_BIT_FIFO_END_ADDR_CHG_RID_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_FIFO_END_ADDR_CHG_RID_get
#define scpb_reg_field_range_FIFO_END_ADDR_CHG_RID_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_FIFO_END_ADDR_CHG_RID_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_FIFO_END_ADDR_CHG_RID_get( LOG_CALLER_DEFN
                                                                      scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_FIFO_END_ADDR_CHG_RID_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_FIFO_END_ADDR_CHG_RID_get", stop_bit, 7 );
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
    /* (0x00103d04 bits 7:0) field FIFO_END_ADDR_CHG_RID of register PMC_SCPB_REG_REG_FIFO_CFG_RID */
    reg_value = _scpb_reg_reg_FIFO_CFG_RID_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    field_value = (reg_value & SCPB_REG_REG_FIFO_CFG_RID_BIT_FIFO_END_ADDR_CHG_RID_MSK)
                  >> SCPB_REG_REG_FIFO_CFG_RID_BIT_FIFO_END_ADDR_CHG_RID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_FIFO_CFG_RID_BIT_FIFO_END_ADDR_CHG_RID_MSK, SCPB_REG_REG_FIFO_CFG_RID_BIT_FIFO_END_ADDR_CHG_RID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_FIFO_END_ADDR_CHG_RID_get", start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_FIFO_END_ADDR_CHG_WID_set
#define scpb_reg_field_FIFO_END_ADDR_CHG_WID_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_FIFO_END_ADDR_CHG_WID_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_FIFO_END_ADDR_CHG_WID_set( LOG_CALLER_DEFN
                                                              scpb_reg_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_FIFO_END_ADDR_CHG_WID_set", value, 1023);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_FIFO_END_ADDR_CHG_WID_set", value );

    /* (0x00103d08 bits 9:0) field FIFO_END_ADDR_CHG_WID of register PMC_SCPB_REG_REG_FIFO_CFG_WID */
    _scpb_reg_reg_FIFO_CFG_WID_field_set( LOG_CALLER_ARGS
                                          b_ptr,
                                          h_ptr,
                                          SCPB_REG_REG_FIFO_CFG_WID_BIT_FIFO_END_ADDR_CHG_WID_MSK,
                                          SCPB_REG_REG_FIFO_CFG_WID_BIT_FIFO_END_ADDR_CHG_WID_OFF,
                                          value);
}


#ifndef scpb_reg_field_FIFO_END_ADDR_CHG_WID_get
#define scpb_reg_field_FIFO_END_ADDR_CHG_WID_get( b_ptr, h_ptr ) \
       _scpb_reg_field_FIFO_END_ADDR_CHG_WID_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_FIFO_END_ADDR_CHG_WID_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00103d08 bits 9:0) field FIFO_END_ADDR_CHG_WID of register PMC_SCPB_REG_REG_FIFO_CFG_WID */
    reg_value = _scpb_reg_reg_FIFO_CFG_WID_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    value = (reg_value & SCPB_REG_REG_FIFO_CFG_WID_BIT_FIFO_END_ADDR_CHG_WID_MSK) >> SCPB_REG_REG_FIFO_CFG_WID_BIT_FIFO_END_ADDR_CHG_WID_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_FIFO_END_ADDR_CHG_WID_get", value );

    return value;
}

#ifndef scpb_reg_field_range_FIFO_END_ADDR_CHG_WID_set
#define scpb_reg_field_range_FIFO_END_ADDR_CHG_WID_set( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_FIFO_END_ADDR_CHG_WID_set( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_FIFO_END_ADDR_CHG_WID_set( LOG_CALLER_DEFN
                                                                    scpb_reg_buffer_t *b_ptr,
                                                                    cpb_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_FIFO_END_ADDR_CHG_WID_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_FIFO_END_ADDR_CHG_WID_set", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_FIFO_END_ADDR_CHG_WID_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00103d08 bits 9:0) field FIFO_END_ADDR_CHG_WID of register PMC_SCPB_REG_REG_FIFO_CFG_WID */
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
        /* (0x00103d08 bits 9:0) field FIFO_END_ADDR_CHG_WID of register PMC_SCPB_REG_REG_FIFO_CFG_WID */
        _scpb_reg_reg_FIFO_CFG_WID_field_set( LOG_CALLER_ARGS
                                              b_ptr,
                                              h_ptr,
                                              subfield_mask << (SCPB_REG_REG_FIFO_CFG_WID_BIT_FIFO_END_ADDR_CHG_WID_OFF + subfield_offset),
                                              SCPB_REG_REG_FIFO_CFG_WID_BIT_FIFO_END_ADDR_CHG_WID_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_FIFO_END_ADDR_CHG_WID_get
#define scpb_reg_field_range_FIFO_END_ADDR_CHG_WID_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_FIFO_END_ADDR_CHG_WID_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_FIFO_END_ADDR_CHG_WID_get( LOG_CALLER_DEFN
                                                                      scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_FIFO_END_ADDR_CHG_WID_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_FIFO_END_ADDR_CHG_WID_get", stop_bit, 9 );
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
    /* (0x00103d08 bits 9:0) field FIFO_END_ADDR_CHG_WID of register PMC_SCPB_REG_REG_FIFO_CFG_WID */
    reg_value = _scpb_reg_reg_FIFO_CFG_WID_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    field_value = (reg_value & SCPB_REG_REG_FIFO_CFG_WID_BIT_FIFO_END_ADDR_CHG_WID_MSK)
                  >> SCPB_REG_REG_FIFO_CFG_WID_BIT_FIFO_END_ADDR_CHG_WID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_FIFO_CFG_WID_BIT_FIFO_END_ADDR_CHG_WID_MSK, SCPB_REG_REG_FIFO_CFG_WID_BIT_FIFO_END_ADDR_CHG_WID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_FIFO_END_ADDR_CHG_WID_get", start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_DBG_RADDR_set
#define scpb_reg_field_DBG_RADDR_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_DBG_RADDR_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_DBG_RADDR_set( LOG_CALLER_DEFN
                                                  scpb_reg_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 2047)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_DBG_RADDR_set", value, 2047);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_DBG_RADDR_set", value );

    /* (0x00103e00 bits 10:0) field DBG_RADDR of register PMC_SCPB_REG_REG_DBG_CTRL */
    _scpb_reg_reg_DBG_CTRL_field_set( LOG_CALLER_ARGS
                                      b_ptr,
                                      h_ptr,
                                      SCPB_REG_REG_DBG_CTRL_BIT_DBG_RADDR_MSK,
                                      SCPB_REG_REG_DBG_CTRL_BIT_DBG_RADDR_OFF,
                                      value);
}


#ifndef scpb_reg_field_DBG_RADDR_get
#define scpb_reg_field_DBG_RADDR_get( b_ptr, h_ptr ) \
       _scpb_reg_field_DBG_RADDR_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_DBG_RADDR_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00103e00 bits 10:0) field DBG_RADDR of register PMC_SCPB_REG_REG_DBG_CTRL */
    reg_value = _scpb_reg_reg_DBG_CTRL_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    value = (reg_value & SCPB_REG_REG_DBG_CTRL_BIT_DBG_RADDR_MSK) >> SCPB_REG_REG_DBG_CTRL_BIT_DBG_RADDR_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_DBG_RADDR_get", value );

    return value;
}

#ifndef scpb_reg_field_range_DBG_RADDR_set
#define scpb_reg_field_range_DBG_RADDR_set( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_DBG_RADDR_set( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_DBG_RADDR_set( LOG_CALLER_DEFN
                                                        scpb_reg_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_DBG_RADDR_set", stop_bit, start_bit );
    if (stop_bit > 10) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_DBG_RADDR_set", stop_bit, 10 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_DBG_RADDR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00103e00 bits 10:0) field DBG_RADDR of register PMC_SCPB_REG_REG_DBG_CTRL */
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
        /* (0x00103e00 bits 10:0) field DBG_RADDR of register PMC_SCPB_REG_REG_DBG_CTRL */
        _scpb_reg_reg_DBG_CTRL_field_set( LOG_CALLER_ARGS
                                          b_ptr,
                                          h_ptr,
                                          subfield_mask << (SCPB_REG_REG_DBG_CTRL_BIT_DBG_RADDR_OFF + subfield_offset),
                                          SCPB_REG_REG_DBG_CTRL_BIT_DBG_RADDR_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_DBG_RADDR_get
#define scpb_reg_field_range_DBG_RADDR_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_DBG_RADDR_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_DBG_RADDR_get( LOG_CALLER_DEFN
                                                          scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_DBG_RADDR_get", stop_bit, start_bit );
    if (stop_bit > 10) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_DBG_RADDR_get", stop_bit, 10 );
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
    /* (0x00103e00 bits 10:0) field DBG_RADDR of register PMC_SCPB_REG_REG_DBG_CTRL */
    reg_value = _scpb_reg_reg_DBG_CTRL_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    field_value = (reg_value & SCPB_REG_REG_DBG_CTRL_BIT_DBG_RADDR_MSK)
                  >> SCPB_REG_REG_DBG_CTRL_BIT_DBG_RADDR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_DBG_CTRL_BIT_DBG_RADDR_MSK, SCPB_REG_REG_DBG_CTRL_BIT_DBG_RADDR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_DBG_RADDR_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_burstsize4
 * ==================================================================================
 */

#ifndef scpb_reg_lfield_GPO_set
#define scpb_reg_lfield_GPO_set( b_ptr, h_ptr, value ) \
       _scpb_reg_lfield_GPO_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_lfield_GPO_set( LOG_CALLER_DEFN
                                             scpb_reg_buffer_t *b_ptr,
                                             cpb_handle_t *h_ptr,
                                             UINT32 value[4] )
{
    IOLOG( "%s ", "scpb_reg_lfield_GPO_set");
    _scpb_reg_reg_GPO_array_burst_write( LOG_CALLER_ARGS
                                         b_ptr,
                                         h_ptr,
                                         0,
                                         4,
                                         value);
}


#ifndef scpb_reg_lfield_GPO_get
#define scpb_reg_lfield_GPO_get( b_ptr, h_ptr, value ) \
       _scpb_reg_lfield_GPO_get( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_lfield_GPO_get( LOG_CALLER_DEFN
                                             scpb_reg_buffer_t *b_ptr,
                                             cpb_handle_t *h_ptr,
                                             UINT32 value[4] )
{
    IOLOG( "%s ", "scpb_reg_lfield_GPO_get");
    _scpb_reg_reg_GPO_array_burst_read( LOG_CALLER_ARGS
                                        b_ptr,
                                        h_ptr,
                                        0,
                                        4,
                                        (UINT32 *)value);
}


#ifndef scpb_reg_lfield_range_GPO_set
#define scpb_reg_lfield_range_GPO_set( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_lfield_range_GPO_set( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_lfield_range_GPO_set( LOG_CALLER_DEFN
                                                   scpb_reg_buffer_t *b_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "scpb_reg_lfield_range_GPO_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "scpb_reg_lfield_range_GPO_set", start_bit, stop_bit, value);
    _scpb_reg_reg_GPO_array_field_set( LOG_CALLER_ARGS
                                       b_ptr,
                                       h_ptr,
                                       word_number,
                                       mask,
                                       field_ofs,
                                       value);
}


#ifndef scpb_reg_lfield_range_GPO_get
#define scpb_reg_lfield_range_GPO_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_lfield_range_GPO_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_lfield_range_GPO_get( LOG_CALLER_DEFN
                                                     scpb_reg_buffer_t *b_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "scpb_reg_lfield_range_GPO_get", start_bit, stop_bit );
    }
    reg_value = _scpb_reg_reg_GPO_array_read( LOG_CALLER_ARGS
                                              b_ptr,
                                              h_ptr,
                                              word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "scpb_reg_lfield_range_GPO_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size150__M_size64
 * ==================================================================================
 */

#ifndef scpb_reg_field_IPT_BLOCK_VALID_set
#define scpb_reg_field_IPT_BLOCK_VALID_set( b_ptr, h_ptr, N, M, value ) \
       _scpb_reg_field_IPT_BLOCK_VALID_set( LOG_CALLER b_ptr, h_ptr, N, M, value )
#endif
static INLINE void _scpb_reg_field_IPT_BLOCK_VALID_set( LOG_CALLER_DEFN
                                                        scpb_reg_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32  M,
                                                        UINT32 value )
{
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_IPT_BLOCK_VALID_set", N, 149);
    if (M > 63)
        IO_RANGE_CHECK("%s M is %d but max is %d", "scpb_reg_field_IPT_BLOCK_VALID_set", M, 63);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_IPT_BLOCK_VALID_set", value, 1);
    IOLOG( "%s <= N=%d M=%d 0x%08x", "scpb_reg_field_IPT_BLOCK_VALID_set", N, M, value );

    /* ((0x00100000 + (N) * 0x100 + (M) * 0x4) bits 31) field IPT_BLOCK_VALID of register PMC_SCPB_REG_REG_BLOCK_TABLE_REG index N=0..149 index M=0..63 */
    _scpb_reg_reg_BLOCK_TABLE_REG_array_field_set( LOG_CALLER_ARGS
                                                   b_ptr,
                                                   h_ptr,
                                                   N,
                                                   M,
                                                   SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_BLOCK_VALID_MSK,
                                                   SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_BLOCK_VALID_OFF,
                                                   value);
}


#ifndef scpb_reg_field_IPT_BLOCK_VALID_get
#define scpb_reg_field_IPT_BLOCK_VALID_get( b_ptr, h_ptr, N, M ) \
       _scpb_reg_field_IPT_BLOCK_VALID_get( LOG_CALLER b_ptr, h_ptr, N, M )
#endif
static INLINE UINT32 _scpb_reg_field_IPT_BLOCK_VALID_get( LOG_CALLER_DEFN
                                                          scpb_reg_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32  M )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_IPT_BLOCK_VALID_get", N, 149);
    if (M > 63)
        IO_RANGE_CHECK("%s M is %d but max is %d", "scpb_reg_field_IPT_BLOCK_VALID_get", M, 63);
    /* ((0x00100000 + (N) * 0x100 + (M) * 0x4) bits 31) field IPT_BLOCK_VALID of register PMC_SCPB_REG_REG_BLOCK_TABLE_REG index N=0..149 index M=0..63 */
    reg_value = _scpb_reg_reg_BLOCK_TABLE_REG_array_read( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr,
                                                          N,
                                                          M);
    value = (reg_value & SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_BLOCK_VALID_MSK) >> SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_BLOCK_VALID_OFF;
    IOLOG( "%s -> N=%d M=%d 0x%08x", "scpb_reg_field_IPT_BLOCK_VALID_get", N, M, value );

    return value;
}

#ifndef scpb_reg_field_IPT_WQID_set
#define scpb_reg_field_IPT_WQID_set( b_ptr, h_ptr, N, M, value ) \
       _scpb_reg_field_IPT_WQID_set( LOG_CALLER b_ptr, h_ptr, N, M, value )
#endif
static INLINE void _scpb_reg_field_IPT_WQID_set( LOG_CALLER_DEFN
                                                 scpb_reg_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32  M,
                                                 UINT32 value )
{
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_IPT_WQID_set", N, 149);
    if (M > 63)
        IO_RANGE_CHECK("%s M is %d but max is %d", "scpb_reg_field_IPT_WQID_set", M, 63);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_IPT_WQID_set", value, 255);
    IOLOG( "%s <= N=%d M=%d 0x%08x", "scpb_reg_field_IPT_WQID_set", N, M, value );

    /* ((0x00100000 + (N) * 0x100 + (M) * 0x4) bits 23:16) field IPT_WQID of register PMC_SCPB_REG_REG_BLOCK_TABLE_REG index N=0..149 index M=0..63 */
    _scpb_reg_reg_BLOCK_TABLE_REG_array_field_set( LOG_CALLER_ARGS
                                                   b_ptr,
                                                   h_ptr,
                                                   N,
                                                   M,
                                                   SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_WQID_MSK,
                                                   SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_WQID_OFF,
                                                   value);
}


#ifndef scpb_reg_field_IPT_WQID_get
#define scpb_reg_field_IPT_WQID_get( b_ptr, h_ptr, N, M ) \
       _scpb_reg_field_IPT_WQID_get( LOG_CALLER b_ptr, h_ptr, N, M )
#endif
static INLINE UINT32 _scpb_reg_field_IPT_WQID_get( LOG_CALLER_DEFN
                                                   scpb_reg_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32  M )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_IPT_WQID_get", N, 149);
    if (M > 63)
        IO_RANGE_CHECK("%s M is %d but max is %d", "scpb_reg_field_IPT_WQID_get", M, 63);
    /* ((0x00100000 + (N) * 0x100 + (M) * 0x4) bits 23:16) field IPT_WQID of register PMC_SCPB_REG_REG_BLOCK_TABLE_REG index N=0..149 index M=0..63 */
    reg_value = _scpb_reg_reg_BLOCK_TABLE_REG_array_read( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr,
                                                          N,
                                                          M);
    value = (reg_value & SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_WQID_MSK) >> SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_WQID_OFF;
    IOLOG( "%s -> N=%d M=%d 0x%08x", "scpb_reg_field_IPT_WQID_get", N, M, value );

    return value;
}

#ifndef scpb_reg_field_range_IPT_WQID_set
#define scpb_reg_field_range_IPT_WQID_set( b_ptr, h_ptr, N, M, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_IPT_WQID_set( LOG_CALLER b_ptr, h_ptr, N, M, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_IPT_WQID_set( LOG_CALLER_DEFN
                                                       scpb_reg_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32  M,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_IPT_WQID_set", N, 149);
    if (M > 63)
        IO_RANGE_CHECK("%s M is %d but max is %d", "scpb_reg_field_range_IPT_WQID_set", M, 63);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_IPT_WQID_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_IPT_WQID_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d M=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_IPT_WQID_set", N, M, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00100000 + (N) * 0x100 + (M) * 0x4) bits 23:16) field IPT_WQID of register PMC_SCPB_REG_REG_BLOCK_TABLE_REG index N=0..149 index M=0..63 */
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
        /* ((0x00100000 + (N) * 0x100 + (M) * 0x4) bits 23:16) field IPT_WQID of register PMC_SCPB_REG_REG_BLOCK_TABLE_REG index N=0..149 index M=0..63 */
        _scpb_reg_reg_BLOCK_TABLE_REG_array_field_set( LOG_CALLER_ARGS
                                                       b_ptr,
                                                       h_ptr,
                                                       N,
                                                       M,
                                                       subfield_mask << (SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_WQID_OFF + subfield_offset),
                                                       SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_WQID_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_IPT_WQID_get
#define scpb_reg_field_range_IPT_WQID_get( b_ptr, h_ptr, N, M, start_bit, stop_bit ) \
       _scpb_reg_field_range_IPT_WQID_get( LOG_CALLER b_ptr, h_ptr, N, M, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_IPT_WQID_get( LOG_CALLER_DEFN
                                                         scpb_reg_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32  M,
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

    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_IPT_WQID_get", N, 149);
    if (M > 63)
        IO_RANGE_CHECK("%s M is %d but max is %d", "scpb_reg_field_range_IPT_WQID_get", M, 63);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_IPT_WQID_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_IPT_WQID_get", stop_bit, 7 );
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
    /* ((0x00100000 + (N) * 0x100 + (M) * 0x4) bits 23:16) field IPT_WQID of register PMC_SCPB_REG_REG_BLOCK_TABLE_REG index N=0..149 index M=0..63 */
    reg_value = _scpb_reg_reg_BLOCK_TABLE_REG_array_read( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr,
                                                          N,
                                                          M);
    field_value = (reg_value & SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_WQID_MSK)
                  >> SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_WQID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_WQID_MSK, SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_WQID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d M=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_IPT_WQID_get", N, M, start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_IPT_LBA_set
#define scpb_reg_field_IPT_LBA_set( b_ptr, h_ptr, N, M, value ) \
       _scpb_reg_field_IPT_LBA_set( LOG_CALLER b_ptr, h_ptr, N, M, value )
#endif
static INLINE void _scpb_reg_field_IPT_LBA_set( LOG_CALLER_DEFN
                                                scpb_reg_buffer_t *b_ptr,
                                                cpb_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32  M,
                                                UINT32 value )
{
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_IPT_LBA_set", N, 149);
    if (M > 63)
        IO_RANGE_CHECK("%s M is %d but max is %d", "scpb_reg_field_IPT_LBA_set", M, 63);
    if (value > 2047)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_IPT_LBA_set", value, 2047);
    IOLOG( "%s <= N=%d M=%d 0x%08x", "scpb_reg_field_IPT_LBA_set", N, M, value );

    /* ((0x00100000 + (N) * 0x100 + (M) * 0x4) bits 0:10) field IPT_LBA of register PMC_SCPB_REG_REG_BLOCK_TABLE_REG index N=0..149 index M=0..63 */
    _scpb_reg_reg_BLOCK_TABLE_REG_array_field_set( LOG_CALLER_ARGS
                                                   b_ptr,
                                                   h_ptr,
                                                   N,
                                                   M,
                                                   SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_LBA_MSK,
                                                   SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_LBA_OFF,
                                                   value);
}


#ifndef scpb_reg_field_IPT_LBA_get
#define scpb_reg_field_IPT_LBA_get( b_ptr, h_ptr, N, M ) \
       _scpb_reg_field_IPT_LBA_get( LOG_CALLER b_ptr, h_ptr, N, M )
#endif
static INLINE UINT32 _scpb_reg_field_IPT_LBA_get( LOG_CALLER_DEFN
                                                  scpb_reg_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32  M )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_IPT_LBA_get", N, 149);
    if (M > 63)
        IO_RANGE_CHECK("%s M is %d but max is %d", "scpb_reg_field_IPT_LBA_get", M, 63);
    /* ((0x00100000 + (N) * 0x100 + (M) * 0x4) bits 0:10) field IPT_LBA of register PMC_SCPB_REG_REG_BLOCK_TABLE_REG index N=0..149 index M=0..63 */
    reg_value = _scpb_reg_reg_BLOCK_TABLE_REG_array_read( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr,
                                                          N,
                                                          M);
    value = (reg_value & SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_LBA_MSK) >> SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_LBA_OFF;
    IOLOG( "%s -> N=%d M=%d 0x%08x", "scpb_reg_field_IPT_LBA_get", N, M, value );

    return value;
}

#ifndef scpb_reg_field_range_IPT_LBA_set
#define scpb_reg_field_range_IPT_LBA_set( b_ptr, h_ptr, N, M, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_IPT_LBA_set( LOG_CALLER b_ptr, h_ptr, N, M, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_IPT_LBA_set( LOG_CALLER_DEFN
                                                      scpb_reg_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32  M,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_IPT_LBA_set", N, 149);
    if (M > 63)
        IO_RANGE_CHECK("%s M is %d but max is %d", "scpb_reg_field_range_IPT_LBA_set", M, 63);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_IPT_LBA_set", stop_bit, start_bit );
    if (stop_bit > 10) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_IPT_LBA_set", stop_bit, 10 );
    IOLOG( "%s <= N=%d M=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_IPT_LBA_set", N, M, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00100000 + (N) * 0x100 + (M) * 0x4) bits 0:10) field IPT_LBA of register PMC_SCPB_REG_REG_BLOCK_TABLE_REG index N=0..149 index M=0..63 */
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
        /* ((0x00100000 + (N) * 0x100 + (M) * 0x4) bits 0:10) field IPT_LBA of register PMC_SCPB_REG_REG_BLOCK_TABLE_REG index N=0..149 index M=0..63 */
        _scpb_reg_reg_BLOCK_TABLE_REG_array_field_set( LOG_CALLER_ARGS
                                                       b_ptr,
                                                       h_ptr,
                                                       N,
                                                       M,
                                                       subfield_mask << (SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_LBA_OFF + subfield_offset),
                                                       SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_LBA_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_IPT_LBA_get
#define scpb_reg_field_range_IPT_LBA_get( b_ptr, h_ptr, N, M, start_bit, stop_bit ) \
       _scpb_reg_field_range_IPT_LBA_get( LOG_CALLER b_ptr, h_ptr, N, M, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_IPT_LBA_get( LOG_CALLER_DEFN
                                                        scpb_reg_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32  M,
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

    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_IPT_LBA_get", N, 149);
    if (M > 63)
        IO_RANGE_CHECK("%s M is %d but max is %d", "scpb_reg_field_range_IPT_LBA_get", M, 63);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_IPT_LBA_get", stop_bit, start_bit );
    if (stop_bit > 10) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_IPT_LBA_get", stop_bit, 10 );
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
    /* ((0x00100000 + (N) * 0x100 + (M) * 0x4) bits 0:10) field IPT_LBA of register PMC_SCPB_REG_REG_BLOCK_TABLE_REG index N=0..149 index M=0..63 */
    reg_value = _scpb_reg_reg_BLOCK_TABLE_REG_array_read( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr,
                                                          N,
                                                          M);
    field_value = (reg_value & SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_LBA_MSK)
                  >> SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_LBA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_LBA_MSK, SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_LBA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d M=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_IPT_LBA_get", N, M, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size32
 * ==================================================================================
 */

#ifndef scpb_reg_field_PAGE_VALID_set
#define scpb_reg_field_PAGE_VALID_set( b_ptr, h_ptr, N, value ) \
       _scpb_reg_field_PAGE_VALID_set( LOG_CALLER b_ptr, h_ptr, N, value )
#endif
static INLINE void _scpb_reg_field_PAGE_VALID_set( LOG_CALLER_DEFN
                                                   scpb_reg_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 31)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_PAGE_VALID_set", N, 31);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_PAGE_VALID_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "scpb_reg_field_PAGE_VALID_set", N, value );

    /* ((0x00104000 + (N) * 0x4) bits 24) field PAGE_VALID of register PMC_SCPB_REG_REG_PAGE_TABLE_REG index N=0..31 */
    _scpb_reg_reg_PAGE_TABLE_REG_array_field_set( LOG_CALLER_ARGS
                                                  b_ptr,
                                                  h_ptr,
                                                  N,
                                                  SCPB_REG_REG_PAGE_TABLE_REG_BIT_PAGE_VALID_MSK,
                                                  SCPB_REG_REG_PAGE_TABLE_REG_BIT_PAGE_VALID_OFF,
                                                  value);
}


#ifndef scpb_reg_field_PAGE_VALID_get
#define scpb_reg_field_PAGE_VALID_get( b_ptr, h_ptr, N ) \
       _scpb_reg_field_PAGE_VALID_get( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_field_PAGE_VALID_get( LOG_CALLER_DEFN
                                                     scpb_reg_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 31)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_PAGE_VALID_get", N, 31);
    /* ((0x00104000 + (N) * 0x4) bits 24) field PAGE_VALID of register PMC_SCPB_REG_REG_PAGE_TABLE_REG index N=0..31 */
    reg_value = _scpb_reg_reg_PAGE_TABLE_REG_array_read( LOG_CALLER_ARGS
                                                         b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & SCPB_REG_REG_PAGE_TABLE_REG_BIT_PAGE_VALID_MSK) >> SCPB_REG_REG_PAGE_TABLE_REG_BIT_PAGE_VALID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "scpb_reg_field_PAGE_VALID_get", N, value );

    return value;
}

#ifndef scpb_reg_field_IPT_QS_ID_set
#define scpb_reg_field_IPT_QS_ID_set( b_ptr, h_ptr, N, value ) \
       _scpb_reg_field_IPT_QS_ID_set( LOG_CALLER b_ptr, h_ptr, N, value )
#endif
static INLINE void _scpb_reg_field_IPT_QS_ID_set( LOG_CALLER_DEFN
                                                  scpb_reg_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 31)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_IPT_QS_ID_set", N, 31);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_IPT_QS_ID_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "scpb_reg_field_IPT_QS_ID_set", N, value );

    /* ((0x00104000 + (N) * 0x4) bits 17:16) field IPT_QS_ID of register PMC_SCPB_REG_REG_PAGE_TABLE_REG index N=0..31 */
    _scpb_reg_reg_PAGE_TABLE_REG_array_field_set( LOG_CALLER_ARGS
                                                  b_ptr,
                                                  h_ptr,
                                                  N,
                                                  SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_QS_ID_MSK,
                                                  SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_QS_ID_OFF,
                                                  value);
}


#ifndef scpb_reg_field_IPT_QS_ID_get
#define scpb_reg_field_IPT_QS_ID_get( b_ptr, h_ptr, N ) \
       _scpb_reg_field_IPT_QS_ID_get( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_field_IPT_QS_ID_get( LOG_CALLER_DEFN
                                                    scpb_reg_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 31)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_IPT_QS_ID_get", N, 31);
    /* ((0x00104000 + (N) * 0x4) bits 17:16) field IPT_QS_ID of register PMC_SCPB_REG_REG_PAGE_TABLE_REG index N=0..31 */
    reg_value = _scpb_reg_reg_PAGE_TABLE_REG_array_read( LOG_CALLER_ARGS
                                                         b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_QS_ID_MSK) >> SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_QS_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "scpb_reg_field_IPT_QS_ID_get", N, value );

    return value;
}

#ifndef scpb_reg_field_range_IPT_QS_ID_set
#define scpb_reg_field_range_IPT_QS_ID_set( b_ptr, h_ptr, N, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_IPT_QS_ID_set( LOG_CALLER b_ptr, h_ptr, N, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_IPT_QS_ID_set( LOG_CALLER_DEFN
                                                        scpb_reg_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (N > 31)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_IPT_QS_ID_set", N, 31);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_IPT_QS_ID_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_IPT_QS_ID_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_IPT_QS_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00104000 + (N) * 0x4) bits 17:16) field IPT_QS_ID of register PMC_SCPB_REG_REG_PAGE_TABLE_REG index N=0..31 */
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
        /* ((0x00104000 + (N) * 0x4) bits 17:16) field IPT_QS_ID of register PMC_SCPB_REG_REG_PAGE_TABLE_REG index N=0..31 */
        _scpb_reg_reg_PAGE_TABLE_REG_array_field_set( LOG_CALLER_ARGS
                                                      b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_QS_ID_OFF + subfield_offset),
                                                      SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_QS_ID_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_IPT_QS_ID_get
#define scpb_reg_field_range_IPT_QS_ID_get( b_ptr, h_ptr, N, start_bit, stop_bit ) \
       _scpb_reg_field_range_IPT_QS_ID_get( LOG_CALLER b_ptr, h_ptr, N, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_IPT_QS_ID_get( LOG_CALLER_DEFN
                                                          scpb_reg_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
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

    if (N > 31)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_IPT_QS_ID_get", N, 31);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_IPT_QS_ID_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_IPT_QS_ID_get", stop_bit, 1 );
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
    /* ((0x00104000 + (N) * 0x4) bits 17:16) field IPT_QS_ID of register PMC_SCPB_REG_REG_PAGE_TABLE_REG index N=0..31 */
    reg_value = _scpb_reg_reg_PAGE_TABLE_REG_array_read( LOG_CALLER_ARGS
                                                         b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_QS_ID_MSK)
                  >> SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_QS_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_QS_ID_MSK, SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_QS_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_IPT_QS_ID_get", N, start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_IPT_WP_ID_set
#define scpb_reg_field_IPT_WP_ID_set( b_ptr, h_ptr, N, value ) \
       _scpb_reg_field_IPT_WP_ID_set( LOG_CALLER b_ptr, h_ptr, N, value )
#endif
static INLINE void _scpb_reg_field_IPT_WP_ID_set( LOG_CALLER_DEFN
                                                  scpb_reg_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 31)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_IPT_WP_ID_set", N, 31);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_IPT_WP_ID_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "scpb_reg_field_IPT_WP_ID_set", N, value );

    /* ((0x00104000 + (N) * 0x4) bits 9:8) field IPT_WP_ID of register PMC_SCPB_REG_REG_PAGE_TABLE_REG index N=0..31 */
    _scpb_reg_reg_PAGE_TABLE_REG_array_field_set( LOG_CALLER_ARGS
                                                  b_ptr,
                                                  h_ptr,
                                                  N,
                                                  SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_WP_ID_MSK,
                                                  SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_WP_ID_OFF,
                                                  value);
}


#ifndef scpb_reg_field_IPT_WP_ID_get
#define scpb_reg_field_IPT_WP_ID_get( b_ptr, h_ptr, N ) \
       _scpb_reg_field_IPT_WP_ID_get( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_field_IPT_WP_ID_get( LOG_CALLER_DEFN
                                                    scpb_reg_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 31)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_IPT_WP_ID_get", N, 31);
    /* ((0x00104000 + (N) * 0x4) bits 9:8) field IPT_WP_ID of register PMC_SCPB_REG_REG_PAGE_TABLE_REG index N=0..31 */
    reg_value = _scpb_reg_reg_PAGE_TABLE_REG_array_read( LOG_CALLER_ARGS
                                                         b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_WP_ID_MSK) >> SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_WP_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "scpb_reg_field_IPT_WP_ID_get", N, value );

    return value;
}

#ifndef scpb_reg_field_range_IPT_WP_ID_set
#define scpb_reg_field_range_IPT_WP_ID_set( b_ptr, h_ptr, N, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_IPT_WP_ID_set( LOG_CALLER b_ptr, h_ptr, N, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_IPT_WP_ID_set( LOG_CALLER_DEFN
                                                        scpb_reg_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (N > 31)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_IPT_WP_ID_set", N, 31);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_IPT_WP_ID_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_IPT_WP_ID_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_IPT_WP_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00104000 + (N) * 0x4) bits 9:8) field IPT_WP_ID of register PMC_SCPB_REG_REG_PAGE_TABLE_REG index N=0..31 */
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
        /* ((0x00104000 + (N) * 0x4) bits 9:8) field IPT_WP_ID of register PMC_SCPB_REG_REG_PAGE_TABLE_REG index N=0..31 */
        _scpb_reg_reg_PAGE_TABLE_REG_array_field_set( LOG_CALLER_ARGS
                                                      b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_WP_ID_OFF + subfield_offset),
                                                      SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_WP_ID_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_IPT_WP_ID_get
#define scpb_reg_field_range_IPT_WP_ID_get( b_ptr, h_ptr, N, start_bit, stop_bit ) \
       _scpb_reg_field_range_IPT_WP_ID_get( LOG_CALLER b_ptr, h_ptr, N, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_IPT_WP_ID_get( LOG_CALLER_DEFN
                                                          scpb_reg_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
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

    if (N > 31)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_IPT_WP_ID_get", N, 31);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_IPT_WP_ID_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_IPT_WP_ID_get", stop_bit, 1 );
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
    /* ((0x00104000 + (N) * 0x4) bits 9:8) field IPT_WP_ID of register PMC_SCPB_REG_REG_PAGE_TABLE_REG index N=0..31 */
    reg_value = _scpb_reg_reg_PAGE_TABLE_REG_array_read( LOG_CALLER_ARGS
                                                         b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_WP_ID_MSK)
                  >> SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_WP_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_WP_ID_MSK, SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_WP_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_IPT_WP_ID_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */

#ifndef scpb_reg_field_REMAP_ABORT_set
#define scpb_reg_field_REMAP_ABORT_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_REMAP_ABORT_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_REMAP_ABORT_set( LOG_CALLER_DEFN
                                                    scpb_reg_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_REMAP_ABORT_set", value, 1);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_REMAP_ABORT_set", value );

    /* (0x00104200 bits 28) field REMAP_ABORT of register PMC_SCPB_REG_REG_REMAP_REQ */
    _scpb_reg_reg_REMAP_REQ_field_set( LOG_CALLER_ARGS
                                       b_ptr,
                                       h_ptr,
                                       SCPB_REG_REG_REMAP_REQ_BIT_REMAP_ABORT_MSK,
                                       SCPB_REG_REG_REMAP_REQ_BIT_REMAP_ABORT_OFF,
                                       value);
}


#ifndef scpb_reg_field_REMAP_ABORT_get
#define scpb_reg_field_REMAP_ABORT_get( b_ptr, h_ptr ) \
       _scpb_reg_field_REMAP_ABORT_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_REMAP_ABORT_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00104200 bits 28) field REMAP_ABORT of register PMC_SCPB_REG_REG_REMAP_REQ */
    reg_value = _scpb_reg_reg_REMAP_REQ_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    value = (reg_value & SCPB_REG_REG_REMAP_REQ_BIT_REMAP_ABORT_MSK) >> SCPB_REG_REG_REMAP_REQ_BIT_REMAP_ABORT_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_REMAP_ABORT_get", value );

    return value;
}

#ifndef scpb_reg_field_REMAP_ABORT_poll
#define scpb_reg_field_REMAP_ABORT_poll( b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_field_REMAP_ABORT_poll( LOG_CALLER b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_field_REMAP_ABORT_poll( LOG_CALLER_DEFN
                                                                     scpb_reg_buffer_t *b_ptr,
                                                                     cpb_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "scpb_reg_field_REMAP_ABORT_poll", value );

    /* (0x00104200 bits 28) field REMAP_ABORT of register PMC_SCPB_REG_REG_REMAP_REQ */
    return _scpb_reg_reg_REMAP_REQ_poll( LOG_CALLER_ARGS
                                         b_ptr,
                                         h_ptr,
                                         SCPB_REG_REG_REMAP_REQ_BIT_REMAP_ABORT_MSK,
                                         (value<<SCPB_REG_REG_REMAP_REQ_BIT_REMAP_ABORT_OFF),
                                         cmp,
                                         max_count,
                                         num_failed_polls,
                                         delay_between_polls_in_microseconds);
}


#ifndef scpb_reg_field_REMAP_REQ_set
#define scpb_reg_field_REMAP_REQ_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_REMAP_REQ_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_REMAP_REQ_set( LOG_CALLER_DEFN
                                                  scpb_reg_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_REMAP_REQ_set", value, 1);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_REMAP_REQ_set", value );

    /* (0x00104200 bits 24) field REMAP_REQ of register PMC_SCPB_REG_REG_REMAP_REQ */
    _scpb_reg_reg_REMAP_REQ_field_set( LOG_CALLER_ARGS
                                       b_ptr,
                                       h_ptr,
                                       SCPB_REG_REG_REMAP_REQ_BIT_REMAP_REQ_MSK,
                                       SCPB_REG_REG_REMAP_REQ_BIT_REMAP_REQ_OFF,
                                       value);
}


#ifndef scpb_reg_field_REMAP_REQ_get
#define scpb_reg_field_REMAP_REQ_get( b_ptr, h_ptr ) \
       _scpb_reg_field_REMAP_REQ_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_REMAP_REQ_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00104200 bits 24) field REMAP_REQ of register PMC_SCPB_REG_REG_REMAP_REQ */
    reg_value = _scpb_reg_reg_REMAP_REQ_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    value = (reg_value & SCPB_REG_REG_REMAP_REQ_BIT_REMAP_REQ_MSK) >> SCPB_REG_REG_REMAP_REQ_BIT_REMAP_REQ_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_REMAP_REQ_get", value );

    return value;
}

#ifndef scpb_reg_field_REMAP_REQ_poll
#define scpb_reg_field_REMAP_REQ_poll( b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_field_REMAP_REQ_poll( LOG_CALLER b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_field_REMAP_REQ_poll( LOG_CALLER_DEFN
                                                                   scpb_reg_buffer_t *b_ptr,
                                                                   cpb_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "scpb_reg_field_REMAP_REQ_poll", value );

    /* (0x00104200 bits 24) field REMAP_REQ of register PMC_SCPB_REG_REG_REMAP_REQ */
    return _scpb_reg_reg_REMAP_REQ_poll( LOG_CALLER_ARGS
                                         b_ptr,
                                         h_ptr,
                                         SCPB_REG_REG_REMAP_REQ_BIT_REMAP_REQ_MSK,
                                         (value<<SCPB_REG_REG_REMAP_REQ_BIT_REMAP_REQ_OFF),
                                         cmp,
                                         max_count,
                                         num_failed_polls,
                                         delay_between_polls_in_microseconds);
}


#ifndef scpb_reg_field_FIFO_END_ADDR_CHG_ABORT_set
#define scpb_reg_field_FIFO_END_ADDR_CHG_ABORT_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_FIFO_END_ADDR_CHG_ABORT_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_FIFO_END_ADDR_CHG_ABORT_set( LOG_CALLER_DEFN
                                                                scpb_reg_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_FIFO_END_ADDR_CHG_ABORT_set", value, 1);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_FIFO_END_ADDR_CHG_ABORT_set", value );

    /* (0x00103d00 bits 28) field FIFO_END_ADDR_CHG_ABORT of register PMC_SCPB_REG_REG_FIFO_CFG */
    _scpb_reg_reg_FIFO_CFG_field_set( LOG_CALLER_ARGS
                                      b_ptr,
                                      h_ptr,
                                      SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_CHG_ABORT_MSK,
                                      SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_CHG_ABORT_OFF,
                                      value);
}


#ifndef scpb_reg_field_FIFO_END_ADDR_CHG_ABORT_get
#define scpb_reg_field_FIFO_END_ADDR_CHG_ABORT_get( b_ptr, h_ptr ) \
       _scpb_reg_field_FIFO_END_ADDR_CHG_ABORT_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_FIFO_END_ADDR_CHG_ABORT_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00103d00 bits 28) field FIFO_END_ADDR_CHG_ABORT of register PMC_SCPB_REG_REG_FIFO_CFG */
    reg_value = _scpb_reg_reg_FIFO_CFG_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    value = (reg_value & SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_CHG_ABORT_MSK) >> SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_CHG_ABORT_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_FIFO_END_ADDR_CHG_ABORT_get", value );

    return value;
}

#ifndef scpb_reg_field_FIFO_END_ADDR_CHG_ABORT_poll
#define scpb_reg_field_FIFO_END_ADDR_CHG_ABORT_poll( b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_field_FIFO_END_ADDR_CHG_ABORT_poll( LOG_CALLER b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_field_FIFO_END_ADDR_CHG_ABORT_poll( LOG_CALLER_DEFN
                                                                                 scpb_reg_buffer_t *b_ptr,
                                                                                 cpb_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "scpb_reg_field_FIFO_END_ADDR_CHG_ABORT_poll", value );

    /* (0x00103d00 bits 28) field FIFO_END_ADDR_CHG_ABORT of register PMC_SCPB_REG_REG_FIFO_CFG */
    return _scpb_reg_reg_FIFO_CFG_poll( LOG_CALLER_ARGS
                                        b_ptr,
                                        h_ptr,
                                        SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_CHG_ABORT_MSK,
                                        (value<<SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_CHG_ABORT_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}


#ifndef scpb_reg_field_FIFO_END_ADDR_CHG_REQ_set
#define scpb_reg_field_FIFO_END_ADDR_CHG_REQ_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_FIFO_END_ADDR_CHG_REQ_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_FIFO_END_ADDR_CHG_REQ_set( LOG_CALLER_DEFN
                                                              scpb_reg_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_FIFO_END_ADDR_CHG_REQ_set", value, 1);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_FIFO_END_ADDR_CHG_REQ_set", value );

    /* (0x00103d00 bits 24) field FIFO_END_ADDR_CHG_REQ of register PMC_SCPB_REG_REG_FIFO_CFG */
    _scpb_reg_reg_FIFO_CFG_field_set( LOG_CALLER_ARGS
                                      b_ptr,
                                      h_ptr,
                                      SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_CHG_REQ_MSK,
                                      SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_CHG_REQ_OFF,
                                      value);
}


#ifndef scpb_reg_field_FIFO_END_ADDR_CHG_REQ_get
#define scpb_reg_field_FIFO_END_ADDR_CHG_REQ_get( b_ptr, h_ptr ) \
       _scpb_reg_field_FIFO_END_ADDR_CHG_REQ_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_FIFO_END_ADDR_CHG_REQ_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00103d00 bits 24) field FIFO_END_ADDR_CHG_REQ of register PMC_SCPB_REG_REG_FIFO_CFG */
    reg_value = _scpb_reg_reg_FIFO_CFG_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    value = (reg_value & SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_CHG_REQ_MSK) >> SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_CHG_REQ_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_FIFO_END_ADDR_CHG_REQ_get", value );

    return value;
}

#ifndef scpb_reg_field_FIFO_END_ADDR_CHG_REQ_poll
#define scpb_reg_field_FIFO_END_ADDR_CHG_REQ_poll( b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_field_FIFO_END_ADDR_CHG_REQ_poll( LOG_CALLER b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_field_FIFO_END_ADDR_CHG_REQ_poll( LOG_CALLER_DEFN
                                                                               scpb_reg_buffer_t *b_ptr,
                                                                               cpb_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "scpb_reg_field_FIFO_END_ADDR_CHG_REQ_poll", value );

    /* (0x00103d00 bits 24) field FIFO_END_ADDR_CHG_REQ of register PMC_SCPB_REG_REG_FIFO_CFG */
    return _scpb_reg_reg_FIFO_CFG_poll( LOG_CALLER_ARGS
                                        b_ptr,
                                        h_ptr,
                                        SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_CHG_REQ_MSK,
                                        (value<<SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_CHG_REQ_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}


#ifndef scpb_reg_field_DBG_TRIG_set
#define scpb_reg_field_DBG_TRIG_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_DBG_TRIG_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_DBG_TRIG_set( LOG_CALLER_DEFN
                                                 scpb_reg_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_DBG_TRIG_set", value, 1);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_DBG_TRIG_set", value );

    /* (0x00103e00 bits 16) field DBG_TRIG of register PMC_SCPB_REG_REG_DBG_CTRL */
    _scpb_reg_reg_DBG_CTRL_field_set( LOG_CALLER_ARGS
                                      b_ptr,
                                      h_ptr,
                                      SCPB_REG_REG_DBG_CTRL_BIT_DBG_TRIG_MSK,
                                      SCPB_REG_REG_DBG_CTRL_BIT_DBG_TRIG_OFF,
                                      value);
}


#ifndef scpb_reg_field_DBG_TRIG_get
#define scpb_reg_field_DBG_TRIG_get( b_ptr, h_ptr ) \
       _scpb_reg_field_DBG_TRIG_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_DBG_TRIG_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00103e00 bits 16) field DBG_TRIG of register PMC_SCPB_REG_REG_DBG_CTRL */
    reg_value = _scpb_reg_reg_DBG_CTRL_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    value = (reg_value & SCPB_REG_REG_DBG_CTRL_BIT_DBG_TRIG_MSK) >> SCPB_REG_REG_DBG_CTRL_BIT_DBG_TRIG_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_DBG_TRIG_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_indirect_N_size108
 * ==================================================================================
 */

#ifndef scpb_reg_field_WPTR_RST_PUSH_P3_set
#define scpb_reg_field_WPTR_RST_PUSH_P3_set( b_ptr, h_ptr, N, value ) \
       _scpb_reg_field_WPTR_RST_PUSH_P3_set( LOG_CALLER b_ptr, h_ptr, N, value )
#endif
static INLINE void _scpb_reg_field_WPTR_RST_PUSH_P3_set( LOG_CALLER_DEFN
                                                         scpb_reg_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 107)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_WPTR_RST_PUSH_P3_set", N, 107);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_WPTR_RST_PUSH_P3_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "scpb_reg_field_WPTR_RST_PUSH_P3_set", N, value );

    /* ((0x00101800 + (N) * 0x4) bits 0) field WPTR_RST_PUSH_P3 of register PMC_SCPB_REG_REG_PUSH_PORT_P3_RST index N=0..107 */
    _scpb_reg_reg_PUSH_PORT_P3_RST_array_field_set( LOG_CALLER_ARGS
                                                    b_ptr,
                                                    h_ptr,
                                                    N,
                                                    SCPB_REG_REG_PUSH_PORT_P3_RST_BIT_WPTR_RST_PUSH_P3_MSK,
                                                    SCPB_REG_REG_PUSH_PORT_P3_RST_BIT_WPTR_RST_PUSH_P3_OFF,
                                                    value);
}


#ifndef scpb_reg_field_WPTR_RST_PUSH_P3_get
#define scpb_reg_field_WPTR_RST_PUSH_P3_get( b_ptr, h_ptr, N ) \
       _scpb_reg_field_WPTR_RST_PUSH_P3_get( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_field_WPTR_RST_PUSH_P3_get( LOG_CALLER_DEFN
                                                           scpb_reg_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 107)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_WPTR_RST_PUSH_P3_get", N, 107);
    /* ((0x00101800 + (N) * 0x4) bits 0) field WPTR_RST_PUSH_P3 of register PMC_SCPB_REG_REG_PUSH_PORT_P3_RST index N=0..107 */
    reg_value = _scpb_reg_reg_PUSH_PORT_P3_RST_array_read( LOG_CALLER_ARGS
                                                           b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & SCPB_REG_REG_PUSH_PORT_P3_RST_BIT_WPTR_RST_PUSH_P3_MSK) >> SCPB_REG_REG_PUSH_PORT_P3_RST_BIT_WPTR_RST_PUSH_P3_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "scpb_reg_field_WPTR_RST_PUSH_P3_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_indirect_N_size120
 * ==================================================================================
 */

#ifndef scpb_reg_field_WPTR_RST_PUSH_P2_set
#define scpb_reg_field_WPTR_RST_PUSH_P2_set( b_ptr, h_ptr, N, value ) \
       _scpb_reg_field_WPTR_RST_PUSH_P2_set( LOG_CALLER b_ptr, h_ptr, N, value )
#endif
static INLINE void _scpb_reg_field_WPTR_RST_PUSH_P2_set( LOG_CALLER_DEFN
                                                         scpb_reg_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 119)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_WPTR_RST_PUSH_P2_set", N, 119);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_WPTR_RST_PUSH_P2_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "scpb_reg_field_WPTR_RST_PUSH_P2_set", N, value );

    /* ((0x00101000 + (N) * 0x4) bits 0) field WPTR_RST_PUSH_P2 of register PMC_SCPB_REG_REG_PUSH_PORT_P2_RST index N=0..119 */
    _scpb_reg_reg_PUSH_PORT_P2_RST_array_field_set( LOG_CALLER_ARGS
                                                    b_ptr,
                                                    h_ptr,
                                                    N,
                                                    SCPB_REG_REG_PUSH_PORT_P2_RST_BIT_WPTR_RST_PUSH_P2_MSK,
                                                    SCPB_REG_REG_PUSH_PORT_P2_RST_BIT_WPTR_RST_PUSH_P2_OFF,
                                                    value);
}


#ifndef scpb_reg_field_WPTR_RST_PUSH_P2_get
#define scpb_reg_field_WPTR_RST_PUSH_P2_get( b_ptr, h_ptr, N ) \
       _scpb_reg_field_WPTR_RST_PUSH_P2_get( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_field_WPTR_RST_PUSH_P2_get( LOG_CALLER_DEFN
                                                           scpb_reg_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 119)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_WPTR_RST_PUSH_P2_get", N, 119);
    /* ((0x00101000 + (N) * 0x4) bits 0) field WPTR_RST_PUSH_P2 of register PMC_SCPB_REG_REG_PUSH_PORT_P2_RST index N=0..119 */
    reg_value = _scpb_reg_reg_PUSH_PORT_P2_RST_array_read( LOG_CALLER_ARGS
                                                           b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & SCPB_REG_REG_PUSH_PORT_P2_RST_BIT_WPTR_RST_PUSH_P2_MSK) >> SCPB_REG_REG_PUSH_PORT_P2_RST_BIT_WPTR_RST_PUSH_P2_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "scpb_reg_field_WPTR_RST_PUSH_P2_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_indirect_N_size150
 * ==================================================================================
 */

#ifndef scpb_reg_field_WPTR_RST_PUSH_P1_set
#define scpb_reg_field_WPTR_RST_PUSH_P1_set( b_ptr, h_ptr, N, value ) \
       _scpb_reg_field_WPTR_RST_PUSH_P1_set( LOG_CALLER b_ptr, h_ptr, N, value )
#endif
static INLINE void _scpb_reg_field_WPTR_RST_PUSH_P1_set( LOG_CALLER_DEFN
                                                         scpb_reg_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_WPTR_RST_PUSH_P1_set", N, 149);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_WPTR_RST_PUSH_P1_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "scpb_reg_field_WPTR_RST_PUSH_P1_set", N, value );

    /* ((0x00100800 + (N) * 0x4) bits 0) field WPTR_RST_PUSH_P1 of register PMC_SCPB_REG_REG_PUSH_PORT_P1_RST index N=0..149 */
    _scpb_reg_reg_PUSH_PORT_P1_RST_array_field_set( LOG_CALLER_ARGS
                                                    b_ptr,
                                                    h_ptr,
                                                    N,
                                                    SCPB_REG_REG_PUSH_PORT_P1_RST_BIT_WPTR_RST_PUSH_P1_MSK,
                                                    SCPB_REG_REG_PUSH_PORT_P1_RST_BIT_WPTR_RST_PUSH_P1_OFF,
                                                    value);
}


#ifndef scpb_reg_field_WPTR_RST_PUSH_P1_get
#define scpb_reg_field_WPTR_RST_PUSH_P1_get( b_ptr, h_ptr, N ) \
       _scpb_reg_field_WPTR_RST_PUSH_P1_get( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_field_WPTR_RST_PUSH_P1_get( LOG_CALLER_DEFN
                                                           scpb_reg_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_WPTR_RST_PUSH_P1_get", N, 149);
    /* ((0x00100800 + (N) * 0x4) bits 0) field WPTR_RST_PUSH_P1 of register PMC_SCPB_REG_REG_PUSH_PORT_P1_RST index N=0..149 */
    reg_value = _scpb_reg_reg_PUSH_PORT_P1_RST_array_read( LOG_CALLER_ARGS
                                                           b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & SCPB_REG_REG_PUSH_PORT_P1_RST_BIT_WPTR_RST_PUSH_P1_MSK) >> SCPB_REG_REG_PUSH_PORT_P1_RST_BIT_WPTR_RST_PUSH_P1_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "scpb_reg_field_WPTR_RST_PUSH_P1_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_indirect_N_size150__M_size64
 * ==================================================================================
 */

#ifndef scpb_reg_field_WPTR_RST_PUSH_P0_set
#define scpb_reg_field_WPTR_RST_PUSH_P0_set( b_ptr, h_ptr, N, M, value ) \
       _scpb_reg_field_WPTR_RST_PUSH_P0_set( LOG_CALLER b_ptr, h_ptr, N, M, value )
#endif
static INLINE void _scpb_reg_field_WPTR_RST_PUSH_P0_set( LOG_CALLER_DEFN
                                                         scpb_reg_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32  M,
                                                         UINT32 value )
{
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_WPTR_RST_PUSH_P0_set", N, 149);
    if (M > 63)
        IO_RANGE_CHECK("%s M is %d but max is %d", "scpb_reg_field_WPTR_RST_PUSH_P0_set", M, 63);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_WPTR_RST_PUSH_P0_set", value, 1);
    IOLOG( "%s <= N=%d M=%d 0x%08x", "scpb_reg_field_WPTR_RST_PUSH_P0_set", N, M, value );

    /* ((0x00100000 + (N) * 0x100 + (M) * 0x4) bits 0) field WPTR_RST_PUSH_P0 of register PMC_SCPB_REG_REG_BLOCK_TABLE_REG index N=0..149 index M=0..63 */
    _scpb_reg_reg_BLOCK_TABLE_REG_array_field_set( LOG_CALLER_ARGS
                                                   b_ptr,
                                                   h_ptr,
                                                   N,
                                                   M,
                                                   SCPB_REG_REG_BLOCK_TABLE_REG_BIT_WPTR_RST_PUSH_P0_MSK,
                                                   SCPB_REG_REG_BLOCK_TABLE_REG_BIT_WPTR_RST_PUSH_P0_OFF,
                                                   value);
}


#ifndef scpb_reg_field_WPTR_RST_PUSH_P0_get
#define scpb_reg_field_WPTR_RST_PUSH_P0_get( b_ptr, h_ptr, N, M ) \
       _scpb_reg_field_WPTR_RST_PUSH_P0_get( LOG_CALLER b_ptr, h_ptr, N, M )
#endif
static INLINE UINT32 _scpb_reg_field_WPTR_RST_PUSH_P0_get( LOG_CALLER_DEFN
                                                           scpb_reg_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32  M )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_WPTR_RST_PUSH_P0_get", N, 149);
    if (M > 63)
        IO_RANGE_CHECK("%s M is %d but max is %d", "scpb_reg_field_WPTR_RST_PUSH_P0_get", M, 63);
    /* ((0x00100000 + (N) * 0x100 + (M) * 0x4) bits 0) field WPTR_RST_PUSH_P0 of register PMC_SCPB_REG_REG_BLOCK_TABLE_REG index N=0..149 index M=0..63 */
    reg_value = _scpb_reg_reg_BLOCK_TABLE_REG_array_read( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr,
                                                          N,
                                                          M);
    value = (reg_value & SCPB_REG_REG_BLOCK_TABLE_REG_BIT_WPTR_RST_PUSH_P0_MSK) >> SCPB_REG_REG_BLOCK_TABLE_REG_BIT_WPTR_RST_PUSH_P0_OFF;
    IOLOG( "%s -> N=%d M=%d 0x%08x", "scpb_reg_field_WPTR_RST_PUSH_P0_get", N, M, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_indirect_N_size96
 * ==================================================================================
 */

#ifndef scpb_reg_field_RPTR_RST_POP_set
#define scpb_reg_field_RPTR_RST_POP_set( b_ptr, h_ptr, N, value ) \
       _scpb_reg_field_RPTR_RST_POP_set( LOG_CALLER b_ptr, h_ptr, N, value )
#endif
static INLINE void _scpb_reg_field_RPTR_RST_POP_set( LOG_CALLER_DEFN
                                                     scpb_reg_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_RPTR_RST_POP_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_RPTR_RST_POP_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "scpb_reg_field_RPTR_RST_POP_set", N, value );

    /* ((0x00102000 + (N) * 0x4) bits 0) field RPTR_RST_POP of register PMC_SCPB_REG_REG_POP_PORT_RST index N=0..95 */
    _scpb_reg_reg_POP_PORT_RST_array_field_set( LOG_CALLER_ARGS
                                                b_ptr,
                                                h_ptr,
                                                N,
                                                SCPB_REG_REG_POP_PORT_RST_BIT_RPTR_RST_POP_MSK,
                                                SCPB_REG_REG_POP_PORT_RST_BIT_RPTR_RST_POP_OFF,
                                                value);
}


#ifndef scpb_reg_field_RPTR_RST_POP_get
#define scpb_reg_field_RPTR_RST_POP_get( b_ptr, h_ptr, N ) \
       _scpb_reg_field_RPTR_RST_POP_get( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_field_RPTR_RST_POP_get( LOG_CALLER_DEFN
                                                       scpb_reg_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_RPTR_RST_POP_get", N, 95);
    /* ((0x00102000 + (N) * 0x4) bits 0) field RPTR_RST_POP of register PMC_SCPB_REG_REG_POP_PORT_RST index N=0..95 */
    reg_value = _scpb_reg_reg_POP_PORT_RST_array_read( LOG_CALLER_ARGS
                                                       b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & SCPB_REG_REG_POP_PORT_RST_BIT_RPTR_RST_POP_MSK) >> SCPB_REG_REG_POP_PORT_RST_BIT_RPTR_RST_POP_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "scpb_reg_field_RPTR_RST_POP_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_rising
 * ==================================================================================
 */

#ifndef scpb_reg_field_PAGE_IPT_ERR_I_set_to_clear
#define scpb_reg_field_PAGE_IPT_ERR_I_set_to_clear( b_ptr, h_ptr, value ) \
       _scpb_reg_field_PAGE_IPT_ERR_I_set_to_clear( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_PAGE_IPT_ERR_I_set_to_clear( LOG_CALLER_DEFN
                                                                scpb_reg_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_PAGE_IPT_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_PAGE_IPT_ERR_I_set_to_clear", value );

    /* (0x00104100 bits 0) field PAGE_IPT_ERR_I of register PMC_SCPB_REG_REG_PAGE_IPT_ERR_INT_REG */
    _scpb_reg_reg_PAGE_IPT_ERR_INT_REG_action_on_write_field_set( LOG_CALLER_ARGS
                                                                  b_ptr,
                                                                  h_ptr,
                                                                  SCPB_REG_REG_PAGE_IPT_ERR_INT_REG_BIT_PAGE_IPT_ERR_I_MSK,
                                                                  SCPB_REG_REG_PAGE_IPT_ERR_INT_REG_BIT_PAGE_IPT_ERR_I_OFF,
                                                                  value);
}


#ifndef scpb_reg_field_PAGE_IPT_ERR_I_get
#define scpb_reg_field_PAGE_IPT_ERR_I_get( b_ptr, h_ptr ) \
       _scpb_reg_field_PAGE_IPT_ERR_I_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_PAGE_IPT_ERR_I_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00104100 bits 0) field PAGE_IPT_ERR_I of register PMC_SCPB_REG_REG_PAGE_IPT_ERR_INT_REG */
    reg_value = _scpb_reg_reg_PAGE_IPT_ERR_INT_REG_read( LOG_CALLER_ARGS
                                                         b_ptr,
                                                         h_ptr);
    value = (reg_value & SCPB_REG_REG_PAGE_IPT_ERR_INT_REG_BIT_PAGE_IPT_ERR_I_MSK) >> SCPB_REG_REG_PAGE_IPT_ERR_INT_REG_BIT_PAGE_IPT_ERR_I_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_PAGE_IPT_ERR_I_get", value );

    return value;
}

#ifndef scpb_reg_field_PAGE_IPT_ERR_I_poll
#define scpb_reg_field_PAGE_IPT_ERR_I_poll( b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_field_PAGE_IPT_ERR_I_poll( LOG_CALLER b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_field_PAGE_IPT_ERR_I_poll( LOG_CALLER_DEFN
                                                                        scpb_reg_buffer_t *b_ptr,
                                                                        cpb_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "scpb_reg_field_PAGE_IPT_ERR_I_poll", value );

    /* (0x00104100 bits 0) field PAGE_IPT_ERR_I of register PMC_SCPB_REG_REG_PAGE_IPT_ERR_INT_REG */
    return _scpb_reg_reg_PAGE_IPT_ERR_INT_REG_poll( LOG_CALLER_ARGS
                                                    b_ptr,
                                                    h_ptr,
                                                    SCPB_REG_REG_PAGE_IPT_ERR_INT_REG_BIT_PAGE_IPT_ERR_I_MSK,
                                                    (value<<SCPB_REG_REG_PAGE_IPT_ERR_INT_REG_BIT_PAGE_IPT_ERR_I_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}


#ifndef scpb_reg_field_BLOCK_IPT_ERR_I_set_to_clear
#define scpb_reg_field_BLOCK_IPT_ERR_I_set_to_clear( b_ptr, h_ptr, value ) \
       _scpb_reg_field_BLOCK_IPT_ERR_I_set_to_clear( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_BLOCK_IPT_ERR_I_set_to_clear( LOG_CALLER_DEFN
                                                                 scpb_reg_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_BLOCK_IPT_ERR_I_set_to_clear", value );

    /* (0x00104104 bits 31:0) field BLOCK_IPT_ERR_I of register PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG */
    _scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_action_on_write_field_set( LOG_CALLER_ARGS
                                                                   b_ptr,
                                                                   h_ptr,
                                                                   SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG_BIT_BLOCK_IPT_ERR_I_MSK,
                                                                   SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG_BIT_BLOCK_IPT_ERR_I_OFF,
                                                                   value);
}


#ifndef scpb_reg_field_BLOCK_IPT_ERR_I_get
#define scpb_reg_field_BLOCK_IPT_ERR_I_get( b_ptr, h_ptr ) \
       _scpb_reg_field_BLOCK_IPT_ERR_I_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_BLOCK_IPT_ERR_I_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00104104 bits 31:0) field BLOCK_IPT_ERR_I of register PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG */
    reg_value = _scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_read( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr);
    value = (reg_value & SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG_BIT_BLOCK_IPT_ERR_I_MSK) >> SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG_BIT_BLOCK_IPT_ERR_I_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_BLOCK_IPT_ERR_I_get", value );

    return value;
}

#ifndef scpb_reg_field_range_BLOCK_IPT_ERR_I_set_to_clear
#define scpb_reg_field_range_BLOCK_IPT_ERR_I_set_to_clear( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_BLOCK_IPT_ERR_I_set_to_clear( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_BLOCK_IPT_ERR_I_set_to_clear( LOG_CALLER_DEFN
                                                                       scpb_reg_buffer_t *b_ptr,
                                                                       cpb_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_BLOCK_IPT_ERR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_BLOCK_IPT_ERR_I_set_to_clear", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_BLOCK_IPT_ERR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00104104 bits 31:0) field BLOCK_IPT_ERR_I of register PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG */
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
        /* (0x00104104 bits 31:0) field BLOCK_IPT_ERR_I of register PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG */
        _scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_action_on_write_field_set( LOG_CALLER_ARGS
                                                                       b_ptr,
                                                                       h_ptr,
                                                                       subfield_mask << (SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG_BIT_BLOCK_IPT_ERR_I_OFF + subfield_offset),
                                                                       SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG_BIT_BLOCK_IPT_ERR_I_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_BLOCK_IPT_ERR_I_get
#define scpb_reg_field_range_BLOCK_IPT_ERR_I_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_BLOCK_IPT_ERR_I_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_BLOCK_IPT_ERR_I_get( LOG_CALLER_DEFN
                                                                scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_BLOCK_IPT_ERR_I_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_BLOCK_IPT_ERR_I_get", stop_bit, 31 );
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
    /* (0x00104104 bits 31:0) field BLOCK_IPT_ERR_I of register PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG */
    reg_value = _scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_read( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr);
    field_value = (reg_value & SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG_BIT_BLOCK_IPT_ERR_I_MSK)
                  >> SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG_BIT_BLOCK_IPT_ERR_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG_BIT_BLOCK_IPT_ERR_I_MSK, SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG_BIT_BLOCK_IPT_ERR_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_BLOCK_IPT_ERR_I_get", start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_range_BLOCK_IPT_ERR_I_poll
#define scpb_reg_field_range_BLOCK_IPT_ERR_I_poll( b_ptr, h_ptr, start_bit, stop_bit, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_field_range_BLOCK_IPT_ERR_I_poll( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_field_range_BLOCK_IPT_ERR_I_poll( LOG_CALLER_DEFN
                                                                               scpb_reg_buffer_t *b_ptr,
                                                                               cpb_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_BLOCK_IPT_ERR_I_poll", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_BLOCK_IPT_ERR_I_poll", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_BLOCK_IPT_ERR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00104104 bits 31:0) field BLOCK_IPT_ERR_I of register PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG */
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
        /* (0x00104104 bits 31:0) field BLOCK_IPT_ERR_I of register PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG */
        return _scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_poll( LOG_CALLER_ARGS
                                                         b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG_BIT_BLOCK_IPT_ERR_I_OFF + subfield_offset),
                                                         value << (SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG_BIT_BLOCK_IPT_ERR_I_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}


#ifndef scpb_reg_field_BLOCK_IPT_ERR_I_poll
#define scpb_reg_field_BLOCK_IPT_ERR_I_poll( b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_field_BLOCK_IPT_ERR_I_poll( LOG_CALLER b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_field_BLOCK_IPT_ERR_I_poll( LOG_CALLER_DEFN
                                                                         scpb_reg_buffer_t *b_ptr,
                                                                         cpb_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "scpb_reg_field_BLOCK_IPT_ERR_I_poll", value );

    /* (0x00104104 bits 31:0) field BLOCK_IPT_ERR_I of register PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG */
    return _scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_poll( LOG_CALLER_ARGS
                                                     b_ptr,
                                                     h_ptr,
                                                     SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG_BIT_BLOCK_IPT_ERR_I_MSK,
                                                     (value<<SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG_BIT_BLOCK_IPT_ERR_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}


#ifndef scpb_reg_field_HOQ_DLY_OVERFLOW_I_set_to_clear
#define scpb_reg_field_HOQ_DLY_OVERFLOW_I_set_to_clear( b_ptr, h_ptr, value ) \
       _scpb_reg_field_HOQ_DLY_OVERFLOW_I_set_to_clear( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_HOQ_DLY_OVERFLOW_I_set_to_clear( LOG_CALLER_DEFN
                                                                    scpb_reg_buffer_t *b_ptr,
                                                                    cpb_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_HOQ_DLY_OVERFLOW_I_set_to_clear", value, 15);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_HOQ_DLY_OVERFLOW_I_set_to_clear", value );

    /* (0x00104108 bits 19:16) bits 0:3 use field HOQ_DLY_OVERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
    _scpb_reg_reg_PREFETCH_FIFO_INT_REG_action_on_write_field_set( LOG_CALLER_ARGS
                                                                   b_ptr,
                                                                   h_ptr,
                                                                   SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_DLY_OVERFLOW_I_MSK,
                                                                   SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_DLY_OVERFLOW_I_OFF,
                                                                   value);
}


#ifndef scpb_reg_field_HOQ_DLY_OVERFLOW_I_get
#define scpb_reg_field_HOQ_DLY_OVERFLOW_I_get( b_ptr, h_ptr ) \
       _scpb_reg_field_HOQ_DLY_OVERFLOW_I_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_HOQ_DLY_OVERFLOW_I_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00104108 bits 19:16) bits 0:3 use field HOQ_DLY_OVERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
    reg_value = _scpb_reg_reg_PREFETCH_FIFO_INT_REG_read( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr);
    value = (reg_value & SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_DLY_OVERFLOW_I_MSK) >> SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_DLY_OVERFLOW_I_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_HOQ_DLY_OVERFLOW_I_get", value );

    return value;
}

#ifndef scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_set_to_clear
#define scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_set_to_clear( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_set_to_clear( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_set_to_clear( LOG_CALLER_DEFN
                                                                          scpb_reg_buffer_t *b_ptr,
                                                                          cpb_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_set_to_clear", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00104108 bits 19:16) bits 0:3 use field HOQ_DLY_OVERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
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
        /* (0x00104108 bits 19:16) bits 0:3 use field HOQ_DLY_OVERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
        _scpb_reg_reg_PREFETCH_FIFO_INT_REG_action_on_write_field_set( LOG_CALLER_ARGS
                                                                       b_ptr,
                                                                       h_ptr,
                                                                       subfield_mask << (SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_DLY_OVERFLOW_I_OFF + subfield_offset),
                                                                       SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_DLY_OVERFLOW_I_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_get
#define scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_get( LOG_CALLER_DEFN
                                                                   scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_get", stop_bit, 3 );
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
    /* (0x00104108 bits 19:16) bits 0:3 use field HOQ_DLY_OVERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
    reg_value = _scpb_reg_reg_PREFETCH_FIFO_INT_REG_read( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr);
    field_value = (reg_value & SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_DLY_OVERFLOW_I_MSK)
                  >> SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_DLY_OVERFLOW_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_DLY_OVERFLOW_I_MSK, SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_DLY_OVERFLOW_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_get", start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_poll
#define scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_poll( b_ptr, h_ptr, start_bit, stop_bit, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_poll( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_poll( LOG_CALLER_DEFN
                                                                                  scpb_reg_buffer_t *b_ptr,
                                                                                  cpb_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_poll", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_poll", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00104108 bits 19:16) bits 0:3 use field HOQ_DLY_OVERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00104108 bits 19:16) bits 0:3 use field HOQ_DLY_OVERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
        return _scpb_reg_reg_PREFETCH_FIFO_INT_REG_poll( LOG_CALLER_ARGS
                                                         b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_DLY_OVERFLOW_I_OFF + subfield_offset),
                                                         value << (SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_DLY_OVERFLOW_I_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}


#ifndef scpb_reg_field_HOQ_DLY_OVERFLOW_I_poll
#define scpb_reg_field_HOQ_DLY_OVERFLOW_I_poll( b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_field_HOQ_DLY_OVERFLOW_I_poll( LOG_CALLER b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_field_HOQ_DLY_OVERFLOW_I_poll( LOG_CALLER_DEFN
                                                                            scpb_reg_buffer_t *b_ptr,
                                                                            cpb_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "scpb_reg_field_HOQ_DLY_OVERFLOW_I_poll", value );

    /* (0x00104108 bits 19:16) bits 0:3 use field HOQ_DLY_OVERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
    return _scpb_reg_reg_PREFETCH_FIFO_INT_REG_poll( LOG_CALLER_ARGS
                                                     b_ptr,
                                                     h_ptr,
                                                     SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_DLY_OVERFLOW_I_MSK,
                                                     (value<<SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_DLY_OVERFLOW_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}


#ifndef scpb_reg_field_HOQ_CFC_UNDERFLOW_I_set_to_clear
#define scpb_reg_field_HOQ_CFC_UNDERFLOW_I_set_to_clear( b_ptr, h_ptr, value ) \
       _scpb_reg_field_HOQ_CFC_UNDERFLOW_I_set_to_clear( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_HOQ_CFC_UNDERFLOW_I_set_to_clear( LOG_CALLER_DEFN
                                                                     scpb_reg_buffer_t *b_ptr,
                                                                     cpb_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_HOQ_CFC_UNDERFLOW_I_set_to_clear", value, 15);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_HOQ_CFC_UNDERFLOW_I_set_to_clear", value );

    /* (0x00104108 bits 11:8) bits 0:3 use field HOQ_CFC_UNDERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
    _scpb_reg_reg_PREFETCH_FIFO_INT_REG_action_on_write_field_set( LOG_CALLER_ARGS
                                                                   b_ptr,
                                                                   h_ptr,
                                                                   SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_UNDERFLOW_I_MSK,
                                                                   SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_UNDERFLOW_I_OFF,
                                                                   value);
}


#ifndef scpb_reg_field_HOQ_CFC_UNDERFLOW_I_get
#define scpb_reg_field_HOQ_CFC_UNDERFLOW_I_get( b_ptr, h_ptr ) \
       _scpb_reg_field_HOQ_CFC_UNDERFLOW_I_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_HOQ_CFC_UNDERFLOW_I_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00104108 bits 11:8) bits 0:3 use field HOQ_CFC_UNDERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
    reg_value = _scpb_reg_reg_PREFETCH_FIFO_INT_REG_read( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr);
    value = (reg_value & SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_UNDERFLOW_I_MSK) >> SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_UNDERFLOW_I_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_HOQ_CFC_UNDERFLOW_I_get", value );

    return value;
}

#ifndef scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_set_to_clear
#define scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_set_to_clear( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_set_to_clear( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_set_to_clear( LOG_CALLER_DEFN
                                                                           scpb_reg_buffer_t *b_ptr,
                                                                           cpb_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_set_to_clear", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00104108 bits 11:8) bits 0:3 use field HOQ_CFC_UNDERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
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
        /* (0x00104108 bits 11:8) bits 0:3 use field HOQ_CFC_UNDERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
        _scpb_reg_reg_PREFETCH_FIFO_INT_REG_action_on_write_field_set( LOG_CALLER_ARGS
                                                                       b_ptr,
                                                                       h_ptr,
                                                                       subfield_mask << (SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_UNDERFLOW_I_OFF + subfield_offset),
                                                                       SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_UNDERFLOW_I_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_get
#define scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_get( LOG_CALLER_DEFN
                                                                    scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_get", stop_bit, 3 );
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
    /* (0x00104108 bits 11:8) bits 0:3 use field HOQ_CFC_UNDERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
    reg_value = _scpb_reg_reg_PREFETCH_FIFO_INT_REG_read( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr);
    field_value = (reg_value & SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_UNDERFLOW_I_MSK)
                  >> SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_UNDERFLOW_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_UNDERFLOW_I_MSK, SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_UNDERFLOW_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_get", start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_poll
#define scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_poll( b_ptr, h_ptr, start_bit, stop_bit, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_poll( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_poll( LOG_CALLER_DEFN
                                                                                   scpb_reg_buffer_t *b_ptr,
                                                                                   cpb_handle_t *h_ptr,
                                                                                   UINT32 start_bit,
                                                                                   UINT32 stop_bit,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_poll", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_poll", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00104108 bits 11:8) bits 0:3 use field HOQ_CFC_UNDERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00104108 bits 11:8) bits 0:3 use field HOQ_CFC_UNDERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
        return _scpb_reg_reg_PREFETCH_FIFO_INT_REG_poll( LOG_CALLER_ARGS
                                                         b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_UNDERFLOW_I_OFF + subfield_offset),
                                                         value << (SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_UNDERFLOW_I_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}


#ifndef scpb_reg_field_HOQ_CFC_UNDERFLOW_I_poll
#define scpb_reg_field_HOQ_CFC_UNDERFLOW_I_poll( b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_field_HOQ_CFC_UNDERFLOW_I_poll( LOG_CALLER b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_field_HOQ_CFC_UNDERFLOW_I_poll( LOG_CALLER_DEFN
                                                                             scpb_reg_buffer_t *b_ptr,
                                                                             cpb_handle_t *h_ptr,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "scpb_reg_field_HOQ_CFC_UNDERFLOW_I_poll", value );

    /* (0x00104108 bits 11:8) bits 0:3 use field HOQ_CFC_UNDERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
    return _scpb_reg_reg_PREFETCH_FIFO_INT_REG_poll( LOG_CALLER_ARGS
                                                     b_ptr,
                                                     h_ptr,
                                                     SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_UNDERFLOW_I_MSK,
                                                     (value<<SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_UNDERFLOW_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}


#ifndef scpb_reg_field_HOQ_CFC_OVERFLOW_I_set_to_clear
#define scpb_reg_field_HOQ_CFC_OVERFLOW_I_set_to_clear( b_ptr, h_ptr, value ) \
       _scpb_reg_field_HOQ_CFC_OVERFLOW_I_set_to_clear( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_HOQ_CFC_OVERFLOW_I_set_to_clear( LOG_CALLER_DEFN
                                                                    scpb_reg_buffer_t *b_ptr,
                                                                    cpb_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_HOQ_CFC_OVERFLOW_I_set_to_clear", value, 15);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_HOQ_CFC_OVERFLOW_I_set_to_clear", value );

    /* (0x00104108 bits 3:0) bits 0:3 use field HOQ_CFC_OVERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
    _scpb_reg_reg_PREFETCH_FIFO_INT_REG_action_on_write_field_set( LOG_CALLER_ARGS
                                                                   b_ptr,
                                                                   h_ptr,
                                                                   SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_OVERFLOW_I_MSK,
                                                                   SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_OVERFLOW_I_OFF,
                                                                   value);
}


#ifndef scpb_reg_field_HOQ_CFC_OVERFLOW_I_get
#define scpb_reg_field_HOQ_CFC_OVERFLOW_I_get( b_ptr, h_ptr ) \
       _scpb_reg_field_HOQ_CFC_OVERFLOW_I_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_HOQ_CFC_OVERFLOW_I_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00104108 bits 3:0) bits 0:3 use field HOQ_CFC_OVERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
    reg_value = _scpb_reg_reg_PREFETCH_FIFO_INT_REG_read( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr);
    value = (reg_value & SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_OVERFLOW_I_MSK) >> SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_OVERFLOW_I_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_HOQ_CFC_OVERFLOW_I_get", value );

    return value;
}

#ifndef scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_set_to_clear
#define scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_set_to_clear( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_set_to_clear( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_set_to_clear( LOG_CALLER_DEFN
                                                                          scpb_reg_buffer_t *b_ptr,
                                                                          cpb_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_set_to_clear", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00104108 bits 3:0) bits 0:3 use field HOQ_CFC_OVERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
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
        /* (0x00104108 bits 3:0) bits 0:3 use field HOQ_CFC_OVERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
        _scpb_reg_reg_PREFETCH_FIFO_INT_REG_action_on_write_field_set( LOG_CALLER_ARGS
                                                                       b_ptr,
                                                                       h_ptr,
                                                                       subfield_mask << (SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_OVERFLOW_I_OFF + subfield_offset),
                                                                       SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_OVERFLOW_I_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_get
#define scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_get( LOG_CALLER_DEFN
                                                                   scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_get", stop_bit, 3 );
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
    /* (0x00104108 bits 3:0) bits 0:3 use field HOQ_CFC_OVERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
    reg_value = _scpb_reg_reg_PREFETCH_FIFO_INT_REG_read( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr);
    field_value = (reg_value & SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_OVERFLOW_I_MSK)
                  >> SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_OVERFLOW_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_OVERFLOW_I_MSK, SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_OVERFLOW_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_get", start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_poll
#define scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_poll( b_ptr, h_ptr, start_bit, stop_bit, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_poll( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_poll( LOG_CALLER_DEFN
                                                                                  scpb_reg_buffer_t *b_ptr,
                                                                                  cpb_handle_t *h_ptr,
                                                                                  UINT32 start_bit,
                                                                                  UINT32 stop_bit,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_poll", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_poll", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00104108 bits 3:0) bits 0:3 use field HOQ_CFC_OVERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00104108 bits 3:0) bits 0:3 use field HOQ_CFC_OVERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
        return _scpb_reg_reg_PREFETCH_FIFO_INT_REG_poll( LOG_CALLER_ARGS
                                                         b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_OVERFLOW_I_OFF + subfield_offset),
                                                         value << (SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_OVERFLOW_I_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}


#ifndef scpb_reg_field_HOQ_CFC_OVERFLOW_I_poll
#define scpb_reg_field_HOQ_CFC_OVERFLOW_I_poll( b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_field_HOQ_CFC_OVERFLOW_I_poll( LOG_CALLER b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_field_HOQ_CFC_OVERFLOW_I_poll( LOG_CALLER_DEFN
                                                                            scpb_reg_buffer_t *b_ptr,
                                                                            cpb_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "scpb_reg_field_HOQ_CFC_OVERFLOW_I_poll", value );

    /* (0x00104108 bits 3:0) bits 0:3 use field HOQ_CFC_OVERFLOW_I of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG */
    return _scpb_reg_reg_PREFETCH_FIFO_INT_REG_poll( LOG_CALLER_ARGS
                                                     b_ptr,
                                                     h_ptr,
                                                     SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_OVERFLOW_I_MSK,
                                                     (value<<SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_OVERFLOW_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}


#ifndef scpb_reg_field_FIFO_UDR_I_set_to_clear
#define scpb_reg_field_FIFO_UDR_I_set_to_clear( b_ptr, h_ptr, value ) \
       _scpb_reg_field_FIFO_UDR_I_set_to_clear( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_FIFO_UDR_I_set_to_clear( LOG_CALLER_DEFN
                                                            scpb_reg_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_FIFO_UDR_I_set_to_clear", value );

    /* (0x00102800 bits 8) field FIFO_UDR_I of register PMC_SCPB_REG_REG_FIFO_INT_REG */
    _scpb_reg_reg_FIFO_INT_REG_action_on_write_field_set( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr,
                                                          SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_UDR_I_MSK,
                                                          SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_UDR_I_OFF,
                                                          value);
}


#ifndef scpb_reg_field_FIFO_UDR_I_get
#define scpb_reg_field_FIFO_UDR_I_get( b_ptr, h_ptr ) \
       _scpb_reg_field_FIFO_UDR_I_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_FIFO_UDR_I_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00102800 bits 8) field FIFO_UDR_I of register PMC_SCPB_REG_REG_FIFO_INT_REG */
    reg_value = _scpb_reg_reg_FIFO_INT_REG_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    value = (reg_value & SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_UDR_I_MSK) >> SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_FIFO_UDR_I_get", value );

    return value;
}

#ifndef scpb_reg_field_FIFO_UDR_I_poll
#define scpb_reg_field_FIFO_UDR_I_poll( b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_field_FIFO_UDR_I_poll( LOG_CALLER b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_field_FIFO_UDR_I_poll( LOG_CALLER_DEFN
                                                                    scpb_reg_buffer_t *b_ptr,
                                                                    cpb_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "scpb_reg_field_FIFO_UDR_I_poll", value );

    /* (0x00102800 bits 8) field FIFO_UDR_I of register PMC_SCPB_REG_REG_FIFO_INT_REG */
    return _scpb_reg_reg_FIFO_INT_REG_poll( LOG_CALLER_ARGS
                                            b_ptr,
                                            h_ptr,
                                            SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_UDR_I_MSK,
                                            (value<<SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_UDR_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}


#ifndef scpb_reg_field_FIFO_OVR_PUSH_I_set_to_clear
#define scpb_reg_field_FIFO_OVR_PUSH_I_set_to_clear( b_ptr, h_ptr, value ) \
       _scpb_reg_field_FIFO_OVR_PUSH_I_set_to_clear( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_FIFO_OVR_PUSH_I_set_to_clear( LOG_CALLER_DEFN
                                                                 scpb_reg_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_FIFO_OVR_PUSH_I_set_to_clear", value, 15);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_FIFO_OVR_PUSH_I_set_to_clear", value );

    /* (0x00102800 bits 3:0) field FIFO_OVR_PUSH_I of register PMC_SCPB_REG_REG_FIFO_INT_REG */
    _scpb_reg_reg_FIFO_INT_REG_action_on_write_field_set( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr,
                                                          SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_OVR_PUSH_I_MSK,
                                                          SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_OVR_PUSH_I_OFF,
                                                          value);
}


#ifndef scpb_reg_field_FIFO_OVR_PUSH_I_get
#define scpb_reg_field_FIFO_OVR_PUSH_I_get( b_ptr, h_ptr ) \
       _scpb_reg_field_FIFO_OVR_PUSH_I_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_FIFO_OVR_PUSH_I_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00102800 bits 3:0) field FIFO_OVR_PUSH_I of register PMC_SCPB_REG_REG_FIFO_INT_REG */
    reg_value = _scpb_reg_reg_FIFO_INT_REG_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    value = (reg_value & SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_OVR_PUSH_I_MSK) >> SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_OVR_PUSH_I_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_FIFO_OVR_PUSH_I_get", value );

    return value;
}

#ifndef scpb_reg_field_range_FIFO_OVR_PUSH_I_set_to_clear
#define scpb_reg_field_range_FIFO_OVR_PUSH_I_set_to_clear( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_FIFO_OVR_PUSH_I_set_to_clear( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_FIFO_OVR_PUSH_I_set_to_clear( LOG_CALLER_DEFN
                                                                       scpb_reg_buffer_t *b_ptr,
                                                                       cpb_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_FIFO_OVR_PUSH_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_FIFO_OVR_PUSH_I_set_to_clear", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_FIFO_OVR_PUSH_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00102800 bits 3:0) field FIFO_OVR_PUSH_I of register PMC_SCPB_REG_REG_FIFO_INT_REG */
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
        /* (0x00102800 bits 3:0) field FIFO_OVR_PUSH_I of register PMC_SCPB_REG_REG_FIFO_INT_REG */
        _scpb_reg_reg_FIFO_INT_REG_action_on_write_field_set( LOG_CALLER_ARGS
                                                              b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_OVR_PUSH_I_OFF + subfield_offset),
                                                              SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_OVR_PUSH_I_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_FIFO_OVR_PUSH_I_get
#define scpb_reg_field_range_FIFO_OVR_PUSH_I_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_FIFO_OVR_PUSH_I_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_FIFO_OVR_PUSH_I_get( LOG_CALLER_DEFN
                                                                scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_FIFO_OVR_PUSH_I_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_FIFO_OVR_PUSH_I_get", stop_bit, 3 );
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
    /* (0x00102800 bits 3:0) field FIFO_OVR_PUSH_I of register PMC_SCPB_REG_REG_FIFO_INT_REG */
    reg_value = _scpb_reg_reg_FIFO_INT_REG_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    field_value = (reg_value & SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_OVR_PUSH_I_MSK)
                  >> SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_OVR_PUSH_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_OVR_PUSH_I_MSK, SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_OVR_PUSH_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_FIFO_OVR_PUSH_I_get", start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_range_FIFO_OVR_PUSH_I_poll
#define scpb_reg_field_range_FIFO_OVR_PUSH_I_poll( b_ptr, h_ptr, start_bit, stop_bit, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_field_range_FIFO_OVR_PUSH_I_poll( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_field_range_FIFO_OVR_PUSH_I_poll( LOG_CALLER_DEFN
                                                                               scpb_reg_buffer_t *b_ptr,
                                                                               cpb_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_FIFO_OVR_PUSH_I_poll", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_FIFO_OVR_PUSH_I_poll", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_FIFO_OVR_PUSH_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00102800 bits 3:0) field FIFO_OVR_PUSH_I of register PMC_SCPB_REG_REG_FIFO_INT_REG */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00102800 bits 3:0) field FIFO_OVR_PUSH_I of register PMC_SCPB_REG_REG_FIFO_INT_REG */
        return _scpb_reg_reg_FIFO_INT_REG_poll( LOG_CALLER_ARGS
                                                b_ptr,
                                                h_ptr,
                                                subfield_mask << (SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_OVR_PUSH_I_OFF + subfield_offset),
                                                value << (SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_OVR_PUSH_I_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}


#ifndef scpb_reg_field_FIFO_OVR_PUSH_I_poll
#define scpb_reg_field_FIFO_OVR_PUSH_I_poll( b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_field_FIFO_OVR_PUSH_I_poll( LOG_CALLER b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_field_FIFO_OVR_PUSH_I_poll( LOG_CALLER_DEFN
                                                                         scpb_reg_buffer_t *b_ptr,
                                                                         cpb_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "scpb_reg_field_FIFO_OVR_PUSH_I_poll", value );

    /* (0x00102800 bits 3:0) field FIFO_OVR_PUSH_I of register PMC_SCPB_REG_REG_FIFO_INT_REG */
    return _scpb_reg_reg_FIFO_INT_REG_poll( LOG_CALLER_ARGS
                                            b_ptr,
                                            h_ptr,
                                            SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_OVR_PUSH_I_MSK,
                                            (value<<SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_OVR_PUSH_I_OFF),
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

#ifndef scpb_reg_field_PAGE_IPT_ERR_E_set
#define scpb_reg_field_PAGE_IPT_ERR_E_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_PAGE_IPT_ERR_E_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_PAGE_IPT_ERR_E_set( LOG_CALLER_DEFN
                                                       scpb_reg_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_PAGE_IPT_ERR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_PAGE_IPT_ERR_E_set", value );

    /* (0x00104110 bits 0) field PAGE_IPT_ERR_E of register PMC_SCPB_REG_REG_PAGE_IPT_ERR_INT_EN_REG */
    _scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_field_set( LOG_CALLER_ARGS
                                                     b_ptr,
                                                     h_ptr,
                                                     SCPB_REG_REG_PAGE_IPT_ERR_INT_EN_REG_BIT_PAGE_IPT_ERR_E_MSK,
                                                     SCPB_REG_REG_PAGE_IPT_ERR_INT_EN_REG_BIT_PAGE_IPT_ERR_E_OFF,
                                                     value);
}


#ifndef scpb_reg_field_PAGE_IPT_ERR_E_get
#define scpb_reg_field_PAGE_IPT_ERR_E_get( b_ptr, h_ptr ) \
       _scpb_reg_field_PAGE_IPT_ERR_E_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_PAGE_IPT_ERR_E_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00104110 bits 0) field PAGE_IPT_ERR_E of register PMC_SCPB_REG_REG_PAGE_IPT_ERR_INT_EN_REG */
    reg_value = _scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_read( LOG_CALLER_ARGS
                                                            b_ptr,
                                                            h_ptr);
    value = (reg_value & SCPB_REG_REG_PAGE_IPT_ERR_INT_EN_REG_BIT_PAGE_IPT_ERR_E_MSK) >> SCPB_REG_REG_PAGE_IPT_ERR_INT_EN_REG_BIT_PAGE_IPT_ERR_E_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_PAGE_IPT_ERR_E_get", value );

    return value;
}

#ifndef scpb_reg_field_BLOCK_IPT_ERR_E_set
#define scpb_reg_field_BLOCK_IPT_ERR_E_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_BLOCK_IPT_ERR_E_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_BLOCK_IPT_ERR_E_set( LOG_CALLER_DEFN
                                                        scpb_reg_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_BLOCK_IPT_ERR_E_set", value );

    /* (0x00104114 bits 31:0) field BLOCK_IPT_ERR_E of register PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG */
    _scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_field_set( LOG_CALLER_ARGS
                                                      b_ptr,
                                                      h_ptr,
                                                      SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG_BIT_BLOCK_IPT_ERR_E_MSK,
                                                      SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG_BIT_BLOCK_IPT_ERR_E_OFF,
                                                      value);
}


#ifndef scpb_reg_field_BLOCK_IPT_ERR_E_get
#define scpb_reg_field_BLOCK_IPT_ERR_E_get( b_ptr, h_ptr ) \
       _scpb_reg_field_BLOCK_IPT_ERR_E_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_BLOCK_IPT_ERR_E_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00104114 bits 31:0) field BLOCK_IPT_ERR_E of register PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG */
    reg_value = _scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_read( LOG_CALLER_ARGS
                                                             b_ptr,
                                                             h_ptr);
    value = (reg_value & SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG_BIT_BLOCK_IPT_ERR_E_MSK) >> SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG_BIT_BLOCK_IPT_ERR_E_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_BLOCK_IPT_ERR_E_get", value );

    return value;
}

#ifndef scpb_reg_field_range_BLOCK_IPT_ERR_E_set
#define scpb_reg_field_range_BLOCK_IPT_ERR_E_set( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_BLOCK_IPT_ERR_E_set( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_BLOCK_IPT_ERR_E_set( LOG_CALLER_DEFN
                                                              scpb_reg_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_BLOCK_IPT_ERR_E_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_BLOCK_IPT_ERR_E_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_BLOCK_IPT_ERR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00104114 bits 31:0) field BLOCK_IPT_ERR_E of register PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG */
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
        /* (0x00104114 bits 31:0) field BLOCK_IPT_ERR_E of register PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG */
        _scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_field_set( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG_BIT_BLOCK_IPT_ERR_E_OFF + subfield_offset),
                                                          SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG_BIT_BLOCK_IPT_ERR_E_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_BLOCK_IPT_ERR_E_get
#define scpb_reg_field_range_BLOCK_IPT_ERR_E_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_BLOCK_IPT_ERR_E_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_BLOCK_IPT_ERR_E_get( LOG_CALLER_DEFN
                                                                scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_BLOCK_IPT_ERR_E_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_BLOCK_IPT_ERR_E_get", stop_bit, 31 );
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
    /* (0x00104114 bits 31:0) field BLOCK_IPT_ERR_E of register PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG */
    reg_value = _scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_read( LOG_CALLER_ARGS
                                                             b_ptr,
                                                             h_ptr);
    field_value = (reg_value & SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG_BIT_BLOCK_IPT_ERR_E_MSK)
                  >> SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG_BIT_BLOCK_IPT_ERR_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG_BIT_BLOCK_IPT_ERR_E_MSK, SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG_BIT_BLOCK_IPT_ERR_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_BLOCK_IPT_ERR_E_get", start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_HOQ_DLY_OVERFLOW_E_set
#define scpb_reg_field_HOQ_DLY_OVERFLOW_E_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_HOQ_DLY_OVERFLOW_E_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_HOQ_DLY_OVERFLOW_E_set( LOG_CALLER_DEFN
                                                           scpb_reg_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_HOQ_DLY_OVERFLOW_E_set", value, 15);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_HOQ_DLY_OVERFLOW_E_set", value );

    /* (0x00104118 bits 19:16) bits 0:3 use field HOQ_DLY_OVERFLOW_E of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG */
    _scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_field_set( LOG_CALLER_ARGS
                                                      b_ptr,
                                                      h_ptr,
                                                      SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_DLY_OVERFLOW_E_MSK,
                                                      SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_DLY_OVERFLOW_E_OFF,
                                                      value);
}


#ifndef scpb_reg_field_HOQ_DLY_OVERFLOW_E_get
#define scpb_reg_field_HOQ_DLY_OVERFLOW_E_get( b_ptr, h_ptr ) \
       _scpb_reg_field_HOQ_DLY_OVERFLOW_E_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_HOQ_DLY_OVERFLOW_E_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00104118 bits 19:16) bits 0:3 use field HOQ_DLY_OVERFLOW_E of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG */
    reg_value = _scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_read( LOG_CALLER_ARGS
                                                             b_ptr,
                                                             h_ptr);
    value = (reg_value & SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_DLY_OVERFLOW_E_MSK) >> SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_DLY_OVERFLOW_E_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_HOQ_DLY_OVERFLOW_E_get", value );

    return value;
}

#ifndef scpb_reg_field_range_HOQ_DLY_OVERFLOW_E_set
#define scpb_reg_field_range_HOQ_DLY_OVERFLOW_E_set( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_HOQ_DLY_OVERFLOW_E_set( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_HOQ_DLY_OVERFLOW_E_set( LOG_CALLER_DEFN
                                                                 scpb_reg_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_HOQ_DLY_OVERFLOW_E_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_HOQ_DLY_OVERFLOW_E_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_HOQ_DLY_OVERFLOW_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00104118 bits 19:16) bits 0:3 use field HOQ_DLY_OVERFLOW_E of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG */
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
        /* (0x00104118 bits 19:16) bits 0:3 use field HOQ_DLY_OVERFLOW_E of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG */
        _scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_field_set( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_DLY_OVERFLOW_E_OFF + subfield_offset),
                                                          SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_DLY_OVERFLOW_E_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_HOQ_DLY_OVERFLOW_E_get
#define scpb_reg_field_range_HOQ_DLY_OVERFLOW_E_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_HOQ_DLY_OVERFLOW_E_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_HOQ_DLY_OVERFLOW_E_get( LOG_CALLER_DEFN
                                                                   scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_HOQ_DLY_OVERFLOW_E_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_HOQ_DLY_OVERFLOW_E_get", stop_bit, 3 );
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
    /* (0x00104118 bits 19:16) bits 0:3 use field HOQ_DLY_OVERFLOW_E of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG */
    reg_value = _scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_read( LOG_CALLER_ARGS
                                                             b_ptr,
                                                             h_ptr);
    field_value = (reg_value & SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_DLY_OVERFLOW_E_MSK)
                  >> SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_DLY_OVERFLOW_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_DLY_OVERFLOW_E_MSK, SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_DLY_OVERFLOW_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_HOQ_DLY_OVERFLOW_E_get", start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_HOQ_CFC_UNDERFLOW_E_set
#define scpb_reg_field_HOQ_CFC_UNDERFLOW_E_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_HOQ_CFC_UNDERFLOW_E_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_HOQ_CFC_UNDERFLOW_E_set( LOG_CALLER_DEFN
                                                            scpb_reg_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_HOQ_CFC_UNDERFLOW_E_set", value, 15);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_HOQ_CFC_UNDERFLOW_E_set", value );

    /* (0x00104118 bits 11:8) bits 0:3 use field HOQ_CFC_UNDERFLOW_E of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG */
    _scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_field_set( LOG_CALLER_ARGS
                                                      b_ptr,
                                                      h_ptr,
                                                      SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_UNDERFLOW_E_MSK,
                                                      SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_UNDERFLOW_E_OFF,
                                                      value);
}


#ifndef scpb_reg_field_HOQ_CFC_UNDERFLOW_E_get
#define scpb_reg_field_HOQ_CFC_UNDERFLOW_E_get( b_ptr, h_ptr ) \
       _scpb_reg_field_HOQ_CFC_UNDERFLOW_E_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_HOQ_CFC_UNDERFLOW_E_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00104118 bits 11:8) bits 0:3 use field HOQ_CFC_UNDERFLOW_E of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG */
    reg_value = _scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_read( LOG_CALLER_ARGS
                                                             b_ptr,
                                                             h_ptr);
    value = (reg_value & SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_UNDERFLOW_E_MSK) >> SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_UNDERFLOW_E_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_HOQ_CFC_UNDERFLOW_E_get", value );

    return value;
}

#ifndef scpb_reg_field_range_HOQ_CFC_UNDERFLOW_E_set
#define scpb_reg_field_range_HOQ_CFC_UNDERFLOW_E_set( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_HOQ_CFC_UNDERFLOW_E_set( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_HOQ_CFC_UNDERFLOW_E_set( LOG_CALLER_DEFN
                                                                  scpb_reg_buffer_t *b_ptr,
                                                                  cpb_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_HOQ_CFC_UNDERFLOW_E_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_HOQ_CFC_UNDERFLOW_E_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_HOQ_CFC_UNDERFLOW_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00104118 bits 11:8) bits 0:3 use field HOQ_CFC_UNDERFLOW_E of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG */
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
        /* (0x00104118 bits 11:8) bits 0:3 use field HOQ_CFC_UNDERFLOW_E of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG */
        _scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_field_set( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_UNDERFLOW_E_OFF + subfield_offset),
                                                          SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_UNDERFLOW_E_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_HOQ_CFC_UNDERFLOW_E_get
#define scpb_reg_field_range_HOQ_CFC_UNDERFLOW_E_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_HOQ_CFC_UNDERFLOW_E_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_HOQ_CFC_UNDERFLOW_E_get( LOG_CALLER_DEFN
                                                                    scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_HOQ_CFC_UNDERFLOW_E_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_HOQ_CFC_UNDERFLOW_E_get", stop_bit, 3 );
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
    /* (0x00104118 bits 11:8) bits 0:3 use field HOQ_CFC_UNDERFLOW_E of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG */
    reg_value = _scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_read( LOG_CALLER_ARGS
                                                             b_ptr,
                                                             h_ptr);
    field_value = (reg_value & SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_UNDERFLOW_E_MSK)
                  >> SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_UNDERFLOW_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_UNDERFLOW_E_MSK, SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_UNDERFLOW_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_HOQ_CFC_UNDERFLOW_E_get", start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_HOQ_CFC_OVERFLOW_E_set
#define scpb_reg_field_HOQ_CFC_OVERFLOW_E_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_HOQ_CFC_OVERFLOW_E_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_HOQ_CFC_OVERFLOW_E_set( LOG_CALLER_DEFN
                                                           scpb_reg_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_HOQ_CFC_OVERFLOW_E_set", value, 15);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_HOQ_CFC_OVERFLOW_E_set", value );

    /* (0x00104118 bits 3:0) bits 0:3 use field HOQ_CFC_OVERFLOW_E of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG */
    _scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_field_set( LOG_CALLER_ARGS
                                                      b_ptr,
                                                      h_ptr,
                                                      SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_OVERFLOW_E_MSK,
                                                      SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_OVERFLOW_E_OFF,
                                                      value);
}


#ifndef scpb_reg_field_HOQ_CFC_OVERFLOW_E_get
#define scpb_reg_field_HOQ_CFC_OVERFLOW_E_get( b_ptr, h_ptr ) \
       _scpb_reg_field_HOQ_CFC_OVERFLOW_E_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_HOQ_CFC_OVERFLOW_E_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00104118 bits 3:0) bits 0:3 use field HOQ_CFC_OVERFLOW_E of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG */
    reg_value = _scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_read( LOG_CALLER_ARGS
                                                             b_ptr,
                                                             h_ptr);
    value = (reg_value & SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_OVERFLOW_E_MSK) >> SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_OVERFLOW_E_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_HOQ_CFC_OVERFLOW_E_get", value );

    return value;
}

#ifndef scpb_reg_field_range_HOQ_CFC_OVERFLOW_E_set
#define scpb_reg_field_range_HOQ_CFC_OVERFLOW_E_set( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_HOQ_CFC_OVERFLOW_E_set( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_HOQ_CFC_OVERFLOW_E_set( LOG_CALLER_DEFN
                                                                 scpb_reg_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_HOQ_CFC_OVERFLOW_E_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_HOQ_CFC_OVERFLOW_E_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_HOQ_CFC_OVERFLOW_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00104118 bits 3:0) bits 0:3 use field HOQ_CFC_OVERFLOW_E of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG */
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
        /* (0x00104118 bits 3:0) bits 0:3 use field HOQ_CFC_OVERFLOW_E of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG */
        _scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_field_set( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_OVERFLOW_E_OFF + subfield_offset),
                                                          SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_OVERFLOW_E_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_HOQ_CFC_OVERFLOW_E_get
#define scpb_reg_field_range_HOQ_CFC_OVERFLOW_E_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_HOQ_CFC_OVERFLOW_E_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_HOQ_CFC_OVERFLOW_E_get( LOG_CALLER_DEFN
                                                                   scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_HOQ_CFC_OVERFLOW_E_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_HOQ_CFC_OVERFLOW_E_get", stop_bit, 3 );
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
    /* (0x00104118 bits 3:0) bits 0:3 use field HOQ_CFC_OVERFLOW_E of register PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG */
    reg_value = _scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_read( LOG_CALLER_ARGS
                                                             b_ptr,
                                                             h_ptr);
    field_value = (reg_value & SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_OVERFLOW_E_MSK)
                  >> SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_OVERFLOW_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_OVERFLOW_E_MSK, SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_OVERFLOW_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_HOQ_CFC_OVERFLOW_E_get", start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_FIFO_UDR_E_set
#define scpb_reg_field_FIFO_UDR_E_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_FIFO_UDR_E_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_FIFO_UDR_E_set( LOG_CALLER_DEFN
                                                   scpb_reg_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_FIFO_UDR_E_set", value );

    /* (0x00102804 bits 8) field FIFO_UDR_E of register PMC_SCPB_REG_REG_FIFO_INT_EN_REG */
    _scpb_reg_reg_FIFO_INT_EN_REG_field_set( LOG_CALLER_ARGS
                                             b_ptr,
                                             h_ptr,
                                             SCPB_REG_REG_FIFO_INT_EN_REG_BIT_FIFO_UDR_E_MSK,
                                             SCPB_REG_REG_FIFO_INT_EN_REG_BIT_FIFO_UDR_E_OFF,
                                             value);
}


#ifndef scpb_reg_field_FIFO_UDR_E_get
#define scpb_reg_field_FIFO_UDR_E_get( b_ptr, h_ptr ) \
       _scpb_reg_field_FIFO_UDR_E_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_FIFO_UDR_E_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00102804 bits 8) field FIFO_UDR_E of register PMC_SCPB_REG_REG_FIFO_INT_EN_REG */
    reg_value = _scpb_reg_reg_FIFO_INT_EN_REG_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    value = (reg_value & SCPB_REG_REG_FIFO_INT_EN_REG_BIT_FIFO_UDR_E_MSK) >> SCPB_REG_REG_FIFO_INT_EN_REG_BIT_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_FIFO_UDR_E_get", value );

    return value;
}

#ifndef scpb_reg_field_FIFO_OVR_PUSH_E_set
#define scpb_reg_field_FIFO_OVR_PUSH_E_set( b_ptr, h_ptr, value ) \
       _scpb_reg_field_FIFO_OVR_PUSH_E_set( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_field_FIFO_OVR_PUSH_E_set( LOG_CALLER_DEFN
                                                        scpb_reg_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "scpb_reg_field_FIFO_OVR_PUSH_E_set", value, 15);
    IOLOG( "%s <= 0x%08x", "scpb_reg_field_FIFO_OVR_PUSH_E_set", value );

    /* (0x00102804 bits 3:0) field FIFO_OVR_PUSH_E of register PMC_SCPB_REG_REG_FIFO_INT_EN_REG */
    _scpb_reg_reg_FIFO_INT_EN_REG_field_set( LOG_CALLER_ARGS
                                             b_ptr,
                                             h_ptr,
                                             SCPB_REG_REG_FIFO_INT_EN_REG_BIT_FIFO_OVR_PUSH_E_MSK,
                                             SCPB_REG_REG_FIFO_INT_EN_REG_BIT_FIFO_OVR_PUSH_E_OFF,
                                             value);
}


#ifndef scpb_reg_field_FIFO_OVR_PUSH_E_get
#define scpb_reg_field_FIFO_OVR_PUSH_E_get( b_ptr, h_ptr ) \
       _scpb_reg_field_FIFO_OVR_PUSH_E_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_FIFO_OVR_PUSH_E_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00102804 bits 3:0) field FIFO_OVR_PUSH_E of register PMC_SCPB_REG_REG_FIFO_INT_EN_REG */
    reg_value = _scpb_reg_reg_FIFO_INT_EN_REG_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    value = (reg_value & SCPB_REG_REG_FIFO_INT_EN_REG_BIT_FIFO_OVR_PUSH_E_MSK) >> SCPB_REG_REG_FIFO_INT_EN_REG_BIT_FIFO_OVR_PUSH_E_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_FIFO_OVR_PUSH_E_get", value );

    return value;
}

#ifndef scpb_reg_field_range_FIFO_OVR_PUSH_E_set
#define scpb_reg_field_range_FIFO_OVR_PUSH_E_set( b_ptr, h_ptr, start_bit, stop_bit, value ) \
       _scpb_reg_field_range_FIFO_OVR_PUSH_E_set( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit, value )
#endif
static INLINE void _scpb_reg_field_range_FIFO_OVR_PUSH_E_set( LOG_CALLER_DEFN
                                                              scpb_reg_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_FIFO_OVR_PUSH_E_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_FIFO_OVR_PUSH_E_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_FIFO_OVR_PUSH_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00102804 bits 3:0) field FIFO_OVR_PUSH_E of register PMC_SCPB_REG_REG_FIFO_INT_EN_REG */
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
        /* (0x00102804 bits 3:0) field FIFO_OVR_PUSH_E of register PMC_SCPB_REG_REG_FIFO_INT_EN_REG */
        _scpb_reg_reg_FIFO_INT_EN_REG_field_set( LOG_CALLER_ARGS
                                                 b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (SCPB_REG_REG_FIFO_INT_EN_REG_BIT_FIFO_OVR_PUSH_E_OFF + subfield_offset),
                                                 SCPB_REG_REG_FIFO_INT_EN_REG_BIT_FIFO_OVR_PUSH_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}


#ifndef scpb_reg_field_range_FIFO_OVR_PUSH_E_get
#define scpb_reg_field_range_FIFO_OVR_PUSH_E_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_FIFO_OVR_PUSH_E_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_FIFO_OVR_PUSH_E_get( LOG_CALLER_DEFN
                                                                scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_FIFO_OVR_PUSH_E_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_FIFO_OVR_PUSH_E_get", stop_bit, 3 );
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
    /* (0x00102804 bits 3:0) field FIFO_OVR_PUSH_E of register PMC_SCPB_REG_REG_FIFO_INT_EN_REG */
    reg_value = _scpb_reg_reg_FIFO_INT_EN_REG_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    field_value = (reg_value & SCPB_REG_REG_FIFO_INT_EN_REG_BIT_FIFO_OVR_PUSH_E_MSK)
                  >> SCPB_REG_REG_FIFO_INT_EN_REG_BIT_FIFO_OVR_PUSH_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_FIFO_INT_EN_REG_BIT_FIFO_OVR_PUSH_E_MSK, SCPB_REG_REG_FIFO_INT_EN_REG_BIT_FIFO_OVR_PUSH_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_FIFO_OVR_PUSH_E_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */

#ifndef scpb_reg_field_REMAP_SUCCESS_get
#define scpb_reg_field_REMAP_SUCCESS_get( b_ptr, h_ptr ) \
       _scpb_reg_field_REMAP_SUCCESS_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_REMAP_SUCCESS_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0010420c bits 0) field REMAP_SUCCESS of register PMC_SCPB_REG_REG_REMAP_STAT */
    reg_value = _scpb_reg_reg_REMAP_STAT_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    value = (reg_value & SCPB_REG_REG_REMAP_STAT_BIT_REMAP_SUCCESS_MSK) >> SCPB_REG_REG_REMAP_STAT_BIT_REMAP_SUCCESS_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_REMAP_SUCCESS_get", value );

    return value;
}

#ifndef scpb_reg_field_FIFO_END_ADDR_CHG_SUCCESS_get
#define scpb_reg_field_FIFO_END_ADDR_CHG_SUCCESS_get( b_ptr, h_ptr ) \
       _scpb_reg_field_FIFO_END_ADDR_CHG_SUCCESS_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_FIFO_END_ADDR_CHG_SUCCESS_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00103d0c bits 0) field FIFO_END_ADDR_CHG_SUCCESS of register PMC_SCPB_REG_REG_FIFO_CFG_STAT */
    reg_value = _scpb_reg_reg_FIFO_CFG_STAT_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    value = (reg_value & SCPB_REG_REG_FIFO_CFG_STAT_BIT_FIFO_END_ADDR_CHG_SUCCESS_MSK) >> SCPB_REG_REG_FIFO_CFG_STAT_BIT_FIFO_END_ADDR_CHG_SUCCESS_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_FIFO_END_ADDR_CHG_SUCCESS_get", value );

    return value;
}

#ifndef scpb_reg_field_FIFO_END_ADDR_CHG_SUCCESS_poll
#define scpb_reg_field_FIFO_END_ADDR_CHG_SUCCESS_poll( b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_field_FIFO_END_ADDR_CHG_SUCCESS_poll( LOG_CALLER b_ptr, h_ptr, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_field_FIFO_END_ADDR_CHG_SUCCESS_poll( LOG_CALLER_DEFN
                                                                                   scpb_reg_buffer_t *b_ptr,
                                                                                   cpb_handle_t *h_ptr,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "scpb_reg_field_FIFO_END_ADDR_CHG_SUCCESS_poll", value );

    /* (0x00103d0c bits 0) field FIFO_END_ADDR_CHG_SUCCESS of register PMC_SCPB_REG_REG_FIFO_CFG_STAT */
    return _scpb_reg_reg_FIFO_CFG_STAT_poll( LOG_CALLER_ARGS
                                             b_ptr,
                                             h_ptr,
                                             SCPB_REG_REG_FIFO_CFG_STAT_BIT_FIFO_END_ADDR_CHG_SUCCESS_MSK,
                                             (value<<SCPB_REG_REG_FIFO_CFG_STAT_BIT_FIFO_END_ADDR_CHG_SUCCESS_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}


#ifndef scpb_reg_field_DBG_RDATA_get
#define scpb_reg_field_DBG_RDATA_get( b_ptr, h_ptr ) \
       _scpb_reg_field_DBG_RDATA_get( LOG_CALLER b_ptr, h_ptr )
#endif
static INLINE UINT32 _scpb_reg_field_DBG_RDATA_get( LOG_CALLER_DEFN scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00103e04 bits 31:0) field DBG_RDATA of register PMC_SCPB_REG_REG_DBG_RDATA */
    reg_value = _scpb_reg_reg_DBG_RDATA_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    value = (reg_value & SCPB_REG_REG_DBG_RDATA_BIT_DBG_RDATA_MSK) >> SCPB_REG_REG_DBG_RDATA_BIT_DBG_RDATA_OFF;
    IOLOG( "%s -> 0x%08x", "scpb_reg_field_DBG_RDATA_get", value );

    return value;
}

#ifndef scpb_reg_field_range_DBG_RDATA_get
#define scpb_reg_field_range_DBG_RDATA_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_field_range_DBG_RDATA_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_DBG_RDATA_get( LOG_CALLER_DEFN
                                                          scpb_reg_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_DBG_RDATA_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_DBG_RDATA_get", stop_bit, 31 );
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
    /* (0x00103e04 bits 31:0) field DBG_RDATA of register PMC_SCPB_REG_REG_DBG_RDATA */
    reg_value = _scpb_reg_reg_DBG_RDATA_read( LOG_CALLER_ARGS b_ptr, h_ptr);
    field_value = (reg_value & SCPB_REG_REG_DBG_RDATA_BIT_DBG_RDATA_MSK)
                  >> SCPB_REG_REG_DBG_RDATA_BIT_DBG_RDATA_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_DBG_RDATA_BIT_DBG_RDATA_MSK, SCPB_REG_REG_DBG_RDATA_BIT_DBG_RDATA_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_DBG_RDATA_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status_N_burstsize4
 * ==================================================================================
 */

#ifndef scpb_reg_lfield_GPI_get
#define scpb_reg_lfield_GPI_get( b_ptr, h_ptr, value ) \
       _scpb_reg_lfield_GPI_get( LOG_CALLER b_ptr, h_ptr, value )
#endif
static INLINE void _scpb_reg_lfield_GPI_get( LOG_CALLER_DEFN
                                             scpb_reg_buffer_t *b_ptr,
                                             cpb_handle_t *h_ptr,
                                             UINT32 value[4] )
{
    IOLOG( "%s ", "scpb_reg_lfield_GPI_get");
    _scpb_reg_reg_GPI_array_burst_read( LOG_CALLER_ARGS
                                        b_ptr,
                                        h_ptr,
                                        0,
                                        4,
                                        (UINT32 *)value);
}


#ifndef scpb_reg_lfield_range_GPI_get
#define scpb_reg_lfield_range_GPI_get( b_ptr, h_ptr, start_bit, stop_bit ) \
       _scpb_reg_lfield_range_GPI_get( LOG_CALLER b_ptr, h_ptr, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_lfield_range_GPI_get( LOG_CALLER_DEFN
                                                     scpb_reg_buffer_t *b_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "scpb_reg_lfield_range_GPI_get", start_bit, stop_bit );
    }
    reg_value = _scpb_reg_reg_GPI_array_read( LOG_CALLER_ARGS
                                              b_ptr,
                                              h_ptr,
                                              word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "scpb_reg_lfield_range_GPI_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status_N_size150__M_size64
 * ==================================================================================
 */

#ifndef scpb_reg_field_IPT_BLOCK_ACTIVE_get
#define scpb_reg_field_IPT_BLOCK_ACTIVE_get( b_ptr, h_ptr, N, M ) \
       _scpb_reg_field_IPT_BLOCK_ACTIVE_get( LOG_CALLER b_ptr, h_ptr, N, M )
#endif
static INLINE UINT32 _scpb_reg_field_IPT_BLOCK_ACTIVE_get( LOG_CALLER_DEFN
                                                           scpb_reg_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32  M )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_IPT_BLOCK_ACTIVE_get", N, 149);
    if (M > 63)
        IO_RANGE_CHECK("%s M is %d but max is %d", "scpb_reg_field_IPT_BLOCK_ACTIVE_get", M, 63);
    /* ((0x00100000 + (N) * 0x100 + (M) * 0x4) bits 30) field IPT_BLOCK_ACTIVE of register PMC_SCPB_REG_REG_BLOCK_TABLE_REG index N=0..149 index M=0..63 */
    reg_value = _scpb_reg_reg_BLOCK_TABLE_REG_array_read( LOG_CALLER_ARGS
                                                          b_ptr,
                                                          h_ptr,
                                                          N,
                                                          M);
    value = (reg_value & SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_BLOCK_ACTIVE_MSK) >> SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_BLOCK_ACTIVE_OFF;
    IOLOG( "%s -> N=%d M=%d 0x%08x", "scpb_reg_field_IPT_BLOCK_ACTIVE_get", N, M, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status_N_size8
 * ==================================================================================
 */

#ifndef scpb_reg_field_WPTR_RST_PUSH_P0_VAL_get
#define scpb_reg_field_WPTR_RST_PUSH_P0_VAL_get( b_ptr, h_ptr, N ) \
       _scpb_reg_field_WPTR_RST_PUSH_P0_VAL_get( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_field_WPTR_RST_PUSH_P0_VAL_get( LOG_CALLER_DEFN
                                                               scpb_reg_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_WPTR_RST_PUSH_P0_VAL_get", N, 7);
    /* ((0x00100400 + (N) * 0x4) bits 31:0) field WPTR_RST_PUSH_P0_VAL of register PMC_SCPB_REG_REG_PUSH_PORT_P0_RST_VAL index N=0..7 */
    reg_value = _scpb_reg_reg_PUSH_PORT_P0_RST_VAL_array_read( LOG_CALLER_ARGS
                                                               b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & SCPB_REG_REG_PUSH_PORT_P0_RST_VAL_BIT_WPTR_RST_PUSH_P0_VAL_MSK) >> SCPB_REG_REG_PUSH_PORT_P0_RST_VAL_BIT_WPTR_RST_PUSH_P0_VAL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "scpb_reg_field_WPTR_RST_PUSH_P0_VAL_get", N, value );

    return value;
}

#ifndef scpb_reg_field_range_WPTR_RST_PUSH_P0_VAL_get
#define scpb_reg_field_range_WPTR_RST_PUSH_P0_VAL_get( b_ptr, h_ptr, N, start_bit, stop_bit ) \
       _scpb_reg_field_range_WPTR_RST_PUSH_P0_VAL_get( LOG_CALLER b_ptr, h_ptr, N, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_WPTR_RST_PUSH_P0_VAL_get( LOG_CALLER_DEFN
                                                                     scpb_reg_buffer_t *b_ptr,
                                                                     cpb_handle_t *h_ptr,
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

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_WPTR_RST_PUSH_P0_VAL_get", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_WPTR_RST_PUSH_P0_VAL_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_WPTR_RST_PUSH_P0_VAL_get", stop_bit, 31 );
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
    /* ((0x00100400 + (N) * 0x4) bits 31:0) field WPTR_RST_PUSH_P0_VAL of register PMC_SCPB_REG_REG_PUSH_PORT_P0_RST_VAL index N=0..7 */
    reg_value = _scpb_reg_reg_PUSH_PORT_P0_RST_VAL_array_read( LOG_CALLER_ARGS
                                                               b_ptr,
                                                               h_ptr,
                                                               N);
    field_value = (reg_value & SCPB_REG_REG_PUSH_PORT_P0_RST_VAL_BIT_WPTR_RST_PUSH_P0_VAL_MSK)
                  >> SCPB_REG_REG_PUSH_PORT_P0_RST_VAL_BIT_WPTR_RST_PUSH_P0_VAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_PUSH_PORT_P0_RST_VAL_BIT_WPTR_RST_PUSH_P0_VAL_MSK, SCPB_REG_REG_PUSH_PORT_P0_RST_VAL_BIT_WPTR_RST_PUSH_P0_VAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_WPTR_RST_PUSH_P0_VAL_get", N, start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_WPTR_RST_PUSH_P1_VAL_get
#define scpb_reg_field_WPTR_RST_PUSH_P1_VAL_get( b_ptr, h_ptr, N ) \
       _scpb_reg_field_WPTR_RST_PUSH_P1_VAL_get( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_field_WPTR_RST_PUSH_P1_VAL_get( LOG_CALLER_DEFN
                                                               scpb_reg_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_WPTR_RST_PUSH_P1_VAL_get", N, 7);
    /* ((0x00100c00 + (N) * 0x4) bits 31:0) field WPTR_RST_PUSH_P1_VAL of register PMC_SCPB_REG_REG_PUSH_PORT_P1_RST_VAL index N=0..7 */
    reg_value = _scpb_reg_reg_PUSH_PORT_P1_RST_VAL_array_read( LOG_CALLER_ARGS
                                                               b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & SCPB_REG_REG_PUSH_PORT_P1_RST_VAL_BIT_WPTR_RST_PUSH_P1_VAL_MSK) >> SCPB_REG_REG_PUSH_PORT_P1_RST_VAL_BIT_WPTR_RST_PUSH_P1_VAL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "scpb_reg_field_WPTR_RST_PUSH_P1_VAL_get", N, value );

    return value;
}

#ifndef scpb_reg_field_range_WPTR_RST_PUSH_P1_VAL_get
#define scpb_reg_field_range_WPTR_RST_PUSH_P1_VAL_get( b_ptr, h_ptr, N, start_bit, stop_bit ) \
       _scpb_reg_field_range_WPTR_RST_PUSH_P1_VAL_get( LOG_CALLER b_ptr, h_ptr, N, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_WPTR_RST_PUSH_P1_VAL_get( LOG_CALLER_DEFN
                                                                     scpb_reg_buffer_t *b_ptr,
                                                                     cpb_handle_t *h_ptr,
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

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_WPTR_RST_PUSH_P1_VAL_get", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_WPTR_RST_PUSH_P1_VAL_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_WPTR_RST_PUSH_P1_VAL_get", stop_bit, 31 );
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
    /* ((0x00100c00 + (N) * 0x4) bits 31:0) field WPTR_RST_PUSH_P1_VAL of register PMC_SCPB_REG_REG_PUSH_PORT_P1_RST_VAL index N=0..7 */
    reg_value = _scpb_reg_reg_PUSH_PORT_P1_RST_VAL_array_read( LOG_CALLER_ARGS
                                                               b_ptr,
                                                               h_ptr,
                                                               N);
    field_value = (reg_value & SCPB_REG_REG_PUSH_PORT_P1_RST_VAL_BIT_WPTR_RST_PUSH_P1_VAL_MSK)
                  >> SCPB_REG_REG_PUSH_PORT_P1_RST_VAL_BIT_WPTR_RST_PUSH_P1_VAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_PUSH_PORT_P1_RST_VAL_BIT_WPTR_RST_PUSH_P1_VAL_MSK, SCPB_REG_REG_PUSH_PORT_P1_RST_VAL_BIT_WPTR_RST_PUSH_P1_VAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_WPTR_RST_PUSH_P1_VAL_get", N, start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_WPTR_RST_PUSH_P2_VAL_get
#define scpb_reg_field_WPTR_RST_PUSH_P2_VAL_get( b_ptr, h_ptr, N ) \
       _scpb_reg_field_WPTR_RST_PUSH_P2_VAL_get( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_field_WPTR_RST_PUSH_P2_VAL_get( LOG_CALLER_DEFN
                                                               scpb_reg_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_WPTR_RST_PUSH_P2_VAL_get", N, 7);
    /* ((0x00101400 + (N) * 0x4) bits 31:0) field WPTR_RST_PUSH_P2_VAL of register PMC_SCPB_REG_REG_PUSH_PORT_P2_RST_VAL index N=0..7 */
    reg_value = _scpb_reg_reg_PUSH_PORT_P2_RST_VAL_array_read( LOG_CALLER_ARGS
                                                               b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & SCPB_REG_REG_PUSH_PORT_P2_RST_VAL_BIT_WPTR_RST_PUSH_P2_VAL_MSK) >> SCPB_REG_REG_PUSH_PORT_P2_RST_VAL_BIT_WPTR_RST_PUSH_P2_VAL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "scpb_reg_field_WPTR_RST_PUSH_P2_VAL_get", N, value );

    return value;
}

#ifndef scpb_reg_field_range_WPTR_RST_PUSH_P2_VAL_get
#define scpb_reg_field_range_WPTR_RST_PUSH_P2_VAL_get( b_ptr, h_ptr, N, start_bit, stop_bit ) \
       _scpb_reg_field_range_WPTR_RST_PUSH_P2_VAL_get( LOG_CALLER b_ptr, h_ptr, N, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_WPTR_RST_PUSH_P2_VAL_get( LOG_CALLER_DEFN
                                                                     scpb_reg_buffer_t *b_ptr,
                                                                     cpb_handle_t *h_ptr,
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

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_WPTR_RST_PUSH_P2_VAL_get", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_WPTR_RST_PUSH_P2_VAL_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_WPTR_RST_PUSH_P2_VAL_get", stop_bit, 31 );
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
    /* ((0x00101400 + (N) * 0x4) bits 31:0) field WPTR_RST_PUSH_P2_VAL of register PMC_SCPB_REG_REG_PUSH_PORT_P2_RST_VAL index N=0..7 */
    reg_value = _scpb_reg_reg_PUSH_PORT_P2_RST_VAL_array_read( LOG_CALLER_ARGS
                                                               b_ptr,
                                                               h_ptr,
                                                               N);
    field_value = (reg_value & SCPB_REG_REG_PUSH_PORT_P2_RST_VAL_BIT_WPTR_RST_PUSH_P2_VAL_MSK)
                  >> SCPB_REG_REG_PUSH_PORT_P2_RST_VAL_BIT_WPTR_RST_PUSH_P2_VAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_PUSH_PORT_P2_RST_VAL_BIT_WPTR_RST_PUSH_P2_VAL_MSK, SCPB_REG_REG_PUSH_PORT_P2_RST_VAL_BIT_WPTR_RST_PUSH_P2_VAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_WPTR_RST_PUSH_P2_VAL_get", N, start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_WPTR_RST_PUSH_P3_VAL_get
#define scpb_reg_field_WPTR_RST_PUSH_P3_VAL_get( b_ptr, h_ptr, N ) \
       _scpb_reg_field_WPTR_RST_PUSH_P3_VAL_get( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_field_WPTR_RST_PUSH_P3_VAL_get( LOG_CALLER_DEFN
                                                               scpb_reg_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_WPTR_RST_PUSH_P3_VAL_get", N, 7);
    /* ((0x00101c00 + (N) * 0x4) bits 31:0) field WPTR_RST_PUSH_P3_VAL of register PMC_SCPB_REG_REG_PUSH_PORT_P3_RST_VAL index N=0..7 */
    reg_value = _scpb_reg_reg_PUSH_PORT_P3_RST_VAL_array_read( LOG_CALLER_ARGS
                                                               b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & SCPB_REG_REG_PUSH_PORT_P3_RST_VAL_BIT_WPTR_RST_PUSH_P3_VAL_MSK) >> SCPB_REG_REG_PUSH_PORT_P3_RST_VAL_BIT_WPTR_RST_PUSH_P3_VAL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "scpb_reg_field_WPTR_RST_PUSH_P3_VAL_get", N, value );

    return value;
}

#ifndef scpb_reg_field_range_WPTR_RST_PUSH_P3_VAL_get
#define scpb_reg_field_range_WPTR_RST_PUSH_P3_VAL_get( b_ptr, h_ptr, N, start_bit, stop_bit ) \
       _scpb_reg_field_range_WPTR_RST_PUSH_P3_VAL_get( LOG_CALLER b_ptr, h_ptr, N, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_WPTR_RST_PUSH_P3_VAL_get( LOG_CALLER_DEFN
                                                                     scpb_reg_buffer_t *b_ptr,
                                                                     cpb_handle_t *h_ptr,
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

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_WPTR_RST_PUSH_P3_VAL_get", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_WPTR_RST_PUSH_P3_VAL_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_WPTR_RST_PUSH_P3_VAL_get", stop_bit, 31 );
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
    /* ((0x00101c00 + (N) * 0x4) bits 31:0) field WPTR_RST_PUSH_P3_VAL of register PMC_SCPB_REG_REG_PUSH_PORT_P3_RST_VAL index N=0..7 */
    reg_value = _scpb_reg_reg_PUSH_PORT_P3_RST_VAL_array_read( LOG_CALLER_ARGS
                                                               b_ptr,
                                                               h_ptr,
                                                               N);
    field_value = (reg_value & SCPB_REG_REG_PUSH_PORT_P3_RST_VAL_BIT_WPTR_RST_PUSH_P3_VAL_MSK)
                  >> SCPB_REG_REG_PUSH_PORT_P3_RST_VAL_BIT_WPTR_RST_PUSH_P3_VAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_PUSH_PORT_P3_RST_VAL_BIT_WPTR_RST_PUSH_P3_VAL_MSK, SCPB_REG_REG_PUSH_PORT_P3_RST_VAL_BIT_WPTR_RST_PUSH_P3_VAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_WPTR_RST_PUSH_P3_VAL_get", N, start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_RPTR_RST_POP_VAL_get
#define scpb_reg_field_RPTR_RST_POP_VAL_get( b_ptr, h_ptr, N ) \
       _scpb_reg_field_RPTR_RST_POP_VAL_get( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_field_RPTR_RST_POP_VAL_get( LOG_CALLER_DEFN
                                                           scpb_reg_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_RPTR_RST_POP_VAL_get", N, 7);
    /* ((0x00102400 + (N) * 0x4) bits 31:0) field RPTR_RST_POP_VAL of register PMC_SCPB_REG_REG_RPTR_RST_POP_VAL_REG index N=0..7 */
    reg_value = _scpb_reg_reg_RPTR_RST_POP_VAL_REG_array_read( LOG_CALLER_ARGS
                                                               b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & SCPB_REG_REG_RPTR_RST_POP_VAL_REG_BIT_RPTR_RST_POP_VAL_MSK) >> SCPB_REG_REG_RPTR_RST_POP_VAL_REG_BIT_RPTR_RST_POP_VAL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "scpb_reg_field_RPTR_RST_POP_VAL_get", N, value );

    return value;
}

#ifndef scpb_reg_field_range_RPTR_RST_POP_VAL_get
#define scpb_reg_field_range_RPTR_RST_POP_VAL_get( b_ptr, h_ptr, N, start_bit, stop_bit ) \
       _scpb_reg_field_range_RPTR_RST_POP_VAL_get( LOG_CALLER b_ptr, h_ptr, N, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_RPTR_RST_POP_VAL_get( LOG_CALLER_DEFN
                                                                 scpb_reg_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
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

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_RPTR_RST_POP_VAL_get", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_RPTR_RST_POP_VAL_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_RPTR_RST_POP_VAL_get", stop_bit, 31 );
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
    /* ((0x00102400 + (N) * 0x4) bits 31:0) field RPTR_RST_POP_VAL of register PMC_SCPB_REG_REG_RPTR_RST_POP_VAL_REG index N=0..7 */
    reg_value = _scpb_reg_reg_RPTR_RST_POP_VAL_REG_array_read( LOG_CALLER_ARGS
                                                               b_ptr,
                                                               h_ptr,
                                                               N);
    field_value = (reg_value & SCPB_REG_REG_RPTR_RST_POP_VAL_REG_BIT_RPTR_RST_POP_VAL_MSK)
                  >> SCPB_REG_REG_RPTR_RST_POP_VAL_REG_BIT_RPTR_RST_POP_VAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_RPTR_RST_POP_VAL_REG_BIT_RPTR_RST_POP_VAL_MSK, SCPB_REG_REG_RPTR_RST_POP_VAL_REG_BIT_RPTR_RST_POP_VAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_RPTR_RST_POP_VAL_get", N, start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_FIFO_PKT_EMPTY_get
#define scpb_reg_field_FIFO_PKT_EMPTY_get( b_ptr, h_ptr, N ) \
       _scpb_reg_field_FIFO_PKT_EMPTY_get( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_field_FIFO_PKT_EMPTY_get( LOG_CALLER_DEFN
                                                         scpb_reg_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_FIFO_PKT_EMPTY_get", N, 7);
    /* ((0x00102600 + (N) * 0x4) bits 31:0) field FIFO_PKT_EMPTY of register PMC_SCPB_REG_REG_FIFO_PTR_EMPTY_REG index N=0..7 */
    reg_value = _scpb_reg_reg_FIFO_PTR_EMPTY_REG_array_read( LOG_CALLER_ARGS
                                                             b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & SCPB_REG_REG_FIFO_PTR_EMPTY_REG_BIT_FIFO_PKT_EMPTY_MSK) >> SCPB_REG_REG_FIFO_PTR_EMPTY_REG_BIT_FIFO_PKT_EMPTY_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "scpb_reg_field_FIFO_PKT_EMPTY_get", N, value );

    return value;
}

#ifndef scpb_reg_field_range_FIFO_PKT_EMPTY_get
#define scpb_reg_field_range_FIFO_PKT_EMPTY_get( b_ptr, h_ptr, N, start_bit, stop_bit ) \
       _scpb_reg_field_range_FIFO_PKT_EMPTY_get( LOG_CALLER b_ptr, h_ptr, N, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_FIFO_PKT_EMPTY_get( LOG_CALLER_DEFN
                                                               scpb_reg_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
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

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_FIFO_PKT_EMPTY_get", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_FIFO_PKT_EMPTY_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_FIFO_PKT_EMPTY_get", stop_bit, 31 );
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
    /* ((0x00102600 + (N) * 0x4) bits 31:0) field FIFO_PKT_EMPTY of register PMC_SCPB_REG_REG_FIFO_PTR_EMPTY_REG index N=0..7 */
    reg_value = _scpb_reg_reg_FIFO_PTR_EMPTY_REG_array_read( LOG_CALLER_ARGS
                                                             b_ptr,
                                                             h_ptr,
                                                             N);
    field_value = (reg_value & SCPB_REG_REG_FIFO_PTR_EMPTY_REG_BIT_FIFO_PKT_EMPTY_MSK)
                  >> SCPB_REG_REG_FIFO_PTR_EMPTY_REG_BIT_FIFO_PKT_EMPTY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_FIFO_PTR_EMPTY_REG_BIT_FIFO_PKT_EMPTY_MSK, SCPB_REG_REG_FIFO_PTR_EMPTY_REG_BIT_FIFO_PKT_EMPTY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_FIFO_PKT_EMPTY_get", N, start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_range_FIFO_PKT_EMPTY_poll
#define scpb_reg_field_range_FIFO_PKT_EMPTY_poll( b_ptr, h_ptr, N, start_bit, stop_bit, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_field_range_FIFO_PKT_EMPTY_poll( LOG_CALLER b_ptr, h_ptr, N, start_bit, stop_bit, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_field_range_FIFO_PKT_EMPTY_poll( LOG_CALLER_DEFN
                                                                              scpb_reg_buffer_t *b_ptr,
                                                                              cpb_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_FIFO_PKT_EMPTY_poll", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_FIFO_PKT_EMPTY_poll", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_FIFO_PKT_EMPTY_poll", stop_bit, 31 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_FIFO_PKT_EMPTY_poll", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00102600 + (N) * 0x4) bits 31:0) field FIFO_PKT_EMPTY of register PMC_SCPB_REG_REG_FIFO_PTR_EMPTY_REG index N=0..7 */
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
        /* ((0x00102600 + (N) * 0x4) bits 31:0) field FIFO_PKT_EMPTY of register PMC_SCPB_REG_REG_FIFO_PTR_EMPTY_REG index N=0..7 */
        return _scpb_reg_reg_FIFO_PTR_EMPTY_REG_array_poll( LOG_CALLER_ARGS
                                                            b_ptr,
                                                            h_ptr,
                                                            N,
                                                            subfield_mask << (SCPB_REG_REG_FIFO_PTR_EMPTY_REG_BIT_FIFO_PKT_EMPTY_OFF + subfield_offset),
                                                            value << (SCPB_REG_REG_FIFO_PTR_EMPTY_REG_BIT_FIFO_PKT_EMPTY_OFF + subfield_offset),
                                                            cmp,
                                                            max_count,
                                                            num_failed_polls,
                                                            delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}


#ifndef scpb_reg_field_FIFO_PKT_EMPTY_poll
#define scpb_reg_field_FIFO_PKT_EMPTY_poll( b_ptr, h_ptr, N, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds ) \
       _scpb_reg_field_FIFO_PKT_EMPTY_poll( LOG_CALLER b_ptr, h_ptr, N, value, cmp, max_count, num_failed_polls, delay_between_polls_in_microseconds )
#endif
static INLINE PMC_POLL_RETURN_TYPE _scpb_reg_field_FIFO_PKT_EMPTY_poll( LOG_CALLER_DEFN
                                                                        scpb_reg_buffer_t *b_ptr,
                                                                        cpb_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "scpb_reg_field_FIFO_PKT_EMPTY_poll", N, value );

    /* ((0x00102600 + (N) * 0x4) bits 31:0) field FIFO_PKT_EMPTY of register PMC_SCPB_REG_REG_FIFO_PTR_EMPTY_REG index N=0..7 */
    return _scpb_reg_reg_FIFO_PTR_EMPTY_REG_array_poll( LOG_CALLER_ARGS
                                                        b_ptr,
                                                        h_ptr,
                                                        N,
                                                        SCPB_REG_REG_FIFO_PTR_EMPTY_REG_BIT_FIFO_PKT_EMPTY_MSK,
                                                        (value<<SCPB_REG_REG_FIFO_PTR_EMPTY_REG_BIT_FIFO_PKT_EMPTY_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}


#ifndef scpb_reg_field_FIFO_UDR_STATUS_get
#define scpb_reg_field_FIFO_UDR_STATUS_get( b_ptr, h_ptr, N ) \
       _scpb_reg_field_FIFO_UDR_STATUS_get( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_field_FIFO_UDR_STATUS_get( LOG_CALLER_DEFN
                                                          scpb_reg_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_FIFO_UDR_STATUS_get", N, 7);
    /* ((0x00103000 + (N) * 0x4) bits 31:0) bits 0:31 use field FIFO_UDR_STATUS of register PMC_SCPB_REG_REG_FIFO_UDR_STAT_REG_0 index N=0..7 */
    reg_value = _scpb_reg_reg_FIFO_UDR_STAT_REG_0_array_read( LOG_CALLER_ARGS
                                                              b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & SCPB_REG_REG_FIFO_UDR_STAT_REG_0_BIT_FIFO_UDR_STATUS_MSK) >> SCPB_REG_REG_FIFO_UDR_STAT_REG_0_BIT_FIFO_UDR_STATUS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "scpb_reg_field_FIFO_UDR_STATUS_get", N, value );

    return value;
}

#ifndef scpb_reg_field_range_FIFO_UDR_STATUS_get
#define scpb_reg_field_range_FIFO_UDR_STATUS_get( b_ptr, h_ptr, N, start_bit, stop_bit ) \
       _scpb_reg_field_range_FIFO_UDR_STATUS_get( LOG_CALLER b_ptr, h_ptr, N, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_FIFO_UDR_STATUS_get( LOG_CALLER_DEFN
                                                                scpb_reg_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
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

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_FIFO_UDR_STATUS_get", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_FIFO_UDR_STATUS_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_FIFO_UDR_STATUS_get", stop_bit, 31 );
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
    /* ((0x00103000 + (N) * 0x4) bits 31:0) bits 0:31 use field FIFO_UDR_STATUS of register PMC_SCPB_REG_REG_FIFO_UDR_STAT_REG_0 index N=0..7 */
    reg_value = _scpb_reg_reg_FIFO_UDR_STAT_REG_0_array_read( LOG_CALLER_ARGS
                                                              b_ptr,
                                                              h_ptr,
                                                              N);
    field_value = (reg_value & SCPB_REG_REG_FIFO_UDR_STAT_REG_0_BIT_FIFO_UDR_STATUS_MSK)
                  >> SCPB_REG_REG_FIFO_UDR_STAT_REG_0_BIT_FIFO_UDR_STATUS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_FIFO_UDR_STAT_REG_0_BIT_FIFO_UDR_STATUS_MSK, SCPB_REG_REG_FIFO_UDR_STAT_REG_0_BIT_FIFO_UDR_STATUS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_FIFO_UDR_STATUS_get", N, start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_P0_FIFO_OVR_STATUS_get
#define scpb_reg_field_P0_FIFO_OVR_STATUS_get( b_ptr, h_ptr, N ) \
       _scpb_reg_field_P0_FIFO_OVR_STATUS_get( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_field_P0_FIFO_OVR_STATUS_get( LOG_CALLER_DEFN
                                                             scpb_reg_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_P0_FIFO_OVR_STATUS_get", N, 7);
    /* ((0x00103100 + (N) * 0x4) bits 31:0) field P0_FIFO_OVR_STATUS of register PMC_SCPB_REG_REG_P0_FIFO_OVR_STAT_REG index N=0..7 */
    reg_value = _scpb_reg_reg_P0_FIFO_OVR_STAT_REG_array_read( LOG_CALLER_ARGS
                                                               b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & SCPB_REG_REG_P0_FIFO_OVR_STAT_REG_BIT_P0_FIFO_OVR_STATUS_MSK) >> SCPB_REG_REG_P0_FIFO_OVR_STAT_REG_BIT_P0_FIFO_OVR_STATUS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "scpb_reg_field_P0_FIFO_OVR_STATUS_get", N, value );

    return value;
}

#ifndef scpb_reg_field_range_P0_FIFO_OVR_STATUS_get
#define scpb_reg_field_range_P0_FIFO_OVR_STATUS_get( b_ptr, h_ptr, N, start_bit, stop_bit ) \
       _scpb_reg_field_range_P0_FIFO_OVR_STATUS_get( LOG_CALLER b_ptr, h_ptr, N, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_P0_FIFO_OVR_STATUS_get( LOG_CALLER_DEFN
                                                                   scpb_reg_buffer_t *b_ptr,
                                                                   cpb_handle_t *h_ptr,
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

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_P0_FIFO_OVR_STATUS_get", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_P0_FIFO_OVR_STATUS_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_P0_FIFO_OVR_STATUS_get", stop_bit, 31 );
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
    /* ((0x00103100 + (N) * 0x4) bits 31:0) field P0_FIFO_OVR_STATUS of register PMC_SCPB_REG_REG_P0_FIFO_OVR_STAT_REG index N=0..7 */
    reg_value = _scpb_reg_reg_P0_FIFO_OVR_STAT_REG_array_read( LOG_CALLER_ARGS
                                                               b_ptr,
                                                               h_ptr,
                                                               N);
    field_value = (reg_value & SCPB_REG_REG_P0_FIFO_OVR_STAT_REG_BIT_P0_FIFO_OVR_STATUS_MSK)
                  >> SCPB_REG_REG_P0_FIFO_OVR_STAT_REG_BIT_P0_FIFO_OVR_STATUS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_P0_FIFO_OVR_STAT_REG_BIT_P0_FIFO_OVR_STATUS_MSK, SCPB_REG_REG_P0_FIFO_OVR_STAT_REG_BIT_P0_FIFO_OVR_STATUS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_P0_FIFO_OVR_STATUS_get", N, start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_P1_FIFO_OVR_STATUS_get
#define scpb_reg_field_P1_FIFO_OVR_STATUS_get( b_ptr, h_ptr, N ) \
       _scpb_reg_field_P1_FIFO_OVR_STATUS_get( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_field_P1_FIFO_OVR_STATUS_get( LOG_CALLER_DEFN
                                                             scpb_reg_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_P1_FIFO_OVR_STATUS_get", N, 7);
    /* ((0x00103200 + (N) * 0x4) bits 31:0) field P1_FIFO_OVR_STATUS of register PMC_SCPB_REG_REG_P1_FIFO_OVR_STAT_REG index N=0..7 */
    reg_value = _scpb_reg_reg_P1_FIFO_OVR_STAT_REG_array_read( LOG_CALLER_ARGS
                                                               b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & SCPB_REG_REG_P1_FIFO_OVR_STAT_REG_BIT_P1_FIFO_OVR_STATUS_MSK) >> SCPB_REG_REG_P1_FIFO_OVR_STAT_REG_BIT_P1_FIFO_OVR_STATUS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "scpb_reg_field_P1_FIFO_OVR_STATUS_get", N, value );

    return value;
}

#ifndef scpb_reg_field_range_P1_FIFO_OVR_STATUS_get
#define scpb_reg_field_range_P1_FIFO_OVR_STATUS_get( b_ptr, h_ptr, N, start_bit, stop_bit ) \
       _scpb_reg_field_range_P1_FIFO_OVR_STATUS_get( LOG_CALLER b_ptr, h_ptr, N, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_P1_FIFO_OVR_STATUS_get( LOG_CALLER_DEFN
                                                                   scpb_reg_buffer_t *b_ptr,
                                                                   cpb_handle_t *h_ptr,
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

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_P1_FIFO_OVR_STATUS_get", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_P1_FIFO_OVR_STATUS_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_P1_FIFO_OVR_STATUS_get", stop_bit, 31 );
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
    /* ((0x00103200 + (N) * 0x4) bits 31:0) field P1_FIFO_OVR_STATUS of register PMC_SCPB_REG_REG_P1_FIFO_OVR_STAT_REG index N=0..7 */
    reg_value = _scpb_reg_reg_P1_FIFO_OVR_STAT_REG_array_read( LOG_CALLER_ARGS
                                                               b_ptr,
                                                               h_ptr,
                                                               N);
    field_value = (reg_value & SCPB_REG_REG_P1_FIFO_OVR_STAT_REG_BIT_P1_FIFO_OVR_STATUS_MSK)
                  >> SCPB_REG_REG_P1_FIFO_OVR_STAT_REG_BIT_P1_FIFO_OVR_STATUS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_P1_FIFO_OVR_STAT_REG_BIT_P1_FIFO_OVR_STATUS_MSK, SCPB_REG_REG_P1_FIFO_OVR_STAT_REG_BIT_P1_FIFO_OVR_STATUS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_P1_FIFO_OVR_STATUS_get", N, start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_P2_FIFO_OVR_STATUS_get
#define scpb_reg_field_P2_FIFO_OVR_STATUS_get( b_ptr, h_ptr, N ) \
       _scpb_reg_field_P2_FIFO_OVR_STATUS_get( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_field_P2_FIFO_OVR_STATUS_get( LOG_CALLER_DEFN
                                                             scpb_reg_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_P2_FIFO_OVR_STATUS_get", N, 7);
    /* ((0x00103400 + (N) * 0x4) bits 31:0) field P2_FIFO_OVR_STATUS of register PMC_SCPB_REG_REG_P2_FIFO_OVR_STAT_REG index N=0..7 */
    reg_value = _scpb_reg_reg_P2_FIFO_OVR_STAT_REG_array_read( LOG_CALLER_ARGS
                                                               b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & SCPB_REG_REG_P2_FIFO_OVR_STAT_REG_BIT_P2_FIFO_OVR_STATUS_MSK) >> SCPB_REG_REG_P2_FIFO_OVR_STAT_REG_BIT_P2_FIFO_OVR_STATUS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "scpb_reg_field_P2_FIFO_OVR_STATUS_get", N, value );

    return value;
}

#ifndef scpb_reg_field_range_P2_FIFO_OVR_STATUS_get
#define scpb_reg_field_range_P2_FIFO_OVR_STATUS_get( b_ptr, h_ptr, N, start_bit, stop_bit ) \
       _scpb_reg_field_range_P2_FIFO_OVR_STATUS_get( LOG_CALLER b_ptr, h_ptr, N, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_P2_FIFO_OVR_STATUS_get( LOG_CALLER_DEFN
                                                                   scpb_reg_buffer_t *b_ptr,
                                                                   cpb_handle_t *h_ptr,
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

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_P2_FIFO_OVR_STATUS_get", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_P2_FIFO_OVR_STATUS_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_P2_FIFO_OVR_STATUS_get", stop_bit, 31 );
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
    /* ((0x00103400 + (N) * 0x4) bits 31:0) field P2_FIFO_OVR_STATUS of register PMC_SCPB_REG_REG_P2_FIFO_OVR_STAT_REG index N=0..7 */
    reg_value = _scpb_reg_reg_P2_FIFO_OVR_STAT_REG_array_read( LOG_CALLER_ARGS
                                                               b_ptr,
                                                               h_ptr,
                                                               N);
    field_value = (reg_value & SCPB_REG_REG_P2_FIFO_OVR_STAT_REG_BIT_P2_FIFO_OVR_STATUS_MSK)
                  >> SCPB_REG_REG_P2_FIFO_OVR_STAT_REG_BIT_P2_FIFO_OVR_STATUS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_P2_FIFO_OVR_STAT_REG_BIT_P2_FIFO_OVR_STATUS_MSK, SCPB_REG_REG_P2_FIFO_OVR_STAT_REG_BIT_P2_FIFO_OVR_STATUS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_P2_FIFO_OVR_STATUS_get", N, start_bit, stop_bit, value );

    return value;
}

#ifndef scpb_reg_field_P3_FIFO_OVR_STATUS_get
#define scpb_reg_field_P3_FIFO_OVR_STATUS_get( b_ptr, h_ptr, N ) \
       _scpb_reg_field_P3_FIFO_OVR_STATUS_get( LOG_CALLER b_ptr, h_ptr, N )
#endif
static INLINE UINT32 _scpb_reg_field_P3_FIFO_OVR_STATUS_get( LOG_CALLER_DEFN
                                                             scpb_reg_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_P3_FIFO_OVR_STATUS_get", N, 7);
    /* ((0x00103500 + (N) * 0x4) bits 31:0) field P3_FIFO_OVR_STATUS of register PMC_SCPB_REG_REG_P3_FIFO_OVR_STAT_REG index N=0..7 */
    reg_value = _scpb_reg_reg_P3_FIFO_OVR_STAT_REG_array_read( LOG_CALLER_ARGS
                                                               b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & SCPB_REG_REG_P3_FIFO_OVR_STAT_REG_BIT_P3_FIFO_OVR_STATUS_MSK) >> SCPB_REG_REG_P3_FIFO_OVR_STAT_REG_BIT_P3_FIFO_OVR_STATUS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "scpb_reg_field_P3_FIFO_OVR_STATUS_get", N, value );

    return value;
}

#ifndef scpb_reg_field_range_P3_FIFO_OVR_STATUS_get
#define scpb_reg_field_range_P3_FIFO_OVR_STATUS_get( b_ptr, h_ptr, N, start_bit, stop_bit ) \
       _scpb_reg_field_range_P3_FIFO_OVR_STATUS_get( LOG_CALLER b_ptr, h_ptr, N, start_bit, stop_bit )
#endif
static INLINE UINT32 _scpb_reg_field_range_P3_FIFO_OVR_STATUS_get( LOG_CALLER_DEFN
                                                                   scpb_reg_buffer_t *b_ptr,
                                                                   cpb_handle_t *h_ptr,
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

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "scpb_reg_field_range_P3_FIFO_OVR_STATUS_get", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "scpb_reg_field_range_P3_FIFO_OVR_STATUS_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "scpb_reg_field_range_P3_FIFO_OVR_STATUS_get", stop_bit, 31 );
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
    /* ((0x00103500 + (N) * 0x4) bits 31:0) field P3_FIFO_OVR_STATUS of register PMC_SCPB_REG_REG_P3_FIFO_OVR_STAT_REG index N=0..7 */
    reg_value = _scpb_reg_reg_P3_FIFO_OVR_STAT_REG_array_read( LOG_CALLER_ARGS
                                                               b_ptr,
                                                               h_ptr,
                                                               N);
    field_value = (reg_value & SCPB_REG_REG_P3_FIFO_OVR_STAT_REG_BIT_P3_FIFO_OVR_STATUS_MSK)
                  >> SCPB_REG_REG_P3_FIFO_OVR_STAT_REG_BIT_P3_FIFO_OVR_STATUS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SCPB_REG_REG_P3_FIFO_OVR_STAT_REG_BIT_P3_FIFO_OVR_STATUS_MSK, SCPB_REG_REG_P3_FIFO_OVR_STAT_REG_BIT_P3_FIFO_OVR_STATUS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "scpb_reg_field_range_P3_FIFO_OVR_STATUS_get", N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SCPB_REG_IO_INLINE_H */
