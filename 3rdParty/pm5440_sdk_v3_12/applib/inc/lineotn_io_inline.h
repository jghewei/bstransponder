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
 *     and register accessor functions for the lineotn block
 *****************************************************************************/
#ifndef _LINEOTN_IO_INLINE_H
#define _LINEOTN_IO_INLINE_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "lineotn_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define LINEOTN_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for lineotn
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
    lineotn_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} lineotn_buffer_t;
static INLINE void lineotn_buffer_init( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void lineotn_buffer_init( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "lineotn_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void lineotn_buffer_flush( lineotn_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void lineotn_buffer_flush( lineotn_buffer_t *b_ptr )
{
    IOLOG( "lineotn_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 lineotn_reg_read( lineotn_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 lineotn_reg_read( lineotn_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
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
static INLINE void lineotn_reg_write( lineotn_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_reg_write( lineotn_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
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

static INLINE void lineotn_field_set( lineotn_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 mask,
                                      UINT32 unused_mask,
                                      UINT32 ofs,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_field_set( lineotn_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
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

static INLINE void lineotn_action_on_write_field_set( lineotn_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_action_on_write_field_set( lineotn_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
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

static INLINE void lineotn_burst_read( lineotn_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void lineotn_burst_read( lineotn_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
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

static INLINE void lineotn_burst_write( lineotn_buffer_t *b_ptr,
                                        lineotn_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void lineotn_burst_write( lineotn_buffer_t *b_ptr,
                                        lineotn_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE lineotn_poll( lineotn_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 mask,
                                                 UINT32 value,
                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                 UINT32 max_count,
                                                 UINT32 *num_failed_polls,
                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE lineotn_poll( lineotn_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
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
 *  register access functions for lineotn
 * ==================================================================================
 */

static INLINE void lineotn_reg_MASTER_CONFIG_write( lineotn_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_reg_MASTER_CONFIG_write( lineotn_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "lineotn_reg_MASTER_CONFIG_write", value );
    lineotn_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_LINEOTN_REG_MASTER_CONFIG,
                       value);
}

static INLINE void lineotn_reg_MASTER_CONFIG_field_set( lineotn_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_reg_MASTER_CONFIG_field_set( lineotn_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "lineotn_reg_MASTER_CONFIG_field_set", mask, ofs, value );
    lineotn_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_LINEOTN_REG_MASTER_CONFIG,
                       mask,
                       PMC_LINEOTN_REG_MASTER_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 lineotn_reg_MASTER_CONFIG_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_reg_MASTER_CONFIG_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = lineotn_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_LINEOTN_REG_MASTER_CONFIG);

    IOLOG( "%s -> 0x%08x;", "lineotn_reg_MASTER_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void lineotn_reg_INT0_EN_write( lineotn_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_reg_INT0_EN_write( lineotn_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "lineotn_reg_INT0_EN_write", value );
    lineotn_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_LINEOTN_REG_INT0_EN,
                       value);
}

static INLINE void lineotn_reg_INT0_EN_field_set( lineotn_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_reg_INT0_EN_field_set( lineotn_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "lineotn_reg_INT0_EN_field_set", mask, ofs, value );
    lineotn_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_LINEOTN_REG_INT0_EN,
                       mask,
                       PMC_LINEOTN_REG_INT0_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 lineotn_reg_INT0_EN_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_reg_INT0_EN_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = lineotn_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_LINEOTN_REG_INT0_EN);

    IOLOG( "%s -> 0x%08x;", "lineotn_reg_INT0_EN_read", reg_value);
    return reg_value;
}

static INLINE void lineotn_reg_INT1_EN_write( lineotn_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_reg_INT1_EN_write( lineotn_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "lineotn_reg_INT1_EN_write", value );
    lineotn_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_LINEOTN_REG_INT1_EN,
                       value);
}

static INLINE void lineotn_reg_INT1_EN_field_set( lineotn_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_reg_INT1_EN_field_set( lineotn_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "lineotn_reg_INT1_EN_field_set", mask, ofs, value );
    lineotn_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_LINEOTN_REG_INT1_EN,
                       mask,
                       PMC_LINEOTN_REG_INT1_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 lineotn_reg_INT1_EN_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_reg_INT1_EN_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = lineotn_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_LINEOTN_REG_INT1_EN);

    IOLOG( "%s -> 0x%08x;", "lineotn_reg_INT1_EN_read", reg_value);
    return reg_value;
}

static INLINE void lineotn_reg_INT2_EN_write( lineotn_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_reg_INT2_EN_write( lineotn_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "lineotn_reg_INT2_EN_write", value );
    lineotn_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_LINEOTN_REG_INT2_EN,
                       value);
}

static INLINE void lineotn_reg_INT2_EN_field_set( lineotn_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_reg_INT2_EN_field_set( lineotn_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "lineotn_reg_INT2_EN_field_set", mask, ofs, value );
    lineotn_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_LINEOTN_REG_INT2_EN,
                       mask,
                       PMC_LINEOTN_REG_INT2_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 lineotn_reg_INT2_EN_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_reg_INT2_EN_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = lineotn_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_LINEOTN_REG_INT2_EN);

    IOLOG( "%s -> 0x%08x;", "lineotn_reg_INT2_EN_read", reg_value);
    return reg_value;
}

static INLINE void lineotn_reg_INT3_EN_write( lineotn_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_reg_INT3_EN_write( lineotn_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "lineotn_reg_INT3_EN_write", value );
    lineotn_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_LINEOTN_REG_INT3_EN,
                       value);
}

static INLINE void lineotn_reg_INT3_EN_field_set( lineotn_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_reg_INT3_EN_field_set( lineotn_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "lineotn_reg_INT3_EN_field_set", mask, ofs, value );
    lineotn_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_LINEOTN_REG_INT3_EN,
                       mask,
                       PMC_LINEOTN_REG_INT3_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 lineotn_reg_INT3_EN_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_reg_INT3_EN_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = lineotn_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_LINEOTN_REG_INT3_EN);

    IOLOG( "%s -> 0x%08x;", "lineotn_reg_INT3_EN_read", reg_value);
    return reg_value;
}

static INLINE UINT32 lineotn_reg_INT0_SUMMARY_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_reg_INT0_SUMMARY_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = lineotn_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_LINEOTN_REG_INT0_SUMMARY);

    IOLOG( "%s -> 0x%08x;", "lineotn_reg_INT0_SUMMARY_read", reg_value);
    return reg_value;
}

static INLINE UINT32 lineotn_reg_INT1_SUMMARY_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_reg_INT1_SUMMARY_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = lineotn_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_LINEOTN_REG_INT1_SUMMARY);

    IOLOG( "%s -> 0x%08x;", "lineotn_reg_INT1_SUMMARY_read", reg_value);
    return reg_value;
}

static INLINE UINT32 lineotn_reg_INT2_SUMMARY_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_reg_INT2_SUMMARY_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = lineotn_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_LINEOTN_REG_INT2_SUMMARY);

    IOLOG( "%s -> 0x%08x;", "lineotn_reg_INT2_SUMMARY_read", reg_value);
    return reg_value;
}

static INLINE UINT32 lineotn_reg_INT3_SUMMARY_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_reg_INT3_SUMMARY_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = lineotn_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_LINEOTN_REG_INT3_SUMMARY);

    IOLOG( "%s -> 0x%08x;", "lineotn_reg_INT3_SUMMARY_read", reg_value);
    return reg_value;
}

static INLINE UINT32 lineotn_reg_TIP_STAT_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_reg_TIP_STAT_read( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = lineotn_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_LINEOTN_REG_TIP_STAT);

    IOLOG( "%s -> 0x%08x;", "lineotn_reg_TIP_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE lineotn_reg_TIP_STAT_poll( lineotn_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE lineotn_reg_TIP_STAT_poll( lineotn_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "lineotn_reg_TIP_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return lineotn_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_LINEOTN_REG_TIP_STAT,
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
static INLINE void lineotn_field_PCBI_LCLK_MASK_set( lineotn_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_field_PCBI_LCLK_MASK_set( lineotn_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lineotn_field_PCBI_LCLK_MASK_set", value, 1);
    IOLOG( "%s <= 0x%08x", "lineotn_field_PCBI_LCLK_MASK_set", value );

    /* (0x00000000 bits 2) field PCBI_LCLK_MASK of register PMC_LINEOTN_REG_MASTER_CONFIG */
    lineotn_reg_MASTER_CONFIG_field_set( b_ptr,
                                         h_ptr,
                                         LINEOTN_REG_MASTER_CONFIG_BIT_PCBI_LCLK_MASK_MSK,
                                         LINEOTN_REG_MASTER_CONFIG_BIT_PCBI_LCLK_MASK_OFF,
                                         value);
}

static INLINE UINT32 lineotn_field_PCBI_LCLK_MASK_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_field_PCBI_LCLK_MASK_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 2) field PCBI_LCLK_MASK of register PMC_LINEOTN_REG_MASTER_CONFIG */
    reg_value = lineotn_reg_MASTER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & LINEOTN_REG_MASTER_CONFIG_BIT_PCBI_LCLK_MASK_MSK) >> LINEOTN_REG_MASTER_CONFIG_BIT_PCBI_LCLK_MASK_OFF;
    IOLOG( "%s -> 0x%08x", "lineotn_field_PCBI_LCLK_MASK_get", value );

    return value;
}
static INLINE void lineotn_field_INT0_E_set( lineotn_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_field_INT0_E_set( lineotn_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32 value )
{
    if (value > 131071)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lineotn_field_INT0_E_set", value, 131071);
    IOLOG( "%s <= 0x%08x", "lineotn_field_INT0_E_set", value );

    /* (0x00000008 bits 16:0) bits 0:16 use field INT0_E of register PMC_LINEOTN_REG_INT0_EN */
    lineotn_reg_INT0_EN_field_set( b_ptr,
                                   h_ptr,
                                   LINEOTN_REG_INT0_EN_BIT_INT0_E_MSK,
                                   LINEOTN_REG_INT0_EN_BIT_INT0_E_OFF,
                                   value);
}

static INLINE UINT32 lineotn_field_INT0_E_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_field_INT0_E_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 16:0) bits 0:16 use field INT0_E of register PMC_LINEOTN_REG_INT0_EN */
    reg_value = lineotn_reg_INT0_EN_read(  b_ptr, h_ptr);
    value = (reg_value & LINEOTN_REG_INT0_EN_BIT_INT0_E_MSK) >> LINEOTN_REG_INT0_EN_BIT_INT0_E_OFF;
    IOLOG( "%s -> 0x%08x", "lineotn_field_INT0_E_get", value );

    return value;
}
static INLINE void lineotn_field_range_INT0_E_set( lineotn_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_field_range_INT0_E_set( lineotn_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lineotn_field_range_INT0_E_set", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lineotn_field_range_INT0_E_set", stop_bit, 16 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "lineotn_field_range_INT0_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000008 bits 16:0) bits 0:16 use field INT0_E of register PMC_LINEOTN_REG_INT0_EN */
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
        /* (0x00000008 bits 16:0) bits 0:16 use field INT0_E of register PMC_LINEOTN_REG_INT0_EN */
        lineotn_reg_INT0_EN_field_set( b_ptr,
                                       h_ptr,
                                       subfield_mask << (LINEOTN_REG_INT0_EN_BIT_INT0_E_OFF + subfield_offset),
                                       LINEOTN_REG_INT0_EN_BIT_INT0_E_OFF + subfield_offset,
                                       value >> subfield_shift);
    }
}

static INLINE UINT32 lineotn_field_range_INT0_E_get( lineotn_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_field_range_INT0_E_get( lineotn_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lineotn_field_range_INT0_E_get", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lineotn_field_range_INT0_E_get", stop_bit, 16 );
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
    /* (0x00000008 bits 16:0) bits 0:16 use field INT0_E of register PMC_LINEOTN_REG_INT0_EN */
    reg_value = lineotn_reg_INT0_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & LINEOTN_REG_INT0_EN_BIT_INT0_E_MSK)
                  >> LINEOTN_REG_INT0_EN_BIT_INT0_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, LINEOTN_REG_INT0_EN_BIT_INT0_E_MSK, LINEOTN_REG_INT0_EN_BIT_INT0_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "lineotn_field_range_INT0_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void lineotn_field_INT1_E_set( lineotn_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_field_INT1_E_set( lineotn_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32 value )
{
    if (value > 131071)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lineotn_field_INT1_E_set", value, 131071);
    IOLOG( "%s <= 0x%08x", "lineotn_field_INT1_E_set", value );

    /* (0x00000010 bits 16:0) bits 0:16 use field INT1_E of register PMC_LINEOTN_REG_INT1_EN */
    lineotn_reg_INT1_EN_field_set( b_ptr,
                                   h_ptr,
                                   LINEOTN_REG_INT1_EN_BIT_INT1_E_MSK,
                                   LINEOTN_REG_INT1_EN_BIT_INT1_E_OFF,
                                   value);
}

static INLINE UINT32 lineotn_field_INT1_E_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_field_INT1_E_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 16:0) bits 0:16 use field INT1_E of register PMC_LINEOTN_REG_INT1_EN */
    reg_value = lineotn_reg_INT1_EN_read(  b_ptr, h_ptr);
    value = (reg_value & LINEOTN_REG_INT1_EN_BIT_INT1_E_MSK) >> LINEOTN_REG_INT1_EN_BIT_INT1_E_OFF;
    IOLOG( "%s -> 0x%08x", "lineotn_field_INT1_E_get", value );

    return value;
}
static INLINE void lineotn_field_range_INT1_E_set( lineotn_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_field_range_INT1_E_set( lineotn_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lineotn_field_range_INT1_E_set", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lineotn_field_range_INT1_E_set", stop_bit, 16 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "lineotn_field_range_INT1_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000010 bits 16:0) bits 0:16 use field INT1_E of register PMC_LINEOTN_REG_INT1_EN */
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
        /* (0x00000010 bits 16:0) bits 0:16 use field INT1_E of register PMC_LINEOTN_REG_INT1_EN */
        lineotn_reg_INT1_EN_field_set( b_ptr,
                                       h_ptr,
                                       subfield_mask << (LINEOTN_REG_INT1_EN_BIT_INT1_E_OFF + subfield_offset),
                                       LINEOTN_REG_INT1_EN_BIT_INT1_E_OFF + subfield_offset,
                                       value >> subfield_shift);
    }
}

static INLINE UINT32 lineotn_field_range_INT1_E_get( lineotn_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_field_range_INT1_E_get( lineotn_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lineotn_field_range_INT1_E_get", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lineotn_field_range_INT1_E_get", stop_bit, 16 );
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
    /* (0x00000010 bits 16:0) bits 0:16 use field INT1_E of register PMC_LINEOTN_REG_INT1_EN */
    reg_value = lineotn_reg_INT1_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & LINEOTN_REG_INT1_EN_BIT_INT1_E_MSK)
                  >> LINEOTN_REG_INT1_EN_BIT_INT1_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, LINEOTN_REG_INT1_EN_BIT_INT1_E_MSK, LINEOTN_REG_INT1_EN_BIT_INT1_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "lineotn_field_range_INT1_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void lineotn_field_INT2_E_set( lineotn_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_field_INT2_E_set( lineotn_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32 value )
{
    if (value > 131071)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lineotn_field_INT2_E_set", value, 131071);
    IOLOG( "%s <= 0x%08x", "lineotn_field_INT2_E_set", value );

    /* (0x00000018 bits 16:0) bits 0:16 use field INT2_E of register PMC_LINEOTN_REG_INT2_EN */
    lineotn_reg_INT2_EN_field_set( b_ptr,
                                   h_ptr,
                                   LINEOTN_REG_INT2_EN_BIT_INT2_E_MSK,
                                   LINEOTN_REG_INT2_EN_BIT_INT2_E_OFF,
                                   value);
}

static INLINE UINT32 lineotn_field_INT2_E_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_field_INT2_E_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 16:0) bits 0:16 use field INT2_E of register PMC_LINEOTN_REG_INT2_EN */
    reg_value = lineotn_reg_INT2_EN_read(  b_ptr, h_ptr);
    value = (reg_value & LINEOTN_REG_INT2_EN_BIT_INT2_E_MSK) >> LINEOTN_REG_INT2_EN_BIT_INT2_E_OFF;
    IOLOG( "%s -> 0x%08x", "lineotn_field_INT2_E_get", value );

    return value;
}
static INLINE void lineotn_field_range_INT2_E_set( lineotn_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_field_range_INT2_E_set( lineotn_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lineotn_field_range_INT2_E_set", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lineotn_field_range_INT2_E_set", stop_bit, 16 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "lineotn_field_range_INT2_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000018 bits 16:0) bits 0:16 use field INT2_E of register PMC_LINEOTN_REG_INT2_EN */
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
        /* (0x00000018 bits 16:0) bits 0:16 use field INT2_E of register PMC_LINEOTN_REG_INT2_EN */
        lineotn_reg_INT2_EN_field_set( b_ptr,
                                       h_ptr,
                                       subfield_mask << (LINEOTN_REG_INT2_EN_BIT_INT2_E_OFF + subfield_offset),
                                       LINEOTN_REG_INT2_EN_BIT_INT2_E_OFF + subfield_offset,
                                       value >> subfield_shift);
    }
}

static INLINE UINT32 lineotn_field_range_INT2_E_get( lineotn_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_field_range_INT2_E_get( lineotn_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lineotn_field_range_INT2_E_get", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lineotn_field_range_INT2_E_get", stop_bit, 16 );
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
    /* (0x00000018 bits 16:0) bits 0:16 use field INT2_E of register PMC_LINEOTN_REG_INT2_EN */
    reg_value = lineotn_reg_INT2_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & LINEOTN_REG_INT2_EN_BIT_INT2_E_MSK)
                  >> LINEOTN_REG_INT2_EN_BIT_INT2_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, LINEOTN_REG_INT2_EN_BIT_INT2_E_MSK, LINEOTN_REG_INT2_EN_BIT_INT2_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "lineotn_field_range_INT2_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void lineotn_field_INT3_E_set( lineotn_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_field_INT3_E_set( lineotn_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32 value )
{
    if (value > 131071)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "lineotn_field_INT3_E_set", value, 131071);
    IOLOG( "%s <= 0x%08x", "lineotn_field_INT3_E_set", value );

    /* (0x00000020 bits 16:0) bits 0:16 use field INT3_E of register PMC_LINEOTN_REG_INT3_EN */
    lineotn_reg_INT3_EN_field_set( b_ptr,
                                   h_ptr,
                                   LINEOTN_REG_INT3_EN_BIT_INT3_E_MSK,
                                   LINEOTN_REG_INT3_EN_BIT_INT3_E_OFF,
                                   value);
}

static INLINE UINT32 lineotn_field_INT3_E_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_field_INT3_E_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 16:0) bits 0:16 use field INT3_E of register PMC_LINEOTN_REG_INT3_EN */
    reg_value = lineotn_reg_INT3_EN_read(  b_ptr, h_ptr);
    value = (reg_value & LINEOTN_REG_INT3_EN_BIT_INT3_E_MSK) >> LINEOTN_REG_INT3_EN_BIT_INT3_E_OFF;
    IOLOG( "%s -> 0x%08x", "lineotn_field_INT3_E_get", value );

    return value;
}
static INLINE void lineotn_field_range_INT3_E_set( lineotn_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void lineotn_field_range_INT3_E_set( lineotn_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lineotn_field_range_INT3_E_set", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lineotn_field_range_INT3_E_set", stop_bit, 16 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "lineotn_field_range_INT3_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000020 bits 16:0) bits 0:16 use field INT3_E of register PMC_LINEOTN_REG_INT3_EN */
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
        /* (0x00000020 bits 16:0) bits 0:16 use field INT3_E of register PMC_LINEOTN_REG_INT3_EN */
        lineotn_reg_INT3_EN_field_set( b_ptr,
                                       h_ptr,
                                       subfield_mask << (LINEOTN_REG_INT3_EN_BIT_INT3_E_OFF + subfield_offset),
                                       LINEOTN_REG_INT3_EN_BIT_INT3_E_OFF + subfield_offset,
                                       value >> subfield_shift);
    }
}

static INLINE UINT32 lineotn_field_range_INT3_E_get( lineotn_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_field_range_INT3_E_get( lineotn_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lineotn_field_range_INT3_E_get", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lineotn_field_range_INT3_E_get", stop_bit, 16 );
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
    /* (0x00000020 bits 16:0) bits 0:16 use field INT3_E of register PMC_LINEOTN_REG_INT3_EN */
    reg_value = lineotn_reg_INT3_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & LINEOTN_REG_INT3_EN_BIT_INT3_E_MSK)
                  >> LINEOTN_REG_INT3_EN_BIT_INT3_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, LINEOTN_REG_INT3_EN_BIT_INT3_E_MSK, LINEOTN_REG_INT3_EN_BIT_INT3_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "lineotn_field_range_INT3_E_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 lineotn_field_INT0_SUMMARY_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_field_INT0_SUMMARY_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 16:0) bits 0:16 use field INT0_SUMMARY of register PMC_LINEOTN_REG_INT0_SUMMARY */
    reg_value = lineotn_reg_INT0_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & LINEOTN_REG_INT0_SUMMARY_BIT_INT0_SUMMARY_MSK) >> LINEOTN_REG_INT0_SUMMARY_BIT_INT0_SUMMARY_OFF;
    IOLOG( "%s -> 0x%08x", "lineotn_field_INT0_SUMMARY_get", value );

    return value;
}
static INLINE UINT32 lineotn_field_range_INT0_SUMMARY_get( lineotn_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_field_range_INT0_SUMMARY_get( lineotn_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lineotn_field_range_INT0_SUMMARY_get", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lineotn_field_range_INT0_SUMMARY_get", stop_bit, 16 );
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
    /* (0x0000000c bits 16:0) bits 0:16 use field INT0_SUMMARY of register PMC_LINEOTN_REG_INT0_SUMMARY */
    reg_value = lineotn_reg_INT0_SUMMARY_read(  b_ptr, h_ptr);
    field_value = (reg_value & LINEOTN_REG_INT0_SUMMARY_BIT_INT0_SUMMARY_MSK)
                  >> LINEOTN_REG_INT0_SUMMARY_BIT_INT0_SUMMARY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, LINEOTN_REG_INT0_SUMMARY_BIT_INT0_SUMMARY_MSK, LINEOTN_REG_INT0_SUMMARY_BIT_INT0_SUMMARY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "lineotn_field_range_INT0_SUMMARY_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 lineotn_field_INT1_SUMMARY_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_field_INT1_SUMMARY_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 16:0) bits 0:16 use field INT1_SUMMARY of register PMC_LINEOTN_REG_INT1_SUMMARY */
    reg_value = lineotn_reg_INT1_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & LINEOTN_REG_INT1_SUMMARY_BIT_INT1_SUMMARY_MSK) >> LINEOTN_REG_INT1_SUMMARY_BIT_INT1_SUMMARY_OFF;
    IOLOG( "%s -> 0x%08x", "lineotn_field_INT1_SUMMARY_get", value );

    return value;
}
static INLINE UINT32 lineotn_field_range_INT1_SUMMARY_get( lineotn_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_field_range_INT1_SUMMARY_get( lineotn_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lineotn_field_range_INT1_SUMMARY_get", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lineotn_field_range_INT1_SUMMARY_get", stop_bit, 16 );
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
    /* (0x00000014 bits 16:0) bits 0:16 use field INT1_SUMMARY of register PMC_LINEOTN_REG_INT1_SUMMARY */
    reg_value = lineotn_reg_INT1_SUMMARY_read(  b_ptr, h_ptr);
    field_value = (reg_value & LINEOTN_REG_INT1_SUMMARY_BIT_INT1_SUMMARY_MSK)
                  >> LINEOTN_REG_INT1_SUMMARY_BIT_INT1_SUMMARY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, LINEOTN_REG_INT1_SUMMARY_BIT_INT1_SUMMARY_MSK, LINEOTN_REG_INT1_SUMMARY_BIT_INT1_SUMMARY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "lineotn_field_range_INT1_SUMMARY_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 lineotn_field_INT2_SUMMARY_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_field_INT2_SUMMARY_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 16:0) bits 0:16 use field INT2_SUMMARY of register PMC_LINEOTN_REG_INT2_SUMMARY */
    reg_value = lineotn_reg_INT2_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & LINEOTN_REG_INT2_SUMMARY_BIT_INT2_SUMMARY_MSK) >> LINEOTN_REG_INT2_SUMMARY_BIT_INT2_SUMMARY_OFF;
    IOLOG( "%s -> 0x%08x", "lineotn_field_INT2_SUMMARY_get", value );

    return value;
}
static INLINE UINT32 lineotn_field_range_INT2_SUMMARY_get( lineotn_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_field_range_INT2_SUMMARY_get( lineotn_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lineotn_field_range_INT2_SUMMARY_get", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lineotn_field_range_INT2_SUMMARY_get", stop_bit, 16 );
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
    /* (0x0000001c bits 16:0) bits 0:16 use field INT2_SUMMARY of register PMC_LINEOTN_REG_INT2_SUMMARY */
    reg_value = lineotn_reg_INT2_SUMMARY_read(  b_ptr, h_ptr);
    field_value = (reg_value & LINEOTN_REG_INT2_SUMMARY_BIT_INT2_SUMMARY_MSK)
                  >> LINEOTN_REG_INT2_SUMMARY_BIT_INT2_SUMMARY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, LINEOTN_REG_INT2_SUMMARY_BIT_INT2_SUMMARY_MSK, LINEOTN_REG_INT2_SUMMARY_BIT_INT2_SUMMARY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "lineotn_field_range_INT2_SUMMARY_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 lineotn_field_INT3_SUMMARY_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_field_INT3_SUMMARY_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 16:0) bits 0:16 use field INT3_SUMMARY of register PMC_LINEOTN_REG_INT3_SUMMARY */
    reg_value = lineotn_reg_INT3_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & LINEOTN_REG_INT3_SUMMARY_BIT_INT3_SUMMARY_MSK) >> LINEOTN_REG_INT3_SUMMARY_BIT_INT3_SUMMARY_OFF;
    IOLOG( "%s -> 0x%08x", "lineotn_field_INT3_SUMMARY_get", value );

    return value;
}
static INLINE UINT32 lineotn_field_range_INT3_SUMMARY_get( lineotn_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_field_range_INT3_SUMMARY_get( lineotn_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "lineotn_field_range_INT3_SUMMARY_get", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "lineotn_field_range_INT3_SUMMARY_get", stop_bit, 16 );
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
    /* (0x00000024 bits 16:0) bits 0:16 use field INT3_SUMMARY of register PMC_LINEOTN_REG_INT3_SUMMARY */
    reg_value = lineotn_reg_INT3_SUMMARY_read(  b_ptr, h_ptr);
    field_value = (reg_value & LINEOTN_REG_INT3_SUMMARY_BIT_INT3_SUMMARY_MSK)
                  >> LINEOTN_REG_INT3_SUMMARY_BIT_INT3_SUMMARY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, LINEOTN_REG_INT3_SUMMARY_BIT_INT3_SUMMARY_MSK, LINEOTN_REG_INT3_SUMMARY_BIT_INT3_SUMMARY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "lineotn_field_range_INT3_SUMMARY_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 lineotn_field_OTN_OTU_TIP_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_field_OTN_OTU_TIP_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 2) field OTN_OTU_TIP of register PMC_LINEOTN_REG_TIP_STAT */
    reg_value = lineotn_reg_TIP_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & LINEOTN_REG_TIP_STAT_BIT_OTN_OTU_TIP_MSK) >> LINEOTN_REG_TIP_STAT_BIT_OTN_OTU_TIP_OFF;
    IOLOG( "%s -> 0x%08x", "lineotn_field_OTN_OTU_TIP_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE lineotn_field_OTN_OTU_TIP_poll( lineotn_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE lineotn_field_OTN_OTU_TIP_poll( lineotn_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "lineotn_field_OTN_OTU_TIP_poll", value );

    /* (0x00000028 bits 2) field OTN_OTU_TIP of register PMC_LINEOTN_REG_TIP_STAT */
    return lineotn_reg_TIP_STAT_poll( b_ptr,
                                      h_ptr,
                                      LINEOTN_REG_TIP_STAT_BIT_OTN_OTU_TIP_MSK,
                                      (value<<LINEOTN_REG_TIP_STAT_BIT_OTN_OTU_TIP_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE UINT32 lineotn_field_MFEC_TIP_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 lineotn_field_MFEC_TIP_get( lineotn_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 1) field MFEC_TIP of register PMC_LINEOTN_REG_TIP_STAT */
    reg_value = lineotn_reg_TIP_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & LINEOTN_REG_TIP_STAT_BIT_MFEC_TIP_MSK) >> LINEOTN_REG_TIP_STAT_BIT_MFEC_TIP_OFF;
    IOLOG( "%s -> 0x%08x", "lineotn_field_MFEC_TIP_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE lineotn_field_MFEC_TIP_poll( lineotn_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE lineotn_field_MFEC_TIP_poll( lineotn_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "lineotn_field_MFEC_TIP_poll", value );

    /* (0x00000028 bits 1) field MFEC_TIP of register PMC_LINEOTN_REG_TIP_STAT */
    return lineotn_reg_TIP_STAT_poll( b_ptr,
                                      h_ptr,
                                      LINEOTN_REG_TIP_STAT_BIT_MFEC_TIP_MSK,
                                      (value<<LINEOTN_REG_TIP_STAT_BIT_MFEC_TIP_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LINEOTN_IO_INLINE_H */
