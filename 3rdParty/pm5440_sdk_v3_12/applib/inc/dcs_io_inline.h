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
 *     and register accessor functions for the dcs block
 *****************************************************************************/
#ifndef _DCS_IO_INLINE_H
#define _DCS_IO_INLINE_H

#include "cpb_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "dcs_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DCS_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for dcs
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
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} dcs_buffer_t;
static INLINE void dcs_buffer_init( dcs_buffer_t *b_ptr,
                                    cpb_handle_t *h_ptr,
                                    UINT32 A ) ALWAYS_INLINE;
static INLINE void dcs_buffer_init( dcs_buffer_t *b_ptr,
                                    cpb_handle_t *h_ptr,
                                    UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x2000) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "dcs_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void dcs_buffer_flush( dcs_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void dcs_buffer_flush( dcs_buffer_t *b_ptr )
{
    IOLOG( "dcs_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 dcs_reg_read( dcs_buffer_t *b_ptr,
                                   cpb_handle_t *h_ptr,
                                   UINT32 A,
                                   UINT32 mem_type,
                                   UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 dcs_reg_read( dcs_buffer_t *b_ptr,
                                   cpb_handle_t *h_ptr,
                                   UINT32 A,
                                   UINT32 mem_type,
                                   UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->A)*0x2000),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (A)*0x2000) + reg);
    return value;
}
static INLINE void dcs_reg_write( dcs_buffer_t *b_ptr,
                                  cpb_handle_t *h_ptr,
                                  UINT32 A,
                                  UINT32 mem_type,
                                  UINT32 reg,
                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_write( dcs_buffer_t *b_ptr,
                                  cpb_handle_t *h_ptr,
                                  UINT32 A,
                                  UINT32 mem_type,
                                  UINT32 reg,
                                  UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->A)*0x2000),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x2000) + reg,
                         value);
}

static INLINE void dcs_field_set( dcs_buffer_t *b_ptr,
                                  cpb_handle_t *h_ptr,
                                  UINT32 A,
                                  UINT32 mem_type,
                                  UINT32 reg,
                                  UINT32 mask,
                                  UINT32 unused_mask,
                                  UINT32 ofs,
                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_set( dcs_buffer_t *b_ptr,
                                  cpb_handle_t *h_ptr,
                                  UINT32 A,
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
                          (b_ptr->base_address + (b_ptr->A)*0x2000),
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
                             (((pmc_handle_t *)h_ptr)->base_address + (A)*0x2000) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x2000) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void dcs_action_on_write_field_set( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mem_type,
                                                  UINT32 reg,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_action_on_write_field_set( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mem_type,
                                                  UINT32 reg,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_action_on_write_field_set( b_ptr->coalesce_handle,
                                         mem_type,
                                         (b_ptr->base_address + (b_ptr->A)*0x2000),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x2000) + reg,
                         value<<ofs);
}

static INLINE void dcs_burst_read( dcs_buffer_t *b_ptr,
                                   cpb_handle_t *h_ptr,
                                   UINT32 A,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 len,
                                   UINT32 *value ) ALWAYS_INLINE;
static INLINE void dcs_burst_read( dcs_buffer_t *b_ptr,
                                   cpb_handle_t *h_ptr,
                                   UINT32 A,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 len,
                                   UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_read( b_ptr->coalesce_handle,
                          mem_type,
                          (b_ptr->base_address + (b_ptr->A)*0x2000),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (A)*0x2000) + reg,
                          len,
                          value);
}

static INLINE void dcs_burst_write( dcs_buffer_t *b_ptr,
                                    cpb_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void dcs_burst_write( dcs_buffer_t *b_ptr,
                                    cpb_handle_t *h_ptr,
                                    UINT32 A,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_write( b_ptr->coalesce_handle,
                           mem_type,
                           (b_ptr->base_address + (b_ptr->A)*0x2000),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x2000) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE dcs_poll( dcs_buffer_t *b_ptr,
                                             cpb_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 mem_type,
                                             UINT32 reg,
                                             UINT32 mask,
                                             UINT32 value,
                                             PMC_POLL_COMPARISON_TYPE cmp,
                                             UINT32 max_count,
                                             UINT32 *num_failed_polls,
                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE dcs_poll( dcs_buffer_t *b_ptr,
                                             cpb_handle_t *h_ptr,
                                             UINT32 A,
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
                           (b_ptr->base_address + (b_ptr->A)*0x2000),
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
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x2000) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for dcs
 * ==================================================================================
 */

static INLINE void dcs_reg_CONFIG_write( dcs_buffer_t *b_ptr,
                                         cpb_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_CONFIG_write( dcs_buffer_t *b_ptr,
                                         cpb_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcs_reg_CONFIG_write", value );
    dcs_reg_write( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_CONFIG,
                   PMC_DCS_REG_CONFIG,
                   value);
}

static INLINE void dcs_reg_CONFIG_field_set( dcs_buffer_t *b_ptr,
                                             cpb_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 mask,
                                             UINT32 ofs,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_CONFIG_field_set( dcs_buffer_t *b_ptr,
                                             cpb_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 mask,
                                             UINT32 ofs,
                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "dcs_reg_CONFIG_field_set", A, mask, ofs, value );
    dcs_field_set( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_CONFIG,
                   PMC_DCS_REG_CONFIG,
                   mask,
                   PMC_DCS_REG_CONFIG_UNUSED_MASK,
                   ofs,
                   value);

}

static INLINE UINT32 dcs_reg_CONFIG_read( dcs_buffer_t *b_ptr,
                                          cpb_handle_t *h_ptr,
                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_reg_CONFIG_read( dcs_buffer_t *b_ptr,
                                          cpb_handle_t *h_ptr,
                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = dcs_reg_read( b_ptr,
                              h_ptr,
                              A,
                              MEM_TYPE_CONFIG,
                              PMC_DCS_REG_CONFIG);

    IOLOG( "%s -> 0x%08x; A=%d", "dcs_reg_CONFIG_read", reg_value, A);
    return reg_value;
}

static INLINE void dcs_reg_TRANSFER_MODE_write( dcs_buffer_t *b_ptr,
                                                cpb_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_TRANSFER_MODE_write( dcs_buffer_t *b_ptr,
                                                cpb_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcs_reg_TRANSFER_MODE_write", value );
    dcs_reg_write( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_CONFIG,
                   PMC_DCS_REG_TRANSFER_MODE,
                   value);
}

static INLINE void dcs_reg_TRANSFER_MODE_field_set( dcs_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_TRANSFER_MODE_field_set( dcs_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "dcs_reg_TRANSFER_MODE_field_set", A, mask, ofs, value );
    dcs_field_set( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_CONFIG,
                   PMC_DCS_REG_TRANSFER_MODE,
                   mask,
                   PMC_DCS_REG_TRANSFER_MODE_UNUSED_MASK,
                   ofs,
                   value);

}

static INLINE UINT32 dcs_reg_TRANSFER_MODE_read( dcs_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_reg_TRANSFER_MODE_read( dcs_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = dcs_reg_read( b_ptr,
                              h_ptr,
                              A,
                              MEM_TYPE_CONFIG,
                              PMC_DCS_REG_TRANSFER_MODE);

    IOLOG( "%s -> 0x%08x; A=%d", "dcs_reg_TRANSFER_MODE_read", reg_value, A);
    return reg_value;
}

static INLINE void dcs_reg_CREDIT_CONFIG_write( dcs_buffer_t *b_ptr,
                                                cpb_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_CREDIT_CONFIG_write( dcs_buffer_t *b_ptr,
                                                cpb_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcs_reg_CREDIT_CONFIG_write", value );
    dcs_reg_write( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_CONFIG,
                   PMC_DCS_REG_CREDIT_CONFIG,
                   value);
}

static INLINE void dcs_reg_CREDIT_CONFIG_field_set( dcs_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_CREDIT_CONFIG_field_set( dcs_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "dcs_reg_CREDIT_CONFIG_field_set", A, mask, ofs, value );
    dcs_field_set( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_CONFIG,
                   PMC_DCS_REG_CREDIT_CONFIG,
                   mask,
                   PMC_DCS_REG_CREDIT_CONFIG_UNUSED_MASK,
                   ofs,
                   value);

}

static INLINE UINT32 dcs_reg_CREDIT_CONFIG_read( dcs_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_reg_CREDIT_CONFIG_read( dcs_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = dcs_reg_read( b_ptr,
                              h_ptr,
                              A,
                              MEM_TYPE_CONFIG,
                              PMC_DCS_REG_CREDIT_CONFIG);

    IOLOG( "%s -> 0x%08x; A=%d", "dcs_reg_CREDIT_CONFIG_read", reg_value, A);
    return reg_value;
}

static INLINE void dcs_reg_QUEUE_OFFSET_CONFIG_write( dcs_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_QUEUE_OFFSET_CONFIG_write( dcs_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcs_reg_QUEUE_OFFSET_CONFIG_write", value );
    dcs_reg_write( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_CONFIG,
                   PMC_DCS_REG_QUEUE_OFFSET_CONFIG,
                   value);
}

static INLINE void dcs_reg_QUEUE_OFFSET_CONFIG_field_set( dcs_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_QUEUE_OFFSET_CONFIG_field_set( dcs_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "dcs_reg_QUEUE_OFFSET_CONFIG_field_set", A, mask, ofs, value );
    dcs_field_set( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_CONFIG,
                   PMC_DCS_REG_QUEUE_OFFSET_CONFIG,
                   mask,
                   PMC_DCS_REG_QUEUE_OFFSET_CONFIG_UNUSED_MASK,
                   ofs,
                   value);

}

static INLINE UINT32 dcs_reg_QUEUE_OFFSET_CONFIG_read( dcs_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_reg_QUEUE_OFFSET_CONFIG_read( dcs_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = dcs_reg_read( b_ptr,
                              h_ptr,
                              A,
                              MEM_TYPE_CONFIG,
                              PMC_DCS_REG_QUEUE_OFFSET_CONFIG);

    IOLOG( "%s -> 0x%08x; A=%d", "dcs_reg_QUEUE_OFFSET_CONFIG_read", reg_value, A);
    return reg_value;
}

static INLINE void dcs_reg_XOFF_MODE_write( dcs_buffer_t *b_ptr,
                                            cpb_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_XOFF_MODE_write( dcs_buffer_t *b_ptr,
                                            cpb_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcs_reg_XOFF_MODE_write", value );
    dcs_reg_write( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_CONFIG,
                   PMC_DCS_REG_XOFF_MODE,
                   value);
}

static INLINE void dcs_reg_XOFF_MODE_field_set( dcs_buffer_t *b_ptr,
                                                cpb_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_XOFF_MODE_field_set( dcs_buffer_t *b_ptr,
                                                cpb_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "dcs_reg_XOFF_MODE_field_set", A, mask, ofs, value );
    dcs_field_set( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_CONFIG,
                   PMC_DCS_REG_XOFF_MODE,
                   mask,
                   PMC_DCS_REG_XOFF_MODE_UNUSED_MASK,
                   ofs,
                   value);

}

static INLINE UINT32 dcs_reg_XOFF_MODE_read( dcs_buffer_t *b_ptr,
                                             cpb_handle_t *h_ptr,
                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_reg_XOFF_MODE_read( dcs_buffer_t *b_ptr,
                                             cpb_handle_t *h_ptr,
                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = dcs_reg_read( b_ptr,
                              h_ptr,
                              A,
                              MEM_TYPE_CONFIG,
                              PMC_DCS_REG_XOFF_MODE);

    IOLOG( "%s -> 0x%08x; A=%d", "dcs_reg_XOFF_MODE_read", reg_value, A);
    return reg_value;
}

static INLINE void dcs_reg_TOP_OF_PAGE_CONFIG_write( dcs_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_TOP_OF_PAGE_CONFIG_write( dcs_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcs_reg_TOP_OF_PAGE_CONFIG_write", value );
    dcs_reg_write( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_CONFIG,
                   PMC_DCS_REG_TOP_OF_PAGE_CONFIG,
                   value);
}

static INLINE void dcs_reg_TOP_OF_PAGE_CONFIG_field_set( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_TOP_OF_PAGE_CONFIG_field_set( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "dcs_reg_TOP_OF_PAGE_CONFIG_field_set", A, mask, ofs, value );
    dcs_field_set( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_CONFIG,
                   PMC_DCS_REG_TOP_OF_PAGE_CONFIG,
                   mask,
                   PMC_DCS_REG_TOP_OF_PAGE_CONFIG_UNUSED_MASK,
                   ofs,
                   value);

}

static INLINE UINT32 dcs_reg_TOP_OF_PAGE_CONFIG_read( dcs_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_reg_TOP_OF_PAGE_CONFIG_read( dcs_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = dcs_reg_read( b_ptr,
                              h_ptr,
                              A,
                              MEM_TYPE_CONFIG,
                              PMC_DCS_REG_TOP_OF_PAGE_CONFIG);

    IOLOG( "%s -> 0x%08x; A=%d", "dcs_reg_TOP_OF_PAGE_CONFIG_read", reg_value, A);
    return reg_value;
}

static INLINE void dcs_reg_QUEUE_RESET_write( dcs_buffer_t *b_ptr,
                                              cpb_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_QUEUE_RESET_write( dcs_buffer_t *b_ptr,
                                              cpb_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcs_reg_QUEUE_RESET_write", value );
    dcs_reg_write( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_CONFIG,
                   PMC_DCS_REG_QUEUE_RESET,
                   value);
}

static INLINE void dcs_reg_QUEUE_RESET_field_set( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_QUEUE_RESET_field_set( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "dcs_reg_QUEUE_RESET_field_set", A, mask, ofs, value );
    dcs_field_set( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_CONFIG,
                   PMC_DCS_REG_QUEUE_RESET,
                   mask,
                   PMC_DCS_REG_QUEUE_RESET_UNUSED_MASK,
                   ofs,
                   value);

}

static INLINE UINT32 dcs_reg_QUEUE_RESET_read( dcs_buffer_t *b_ptr,
                                               cpb_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_reg_QUEUE_RESET_read( dcs_buffer_t *b_ptr,
                                               cpb_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = dcs_reg_read( b_ptr,
                              h_ptr,
                              A,
                              MEM_TYPE_CONFIG,
                              PMC_DCS_REG_QUEUE_RESET);

    IOLOG( "%s -> 0x%08x; A=%d", "dcs_reg_QUEUE_RESET_read", reg_value, A);
    return reg_value;
}

static INLINE void dcs_reg_PAGE_SWAP_CTRL_write( dcs_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_PAGE_SWAP_CTRL_write( dcs_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcs_reg_PAGE_SWAP_CTRL_write", value );
    dcs_reg_write( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_CONFIG,
                   PMC_DCS_REG_PAGE_SWAP_CTRL,
                   value);
}

static INLINE void dcs_reg_PAGE_SWAP_CTRL_field_set( dcs_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_PAGE_SWAP_CTRL_field_set( dcs_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "dcs_reg_PAGE_SWAP_CTRL_field_set", A, mask, ofs, value );
    dcs_field_set( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_CONFIG,
                   PMC_DCS_REG_PAGE_SWAP_CTRL,
                   mask,
                   PMC_DCS_REG_PAGE_SWAP_CTRL_UNUSED_MASK,
                   ofs,
                   value);

}

static INLINE UINT32 dcs_reg_PAGE_SWAP_CTRL_read( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_reg_PAGE_SWAP_CTRL_read( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = dcs_reg_read( b_ptr,
                              h_ptr,
                              A,
                              MEM_TYPE_CONFIG,
                              PMC_DCS_REG_PAGE_SWAP_CTRL);

    IOLOG( "%s -> 0x%08x; A=%d", "dcs_reg_PAGE_SWAP_CTRL_read", reg_value, A);
    return reg_value;
}

static INLINE void dcs_reg_STANDBY_PAGE_END_CONFIG_write( dcs_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_STANDBY_PAGE_END_CONFIG_write( dcs_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcs_reg_STANDBY_PAGE_END_CONFIG_write", value );
    dcs_reg_write( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_STATUS,
                   PMC_DCS_REG_STANDBY_PAGE_END_CONFIG,
                   value);
}

static INLINE void dcs_reg_STANDBY_PAGE_END_CONFIG_field_set( dcs_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_STANDBY_PAGE_END_CONFIG_field_set( dcs_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "dcs_reg_STANDBY_PAGE_END_CONFIG_field_set", A, mask, ofs, value );
    dcs_field_set( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_STATUS,
                   PMC_DCS_REG_STANDBY_PAGE_END_CONFIG,
                   mask,
                   PMC_DCS_REG_STANDBY_PAGE_END_CONFIG_UNUSED_MASK,
                   ofs,
                   value);

}

static INLINE UINT32 dcs_reg_STANDBY_PAGE_END_CONFIG_read( dcs_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_reg_STANDBY_PAGE_END_CONFIG_read( dcs_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = dcs_reg_read( b_ptr,
                              h_ptr,
                              A,
                              MEM_TYPE_STATUS,
                              PMC_DCS_REG_STANDBY_PAGE_END_CONFIG);

    IOLOG( "%s -> 0x%08x; A=%d", "dcs_reg_STANDBY_PAGE_END_CONFIG_read", reg_value, A);
    return reg_value;
}

static INLINE void dcs_reg_STANDBY_CALENDAR_RAM_array_write( dcs_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_STANDBY_CALENDAR_RAM_array_write( dcs_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "dcs_reg_STANDBY_CALENDAR_RAM_array_write", value );
    dcs_reg_write( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_STATUS,
                   PMC_DCS_REG_STANDBY_CALENDAR_RAM(N),
                   value);
}

static INLINE void dcs_reg_STANDBY_CALENDAR_RAM_array_field_set( dcs_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_reg_STANDBY_CALENDAR_RAM_array_field_set( dcs_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "dcs_reg_STANDBY_CALENDAR_RAM_array_field_set", A, N, mask, ofs, value );
    dcs_field_set( b_ptr,
                   h_ptr,
                   A,
                   MEM_TYPE_STATUS,
                   PMC_DCS_REG_STANDBY_CALENDAR_RAM(N),
                   mask,
                   PMC_DCS_REG_STANDBY_CALENDAR_RAM_UNUSED_MASK,
                   ofs,
                   value);

}

static INLINE UINT32 dcs_reg_STANDBY_CALENDAR_RAM_array_read( dcs_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_reg_STANDBY_CALENDAR_RAM_array_read( dcs_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N )
{
    UINT32 reg_value;

    reg_value = dcs_reg_read( b_ptr,
                              h_ptr,
                              A,
                              MEM_TYPE_STATUS,
                              PMC_DCS_REG_STANDBY_CALENDAR_RAM(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "dcs_reg_STANDBY_CALENDAR_RAM_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE UINT32 dcs_reg_PAGE_SWAP_STAT_read( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_reg_PAGE_SWAP_STAT_read( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = dcs_reg_read( b_ptr,
                              h_ptr,
                              A,
                              MEM_TYPE_STATUS,
                              PMC_DCS_REG_PAGE_SWAP_STAT);

    IOLOG( "%s -> 0x%08x; A=%d", "dcs_reg_PAGE_SWAP_STAT_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE dcs_reg_PAGE_SWAP_STAT_poll( dcs_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE dcs_reg_PAGE_SWAP_STAT_poll( dcs_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "dcs_reg_PAGE_SWAP_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return dcs_poll( b_ptr,
                     h_ptr,
                     A,
                     MEM_TYPE_STATUS,
                     PMC_DCS_REG_PAGE_SWAP_STAT,
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
static INLINE void dcs_field_DCS_ENABLE_set( dcs_buffer_t *b_ptr,
                                             cpb_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_DCS_ENABLE_set( dcs_buffer_t *b_ptr,
                                             cpb_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_ENABLE_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_DCS_ENABLE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "dcs_field_DCS_ENABLE_set", A, value );

    /* (0x00060000 bits 0) field DCS_ENABLE of register PMC_DCS_REG_CONFIG */
    dcs_reg_CONFIG_field_set( b_ptr,
                              h_ptr,
                              A,
                              DCS_REG_CONFIG_BIT_DCS_ENABLE_MSK,
                              DCS_REG_CONFIG_BIT_DCS_ENABLE_OFF,
                              value);
}

static INLINE UINT32 dcs_field_DCS_ENABLE_get( dcs_buffer_t *b_ptr,
                                               cpb_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_DCS_ENABLE_get( dcs_buffer_t *b_ptr,
                                               cpb_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_ENABLE_get", A, 3);
    /* (0x00060000 bits 0) field DCS_ENABLE of register PMC_DCS_REG_CONFIG */
    reg_value = dcs_reg_CONFIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & DCS_REG_CONFIG_BIT_DCS_ENABLE_MSK) >> DCS_REG_CONFIG_BIT_DCS_ENABLE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "dcs_field_DCS_ENABLE_get", A, value );

    return value;
}
static INLINE void dcs_field_DCS_DISABLE_BURST_CREDIT_set( dcs_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_DCS_DISABLE_BURST_CREDIT_set( dcs_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_DISABLE_BURST_CREDIT_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_DCS_DISABLE_BURST_CREDIT_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "dcs_field_DCS_DISABLE_BURST_CREDIT_set", A, value );

    /* (0x00060004 bits 25) field DCS_DISABLE_BURST_CREDIT of register PMC_DCS_REG_TRANSFER_MODE */
    dcs_reg_TRANSFER_MODE_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     DCS_REG_TRANSFER_MODE_BIT_DCS_DISABLE_BURST_CREDIT_MSK,
                                     DCS_REG_TRANSFER_MODE_BIT_DCS_DISABLE_BURST_CREDIT_OFF,
                                     value);
}

static INLINE UINT32 dcs_field_DCS_DISABLE_BURST_CREDIT_get( dcs_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_DCS_DISABLE_BURST_CREDIT_get( dcs_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_DISABLE_BURST_CREDIT_get", A, 3);
    /* (0x00060004 bits 25) field DCS_DISABLE_BURST_CREDIT of register PMC_DCS_REG_TRANSFER_MODE */
    reg_value = dcs_reg_TRANSFER_MODE_read(  b_ptr, h_ptr, A);
    value = (reg_value & DCS_REG_TRANSFER_MODE_BIT_DCS_DISABLE_BURST_CREDIT_MSK) >> DCS_REG_TRANSFER_MODE_BIT_DCS_DISABLE_BURST_CREDIT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "dcs_field_DCS_DISABLE_BURST_CREDIT_get", A, value );

    return value;
}
static INLINE void dcs_field_DCS_INFINITE_QUANTUM_set( dcs_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_DCS_INFINITE_QUANTUM_set( dcs_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_INFINITE_QUANTUM_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_DCS_INFINITE_QUANTUM_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "dcs_field_DCS_INFINITE_QUANTUM_set", A, value );

    /* (0x00060004 bits 24) field DCS_INFINITE_QUANTUM of register PMC_DCS_REG_TRANSFER_MODE */
    dcs_reg_TRANSFER_MODE_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     DCS_REG_TRANSFER_MODE_BIT_DCS_INFINITE_QUANTUM_MSK,
                                     DCS_REG_TRANSFER_MODE_BIT_DCS_INFINITE_QUANTUM_OFF,
                                     value);
}

static INLINE UINT32 dcs_field_DCS_INFINITE_QUANTUM_get( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_DCS_INFINITE_QUANTUM_get( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_INFINITE_QUANTUM_get", A, 3);
    /* (0x00060004 bits 24) field DCS_INFINITE_QUANTUM of register PMC_DCS_REG_TRANSFER_MODE */
    reg_value = dcs_reg_TRANSFER_MODE_read(  b_ptr, h_ptr, A);
    value = (reg_value & DCS_REG_TRANSFER_MODE_BIT_DCS_INFINITE_QUANTUM_MSK) >> DCS_REG_TRANSFER_MODE_BIT_DCS_INFINITE_QUANTUM_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "dcs_field_DCS_INFINITE_QUANTUM_get", A, value );

    return value;
}
static INLINE void dcs_field_DCS_QUANTUM_MODE_set( dcs_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_DCS_QUANTUM_MODE_set( dcs_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_QUANTUM_MODE_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_DCS_QUANTUM_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "dcs_field_DCS_QUANTUM_MODE_set", A, value );

    /* (0x00060004 bits 16) field DCS_QUANTUM_MODE of register PMC_DCS_REG_TRANSFER_MODE */
    dcs_reg_TRANSFER_MODE_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     DCS_REG_TRANSFER_MODE_BIT_DCS_QUANTUM_MODE_MSK,
                                     DCS_REG_TRANSFER_MODE_BIT_DCS_QUANTUM_MODE_OFF,
                                     value);
}

static INLINE UINT32 dcs_field_DCS_QUANTUM_MODE_get( dcs_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_DCS_QUANTUM_MODE_get( dcs_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_QUANTUM_MODE_get", A, 3);
    /* (0x00060004 bits 16) field DCS_QUANTUM_MODE of register PMC_DCS_REG_TRANSFER_MODE */
    reg_value = dcs_reg_TRANSFER_MODE_read(  b_ptr, h_ptr, A);
    value = (reg_value & DCS_REG_TRANSFER_MODE_BIT_DCS_QUANTUM_MODE_MSK) >> DCS_REG_TRANSFER_MODE_BIT_DCS_QUANTUM_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "dcs_field_DCS_QUANTUM_MODE_get", A, value );

    return value;
}
static INLINE void dcs_field_DCS_BURST_MODE_set( dcs_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_DCS_BURST_MODE_set( dcs_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_BURST_MODE_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_DCS_BURST_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "dcs_field_DCS_BURST_MODE_set", A, value );

    /* (0x00060004 bits 8) field DCS_BURST_MODE of register PMC_DCS_REG_TRANSFER_MODE */
    dcs_reg_TRANSFER_MODE_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     DCS_REG_TRANSFER_MODE_BIT_DCS_BURST_MODE_MSK,
                                     DCS_REG_TRANSFER_MODE_BIT_DCS_BURST_MODE_OFF,
                                     value);
}

static INLINE UINT32 dcs_field_DCS_BURST_MODE_get( dcs_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_DCS_BURST_MODE_get( dcs_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_BURST_MODE_get", A, 3);
    /* (0x00060004 bits 8) field DCS_BURST_MODE of register PMC_DCS_REG_TRANSFER_MODE */
    reg_value = dcs_reg_TRANSFER_MODE_read(  b_ptr, h_ptr, A);
    value = (reg_value & DCS_REG_TRANSFER_MODE_BIT_DCS_BURST_MODE_MSK) >> DCS_REG_TRANSFER_MODE_BIT_DCS_BURST_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "dcs_field_DCS_BURST_MODE_get", A, value );

    return value;
}
static INLINE void dcs_field_DCS_XFER_MODE_set( dcs_buffer_t *b_ptr,
                                                cpb_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_DCS_XFER_MODE_set( dcs_buffer_t *b_ptr,
                                                cpb_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_XFER_MODE_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_DCS_XFER_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "dcs_field_DCS_XFER_MODE_set", A, value );

    /* (0x00060004 bits 0) field DCS_XFER_MODE of register PMC_DCS_REG_TRANSFER_MODE */
    dcs_reg_TRANSFER_MODE_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     DCS_REG_TRANSFER_MODE_BIT_DCS_XFER_MODE_MSK,
                                     DCS_REG_TRANSFER_MODE_BIT_DCS_XFER_MODE_OFF,
                                     value);
}

static INLINE UINT32 dcs_field_DCS_XFER_MODE_get( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_DCS_XFER_MODE_get( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_XFER_MODE_get", A, 3);
    /* (0x00060004 bits 0) field DCS_XFER_MODE of register PMC_DCS_REG_TRANSFER_MODE */
    reg_value = dcs_reg_TRANSFER_MODE_read(  b_ptr, h_ptr, A);
    value = (reg_value & DCS_REG_TRANSFER_MODE_BIT_DCS_XFER_MODE_MSK) >> DCS_REG_TRANSFER_MODE_BIT_DCS_XFER_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "dcs_field_DCS_XFER_MODE_get", A, value );

    return value;
}
static INLINE void dcs_field_DCS_BURST_SIZE_set( dcs_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_DCS_BURST_SIZE_set( dcs_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_BURST_SIZE_set", A, 3);
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_DCS_BURST_SIZE_set", value, 511);
    IOLOG( "%s <= A=%d 0x%08x", "dcs_field_DCS_BURST_SIZE_set", A, value );

    /* (0x00060008 bits 24:16) field DCS_BURST_SIZE of register PMC_DCS_REG_CREDIT_CONFIG */
    dcs_reg_CREDIT_CONFIG_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     DCS_REG_CREDIT_CONFIG_BIT_DCS_BURST_SIZE_MSK,
                                     DCS_REG_CREDIT_CONFIG_BIT_DCS_BURST_SIZE_OFF,
                                     value);
}

static INLINE UINT32 dcs_field_DCS_BURST_SIZE_get( dcs_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_DCS_BURST_SIZE_get( dcs_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_BURST_SIZE_get", A, 3);
    /* (0x00060008 bits 24:16) field DCS_BURST_SIZE of register PMC_DCS_REG_CREDIT_CONFIG */
    reg_value = dcs_reg_CREDIT_CONFIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & DCS_REG_CREDIT_CONFIG_BIT_DCS_BURST_SIZE_MSK) >> DCS_REG_CREDIT_CONFIG_BIT_DCS_BURST_SIZE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "dcs_field_DCS_BURST_SIZE_get", A, value );

    return value;
}
static INLINE void dcs_field_range_DCS_BURST_SIZE_set( dcs_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_range_DCS_BURST_SIZE_set( dcs_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_DCS_BURST_SIZE_set", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_DCS_BURST_SIZE_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_DCS_BURST_SIZE_set", stop_bit, 8 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_DCS_BURST_SIZE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00060008 bits 24:16) field DCS_BURST_SIZE of register PMC_DCS_REG_CREDIT_CONFIG */
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
        /* (0x00060008 bits 24:16) field DCS_BURST_SIZE of register PMC_DCS_REG_CREDIT_CONFIG */
        dcs_reg_CREDIT_CONFIG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         subfield_mask << (DCS_REG_CREDIT_CONFIG_BIT_DCS_BURST_SIZE_OFF + subfield_offset),
                                         DCS_REG_CREDIT_CONFIG_BIT_DCS_BURST_SIZE_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 dcs_field_range_DCS_BURST_SIZE_get( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_range_DCS_BURST_SIZE_get( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_DCS_BURST_SIZE_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_DCS_BURST_SIZE_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_DCS_BURST_SIZE_get", stop_bit, 8 );
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
    /* (0x00060008 bits 24:16) field DCS_BURST_SIZE of register PMC_DCS_REG_CREDIT_CONFIG */
    reg_value = dcs_reg_CREDIT_CONFIG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & DCS_REG_CREDIT_CONFIG_BIT_DCS_BURST_SIZE_MSK)
                  >> DCS_REG_CREDIT_CONFIG_BIT_DCS_BURST_SIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCS_REG_CREDIT_CONFIG_BIT_DCS_BURST_SIZE_MSK, DCS_REG_CREDIT_CONFIG_BIT_DCS_BURST_SIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_DCS_BURST_SIZE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcs_field_DCS_QUANTUM_SIZE_set( dcs_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_DCS_QUANTUM_SIZE_set( dcs_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_QUANTUM_SIZE_set", A, 3);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_DCS_QUANTUM_SIZE_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "dcs_field_DCS_QUANTUM_SIZE_set", A, value );

    /* (0x00060008 bits 15:0) field DCS_QUANTUM_SIZE of register PMC_DCS_REG_CREDIT_CONFIG */
    dcs_reg_CREDIT_CONFIG_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     DCS_REG_CREDIT_CONFIG_BIT_DCS_QUANTUM_SIZE_MSK,
                                     DCS_REG_CREDIT_CONFIG_BIT_DCS_QUANTUM_SIZE_OFF,
                                     value);
}

static INLINE UINT32 dcs_field_DCS_QUANTUM_SIZE_get( dcs_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_DCS_QUANTUM_SIZE_get( dcs_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_QUANTUM_SIZE_get", A, 3);
    /* (0x00060008 bits 15:0) field DCS_QUANTUM_SIZE of register PMC_DCS_REG_CREDIT_CONFIG */
    reg_value = dcs_reg_CREDIT_CONFIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & DCS_REG_CREDIT_CONFIG_BIT_DCS_QUANTUM_SIZE_MSK) >> DCS_REG_CREDIT_CONFIG_BIT_DCS_QUANTUM_SIZE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "dcs_field_DCS_QUANTUM_SIZE_get", A, value );

    return value;
}
static INLINE void dcs_field_range_DCS_QUANTUM_SIZE_set( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_range_DCS_QUANTUM_SIZE_set( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_DCS_QUANTUM_SIZE_set", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_DCS_QUANTUM_SIZE_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_DCS_QUANTUM_SIZE_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_DCS_QUANTUM_SIZE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00060008 bits 15:0) field DCS_QUANTUM_SIZE of register PMC_DCS_REG_CREDIT_CONFIG */
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
        /* (0x00060008 bits 15:0) field DCS_QUANTUM_SIZE of register PMC_DCS_REG_CREDIT_CONFIG */
        dcs_reg_CREDIT_CONFIG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         subfield_mask << (DCS_REG_CREDIT_CONFIG_BIT_DCS_QUANTUM_SIZE_OFF + subfield_offset),
                                         DCS_REG_CREDIT_CONFIG_BIT_DCS_QUANTUM_SIZE_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 dcs_field_range_DCS_QUANTUM_SIZE_get( dcs_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_range_DCS_QUANTUM_SIZE_get( dcs_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_DCS_QUANTUM_SIZE_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_DCS_QUANTUM_SIZE_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_DCS_QUANTUM_SIZE_get", stop_bit, 15 );
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
    /* (0x00060008 bits 15:0) field DCS_QUANTUM_SIZE of register PMC_DCS_REG_CREDIT_CONFIG */
    reg_value = dcs_reg_CREDIT_CONFIG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & DCS_REG_CREDIT_CONFIG_BIT_DCS_QUANTUM_SIZE_MSK)
                  >> DCS_REG_CREDIT_CONFIG_BIT_DCS_QUANTUM_SIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCS_REG_CREDIT_CONFIG_BIT_DCS_QUANTUM_SIZE_MSK, DCS_REG_CREDIT_CONFIG_BIT_DCS_QUANTUM_SIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_DCS_QUANTUM_SIZE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcs_field_DCS_QUEUE_OFFSET_set( dcs_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_DCS_QUEUE_OFFSET_set( dcs_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_QUEUE_OFFSET_set", A, 3);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_DCS_QUEUE_OFFSET_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "dcs_field_DCS_QUEUE_OFFSET_set", A, value );

    /* (0x0006000c bits 7:0) field DCS_QUEUE_OFFSET of register PMC_DCS_REG_QUEUE_OFFSET_CONFIG */
    dcs_reg_QUEUE_OFFSET_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           DCS_REG_QUEUE_OFFSET_CONFIG_BIT_DCS_QUEUE_OFFSET_MSK,
                                           DCS_REG_QUEUE_OFFSET_CONFIG_BIT_DCS_QUEUE_OFFSET_OFF,
                                           value);
}

static INLINE UINT32 dcs_field_DCS_QUEUE_OFFSET_get( dcs_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_DCS_QUEUE_OFFSET_get( dcs_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_QUEUE_OFFSET_get", A, 3);
    /* (0x0006000c bits 7:0) field DCS_QUEUE_OFFSET of register PMC_DCS_REG_QUEUE_OFFSET_CONFIG */
    reg_value = dcs_reg_QUEUE_OFFSET_CONFIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & DCS_REG_QUEUE_OFFSET_CONFIG_BIT_DCS_QUEUE_OFFSET_MSK) >> DCS_REG_QUEUE_OFFSET_CONFIG_BIT_DCS_QUEUE_OFFSET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "dcs_field_DCS_QUEUE_OFFSET_get", A, value );

    return value;
}
static INLINE void dcs_field_range_DCS_QUEUE_OFFSET_set( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_range_DCS_QUEUE_OFFSET_set( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_DCS_QUEUE_OFFSET_set", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_DCS_QUEUE_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_DCS_QUEUE_OFFSET_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_DCS_QUEUE_OFFSET_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0006000c bits 7:0) field DCS_QUEUE_OFFSET of register PMC_DCS_REG_QUEUE_OFFSET_CONFIG */
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
        /* (0x0006000c bits 7:0) field DCS_QUEUE_OFFSET of register PMC_DCS_REG_QUEUE_OFFSET_CONFIG */
        dcs_reg_QUEUE_OFFSET_CONFIG_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               subfield_mask << (DCS_REG_QUEUE_OFFSET_CONFIG_BIT_DCS_QUEUE_OFFSET_OFF + subfield_offset),
                                               DCS_REG_QUEUE_OFFSET_CONFIG_BIT_DCS_QUEUE_OFFSET_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 dcs_field_range_DCS_QUEUE_OFFSET_get( dcs_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_range_DCS_QUEUE_OFFSET_get( dcs_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_DCS_QUEUE_OFFSET_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_DCS_QUEUE_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_DCS_QUEUE_OFFSET_get", stop_bit, 7 );
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
    /* (0x0006000c bits 7:0) field DCS_QUEUE_OFFSET of register PMC_DCS_REG_QUEUE_OFFSET_CONFIG */
    reg_value = dcs_reg_QUEUE_OFFSET_CONFIG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & DCS_REG_QUEUE_OFFSET_CONFIG_BIT_DCS_QUEUE_OFFSET_MSK)
                  >> DCS_REG_QUEUE_OFFSET_CONFIG_BIT_DCS_QUEUE_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCS_REG_QUEUE_OFFSET_CONFIG_BIT_DCS_QUEUE_OFFSET_MSK, DCS_REG_QUEUE_OFFSET_CONFIG_BIT_DCS_QUEUE_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_DCS_QUEUE_OFFSET_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcs_field_DCS_LINK_XOFF_MODE_set( dcs_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_DCS_LINK_XOFF_MODE_set( dcs_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_LINK_XOFF_MODE_set", A, 3);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_DCS_LINK_XOFF_MODE_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "dcs_field_DCS_LINK_XOFF_MODE_set", A, value );

    /* (0x00060010 bits 17:16) field DCS_LINK_XOFF_MODE of register PMC_DCS_REG_XOFF_MODE */
    dcs_reg_XOFF_MODE_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 DCS_REG_XOFF_MODE_BIT_DCS_LINK_XOFF_MODE_MSK,
                                 DCS_REG_XOFF_MODE_BIT_DCS_LINK_XOFF_MODE_OFF,
                                 value);
}

static INLINE UINT32 dcs_field_DCS_LINK_XOFF_MODE_get( dcs_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_DCS_LINK_XOFF_MODE_get( dcs_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_LINK_XOFF_MODE_get", A, 3);
    /* (0x00060010 bits 17:16) field DCS_LINK_XOFF_MODE of register PMC_DCS_REG_XOFF_MODE */
    reg_value = dcs_reg_XOFF_MODE_read(  b_ptr, h_ptr, A);
    value = (reg_value & DCS_REG_XOFF_MODE_BIT_DCS_LINK_XOFF_MODE_MSK) >> DCS_REG_XOFF_MODE_BIT_DCS_LINK_XOFF_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "dcs_field_DCS_LINK_XOFF_MODE_get", A, value );

    return value;
}
static INLINE void dcs_field_range_DCS_LINK_XOFF_MODE_set( dcs_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_range_DCS_LINK_XOFF_MODE_set( dcs_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_DCS_LINK_XOFF_MODE_set", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_DCS_LINK_XOFF_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_DCS_LINK_XOFF_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_DCS_LINK_XOFF_MODE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00060010 bits 17:16) field DCS_LINK_XOFF_MODE of register PMC_DCS_REG_XOFF_MODE */
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
        /* (0x00060010 bits 17:16) field DCS_LINK_XOFF_MODE of register PMC_DCS_REG_XOFF_MODE */
        dcs_reg_XOFF_MODE_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     subfield_mask << (DCS_REG_XOFF_MODE_BIT_DCS_LINK_XOFF_MODE_OFF + subfield_offset),
                                     DCS_REG_XOFF_MODE_BIT_DCS_LINK_XOFF_MODE_OFF + subfield_offset,
                                     value >> subfield_shift);
    }
}

static INLINE UINT32 dcs_field_range_DCS_LINK_XOFF_MODE_get( dcs_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_range_DCS_LINK_XOFF_MODE_get( dcs_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_DCS_LINK_XOFF_MODE_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_DCS_LINK_XOFF_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_DCS_LINK_XOFF_MODE_get", stop_bit, 1 );
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
    /* (0x00060010 bits 17:16) field DCS_LINK_XOFF_MODE of register PMC_DCS_REG_XOFF_MODE */
    reg_value = dcs_reg_XOFF_MODE_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & DCS_REG_XOFF_MODE_BIT_DCS_LINK_XOFF_MODE_MSK)
                  >> DCS_REG_XOFF_MODE_BIT_DCS_LINK_XOFF_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCS_REG_XOFF_MODE_BIT_DCS_LINK_XOFF_MODE_MSK, DCS_REG_XOFF_MODE_BIT_DCS_LINK_XOFF_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_DCS_LINK_XOFF_MODE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcs_field_DCS_CH_XOFF_MODE_set( dcs_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_DCS_CH_XOFF_MODE_set( dcs_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_CH_XOFF_MODE_set", A, 3);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_DCS_CH_XOFF_MODE_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "dcs_field_DCS_CH_XOFF_MODE_set", A, value );

    /* (0x00060010 bits 1:0) field DCS_CH_XOFF_MODE of register PMC_DCS_REG_XOFF_MODE */
    dcs_reg_XOFF_MODE_field_set( b_ptr,
                                 h_ptr,
                                 A,
                                 DCS_REG_XOFF_MODE_BIT_DCS_CH_XOFF_MODE_MSK,
                                 DCS_REG_XOFF_MODE_BIT_DCS_CH_XOFF_MODE_OFF,
                                 value);
}

static INLINE UINT32 dcs_field_DCS_CH_XOFF_MODE_get( dcs_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_DCS_CH_XOFF_MODE_get( dcs_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_CH_XOFF_MODE_get", A, 3);
    /* (0x00060010 bits 1:0) field DCS_CH_XOFF_MODE of register PMC_DCS_REG_XOFF_MODE */
    reg_value = dcs_reg_XOFF_MODE_read(  b_ptr, h_ptr, A);
    value = (reg_value & DCS_REG_XOFF_MODE_BIT_DCS_CH_XOFF_MODE_MSK) >> DCS_REG_XOFF_MODE_BIT_DCS_CH_XOFF_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "dcs_field_DCS_CH_XOFF_MODE_get", A, value );

    return value;
}
static INLINE void dcs_field_range_DCS_CH_XOFF_MODE_set( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_range_DCS_CH_XOFF_MODE_set( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_DCS_CH_XOFF_MODE_set", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_DCS_CH_XOFF_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_DCS_CH_XOFF_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_DCS_CH_XOFF_MODE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00060010 bits 1:0) field DCS_CH_XOFF_MODE of register PMC_DCS_REG_XOFF_MODE */
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
        /* (0x00060010 bits 1:0) field DCS_CH_XOFF_MODE of register PMC_DCS_REG_XOFF_MODE */
        dcs_reg_XOFF_MODE_field_set( b_ptr,
                                     h_ptr,
                                     A,
                                     subfield_mask << (DCS_REG_XOFF_MODE_BIT_DCS_CH_XOFF_MODE_OFF + subfield_offset),
                                     DCS_REG_XOFF_MODE_BIT_DCS_CH_XOFF_MODE_OFF + subfield_offset,
                                     value >> subfield_shift);
    }
}

static INLINE UINT32 dcs_field_range_DCS_CH_XOFF_MODE_get( dcs_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_range_DCS_CH_XOFF_MODE_get( dcs_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_DCS_CH_XOFF_MODE_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_DCS_CH_XOFF_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_DCS_CH_XOFF_MODE_get", stop_bit, 1 );
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
    /* (0x00060010 bits 1:0) field DCS_CH_XOFF_MODE of register PMC_DCS_REG_XOFF_MODE */
    reg_value = dcs_reg_XOFF_MODE_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & DCS_REG_XOFF_MODE_BIT_DCS_CH_XOFF_MODE_MSK)
                  >> DCS_REG_XOFF_MODE_BIT_DCS_CH_XOFF_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCS_REG_XOFF_MODE_BIT_DCS_CH_XOFF_MODE_MSK, DCS_REG_XOFF_MODE_BIT_DCS_CH_XOFF_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_DCS_CH_XOFF_MODE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcs_field_DCS_TOP_OF_PAGE_PTR_set( dcs_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_DCS_TOP_OF_PAGE_PTR_set( dcs_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_TOP_OF_PAGE_PTR_set", A, 3);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_DCS_TOP_OF_PAGE_PTR_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "dcs_field_DCS_TOP_OF_PAGE_PTR_set", A, value );

    /* (0x0006001c bits 23:16) field DCS_TOP_OF_PAGE_PTR of register PMC_DCS_REG_TOP_OF_PAGE_CONFIG */
    dcs_reg_TOP_OF_PAGE_CONFIG_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_PTR_MSK,
                                          DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_PTR_OFF,
                                          value);
}

static INLINE UINT32 dcs_field_DCS_TOP_OF_PAGE_PTR_get( dcs_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_DCS_TOP_OF_PAGE_PTR_get( dcs_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_TOP_OF_PAGE_PTR_get", A, 3);
    /* (0x0006001c bits 23:16) field DCS_TOP_OF_PAGE_PTR of register PMC_DCS_REG_TOP_OF_PAGE_CONFIG */
    reg_value = dcs_reg_TOP_OF_PAGE_CONFIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_PTR_MSK) >> DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_PTR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "dcs_field_DCS_TOP_OF_PAGE_PTR_get", A, value );

    return value;
}
static INLINE void dcs_field_range_DCS_TOP_OF_PAGE_PTR_set( dcs_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_range_DCS_TOP_OF_PAGE_PTR_set( dcs_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_DCS_TOP_OF_PAGE_PTR_set", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_DCS_TOP_OF_PAGE_PTR_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_DCS_TOP_OF_PAGE_PTR_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_DCS_TOP_OF_PAGE_PTR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0006001c bits 23:16) field DCS_TOP_OF_PAGE_PTR of register PMC_DCS_REG_TOP_OF_PAGE_CONFIG */
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
        /* (0x0006001c bits 23:16) field DCS_TOP_OF_PAGE_PTR of register PMC_DCS_REG_TOP_OF_PAGE_CONFIG */
        dcs_reg_TOP_OF_PAGE_CONFIG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              subfield_mask << (DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_PTR_OFF + subfield_offset),
                                              DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_PTR_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 dcs_field_range_DCS_TOP_OF_PAGE_PTR_get( dcs_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_range_DCS_TOP_OF_PAGE_PTR_get( dcs_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 A,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_DCS_TOP_OF_PAGE_PTR_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_DCS_TOP_OF_PAGE_PTR_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_DCS_TOP_OF_PAGE_PTR_get", stop_bit, 7 );
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
    /* (0x0006001c bits 23:16) field DCS_TOP_OF_PAGE_PTR of register PMC_DCS_REG_TOP_OF_PAGE_CONFIG */
    reg_value = dcs_reg_TOP_OF_PAGE_CONFIG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_PTR_MSK)
                  >> DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_PTR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_PTR_MSK, DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_PTR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_DCS_TOP_OF_PAGE_PTR_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcs_field_DCS_TOP_OF_PAGE_MODE_set( dcs_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_DCS_TOP_OF_PAGE_MODE_set( dcs_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_TOP_OF_PAGE_MODE_set", A, 3);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_DCS_TOP_OF_PAGE_MODE_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "dcs_field_DCS_TOP_OF_PAGE_MODE_set", A, value );

    /* (0x0006001c bits 9:8) field DCS_TOP_OF_PAGE_MODE of register PMC_DCS_REG_TOP_OF_PAGE_CONFIG */
    dcs_reg_TOP_OF_PAGE_CONFIG_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_MODE_MSK,
                                          DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_MODE_OFF,
                                          value);
}

static INLINE UINT32 dcs_field_DCS_TOP_OF_PAGE_MODE_get( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_DCS_TOP_OF_PAGE_MODE_get( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_TOP_OF_PAGE_MODE_get", A, 3);
    /* (0x0006001c bits 9:8) field DCS_TOP_OF_PAGE_MODE of register PMC_DCS_REG_TOP_OF_PAGE_CONFIG */
    reg_value = dcs_reg_TOP_OF_PAGE_CONFIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_MODE_MSK) >> DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "dcs_field_DCS_TOP_OF_PAGE_MODE_get", A, value );

    return value;
}
static INLINE void dcs_field_range_DCS_TOP_OF_PAGE_MODE_set( dcs_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_range_DCS_TOP_OF_PAGE_MODE_set( dcs_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_DCS_TOP_OF_PAGE_MODE_set", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_DCS_TOP_OF_PAGE_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_DCS_TOP_OF_PAGE_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_DCS_TOP_OF_PAGE_MODE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0006001c bits 9:8) field DCS_TOP_OF_PAGE_MODE of register PMC_DCS_REG_TOP_OF_PAGE_CONFIG */
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
        /* (0x0006001c bits 9:8) field DCS_TOP_OF_PAGE_MODE of register PMC_DCS_REG_TOP_OF_PAGE_CONFIG */
        dcs_reg_TOP_OF_PAGE_CONFIG_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              subfield_mask << (DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_MODE_OFF + subfield_offset),
                                              DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_MODE_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 dcs_field_range_DCS_TOP_OF_PAGE_MODE_get( dcs_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_range_DCS_TOP_OF_PAGE_MODE_get( dcs_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
                                                               UINT32 A,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_DCS_TOP_OF_PAGE_MODE_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_DCS_TOP_OF_PAGE_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_DCS_TOP_OF_PAGE_MODE_get", stop_bit, 1 );
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
    /* (0x0006001c bits 9:8) field DCS_TOP_OF_PAGE_MODE of register PMC_DCS_REG_TOP_OF_PAGE_CONFIG */
    reg_value = dcs_reg_TOP_OF_PAGE_CONFIG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_MODE_MSK)
                  >> DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_MODE_MSK, DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_DCS_TOP_OF_PAGE_MODE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcs_field_DCS_Q_RESET_set( dcs_buffer_t *b_ptr,
                                              cpb_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_DCS_Q_RESET_set( dcs_buffer_t *b_ptr,
                                              cpb_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_Q_RESET_set", A, 3);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_DCS_Q_RESET_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "dcs_field_DCS_Q_RESET_set", A, value );

    /* (0x00060024 bits 7:0) field DCS_Q_RESET of register PMC_DCS_REG_QUEUE_RESET */
    dcs_reg_QUEUE_RESET_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   DCS_REG_QUEUE_RESET_BIT_DCS_Q_RESET_MSK,
                                   DCS_REG_QUEUE_RESET_BIT_DCS_Q_RESET_OFF,
                                   value);
}

static INLINE UINT32 dcs_field_DCS_Q_RESET_get( dcs_buffer_t *b_ptr,
                                                cpb_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_DCS_Q_RESET_get( dcs_buffer_t *b_ptr,
                                                cpb_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_Q_RESET_get", A, 3);
    /* (0x00060024 bits 7:0) field DCS_Q_RESET of register PMC_DCS_REG_QUEUE_RESET */
    reg_value = dcs_reg_QUEUE_RESET_read(  b_ptr, h_ptr, A);
    value = (reg_value & DCS_REG_QUEUE_RESET_BIT_DCS_Q_RESET_MSK) >> DCS_REG_QUEUE_RESET_BIT_DCS_Q_RESET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "dcs_field_DCS_Q_RESET_get", A, value );

    return value;
}
static INLINE void dcs_field_range_DCS_Q_RESET_set( dcs_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_range_DCS_Q_RESET_set( dcs_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_DCS_Q_RESET_set", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_DCS_Q_RESET_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_DCS_Q_RESET_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_DCS_Q_RESET_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00060024 bits 7:0) field DCS_Q_RESET of register PMC_DCS_REG_QUEUE_RESET */
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
        /* (0x00060024 bits 7:0) field DCS_Q_RESET of register PMC_DCS_REG_QUEUE_RESET */
        dcs_reg_QUEUE_RESET_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       subfield_mask << (DCS_REG_QUEUE_RESET_BIT_DCS_Q_RESET_OFF + subfield_offset),
                                       DCS_REG_QUEUE_RESET_BIT_DCS_Q_RESET_OFF + subfield_offset,
                                       value >> subfield_shift);
    }
}

static INLINE UINT32 dcs_field_range_DCS_Q_RESET_get( dcs_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_range_DCS_Q_RESET_get( dcs_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_DCS_Q_RESET_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_DCS_Q_RESET_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_DCS_Q_RESET_get", stop_bit, 7 );
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
    /* (0x00060024 bits 7:0) field DCS_Q_RESET of register PMC_DCS_REG_QUEUE_RESET */
    reg_value = dcs_reg_QUEUE_RESET_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & DCS_REG_QUEUE_RESET_BIT_DCS_Q_RESET_MSK)
                  >> DCS_REG_QUEUE_RESET_BIT_DCS_Q_RESET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCS_REG_QUEUE_RESET_BIT_DCS_Q_RESET_MSK, DCS_REG_QUEUE_RESET_BIT_DCS_Q_RESET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_DCS_Q_RESET_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void dcs_field_DCS_PAGE_SWAP_set( dcs_buffer_t *b_ptr,
                                                cpb_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_DCS_PAGE_SWAP_set( dcs_buffer_t *b_ptr,
                                                cpb_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_PAGE_SWAP_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_DCS_PAGE_SWAP_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "dcs_field_DCS_PAGE_SWAP_set", A, value );

    /* (0x00060014 bits 0) field DCS_PAGE_SWAP of register PMC_DCS_REG_PAGE_SWAP_CTRL */
    dcs_reg_PAGE_SWAP_CTRL_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      DCS_REG_PAGE_SWAP_CTRL_BIT_DCS_PAGE_SWAP_MSK,
                                      DCS_REG_PAGE_SWAP_CTRL_BIT_DCS_PAGE_SWAP_OFF,
                                      value);
}

static INLINE UINT32 dcs_field_DCS_PAGE_SWAP_get( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_DCS_PAGE_SWAP_get( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_PAGE_SWAP_get", A, 3);
    /* (0x00060014 bits 0) field DCS_PAGE_SWAP of register PMC_DCS_REG_PAGE_SWAP_CTRL */
    reg_value = dcs_reg_PAGE_SWAP_CTRL_read(  b_ptr, h_ptr, A);
    value = (reg_value & DCS_REG_PAGE_SWAP_CTRL_BIT_DCS_PAGE_SWAP_MSK) >> DCS_REG_PAGE_SWAP_CTRL_BIT_DCS_PAGE_SWAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "dcs_field_DCS_PAGE_SWAP_get", A, value );

    return value;
}
static INLINE void dcs_field_DCS_TOP_OF_PAGE_set( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_DCS_TOP_OF_PAGE_set( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_TOP_OF_PAGE_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_DCS_TOP_OF_PAGE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "dcs_field_DCS_TOP_OF_PAGE_set", A, value );

    /* (0x0006001c bits 0) field DCS_TOP_OF_PAGE of register PMC_DCS_REG_TOP_OF_PAGE_CONFIG */
    dcs_reg_TOP_OF_PAGE_CONFIG_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_MSK,
                                          DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_OFF,
                                          value);
}

static INLINE UINT32 dcs_field_DCS_TOP_OF_PAGE_get( dcs_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_DCS_TOP_OF_PAGE_get( dcs_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_TOP_OF_PAGE_get", A, 3);
    /* (0x0006001c bits 0) field DCS_TOP_OF_PAGE of register PMC_DCS_REG_TOP_OF_PAGE_CONFIG */
    reg_value = dcs_reg_TOP_OF_PAGE_CONFIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_MSK) >> DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "dcs_field_DCS_TOP_OF_PAGE_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram
 * ==================================================================================
 */
static INLINE void dcs_field_DCS_PAGE_END_PTR_set( dcs_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_DCS_PAGE_END_PTR_set( dcs_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_PAGE_END_PTR_set", A, 3);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_DCS_PAGE_END_PTR_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "dcs_field_DCS_PAGE_END_PTR_set", A, value );

    /* (0x00060020 bits 7:0) field DCS_PAGE_END_PTR of register PMC_DCS_REG_STANDBY_PAGE_END_CONFIG */
    dcs_reg_STANDBY_PAGE_END_CONFIG_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               DCS_REG_STANDBY_PAGE_END_CONFIG_BIT_DCS_PAGE_END_PTR_MSK,
                                               DCS_REG_STANDBY_PAGE_END_CONFIG_BIT_DCS_PAGE_END_PTR_OFF,
                                               value);
}

static INLINE UINT32 dcs_field_DCS_PAGE_END_PTR_get( dcs_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_DCS_PAGE_END_PTR_get( dcs_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_PAGE_END_PTR_get", A, 3);
    /* (0x00060020 bits 7:0) field DCS_PAGE_END_PTR of register PMC_DCS_REG_STANDBY_PAGE_END_CONFIG */
    reg_value = dcs_reg_STANDBY_PAGE_END_CONFIG_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & DCS_REG_STANDBY_PAGE_END_CONFIG_BIT_DCS_PAGE_END_PTR_MSK) >> DCS_REG_STANDBY_PAGE_END_CONFIG_BIT_DCS_PAGE_END_PTR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "dcs_field_DCS_PAGE_END_PTR_get", A, value );

    return value;
}
static INLINE void dcs_field_range_DCS_PAGE_END_PTR_set( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_range_DCS_PAGE_END_PTR_set( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_DCS_PAGE_END_PTR_set", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_DCS_PAGE_END_PTR_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_DCS_PAGE_END_PTR_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_DCS_PAGE_END_PTR_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00060020 bits 7:0) field DCS_PAGE_END_PTR of register PMC_DCS_REG_STANDBY_PAGE_END_CONFIG */
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
        /* (0x00060020 bits 7:0) field DCS_PAGE_END_PTR of register PMC_DCS_REG_STANDBY_PAGE_END_CONFIG */
        dcs_reg_STANDBY_PAGE_END_CONFIG_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (DCS_REG_STANDBY_PAGE_END_CONFIG_BIT_DCS_PAGE_END_PTR_OFF + subfield_offset),
                                                   DCS_REG_STANDBY_PAGE_END_CONFIG_BIT_DCS_PAGE_END_PTR_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 dcs_field_range_DCS_PAGE_END_PTR_get( dcs_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_range_DCS_PAGE_END_PTR_get( dcs_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_DCS_PAGE_END_PTR_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_DCS_PAGE_END_PTR_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_DCS_PAGE_END_PTR_get", stop_bit, 7 );
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
    /* (0x00060020 bits 7:0) field DCS_PAGE_END_PTR of register PMC_DCS_REG_STANDBY_PAGE_END_CONFIG */
    reg_value = dcs_reg_STANDBY_PAGE_END_CONFIG_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & DCS_REG_STANDBY_PAGE_END_CONFIG_BIT_DCS_PAGE_END_PTR_MSK)
                  >> DCS_REG_STANDBY_PAGE_END_CONFIG_BIT_DCS_PAGE_END_PTR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCS_REG_STANDBY_PAGE_END_CONFIG_BIT_DCS_PAGE_END_PTR_MSK, DCS_REG_STANDBY_PAGE_END_CONFIG_BIT_DCS_PAGE_END_PTR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_DCS_PAGE_END_PTR_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size96
 * ==================================================================================
 */
static INLINE void dcs_field_C_RAM_SF1_set( dcs_buffer_t *b_ptr,
                                            cpb_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_C_RAM_SF1_set( dcs_buffer_t *b_ptr,
                                            cpb_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32  N,
                                            UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_C_RAM_SF1_set", A, 3);
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "dcs_field_C_RAM_SF1_set", N, 95);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_C_RAM_SF1_set", value, 3);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "dcs_field_C_RAM_SF1_set", A, N, value );

    /* ((0x00060400 + (N) * 0x4) bits 25:24) field C_RAM_SF1 of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
    dcs_reg_STANDBY_CALENDAR_RAM_array_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  N,
                                                  DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF1_MSK,
                                                  DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF1_OFF,
                                                  value);
}

static INLINE UINT32 dcs_field_C_RAM_SF1_get( dcs_buffer_t *b_ptr,
                                              cpb_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_C_RAM_SF1_get( dcs_buffer_t *b_ptr,
                                              cpb_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_C_RAM_SF1_get", A, 3);
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "dcs_field_C_RAM_SF1_get", N, 95);
    /* ((0x00060400 + (N) * 0x4) bits 25:24) field C_RAM_SF1 of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
    reg_value = dcs_reg_STANDBY_CALENDAR_RAM_array_read( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         N);
    value = (reg_value & DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF1_MSK) >> DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF1_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "dcs_field_C_RAM_SF1_get", A, N, value );

    return value;
}
static INLINE void dcs_field_range_C_RAM_SF1_set( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_range_C_RAM_SF1_set( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_C_RAM_SF1_set", A, 3);
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "dcs_field_range_C_RAM_SF1_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_C_RAM_SF1_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_C_RAM_SF1_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_C_RAM_SF1_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00060400 + (N) * 0x4) bits 25:24) field C_RAM_SF1 of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
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
        /* ((0x00060400 + (N) * 0x4) bits 25:24) field C_RAM_SF1 of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
        dcs_reg_STANDBY_CALENDAR_RAM_array_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N,
                                                      subfield_mask << (DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF1_OFF + subfield_offset),
                                                      DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF1_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 dcs_field_range_C_RAM_SF1_get( dcs_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_range_C_RAM_SF1_get( dcs_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_C_RAM_SF1_get", A, 3);
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "dcs_field_range_C_RAM_SF1_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_C_RAM_SF1_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_C_RAM_SF1_get", stop_bit, 1 );
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
    /* ((0x00060400 + (N) * 0x4) bits 25:24) field C_RAM_SF1 of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
    reg_value = dcs_reg_STANDBY_CALENDAR_RAM_array_read( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         N);
    field_value = (reg_value & DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF1_MSK)
                  >> DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF1_MSK, DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_C_RAM_SF1_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcs_field_C_RAM_SF2_set( dcs_buffer_t *b_ptr,
                                            cpb_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_C_RAM_SF2_set( dcs_buffer_t *b_ptr,
                                            cpb_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32  N,
                                            UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_C_RAM_SF2_set", A, 3);
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "dcs_field_C_RAM_SF2_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_C_RAM_SF2_set", value, 255);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "dcs_field_C_RAM_SF2_set", A, N, value );

    /* ((0x00060400 + (N) * 0x4) bits 23:16) field C_RAM_SF2 of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
    dcs_reg_STANDBY_CALENDAR_RAM_array_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  N,
                                                  DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF2_MSK,
                                                  DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF2_OFF,
                                                  value);
}

static INLINE UINT32 dcs_field_C_RAM_SF2_get( dcs_buffer_t *b_ptr,
                                              cpb_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_C_RAM_SF2_get( dcs_buffer_t *b_ptr,
                                              cpb_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_C_RAM_SF2_get", A, 3);
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "dcs_field_C_RAM_SF2_get", N, 95);
    /* ((0x00060400 + (N) * 0x4) bits 23:16) field C_RAM_SF2 of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
    reg_value = dcs_reg_STANDBY_CALENDAR_RAM_array_read( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         N);
    value = (reg_value & DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF2_MSK) >> DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF2_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "dcs_field_C_RAM_SF2_get", A, N, value );

    return value;
}
static INLINE void dcs_field_range_C_RAM_SF2_set( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_range_C_RAM_SF2_set( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_C_RAM_SF2_set", A, 3);
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "dcs_field_range_C_RAM_SF2_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_C_RAM_SF2_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_C_RAM_SF2_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_C_RAM_SF2_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00060400 + (N) * 0x4) bits 23:16) field C_RAM_SF2 of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
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
        /* ((0x00060400 + (N) * 0x4) bits 23:16) field C_RAM_SF2 of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
        dcs_reg_STANDBY_CALENDAR_RAM_array_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N,
                                                      subfield_mask << (DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF2_OFF + subfield_offset),
                                                      DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF2_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 dcs_field_range_C_RAM_SF2_get( dcs_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_range_C_RAM_SF2_get( dcs_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_C_RAM_SF2_get", A, 3);
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "dcs_field_range_C_RAM_SF2_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_C_RAM_SF2_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_C_RAM_SF2_get", stop_bit, 7 );
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
    /* ((0x00060400 + (N) * 0x4) bits 23:16) field C_RAM_SF2 of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
    reg_value = dcs_reg_STANDBY_CALENDAR_RAM_array_read( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         N);
    field_value = (reg_value & DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF2_MSK)
                  >> DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF2_MSK, DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_C_RAM_SF2_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcs_field_C_RAM_CHANNEL_set( dcs_buffer_t *b_ptr,
                                                cpb_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_C_RAM_CHANNEL_set( dcs_buffer_t *b_ptr,
                                                cpb_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32  N,
                                                UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_C_RAM_CHANNEL_set", A, 3);
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "dcs_field_C_RAM_CHANNEL_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_C_RAM_CHANNEL_set", value, 255);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "dcs_field_C_RAM_CHANNEL_set", A, N, value );

    /* ((0x00060400 + (N) * 0x4) bits 15:8) field C_RAM_CHANNEL of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
    dcs_reg_STANDBY_CALENDAR_RAM_array_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  N,
                                                  DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_CHANNEL_MSK,
                                                  DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_CHANNEL_OFF,
                                                  value);
}

static INLINE UINT32 dcs_field_C_RAM_CHANNEL_get( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_C_RAM_CHANNEL_get( dcs_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_C_RAM_CHANNEL_get", A, 3);
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "dcs_field_C_RAM_CHANNEL_get", N, 95);
    /* ((0x00060400 + (N) * 0x4) bits 15:8) field C_RAM_CHANNEL of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
    reg_value = dcs_reg_STANDBY_CALENDAR_RAM_array_read( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         N);
    value = (reg_value & DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_CHANNEL_MSK) >> DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_CHANNEL_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "dcs_field_C_RAM_CHANNEL_get", A, N, value );

    return value;
}
static INLINE void dcs_field_range_C_RAM_CHANNEL_set( dcs_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_range_C_RAM_CHANNEL_set( dcs_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_C_RAM_CHANNEL_set", A, 3);
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "dcs_field_range_C_RAM_CHANNEL_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_C_RAM_CHANNEL_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_C_RAM_CHANNEL_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_C_RAM_CHANNEL_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00060400 + (N) * 0x4) bits 15:8) field C_RAM_CHANNEL of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
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
        /* ((0x00060400 + (N) * 0x4) bits 15:8) field C_RAM_CHANNEL of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
        dcs_reg_STANDBY_CALENDAR_RAM_array_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N,
                                                      subfield_mask << (DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_CHANNEL_OFF + subfield_offset),
                                                      DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_CHANNEL_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 dcs_field_range_C_RAM_CHANNEL_get( dcs_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_range_C_RAM_CHANNEL_get( dcs_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_C_RAM_CHANNEL_get", A, 3);
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "dcs_field_range_C_RAM_CHANNEL_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_C_RAM_CHANNEL_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_C_RAM_CHANNEL_get", stop_bit, 7 );
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
    /* ((0x00060400 + (N) * 0x4) bits 15:8) field C_RAM_CHANNEL of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
    reg_value = dcs_reg_STANDBY_CALENDAR_RAM_array_read( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         N);
    field_value = (reg_value & DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_CHANNEL_MSK)
                  >> DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_CHANNEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_CHANNEL_MSK, DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_CHANNEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_C_RAM_CHANNEL_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void dcs_field_C_RAM_IDENTIFIER_set( dcs_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_C_RAM_IDENTIFIER_set( dcs_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_C_RAM_IDENTIFIER_set", A, 3);
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "dcs_field_C_RAM_IDENTIFIER_set", N, 95);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "dcs_field_C_RAM_IDENTIFIER_set", value, 127);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "dcs_field_C_RAM_IDENTIFIER_set", A, N, value );

    /* ((0x00060400 + (N) * 0x4) bits 6:0) field C_RAM_IDENTIFIER of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
    dcs_reg_STANDBY_CALENDAR_RAM_array_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  N,
                                                  DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_IDENTIFIER_MSK,
                                                  DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_IDENTIFIER_OFF,
                                                  value);
}

static INLINE UINT32 dcs_field_C_RAM_IDENTIFIER_get( dcs_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_C_RAM_IDENTIFIER_get( dcs_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_C_RAM_IDENTIFIER_get", A, 3);
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "dcs_field_C_RAM_IDENTIFIER_get", N, 95);
    /* ((0x00060400 + (N) * 0x4) bits 6:0) field C_RAM_IDENTIFIER of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
    reg_value = dcs_reg_STANDBY_CALENDAR_RAM_array_read( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         N);
    value = (reg_value & DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_IDENTIFIER_MSK) >> DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_IDENTIFIER_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "dcs_field_C_RAM_IDENTIFIER_get", A, N, value );

    return value;
}
static INLINE void dcs_field_range_C_RAM_IDENTIFIER_set( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void dcs_field_range_C_RAM_IDENTIFIER_set( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_C_RAM_IDENTIFIER_set", A, 3);
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "dcs_field_range_C_RAM_IDENTIFIER_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_C_RAM_IDENTIFIER_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_C_RAM_IDENTIFIER_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_C_RAM_IDENTIFIER_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00060400 + (N) * 0x4) bits 6:0) field C_RAM_IDENTIFIER of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
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
        /* ((0x00060400 + (N) * 0x4) bits 6:0) field C_RAM_IDENTIFIER of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
        dcs_reg_STANDBY_CALENDAR_RAM_array_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N,
                                                      subfield_mask << (DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_IDENTIFIER_OFF + subfield_offset),
                                                      DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_IDENTIFIER_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 dcs_field_range_C_RAM_IDENTIFIER_get( dcs_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_range_C_RAM_IDENTIFIER_get( dcs_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_range_C_RAM_IDENTIFIER_get", A, 3);
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "dcs_field_range_C_RAM_IDENTIFIER_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "dcs_field_range_C_RAM_IDENTIFIER_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "dcs_field_range_C_RAM_IDENTIFIER_get", stop_bit, 6 );
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
    /* ((0x00060400 + (N) * 0x4) bits 6:0) field C_RAM_IDENTIFIER of register PMC_DCS_REG_STANDBY_CALENDAR_RAM index N=0..95 */
    reg_value = dcs_reg_STANDBY_CALENDAR_RAM_array_read( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         N);
    field_value = (reg_value & DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_IDENTIFIER_MSK)
                  >> DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_IDENTIFIER_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_IDENTIFIER_MSK, DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_IDENTIFIER_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "dcs_field_range_C_RAM_IDENTIFIER_get", A, N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 dcs_field_DCS_PAGE_SWAP_STATUS_get( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 dcs_field_DCS_PAGE_SWAP_STATUS_get( dcs_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "dcs_field_DCS_PAGE_SWAP_STATUS_get", A, 3);
    /* (0x00060018 bits 0) field DCS_PAGE_SWAP_STATUS of register PMC_DCS_REG_PAGE_SWAP_STAT */
    reg_value = dcs_reg_PAGE_SWAP_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & DCS_REG_PAGE_SWAP_STAT_BIT_DCS_PAGE_SWAP_STATUS_MSK) >> DCS_REG_PAGE_SWAP_STAT_BIT_DCS_PAGE_SWAP_STATUS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "dcs_field_DCS_PAGE_SWAP_STATUS_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE dcs_field_DCS_PAGE_SWAP_STATUS_poll( dcs_buffer_t *b_ptr,
                                                                        cpb_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE dcs_field_DCS_PAGE_SWAP_STATUS_poll( dcs_buffer_t *b_ptr,
                                                                        cpb_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "dcs_field_DCS_PAGE_SWAP_STATUS_poll", A, value );

    /* (0x00060018 bits 0) field DCS_PAGE_SWAP_STATUS of register PMC_DCS_REG_PAGE_SWAP_STAT */
    return dcs_reg_PAGE_SWAP_STAT_poll( b_ptr,
                                        h_ptr,
                                        A,
                                        DCS_REG_PAGE_SWAP_STAT_BIT_DCS_PAGE_SWAP_STATUS_MSK,
                                        (value<<DCS_REG_PAGE_SWAP_STAT_BIT_DCS_PAGE_SWAP_STATUS_OFF),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DCS_IO_INLINE_H */
