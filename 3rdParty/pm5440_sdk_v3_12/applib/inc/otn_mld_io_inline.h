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
 *     and register accessor functions for the otn_mld block
 *****************************************************************************/
#ifndef _OTN_MLD_IO_INLINE_H
#define _OTN_MLD_IO_INLINE_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "otn_mld_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define OTN_MLD_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for otn_mld
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
} otn_mld_buffer_t;
static INLINE void otn_mld_buffer_init( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void otn_mld_buffer_init( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "otn_mld_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void otn_mld_buffer_flush( otn_mld_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void otn_mld_buffer_flush( otn_mld_buffer_t *b_ptr )
{
    IOLOG( "otn_mld_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 otn_mld_reg_read( otn_mld_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_read( otn_mld_buffer_t *b_ptr,
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
static INLINE void otn_mld_reg_write( otn_mld_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_write( otn_mld_buffer_t *b_ptr,
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

static INLINE void otn_mld_field_set( otn_mld_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 mask,
                                      UINT32 unused_mask,
                                      UINT32 ofs,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_set( otn_mld_buffer_t *b_ptr,
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

static INLINE void otn_mld_action_on_write_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_action_on_write_field_set( otn_mld_buffer_t *b_ptr,
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

static INLINE void otn_mld_burst_read( otn_mld_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void otn_mld_burst_read( otn_mld_buffer_t *b_ptr,
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

static INLINE void otn_mld_burst_write( otn_mld_buffer_t *b_ptr,
                                        lineotn_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void otn_mld_burst_write( otn_mld_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE otn_mld_poll( otn_mld_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 mask,
                                                 UINT32 value,
                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                 UINT32 max_count,
                                                 UINT32 *num_failed_polls,
                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_poll( otn_mld_buffer_t *b_ptr,
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
 *  register access functions for otn_mld
 * ==================================================================================
 */

static INLINE void otn_mld_reg_MASTER_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MASTER_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_MASTER_CONFIG_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MASTER_CONFIG,
                       value);
}

static INLINE void otn_mld_reg_MASTER_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MASTER_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_MASTER_CONFIG_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MASTER_CONFIG,
                       mask,
                       PMC_OTN_MLD_REG_MASTER_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_MASTER_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_MASTER_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_MASTER_CONFIG);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_MASTER_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_LOWPWR_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_LOWPWR_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_LOWPWR_CONFIG_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_LOWPWR_CONFIG,
                       value);
}

static INLINE void otn_mld_reg_LOWPWR_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_LOWPWR_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_LOWPWR_CONFIG_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_LOWPWR_CONFIG,
                       mask,
                       PMC_OTN_MLD_REG_LOWPWR_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_LOWPWR_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_LOWPWR_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_LOWPWR_CONFIG);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_LOWPWR_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_MUX0_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX0_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_MUX0_CONFIG_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX0_CONFIG,
                       value);
}

static INLINE void otn_mld_reg_MUX0_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX0_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_MUX0_CONFIG_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX0_CONFIG,
                       mask,
                       PMC_OTN_MLD_REG_MUX0_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_MUX0_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_MUX0_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_MUX0_CONFIG);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_MUX0_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_MUX1_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX1_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_MUX1_CONFIG_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX1_CONFIG,
                       value);
}

static INLINE void otn_mld_reg_MUX1_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX1_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_MUX1_CONFIG_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX1_CONFIG,
                       mask,
                       PMC_OTN_MLD_REG_MUX1_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_MUX1_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_MUX1_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_MUX1_CONFIG);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_MUX1_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_MUX2_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX2_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_MUX2_CONFIG_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX2_CONFIG,
                       value);
}

static INLINE void otn_mld_reg_MUX2_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX2_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_MUX2_CONFIG_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX2_CONFIG,
                       mask,
                       PMC_OTN_MLD_REG_MUX2_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_MUX2_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_MUX2_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_MUX2_CONFIG);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_MUX2_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_MUX3_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX3_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_MUX3_CONFIG_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX3_CONFIG,
                       value);
}

static INLINE void otn_mld_reg_MUX3_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX3_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_MUX3_CONFIG_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX3_CONFIG,
                       mask,
                       PMC_OTN_MLD_REG_MUX3_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_MUX3_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_MUX3_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_MUX3_CONFIG);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_MUX3_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_MUX4_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX4_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_MUX4_CONFIG_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX4_CONFIG,
                       value);
}

static INLINE void otn_mld_reg_MUX4_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX4_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_MUX4_CONFIG_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX4_CONFIG,
                       mask,
                       PMC_OTN_MLD_REG_MUX4_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_MUX4_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_MUX4_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_MUX4_CONFIG);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_MUX4_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_MUX5_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX5_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_MUX5_CONFIG_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX5_CONFIG,
                       value);
}

static INLINE void otn_mld_reg_MUX5_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX5_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_MUX5_CONFIG_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX5_CONFIG,
                       mask,
                       PMC_OTN_MLD_REG_MUX5_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_MUX5_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_MUX5_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_MUX5_CONFIG);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_MUX5_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_MUX6_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX6_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_MUX6_CONFIG_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX6_CONFIG,
                       value);
}

static INLINE void otn_mld_reg_MUX6_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX6_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_MUX6_CONFIG_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX6_CONFIG,
                       mask,
                       PMC_OTN_MLD_REG_MUX6_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_MUX6_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_MUX6_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_MUX6_CONFIG);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_MUX6_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_MUX7_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX7_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_MUX7_CONFIG_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX7_CONFIG,
                       value);
}

static INLINE void otn_mld_reg_MUX7_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX7_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_MUX7_CONFIG_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX7_CONFIG,
                       mask,
                       PMC_OTN_MLD_REG_MUX7_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_MUX7_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_MUX7_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_MUX7_CONFIG);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_MUX7_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_MUX8_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX8_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_MUX8_CONFIG_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX8_CONFIG,
                       value);
}

static INLINE void otn_mld_reg_MUX8_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX8_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_MUX8_CONFIG_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX8_CONFIG,
                       mask,
                       PMC_OTN_MLD_REG_MUX8_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_MUX8_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_MUX8_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_MUX8_CONFIG);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_MUX8_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_MUX9_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX9_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_MUX9_CONFIG_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX9_CONFIG,
                       value);
}

static INLINE void otn_mld_reg_MUX9_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX9_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_MUX9_CONFIG_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX9_CONFIG,
                       mask,
                       PMC_OTN_MLD_REG_MUX9_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_MUX9_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_MUX9_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_MUX9_CONFIG);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_MUX9_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_MUX10_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX10_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_MUX10_CONFIG_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX10_CONFIG,
                       value);
}

static INLINE void otn_mld_reg_MUX10_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX10_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_MUX10_CONFIG_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX10_CONFIG,
                       mask,
                       PMC_OTN_MLD_REG_MUX10_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_MUX10_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_MUX10_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_MUX10_CONFIG);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_MUX10_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_MUX11_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX11_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_MUX11_CONFIG_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX11_CONFIG,
                       value);
}

static INLINE void otn_mld_reg_MUX11_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX11_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_MUX11_CONFIG_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX11_CONFIG,
                       mask,
                       PMC_OTN_MLD_REG_MUX11_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_MUX11_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_MUX11_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_MUX11_CONFIG);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_MUX11_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_MUX12_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX12_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_MUX12_CONFIG_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX12_CONFIG,
                       value);
}

static INLINE void otn_mld_reg_MUX12_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX12_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_MUX12_CONFIG_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX12_CONFIG,
                       mask,
                       PMC_OTN_MLD_REG_MUX12_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_MUX12_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_MUX12_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_MUX12_CONFIG);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_MUX12_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_MUX13_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX13_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_MUX13_CONFIG_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX13_CONFIG,
                       value);
}

static INLINE void otn_mld_reg_MUX13_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_MUX13_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_MUX13_CONFIG_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_MUX13_CONFIG,
                       mask,
                       PMC_OTN_MLD_REG_MUX13_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_MUX13_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_MUX13_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_MUX13_CONFIG);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_MUX13_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT40G0_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G0_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT40G0_CONFIG_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G0_CONFIG,
                       value);
}

static INLINE void otn_mld_reg_HADAPT40G0_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G0_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT40G0_CONFIG_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G0_CONFIG,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT40G0_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT40G0_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G0_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT40G0_CONFIG);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G0_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT40G0_MODE0_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G0_MODE0_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT40G0_MODE0_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G0_MODE0,
                       value);
}

static INLINE void otn_mld_reg_HADAPT40G0_MODE0_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G0_MODE0_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT40G0_MODE0_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G0_MODE0,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT40G0_MODE0_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT40G0_MODE0_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G0_MODE0_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT40G0_MODE0);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G0_MODE0_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT40G0_MODE1_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G0_MODE1_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT40G0_MODE1_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G0_MODE1,
                       value);
}

static INLINE void otn_mld_reg_HADAPT40G0_MODE1_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G0_MODE1_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT40G0_MODE1_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G0_MODE1,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT40G0_MODE1_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT40G0_MODE1_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G0_MODE1_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT40G0_MODE1);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G0_MODE1_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT40G0_MODE2_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G0_MODE2_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT40G0_MODE2_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G0_MODE2,
                       value);
}

static INLINE void otn_mld_reg_HADAPT40G0_MODE2_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G0_MODE2_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT40G0_MODE2_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G0_MODE2,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT40G0_MODE2_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT40G0_MODE2_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G0_MODE2_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT40G0_MODE2);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G0_MODE2_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT40G0_MODE3_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G0_MODE3_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT40G0_MODE3_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G0_MODE3,
                       value);
}

static INLINE void otn_mld_reg_HADAPT40G0_MODE3_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G0_MODE3_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT40G0_MODE3_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G0_MODE3,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT40G0_MODE3_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT40G0_MODE3_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G0_MODE3_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT40G0_MODE3);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G0_MODE3_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT40G0_MODE4_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G0_MODE4_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT40G0_MODE4_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G0_MODE4,
                       value);
}

static INLINE void otn_mld_reg_HADAPT40G0_MODE4_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G0_MODE4_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT40G0_MODE4_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G0_MODE4,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT40G0_MODE4_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT40G0_MODE4_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G0_MODE4_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT40G0_MODE4);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G0_MODE4_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT40G1_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G1_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT40G1_CONFIG_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G1_CONFIG,
                       value);
}

static INLINE void otn_mld_reg_HADAPT40G1_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G1_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT40G1_CONFIG_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G1_CONFIG,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT40G1_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT40G1_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G1_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT40G1_CONFIG);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G1_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT40G1_MODE0_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G1_MODE0_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT40G1_MODE0_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G1_MODE0,
                       value);
}

static INLINE void otn_mld_reg_HADAPT40G1_MODE0_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G1_MODE0_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT40G1_MODE0_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G1_MODE0,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT40G1_MODE0_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT40G1_MODE0_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G1_MODE0_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT40G1_MODE0);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G1_MODE0_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT40G1_MODE1_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G1_MODE1_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT40G1_MODE1_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G1_MODE1,
                       value);
}

static INLINE void otn_mld_reg_HADAPT40G1_MODE1_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G1_MODE1_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT40G1_MODE1_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G1_MODE1,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT40G1_MODE1_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT40G1_MODE1_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G1_MODE1_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT40G1_MODE1);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G1_MODE1_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT40G1_MODE2_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G1_MODE2_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT40G1_MODE2_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G1_MODE2,
                       value);
}

static INLINE void otn_mld_reg_HADAPT40G1_MODE2_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G1_MODE2_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT40G1_MODE2_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G1_MODE2,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT40G1_MODE2_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT40G1_MODE2_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G1_MODE2_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT40G1_MODE2);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G1_MODE2_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT40G1_MODE3_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G1_MODE3_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT40G1_MODE3_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G1_MODE3,
                       value);
}

static INLINE void otn_mld_reg_HADAPT40G1_MODE3_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G1_MODE3_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT40G1_MODE3_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G1_MODE3,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT40G1_MODE3_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT40G1_MODE3_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G1_MODE3_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT40G1_MODE3);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G1_MODE3_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT40G1_MODE4_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G1_MODE4_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT40G1_MODE4_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G1_MODE4,
                       value);
}

static INLINE void otn_mld_reg_HADAPT40G1_MODE4_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G1_MODE4_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT40G1_MODE4_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT40G1_MODE4,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT40G1_MODE4_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT40G1_MODE4_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G1_MODE4_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT40G1_MODE4);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G1_MODE4_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT100G_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_CONFIG_write( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT100G_CONFIG_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_CONFIG,
                       value);
}

static INLINE void otn_mld_reg_HADAPT100G_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_CONFIG_field_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT100G_CONFIG_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_CONFIG,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT100G_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT100G_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_CONFIG_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT100G_CONFIG);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT100G_MODE0_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_MODE0_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT100G_MODE0_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_MODE0,
                       value);
}

static INLINE void otn_mld_reg_HADAPT100G_MODE0_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_MODE0_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT100G_MODE0_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_MODE0,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT100G_MODE0_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT100G_MODE0_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_MODE0_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT100G_MODE0);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_MODE0_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT100G_MODE1_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_MODE1_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT100G_MODE1_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_MODE1,
                       value);
}

static INLINE void otn_mld_reg_HADAPT100G_MODE1_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_MODE1_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT100G_MODE1_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_MODE1,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT100G_MODE1_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT100G_MODE1_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_MODE1_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT100G_MODE1);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_MODE1_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT100G_MODE2_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_MODE2_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT100G_MODE2_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_MODE2,
                       value);
}

static INLINE void otn_mld_reg_HADAPT100G_MODE2_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_MODE2_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT100G_MODE2_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_MODE2,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT100G_MODE2_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT100G_MODE2_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_MODE2_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT100G_MODE2);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_MODE2_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT100G_MODE3_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_MODE3_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT100G_MODE3_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_MODE3,
                       value);
}

static INLINE void otn_mld_reg_HADAPT100G_MODE3_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_MODE3_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT100G_MODE3_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_MODE3,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT100G_MODE3_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT100G_MODE3_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_MODE3_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT100G_MODE3);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_MODE3_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT100G_MODE4_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_MODE4_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT100G_MODE4_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_MODE4,
                       value);
}

static INLINE void otn_mld_reg_HADAPT100G_MODE4_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_MODE4_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT100G_MODE4_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_MODE4,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT100G_MODE4_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT100G_MODE4_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_MODE4_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT100G_MODE4);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_MODE4_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT100G_MODE5_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_MODE5_write( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT100G_MODE5_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_MODE5,
                       value);
}

static INLINE void otn_mld_reg_HADAPT100G_MODE5_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_MODE5_field_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT100G_MODE5_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_MODE5,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT100G_MODE5_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT100G_MODE5_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_MODE5_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT100G_MODE5);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_MODE5_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN0_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN0_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT100G_PATTERN0_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN0,
                       value);
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN0_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN0_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT100G_PATTERN0_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN0,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN0_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN0_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN0_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT100G_PATTERN0);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_PATTERN0_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN1_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN1_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT100G_PATTERN1_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN1,
                       value);
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN1_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN1_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT100G_PATTERN1_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN1,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN1_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN1_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN1_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT100G_PATTERN1);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_PATTERN1_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN2_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN2_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT100G_PATTERN2_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN2,
                       value);
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN2_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN2_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT100G_PATTERN2_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN2,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN2_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN2_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN2_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT100G_PATTERN2);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_PATTERN2_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN3_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN3_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT100G_PATTERN3_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN3,
                       value);
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN3_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN3_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT100G_PATTERN3_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN3,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN3_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN3_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN3_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT100G_PATTERN3);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_PATTERN3_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN4_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN4_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT100G_PATTERN4_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN4,
                       value);
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN4_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN4_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT100G_PATTERN4_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN4,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN4_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN4_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN4_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT100G_PATTERN4);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_PATTERN4_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN5_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN5_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT100G_PATTERN5_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN5,
                       value);
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN5_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN5_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT100G_PATTERN5_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN5,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN5_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN5_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN5_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT100G_PATTERN5);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_PATTERN5_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN6_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN6_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT100G_PATTERN6_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN6,
                       value);
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN6_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN6_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT100G_PATTERN6_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN6,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN6_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN6_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN6_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT100G_PATTERN6);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_PATTERN6_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN7_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN7_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT100G_PATTERN7_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN7,
                       value);
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN7_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN7_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT100G_PATTERN7_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN7,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN7_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN7_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN7_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT100G_PATTERN7);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_PATTERN7_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN8_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN8_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT100G_PATTERN8_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN8,
                       value);
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN8_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN8_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT100G_PATTERN8_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN8,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN8_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN8_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN8_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT100G_PATTERN8);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_PATTERN8_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN9_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN9_write( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT100G_PATTERN9_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN9,
                       value);
}

static INLINE void otn_mld_reg_HADAPT100G_PATTERN9_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_PATTERN9_field_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT100G_PATTERN9_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN9,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT100G_PATTERN9_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN9_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_PATTERN9_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_HADAPT100G_PATTERN9);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_PATTERN9_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_LANE_CONFIG_array_write( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_LANE_CONFIG_array_write( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_LANE_CONFIG_array_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_LANE_CONFIG(N),
                       value);
}

static INLINE void otn_mld_reg_LANE_CONFIG_array_field_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_LANE_CONFIG_array_field_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_LANE_CONFIG_array_field_set", N, mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_LANE_CONFIG(N),
                       mask,
                       PMC_OTN_MLD_REG_LANE_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_LANE_CONFIG_array_read( otn_mld_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_LANE_CONFIG_array_read( otn_mld_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_LANE_CONFIG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otn_mld_reg_LANE_CONFIG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otn_mld_reg_DISTRIBUTION_MUX_CONFIG_array_write( otn_mld_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_DISTRIBUTION_MUX_CONFIG_array_write( otn_mld_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_DISTRIBUTION_MUX_CONFIG_array_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG(N),
                       value);
}

static INLINE void otn_mld_reg_DISTRIBUTION_MUX_CONFIG_array_field_set( otn_mld_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_DISTRIBUTION_MUX_CONFIG_array_field_set( otn_mld_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_DISTRIBUTION_MUX_CONFIG_array_field_set", N, mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG(N),
                       mask,
                       PMC_OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_DISTRIBUTION_MUX_CONFIG_array_read( otn_mld_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_DISTRIBUTION_MUX_CONFIG_array_read( otn_mld_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32  N )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otn_mld_reg_DISTRIBUTION_MUX_CONFIG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otn_mld_reg_DEFECT_RCP_CHNL_CONFIG_array_write( otn_mld_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_DEFECT_RCP_CHNL_CONFIG_array_write( otn_mld_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_DEFECT_RCP_CHNL_CONFIG_array_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG(N),
                       value);
}

static INLINE void otn_mld_reg_DEFECT_RCP_CHNL_CONFIG_array_field_set( otn_mld_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_DEFECT_RCP_CHNL_CONFIG_array_field_set( otn_mld_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_DEFECT_RCP_CHNL_CONFIG_array_field_set", N, mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG(N),
                       mask,
                       PMC_OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_DEFECT_RCP_CHNL_CONFIG_array_read( otn_mld_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_DEFECT_RCP_CHNL_CONFIG_array_read( otn_mld_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otn_mld_reg_DEFECT_RCP_CHNL_CONFIG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otn_mld_reg_DLOM_TO_FFAIL_MAPPING_array_write( otn_mld_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_DLOM_TO_FFAIL_MAPPING_array_write( otn_mld_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_DLOM_TO_FFAIL_MAPPING_array_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_DLOM_TO_FFAIL_MAPPING(N),
                       value);
}

static INLINE void otn_mld_reg_DLOM_TO_FFAIL_MAPPING_array_field_set( otn_mld_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_DLOM_TO_FFAIL_MAPPING_array_field_set( otn_mld_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_DLOM_TO_FFAIL_MAPPING_array_field_set", N, mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_DLOM_TO_FFAIL_MAPPING(N),
                       mask,
                       PMC_OTN_MLD_REG_DLOM_TO_FFAIL_MAPPING_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_DLOM_TO_FFAIL_MAPPING_array_read( otn_mld_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_DLOM_TO_FFAIL_MAPPING_array_read( otn_mld_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_DLOM_TO_FFAIL_MAPPING(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otn_mld_reg_DLOM_TO_FFAIL_MAPPING_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otn_mld_reg_BERGEN32_CONFIG1_array_write( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_BERGEN32_CONFIG1_array_write( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_BERGEN32_CONFIG1_array_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_BERGEN32_CONFIG1(N),
                       value);
}

static INLINE void otn_mld_reg_BERGEN32_CONFIG1_array_field_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_BERGEN32_CONFIG1_array_field_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_BERGEN32_CONFIG1_array_field_set", N, mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_BERGEN32_CONFIG1(N),
                       mask,
                       PMC_OTN_MLD_REG_BERGEN32_CONFIG1_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_BERGEN32_CONFIG1_array_read( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_BERGEN32_CONFIG1_array_read( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_BERGEN32_CONFIG1(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otn_mld_reg_BERGEN32_CONFIG1_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otn_mld_reg_BERGEN32_CONFIG2_array_write( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_BERGEN32_CONFIG2_array_write( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_BERGEN32_CONFIG2_array_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_BERGEN32_CONFIG2(N),
                       value);
}

static INLINE void otn_mld_reg_BERGEN32_CONFIG2_array_field_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_BERGEN32_CONFIG2_array_field_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_BERGEN32_CONFIG2_array_field_set", N, mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_BERGEN32_CONFIG2(N),
                       mask,
                       PMC_OTN_MLD_REG_BERGEN32_CONFIG2_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_BERGEN32_CONFIG2_array_read( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_BERGEN32_CONFIG2_array_read( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_BERGEN32_CONFIG2(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otn_mld_reg_BERGEN32_CONFIG2_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otn_mld_reg_OPSM23_SFI_PATH_CONFIG_array_write( otn_mld_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_OPSM23_SFI_PATH_CONFIG_array_write( otn_mld_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_OPSM23_SFI_PATH_CONFIG_array_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG(N),
                       value);
}

static INLINE void otn_mld_reg_OPSM23_SFI_PATH_CONFIG_array_field_set( otn_mld_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_OPSM23_SFI_PATH_CONFIG_array_field_set( otn_mld_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_OPSM23_SFI_PATH_CONFIG_array_field_set", N, mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG(N),
                       mask,
                       PMC_OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_OPSM23_SFI_PATH_CONFIG_array_read( otn_mld_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_OPSM23_SFI_PATH_CONFIG_array_read( otn_mld_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otn_mld_reg_OPSM23_SFI_PATH_CONFIG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otn_mld_reg_RINGOSC_32_CONFIG_array_write( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_RINGOSC_32_CONFIG_array_write( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_RINGOSC_32_CONFIG_array_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_RINGOSC_32_CONFIG(N),
                       value);
}

static INLINE void otn_mld_reg_RINGOSC_32_CONFIG_array_field_set( otn_mld_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_RINGOSC_32_CONFIG_array_field_set( otn_mld_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_RINGOSC_32_CONFIG_array_field_set", N, mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_OTN_MLD_REG_RINGOSC_32_CONFIG(N),
                       mask,
                       PMC_OTN_MLD_REG_RINGOSC_32_CONFIG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_RINGOSC_32_CONFIG_array_read( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_RINGOSC_32_CONFIG_array_read( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_OTN_MLD_REG_RINGOSC_32_CONFIG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "otn_mld_reg_RINGOSC_32_CONFIG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT40G0_INT_EN_write( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G0_INT_EN_write( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT40G0_INT_EN_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTN_MLD_REG_HADAPT40G0_INT_EN,
                       value);
}

static INLINE void otn_mld_reg_HADAPT40G0_INT_EN_field_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G0_INT_EN_field_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT40G0_INT_EN_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTN_MLD_REG_HADAPT40G0_INT_EN,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT40G0_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT40G0_INT_EN_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G0_INT_EN_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTN_MLD_REG_HADAPT40G0_INT_EN);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G0_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT40G1_INT_EN_write( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G1_INT_EN_write( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT40G1_INT_EN_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTN_MLD_REG_HADAPT40G1_INT_EN,
                       value);
}

static INLINE void otn_mld_reg_HADAPT40G1_INT_EN_field_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G1_INT_EN_field_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT40G1_INT_EN_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTN_MLD_REG_HADAPT40G1_INT_EN,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT40G1_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT40G1_INT_EN_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G1_INT_EN_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTN_MLD_REG_HADAPT40G1_INT_EN);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G1_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void otn_mld_reg_HADAPT100G_INT_EN_write( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_INT_EN_write( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT100G_INT_EN_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTN_MLD_REG_HADAPT100G_INT_EN,
                       value);
}

static INLINE void otn_mld_reg_HADAPT100G_INT_EN_field_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_INT_EN_field_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT100G_INT_EN_field_set", mask, ofs, value );
    otn_mld_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTN_MLD_REG_HADAPT100G_INT_EN,
                       mask,
                       PMC_OTN_MLD_REG_HADAPT100G_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT100G_INT_EN_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_INT_EN_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTN_MLD_REG_HADAPT100G_INT_EN);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE UINT32 otn_mld_reg_HADAPT_INT_SUMMARY_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT_INT_SUMMARY_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTN_MLD_REG_HADAPT_INT_SUMMARY);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT_INT_SUMMARY_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT_INT_SUMMARY_poll( otn_mld_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT_INT_SUMMARY_poll( otn_mld_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otn_mld_reg_HADAPT_INT_SUMMARY_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otn_mld_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_OTN_MLD_REG_HADAPT_INT_SUMMARY,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void otn_mld_reg_HADAPT40G0_INT_write( otn_mld_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G0_INT_write( otn_mld_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT40G0_INT_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTN_MLD_REG_HADAPT40G0_INT,
                       value);
}

static INLINE void otn_mld_reg_HADAPT40G0_INT_action_on_write_field_set( otn_mld_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G0_INT_action_on_write_field_set( otn_mld_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT40G0_INT_action_on_write_field_set", mask, ofs, value );
    otn_mld_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OTN_MLD_REG_HADAPT40G0_INT,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT40G0_INT_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G0_INT_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTN_MLD_REG_HADAPT40G0_INT);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G0_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT40G0_INT_poll( otn_mld_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT40G0_INT_poll( otn_mld_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otn_mld_reg_HADAPT40G0_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otn_mld_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_OTN_MLD_REG_HADAPT40G0_INT,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void otn_mld_reg_HADAPT40G1_INT_write( otn_mld_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G1_INT_write( otn_mld_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT40G1_INT_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTN_MLD_REG_HADAPT40G1_INT,
                       value);
}

static INLINE void otn_mld_reg_HADAPT40G1_INT_action_on_write_field_set( otn_mld_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT40G1_INT_action_on_write_field_set( otn_mld_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT40G1_INT_action_on_write_field_set", mask, ofs, value );
    otn_mld_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OTN_MLD_REG_HADAPT40G1_INT,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT40G1_INT_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G1_INT_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTN_MLD_REG_HADAPT40G1_INT);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G1_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT40G1_INT_poll( otn_mld_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT40G1_INT_poll( otn_mld_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otn_mld_reg_HADAPT40G1_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otn_mld_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_OTN_MLD_REG_HADAPT40G1_INT,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE void otn_mld_reg_HADAPT100G_INT_write( otn_mld_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_INT_write( otn_mld_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "otn_mld_reg_HADAPT100G_INT_write", value );
    otn_mld_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OTN_MLD_REG_HADAPT100G_INT,
                       value);
}

static INLINE void otn_mld_reg_HADAPT100G_INT_action_on_write_field_set( otn_mld_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_reg_HADAPT100G_INT_action_on_write_field_set( otn_mld_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "otn_mld_reg_HADAPT100G_INT_action_on_write_field_set", mask, ofs, value );
    otn_mld_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_OTN_MLD_REG_HADAPT100G_INT,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 otn_mld_reg_HADAPT100G_INT_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_INT_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTN_MLD_REG_HADAPT100G_INT);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_INT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT100G_INT_poll( otn_mld_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT100G_INT_poll( otn_mld_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otn_mld_reg_HADAPT100G_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otn_mld_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_OTN_MLD_REG_HADAPT100G_INT,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE UINT32 otn_mld_reg_HADAPT40G0_INT_VAL_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G0_INT_VAL_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTN_MLD_REG_HADAPT40G0_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G0_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT40G0_INT_VAL_poll( otn_mld_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT40G0_INT_VAL_poll( otn_mld_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otn_mld_reg_HADAPT40G0_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otn_mld_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_OTN_MLD_REG_HADAPT40G0_INT_VAL,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE UINT32 otn_mld_reg_HADAPT40G1_INT_VAL_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G1_INT_VAL_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTN_MLD_REG_HADAPT40G1_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G1_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT40G1_INT_VAL_poll( otn_mld_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT40G1_INT_VAL_poll( otn_mld_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otn_mld_reg_HADAPT40G1_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otn_mld_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_OTN_MLD_REG_HADAPT40G1_INT_VAL,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE UINT32 otn_mld_reg_HADAPT100G_INT_VAL_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_INT_VAL_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTN_MLD_REG_HADAPT100G_INT_VAL);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_INT_VAL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT100G_INT_VAL_poll( otn_mld_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT100G_INT_VAL_poll( otn_mld_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "otn_mld_reg_HADAPT100G_INT_VAL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return otn_mld_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_OTN_MLD_REG_HADAPT100G_INT_VAL,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE UINT32 otn_mld_reg_INT_SUMMARY_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_INT_SUMMARY_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTN_MLD_REG_INT_SUMMARY);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_INT_SUMMARY_read", reg_value);
    return reg_value;
}

static INLINE UINT32 otn_mld_reg_HADAPT40G0_STAT_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G0_STAT_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTN_MLD_REG_HADAPT40G0_STAT);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G0_STAT_read", reg_value);
    return reg_value;
}

static INLINE UINT32 otn_mld_reg_HADAPT40G1_STAT_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT40G1_STAT_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTN_MLD_REG_HADAPT40G1_STAT);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT40G1_STAT_read", reg_value);
    return reg_value;
}

static INLINE UINT32 otn_mld_reg_HADAPT100G_STAT_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_reg_HADAPT100G_STAT_read( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = otn_mld_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_OTN_MLD_REG_HADAPT100G_STAT);

    IOLOG( "%s -> 0x%08x;", "otn_mld_reg_HADAPT100G_STAT_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void otn_mld_field_SFIS_N10_ENABLE_N5_set( otn_mld_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_SFIS_N10_ENABLE_N5_set( otn_mld_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_SFIS_N10_ENABLE_N5_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_SFIS_N10_ENABLE_N5_set", value );

    /* (0x00010000 bits 6) field SFIS_N10_ENABLE_N5 of register PMC_OTN_MLD_REG_MASTER_CONFIG */
    otn_mld_reg_MASTER_CONFIG_field_set( b_ptr,
                                         h_ptr,
                                         OTN_MLD_REG_MASTER_CONFIG_BIT_SFIS_N10_ENABLE_N5_MSK,
                                         OTN_MLD_REG_MASTER_CONFIG_BIT_SFIS_N10_ENABLE_N5_OFF,
                                         value);
}

static INLINE UINT32 otn_mld_field_SFIS_N10_ENABLE_N5_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_SFIS_N10_ENABLE_N5_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010000 bits 6) field SFIS_N10_ENABLE_N5 of register PMC_OTN_MLD_REG_MASTER_CONFIG */
    reg_value = otn_mld_reg_MASTER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MASTER_CONFIG_BIT_SFIS_N10_ENABLE_N5_MSK) >> OTN_MLD_REG_MASTER_CONFIG_BIT_SFIS_N10_ENABLE_N5_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_SFIS_N10_ENABLE_N5_get", value );

    return value;
}
static INLINE void otn_mld_field_OTU4_SEL_set( otn_mld_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_OTU4_SEL_set( otn_mld_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_OTU4_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_OTU4_SEL_set", value );

    /* (0x00010000 bits 4) field OTU4_SEL of register PMC_OTN_MLD_REG_MASTER_CONFIG */
    otn_mld_reg_MASTER_CONFIG_field_set( b_ptr,
                                         h_ptr,
                                         OTN_MLD_REG_MASTER_CONFIG_BIT_OTU4_SEL_MSK,
                                         OTN_MLD_REG_MASTER_CONFIG_BIT_OTU4_SEL_OFF,
                                         value);
}

static INLINE UINT32 otn_mld_field_OTU4_SEL_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_OTU4_SEL_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010000 bits 4) field OTU4_SEL of register PMC_OTN_MLD_REG_MASTER_CONFIG */
    reg_value = otn_mld_reg_MASTER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MASTER_CONFIG_BIT_OTU4_SEL_MSK) >> OTN_MLD_REG_MASTER_CONFIG_BIT_OTU4_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_OTU4_SEL_get", value );

    return value;
}
static INLINE void otn_mld_field_HADAPT40G_RESET_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G_RESET_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_HADAPT40G_RESET_set", value, 3);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G_RESET_set", value );

    /* (0x00010000 bits 3:2) bits 0:1 use field HADAPT40G_RESET of register PMC_OTN_MLD_REG_MASTER_CONFIG */
    otn_mld_reg_MASTER_CONFIG_field_set( b_ptr,
                                         h_ptr,
                                         OTN_MLD_REG_MASTER_CONFIG_BIT_HADAPT40G_RESET_MSK,
                                         OTN_MLD_REG_MASTER_CONFIG_BIT_HADAPT40G_RESET_OFF,
                                         value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G_RESET_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G_RESET_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010000 bits 3:2) bits 0:1 use field HADAPT40G_RESET of register PMC_OTN_MLD_REG_MASTER_CONFIG */
    reg_value = otn_mld_reg_MASTER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MASTER_CONFIG_BIT_HADAPT40G_RESET_MSK) >> OTN_MLD_REG_MASTER_CONFIG_BIT_HADAPT40G_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G_RESET_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT40G_RESET_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT40G_RESET_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G_RESET_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G_RESET_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G_RESET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010000 bits 3:2) bits 0:1 use field HADAPT40G_RESET of register PMC_OTN_MLD_REG_MASTER_CONFIG */
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
        /* (0x00010000 bits 3:2) bits 0:1 use field HADAPT40G_RESET of register PMC_OTN_MLD_REG_MASTER_CONFIG */
        otn_mld_reg_MASTER_CONFIG_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (OTN_MLD_REG_MASTER_CONFIG_BIT_HADAPT40G_RESET_OFF + subfield_offset),
                                             OTN_MLD_REG_MASTER_CONFIG_BIT_HADAPT40G_RESET_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT40G_RESET_get( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G_RESET_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G_RESET_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G_RESET_get", stop_bit, 1 );
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
    /* (0x00010000 bits 3:2) bits 0:1 use field HADAPT40G_RESET of register PMC_OTN_MLD_REG_MASTER_CONFIG */
    reg_value = otn_mld_reg_MASTER_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_MASTER_CONFIG_BIT_HADAPT40G_RESET_MSK)
                  >> OTN_MLD_REG_MASTER_CONFIG_BIT_HADAPT40G_RESET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_MASTER_CONFIG_BIT_HADAPT40G_RESET_MSK, OTN_MLD_REG_MASTER_CONFIG_BIT_HADAPT40G_RESET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G_RESET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_RESET_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_RESET_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_HADAPT100G_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_RESET_set", value );

    /* (0x00010000 bits 1) field HADAPT100G_RESET of register PMC_OTN_MLD_REG_MASTER_CONFIG */
    otn_mld_reg_MASTER_CONFIG_field_set( b_ptr,
                                         h_ptr,
                                         OTN_MLD_REG_MASTER_CONFIG_BIT_HADAPT100G_RESET_MSK,
                                         OTN_MLD_REG_MASTER_CONFIG_BIT_HADAPT100G_RESET_OFF,
                                         value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_RESET_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_RESET_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010000 bits 1) field HADAPT100G_RESET of register PMC_OTN_MLD_REG_MASTER_CONFIG */
    reg_value = otn_mld_reg_MASTER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MASTER_CONFIG_BIT_HADAPT100G_RESET_MSK) >> OTN_MLD_REG_MASTER_CONFIG_BIT_HADAPT100G_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_RESET_get", value );

    return value;
}
static INLINE void otn_mld_field_MLD_TOP_RESET_set( otn_mld_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_MLD_TOP_RESET_set( otn_mld_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_MLD_TOP_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_MLD_TOP_RESET_set", value );

    /* (0x00010000 bits 0) field MLD_TOP_RESET of register PMC_OTN_MLD_REG_MASTER_CONFIG */
    otn_mld_reg_MASTER_CONFIG_field_set( b_ptr,
                                         h_ptr,
                                         OTN_MLD_REG_MASTER_CONFIG_BIT_MLD_TOP_RESET_MSK,
                                         OTN_MLD_REG_MASTER_CONFIG_BIT_MLD_TOP_RESET_OFF,
                                         value);
}

static INLINE UINT32 otn_mld_field_MLD_TOP_RESET_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_MLD_TOP_RESET_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010000 bits 0) field MLD_TOP_RESET of register PMC_OTN_MLD_REG_MASTER_CONFIG */
    reg_value = otn_mld_reg_MASTER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MASTER_CONFIG_BIT_MLD_TOP_RESET_MSK) >> OTN_MLD_REG_MASTER_CONFIG_BIT_MLD_TOP_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_MLD_TOP_RESET_get", value );

    return value;
}
static INLINE void otn_mld_field_SFI51_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_SFI51_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_SFI51_LOWPWR_set", value, 3);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_SFI51_LOWPWR_set", value );

    /* (0x00010004 bits 13:12) bits 0:1 use field SFI51_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
    otn_mld_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                         h_ptr,
                                         OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFI51_LOWPWR_MSK,
                                         OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFI51_LOWPWR_OFF,
                                         value);
}

static INLINE UINT32 otn_mld_field_SFI51_LOWPWR_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_SFI51_LOWPWR_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010004 bits 13:12) bits 0:1 use field SFI51_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
    reg_value = otn_mld_reg_LOWPWR_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFI51_LOWPWR_MSK) >> OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFI51_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_SFI51_LOWPWR_get", value );

    return value;
}
static INLINE void otn_mld_field_range_SFI51_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_SFI51_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_SFI51_LOWPWR_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_SFI51_LOWPWR_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_SFI51_LOWPWR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010004 bits 13:12) bits 0:1 use field SFI51_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
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
        /* (0x00010004 bits 13:12) bits 0:1 use field SFI51_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
        otn_mld_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFI51_LOWPWR_OFF + subfield_offset),
                                             OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFI51_LOWPWR_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_SFI51_LOWPWR_get( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_SFI51_LOWPWR_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_SFI51_LOWPWR_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_SFI51_LOWPWR_get", stop_bit, 1 );
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
    /* (0x00010004 bits 13:12) bits 0:1 use field SFI51_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
    reg_value = otn_mld_reg_LOWPWR_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFI51_LOWPWR_MSK)
                  >> OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFI51_LOWPWR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFI51_LOWPWR_MSK, OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFI51_LOWPWR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_SFI51_LOWPWR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_SFIS_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_SFIS_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_SFIS_LOWPWR_set", value, 15);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_SFIS_LOWPWR_set", value );

    /* (0x00010004 bits 11:8) bits 0:3 use field SFIS_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
    otn_mld_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                         h_ptr,
                                         OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFIS_LOWPWR_MSK,
                                         OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFIS_LOWPWR_OFF,
                                         value);
}

static INLINE UINT32 otn_mld_field_SFIS_LOWPWR_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_SFIS_LOWPWR_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010004 bits 11:8) bits 0:3 use field SFIS_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
    reg_value = otn_mld_reg_LOWPWR_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFIS_LOWPWR_MSK) >> OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFIS_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_SFIS_LOWPWR_get", value );

    return value;
}
static INLINE void otn_mld_field_range_SFIS_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_SFIS_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_SFIS_LOWPWR_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_SFIS_LOWPWR_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_SFIS_LOWPWR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010004 bits 11:8) bits 0:3 use field SFIS_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
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
        /* (0x00010004 bits 11:8) bits 0:3 use field SFIS_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
        otn_mld_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFIS_LOWPWR_OFF + subfield_offset),
                                             OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFIS_LOWPWR_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_SFIS_LOWPWR_get( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_SFIS_LOWPWR_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_SFIS_LOWPWR_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_SFIS_LOWPWR_get", stop_bit, 3 );
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
    /* (0x00010004 bits 11:8) bits 0:3 use field SFIS_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
    reg_value = otn_mld_reg_LOWPWR_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFIS_LOWPWR_MSK)
                  >> OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFIS_LOWPWR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFIS_LOWPWR_MSK, OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFIS_LOWPWR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_SFIS_LOWPWR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT40G_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_HADAPT40G_LOWPWR_set", value, 3);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G_LOWPWR_set", value );

    /* (0x00010004 bits 7:6) bits 0:1 use field HADAPT40G_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
    otn_mld_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                         h_ptr,
                                         OTN_MLD_REG_LOWPWR_CONFIG_BIT_HADAPT40G_LOWPWR_MSK,
                                         OTN_MLD_REG_LOWPWR_CONFIG_BIT_HADAPT40G_LOWPWR_OFF,
                                         value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G_LOWPWR_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G_LOWPWR_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010004 bits 7:6) bits 0:1 use field HADAPT40G_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
    reg_value = otn_mld_reg_LOWPWR_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_LOWPWR_CONFIG_BIT_HADAPT40G_LOWPWR_MSK) >> OTN_MLD_REG_LOWPWR_CONFIG_BIT_HADAPT40G_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G_LOWPWR_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT40G_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT40G_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G_LOWPWR_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G_LOWPWR_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G_LOWPWR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010004 bits 7:6) bits 0:1 use field HADAPT40G_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
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
        /* (0x00010004 bits 7:6) bits 0:1 use field HADAPT40G_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
        otn_mld_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (OTN_MLD_REG_LOWPWR_CONFIG_BIT_HADAPT40G_LOWPWR_OFF + subfield_offset),
                                             OTN_MLD_REG_LOWPWR_CONFIG_BIT_HADAPT40G_LOWPWR_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT40G_LOWPWR_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G_LOWPWR_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G_LOWPWR_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G_LOWPWR_get", stop_bit, 1 );
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
    /* (0x00010004 bits 7:6) bits 0:1 use field HADAPT40G_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
    reg_value = otn_mld_reg_LOWPWR_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_LOWPWR_CONFIG_BIT_HADAPT40G_LOWPWR_MSK)
                  >> OTN_MLD_REG_LOWPWR_CONFIG_BIT_HADAPT40G_LOWPWR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_LOWPWR_CONFIG_BIT_HADAPT40G_LOWPWR_MSK, OTN_MLD_REG_LOWPWR_CONFIG_BIT_HADAPT40G_LOWPWR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G_LOWPWR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_ADAPT100G_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_ADAPT100G_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_ADAPT100G_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_ADAPT100G_LOWPWR_set", value );

    /* (0x00010004 bits 5) field ADAPT100G_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
    otn_mld_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                         h_ptr,
                                         OTN_MLD_REG_LOWPWR_CONFIG_BIT_ADAPT100G_LOWPWR_MSK,
                                         OTN_MLD_REG_LOWPWR_CONFIG_BIT_ADAPT100G_LOWPWR_OFF,
                                         value);
}

static INLINE UINT32 otn_mld_field_ADAPT100G_LOWPWR_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_ADAPT100G_LOWPWR_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010004 bits 5) field ADAPT100G_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
    reg_value = otn_mld_reg_LOWPWR_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_LOWPWR_CONFIG_BIT_ADAPT100G_LOWPWR_MSK) >> OTN_MLD_REG_LOWPWR_CONFIG_BIT_ADAPT100G_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_ADAPT100G_LOWPWR_get", value );

    return value;
}
static INLINE void otn_mld_field_OPSM23_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_OPSM23_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_OPSM23_LOWPWR_set", value, 7);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_OPSM23_LOWPWR_set", value );

    /* (0x00010004 bits 4:2) bits 0:2 use field OPSM23_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
    otn_mld_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                         h_ptr,
                                         OTN_MLD_REG_LOWPWR_CONFIG_BIT_OPSM23_LOWPWR_MSK,
                                         OTN_MLD_REG_LOWPWR_CONFIG_BIT_OPSM23_LOWPWR_OFF,
                                         value);
}

static INLINE UINT32 otn_mld_field_OPSM23_LOWPWR_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_OPSM23_LOWPWR_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010004 bits 4:2) bits 0:2 use field OPSM23_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
    reg_value = otn_mld_reg_LOWPWR_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_LOWPWR_CONFIG_BIT_OPSM23_LOWPWR_MSK) >> OTN_MLD_REG_LOWPWR_CONFIG_BIT_OPSM23_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_OPSM23_LOWPWR_get", value );

    return value;
}
static INLINE void otn_mld_field_range_OPSM23_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_OPSM23_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_OPSM23_LOWPWR_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_OPSM23_LOWPWR_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_OPSM23_LOWPWR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010004 bits 4:2) bits 0:2 use field OPSM23_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
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
        /* (0x00010004 bits 4:2) bits 0:2 use field OPSM23_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
        otn_mld_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                             h_ptr,
                                             subfield_mask << (OTN_MLD_REG_LOWPWR_CONFIG_BIT_OPSM23_LOWPWR_OFF + subfield_offset),
                                             OTN_MLD_REG_LOWPWR_CONFIG_BIT_OPSM23_LOWPWR_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_OPSM23_LOWPWR_get( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_OPSM23_LOWPWR_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_OPSM23_LOWPWR_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_OPSM23_LOWPWR_get", stop_bit, 2 );
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
    /* (0x00010004 bits 4:2) bits 0:2 use field OPSM23_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
    reg_value = otn_mld_reg_LOWPWR_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_LOWPWR_CONFIG_BIT_OPSM23_LOWPWR_MSK)
                  >> OTN_MLD_REG_LOWPWR_CONFIG_BIT_OPSM23_LOWPWR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_LOWPWR_CONFIG_BIT_OPSM23_LOWPWR_MSK, OTN_MLD_REG_LOWPWR_CONFIG_BIT_OPSM23_LOWPWR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_OPSM23_LOWPWR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_OPSM4_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_OPSM4_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_OPSM4_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_OPSM4_LOWPWR_set", value );

    /* (0x00010004 bits 1) field OPSM4_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
    otn_mld_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                         h_ptr,
                                         OTN_MLD_REG_LOWPWR_CONFIG_BIT_OPSM4_LOWPWR_MSK,
                                         OTN_MLD_REG_LOWPWR_CONFIG_BIT_OPSM4_LOWPWR_OFF,
                                         value);
}

static INLINE UINT32 otn_mld_field_OPSM4_LOWPWR_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_OPSM4_LOWPWR_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010004 bits 1) field OPSM4_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
    reg_value = otn_mld_reg_LOWPWR_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_LOWPWR_CONFIG_BIT_OPSM4_LOWPWR_MSK) >> OTN_MLD_REG_LOWPWR_CONFIG_BIT_OPSM4_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_OPSM4_LOWPWR_get", value );

    return value;
}
static INLINE void otn_mld_field_MLD_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_MLD_LOWPWR_set( otn_mld_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_MLD_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_MLD_LOWPWR_set", value );

    /* (0x00010004 bits 0) field MLD_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
    otn_mld_reg_LOWPWR_CONFIG_field_set( b_ptr,
                                         h_ptr,
                                         OTN_MLD_REG_LOWPWR_CONFIG_BIT_MLD_LOWPWR_MSK,
                                         OTN_MLD_REG_LOWPWR_CONFIG_BIT_MLD_LOWPWR_OFF,
                                         value);
}

static INLINE UINT32 otn_mld_field_MLD_LOWPWR_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_MLD_LOWPWR_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010004 bits 0) field MLD_LOWPWR of register PMC_OTN_MLD_REG_LOWPWR_CONFIG */
    reg_value = otn_mld_reg_LOWPWR_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_LOWPWR_CONFIG_BIT_MLD_LOWPWR_MSK) >> OTN_MLD_REG_LOWPWR_CONFIG_BIT_MLD_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_MLD_LOWPWR_get", value );

    return value;
}
static INLINE void otn_mld_field_MUX0_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_MUX0_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_MUX0_LINE_STREAM_set", value, 3);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_MUX0_LINE_STREAM_set", value );

    /* (0x00010068 bits 1:0) bits 0:1 use field MUX0_LINE_STREAM of register PMC_OTN_MLD_REG_MUX0_CONFIG */
    otn_mld_reg_MUX0_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       OTN_MLD_REG_MUX0_CONFIG_BIT_MUX0_LINE_STREAM_MSK,
                                       OTN_MLD_REG_MUX0_CONFIG_BIT_MUX0_LINE_STREAM_OFF,
                                       value);
}

static INLINE UINT32 otn_mld_field_MUX0_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_MUX0_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010068 bits 1:0) bits 0:1 use field MUX0_LINE_STREAM of register PMC_OTN_MLD_REG_MUX0_CONFIG */
    reg_value = otn_mld_reg_MUX0_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MUX0_CONFIG_BIT_MUX0_LINE_STREAM_MSK) >> OTN_MLD_REG_MUX0_CONFIG_BIT_MUX0_LINE_STREAM_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_MUX0_LINE_STREAM_get", value );

    return value;
}
static INLINE void otn_mld_field_range_MUX0_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_MUX0_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX0_LINE_STREAM_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX0_LINE_STREAM_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX0_LINE_STREAM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010068 bits 1:0) bits 0:1 use field MUX0_LINE_STREAM of register PMC_OTN_MLD_REG_MUX0_CONFIG */
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
        /* (0x00010068 bits 1:0) bits 0:1 use field MUX0_LINE_STREAM of register PMC_OTN_MLD_REG_MUX0_CONFIG */
        otn_mld_reg_MUX0_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (OTN_MLD_REG_MUX0_CONFIG_BIT_MUX0_LINE_STREAM_OFF + subfield_offset),
                                           OTN_MLD_REG_MUX0_CONFIG_BIT_MUX0_LINE_STREAM_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_MUX0_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_MUX0_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX0_LINE_STREAM_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX0_LINE_STREAM_get", stop_bit, 1 );
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
    /* (0x00010068 bits 1:0) bits 0:1 use field MUX0_LINE_STREAM of register PMC_OTN_MLD_REG_MUX0_CONFIG */
    reg_value = otn_mld_reg_MUX0_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_MUX0_CONFIG_BIT_MUX0_LINE_STREAM_MSK)
                  >> OTN_MLD_REG_MUX0_CONFIG_BIT_MUX0_LINE_STREAM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_MUX0_CONFIG_BIT_MUX0_LINE_STREAM_MSK, OTN_MLD_REG_MUX0_CONFIG_BIT_MUX0_LINE_STREAM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX0_LINE_STREAM_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_MUX1_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_MUX1_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_MUX1_LINE_STREAM_set", value, 3);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_MUX1_LINE_STREAM_set", value );

    /* (0x0001006c bits 1:0) bits 0:1 use field MUX1_LINE_STREAM of register PMC_OTN_MLD_REG_MUX1_CONFIG */
    otn_mld_reg_MUX1_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       OTN_MLD_REG_MUX1_CONFIG_BIT_MUX1_LINE_STREAM_MSK,
                                       OTN_MLD_REG_MUX1_CONFIG_BIT_MUX1_LINE_STREAM_OFF,
                                       value);
}

static INLINE UINT32 otn_mld_field_MUX1_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_MUX1_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001006c bits 1:0) bits 0:1 use field MUX1_LINE_STREAM of register PMC_OTN_MLD_REG_MUX1_CONFIG */
    reg_value = otn_mld_reg_MUX1_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MUX1_CONFIG_BIT_MUX1_LINE_STREAM_MSK) >> OTN_MLD_REG_MUX1_CONFIG_BIT_MUX1_LINE_STREAM_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_MUX1_LINE_STREAM_get", value );

    return value;
}
static INLINE void otn_mld_field_range_MUX1_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_MUX1_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX1_LINE_STREAM_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX1_LINE_STREAM_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX1_LINE_STREAM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001006c bits 1:0) bits 0:1 use field MUX1_LINE_STREAM of register PMC_OTN_MLD_REG_MUX1_CONFIG */
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
        /* (0x0001006c bits 1:0) bits 0:1 use field MUX1_LINE_STREAM of register PMC_OTN_MLD_REG_MUX1_CONFIG */
        otn_mld_reg_MUX1_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (OTN_MLD_REG_MUX1_CONFIG_BIT_MUX1_LINE_STREAM_OFF + subfield_offset),
                                           OTN_MLD_REG_MUX1_CONFIG_BIT_MUX1_LINE_STREAM_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_MUX1_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_MUX1_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX1_LINE_STREAM_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX1_LINE_STREAM_get", stop_bit, 1 );
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
    /* (0x0001006c bits 1:0) bits 0:1 use field MUX1_LINE_STREAM of register PMC_OTN_MLD_REG_MUX1_CONFIG */
    reg_value = otn_mld_reg_MUX1_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_MUX1_CONFIG_BIT_MUX1_LINE_STREAM_MSK)
                  >> OTN_MLD_REG_MUX1_CONFIG_BIT_MUX1_LINE_STREAM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_MUX1_CONFIG_BIT_MUX1_LINE_STREAM_MSK, OTN_MLD_REG_MUX1_CONFIG_BIT_MUX1_LINE_STREAM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX1_LINE_STREAM_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_MUX2_SYS_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_MUX2_SYS_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_MUX2_SYS_STREAM_set", value, 7);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_MUX2_SYS_STREAM_set", value );

    /* (0x00010070 bits 3:1) bits 0:2 use field MUX2_SYS_STREAM of register PMC_OTN_MLD_REG_MUX2_CONFIG */
    otn_mld_reg_MUX2_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       OTN_MLD_REG_MUX2_CONFIG_BIT_MUX2_SYS_STREAM_MSK,
                                       OTN_MLD_REG_MUX2_CONFIG_BIT_MUX2_SYS_STREAM_OFF,
                                       value);
}

static INLINE UINT32 otn_mld_field_MUX2_SYS_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_MUX2_SYS_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010070 bits 3:1) bits 0:2 use field MUX2_SYS_STREAM of register PMC_OTN_MLD_REG_MUX2_CONFIG */
    reg_value = otn_mld_reg_MUX2_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MUX2_CONFIG_BIT_MUX2_SYS_STREAM_MSK) >> OTN_MLD_REG_MUX2_CONFIG_BIT_MUX2_SYS_STREAM_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_MUX2_SYS_STREAM_get", value );

    return value;
}
static INLINE void otn_mld_field_range_MUX2_SYS_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_MUX2_SYS_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX2_SYS_STREAM_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX2_SYS_STREAM_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX2_SYS_STREAM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010070 bits 3:1) bits 0:2 use field MUX2_SYS_STREAM of register PMC_OTN_MLD_REG_MUX2_CONFIG */
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
        /* (0x00010070 bits 3:1) bits 0:2 use field MUX2_SYS_STREAM of register PMC_OTN_MLD_REG_MUX2_CONFIG */
        otn_mld_reg_MUX2_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (OTN_MLD_REG_MUX2_CONFIG_BIT_MUX2_SYS_STREAM_OFF + subfield_offset),
                                           OTN_MLD_REG_MUX2_CONFIG_BIT_MUX2_SYS_STREAM_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_MUX2_SYS_STREAM_get( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_MUX2_SYS_STREAM_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX2_SYS_STREAM_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX2_SYS_STREAM_get", stop_bit, 2 );
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
    /* (0x00010070 bits 3:1) bits 0:2 use field MUX2_SYS_STREAM of register PMC_OTN_MLD_REG_MUX2_CONFIG */
    reg_value = otn_mld_reg_MUX2_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_MUX2_CONFIG_BIT_MUX2_SYS_STREAM_MSK)
                  >> OTN_MLD_REG_MUX2_CONFIG_BIT_MUX2_SYS_STREAM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_MUX2_CONFIG_BIT_MUX2_SYS_STREAM_MSK, OTN_MLD_REG_MUX2_CONFIG_BIT_MUX2_SYS_STREAM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX2_SYS_STREAM_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_MUX3_LINERX_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_MUX3_LINERX_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_MUX3_LINERX_STREAM_set", value, 3);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_MUX3_LINERX_STREAM_set", value );

    /* (0x00010074 bits 1:0) bits 0:1 use field MUX3_LINERX_STREAM of register PMC_OTN_MLD_REG_MUX3_CONFIG */
    otn_mld_reg_MUX3_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       OTN_MLD_REG_MUX3_CONFIG_BIT_MUX3_LINERX_STREAM_MSK,
                                       OTN_MLD_REG_MUX3_CONFIG_BIT_MUX3_LINERX_STREAM_OFF,
                                       value);
}

static INLINE UINT32 otn_mld_field_MUX3_LINERX_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_MUX3_LINERX_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010074 bits 1:0) bits 0:1 use field MUX3_LINERX_STREAM of register PMC_OTN_MLD_REG_MUX3_CONFIG */
    reg_value = otn_mld_reg_MUX3_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MUX3_CONFIG_BIT_MUX3_LINERX_STREAM_MSK) >> OTN_MLD_REG_MUX3_CONFIG_BIT_MUX3_LINERX_STREAM_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_MUX3_LINERX_STREAM_get", value );

    return value;
}
static INLINE void otn_mld_field_range_MUX3_LINERX_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_MUX3_LINERX_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX3_LINERX_STREAM_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX3_LINERX_STREAM_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX3_LINERX_STREAM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010074 bits 1:0) bits 0:1 use field MUX3_LINERX_STREAM of register PMC_OTN_MLD_REG_MUX3_CONFIG */
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
        /* (0x00010074 bits 1:0) bits 0:1 use field MUX3_LINERX_STREAM of register PMC_OTN_MLD_REG_MUX3_CONFIG */
        otn_mld_reg_MUX3_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (OTN_MLD_REG_MUX3_CONFIG_BIT_MUX3_LINERX_STREAM_OFF + subfield_offset),
                                           OTN_MLD_REG_MUX3_CONFIG_BIT_MUX3_LINERX_STREAM_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_MUX3_LINERX_STREAM_get( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_MUX3_LINERX_STREAM_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX3_LINERX_STREAM_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX3_LINERX_STREAM_get", stop_bit, 1 );
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
    /* (0x00010074 bits 1:0) bits 0:1 use field MUX3_LINERX_STREAM of register PMC_OTN_MLD_REG_MUX3_CONFIG */
    reg_value = otn_mld_reg_MUX3_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_MUX3_CONFIG_BIT_MUX3_LINERX_STREAM_MSK)
                  >> OTN_MLD_REG_MUX3_CONFIG_BIT_MUX3_LINERX_STREAM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_MUX3_CONFIG_BIT_MUX3_LINERX_STREAM_MSK, OTN_MLD_REG_MUX3_CONFIG_BIT_MUX3_LINERX_STREAM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX3_LINERX_STREAM_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_MUX4_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_MUX4_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_MUX4_LINE_STREAM_set", value, 3);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_MUX4_LINE_STREAM_set", value );

    /* (0x00010078 bits 1:0) bits 0:1 use field MUX4_LINE_STREAM of register PMC_OTN_MLD_REG_MUX4_CONFIG */
    otn_mld_reg_MUX4_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       OTN_MLD_REG_MUX4_CONFIG_BIT_MUX4_LINE_STREAM_MSK,
                                       OTN_MLD_REG_MUX4_CONFIG_BIT_MUX4_LINE_STREAM_OFF,
                                       value);
}

static INLINE UINT32 otn_mld_field_MUX4_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_MUX4_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010078 bits 1:0) bits 0:1 use field MUX4_LINE_STREAM of register PMC_OTN_MLD_REG_MUX4_CONFIG */
    reg_value = otn_mld_reg_MUX4_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MUX4_CONFIG_BIT_MUX4_LINE_STREAM_MSK) >> OTN_MLD_REG_MUX4_CONFIG_BIT_MUX4_LINE_STREAM_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_MUX4_LINE_STREAM_get", value );

    return value;
}
static INLINE void otn_mld_field_range_MUX4_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_MUX4_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX4_LINE_STREAM_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX4_LINE_STREAM_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX4_LINE_STREAM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010078 bits 1:0) bits 0:1 use field MUX4_LINE_STREAM of register PMC_OTN_MLD_REG_MUX4_CONFIG */
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
        /* (0x00010078 bits 1:0) bits 0:1 use field MUX4_LINE_STREAM of register PMC_OTN_MLD_REG_MUX4_CONFIG */
        otn_mld_reg_MUX4_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (OTN_MLD_REG_MUX4_CONFIG_BIT_MUX4_LINE_STREAM_OFF + subfield_offset),
                                           OTN_MLD_REG_MUX4_CONFIG_BIT_MUX4_LINE_STREAM_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_MUX4_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_MUX4_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX4_LINE_STREAM_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX4_LINE_STREAM_get", stop_bit, 1 );
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
    /* (0x00010078 bits 1:0) bits 0:1 use field MUX4_LINE_STREAM of register PMC_OTN_MLD_REG_MUX4_CONFIG */
    reg_value = otn_mld_reg_MUX4_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_MUX4_CONFIG_BIT_MUX4_LINE_STREAM_MSK)
                  >> OTN_MLD_REG_MUX4_CONFIG_BIT_MUX4_LINE_STREAM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_MUX4_CONFIG_BIT_MUX4_LINE_STREAM_MSK, OTN_MLD_REG_MUX4_CONFIG_BIT_MUX4_LINE_STREAM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX4_LINE_STREAM_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_MUX5_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_MUX5_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_MUX5_LINE_STREAM_set", value, 3);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_MUX5_LINE_STREAM_set", value );

    /* (0x0001007c bits 1:0) bits 0:1 use field MUX5_LINE_STREAM of register PMC_OTN_MLD_REG_MUX5_CONFIG */
    otn_mld_reg_MUX5_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       OTN_MLD_REG_MUX5_CONFIG_BIT_MUX5_LINE_STREAM_MSK,
                                       OTN_MLD_REG_MUX5_CONFIG_BIT_MUX5_LINE_STREAM_OFF,
                                       value);
}

static INLINE UINT32 otn_mld_field_MUX5_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_MUX5_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001007c bits 1:0) bits 0:1 use field MUX5_LINE_STREAM of register PMC_OTN_MLD_REG_MUX5_CONFIG */
    reg_value = otn_mld_reg_MUX5_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MUX5_CONFIG_BIT_MUX5_LINE_STREAM_MSK) >> OTN_MLD_REG_MUX5_CONFIG_BIT_MUX5_LINE_STREAM_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_MUX5_LINE_STREAM_get", value );

    return value;
}
static INLINE void otn_mld_field_range_MUX5_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_MUX5_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX5_LINE_STREAM_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX5_LINE_STREAM_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX5_LINE_STREAM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001007c bits 1:0) bits 0:1 use field MUX5_LINE_STREAM of register PMC_OTN_MLD_REG_MUX5_CONFIG */
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
        /* (0x0001007c bits 1:0) bits 0:1 use field MUX5_LINE_STREAM of register PMC_OTN_MLD_REG_MUX5_CONFIG */
        otn_mld_reg_MUX5_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (OTN_MLD_REG_MUX5_CONFIG_BIT_MUX5_LINE_STREAM_OFF + subfield_offset),
                                           OTN_MLD_REG_MUX5_CONFIG_BIT_MUX5_LINE_STREAM_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_MUX5_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_MUX5_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX5_LINE_STREAM_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX5_LINE_STREAM_get", stop_bit, 1 );
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
    /* (0x0001007c bits 1:0) bits 0:1 use field MUX5_LINE_STREAM of register PMC_OTN_MLD_REG_MUX5_CONFIG */
    reg_value = otn_mld_reg_MUX5_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_MUX5_CONFIG_BIT_MUX5_LINE_STREAM_MSK)
                  >> OTN_MLD_REG_MUX5_CONFIG_BIT_MUX5_LINE_STREAM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_MUX5_CONFIG_BIT_MUX5_LINE_STREAM_MSK, OTN_MLD_REG_MUX5_CONFIG_BIT_MUX5_LINE_STREAM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX5_LINE_STREAM_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_MUX6_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_MUX6_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_MUX6_LINE_STREAM_set", value, 3);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_MUX6_LINE_STREAM_set", value );

    /* (0x00010080 bits 1:0) bits 0:1 use field MUX6_LINE_STREAM of register PMC_OTN_MLD_REG_MUX6_CONFIG */
    otn_mld_reg_MUX6_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       OTN_MLD_REG_MUX6_CONFIG_BIT_MUX6_LINE_STREAM_MSK,
                                       OTN_MLD_REG_MUX6_CONFIG_BIT_MUX6_LINE_STREAM_OFF,
                                       value);
}

static INLINE UINT32 otn_mld_field_MUX6_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_MUX6_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010080 bits 1:0) bits 0:1 use field MUX6_LINE_STREAM of register PMC_OTN_MLD_REG_MUX6_CONFIG */
    reg_value = otn_mld_reg_MUX6_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MUX6_CONFIG_BIT_MUX6_LINE_STREAM_MSK) >> OTN_MLD_REG_MUX6_CONFIG_BIT_MUX6_LINE_STREAM_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_MUX6_LINE_STREAM_get", value );

    return value;
}
static INLINE void otn_mld_field_range_MUX6_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_MUX6_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX6_LINE_STREAM_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX6_LINE_STREAM_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX6_LINE_STREAM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010080 bits 1:0) bits 0:1 use field MUX6_LINE_STREAM of register PMC_OTN_MLD_REG_MUX6_CONFIG */
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
        /* (0x00010080 bits 1:0) bits 0:1 use field MUX6_LINE_STREAM of register PMC_OTN_MLD_REG_MUX6_CONFIG */
        otn_mld_reg_MUX6_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (OTN_MLD_REG_MUX6_CONFIG_BIT_MUX6_LINE_STREAM_OFF + subfield_offset),
                                           OTN_MLD_REG_MUX6_CONFIG_BIT_MUX6_LINE_STREAM_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_MUX6_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_MUX6_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX6_LINE_STREAM_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX6_LINE_STREAM_get", stop_bit, 1 );
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
    /* (0x00010080 bits 1:0) bits 0:1 use field MUX6_LINE_STREAM of register PMC_OTN_MLD_REG_MUX6_CONFIG */
    reg_value = otn_mld_reg_MUX6_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_MUX6_CONFIG_BIT_MUX6_LINE_STREAM_MSK)
                  >> OTN_MLD_REG_MUX6_CONFIG_BIT_MUX6_LINE_STREAM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_MUX6_CONFIG_BIT_MUX6_LINE_STREAM_MSK, OTN_MLD_REG_MUX6_CONFIG_BIT_MUX6_LINE_STREAM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX6_LINE_STREAM_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_MUX7_SYS_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_MUX7_SYS_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_MUX7_SYS_STREAM_set", value, 7);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_MUX7_SYS_STREAM_set", value );

    /* (0x00010084 bits 4:2) bits 0:2 use field MUX7_SYS_STREAM of register PMC_OTN_MLD_REG_MUX7_CONFIG */
    otn_mld_reg_MUX7_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_SYS_STREAM_MSK,
                                       OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_SYS_STREAM_OFF,
                                       value);
}

static INLINE UINT32 otn_mld_field_MUX7_SYS_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_MUX7_SYS_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010084 bits 4:2) bits 0:2 use field MUX7_SYS_STREAM of register PMC_OTN_MLD_REG_MUX7_CONFIG */
    reg_value = otn_mld_reg_MUX7_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_SYS_STREAM_MSK) >> OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_SYS_STREAM_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_MUX7_SYS_STREAM_get", value );

    return value;
}
static INLINE void otn_mld_field_range_MUX7_SYS_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_MUX7_SYS_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX7_SYS_STREAM_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX7_SYS_STREAM_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX7_SYS_STREAM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010084 bits 4:2) bits 0:2 use field MUX7_SYS_STREAM of register PMC_OTN_MLD_REG_MUX7_CONFIG */
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
        /* (0x00010084 bits 4:2) bits 0:2 use field MUX7_SYS_STREAM of register PMC_OTN_MLD_REG_MUX7_CONFIG */
        otn_mld_reg_MUX7_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_SYS_STREAM_OFF + subfield_offset),
                                           OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_SYS_STREAM_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_MUX7_SYS_STREAM_get( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_MUX7_SYS_STREAM_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX7_SYS_STREAM_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX7_SYS_STREAM_get", stop_bit, 2 );
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
    /* (0x00010084 bits 4:2) bits 0:2 use field MUX7_SYS_STREAM of register PMC_OTN_MLD_REG_MUX7_CONFIG */
    reg_value = otn_mld_reg_MUX7_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_SYS_STREAM_MSK)
                  >> OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_SYS_STREAM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_SYS_STREAM_MSK, OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_SYS_STREAM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX7_SYS_STREAM_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_MUX7_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_MUX7_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_MUX7_LINE_STREAM_set", value, 3);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_MUX7_LINE_STREAM_set", value );

    /* (0x00010084 bits 1:0) bits 0:1 use field MUX7_LINE_STREAM of register PMC_OTN_MLD_REG_MUX7_CONFIG */
    otn_mld_reg_MUX7_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_LINE_STREAM_MSK,
                                       OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_LINE_STREAM_OFF,
                                       value);
}

static INLINE UINT32 otn_mld_field_MUX7_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_MUX7_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010084 bits 1:0) bits 0:1 use field MUX7_LINE_STREAM of register PMC_OTN_MLD_REG_MUX7_CONFIG */
    reg_value = otn_mld_reg_MUX7_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_LINE_STREAM_MSK) >> OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_LINE_STREAM_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_MUX7_LINE_STREAM_get", value );

    return value;
}
static INLINE void otn_mld_field_range_MUX7_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_MUX7_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX7_LINE_STREAM_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX7_LINE_STREAM_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX7_LINE_STREAM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010084 bits 1:0) bits 0:1 use field MUX7_LINE_STREAM of register PMC_OTN_MLD_REG_MUX7_CONFIG */
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
        /* (0x00010084 bits 1:0) bits 0:1 use field MUX7_LINE_STREAM of register PMC_OTN_MLD_REG_MUX7_CONFIG */
        otn_mld_reg_MUX7_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_LINE_STREAM_OFF + subfield_offset),
                                           OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_LINE_STREAM_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_MUX7_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_MUX7_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX7_LINE_STREAM_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX7_LINE_STREAM_get", stop_bit, 1 );
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
    /* (0x00010084 bits 1:0) bits 0:1 use field MUX7_LINE_STREAM of register PMC_OTN_MLD_REG_MUX7_CONFIG */
    reg_value = otn_mld_reg_MUX7_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_LINE_STREAM_MSK)
                  >> OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_LINE_STREAM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_LINE_STREAM_MSK, OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_LINE_STREAM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX7_LINE_STREAM_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_MUX8_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_MUX8_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_MUX8_LINE_STREAM_set", value, 3);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_MUX8_LINE_STREAM_set", value );

    /* (0x00010088 bits 1:0) bits 0:1 use field MUX8_LINE_STREAM of register PMC_OTN_MLD_REG_MUX8_CONFIG */
    otn_mld_reg_MUX8_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       OTN_MLD_REG_MUX8_CONFIG_BIT_MUX8_LINE_STREAM_MSK,
                                       OTN_MLD_REG_MUX8_CONFIG_BIT_MUX8_LINE_STREAM_OFF,
                                       value);
}

static INLINE UINT32 otn_mld_field_MUX8_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_MUX8_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010088 bits 1:0) bits 0:1 use field MUX8_LINE_STREAM of register PMC_OTN_MLD_REG_MUX8_CONFIG */
    reg_value = otn_mld_reg_MUX8_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MUX8_CONFIG_BIT_MUX8_LINE_STREAM_MSK) >> OTN_MLD_REG_MUX8_CONFIG_BIT_MUX8_LINE_STREAM_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_MUX8_LINE_STREAM_get", value );

    return value;
}
static INLINE void otn_mld_field_range_MUX8_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_MUX8_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX8_LINE_STREAM_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX8_LINE_STREAM_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX8_LINE_STREAM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010088 bits 1:0) bits 0:1 use field MUX8_LINE_STREAM of register PMC_OTN_MLD_REG_MUX8_CONFIG */
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
        /* (0x00010088 bits 1:0) bits 0:1 use field MUX8_LINE_STREAM of register PMC_OTN_MLD_REG_MUX8_CONFIG */
        otn_mld_reg_MUX8_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (OTN_MLD_REG_MUX8_CONFIG_BIT_MUX8_LINE_STREAM_OFF + subfield_offset),
                                           OTN_MLD_REG_MUX8_CONFIG_BIT_MUX8_LINE_STREAM_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_MUX8_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_MUX8_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX8_LINE_STREAM_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX8_LINE_STREAM_get", stop_bit, 1 );
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
    /* (0x00010088 bits 1:0) bits 0:1 use field MUX8_LINE_STREAM of register PMC_OTN_MLD_REG_MUX8_CONFIG */
    reg_value = otn_mld_reg_MUX8_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_MUX8_CONFIG_BIT_MUX8_LINE_STREAM_MSK)
                  >> OTN_MLD_REG_MUX8_CONFIG_BIT_MUX8_LINE_STREAM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_MUX8_CONFIG_BIT_MUX8_LINE_STREAM_MSK, OTN_MLD_REG_MUX8_CONFIG_BIT_MUX8_LINE_STREAM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX8_LINE_STREAM_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_MUX9_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_MUX9_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_MUX9_LINE_STREAM_set", value, 3);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_MUX9_LINE_STREAM_set", value );

    /* (0x0001008c bits 1:0) bits 0:1 use field MUX9_LINE_STREAM of register PMC_OTN_MLD_REG_MUX9_CONFIG */
    otn_mld_reg_MUX9_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       OTN_MLD_REG_MUX9_CONFIG_BIT_MUX9_LINE_STREAM_MSK,
                                       OTN_MLD_REG_MUX9_CONFIG_BIT_MUX9_LINE_STREAM_OFF,
                                       value);
}

static INLINE UINT32 otn_mld_field_MUX9_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_MUX9_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001008c bits 1:0) bits 0:1 use field MUX9_LINE_STREAM of register PMC_OTN_MLD_REG_MUX9_CONFIG */
    reg_value = otn_mld_reg_MUX9_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MUX9_CONFIG_BIT_MUX9_LINE_STREAM_MSK) >> OTN_MLD_REG_MUX9_CONFIG_BIT_MUX9_LINE_STREAM_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_MUX9_LINE_STREAM_get", value );

    return value;
}
static INLINE void otn_mld_field_range_MUX9_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_MUX9_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX9_LINE_STREAM_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX9_LINE_STREAM_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX9_LINE_STREAM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001008c bits 1:0) bits 0:1 use field MUX9_LINE_STREAM of register PMC_OTN_MLD_REG_MUX9_CONFIG */
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
        /* (0x0001008c bits 1:0) bits 0:1 use field MUX9_LINE_STREAM of register PMC_OTN_MLD_REG_MUX9_CONFIG */
        otn_mld_reg_MUX9_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (OTN_MLD_REG_MUX9_CONFIG_BIT_MUX9_LINE_STREAM_OFF + subfield_offset),
                                           OTN_MLD_REG_MUX9_CONFIG_BIT_MUX9_LINE_STREAM_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_MUX9_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_MUX9_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX9_LINE_STREAM_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX9_LINE_STREAM_get", stop_bit, 1 );
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
    /* (0x0001008c bits 1:0) bits 0:1 use field MUX9_LINE_STREAM of register PMC_OTN_MLD_REG_MUX9_CONFIG */
    reg_value = otn_mld_reg_MUX9_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_MUX9_CONFIG_BIT_MUX9_LINE_STREAM_MSK)
                  >> OTN_MLD_REG_MUX9_CONFIG_BIT_MUX9_LINE_STREAM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_MUX9_CONFIG_BIT_MUX9_LINE_STREAM_MSK, OTN_MLD_REG_MUX9_CONFIG_BIT_MUX9_LINE_STREAM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX9_LINE_STREAM_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_MUX10_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_MUX10_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_MUX10_LINE_STREAM_set", value, 3);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_MUX10_LINE_STREAM_set", value );

    /* (0x00010090 bits 1:0) bits 0:1 use field MUX10_LINE_STREAM of register PMC_OTN_MLD_REG_MUX10_CONFIG */
    otn_mld_reg_MUX10_CONFIG_field_set( b_ptr,
                                        h_ptr,
                                        OTN_MLD_REG_MUX10_CONFIG_BIT_MUX10_LINE_STREAM_MSK,
                                        OTN_MLD_REG_MUX10_CONFIG_BIT_MUX10_LINE_STREAM_OFF,
                                        value);
}

static INLINE UINT32 otn_mld_field_MUX10_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_MUX10_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010090 bits 1:0) bits 0:1 use field MUX10_LINE_STREAM of register PMC_OTN_MLD_REG_MUX10_CONFIG */
    reg_value = otn_mld_reg_MUX10_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MUX10_CONFIG_BIT_MUX10_LINE_STREAM_MSK) >> OTN_MLD_REG_MUX10_CONFIG_BIT_MUX10_LINE_STREAM_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_MUX10_LINE_STREAM_get", value );

    return value;
}
static INLINE void otn_mld_field_range_MUX10_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_MUX10_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX10_LINE_STREAM_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX10_LINE_STREAM_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX10_LINE_STREAM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010090 bits 1:0) bits 0:1 use field MUX10_LINE_STREAM of register PMC_OTN_MLD_REG_MUX10_CONFIG */
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
        /* (0x00010090 bits 1:0) bits 0:1 use field MUX10_LINE_STREAM of register PMC_OTN_MLD_REG_MUX10_CONFIG */
        otn_mld_reg_MUX10_CONFIG_field_set( b_ptr,
                                            h_ptr,
                                            subfield_mask << (OTN_MLD_REG_MUX10_CONFIG_BIT_MUX10_LINE_STREAM_OFF + subfield_offset),
                                            OTN_MLD_REG_MUX10_CONFIG_BIT_MUX10_LINE_STREAM_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_MUX10_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_MUX10_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX10_LINE_STREAM_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX10_LINE_STREAM_get", stop_bit, 1 );
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
    /* (0x00010090 bits 1:0) bits 0:1 use field MUX10_LINE_STREAM of register PMC_OTN_MLD_REG_MUX10_CONFIG */
    reg_value = otn_mld_reg_MUX10_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_MUX10_CONFIG_BIT_MUX10_LINE_STREAM_MSK)
                  >> OTN_MLD_REG_MUX10_CONFIG_BIT_MUX10_LINE_STREAM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_MUX10_CONFIG_BIT_MUX10_LINE_STREAM_MSK, OTN_MLD_REG_MUX10_CONFIG_BIT_MUX10_LINE_STREAM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX10_LINE_STREAM_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_MUX11_SYS_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_MUX11_SYS_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_MUX11_SYS_STREAM_set", value, 7);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_MUX11_SYS_STREAM_set", value );

    /* (0x00010094 bits 4:2) bits 0:2 use field MUX11_SYS_STREAM of register PMC_OTN_MLD_REG_MUX11_CONFIG */
    otn_mld_reg_MUX11_CONFIG_field_set( b_ptr,
                                        h_ptr,
                                        OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_SYS_STREAM_MSK,
                                        OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_SYS_STREAM_OFF,
                                        value);
}

static INLINE UINT32 otn_mld_field_MUX11_SYS_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_MUX11_SYS_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010094 bits 4:2) bits 0:2 use field MUX11_SYS_STREAM of register PMC_OTN_MLD_REG_MUX11_CONFIG */
    reg_value = otn_mld_reg_MUX11_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_SYS_STREAM_MSK) >> OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_SYS_STREAM_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_MUX11_SYS_STREAM_get", value );

    return value;
}
static INLINE void otn_mld_field_range_MUX11_SYS_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_MUX11_SYS_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX11_SYS_STREAM_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX11_SYS_STREAM_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX11_SYS_STREAM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010094 bits 4:2) bits 0:2 use field MUX11_SYS_STREAM of register PMC_OTN_MLD_REG_MUX11_CONFIG */
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
        /* (0x00010094 bits 4:2) bits 0:2 use field MUX11_SYS_STREAM of register PMC_OTN_MLD_REG_MUX11_CONFIG */
        otn_mld_reg_MUX11_CONFIG_field_set( b_ptr,
                                            h_ptr,
                                            subfield_mask << (OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_SYS_STREAM_OFF + subfield_offset),
                                            OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_SYS_STREAM_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_MUX11_SYS_STREAM_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_MUX11_SYS_STREAM_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX11_SYS_STREAM_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX11_SYS_STREAM_get", stop_bit, 2 );
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
    /* (0x00010094 bits 4:2) bits 0:2 use field MUX11_SYS_STREAM of register PMC_OTN_MLD_REG_MUX11_CONFIG */
    reg_value = otn_mld_reg_MUX11_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_SYS_STREAM_MSK)
                  >> OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_SYS_STREAM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_SYS_STREAM_MSK, OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_SYS_STREAM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX11_SYS_STREAM_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_MUX11_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_MUX11_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_MUX11_LINE_STREAM_set", value, 3);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_MUX11_LINE_STREAM_set", value );

    /* (0x00010094 bits 1:0) bits 0:1 use field MUX11_LINE_STREAM of register PMC_OTN_MLD_REG_MUX11_CONFIG */
    otn_mld_reg_MUX11_CONFIG_field_set( b_ptr,
                                        h_ptr,
                                        OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_LINE_STREAM_MSK,
                                        OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_LINE_STREAM_OFF,
                                        value);
}

static INLINE UINT32 otn_mld_field_MUX11_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_MUX11_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010094 bits 1:0) bits 0:1 use field MUX11_LINE_STREAM of register PMC_OTN_MLD_REG_MUX11_CONFIG */
    reg_value = otn_mld_reg_MUX11_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_LINE_STREAM_MSK) >> OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_LINE_STREAM_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_MUX11_LINE_STREAM_get", value );

    return value;
}
static INLINE void otn_mld_field_range_MUX11_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_MUX11_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX11_LINE_STREAM_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX11_LINE_STREAM_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX11_LINE_STREAM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010094 bits 1:0) bits 0:1 use field MUX11_LINE_STREAM of register PMC_OTN_MLD_REG_MUX11_CONFIG */
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
        /* (0x00010094 bits 1:0) bits 0:1 use field MUX11_LINE_STREAM of register PMC_OTN_MLD_REG_MUX11_CONFIG */
        otn_mld_reg_MUX11_CONFIG_field_set( b_ptr,
                                            h_ptr,
                                            subfield_mask << (OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_LINE_STREAM_OFF + subfield_offset),
                                            OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_LINE_STREAM_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_MUX11_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_MUX11_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX11_LINE_STREAM_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX11_LINE_STREAM_get", stop_bit, 1 );
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
    /* (0x00010094 bits 1:0) bits 0:1 use field MUX11_LINE_STREAM of register PMC_OTN_MLD_REG_MUX11_CONFIG */
    reg_value = otn_mld_reg_MUX11_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_LINE_STREAM_MSK)
                  >> OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_LINE_STREAM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_LINE_STREAM_MSK, OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_LINE_STREAM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX11_LINE_STREAM_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_MUX12_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_MUX12_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_MUX12_LINE_STREAM_set", value, 3);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_MUX12_LINE_STREAM_set", value );

    /* (0x00010098 bits 1:0) bits 0:1 use field MUX12_LINE_STREAM of register PMC_OTN_MLD_REG_MUX12_CONFIG */
    otn_mld_reg_MUX12_CONFIG_field_set( b_ptr,
                                        h_ptr,
                                        OTN_MLD_REG_MUX12_CONFIG_BIT_MUX12_LINE_STREAM_MSK,
                                        OTN_MLD_REG_MUX12_CONFIG_BIT_MUX12_LINE_STREAM_OFF,
                                        value);
}

static INLINE UINT32 otn_mld_field_MUX12_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_MUX12_LINE_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010098 bits 1:0) bits 0:1 use field MUX12_LINE_STREAM of register PMC_OTN_MLD_REG_MUX12_CONFIG */
    reg_value = otn_mld_reg_MUX12_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MUX12_CONFIG_BIT_MUX12_LINE_STREAM_MSK) >> OTN_MLD_REG_MUX12_CONFIG_BIT_MUX12_LINE_STREAM_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_MUX12_LINE_STREAM_get", value );

    return value;
}
static INLINE void otn_mld_field_range_MUX12_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_MUX12_LINE_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX12_LINE_STREAM_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX12_LINE_STREAM_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX12_LINE_STREAM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010098 bits 1:0) bits 0:1 use field MUX12_LINE_STREAM of register PMC_OTN_MLD_REG_MUX12_CONFIG */
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
        /* (0x00010098 bits 1:0) bits 0:1 use field MUX12_LINE_STREAM of register PMC_OTN_MLD_REG_MUX12_CONFIG */
        otn_mld_reg_MUX12_CONFIG_field_set( b_ptr,
                                            h_ptr,
                                            subfield_mask << (OTN_MLD_REG_MUX12_CONFIG_BIT_MUX12_LINE_STREAM_OFF + subfield_offset),
                                            OTN_MLD_REG_MUX12_CONFIG_BIT_MUX12_LINE_STREAM_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_MUX12_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_MUX12_LINE_STREAM_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX12_LINE_STREAM_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX12_LINE_STREAM_get", stop_bit, 1 );
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
    /* (0x00010098 bits 1:0) bits 0:1 use field MUX12_LINE_STREAM of register PMC_OTN_MLD_REG_MUX12_CONFIG */
    reg_value = otn_mld_reg_MUX12_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_MUX12_CONFIG_BIT_MUX12_LINE_STREAM_MSK)
                  >> OTN_MLD_REG_MUX12_CONFIG_BIT_MUX12_LINE_STREAM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_MUX12_CONFIG_BIT_MUX12_LINE_STREAM_MSK, OTN_MLD_REG_MUX12_CONFIG_BIT_MUX12_LINE_STREAM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX12_LINE_STREAM_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_MUX13_SYS_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_MUX13_SYS_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_MUX13_SYS_STREAM_set", value, 7);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_MUX13_SYS_STREAM_set", value );

    /* (0x0001009c bits 3:1) bits 0:2 use field MUX13_SYS_STREAM of register PMC_OTN_MLD_REG_MUX13_CONFIG */
    otn_mld_reg_MUX13_CONFIG_field_set( b_ptr,
                                        h_ptr,
                                        OTN_MLD_REG_MUX13_CONFIG_BIT_MUX13_SYS_STREAM_MSK,
                                        OTN_MLD_REG_MUX13_CONFIG_BIT_MUX13_SYS_STREAM_OFF,
                                        value);
}

static INLINE UINT32 otn_mld_field_MUX13_SYS_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_MUX13_SYS_STREAM_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001009c bits 3:1) bits 0:2 use field MUX13_SYS_STREAM of register PMC_OTN_MLD_REG_MUX13_CONFIG */
    reg_value = otn_mld_reg_MUX13_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_MUX13_CONFIG_BIT_MUX13_SYS_STREAM_MSK) >> OTN_MLD_REG_MUX13_CONFIG_BIT_MUX13_SYS_STREAM_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_MUX13_SYS_STREAM_get", value );

    return value;
}
static INLINE void otn_mld_field_range_MUX13_SYS_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_MUX13_SYS_STREAM_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX13_SYS_STREAM_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX13_SYS_STREAM_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX13_SYS_STREAM_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001009c bits 3:1) bits 0:2 use field MUX13_SYS_STREAM of register PMC_OTN_MLD_REG_MUX13_CONFIG */
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
        /* (0x0001009c bits 3:1) bits 0:2 use field MUX13_SYS_STREAM of register PMC_OTN_MLD_REG_MUX13_CONFIG */
        otn_mld_reg_MUX13_CONFIG_field_set( b_ptr,
                                            h_ptr,
                                            subfield_mask << (OTN_MLD_REG_MUX13_CONFIG_BIT_MUX13_SYS_STREAM_OFF + subfield_offset),
                                            OTN_MLD_REG_MUX13_CONFIG_BIT_MUX13_SYS_STREAM_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_MUX13_SYS_STREAM_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_MUX13_SYS_STREAM_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_MUX13_SYS_STREAM_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_MUX13_SYS_STREAM_get", stop_bit, 2 );
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
    /* (0x0001009c bits 3:1) bits 0:2 use field MUX13_SYS_STREAM of register PMC_OTN_MLD_REG_MUX13_CONFIG */
    reg_value = otn_mld_reg_MUX13_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_MUX13_CONFIG_BIT_MUX13_SYS_STREAM_MSK)
                  >> OTN_MLD_REG_MUX13_CONFIG_BIT_MUX13_SYS_STREAM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_MUX13_CONFIG_BIT_MUX13_SYS_STREAM_MSK, OTN_MLD_REG_MUX13_CONFIG_BIT_MUX13_SYS_STREAM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_MUX13_SYS_STREAM_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT40G0_DPQPSK_MODE_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G0_DPQPSK_MODE_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_HADAPT40G0_DPQPSK_MODE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G0_DPQPSK_MODE_set", value );

    /* (0x00010318 bits 1) field HADAPT40G0_DPQPSK_MODE of register PMC_OTN_MLD_REG_HADAPT40G0_CONFIG */
    otn_mld_reg_HADAPT40G0_CONFIG_field_set( b_ptr,
                                             h_ptr,
                                             OTN_MLD_REG_HADAPT40G0_CONFIG_BIT_HADAPT40G0_DPQPSK_MODE_MSK,
                                             OTN_MLD_REG_HADAPT40G0_CONFIG_BIT_HADAPT40G0_DPQPSK_MODE_OFF,
                                             value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G0_DPQPSK_MODE_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G0_DPQPSK_MODE_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010318 bits 1) field HADAPT40G0_DPQPSK_MODE of register PMC_OTN_MLD_REG_HADAPT40G0_CONFIG */
    reg_value = otn_mld_reg_HADAPT40G0_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G0_CONFIG_BIT_HADAPT40G0_DPQPSK_MODE_MSK) >> OTN_MLD_REG_HADAPT40G0_CONFIG_BIT_HADAPT40G0_DPQPSK_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G0_DPQPSK_MODE_get", value );

    return value;
}
static INLINE void otn_mld_field_HADAPT40G0_PMC_TX_DATA_SEL_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G0_PMC_TX_DATA_SEL_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_HADAPT40G0_PMC_TX_DATA_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G0_PMC_TX_DATA_SEL_set", value );

    /* (0x00010318 bits 0) field HADAPT40G0_PMC_TX_DATA_SEL of register PMC_OTN_MLD_REG_HADAPT40G0_CONFIG */
    otn_mld_reg_HADAPT40G0_CONFIG_field_set( b_ptr,
                                             h_ptr,
                                             OTN_MLD_REG_HADAPT40G0_CONFIG_BIT_HADAPT40G0_PMC_TX_DATA_SEL_MSK,
                                             OTN_MLD_REG_HADAPT40G0_CONFIG_BIT_HADAPT40G0_PMC_TX_DATA_SEL_OFF,
                                             value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G0_PMC_TX_DATA_SEL_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G0_PMC_TX_DATA_SEL_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010318 bits 0) field HADAPT40G0_PMC_TX_DATA_SEL of register PMC_OTN_MLD_REG_HADAPT40G0_CONFIG */
    reg_value = otn_mld_reg_HADAPT40G0_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G0_CONFIG_BIT_HADAPT40G0_PMC_TX_DATA_SEL_MSK) >> OTN_MLD_REG_HADAPT40G0_CONFIG_BIT_HADAPT40G0_PMC_TX_DATA_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G0_PMC_TX_DATA_SEL_get", value );

    return value;
}
static INLINE void otn_mld_field_HADAPT40G0_MODE0_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G0_MODE0_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G0_MODE0_set", value );

    /* (0x0001031c bits 31:0) bits 0:31 use field HADAPT40G0_MODE0 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE0 */
    otn_mld_reg_HADAPT40G0_MODE0_field_set( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT40G0_MODE0_BIT_HADAPT40G0_MODE0_MSK,
                                            OTN_MLD_REG_HADAPT40G0_MODE0_BIT_HADAPT40G0_MODE0_OFF,
                                            value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G0_MODE0_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G0_MODE0_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001031c bits 31:0) bits 0:31 use field HADAPT40G0_MODE0 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE0 */
    reg_value = otn_mld_reg_HADAPT40G0_MODE0_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G0_MODE0_BIT_HADAPT40G0_MODE0_MSK) >> OTN_MLD_REG_HADAPT40G0_MODE0_BIT_HADAPT40G0_MODE0_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G0_MODE0_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT40G0_MODE0_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT40G0_MODE0_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G0_MODE0_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G0_MODE0_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G0_MODE0_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001031c bits 31:0) bits 0:31 use field HADAPT40G0_MODE0 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE0 */
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
        /* (0x0001031c bits 31:0) bits 0:31 use field HADAPT40G0_MODE0 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE0 */
        otn_mld_reg_HADAPT40G0_MODE0_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT40G0_MODE0_BIT_HADAPT40G0_MODE0_OFF + subfield_offset),
                                                OTN_MLD_REG_HADAPT40G0_MODE0_BIT_HADAPT40G0_MODE0_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT40G0_MODE0_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G0_MODE0_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G0_MODE0_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G0_MODE0_get", stop_bit, 31 );
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
    /* (0x0001031c bits 31:0) bits 0:31 use field HADAPT40G0_MODE0 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE0 */
    reg_value = otn_mld_reg_HADAPT40G0_MODE0_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT40G0_MODE0_BIT_HADAPT40G0_MODE0_MSK)
                  >> OTN_MLD_REG_HADAPT40G0_MODE0_BIT_HADAPT40G0_MODE0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT40G0_MODE0_BIT_HADAPT40G0_MODE0_MSK, OTN_MLD_REG_HADAPT40G0_MODE0_BIT_HADAPT40G0_MODE0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G0_MODE0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT40G0_MODE1_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G0_MODE1_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G0_MODE1_set", value );

    /* (0x00010320 bits 31:0) bits 0:31 use field HADAPT40G0_MODE1 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE1 */
    otn_mld_reg_HADAPT40G0_MODE1_field_set( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT40G0_MODE1_BIT_HADAPT40G0_MODE1_MSK,
                                            OTN_MLD_REG_HADAPT40G0_MODE1_BIT_HADAPT40G0_MODE1_OFF,
                                            value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G0_MODE1_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G0_MODE1_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010320 bits 31:0) bits 0:31 use field HADAPT40G0_MODE1 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE1 */
    reg_value = otn_mld_reg_HADAPT40G0_MODE1_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G0_MODE1_BIT_HADAPT40G0_MODE1_MSK) >> OTN_MLD_REG_HADAPT40G0_MODE1_BIT_HADAPT40G0_MODE1_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G0_MODE1_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT40G0_MODE1_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT40G0_MODE1_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G0_MODE1_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G0_MODE1_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G0_MODE1_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010320 bits 31:0) bits 0:31 use field HADAPT40G0_MODE1 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE1 */
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
        /* (0x00010320 bits 31:0) bits 0:31 use field HADAPT40G0_MODE1 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE1 */
        otn_mld_reg_HADAPT40G0_MODE1_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT40G0_MODE1_BIT_HADAPT40G0_MODE1_OFF + subfield_offset),
                                                OTN_MLD_REG_HADAPT40G0_MODE1_BIT_HADAPT40G0_MODE1_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT40G0_MODE1_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G0_MODE1_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G0_MODE1_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G0_MODE1_get", stop_bit, 31 );
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
    /* (0x00010320 bits 31:0) bits 0:31 use field HADAPT40G0_MODE1 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE1 */
    reg_value = otn_mld_reg_HADAPT40G0_MODE1_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT40G0_MODE1_BIT_HADAPT40G0_MODE1_MSK)
                  >> OTN_MLD_REG_HADAPT40G0_MODE1_BIT_HADAPT40G0_MODE1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT40G0_MODE1_BIT_HADAPT40G0_MODE1_MSK, OTN_MLD_REG_HADAPT40G0_MODE1_BIT_HADAPT40G0_MODE1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G0_MODE1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT40G0_MODE2_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G0_MODE2_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G0_MODE2_set", value );

    /* (0x00010324 bits 31:0) bits 0:31 use field HADAPT40G0_MODE2 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE2 */
    otn_mld_reg_HADAPT40G0_MODE2_field_set( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT40G0_MODE2_BIT_HADAPT40G0_MODE2_MSK,
                                            OTN_MLD_REG_HADAPT40G0_MODE2_BIT_HADAPT40G0_MODE2_OFF,
                                            value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G0_MODE2_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G0_MODE2_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010324 bits 31:0) bits 0:31 use field HADAPT40G0_MODE2 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE2 */
    reg_value = otn_mld_reg_HADAPT40G0_MODE2_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G0_MODE2_BIT_HADAPT40G0_MODE2_MSK) >> OTN_MLD_REG_HADAPT40G0_MODE2_BIT_HADAPT40G0_MODE2_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G0_MODE2_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT40G0_MODE2_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT40G0_MODE2_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G0_MODE2_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G0_MODE2_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G0_MODE2_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010324 bits 31:0) bits 0:31 use field HADAPT40G0_MODE2 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE2 */
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
        /* (0x00010324 bits 31:0) bits 0:31 use field HADAPT40G0_MODE2 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE2 */
        otn_mld_reg_HADAPT40G0_MODE2_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT40G0_MODE2_BIT_HADAPT40G0_MODE2_OFF + subfield_offset),
                                                OTN_MLD_REG_HADAPT40G0_MODE2_BIT_HADAPT40G0_MODE2_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT40G0_MODE2_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G0_MODE2_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G0_MODE2_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G0_MODE2_get", stop_bit, 31 );
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
    /* (0x00010324 bits 31:0) bits 0:31 use field HADAPT40G0_MODE2 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE2 */
    reg_value = otn_mld_reg_HADAPT40G0_MODE2_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT40G0_MODE2_BIT_HADAPT40G0_MODE2_MSK)
                  >> OTN_MLD_REG_HADAPT40G0_MODE2_BIT_HADAPT40G0_MODE2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT40G0_MODE2_BIT_HADAPT40G0_MODE2_MSK, OTN_MLD_REG_HADAPT40G0_MODE2_BIT_HADAPT40G0_MODE2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G0_MODE2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT40G0_MODE3_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G0_MODE3_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G0_MODE3_set", value );

    /* (0x00010328 bits 31:0) bits 0:31 use field HADAPT40G0_MODE3 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE3 */
    otn_mld_reg_HADAPT40G0_MODE3_field_set( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT40G0_MODE3_BIT_HADAPT40G0_MODE3_MSK,
                                            OTN_MLD_REG_HADAPT40G0_MODE3_BIT_HADAPT40G0_MODE3_OFF,
                                            value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G0_MODE3_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G0_MODE3_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010328 bits 31:0) bits 0:31 use field HADAPT40G0_MODE3 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE3 */
    reg_value = otn_mld_reg_HADAPT40G0_MODE3_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G0_MODE3_BIT_HADAPT40G0_MODE3_MSK) >> OTN_MLD_REG_HADAPT40G0_MODE3_BIT_HADAPT40G0_MODE3_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G0_MODE3_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT40G0_MODE3_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT40G0_MODE3_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G0_MODE3_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G0_MODE3_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G0_MODE3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010328 bits 31:0) bits 0:31 use field HADAPT40G0_MODE3 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE3 */
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
        /* (0x00010328 bits 31:0) bits 0:31 use field HADAPT40G0_MODE3 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE3 */
        otn_mld_reg_HADAPT40G0_MODE3_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT40G0_MODE3_BIT_HADAPT40G0_MODE3_OFF + subfield_offset),
                                                OTN_MLD_REG_HADAPT40G0_MODE3_BIT_HADAPT40G0_MODE3_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT40G0_MODE3_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G0_MODE3_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G0_MODE3_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G0_MODE3_get", stop_bit, 31 );
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
    /* (0x00010328 bits 31:0) bits 0:31 use field HADAPT40G0_MODE3 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE3 */
    reg_value = otn_mld_reg_HADAPT40G0_MODE3_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT40G0_MODE3_BIT_HADAPT40G0_MODE3_MSK)
                  >> OTN_MLD_REG_HADAPT40G0_MODE3_BIT_HADAPT40G0_MODE3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT40G0_MODE3_BIT_HADAPT40G0_MODE3_MSK, OTN_MLD_REG_HADAPT40G0_MODE3_BIT_HADAPT40G0_MODE3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G0_MODE3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT40G0_MODE4_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G0_MODE4_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G0_MODE4_set", value );

    /* (0x0001032c bits 31:0) bits 0:31 use field HADAPT40G0_MODE4 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE4 */
    otn_mld_reg_HADAPT40G0_MODE4_field_set( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT40G0_MODE4_BIT_HADAPT40G0_MODE4_MSK,
                                            OTN_MLD_REG_HADAPT40G0_MODE4_BIT_HADAPT40G0_MODE4_OFF,
                                            value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G0_MODE4_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G0_MODE4_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001032c bits 31:0) bits 0:31 use field HADAPT40G0_MODE4 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE4 */
    reg_value = otn_mld_reg_HADAPT40G0_MODE4_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G0_MODE4_BIT_HADAPT40G0_MODE4_MSK) >> OTN_MLD_REG_HADAPT40G0_MODE4_BIT_HADAPT40G0_MODE4_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G0_MODE4_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT40G0_MODE4_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT40G0_MODE4_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G0_MODE4_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G0_MODE4_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G0_MODE4_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001032c bits 31:0) bits 0:31 use field HADAPT40G0_MODE4 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE4 */
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
        /* (0x0001032c bits 31:0) bits 0:31 use field HADAPT40G0_MODE4 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE4 */
        otn_mld_reg_HADAPT40G0_MODE4_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT40G0_MODE4_BIT_HADAPT40G0_MODE4_OFF + subfield_offset),
                                                OTN_MLD_REG_HADAPT40G0_MODE4_BIT_HADAPT40G0_MODE4_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT40G0_MODE4_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G0_MODE4_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G0_MODE4_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G0_MODE4_get", stop_bit, 31 );
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
    /* (0x0001032c bits 31:0) bits 0:31 use field HADAPT40G0_MODE4 of register PMC_OTN_MLD_REG_HADAPT40G0_MODE4 */
    reg_value = otn_mld_reg_HADAPT40G0_MODE4_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT40G0_MODE4_BIT_HADAPT40G0_MODE4_MSK)
                  >> OTN_MLD_REG_HADAPT40G0_MODE4_BIT_HADAPT40G0_MODE4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT40G0_MODE4_BIT_HADAPT40G0_MODE4_MSK, OTN_MLD_REG_HADAPT40G0_MODE4_BIT_HADAPT40G0_MODE4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G0_MODE4_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT40G1_DPQPSK_MODE_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G1_DPQPSK_MODE_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_HADAPT40G1_DPQPSK_MODE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G1_DPQPSK_MODE_set", value );

    /* (0x00010340 bits 1) field HADAPT40G1_DPQPSK_MODE of register PMC_OTN_MLD_REG_HADAPT40G1_CONFIG */
    otn_mld_reg_HADAPT40G1_CONFIG_field_set( b_ptr,
                                             h_ptr,
                                             OTN_MLD_REG_HADAPT40G1_CONFIG_BIT_HADAPT40G1_DPQPSK_MODE_MSK,
                                             OTN_MLD_REG_HADAPT40G1_CONFIG_BIT_HADAPT40G1_DPQPSK_MODE_OFF,
                                             value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G1_DPQPSK_MODE_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G1_DPQPSK_MODE_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010340 bits 1) field HADAPT40G1_DPQPSK_MODE of register PMC_OTN_MLD_REG_HADAPT40G1_CONFIG */
    reg_value = otn_mld_reg_HADAPT40G1_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G1_CONFIG_BIT_HADAPT40G1_DPQPSK_MODE_MSK) >> OTN_MLD_REG_HADAPT40G1_CONFIG_BIT_HADAPT40G1_DPQPSK_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G1_DPQPSK_MODE_get", value );

    return value;
}
static INLINE void otn_mld_field_HADAPT40G1_PMC_TX_DATA_SEL_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G1_PMC_TX_DATA_SEL_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_HADAPT40G1_PMC_TX_DATA_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G1_PMC_TX_DATA_SEL_set", value );

    /* (0x00010340 bits 0) field HADAPT40G1_PMC_TX_DATA_SEL of register PMC_OTN_MLD_REG_HADAPT40G1_CONFIG */
    otn_mld_reg_HADAPT40G1_CONFIG_field_set( b_ptr,
                                             h_ptr,
                                             OTN_MLD_REG_HADAPT40G1_CONFIG_BIT_HADAPT40G1_PMC_TX_DATA_SEL_MSK,
                                             OTN_MLD_REG_HADAPT40G1_CONFIG_BIT_HADAPT40G1_PMC_TX_DATA_SEL_OFF,
                                             value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G1_PMC_TX_DATA_SEL_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G1_PMC_TX_DATA_SEL_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010340 bits 0) field HADAPT40G1_PMC_TX_DATA_SEL of register PMC_OTN_MLD_REG_HADAPT40G1_CONFIG */
    reg_value = otn_mld_reg_HADAPT40G1_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G1_CONFIG_BIT_HADAPT40G1_PMC_TX_DATA_SEL_MSK) >> OTN_MLD_REG_HADAPT40G1_CONFIG_BIT_HADAPT40G1_PMC_TX_DATA_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G1_PMC_TX_DATA_SEL_get", value );

    return value;
}
static INLINE void otn_mld_field_HADAPT40G1_MODE0_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G1_MODE0_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G1_MODE0_set", value );

    /* (0x00010344 bits 31:0) bits 0:31 use field HADAPT40G1_MODE0 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE0 */
    otn_mld_reg_HADAPT40G1_MODE0_field_set( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT40G1_MODE0_BIT_HADAPT40G1_MODE0_MSK,
                                            OTN_MLD_REG_HADAPT40G1_MODE0_BIT_HADAPT40G1_MODE0_OFF,
                                            value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G1_MODE0_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G1_MODE0_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010344 bits 31:0) bits 0:31 use field HADAPT40G1_MODE0 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE0 */
    reg_value = otn_mld_reg_HADAPT40G1_MODE0_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G1_MODE0_BIT_HADAPT40G1_MODE0_MSK) >> OTN_MLD_REG_HADAPT40G1_MODE0_BIT_HADAPT40G1_MODE0_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G1_MODE0_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT40G1_MODE0_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT40G1_MODE0_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G1_MODE0_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G1_MODE0_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G1_MODE0_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010344 bits 31:0) bits 0:31 use field HADAPT40G1_MODE0 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE0 */
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
        /* (0x00010344 bits 31:0) bits 0:31 use field HADAPT40G1_MODE0 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE0 */
        otn_mld_reg_HADAPT40G1_MODE0_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT40G1_MODE0_BIT_HADAPT40G1_MODE0_OFF + subfield_offset),
                                                OTN_MLD_REG_HADAPT40G1_MODE0_BIT_HADAPT40G1_MODE0_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT40G1_MODE0_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G1_MODE0_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G1_MODE0_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G1_MODE0_get", stop_bit, 31 );
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
    /* (0x00010344 bits 31:0) bits 0:31 use field HADAPT40G1_MODE0 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE0 */
    reg_value = otn_mld_reg_HADAPT40G1_MODE0_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT40G1_MODE0_BIT_HADAPT40G1_MODE0_MSK)
                  >> OTN_MLD_REG_HADAPT40G1_MODE0_BIT_HADAPT40G1_MODE0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT40G1_MODE0_BIT_HADAPT40G1_MODE0_MSK, OTN_MLD_REG_HADAPT40G1_MODE0_BIT_HADAPT40G1_MODE0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G1_MODE0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT40G1_MODE1_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G1_MODE1_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G1_MODE1_set", value );

    /* (0x00010348 bits 31:0) bits 0:31 use field HADAPT40G1_MODE1 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE1 */
    otn_mld_reg_HADAPT40G1_MODE1_field_set( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT40G1_MODE1_BIT_HADAPT40G1_MODE1_MSK,
                                            OTN_MLD_REG_HADAPT40G1_MODE1_BIT_HADAPT40G1_MODE1_OFF,
                                            value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G1_MODE1_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G1_MODE1_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010348 bits 31:0) bits 0:31 use field HADAPT40G1_MODE1 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE1 */
    reg_value = otn_mld_reg_HADAPT40G1_MODE1_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G1_MODE1_BIT_HADAPT40G1_MODE1_MSK) >> OTN_MLD_REG_HADAPT40G1_MODE1_BIT_HADAPT40G1_MODE1_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G1_MODE1_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT40G1_MODE1_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT40G1_MODE1_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G1_MODE1_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G1_MODE1_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G1_MODE1_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010348 bits 31:0) bits 0:31 use field HADAPT40G1_MODE1 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE1 */
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
        /* (0x00010348 bits 31:0) bits 0:31 use field HADAPT40G1_MODE1 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE1 */
        otn_mld_reg_HADAPT40G1_MODE1_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT40G1_MODE1_BIT_HADAPT40G1_MODE1_OFF + subfield_offset),
                                                OTN_MLD_REG_HADAPT40G1_MODE1_BIT_HADAPT40G1_MODE1_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT40G1_MODE1_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G1_MODE1_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G1_MODE1_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G1_MODE1_get", stop_bit, 31 );
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
    /* (0x00010348 bits 31:0) bits 0:31 use field HADAPT40G1_MODE1 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE1 */
    reg_value = otn_mld_reg_HADAPT40G1_MODE1_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT40G1_MODE1_BIT_HADAPT40G1_MODE1_MSK)
                  >> OTN_MLD_REG_HADAPT40G1_MODE1_BIT_HADAPT40G1_MODE1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT40G1_MODE1_BIT_HADAPT40G1_MODE1_MSK, OTN_MLD_REG_HADAPT40G1_MODE1_BIT_HADAPT40G1_MODE1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G1_MODE1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT40G1_MODE2_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G1_MODE2_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G1_MODE2_set", value );

    /* (0x0001034c bits 31:0) bits 0:31 use field HADAPT40G1_MODE2 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE2 */
    otn_mld_reg_HADAPT40G1_MODE2_field_set( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT40G1_MODE2_BIT_HADAPT40G1_MODE2_MSK,
                                            OTN_MLD_REG_HADAPT40G1_MODE2_BIT_HADAPT40G1_MODE2_OFF,
                                            value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G1_MODE2_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G1_MODE2_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001034c bits 31:0) bits 0:31 use field HADAPT40G1_MODE2 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE2 */
    reg_value = otn_mld_reg_HADAPT40G1_MODE2_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G1_MODE2_BIT_HADAPT40G1_MODE2_MSK) >> OTN_MLD_REG_HADAPT40G1_MODE2_BIT_HADAPT40G1_MODE2_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G1_MODE2_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT40G1_MODE2_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT40G1_MODE2_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G1_MODE2_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G1_MODE2_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G1_MODE2_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001034c bits 31:0) bits 0:31 use field HADAPT40G1_MODE2 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE2 */
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
        /* (0x0001034c bits 31:0) bits 0:31 use field HADAPT40G1_MODE2 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE2 */
        otn_mld_reg_HADAPT40G1_MODE2_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT40G1_MODE2_BIT_HADAPT40G1_MODE2_OFF + subfield_offset),
                                                OTN_MLD_REG_HADAPT40G1_MODE2_BIT_HADAPT40G1_MODE2_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT40G1_MODE2_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G1_MODE2_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G1_MODE2_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G1_MODE2_get", stop_bit, 31 );
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
    /* (0x0001034c bits 31:0) bits 0:31 use field HADAPT40G1_MODE2 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE2 */
    reg_value = otn_mld_reg_HADAPT40G1_MODE2_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT40G1_MODE2_BIT_HADAPT40G1_MODE2_MSK)
                  >> OTN_MLD_REG_HADAPT40G1_MODE2_BIT_HADAPT40G1_MODE2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT40G1_MODE2_BIT_HADAPT40G1_MODE2_MSK, OTN_MLD_REG_HADAPT40G1_MODE2_BIT_HADAPT40G1_MODE2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G1_MODE2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT40G1_MODE3_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G1_MODE3_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G1_MODE3_set", value );

    /* (0x00010350 bits 31:0) bits 0:31 use field HADAPT40G1_MODE3 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE3 */
    otn_mld_reg_HADAPT40G1_MODE3_field_set( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT40G1_MODE3_BIT_HADAPT40G1_MODE3_MSK,
                                            OTN_MLD_REG_HADAPT40G1_MODE3_BIT_HADAPT40G1_MODE3_OFF,
                                            value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G1_MODE3_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G1_MODE3_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010350 bits 31:0) bits 0:31 use field HADAPT40G1_MODE3 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE3 */
    reg_value = otn_mld_reg_HADAPT40G1_MODE3_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G1_MODE3_BIT_HADAPT40G1_MODE3_MSK) >> OTN_MLD_REG_HADAPT40G1_MODE3_BIT_HADAPT40G1_MODE3_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G1_MODE3_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT40G1_MODE3_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT40G1_MODE3_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G1_MODE3_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G1_MODE3_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G1_MODE3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010350 bits 31:0) bits 0:31 use field HADAPT40G1_MODE3 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE3 */
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
        /* (0x00010350 bits 31:0) bits 0:31 use field HADAPT40G1_MODE3 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE3 */
        otn_mld_reg_HADAPT40G1_MODE3_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT40G1_MODE3_BIT_HADAPT40G1_MODE3_OFF + subfield_offset),
                                                OTN_MLD_REG_HADAPT40G1_MODE3_BIT_HADAPT40G1_MODE3_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT40G1_MODE3_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G1_MODE3_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G1_MODE3_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G1_MODE3_get", stop_bit, 31 );
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
    /* (0x00010350 bits 31:0) bits 0:31 use field HADAPT40G1_MODE3 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE3 */
    reg_value = otn_mld_reg_HADAPT40G1_MODE3_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT40G1_MODE3_BIT_HADAPT40G1_MODE3_MSK)
                  >> OTN_MLD_REG_HADAPT40G1_MODE3_BIT_HADAPT40G1_MODE3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT40G1_MODE3_BIT_HADAPT40G1_MODE3_MSK, OTN_MLD_REG_HADAPT40G1_MODE3_BIT_HADAPT40G1_MODE3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G1_MODE3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT40G1_MODE4_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G1_MODE4_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G1_MODE4_set", value );

    /* (0x00010354 bits 31:0) bits 0:31 use field HADAPT40G1_MODE4 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE4 */
    otn_mld_reg_HADAPT40G1_MODE4_field_set( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT40G1_MODE4_BIT_HADAPT40G1_MODE4_MSK,
                                            OTN_MLD_REG_HADAPT40G1_MODE4_BIT_HADAPT40G1_MODE4_OFF,
                                            value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G1_MODE4_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G1_MODE4_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010354 bits 31:0) bits 0:31 use field HADAPT40G1_MODE4 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE4 */
    reg_value = otn_mld_reg_HADAPT40G1_MODE4_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G1_MODE4_BIT_HADAPT40G1_MODE4_MSK) >> OTN_MLD_REG_HADAPT40G1_MODE4_BIT_HADAPT40G1_MODE4_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G1_MODE4_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT40G1_MODE4_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT40G1_MODE4_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G1_MODE4_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G1_MODE4_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G1_MODE4_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010354 bits 31:0) bits 0:31 use field HADAPT40G1_MODE4 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE4 */
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
        /* (0x00010354 bits 31:0) bits 0:31 use field HADAPT40G1_MODE4 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE4 */
        otn_mld_reg_HADAPT40G1_MODE4_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT40G1_MODE4_BIT_HADAPT40G1_MODE4_OFF + subfield_offset),
                                                OTN_MLD_REG_HADAPT40G1_MODE4_BIT_HADAPT40G1_MODE4_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT40G1_MODE4_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G1_MODE4_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G1_MODE4_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G1_MODE4_get", stop_bit, 31 );
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
    /* (0x00010354 bits 31:0) bits 0:31 use field HADAPT40G1_MODE4 of register PMC_OTN_MLD_REG_HADAPT40G1_MODE4 */
    reg_value = otn_mld_reg_HADAPT40G1_MODE4_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT40G1_MODE4_BIT_HADAPT40G1_MODE4_MSK)
                  >> OTN_MLD_REG_HADAPT40G1_MODE4_BIT_HADAPT40G1_MODE4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT40G1_MODE4_BIT_HADAPT40G1_MODE4_MSK, OTN_MLD_REG_HADAPT40G1_MODE4_BIT_HADAPT40G1_MODE4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G1_MODE4_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_SFI_MODE_100G_set( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_SFI_MODE_100G_set( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_HADAPT100G_SFI_MODE_100G_set", value, 1);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_SFI_MODE_100G_set", value );

    /* (0x00010368 bits 0) field HADAPT100G_SFI_MODE_100G of register PMC_OTN_MLD_REG_HADAPT100G_CONFIG */
    otn_mld_reg_HADAPT100G_CONFIG_field_set( b_ptr,
                                             h_ptr,
                                             OTN_MLD_REG_HADAPT100G_CONFIG_BIT_HADAPT100G_SFI_MODE_100G_MSK,
                                             OTN_MLD_REG_HADAPT100G_CONFIG_BIT_HADAPT100G_SFI_MODE_100G_OFF,
                                             value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_SFI_MODE_100G_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_SFI_MODE_100G_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010368 bits 0) field HADAPT100G_SFI_MODE_100G of register PMC_OTN_MLD_REG_HADAPT100G_CONFIG */
    reg_value = otn_mld_reg_HADAPT100G_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_CONFIG_BIT_HADAPT100G_SFI_MODE_100G_MSK) >> OTN_MLD_REG_HADAPT100G_CONFIG_BIT_HADAPT100G_SFI_MODE_100G_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_SFI_MODE_100G_get", value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_MODE_0_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_MODE_0_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_MODE_0_set", value );

    /* (0x0001036c bits 31:0) field HADAPT100G_MODE_0 of register PMC_OTN_MLD_REG_HADAPT100G_MODE0 */
    otn_mld_reg_HADAPT100G_MODE0_field_set( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT100G_MODE0_BIT_HADAPT100G_MODE_0_MSK,
                                            OTN_MLD_REG_HADAPT100G_MODE0_BIT_HADAPT100G_MODE_0_OFF,
                                            value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_MODE_0_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_MODE_0_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001036c bits 31:0) field HADAPT100G_MODE_0 of register PMC_OTN_MLD_REG_HADAPT100G_MODE0 */
    reg_value = otn_mld_reg_HADAPT100G_MODE0_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_MODE0_BIT_HADAPT100G_MODE_0_MSK) >> OTN_MLD_REG_HADAPT100G_MODE0_BIT_HADAPT100G_MODE_0_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_MODE_0_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_MODE_0_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_MODE_0_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_MODE_0_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_MODE_0_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_MODE_0_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001036c bits 31:0) field HADAPT100G_MODE_0 of register PMC_OTN_MLD_REG_HADAPT100G_MODE0 */
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
        /* (0x0001036c bits 31:0) field HADAPT100G_MODE_0 of register PMC_OTN_MLD_REG_HADAPT100G_MODE0 */
        otn_mld_reg_HADAPT100G_MODE0_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT100G_MODE0_BIT_HADAPT100G_MODE_0_OFF + subfield_offset),
                                                OTN_MLD_REG_HADAPT100G_MODE0_BIT_HADAPT100G_MODE_0_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_MODE_0_get( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_MODE_0_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_MODE_0_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_MODE_0_get", stop_bit, 31 );
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
    /* (0x0001036c bits 31:0) field HADAPT100G_MODE_0 of register PMC_OTN_MLD_REG_HADAPT100G_MODE0 */
    reg_value = otn_mld_reg_HADAPT100G_MODE0_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_MODE0_BIT_HADAPT100G_MODE_0_MSK)
                  >> OTN_MLD_REG_HADAPT100G_MODE0_BIT_HADAPT100G_MODE_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_MODE0_BIT_HADAPT100G_MODE_0_MSK, OTN_MLD_REG_HADAPT100G_MODE0_BIT_HADAPT100G_MODE_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_MODE_0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_MODE_1_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_MODE_1_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_MODE_1_set", value );

    /* (0x00010370 bits 31:0) field HADAPT100G_MODE_1 of register PMC_OTN_MLD_REG_HADAPT100G_MODE1 */
    otn_mld_reg_HADAPT100G_MODE1_field_set( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT100G_MODE1_BIT_HADAPT100G_MODE_1_MSK,
                                            OTN_MLD_REG_HADAPT100G_MODE1_BIT_HADAPT100G_MODE_1_OFF,
                                            value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_MODE_1_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_MODE_1_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010370 bits 31:0) field HADAPT100G_MODE_1 of register PMC_OTN_MLD_REG_HADAPT100G_MODE1 */
    reg_value = otn_mld_reg_HADAPT100G_MODE1_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_MODE1_BIT_HADAPT100G_MODE_1_MSK) >> OTN_MLD_REG_HADAPT100G_MODE1_BIT_HADAPT100G_MODE_1_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_MODE_1_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_MODE_1_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_MODE_1_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_MODE_1_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_MODE_1_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_MODE_1_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010370 bits 31:0) field HADAPT100G_MODE_1 of register PMC_OTN_MLD_REG_HADAPT100G_MODE1 */
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
        /* (0x00010370 bits 31:0) field HADAPT100G_MODE_1 of register PMC_OTN_MLD_REG_HADAPT100G_MODE1 */
        otn_mld_reg_HADAPT100G_MODE1_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT100G_MODE1_BIT_HADAPT100G_MODE_1_OFF + subfield_offset),
                                                OTN_MLD_REG_HADAPT100G_MODE1_BIT_HADAPT100G_MODE_1_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_MODE_1_get( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_MODE_1_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_MODE_1_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_MODE_1_get", stop_bit, 31 );
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
    /* (0x00010370 bits 31:0) field HADAPT100G_MODE_1 of register PMC_OTN_MLD_REG_HADAPT100G_MODE1 */
    reg_value = otn_mld_reg_HADAPT100G_MODE1_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_MODE1_BIT_HADAPT100G_MODE_1_MSK)
                  >> OTN_MLD_REG_HADAPT100G_MODE1_BIT_HADAPT100G_MODE_1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_MODE1_BIT_HADAPT100G_MODE_1_MSK, OTN_MLD_REG_HADAPT100G_MODE1_BIT_HADAPT100G_MODE_1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_MODE_1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_MODE_2_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_MODE_2_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_MODE_2_set", value );

    /* (0x00010374 bits 31:0) field HADAPT100G_MODE_2 of register PMC_OTN_MLD_REG_HADAPT100G_MODE2 */
    otn_mld_reg_HADAPT100G_MODE2_field_set( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT100G_MODE2_BIT_HADAPT100G_MODE_2_MSK,
                                            OTN_MLD_REG_HADAPT100G_MODE2_BIT_HADAPT100G_MODE_2_OFF,
                                            value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_MODE_2_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_MODE_2_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010374 bits 31:0) field HADAPT100G_MODE_2 of register PMC_OTN_MLD_REG_HADAPT100G_MODE2 */
    reg_value = otn_mld_reg_HADAPT100G_MODE2_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_MODE2_BIT_HADAPT100G_MODE_2_MSK) >> OTN_MLD_REG_HADAPT100G_MODE2_BIT_HADAPT100G_MODE_2_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_MODE_2_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_MODE_2_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_MODE_2_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_MODE_2_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_MODE_2_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_MODE_2_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010374 bits 31:0) field HADAPT100G_MODE_2 of register PMC_OTN_MLD_REG_HADAPT100G_MODE2 */
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
        /* (0x00010374 bits 31:0) field HADAPT100G_MODE_2 of register PMC_OTN_MLD_REG_HADAPT100G_MODE2 */
        otn_mld_reg_HADAPT100G_MODE2_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT100G_MODE2_BIT_HADAPT100G_MODE_2_OFF + subfield_offset),
                                                OTN_MLD_REG_HADAPT100G_MODE2_BIT_HADAPT100G_MODE_2_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_MODE_2_get( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_MODE_2_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_MODE_2_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_MODE_2_get", stop_bit, 31 );
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
    /* (0x00010374 bits 31:0) field HADAPT100G_MODE_2 of register PMC_OTN_MLD_REG_HADAPT100G_MODE2 */
    reg_value = otn_mld_reg_HADAPT100G_MODE2_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_MODE2_BIT_HADAPT100G_MODE_2_MSK)
                  >> OTN_MLD_REG_HADAPT100G_MODE2_BIT_HADAPT100G_MODE_2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_MODE2_BIT_HADAPT100G_MODE_2_MSK, OTN_MLD_REG_HADAPT100G_MODE2_BIT_HADAPT100G_MODE_2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_MODE_2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_MODE_3_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_MODE_3_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_MODE_3_set", value );

    /* (0x00010378 bits 31:0) field HADAPT100G_MODE_3 of register PMC_OTN_MLD_REG_HADAPT100G_MODE3 */
    otn_mld_reg_HADAPT100G_MODE3_field_set( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT100G_MODE3_BIT_HADAPT100G_MODE_3_MSK,
                                            OTN_MLD_REG_HADAPT100G_MODE3_BIT_HADAPT100G_MODE_3_OFF,
                                            value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_MODE_3_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_MODE_3_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010378 bits 31:0) field HADAPT100G_MODE_3 of register PMC_OTN_MLD_REG_HADAPT100G_MODE3 */
    reg_value = otn_mld_reg_HADAPT100G_MODE3_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_MODE3_BIT_HADAPT100G_MODE_3_MSK) >> OTN_MLD_REG_HADAPT100G_MODE3_BIT_HADAPT100G_MODE_3_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_MODE_3_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_MODE_3_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_MODE_3_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_MODE_3_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_MODE_3_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_MODE_3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010378 bits 31:0) field HADAPT100G_MODE_3 of register PMC_OTN_MLD_REG_HADAPT100G_MODE3 */
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
        /* (0x00010378 bits 31:0) field HADAPT100G_MODE_3 of register PMC_OTN_MLD_REG_HADAPT100G_MODE3 */
        otn_mld_reg_HADAPT100G_MODE3_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT100G_MODE3_BIT_HADAPT100G_MODE_3_OFF + subfield_offset),
                                                OTN_MLD_REG_HADAPT100G_MODE3_BIT_HADAPT100G_MODE_3_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_MODE_3_get( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_MODE_3_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_MODE_3_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_MODE_3_get", stop_bit, 31 );
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
    /* (0x00010378 bits 31:0) field HADAPT100G_MODE_3 of register PMC_OTN_MLD_REG_HADAPT100G_MODE3 */
    reg_value = otn_mld_reg_HADAPT100G_MODE3_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_MODE3_BIT_HADAPT100G_MODE_3_MSK)
                  >> OTN_MLD_REG_HADAPT100G_MODE3_BIT_HADAPT100G_MODE_3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_MODE3_BIT_HADAPT100G_MODE_3_MSK, OTN_MLD_REG_HADAPT100G_MODE3_BIT_HADAPT100G_MODE_3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_MODE_3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_MODE_4_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_MODE_4_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_MODE_4_set", value );

    /* (0x0001037c bits 31:0) field HADAPT100G_MODE_4 of register PMC_OTN_MLD_REG_HADAPT100G_MODE4 */
    otn_mld_reg_HADAPT100G_MODE4_field_set( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT100G_MODE4_BIT_HADAPT100G_MODE_4_MSK,
                                            OTN_MLD_REG_HADAPT100G_MODE4_BIT_HADAPT100G_MODE_4_OFF,
                                            value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_MODE_4_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_MODE_4_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001037c bits 31:0) field HADAPT100G_MODE_4 of register PMC_OTN_MLD_REG_HADAPT100G_MODE4 */
    reg_value = otn_mld_reg_HADAPT100G_MODE4_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_MODE4_BIT_HADAPT100G_MODE_4_MSK) >> OTN_MLD_REG_HADAPT100G_MODE4_BIT_HADAPT100G_MODE_4_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_MODE_4_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_MODE_4_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_MODE_4_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_MODE_4_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_MODE_4_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_MODE_4_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001037c bits 31:0) field HADAPT100G_MODE_4 of register PMC_OTN_MLD_REG_HADAPT100G_MODE4 */
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
        /* (0x0001037c bits 31:0) field HADAPT100G_MODE_4 of register PMC_OTN_MLD_REG_HADAPT100G_MODE4 */
        otn_mld_reg_HADAPT100G_MODE4_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT100G_MODE4_BIT_HADAPT100G_MODE_4_OFF + subfield_offset),
                                                OTN_MLD_REG_HADAPT100G_MODE4_BIT_HADAPT100G_MODE_4_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_MODE_4_get( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_MODE_4_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_MODE_4_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_MODE_4_get", stop_bit, 31 );
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
    /* (0x0001037c bits 31:0) field HADAPT100G_MODE_4 of register PMC_OTN_MLD_REG_HADAPT100G_MODE4 */
    reg_value = otn_mld_reg_HADAPT100G_MODE4_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_MODE4_BIT_HADAPT100G_MODE_4_MSK)
                  >> OTN_MLD_REG_HADAPT100G_MODE4_BIT_HADAPT100G_MODE_4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_MODE4_BIT_HADAPT100G_MODE_4_MSK, OTN_MLD_REG_HADAPT100G_MODE4_BIT_HADAPT100G_MODE_4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_MODE_4_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_MODE_5_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_MODE_5_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_MODE_5_set", value );

    /* (0x00010380 bits 31:0) field HADAPT100G_MODE_5 of register PMC_OTN_MLD_REG_HADAPT100G_MODE5 */
    otn_mld_reg_HADAPT100G_MODE5_field_set( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT100G_MODE5_BIT_HADAPT100G_MODE_5_MSK,
                                            OTN_MLD_REG_HADAPT100G_MODE5_BIT_HADAPT100G_MODE_5_OFF,
                                            value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_MODE_5_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_MODE_5_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010380 bits 31:0) field HADAPT100G_MODE_5 of register PMC_OTN_MLD_REG_HADAPT100G_MODE5 */
    reg_value = otn_mld_reg_HADAPT100G_MODE5_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_MODE5_BIT_HADAPT100G_MODE_5_MSK) >> OTN_MLD_REG_HADAPT100G_MODE5_BIT_HADAPT100G_MODE_5_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_MODE_5_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_MODE_5_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_MODE_5_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_MODE_5_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_MODE_5_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_MODE_5_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010380 bits 31:0) field HADAPT100G_MODE_5 of register PMC_OTN_MLD_REG_HADAPT100G_MODE5 */
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
        /* (0x00010380 bits 31:0) field HADAPT100G_MODE_5 of register PMC_OTN_MLD_REG_HADAPT100G_MODE5 */
        otn_mld_reg_HADAPT100G_MODE5_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT100G_MODE5_BIT_HADAPT100G_MODE_5_OFF + subfield_offset),
                                                OTN_MLD_REG_HADAPT100G_MODE5_BIT_HADAPT100G_MODE_5_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_MODE_5_get( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_MODE_5_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_MODE_5_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_MODE_5_get", stop_bit, 31 );
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
    /* (0x00010380 bits 31:0) field HADAPT100G_MODE_5 of register PMC_OTN_MLD_REG_HADAPT100G_MODE5 */
    reg_value = otn_mld_reg_HADAPT100G_MODE5_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_MODE5_BIT_HADAPT100G_MODE_5_MSK)
                  >> OTN_MLD_REG_HADAPT100G_MODE5_BIT_HADAPT100G_MODE_5_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_MODE5_BIT_HADAPT100G_MODE_5_MSK, OTN_MLD_REG_HADAPT100G_MODE5_BIT_HADAPT100G_MODE_5_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_MODE_5_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_PATTERN_0_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_PATTERN_0_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_0_set", value );

    /* (0x00010384 bits 31:0) field HADAPT100G_PATTERN_0 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN0 */
    otn_mld_reg_HADAPT100G_PATTERN0_field_set( b_ptr,
                                               h_ptr,
                                               OTN_MLD_REG_HADAPT100G_PATTERN0_BIT_HADAPT100G_PATTERN_0_MSK,
                                               OTN_MLD_REG_HADAPT100G_PATTERN0_BIT_HADAPT100G_PATTERN_0_OFF,
                                               value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_0_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_0_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010384 bits 31:0) field HADAPT100G_PATTERN_0 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN0 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN0_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN0_BIT_HADAPT100G_PATTERN_0_MSK) >> OTN_MLD_REG_HADAPT100G_PATTERN0_BIT_HADAPT100G_PATTERN_0_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_0_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_0_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_0_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_0_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_0_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_0_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010384 bits 31:0) field HADAPT100G_PATTERN_0 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN0 */
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
        /* (0x00010384 bits 31:0) field HADAPT100G_PATTERN_0 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN0 */
        otn_mld_reg_HADAPT100G_PATTERN0_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTN_MLD_REG_HADAPT100G_PATTERN0_BIT_HADAPT100G_PATTERN_0_OFF + subfield_offset),
                                                   OTN_MLD_REG_HADAPT100G_PATTERN0_BIT_HADAPT100G_PATTERN_0_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_0_get( otn_mld_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_0_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_0_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_0_get", stop_bit, 31 );
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
    /* (0x00010384 bits 31:0) field HADAPT100G_PATTERN_0 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN0 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN0_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN0_BIT_HADAPT100G_PATTERN_0_MSK)
                  >> OTN_MLD_REG_HADAPT100G_PATTERN0_BIT_HADAPT100G_PATTERN_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_PATTERN0_BIT_HADAPT100G_PATTERN_0_MSK, OTN_MLD_REG_HADAPT100G_PATTERN0_BIT_HADAPT100G_PATTERN_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_PATTERN_1_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_PATTERN_1_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_1_set", value );

    /* (0x00010388 bits 31:0) field HADAPT100G_PATTERN_1 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN1 */
    otn_mld_reg_HADAPT100G_PATTERN1_field_set( b_ptr,
                                               h_ptr,
                                               OTN_MLD_REG_HADAPT100G_PATTERN1_BIT_HADAPT100G_PATTERN_1_MSK,
                                               OTN_MLD_REG_HADAPT100G_PATTERN1_BIT_HADAPT100G_PATTERN_1_OFF,
                                               value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_1_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_1_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010388 bits 31:0) field HADAPT100G_PATTERN_1 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN1 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN1_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN1_BIT_HADAPT100G_PATTERN_1_MSK) >> OTN_MLD_REG_HADAPT100G_PATTERN1_BIT_HADAPT100G_PATTERN_1_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_1_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_1_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_1_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_1_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_1_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_1_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010388 bits 31:0) field HADAPT100G_PATTERN_1 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN1 */
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
        /* (0x00010388 bits 31:0) field HADAPT100G_PATTERN_1 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN1 */
        otn_mld_reg_HADAPT100G_PATTERN1_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTN_MLD_REG_HADAPT100G_PATTERN1_BIT_HADAPT100G_PATTERN_1_OFF + subfield_offset),
                                                   OTN_MLD_REG_HADAPT100G_PATTERN1_BIT_HADAPT100G_PATTERN_1_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_1_get( otn_mld_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_1_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_1_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_1_get", stop_bit, 31 );
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
    /* (0x00010388 bits 31:0) field HADAPT100G_PATTERN_1 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN1 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN1_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN1_BIT_HADAPT100G_PATTERN_1_MSK)
                  >> OTN_MLD_REG_HADAPT100G_PATTERN1_BIT_HADAPT100G_PATTERN_1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_PATTERN1_BIT_HADAPT100G_PATTERN_1_MSK, OTN_MLD_REG_HADAPT100G_PATTERN1_BIT_HADAPT100G_PATTERN_1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_1_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_PATTERN_2_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_PATTERN_2_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_2_set", value );

    /* (0x0001038c bits 31:0) field HADAPT100G_PATTERN_2 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN2 */
    otn_mld_reg_HADAPT100G_PATTERN2_field_set( b_ptr,
                                               h_ptr,
                                               OTN_MLD_REG_HADAPT100G_PATTERN2_BIT_HADAPT100G_PATTERN_2_MSK,
                                               OTN_MLD_REG_HADAPT100G_PATTERN2_BIT_HADAPT100G_PATTERN_2_OFF,
                                               value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_2_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_2_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001038c bits 31:0) field HADAPT100G_PATTERN_2 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN2 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN2_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN2_BIT_HADAPT100G_PATTERN_2_MSK) >> OTN_MLD_REG_HADAPT100G_PATTERN2_BIT_HADAPT100G_PATTERN_2_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_2_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_2_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_2_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_2_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_2_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_2_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001038c bits 31:0) field HADAPT100G_PATTERN_2 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN2 */
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
        /* (0x0001038c bits 31:0) field HADAPT100G_PATTERN_2 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN2 */
        otn_mld_reg_HADAPT100G_PATTERN2_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTN_MLD_REG_HADAPT100G_PATTERN2_BIT_HADAPT100G_PATTERN_2_OFF + subfield_offset),
                                                   OTN_MLD_REG_HADAPT100G_PATTERN2_BIT_HADAPT100G_PATTERN_2_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_2_get( otn_mld_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_2_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_2_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_2_get", stop_bit, 31 );
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
    /* (0x0001038c bits 31:0) field HADAPT100G_PATTERN_2 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN2 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN2_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN2_BIT_HADAPT100G_PATTERN_2_MSK)
                  >> OTN_MLD_REG_HADAPT100G_PATTERN2_BIT_HADAPT100G_PATTERN_2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_PATTERN2_BIT_HADAPT100G_PATTERN_2_MSK, OTN_MLD_REG_HADAPT100G_PATTERN2_BIT_HADAPT100G_PATTERN_2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_2_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_PATTERN_3_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_PATTERN_3_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_3_set", value );

    /* (0x00010390 bits 31:0) field HADAPT100G_PATTERN_3 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN3 */
    otn_mld_reg_HADAPT100G_PATTERN3_field_set( b_ptr,
                                               h_ptr,
                                               OTN_MLD_REG_HADAPT100G_PATTERN3_BIT_HADAPT100G_PATTERN_3_MSK,
                                               OTN_MLD_REG_HADAPT100G_PATTERN3_BIT_HADAPT100G_PATTERN_3_OFF,
                                               value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_3_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_3_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010390 bits 31:0) field HADAPT100G_PATTERN_3 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN3 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN3_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN3_BIT_HADAPT100G_PATTERN_3_MSK) >> OTN_MLD_REG_HADAPT100G_PATTERN3_BIT_HADAPT100G_PATTERN_3_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_3_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_3_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_3_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_3_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_3_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010390 bits 31:0) field HADAPT100G_PATTERN_3 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN3 */
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
        /* (0x00010390 bits 31:0) field HADAPT100G_PATTERN_3 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN3 */
        otn_mld_reg_HADAPT100G_PATTERN3_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTN_MLD_REG_HADAPT100G_PATTERN3_BIT_HADAPT100G_PATTERN_3_OFF + subfield_offset),
                                                   OTN_MLD_REG_HADAPT100G_PATTERN3_BIT_HADAPT100G_PATTERN_3_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_3_get( otn_mld_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_3_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_3_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_3_get", stop_bit, 31 );
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
    /* (0x00010390 bits 31:0) field HADAPT100G_PATTERN_3 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN3 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN3_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN3_BIT_HADAPT100G_PATTERN_3_MSK)
                  >> OTN_MLD_REG_HADAPT100G_PATTERN3_BIT_HADAPT100G_PATTERN_3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_PATTERN3_BIT_HADAPT100G_PATTERN_3_MSK, OTN_MLD_REG_HADAPT100G_PATTERN3_BIT_HADAPT100G_PATTERN_3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_PATTERN_4_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_PATTERN_4_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_4_set", value );

    /* (0x00010394 bits 31:0) field HADAPT100G_PATTERN_4 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN4 */
    otn_mld_reg_HADAPT100G_PATTERN4_field_set( b_ptr,
                                               h_ptr,
                                               OTN_MLD_REG_HADAPT100G_PATTERN4_BIT_HADAPT100G_PATTERN_4_MSK,
                                               OTN_MLD_REG_HADAPT100G_PATTERN4_BIT_HADAPT100G_PATTERN_4_OFF,
                                               value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_4_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_4_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010394 bits 31:0) field HADAPT100G_PATTERN_4 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN4 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN4_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN4_BIT_HADAPT100G_PATTERN_4_MSK) >> OTN_MLD_REG_HADAPT100G_PATTERN4_BIT_HADAPT100G_PATTERN_4_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_4_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_4_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_4_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_4_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_4_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_4_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010394 bits 31:0) field HADAPT100G_PATTERN_4 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN4 */
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
        /* (0x00010394 bits 31:0) field HADAPT100G_PATTERN_4 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN4 */
        otn_mld_reg_HADAPT100G_PATTERN4_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTN_MLD_REG_HADAPT100G_PATTERN4_BIT_HADAPT100G_PATTERN_4_OFF + subfield_offset),
                                                   OTN_MLD_REG_HADAPT100G_PATTERN4_BIT_HADAPT100G_PATTERN_4_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_4_get( otn_mld_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_4_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_4_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_4_get", stop_bit, 31 );
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
    /* (0x00010394 bits 31:0) field HADAPT100G_PATTERN_4 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN4 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN4_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN4_BIT_HADAPT100G_PATTERN_4_MSK)
                  >> OTN_MLD_REG_HADAPT100G_PATTERN4_BIT_HADAPT100G_PATTERN_4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_PATTERN4_BIT_HADAPT100G_PATTERN_4_MSK, OTN_MLD_REG_HADAPT100G_PATTERN4_BIT_HADAPT100G_PATTERN_4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_4_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_PATTERN_5_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_PATTERN_5_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_5_set", value );

    /* (0x00010398 bits 31:0) field HADAPT100G_PATTERN_5 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN5 */
    otn_mld_reg_HADAPT100G_PATTERN5_field_set( b_ptr,
                                               h_ptr,
                                               OTN_MLD_REG_HADAPT100G_PATTERN5_BIT_HADAPT100G_PATTERN_5_MSK,
                                               OTN_MLD_REG_HADAPT100G_PATTERN5_BIT_HADAPT100G_PATTERN_5_OFF,
                                               value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_5_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_5_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010398 bits 31:0) field HADAPT100G_PATTERN_5 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN5 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN5_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN5_BIT_HADAPT100G_PATTERN_5_MSK) >> OTN_MLD_REG_HADAPT100G_PATTERN5_BIT_HADAPT100G_PATTERN_5_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_5_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_5_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_5_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_5_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_5_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_5_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010398 bits 31:0) field HADAPT100G_PATTERN_5 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN5 */
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
        /* (0x00010398 bits 31:0) field HADAPT100G_PATTERN_5 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN5 */
        otn_mld_reg_HADAPT100G_PATTERN5_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTN_MLD_REG_HADAPT100G_PATTERN5_BIT_HADAPT100G_PATTERN_5_OFF + subfield_offset),
                                                   OTN_MLD_REG_HADAPT100G_PATTERN5_BIT_HADAPT100G_PATTERN_5_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_5_get( otn_mld_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_5_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_5_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_5_get", stop_bit, 31 );
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
    /* (0x00010398 bits 31:0) field HADAPT100G_PATTERN_5 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN5 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN5_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN5_BIT_HADAPT100G_PATTERN_5_MSK)
                  >> OTN_MLD_REG_HADAPT100G_PATTERN5_BIT_HADAPT100G_PATTERN_5_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_PATTERN5_BIT_HADAPT100G_PATTERN_5_MSK, OTN_MLD_REG_HADAPT100G_PATTERN5_BIT_HADAPT100G_PATTERN_5_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_5_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_PATTERN_6_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_PATTERN_6_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_6_set", value );

    /* (0x0001039c bits 31:0) field HADAPT100G_PATTERN_6 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN6 */
    otn_mld_reg_HADAPT100G_PATTERN6_field_set( b_ptr,
                                               h_ptr,
                                               OTN_MLD_REG_HADAPT100G_PATTERN6_BIT_HADAPT100G_PATTERN_6_MSK,
                                               OTN_MLD_REG_HADAPT100G_PATTERN6_BIT_HADAPT100G_PATTERN_6_OFF,
                                               value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_6_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_6_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001039c bits 31:0) field HADAPT100G_PATTERN_6 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN6 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN6_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN6_BIT_HADAPT100G_PATTERN_6_MSK) >> OTN_MLD_REG_HADAPT100G_PATTERN6_BIT_HADAPT100G_PATTERN_6_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_6_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_6_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_6_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_6_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_6_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_6_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001039c bits 31:0) field HADAPT100G_PATTERN_6 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN6 */
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
        /* (0x0001039c bits 31:0) field HADAPT100G_PATTERN_6 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN6 */
        otn_mld_reg_HADAPT100G_PATTERN6_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTN_MLD_REG_HADAPT100G_PATTERN6_BIT_HADAPT100G_PATTERN_6_OFF + subfield_offset),
                                                   OTN_MLD_REG_HADAPT100G_PATTERN6_BIT_HADAPT100G_PATTERN_6_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_6_get( otn_mld_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_6_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_6_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_6_get", stop_bit, 31 );
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
    /* (0x0001039c bits 31:0) field HADAPT100G_PATTERN_6 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN6 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN6_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN6_BIT_HADAPT100G_PATTERN_6_MSK)
                  >> OTN_MLD_REG_HADAPT100G_PATTERN6_BIT_HADAPT100G_PATTERN_6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_PATTERN6_BIT_HADAPT100G_PATTERN_6_MSK, OTN_MLD_REG_HADAPT100G_PATTERN6_BIT_HADAPT100G_PATTERN_6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_6_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_PATTERN_7_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_PATTERN_7_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_7_set", value );

    /* (0x000103a0 bits 31:0) field HADAPT100G_PATTERN_7 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN7 */
    otn_mld_reg_HADAPT100G_PATTERN7_field_set( b_ptr,
                                               h_ptr,
                                               OTN_MLD_REG_HADAPT100G_PATTERN7_BIT_HADAPT100G_PATTERN_7_MSK,
                                               OTN_MLD_REG_HADAPT100G_PATTERN7_BIT_HADAPT100G_PATTERN_7_OFF,
                                               value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_7_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_7_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000103a0 bits 31:0) field HADAPT100G_PATTERN_7 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN7 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN7_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN7_BIT_HADAPT100G_PATTERN_7_MSK) >> OTN_MLD_REG_HADAPT100G_PATTERN7_BIT_HADAPT100G_PATTERN_7_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_7_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_7_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_7_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_7_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_7_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_7_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000103a0 bits 31:0) field HADAPT100G_PATTERN_7 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN7 */
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
        /* (0x000103a0 bits 31:0) field HADAPT100G_PATTERN_7 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN7 */
        otn_mld_reg_HADAPT100G_PATTERN7_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTN_MLD_REG_HADAPT100G_PATTERN7_BIT_HADAPT100G_PATTERN_7_OFF + subfield_offset),
                                                   OTN_MLD_REG_HADAPT100G_PATTERN7_BIT_HADAPT100G_PATTERN_7_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_7_get( otn_mld_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_7_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_7_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_7_get", stop_bit, 31 );
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
    /* (0x000103a0 bits 31:0) field HADAPT100G_PATTERN_7 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN7 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN7_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN7_BIT_HADAPT100G_PATTERN_7_MSK)
                  >> OTN_MLD_REG_HADAPT100G_PATTERN7_BIT_HADAPT100G_PATTERN_7_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_PATTERN7_BIT_HADAPT100G_PATTERN_7_MSK, OTN_MLD_REG_HADAPT100G_PATTERN7_BIT_HADAPT100G_PATTERN_7_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_7_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_PATTERN_8_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_PATTERN_8_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_8_set", value );

    /* (0x000103a4 bits 31:0) field HADAPT100G_PATTERN_8 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN8 */
    otn_mld_reg_HADAPT100G_PATTERN8_field_set( b_ptr,
                                               h_ptr,
                                               OTN_MLD_REG_HADAPT100G_PATTERN8_BIT_HADAPT100G_PATTERN_8_MSK,
                                               OTN_MLD_REG_HADAPT100G_PATTERN8_BIT_HADAPT100G_PATTERN_8_OFF,
                                               value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_8_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_8_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000103a4 bits 31:0) field HADAPT100G_PATTERN_8 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN8 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN8_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN8_BIT_HADAPT100G_PATTERN_8_MSK) >> OTN_MLD_REG_HADAPT100G_PATTERN8_BIT_HADAPT100G_PATTERN_8_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_8_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_8_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_8_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_8_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_8_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_8_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000103a4 bits 31:0) field HADAPT100G_PATTERN_8 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN8 */
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
        /* (0x000103a4 bits 31:0) field HADAPT100G_PATTERN_8 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN8 */
        otn_mld_reg_HADAPT100G_PATTERN8_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTN_MLD_REG_HADAPT100G_PATTERN8_BIT_HADAPT100G_PATTERN_8_OFF + subfield_offset),
                                                   OTN_MLD_REG_HADAPT100G_PATTERN8_BIT_HADAPT100G_PATTERN_8_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_8_get( otn_mld_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_8_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_8_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_8_get", stop_bit, 31 );
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
    /* (0x000103a4 bits 31:0) field HADAPT100G_PATTERN_8 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN8 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN8_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN8_BIT_HADAPT100G_PATTERN_8_MSK)
                  >> OTN_MLD_REG_HADAPT100G_PATTERN8_BIT_HADAPT100G_PATTERN_8_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_PATTERN8_BIT_HADAPT100G_PATTERN_8_MSK, OTN_MLD_REG_HADAPT100G_PATTERN8_BIT_HADAPT100G_PATTERN_8_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_8_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_PATTERN_9_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_PATTERN_9_set( otn_mld_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_9_set", value );

    /* (0x000103a8 bits 31:0) field HADAPT100G_PATTERN_9 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN9 */
    otn_mld_reg_HADAPT100G_PATTERN9_field_set( b_ptr,
                                               h_ptr,
                                               OTN_MLD_REG_HADAPT100G_PATTERN9_BIT_HADAPT100G_PATTERN_9_MSK,
                                               OTN_MLD_REG_HADAPT100G_PATTERN9_BIT_HADAPT100G_PATTERN_9_OFF,
                                               value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_9_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_PATTERN_9_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000103a8 bits 31:0) field HADAPT100G_PATTERN_9 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN9 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN9_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN9_BIT_HADAPT100G_PATTERN_9_MSK) >> OTN_MLD_REG_HADAPT100G_PATTERN9_BIT_HADAPT100G_PATTERN_9_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_PATTERN_9_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_9_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_PATTERN_9_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_9_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_9_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_9_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000103a8 bits 31:0) field HADAPT100G_PATTERN_9 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN9 */
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
        /* (0x000103a8 bits 31:0) field HADAPT100G_PATTERN_9 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN9 */
        otn_mld_reg_HADAPT100G_PATTERN9_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (OTN_MLD_REG_HADAPT100G_PATTERN9_BIT_HADAPT100G_PATTERN_9_OFF + subfield_offset),
                                                   OTN_MLD_REG_HADAPT100G_PATTERN9_BIT_HADAPT100G_PATTERN_9_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_9_get( otn_mld_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_PATTERN_9_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_PATTERN_9_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_PATTERN_9_get", stop_bit, 31 );
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
    /* (0x000103a8 bits 31:0) field HADAPT100G_PATTERN_9 of register PMC_OTN_MLD_REG_HADAPT100G_PATTERN9 */
    reg_value = otn_mld_reg_HADAPT100G_PATTERN9_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_PATTERN9_BIT_HADAPT100G_PATTERN_9_MSK)
                  >> OTN_MLD_REG_HADAPT100G_PATTERN9_BIT_HADAPT100G_PATTERN_9_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_PATTERN9_BIT_HADAPT100G_PATTERN_9_MSK, OTN_MLD_REG_HADAPT100G_PATTERN9_BIT_HADAPT100G_PATTERN_9_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_PATTERN_9_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size12
 * ==================================================================================
 */
static INLINE void otn_mld_field_PRECODER_32_RX_BYPASS_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_PRECODER_32_RX_BYPASS_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_PRECODER_32_RX_BYPASS_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_PRECODER_32_RX_BYPASS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otn_mld_field_PRECODER_32_RX_BYPASS_set", N, value );

    /* ((0x00010008 + (N) * 0x04) bits 2) field PRECODER_32_RX_BYPASS of register PMC_OTN_MLD_REG_LANE_CONFIG index N=0..11 */
    otn_mld_reg_LANE_CONFIG_array_field_set( b_ptr,
                                             h_ptr,
                                             N,
                                             OTN_MLD_REG_LANE_CONFIG_BIT_PRECODER_32_RX_BYPASS_MSK,
                                             OTN_MLD_REG_LANE_CONFIG_BIT_PRECODER_32_RX_BYPASS_OFF,
                                             value);
}

static INLINE UINT32 otn_mld_field_PRECODER_32_RX_BYPASS_get( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_PRECODER_32_RX_BYPASS_get( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_PRECODER_32_RX_BYPASS_get", N, 11);
    /* ((0x00010008 + (N) * 0x04) bits 2) field PRECODER_32_RX_BYPASS of register PMC_OTN_MLD_REG_LANE_CONFIG index N=0..11 */
    reg_value = otn_mld_reg_LANE_CONFIG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & OTN_MLD_REG_LANE_CONFIG_BIT_PRECODER_32_RX_BYPASS_MSK) >> OTN_MLD_REG_LANE_CONFIG_BIT_PRECODER_32_RX_BYPASS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otn_mld_field_PRECODER_32_RX_BYPASS_get", N, value );

    return value;
}
static INLINE void otn_mld_field_PRECODER_32_TX_BYPASS_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_PRECODER_32_TX_BYPASS_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_PRECODER_32_TX_BYPASS_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_PRECODER_32_TX_BYPASS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otn_mld_field_PRECODER_32_TX_BYPASS_set", N, value );

    /* ((0x00010008 + (N) * 0x04) bits 1) field PRECODER_32_TX_BYPASS of register PMC_OTN_MLD_REG_LANE_CONFIG index N=0..11 */
    otn_mld_reg_LANE_CONFIG_array_field_set( b_ptr,
                                             h_ptr,
                                             N,
                                             OTN_MLD_REG_LANE_CONFIG_BIT_PRECODER_32_TX_BYPASS_MSK,
                                             OTN_MLD_REG_LANE_CONFIG_BIT_PRECODER_32_TX_BYPASS_OFF,
                                             value);
}

static INLINE UINT32 otn_mld_field_PRECODER_32_TX_BYPASS_get( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_PRECODER_32_TX_BYPASS_get( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_PRECODER_32_TX_BYPASS_get", N, 11);
    /* ((0x00010008 + (N) * 0x04) bits 1) field PRECODER_32_TX_BYPASS of register PMC_OTN_MLD_REG_LANE_CONFIG index N=0..11 */
    reg_value = otn_mld_reg_LANE_CONFIG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & OTN_MLD_REG_LANE_CONFIG_BIT_PRECODER_32_TX_BYPASS_MSK) >> OTN_MLD_REG_LANE_CONFIG_BIT_PRECODER_32_TX_BYPASS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otn_mld_field_PRECODER_32_TX_BYPASS_get", N, value );

    return value;
}
static INLINE void otn_mld_field_BERGEN_32_BYPASS_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_BERGEN_32_BYPASS_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_BERGEN_32_BYPASS_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_BERGEN_32_BYPASS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otn_mld_field_BERGEN_32_BYPASS_set", N, value );

    /* ((0x00010008 + (N) * 0x04) bits 0) field BERGEN_32_BYPASS of register PMC_OTN_MLD_REG_LANE_CONFIG index N=0..11 */
    otn_mld_reg_LANE_CONFIG_array_field_set( b_ptr,
                                             h_ptr,
                                             N,
                                             OTN_MLD_REG_LANE_CONFIG_BIT_BERGEN_32_BYPASS_MSK,
                                             OTN_MLD_REG_LANE_CONFIG_BIT_BERGEN_32_BYPASS_OFF,
                                             value);
}

static INLINE UINT32 otn_mld_field_BERGEN_32_BYPASS_get( otn_mld_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_BERGEN_32_BYPASS_get( otn_mld_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_BERGEN_32_BYPASS_get", N, 11);
    /* ((0x00010008 + (N) * 0x04) bits 0) field BERGEN_32_BYPASS of register PMC_OTN_MLD_REG_LANE_CONFIG index N=0..11 */
    reg_value = otn_mld_reg_LANE_CONFIG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & OTN_MLD_REG_LANE_CONFIG_BIT_BERGEN_32_BYPASS_MSK) >> OTN_MLD_REG_LANE_CONFIG_BIT_BERGEN_32_BYPASS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otn_mld_field_BERGEN_32_BYPASS_get", N, value );

    return value;
}
static INLINE void otn_mld_field_LANE_10G_TX_SRC_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_LANE_10G_TX_SRC_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_LANE_10G_TX_SRC_set", N, 11);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_LANE_10G_TX_SRC_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "otn_mld_field_LANE_10G_TX_SRC_set", N, value );

    /* ((0x00010038 + (N) * 0x04) bits 7:0) bits 0:7 use field LANE_10G_TX_SRC of register PMC_OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG index N=0..11 */
    otn_mld_reg_DISTRIBUTION_MUX_CONFIG_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG_BIT_LANE_10G_TX_SRC_MSK,
                                                         OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG_BIT_LANE_10G_TX_SRC_OFF,
                                                         value);
}

static INLINE UINT32 otn_mld_field_LANE_10G_TX_SRC_get( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_LANE_10G_TX_SRC_get( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_LANE_10G_TX_SRC_get", N, 11);
    /* ((0x00010038 + (N) * 0x04) bits 7:0) bits 0:7 use field LANE_10G_TX_SRC of register PMC_OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG index N=0..11 */
    reg_value = otn_mld_reg_DISTRIBUTION_MUX_CONFIG_array_read( b_ptr,
                                                                h_ptr,
                                                                N);
    value = (reg_value & OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG_BIT_LANE_10G_TX_SRC_MSK) >> OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG_BIT_LANE_10G_TX_SRC_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otn_mld_field_LANE_10G_TX_SRC_get", N, value );

    return value;
}
static INLINE void otn_mld_field_range_LANE_10G_TX_SRC_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_LANE_10G_TX_SRC_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_range_LANE_10G_TX_SRC_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_LANE_10G_TX_SRC_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_LANE_10G_TX_SRC_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_LANE_10G_TX_SRC_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00010038 + (N) * 0x04) bits 7:0) bits 0:7 use field LANE_10G_TX_SRC of register PMC_OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG index N=0..11 */
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
        /* ((0x00010038 + (N) * 0x04) bits 7:0) bits 0:7 use field LANE_10G_TX_SRC of register PMC_OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG index N=0..11 */
        otn_mld_reg_DISTRIBUTION_MUX_CONFIG_array_field_set( b_ptr,
                                                             h_ptr,
                                                             N,
                                                             subfield_mask << (OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG_BIT_LANE_10G_TX_SRC_OFF + subfield_offset),
                                                             OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG_BIT_LANE_10G_TX_SRC_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_LANE_10G_TX_SRC_get( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_LANE_10G_TX_SRC_get( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_range_LANE_10G_TX_SRC_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_LANE_10G_TX_SRC_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_LANE_10G_TX_SRC_get", stop_bit, 7 );
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
    /* ((0x00010038 + (N) * 0x04) bits 7:0) bits 0:7 use field LANE_10G_TX_SRC of register PMC_OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG index N=0..11 */
    reg_value = otn_mld_reg_DISTRIBUTION_MUX_CONFIG_array_read( b_ptr,
                                                                h_ptr,
                                                                N);
    field_value = (reg_value & OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG_BIT_LANE_10G_TX_SRC_MSK)
                  >> OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG_BIT_LANE_10G_TX_SRC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG_BIT_LANE_10G_TX_SRC_MSK, OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG_BIT_LANE_10G_TX_SRC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_LANE_10G_TX_SRC_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_DEFECT_RCP_CHNL_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_DEFECT_RCP_CHNL_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_DEFECT_RCP_CHNL_set", N, 11);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_DEFECT_RCP_CHNL_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "otn_mld_field_DEFECT_RCP_CHNL_set", N, value );

    /* ((0x000100ac + (N) * 0x4) bits 3:0) bits 0:3 use field DEFECT_RCP_CHNL of register PMC_OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG index N=0..11 */
    otn_mld_reg_DEFECT_RCP_CHNL_CONFIG_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG_BIT_DEFECT_RCP_CHNL_MSK,
                                                        OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG_BIT_DEFECT_RCP_CHNL_OFF,
                                                        value);
}

static INLINE UINT32 otn_mld_field_DEFECT_RCP_CHNL_get( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_DEFECT_RCP_CHNL_get( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_DEFECT_RCP_CHNL_get", N, 11);
    /* ((0x000100ac + (N) * 0x4) bits 3:0) bits 0:3 use field DEFECT_RCP_CHNL of register PMC_OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG index N=0..11 */
    reg_value = otn_mld_reg_DEFECT_RCP_CHNL_CONFIG_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG_BIT_DEFECT_RCP_CHNL_MSK) >> OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG_BIT_DEFECT_RCP_CHNL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otn_mld_field_DEFECT_RCP_CHNL_get", N, value );

    return value;
}
static INLINE void otn_mld_field_range_DEFECT_RCP_CHNL_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_DEFECT_RCP_CHNL_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_range_DEFECT_RCP_CHNL_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_DEFECT_RCP_CHNL_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_DEFECT_RCP_CHNL_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_DEFECT_RCP_CHNL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000100ac + (N) * 0x4) bits 3:0) bits 0:3 use field DEFECT_RCP_CHNL of register PMC_OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG index N=0..11 */
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
        /* ((0x000100ac + (N) * 0x4) bits 3:0) bits 0:3 use field DEFECT_RCP_CHNL of register PMC_OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG index N=0..11 */
        otn_mld_reg_DEFECT_RCP_CHNL_CONFIG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            subfield_mask << (OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG_BIT_DEFECT_RCP_CHNL_OFF + subfield_offset),
                                                            OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG_BIT_DEFECT_RCP_CHNL_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_DEFECT_RCP_CHNL_get( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_DEFECT_RCP_CHNL_get( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_range_DEFECT_RCP_CHNL_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_DEFECT_RCP_CHNL_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_DEFECT_RCP_CHNL_get", stop_bit, 3 );
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
    /* ((0x000100ac + (N) * 0x4) bits 3:0) bits 0:3 use field DEFECT_RCP_CHNL of register PMC_OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG index N=0..11 */
    reg_value = otn_mld_reg_DEFECT_RCP_CHNL_CONFIG_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    field_value = (reg_value & OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG_BIT_DEFECT_RCP_CHNL_MSK)
                  >> OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG_BIT_DEFECT_RCP_CHNL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG_BIT_DEFECT_RCP_CHNL_MSK, OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG_BIT_DEFECT_RCP_CHNL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_DEFECT_RCP_CHNL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_DLOM_TO_FFAIL_MAPPING_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_DLOM_TO_FFAIL_MAPPING_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_DLOM_TO_FFAIL_MAPPING_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_DLOM_TO_FFAIL_MAPPING_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otn_mld_field_DLOM_TO_FFAIL_MAPPING_set", N, value );

    /* ((0x000100dc + (N) * 0x4) bits 0) field DLOM_TO_FFAIL_MAPPING of register PMC_OTN_MLD_REG_DLOM_TO_FFAIL_MAPPING index N=0..11 */
    otn_mld_reg_DLOM_TO_FFAIL_MAPPING_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       OTN_MLD_REG_DLOM_TO_FFAIL_MAPPING_BIT_DLOM_TO_FFAIL_MAPPING_MSK,
                                                       OTN_MLD_REG_DLOM_TO_FFAIL_MAPPING_BIT_DLOM_TO_FFAIL_MAPPING_OFF,
                                                       value);
}

static INLINE UINT32 otn_mld_field_DLOM_TO_FFAIL_MAPPING_get( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_DLOM_TO_FFAIL_MAPPING_get( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_DLOM_TO_FFAIL_MAPPING_get", N, 11);
    /* ((0x000100dc + (N) * 0x4) bits 0) field DLOM_TO_FFAIL_MAPPING of register PMC_OTN_MLD_REG_DLOM_TO_FFAIL_MAPPING index N=0..11 */
    reg_value = otn_mld_reg_DLOM_TO_FFAIL_MAPPING_array_read( b_ptr,
                                                              h_ptr,
                                                              N);
    value = (reg_value & OTN_MLD_REG_DLOM_TO_FFAIL_MAPPING_BIT_DLOM_TO_FFAIL_MAPPING_MSK) >> OTN_MLD_REG_DLOM_TO_FFAIL_MAPPING_BIT_DLOM_TO_FFAIL_MAPPING_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otn_mld_field_DLOM_TO_FFAIL_MAPPING_get", N, value );

    return value;
}
static INLINE void otn_mld_field_BERGEN32_BER_IN_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_BERGEN32_BER_IN_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_BERGEN32_BER_IN_set", N, 11);
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_BERGEN32_BER_IN_set", value, 16777215);
    IOLOG( "%s <= N=%d 0x%08x", "otn_mld_field_BERGEN32_BER_IN_set", N, value );

    /* ((0x000101a4 + (N) * 0x4) bits 31:8) bits 0:23 use field BERGEN32_BER_IN of register PMC_OTN_MLD_REG_BERGEN32_CONFIG1 index N=0..11 */
    otn_mld_reg_BERGEN32_CONFIG1_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BER_IN_MSK,
                                                  OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BER_IN_OFF,
                                                  value);
}

static INLINE UINT32 otn_mld_field_BERGEN32_BER_IN_get( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_BERGEN32_BER_IN_get( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_BERGEN32_BER_IN_get", N, 11);
    /* ((0x000101a4 + (N) * 0x4) bits 31:8) bits 0:23 use field BERGEN32_BER_IN of register PMC_OTN_MLD_REG_BERGEN32_CONFIG1 index N=0..11 */
    reg_value = otn_mld_reg_BERGEN32_CONFIG1_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BER_IN_MSK) >> OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BER_IN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otn_mld_field_BERGEN32_BER_IN_get", N, value );

    return value;
}
static INLINE void otn_mld_field_range_BERGEN32_BER_IN_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_BERGEN32_BER_IN_set( otn_mld_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_range_BERGEN32_BER_IN_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_BERGEN32_BER_IN_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_BERGEN32_BER_IN_set", stop_bit, 23 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_BERGEN32_BER_IN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000101a4 + (N) * 0x4) bits 31:8) bits 0:23 use field BERGEN32_BER_IN of register PMC_OTN_MLD_REG_BERGEN32_CONFIG1 index N=0..11 */
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
        /* ((0x000101a4 + (N) * 0x4) bits 31:8) bits 0:23 use field BERGEN32_BER_IN of register PMC_OTN_MLD_REG_BERGEN32_CONFIG1 index N=0..11 */
        otn_mld_reg_BERGEN32_CONFIG1_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BER_IN_OFF + subfield_offset),
                                                      OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BER_IN_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_BERGEN32_BER_IN_get( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_BERGEN32_BER_IN_get( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_range_BERGEN32_BER_IN_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_BERGEN32_BER_IN_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_BERGEN32_BER_IN_get", stop_bit, 23 );
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
    /* ((0x000101a4 + (N) * 0x4) bits 31:8) bits 0:23 use field BERGEN32_BER_IN of register PMC_OTN_MLD_REG_BERGEN32_CONFIG1 index N=0..11 */
    reg_value = otn_mld_reg_BERGEN32_CONFIG1_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BER_IN_MSK)
                  >> OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BER_IN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BER_IN_MSK, OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BER_IN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_BERGEN32_BER_IN_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_BERGEN32_USE_BITLANE_ERR_LIMIT_set( otn_mld_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_BERGEN32_USE_BITLANE_ERR_LIMIT_set( otn_mld_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_BERGEN32_USE_BITLANE_ERR_LIMIT_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_BERGEN32_USE_BITLANE_ERR_LIMIT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otn_mld_field_BERGEN32_USE_BITLANE_ERR_LIMIT_set", N, value );

    /* ((0x000101a4 + (N) * 0x4) bits 7) field BERGEN32_USE_BITLANE_ERR_LIMIT of register PMC_OTN_MLD_REG_BERGEN32_CONFIG1 index N=0..11 */
    otn_mld_reg_BERGEN32_CONFIG1_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_USE_BITLANE_ERR_LIMIT_MSK,
                                                  OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_USE_BITLANE_ERR_LIMIT_OFF,
                                                  value);
}

static INLINE UINT32 otn_mld_field_BERGEN32_USE_BITLANE_ERR_LIMIT_get( otn_mld_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_BERGEN32_USE_BITLANE_ERR_LIMIT_get( otn_mld_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_BERGEN32_USE_BITLANE_ERR_LIMIT_get", N, 11);
    /* ((0x000101a4 + (N) * 0x4) bits 7) field BERGEN32_USE_BITLANE_ERR_LIMIT of register PMC_OTN_MLD_REG_BERGEN32_CONFIG1 index N=0..11 */
    reg_value = otn_mld_reg_BERGEN32_CONFIG1_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_USE_BITLANE_ERR_LIMIT_MSK) >> OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_USE_BITLANE_ERR_LIMIT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otn_mld_field_BERGEN32_USE_BITLANE_ERR_LIMIT_get", N, value );

    return value;
}
static INLINE void otn_mld_field_BERGEN32_BITLANE_ERR_LIMIT_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_BERGEN32_BITLANE_ERR_LIMIT_set( otn_mld_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_BERGEN32_BITLANE_ERR_LIMIT_set", N, 11);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_BERGEN32_BITLANE_ERR_LIMIT_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "otn_mld_field_BERGEN32_BITLANE_ERR_LIMIT_set", N, value );

    /* ((0x000101a4 + (N) * 0x4) bits 6:3) bits 0:3 use field BERGEN32_BITLANE_ERR_LIMIT of register PMC_OTN_MLD_REG_BERGEN32_CONFIG1 index N=0..11 */
    otn_mld_reg_BERGEN32_CONFIG1_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BITLANE_ERR_LIMIT_MSK,
                                                  OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BITLANE_ERR_LIMIT_OFF,
                                                  value);
}

static INLINE UINT32 otn_mld_field_BERGEN32_BITLANE_ERR_LIMIT_get( otn_mld_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_BERGEN32_BITLANE_ERR_LIMIT_get( otn_mld_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_BERGEN32_BITLANE_ERR_LIMIT_get", N, 11);
    /* ((0x000101a4 + (N) * 0x4) bits 6:3) bits 0:3 use field BERGEN32_BITLANE_ERR_LIMIT of register PMC_OTN_MLD_REG_BERGEN32_CONFIG1 index N=0..11 */
    reg_value = otn_mld_reg_BERGEN32_CONFIG1_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BITLANE_ERR_LIMIT_MSK) >> OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BITLANE_ERR_LIMIT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otn_mld_field_BERGEN32_BITLANE_ERR_LIMIT_get", N, value );

    return value;
}
static INLINE void otn_mld_field_range_BERGEN32_BITLANE_ERR_LIMIT_set( otn_mld_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_BERGEN32_BITLANE_ERR_LIMIT_set( otn_mld_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_range_BERGEN32_BITLANE_ERR_LIMIT_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_BERGEN32_BITLANE_ERR_LIMIT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_BERGEN32_BITLANE_ERR_LIMIT_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_BERGEN32_BITLANE_ERR_LIMIT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000101a4 + (N) * 0x4) bits 6:3) bits 0:3 use field BERGEN32_BITLANE_ERR_LIMIT of register PMC_OTN_MLD_REG_BERGEN32_CONFIG1 index N=0..11 */
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
        /* ((0x000101a4 + (N) * 0x4) bits 6:3) bits 0:3 use field BERGEN32_BITLANE_ERR_LIMIT of register PMC_OTN_MLD_REG_BERGEN32_CONFIG1 index N=0..11 */
        otn_mld_reg_BERGEN32_CONFIG1_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BITLANE_ERR_LIMIT_OFF + subfield_offset),
                                                      OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BITLANE_ERR_LIMIT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_BERGEN32_BITLANE_ERR_LIMIT_get( otn_mld_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_BERGEN32_BITLANE_ERR_LIMIT_get( otn_mld_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_range_BERGEN32_BITLANE_ERR_LIMIT_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_BERGEN32_BITLANE_ERR_LIMIT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_BERGEN32_BITLANE_ERR_LIMIT_get", stop_bit, 3 );
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
    /* ((0x000101a4 + (N) * 0x4) bits 6:3) bits 0:3 use field BERGEN32_BITLANE_ERR_LIMIT of register PMC_OTN_MLD_REG_BERGEN32_CONFIG1 index N=0..11 */
    reg_value = otn_mld_reg_BERGEN32_CONFIG1_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BITLANE_ERR_LIMIT_MSK)
                  >> OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BITLANE_ERR_LIMIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BITLANE_ERR_LIMIT_MSK, OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BITLANE_ERR_LIMIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_BERGEN32_BITLANE_ERR_LIMIT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_BERGEN32_ERR_MODE_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_BERGEN32_ERR_MODE_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_BERGEN32_ERR_MODE_set", N, 11);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_BERGEN32_ERR_MODE_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "otn_mld_field_BERGEN32_ERR_MODE_set", N, value );

    /* ((0x000101a4 + (N) * 0x4) bits 2:1) bits 0:1 use field BERGEN32_ERR_MODE of register PMC_OTN_MLD_REG_BERGEN32_CONFIG1 index N=0..11 */
    otn_mld_reg_BERGEN32_CONFIG1_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_ERR_MODE_MSK,
                                                  OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_ERR_MODE_OFF,
                                                  value);
}

static INLINE UINT32 otn_mld_field_BERGEN32_ERR_MODE_get( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_BERGEN32_ERR_MODE_get( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_BERGEN32_ERR_MODE_get", N, 11);
    /* ((0x000101a4 + (N) * 0x4) bits 2:1) bits 0:1 use field BERGEN32_ERR_MODE of register PMC_OTN_MLD_REG_BERGEN32_CONFIG1 index N=0..11 */
    reg_value = otn_mld_reg_BERGEN32_CONFIG1_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_ERR_MODE_MSK) >> OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_ERR_MODE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otn_mld_field_BERGEN32_ERR_MODE_get", N, value );

    return value;
}
static INLINE void otn_mld_field_range_BERGEN32_ERR_MODE_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_BERGEN32_ERR_MODE_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_range_BERGEN32_ERR_MODE_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_BERGEN32_ERR_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_BERGEN32_ERR_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_BERGEN32_ERR_MODE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000101a4 + (N) * 0x4) bits 2:1) bits 0:1 use field BERGEN32_ERR_MODE of register PMC_OTN_MLD_REG_BERGEN32_CONFIG1 index N=0..11 */
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
        /* ((0x000101a4 + (N) * 0x4) bits 2:1) bits 0:1 use field BERGEN32_ERR_MODE of register PMC_OTN_MLD_REG_BERGEN32_CONFIG1 index N=0..11 */
        otn_mld_reg_BERGEN32_CONFIG1_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_ERR_MODE_OFF + subfield_offset),
                                                      OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_ERR_MODE_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_BERGEN32_ERR_MODE_get( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_BERGEN32_ERR_MODE_get( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_range_BERGEN32_ERR_MODE_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_BERGEN32_ERR_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_BERGEN32_ERR_MODE_get", stop_bit, 1 );
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
    /* ((0x000101a4 + (N) * 0x4) bits 2:1) bits 0:1 use field BERGEN32_ERR_MODE of register PMC_OTN_MLD_REG_BERGEN32_CONFIG1 index N=0..11 */
    reg_value = otn_mld_reg_BERGEN32_CONFIG1_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_ERR_MODE_MSK)
                  >> OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_ERR_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_ERR_MODE_MSK, OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_ERR_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_BERGEN32_ERR_MODE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_BERGEN32_ENABLE_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_BERGEN32_ENABLE_set( otn_mld_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_BERGEN32_ENABLE_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_BERGEN32_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otn_mld_field_BERGEN32_ENABLE_set", N, value );

    /* ((0x000101a4 + (N) * 0x4) bits 0) field BERGEN32_ENABLE of register PMC_OTN_MLD_REG_BERGEN32_CONFIG1 index N=0..11 */
    otn_mld_reg_BERGEN32_CONFIG1_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_ENABLE_MSK,
                                                  OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_ENABLE_OFF,
                                                  value);
}

static INLINE UINT32 otn_mld_field_BERGEN32_ENABLE_get( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_BERGEN32_ENABLE_get( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_BERGEN32_ENABLE_get", N, 11);
    /* ((0x000101a4 + (N) * 0x4) bits 0) field BERGEN32_ENABLE of register PMC_OTN_MLD_REG_BERGEN32_CONFIG1 index N=0..11 */
    reg_value = otn_mld_reg_BERGEN32_CONFIG1_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_ENABLE_MSK) >> OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otn_mld_field_BERGEN32_ENABLE_get", N, value );

    return value;
}
static INLINE void otn_mld_field_BERGEN32_ENABLE_BITLANE_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_BERGEN32_ENABLE_BITLANE_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_BERGEN32_ENABLE_BITLANE_set", N, 11);
    IOLOG( "%s <= N=%d 0x%08x", "otn_mld_field_BERGEN32_ENABLE_BITLANE_set", N, value );

    /* ((0x000101d4 + (N) * 0x4) bits 31:0) bits 0:31 use field BERGEN32_ENABLE_BITLANE of register PMC_OTN_MLD_REG_BERGEN32_CONFIG2 index N=0..11 */
    otn_mld_reg_BERGEN32_CONFIG2_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OTN_MLD_REG_BERGEN32_CONFIG2_BIT_BERGEN32_ENABLE_BITLANE_MSK,
                                                  OTN_MLD_REG_BERGEN32_CONFIG2_BIT_BERGEN32_ENABLE_BITLANE_OFF,
                                                  value);
}

static INLINE UINT32 otn_mld_field_BERGEN32_ENABLE_BITLANE_get( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_BERGEN32_ENABLE_BITLANE_get( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_BERGEN32_ENABLE_BITLANE_get", N, 11);
    /* ((0x000101d4 + (N) * 0x4) bits 31:0) bits 0:31 use field BERGEN32_ENABLE_BITLANE of register PMC_OTN_MLD_REG_BERGEN32_CONFIG2 index N=0..11 */
    reg_value = otn_mld_reg_BERGEN32_CONFIG2_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OTN_MLD_REG_BERGEN32_CONFIG2_BIT_BERGEN32_ENABLE_BITLANE_MSK) >> OTN_MLD_REG_BERGEN32_CONFIG2_BIT_BERGEN32_ENABLE_BITLANE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otn_mld_field_BERGEN32_ENABLE_BITLANE_get", N, value );

    return value;
}
static INLINE void otn_mld_field_range_BERGEN32_ENABLE_BITLANE_set( otn_mld_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_BERGEN32_ENABLE_BITLANE_set( otn_mld_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_range_BERGEN32_ENABLE_BITLANE_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_BERGEN32_ENABLE_BITLANE_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_BERGEN32_ENABLE_BITLANE_set", stop_bit, 31 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_BERGEN32_ENABLE_BITLANE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000101d4 + (N) * 0x4) bits 31:0) bits 0:31 use field BERGEN32_ENABLE_BITLANE of register PMC_OTN_MLD_REG_BERGEN32_CONFIG2 index N=0..11 */
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
        /* ((0x000101d4 + (N) * 0x4) bits 31:0) bits 0:31 use field BERGEN32_ENABLE_BITLANE of register PMC_OTN_MLD_REG_BERGEN32_CONFIG2 index N=0..11 */
        otn_mld_reg_BERGEN32_CONFIG2_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (OTN_MLD_REG_BERGEN32_CONFIG2_BIT_BERGEN32_ENABLE_BITLANE_OFF + subfield_offset),
                                                      OTN_MLD_REG_BERGEN32_CONFIG2_BIT_BERGEN32_ENABLE_BITLANE_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_BERGEN32_ENABLE_BITLANE_get( otn_mld_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_BERGEN32_ENABLE_BITLANE_get( otn_mld_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_range_BERGEN32_ENABLE_BITLANE_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_BERGEN32_ENABLE_BITLANE_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_BERGEN32_ENABLE_BITLANE_get", stop_bit, 31 );
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
    /* ((0x000101d4 + (N) * 0x4) bits 31:0) bits 0:31 use field BERGEN32_ENABLE_BITLANE of register PMC_OTN_MLD_REG_BERGEN32_CONFIG2 index N=0..11 */
    reg_value = otn_mld_reg_BERGEN32_CONFIG2_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & OTN_MLD_REG_BERGEN32_CONFIG2_BIT_BERGEN32_ENABLE_BITLANE_MSK)
                  >> OTN_MLD_REG_BERGEN32_CONFIG2_BIT_BERGEN32_ENABLE_BITLANE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_BERGEN32_CONFIG2_BIT_BERGEN32_ENABLE_BITLANE_MSK, OTN_MLD_REG_BERGEN32_CONFIG2_BIT_BERGEN32_ENABLE_BITLANE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_BERGEN32_ENABLE_BITLANE_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size3
 * ==================================================================================
 */
static INLINE void otn_mld_field_PRECODER_128_RX_BYPASS_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_PRECODER_128_RX_BYPASS_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_PRECODER_128_RX_BYPASS_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_PRECODER_128_RX_BYPASS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otn_mld_field_PRECODER_128_RX_BYPASS_set", N, value );

    /* ((0x000100a0 + (N) * 0x04) bits 2) field PRECODER_128_RX_BYPASS of register PMC_OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG index N=0..2 */
    otn_mld_reg_OPSM23_SFI_PATH_CONFIG_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG_BIT_PRECODER_128_RX_BYPASS_MSK,
                                                        OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG_BIT_PRECODER_128_RX_BYPASS_OFF,
                                                        value);
}

static INLINE UINT32 otn_mld_field_PRECODER_128_RX_BYPASS_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_PRECODER_128_RX_BYPASS_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_PRECODER_128_RX_BYPASS_get", N, 2);
    /* ((0x000100a0 + (N) * 0x04) bits 2) field PRECODER_128_RX_BYPASS of register PMC_OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG index N=0..2 */
    reg_value = otn_mld_reg_OPSM23_SFI_PATH_CONFIG_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG_BIT_PRECODER_128_RX_BYPASS_MSK) >> OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG_BIT_PRECODER_128_RX_BYPASS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otn_mld_field_PRECODER_128_RX_BYPASS_get", N, value );

    return value;
}
static INLINE void otn_mld_field_PRECODER_128_TX_BYPASS_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_PRECODER_128_TX_BYPASS_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_PRECODER_128_TX_BYPASS_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_PRECODER_128_TX_BYPASS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otn_mld_field_PRECODER_128_TX_BYPASS_set", N, value );

    /* ((0x000100a0 + (N) * 0x04) bits 1) field PRECODER_128_TX_BYPASS of register PMC_OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG index N=0..2 */
    otn_mld_reg_OPSM23_SFI_PATH_CONFIG_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG_BIT_PRECODER_128_TX_BYPASS_MSK,
                                                        OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG_BIT_PRECODER_128_TX_BYPASS_OFF,
                                                        value);
}

static INLINE UINT32 otn_mld_field_PRECODER_128_TX_BYPASS_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_PRECODER_128_TX_BYPASS_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_PRECODER_128_TX_BYPASS_get", N, 2);
    /* ((0x000100a0 + (N) * 0x04) bits 1) field PRECODER_128_TX_BYPASS of register PMC_OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG index N=0..2 */
    reg_value = otn_mld_reg_OPSM23_SFI_PATH_CONFIG_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG_BIT_PRECODER_128_TX_BYPASS_MSK) >> OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG_BIT_PRECODER_128_TX_BYPASS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otn_mld_field_PRECODER_128_TX_BYPASS_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size36
 * ==================================================================================
 */
static INLINE void otn_mld_field_RINGOSC_32_BYPASS_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_RINGOSC_32_BYPASS_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 35)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_RINGOSC_32_BYPASS_set", N, 35);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_RINGOSC_32_BYPASS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otn_mld_field_RINGOSC_32_BYPASS_set", N, value );

    /* ((0x00010114 + (N) * 0x4) bits 1) field RINGOSC_32_BYPASS of register PMC_OTN_MLD_REG_RINGOSC_32_CONFIG index N=0..35 */
    otn_mld_reg_RINGOSC_32_CONFIG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   OTN_MLD_REG_RINGOSC_32_CONFIG_BIT_RINGOSC_32_BYPASS_MSK,
                                                   OTN_MLD_REG_RINGOSC_32_CONFIG_BIT_RINGOSC_32_BYPASS_OFF,
                                                   value);
}

static INLINE UINT32 otn_mld_field_RINGOSC_32_BYPASS_get( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_RINGOSC_32_BYPASS_get( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 35)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_RINGOSC_32_BYPASS_get", N, 35);
    /* ((0x00010114 + (N) * 0x4) bits 1) field RINGOSC_32_BYPASS of register PMC_OTN_MLD_REG_RINGOSC_32_CONFIG index N=0..35 */
    reg_value = otn_mld_reg_RINGOSC_32_CONFIG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OTN_MLD_REG_RINGOSC_32_CONFIG_BIT_RINGOSC_32_BYPASS_MSK) >> OTN_MLD_REG_RINGOSC_32_CONFIG_BIT_RINGOSC_32_BYPASS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otn_mld_field_RINGOSC_32_BYPASS_get", N, value );

    return value;
}
static INLINE void otn_mld_field_RINGOSC_32_ENABLE_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_RINGOSC_32_ENABLE_set( otn_mld_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 35)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_RINGOSC_32_ENABLE_set", N, 35);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_RINGOSC_32_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "otn_mld_field_RINGOSC_32_ENABLE_set", N, value );

    /* ((0x00010114 + (N) * 0x4) bits 0) field RINGOSC_32_ENABLE of register PMC_OTN_MLD_REG_RINGOSC_32_CONFIG index N=0..35 */
    otn_mld_reg_RINGOSC_32_CONFIG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   OTN_MLD_REG_RINGOSC_32_CONFIG_BIT_RINGOSC_32_ENABLE_MSK,
                                                   OTN_MLD_REG_RINGOSC_32_CONFIG_BIT_RINGOSC_32_ENABLE_OFF,
                                                   value);
}

static INLINE UINT32 otn_mld_field_RINGOSC_32_ENABLE_get( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_RINGOSC_32_ENABLE_get( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 35)
        IO_RANGE_CHECK("%s N is %d but max is %d", "otn_mld_field_RINGOSC_32_ENABLE_get", N, 35);
    /* ((0x00010114 + (N) * 0x4) bits 0) field RINGOSC_32_ENABLE of register PMC_OTN_MLD_REG_RINGOSC_32_CONFIG index N=0..35 */
    reg_value = otn_mld_reg_RINGOSC_32_CONFIG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OTN_MLD_REG_RINGOSC_32_CONFIG_BIT_RINGOSC_32_ENABLE_MSK) >> OTN_MLD_REG_RINGOSC_32_CONFIG_BIT_RINGOSC_32_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "otn_mld_field_RINGOSC_32_ENABLE_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void otn_mld_field_HADAPT40G0_INT_E_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G0_INT_E_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G0_INT_E_set", value );

    /* (0x00010330 bits 31:0) bits 0:31 use field HADAPT40G0_INT_E of register PMC_OTN_MLD_REG_HADAPT40G0_INT_EN */
    otn_mld_reg_HADAPT40G0_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             OTN_MLD_REG_HADAPT40G0_INT_EN_BIT_HADAPT40G0_INT_E_MSK,
                                             OTN_MLD_REG_HADAPT40G0_INT_EN_BIT_HADAPT40G0_INT_E_OFF,
                                             value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G0_INT_E_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G0_INT_E_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010330 bits 31:0) bits 0:31 use field HADAPT40G0_INT_E of register PMC_OTN_MLD_REG_HADAPT40G0_INT_EN */
    reg_value = otn_mld_reg_HADAPT40G0_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G0_INT_EN_BIT_HADAPT40G0_INT_E_MSK) >> OTN_MLD_REG_HADAPT40G0_INT_EN_BIT_HADAPT40G0_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G0_INT_E_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT40G0_INT_E_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT40G0_INT_E_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G0_INT_E_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G0_INT_E_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G0_INT_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010330 bits 31:0) bits 0:31 use field HADAPT40G0_INT_E of register PMC_OTN_MLD_REG_HADAPT40G0_INT_EN */
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
        /* (0x00010330 bits 31:0) bits 0:31 use field HADAPT40G0_INT_E of register PMC_OTN_MLD_REG_HADAPT40G0_INT_EN */
        otn_mld_reg_HADAPT40G0_INT_EN_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (OTN_MLD_REG_HADAPT40G0_INT_EN_BIT_HADAPT40G0_INT_E_OFF + subfield_offset),
                                                 OTN_MLD_REG_HADAPT40G0_INT_EN_BIT_HADAPT40G0_INT_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT40G0_INT_E_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G0_INT_E_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G0_INT_E_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G0_INT_E_get", stop_bit, 31 );
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
    /* (0x00010330 bits 31:0) bits 0:31 use field HADAPT40G0_INT_E of register PMC_OTN_MLD_REG_HADAPT40G0_INT_EN */
    reg_value = otn_mld_reg_HADAPT40G0_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT40G0_INT_EN_BIT_HADAPT40G0_INT_E_MSK)
                  >> OTN_MLD_REG_HADAPT40G0_INT_EN_BIT_HADAPT40G0_INT_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT40G0_INT_EN_BIT_HADAPT40G0_INT_E_MSK, OTN_MLD_REG_HADAPT40G0_INT_EN_BIT_HADAPT40G0_INT_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G0_INT_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT40G1_INT_E_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G1_INT_E_set( otn_mld_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G1_INT_E_set", value );

    /* (0x00010358 bits 31:0) bits 0:31 use field HADAPT40G1_INT_E of register PMC_OTN_MLD_REG_HADAPT40G1_INT_EN */
    otn_mld_reg_HADAPT40G1_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             OTN_MLD_REG_HADAPT40G1_INT_EN_BIT_HADAPT40G1_INT_E_MSK,
                                             OTN_MLD_REG_HADAPT40G1_INT_EN_BIT_HADAPT40G1_INT_E_OFF,
                                             value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G1_INT_E_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G1_INT_E_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010358 bits 31:0) bits 0:31 use field HADAPT40G1_INT_E of register PMC_OTN_MLD_REG_HADAPT40G1_INT_EN */
    reg_value = otn_mld_reg_HADAPT40G1_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G1_INT_EN_BIT_HADAPT40G1_INT_E_MSK) >> OTN_MLD_REG_HADAPT40G1_INT_EN_BIT_HADAPT40G1_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G1_INT_E_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT40G1_INT_E_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT40G1_INT_E_set( otn_mld_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G1_INT_E_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G1_INT_E_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G1_INT_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010358 bits 31:0) bits 0:31 use field HADAPT40G1_INT_E of register PMC_OTN_MLD_REG_HADAPT40G1_INT_EN */
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
        /* (0x00010358 bits 31:0) bits 0:31 use field HADAPT40G1_INT_E of register PMC_OTN_MLD_REG_HADAPT40G1_INT_EN */
        otn_mld_reg_HADAPT40G1_INT_EN_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (OTN_MLD_REG_HADAPT40G1_INT_EN_BIT_HADAPT40G1_INT_E_OFF + subfield_offset),
                                                 OTN_MLD_REG_HADAPT40G1_INT_EN_BIT_HADAPT40G1_INT_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT40G1_INT_E_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G1_INT_E_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G1_INT_E_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G1_INT_E_get", stop_bit, 31 );
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
    /* (0x00010358 bits 31:0) bits 0:31 use field HADAPT40G1_INT_E of register PMC_OTN_MLD_REG_HADAPT40G1_INT_EN */
    reg_value = otn_mld_reg_HADAPT40G1_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT40G1_INT_EN_BIT_HADAPT40G1_INT_E_MSK)
                  >> OTN_MLD_REG_HADAPT40G1_INT_EN_BIT_HADAPT40G1_INT_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT40G1_INT_EN_BIT_HADAPT40G1_INT_E_MSK, OTN_MLD_REG_HADAPT40G1_INT_EN_BIT_HADAPT40G1_INT_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G1_INT_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_RXCLK2_INT_E_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_RXCLK2_INT_E_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_HADAPT100G_RXCLK2_INT_E_set", value, 255);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_RXCLK2_INT_E_set", value );

    /* (0x000103ac bits 15:8) bits 0:7 use field HADAPT100G_RXCLK2_INT_E of register PMC_OTN_MLD_REG_HADAPT100G_INT_EN */
    otn_mld_reg_HADAPT100G_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK2_INT_E_MSK,
                                             OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK2_INT_E_OFF,
                                             value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_RXCLK2_INT_E_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_RXCLK2_INT_E_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000103ac bits 15:8) bits 0:7 use field HADAPT100G_RXCLK2_INT_E of register PMC_OTN_MLD_REG_HADAPT100G_INT_EN */
    reg_value = otn_mld_reg_HADAPT100G_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK2_INT_E_MSK) >> OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK2_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_RXCLK2_INT_E_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_RXCLK2_INT_E_set( otn_mld_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_RXCLK2_INT_E_set( otn_mld_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_E_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_E_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000103ac bits 15:8) bits 0:7 use field HADAPT100G_RXCLK2_INT_E of register PMC_OTN_MLD_REG_HADAPT100G_INT_EN */
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
        /* (0x000103ac bits 15:8) bits 0:7 use field HADAPT100G_RXCLK2_INT_E of register PMC_OTN_MLD_REG_HADAPT100G_INT_EN */
        otn_mld_reg_HADAPT100G_INT_EN_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK2_INT_E_OFF + subfield_offset),
                                                 OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK2_INT_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_RXCLK2_INT_E_get( otn_mld_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_RXCLK2_INT_E_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_E_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_E_get", stop_bit, 7 );
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
    /* (0x000103ac bits 15:8) bits 0:7 use field HADAPT100G_RXCLK2_INT_E of register PMC_OTN_MLD_REG_HADAPT100G_INT_EN */
    reg_value = otn_mld_reg_HADAPT100G_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK2_INT_E_MSK)
                  >> OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK2_INT_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK2_INT_E_MSK, OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK2_INT_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void otn_mld_field_HADAPT100G_RXCLK1_INT_E_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_RXCLK1_INT_E_set( otn_mld_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_HADAPT100G_RXCLK1_INT_E_set", value, 255);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_RXCLK1_INT_E_set", value );

    /* (0x000103ac bits 7:0) bits 0:7 use field HADAPT100G_RXCLK1_INT_E of register PMC_OTN_MLD_REG_HADAPT100G_INT_EN */
    otn_mld_reg_HADAPT100G_INT_EN_field_set( b_ptr,
                                             h_ptr,
                                             OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK1_INT_E_MSK,
                                             OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK1_INT_E_OFF,
                                             value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_RXCLK1_INT_E_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_RXCLK1_INT_E_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000103ac bits 7:0) bits 0:7 use field HADAPT100G_RXCLK1_INT_E of register PMC_OTN_MLD_REG_HADAPT100G_INT_EN */
    reg_value = otn_mld_reg_HADAPT100G_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK1_INT_E_MSK) >> OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK1_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_RXCLK1_INT_E_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_RXCLK1_INT_E_set( otn_mld_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_RXCLK1_INT_E_set( otn_mld_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_E_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_E_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000103ac bits 7:0) bits 0:7 use field HADAPT100G_RXCLK1_INT_E of register PMC_OTN_MLD_REG_HADAPT100G_INT_EN */
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
        /* (0x000103ac bits 7:0) bits 0:7 use field HADAPT100G_RXCLK1_INT_E of register PMC_OTN_MLD_REG_HADAPT100G_INT_EN */
        otn_mld_reg_HADAPT100G_INT_EN_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK1_INT_E_OFF + subfield_offset),
                                                 OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK1_INT_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_RXCLK1_INT_E_get( otn_mld_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_RXCLK1_INT_E_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_E_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_E_get", stop_bit, 7 );
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
    /* (0x000103ac bits 7:0) bits 0:7 use field HADAPT100G_RXCLK1_INT_E of register PMC_OTN_MLD_REG_HADAPT100G_INT_EN */
    reg_value = otn_mld_reg_HADAPT100G_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK1_INT_E_MSK)
                  >> OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK1_INT_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK1_INT_E_MSK, OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK1_INT_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_E_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_summary
 * ==================================================================================
 */
static INLINE UINT32 otn_mld_field_HADAPT100G_INT_SUMMARY_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_INT_SUMMARY_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010110 bits 2) field HADAPT100G_INT_SUMMARY of register PMC_OTN_MLD_REG_HADAPT_INT_SUMMARY */
    reg_value = otn_mld_reg_HADAPT_INT_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT_INT_SUMMARY_BIT_HADAPT100G_INT_SUMMARY_MSK) >> OTN_MLD_REG_HADAPT_INT_SUMMARY_BIT_HADAPT100G_INT_SUMMARY_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_INT_SUMMARY_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT100G_INT_SUMMARY_poll( otn_mld_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT100G_INT_SUMMARY_poll( otn_mld_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otn_mld_field_HADAPT100G_INT_SUMMARY_poll", value );

    /* (0x00010110 bits 2) field HADAPT100G_INT_SUMMARY of register PMC_OTN_MLD_REG_HADAPT_INT_SUMMARY */
    return otn_mld_reg_HADAPT_INT_SUMMARY_poll( b_ptr,
                                                h_ptr,
                                                OTN_MLD_REG_HADAPT_INT_SUMMARY_BIT_HADAPT100G_INT_SUMMARY_MSK,
                                                (value<<OTN_MLD_REG_HADAPT_INT_SUMMARY_BIT_HADAPT100G_INT_SUMMARY_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 otn_mld_field_HADAPT40G1_INT_SUMMARY_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G1_INT_SUMMARY_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010110 bits 1) field HADAPT40G1_INT_SUMMARY of register PMC_OTN_MLD_REG_HADAPT_INT_SUMMARY */
    reg_value = otn_mld_reg_HADAPT_INT_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT_INT_SUMMARY_BIT_HADAPT40G1_INT_SUMMARY_MSK) >> OTN_MLD_REG_HADAPT_INT_SUMMARY_BIT_HADAPT40G1_INT_SUMMARY_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G1_INT_SUMMARY_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT40G1_INT_SUMMARY_poll( otn_mld_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT40G1_INT_SUMMARY_poll( otn_mld_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otn_mld_field_HADAPT40G1_INT_SUMMARY_poll", value );

    /* (0x00010110 bits 1) field HADAPT40G1_INT_SUMMARY of register PMC_OTN_MLD_REG_HADAPT_INT_SUMMARY */
    return otn_mld_reg_HADAPT_INT_SUMMARY_poll( b_ptr,
                                                h_ptr,
                                                OTN_MLD_REG_HADAPT_INT_SUMMARY_BIT_HADAPT40G1_INT_SUMMARY_MSK,
                                                (value<<OTN_MLD_REG_HADAPT_INT_SUMMARY_BIT_HADAPT40G1_INT_SUMMARY_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 otn_mld_field_HADAPT40G0_INT_SUMMARY_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G0_INT_SUMMARY_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010110 bits 0) field HADAPT40G0_INT_SUMMARY of register PMC_OTN_MLD_REG_HADAPT_INT_SUMMARY */
    reg_value = otn_mld_reg_HADAPT_INT_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT_INT_SUMMARY_BIT_HADAPT40G0_INT_SUMMARY_MSK) >> OTN_MLD_REG_HADAPT_INT_SUMMARY_BIT_HADAPT40G0_INT_SUMMARY_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G0_INT_SUMMARY_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT40G0_INT_SUMMARY_poll( otn_mld_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT40G0_INT_SUMMARY_poll( otn_mld_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otn_mld_field_HADAPT40G0_INT_SUMMARY_poll", value );

    /* (0x00010110 bits 0) field HADAPT40G0_INT_SUMMARY of register PMC_OTN_MLD_REG_HADAPT_INT_SUMMARY */
    return otn_mld_reg_HADAPT_INT_SUMMARY_poll( b_ptr,
                                                h_ptr,
                                                OTN_MLD_REG_HADAPT_INT_SUMMARY_BIT_HADAPT40G0_INT_SUMMARY_MSK,
                                                (value<<OTN_MLD_REG_HADAPT_INT_SUMMARY_BIT_HADAPT40G0_INT_SUMMARY_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void otn_mld_field_HADAPT40G0_INT_I_set_to_clear( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G0_INT_I_set_to_clear( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G0_INT_I_set_to_clear", value );

    /* (0x00010334 bits 31:0) bits 0:31 use field HADAPT40G0_INT_I of register PMC_OTN_MLD_REG_HADAPT40G0_INT */
    otn_mld_reg_HADAPT40G0_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          OTN_MLD_REG_HADAPT40G0_INT_BIT_HADAPT40G0_INT_I_MSK,
                                                          OTN_MLD_REG_HADAPT40G0_INT_BIT_HADAPT40G0_INT_I_OFF,
                                                          value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G0_INT_I_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G0_INT_I_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010334 bits 31:0) bits 0:31 use field HADAPT40G0_INT_I of register PMC_OTN_MLD_REG_HADAPT40G0_INT */
    reg_value = otn_mld_reg_HADAPT40G0_INT_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G0_INT_BIT_HADAPT40G0_INT_I_MSK) >> OTN_MLD_REG_HADAPT40G0_INT_BIT_HADAPT40G0_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G0_INT_I_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT40G0_INT_I_set_to_clear( otn_mld_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT40G0_INT_I_set_to_clear( otn_mld_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G0_INT_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G0_INT_I_set_to_clear", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G0_INT_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010334 bits 31:0) bits 0:31 use field HADAPT40G0_INT_I of register PMC_OTN_MLD_REG_HADAPT40G0_INT */
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
        /* (0x00010334 bits 31:0) bits 0:31 use field HADAPT40G0_INT_I of register PMC_OTN_MLD_REG_HADAPT40G0_INT */
        otn_mld_reg_HADAPT40G0_INT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (OTN_MLD_REG_HADAPT40G0_INT_BIT_HADAPT40G0_INT_I_OFF + subfield_offset),
                                                              OTN_MLD_REG_HADAPT40G0_INT_BIT_HADAPT40G0_INT_I_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT40G0_INT_I_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G0_INT_I_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G0_INT_I_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G0_INT_I_get", stop_bit, 31 );
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
    /* (0x00010334 bits 31:0) bits 0:31 use field HADAPT40G0_INT_I of register PMC_OTN_MLD_REG_HADAPT40G0_INT */
    reg_value = otn_mld_reg_HADAPT40G0_INT_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT40G0_INT_BIT_HADAPT40G0_INT_I_MSK)
                  >> OTN_MLD_REG_HADAPT40G0_INT_BIT_HADAPT40G0_INT_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT40G0_INT_BIT_HADAPT40G0_INT_I_MSK, OTN_MLD_REG_HADAPT40G0_INT_BIT_HADAPT40G0_INT_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G0_INT_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT40G0_INT_I_poll( otn_mld_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT40G0_INT_I_poll( otn_mld_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G0_INT_I_poll", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G0_INT_I_poll", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G0_INT_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010334 bits 31:0) bits 0:31 use field HADAPT40G0_INT_I of register PMC_OTN_MLD_REG_HADAPT40G0_INT */
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
        /* (0x00010334 bits 31:0) bits 0:31 use field HADAPT40G0_INT_I of register PMC_OTN_MLD_REG_HADAPT40G0_INT */
        return otn_mld_reg_HADAPT40G0_INT_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT40G0_INT_BIT_HADAPT40G0_INT_I_OFF + subfield_offset),
                                                value << (OTN_MLD_REG_HADAPT40G0_INT_BIT_HADAPT40G0_INT_I_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT40G0_INT_I_poll( otn_mld_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT40G0_INT_I_poll( otn_mld_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otn_mld_field_HADAPT40G0_INT_I_poll", value );

    /* (0x00010334 bits 31:0) bits 0:31 use field HADAPT40G0_INT_I of register PMC_OTN_MLD_REG_HADAPT40G0_INT */
    return otn_mld_reg_HADAPT40G0_INT_poll( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT40G0_INT_BIT_HADAPT40G0_INT_I_MSK,
                                            (value<<OTN_MLD_REG_HADAPT40G0_INT_BIT_HADAPT40G0_INT_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void otn_mld_field_HADAPT40G1_INT_I_set_to_clear( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT40G1_INT_I_set_to_clear( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT40G1_INT_I_set_to_clear", value );

    /* (0x0001035c bits 31:0) bits 0:31 use field HADAPT40G1_INT_I of register PMC_OTN_MLD_REG_HADAPT40G1_INT */
    otn_mld_reg_HADAPT40G1_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          OTN_MLD_REG_HADAPT40G1_INT_BIT_HADAPT40G1_INT_I_MSK,
                                                          OTN_MLD_REG_HADAPT40G1_INT_BIT_HADAPT40G1_INT_I_OFF,
                                                          value);
}

static INLINE UINT32 otn_mld_field_HADAPT40G1_INT_I_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G1_INT_I_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001035c bits 31:0) bits 0:31 use field HADAPT40G1_INT_I of register PMC_OTN_MLD_REG_HADAPT40G1_INT */
    reg_value = otn_mld_reg_HADAPT40G1_INT_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G1_INT_BIT_HADAPT40G1_INT_I_MSK) >> OTN_MLD_REG_HADAPT40G1_INT_BIT_HADAPT40G1_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G1_INT_I_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT40G1_INT_I_set_to_clear( otn_mld_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT40G1_INT_I_set_to_clear( otn_mld_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G1_INT_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G1_INT_I_set_to_clear", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G1_INT_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001035c bits 31:0) bits 0:31 use field HADAPT40G1_INT_I of register PMC_OTN_MLD_REG_HADAPT40G1_INT */
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
        /* (0x0001035c bits 31:0) bits 0:31 use field HADAPT40G1_INT_I of register PMC_OTN_MLD_REG_HADAPT40G1_INT */
        otn_mld_reg_HADAPT40G1_INT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (OTN_MLD_REG_HADAPT40G1_INT_BIT_HADAPT40G1_INT_I_OFF + subfield_offset),
                                                              OTN_MLD_REG_HADAPT40G1_INT_BIT_HADAPT40G1_INT_I_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT40G1_INT_I_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G1_INT_I_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G1_INT_I_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G1_INT_I_get", stop_bit, 31 );
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
    /* (0x0001035c bits 31:0) bits 0:31 use field HADAPT40G1_INT_I of register PMC_OTN_MLD_REG_HADAPT40G1_INT */
    reg_value = otn_mld_reg_HADAPT40G1_INT_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT40G1_INT_BIT_HADAPT40G1_INT_I_MSK)
                  >> OTN_MLD_REG_HADAPT40G1_INT_BIT_HADAPT40G1_INT_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT40G1_INT_BIT_HADAPT40G1_INT_I_MSK, OTN_MLD_REG_HADAPT40G1_INT_BIT_HADAPT40G1_INT_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G1_INT_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT40G1_INT_I_poll( otn_mld_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT40G1_INT_I_poll( otn_mld_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G1_INT_I_poll", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G1_INT_I_poll", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G1_INT_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001035c bits 31:0) bits 0:31 use field HADAPT40G1_INT_I of register PMC_OTN_MLD_REG_HADAPT40G1_INT */
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
        /* (0x0001035c bits 31:0) bits 0:31 use field HADAPT40G1_INT_I of register PMC_OTN_MLD_REG_HADAPT40G1_INT */
        return otn_mld_reg_HADAPT40G1_INT_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT40G1_INT_BIT_HADAPT40G1_INT_I_OFF + subfield_offset),
                                                value << (OTN_MLD_REG_HADAPT40G1_INT_BIT_HADAPT40G1_INT_I_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT40G1_INT_I_poll( otn_mld_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT40G1_INT_I_poll( otn_mld_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otn_mld_field_HADAPT40G1_INT_I_poll", value );

    /* (0x0001035c bits 31:0) bits 0:31 use field HADAPT40G1_INT_I of register PMC_OTN_MLD_REG_HADAPT40G1_INT */
    return otn_mld_reg_HADAPT40G1_INT_poll( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT40G1_INT_BIT_HADAPT40G1_INT_I_MSK,
                                            (value<<OTN_MLD_REG_HADAPT40G1_INT_BIT_HADAPT40G1_INT_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void otn_mld_field_HADAPT100G_RXCLK2_INT_I_set_to_clear( otn_mld_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_RXCLK2_INT_I_set_to_clear( otn_mld_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_HADAPT100G_RXCLK2_INT_I_set_to_clear", value, 255);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_RXCLK2_INT_I_set_to_clear", value );

    /* (0x000103b0 bits 15:8) bits 0:7 use field HADAPT100G_RXCLK2_INT_I of register PMC_OTN_MLD_REG_HADAPT100G_INT */
    otn_mld_reg_HADAPT100G_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK2_INT_I_MSK,
                                                          OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK2_INT_I_OFF,
                                                          value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_RXCLK2_INT_I_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_RXCLK2_INT_I_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000103b0 bits 15:8) bits 0:7 use field HADAPT100G_RXCLK2_INT_I of register PMC_OTN_MLD_REG_HADAPT100G_INT */
    reg_value = otn_mld_reg_HADAPT100G_INT_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK2_INT_I_MSK) >> OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK2_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_RXCLK2_INT_I_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_RXCLK2_INT_I_set_to_clear( otn_mld_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_RXCLK2_INT_I_set_to_clear( otn_mld_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_I_set_to_clear", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000103b0 bits 15:8) bits 0:7 use field HADAPT100G_RXCLK2_INT_I of register PMC_OTN_MLD_REG_HADAPT100G_INT */
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
        /* (0x000103b0 bits 15:8) bits 0:7 use field HADAPT100G_RXCLK2_INT_I of register PMC_OTN_MLD_REG_HADAPT100G_INT */
        otn_mld_reg_HADAPT100G_INT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK2_INT_I_OFF + subfield_offset),
                                                              OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK2_INT_I_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_RXCLK2_INT_I_get( otn_mld_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_RXCLK2_INT_I_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_I_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_I_get", stop_bit, 7 );
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
    /* (0x000103b0 bits 15:8) bits 0:7 use field HADAPT100G_RXCLK2_INT_I of register PMC_OTN_MLD_REG_HADAPT100G_INT */
    reg_value = otn_mld_reg_HADAPT100G_INT_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK2_INT_I_MSK)
                  >> OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK2_INT_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK2_INT_I_MSK, OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK2_INT_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT100G_RXCLK2_INT_I_poll( otn_mld_buffer_t *b_ptr,
                                                                                     lineotn_handle_t *h_ptr,
                                                                                     UINT32 start_bit,
                                                                                     UINT32 stop_bit,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT100G_RXCLK2_INT_I_poll( otn_mld_buffer_t *b_ptr,
                                                                                     lineotn_handle_t *h_ptr,
                                                                                     UINT32 start_bit,
                                                                                     UINT32 stop_bit,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_I_poll", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_I_poll", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000103b0 bits 15:8) bits 0:7 use field HADAPT100G_RXCLK2_INT_I of register PMC_OTN_MLD_REG_HADAPT100G_INT */
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
        if (stop_bit < 7) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 7;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000103b0 bits 15:8) bits 0:7 use field HADAPT100G_RXCLK2_INT_I of register PMC_OTN_MLD_REG_HADAPT100G_INT */
        return otn_mld_reg_HADAPT100G_INT_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK2_INT_I_OFF + subfield_offset),
                                                value << (OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK2_INT_I_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT100G_RXCLK2_INT_I_poll( otn_mld_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT100G_RXCLK2_INT_I_poll( otn_mld_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otn_mld_field_HADAPT100G_RXCLK2_INT_I_poll", value );

    /* (0x000103b0 bits 15:8) bits 0:7 use field HADAPT100G_RXCLK2_INT_I of register PMC_OTN_MLD_REG_HADAPT100G_INT */
    return otn_mld_reg_HADAPT100G_INT_poll( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK2_INT_I_MSK,
                                            (value<<OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK2_INT_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void otn_mld_field_HADAPT100G_RXCLK1_INT_I_set_to_clear( otn_mld_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_HADAPT100G_RXCLK1_INT_I_set_to_clear( otn_mld_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "otn_mld_field_HADAPT100G_RXCLK1_INT_I_set_to_clear", value, 255);
    IOLOG( "%s <= 0x%08x", "otn_mld_field_HADAPT100G_RXCLK1_INT_I_set_to_clear", value );

    /* (0x000103b0 bits 7:0) bits 0:7 use field HADAPT100G_RXCLK1_INT_I of register PMC_OTN_MLD_REG_HADAPT100G_INT */
    otn_mld_reg_HADAPT100G_INT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK1_INT_I_MSK,
                                                          OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK1_INT_I_OFF,
                                                          value);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_RXCLK1_INT_I_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_RXCLK1_INT_I_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000103b0 bits 7:0) bits 0:7 use field HADAPT100G_RXCLK1_INT_I of register PMC_OTN_MLD_REG_HADAPT100G_INT */
    reg_value = otn_mld_reg_HADAPT100G_INT_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK1_INT_I_MSK) >> OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK1_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_RXCLK1_INT_I_get", value );

    return value;
}
static INLINE void otn_mld_field_range_HADAPT100G_RXCLK1_INT_I_set_to_clear( otn_mld_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void otn_mld_field_range_HADAPT100G_RXCLK1_INT_I_set_to_clear( otn_mld_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_I_set_to_clear", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000103b0 bits 7:0) bits 0:7 use field HADAPT100G_RXCLK1_INT_I of register PMC_OTN_MLD_REG_HADAPT100G_INT */
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
        /* (0x000103b0 bits 7:0) bits 0:7 use field HADAPT100G_RXCLK1_INT_I of register PMC_OTN_MLD_REG_HADAPT100G_INT */
        otn_mld_reg_HADAPT100G_INT_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK1_INT_I_OFF + subfield_offset),
                                                              OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK1_INT_I_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 otn_mld_field_range_HADAPT100G_RXCLK1_INT_I_get( otn_mld_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_RXCLK1_INT_I_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_I_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_I_get", stop_bit, 7 );
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
    /* (0x000103b0 bits 7:0) bits 0:7 use field HADAPT100G_RXCLK1_INT_I of register PMC_OTN_MLD_REG_HADAPT100G_INT */
    reg_value = otn_mld_reg_HADAPT100G_INT_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK1_INT_I_MSK)
                  >> OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK1_INT_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK1_INT_I_MSK, OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK1_INT_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT100G_RXCLK1_INT_I_poll( otn_mld_buffer_t *b_ptr,
                                                                                     lineotn_handle_t *h_ptr,
                                                                                     UINT32 start_bit,
                                                                                     UINT32 stop_bit,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT100G_RXCLK1_INT_I_poll( otn_mld_buffer_t *b_ptr,
                                                                                     lineotn_handle_t *h_ptr,
                                                                                     UINT32 start_bit,
                                                                                     UINT32 stop_bit,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_I_poll", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_I_poll", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000103b0 bits 7:0) bits 0:7 use field HADAPT100G_RXCLK1_INT_I of register PMC_OTN_MLD_REG_HADAPT100G_INT */
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
        if (stop_bit < 7) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 7;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000103b0 bits 7:0) bits 0:7 use field HADAPT100G_RXCLK1_INT_I of register PMC_OTN_MLD_REG_HADAPT100G_INT */
        return otn_mld_reg_HADAPT100G_INT_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK1_INT_I_OFF + subfield_offset),
                                                value << (OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK1_INT_I_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT100G_RXCLK1_INT_I_poll( otn_mld_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT100G_RXCLK1_INT_I_poll( otn_mld_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otn_mld_field_HADAPT100G_RXCLK1_INT_I_poll", value );

    /* (0x000103b0 bits 7:0) bits 0:7 use field HADAPT100G_RXCLK1_INT_I of register PMC_OTN_MLD_REG_HADAPT100G_INT */
    return otn_mld_reg_HADAPT100G_INT_poll( b_ptr,
                                            h_ptr,
                                            OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK1_INT_I_MSK,
                                            (value<<OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK1_INT_I_OFF),
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
static INLINE UINT32 otn_mld_field_HADAPT40G0_INT_V_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G0_INT_V_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010338 bits 31:0) bits 0:31 use field HADAPT40G0_INT_V of register PMC_OTN_MLD_REG_HADAPT40G0_INT_VAL */
    reg_value = otn_mld_reg_HADAPT40G0_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G0_INT_VAL_BIT_HADAPT40G0_INT_V_MSK) >> OTN_MLD_REG_HADAPT40G0_INT_VAL_BIT_HADAPT40G0_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G0_INT_V_get", value );

    return value;
}
static INLINE UINT32 otn_mld_field_range_HADAPT40G0_INT_V_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G0_INT_V_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G0_INT_V_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G0_INT_V_get", stop_bit, 31 );
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
    /* (0x00010338 bits 31:0) bits 0:31 use field HADAPT40G0_INT_V of register PMC_OTN_MLD_REG_HADAPT40G0_INT_VAL */
    reg_value = otn_mld_reg_HADAPT40G0_INT_VAL_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT40G0_INT_VAL_BIT_HADAPT40G0_INT_V_MSK)
                  >> OTN_MLD_REG_HADAPT40G0_INT_VAL_BIT_HADAPT40G0_INT_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT40G0_INT_VAL_BIT_HADAPT40G0_INT_V_MSK, OTN_MLD_REG_HADAPT40G0_INT_VAL_BIT_HADAPT40G0_INT_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G0_INT_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT40G0_INT_V_poll( otn_mld_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT40G0_INT_V_poll( otn_mld_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G0_INT_V_poll", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G0_INT_V_poll", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G0_INT_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010338 bits 31:0) bits 0:31 use field HADAPT40G0_INT_V of register PMC_OTN_MLD_REG_HADAPT40G0_INT_VAL */
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
        /* (0x00010338 bits 31:0) bits 0:31 use field HADAPT40G0_INT_V of register PMC_OTN_MLD_REG_HADAPT40G0_INT_VAL */
        return otn_mld_reg_HADAPT40G0_INT_VAL_poll( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (OTN_MLD_REG_HADAPT40G0_INT_VAL_BIT_HADAPT40G0_INT_V_OFF + subfield_offset),
                                                    value << (OTN_MLD_REG_HADAPT40G0_INT_VAL_BIT_HADAPT40G0_INT_V_OFF + subfield_offset),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT40G0_INT_V_poll( otn_mld_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT40G0_INT_V_poll( otn_mld_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otn_mld_field_HADAPT40G0_INT_V_poll", value );

    /* (0x00010338 bits 31:0) bits 0:31 use field HADAPT40G0_INT_V of register PMC_OTN_MLD_REG_HADAPT40G0_INT_VAL */
    return otn_mld_reg_HADAPT40G0_INT_VAL_poll( b_ptr,
                                                h_ptr,
                                                OTN_MLD_REG_HADAPT40G0_INT_VAL_BIT_HADAPT40G0_INT_V_MSK,
                                                (value<<OTN_MLD_REG_HADAPT40G0_INT_VAL_BIT_HADAPT40G0_INT_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 otn_mld_field_HADAPT40G1_INT_V_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G1_INT_V_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010360 bits 31:0) bits 0:31 use field HADAPT40G1_INT_V of register PMC_OTN_MLD_REG_HADAPT40G1_INT_VAL */
    reg_value = otn_mld_reg_HADAPT40G1_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G1_INT_VAL_BIT_HADAPT40G1_INT_V_MSK) >> OTN_MLD_REG_HADAPT40G1_INT_VAL_BIT_HADAPT40G1_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G1_INT_V_get", value );

    return value;
}
static INLINE UINT32 otn_mld_field_range_HADAPT40G1_INT_V_get( otn_mld_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G1_INT_V_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G1_INT_V_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G1_INT_V_get", stop_bit, 31 );
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
    /* (0x00010360 bits 31:0) bits 0:31 use field HADAPT40G1_INT_V of register PMC_OTN_MLD_REG_HADAPT40G1_INT_VAL */
    reg_value = otn_mld_reg_HADAPT40G1_INT_VAL_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT40G1_INT_VAL_BIT_HADAPT40G1_INT_V_MSK)
                  >> OTN_MLD_REG_HADAPT40G1_INT_VAL_BIT_HADAPT40G1_INT_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT40G1_INT_VAL_BIT_HADAPT40G1_INT_V_MSK, OTN_MLD_REG_HADAPT40G1_INT_VAL_BIT_HADAPT40G1_INT_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G1_INT_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT40G1_INT_V_poll( otn_mld_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT40G1_INT_V_poll( otn_mld_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G1_INT_V_poll", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G1_INT_V_poll", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G1_INT_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00010360 bits 31:0) bits 0:31 use field HADAPT40G1_INT_V of register PMC_OTN_MLD_REG_HADAPT40G1_INT_VAL */
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
        /* (0x00010360 bits 31:0) bits 0:31 use field HADAPT40G1_INT_V of register PMC_OTN_MLD_REG_HADAPT40G1_INT_VAL */
        return otn_mld_reg_HADAPT40G1_INT_VAL_poll( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (OTN_MLD_REG_HADAPT40G1_INT_VAL_BIT_HADAPT40G1_INT_V_OFF + subfield_offset),
                                                    value << (OTN_MLD_REG_HADAPT40G1_INT_VAL_BIT_HADAPT40G1_INT_V_OFF + subfield_offset),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT40G1_INT_V_poll( otn_mld_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT40G1_INT_V_poll( otn_mld_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otn_mld_field_HADAPT40G1_INT_V_poll", value );

    /* (0x00010360 bits 31:0) bits 0:31 use field HADAPT40G1_INT_V of register PMC_OTN_MLD_REG_HADAPT40G1_INT_VAL */
    return otn_mld_reg_HADAPT40G1_INT_VAL_poll( b_ptr,
                                                h_ptr,
                                                OTN_MLD_REG_HADAPT40G1_INT_VAL_BIT_HADAPT40G1_INT_V_MSK,
                                                (value<<OTN_MLD_REG_HADAPT40G1_INT_VAL_BIT_HADAPT40G1_INT_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_RXCLK2_INT_V_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_RXCLK2_INT_V_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000103b4 bits 15:8) bits 0:7 use field HADAPT100G_RXCLK2_INT_V of register PMC_OTN_MLD_REG_HADAPT100G_INT_VAL */
    reg_value = otn_mld_reg_HADAPT100G_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK2_INT_V_MSK) >> OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK2_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_RXCLK2_INT_V_get", value );

    return value;
}
static INLINE UINT32 otn_mld_field_range_HADAPT100G_RXCLK2_INT_V_get( otn_mld_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_RXCLK2_INT_V_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_V_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_V_get", stop_bit, 7 );
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
    /* (0x000103b4 bits 15:8) bits 0:7 use field HADAPT100G_RXCLK2_INT_V of register PMC_OTN_MLD_REG_HADAPT100G_INT_VAL */
    reg_value = otn_mld_reg_HADAPT100G_INT_VAL_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK2_INT_V_MSK)
                  >> OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK2_INT_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK2_INT_V_MSK, OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK2_INT_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT100G_RXCLK2_INT_V_poll( otn_mld_buffer_t *b_ptr,
                                                                                     lineotn_handle_t *h_ptr,
                                                                                     UINT32 start_bit,
                                                                                     UINT32 stop_bit,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT100G_RXCLK2_INT_V_poll( otn_mld_buffer_t *b_ptr,
                                                                                     lineotn_handle_t *h_ptr,
                                                                                     UINT32 start_bit,
                                                                                     UINT32 stop_bit,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_V_poll", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_V_poll", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_RXCLK2_INT_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000103b4 bits 15:8) bits 0:7 use field HADAPT100G_RXCLK2_INT_V of register PMC_OTN_MLD_REG_HADAPT100G_INT_VAL */
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
        if (stop_bit < 7) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 7;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000103b4 bits 15:8) bits 0:7 use field HADAPT100G_RXCLK2_INT_V of register PMC_OTN_MLD_REG_HADAPT100G_INT_VAL */
        return otn_mld_reg_HADAPT100G_INT_VAL_poll( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK2_INT_V_OFF + subfield_offset),
                                                    value << (OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK2_INT_V_OFF + subfield_offset),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT100G_RXCLK2_INT_V_poll( otn_mld_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT100G_RXCLK2_INT_V_poll( otn_mld_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otn_mld_field_HADAPT100G_RXCLK2_INT_V_poll", value );

    /* (0x000103b4 bits 15:8) bits 0:7 use field HADAPT100G_RXCLK2_INT_V of register PMC_OTN_MLD_REG_HADAPT100G_INT_VAL */
    return otn_mld_reg_HADAPT100G_INT_VAL_poll( b_ptr,
                                                h_ptr,
                                                OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK2_INT_V_MSK,
                                                (value<<OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK2_INT_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 otn_mld_field_HADAPT100G_RXCLK1_INT_V_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_RXCLK1_INT_V_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000103b4 bits 7:0) bits 0:7 use field HADAPT100G_RXCLK1_INT_V of register PMC_OTN_MLD_REG_HADAPT100G_INT_VAL */
    reg_value = otn_mld_reg_HADAPT100G_INT_VAL_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK1_INT_V_MSK) >> OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK1_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_RXCLK1_INT_V_get", value );

    return value;
}
static INLINE UINT32 otn_mld_field_range_HADAPT100G_RXCLK1_INT_V_get( otn_mld_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_RXCLK1_INT_V_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_V_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_V_get", stop_bit, 7 );
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
    /* (0x000103b4 bits 7:0) bits 0:7 use field HADAPT100G_RXCLK1_INT_V of register PMC_OTN_MLD_REG_HADAPT100G_INT_VAL */
    reg_value = otn_mld_reg_HADAPT100G_INT_VAL_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK1_INT_V_MSK)
                  >> OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK1_INT_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK1_INT_V_MSK, OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK1_INT_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT100G_RXCLK1_INT_V_poll( otn_mld_buffer_t *b_ptr,
                                                                                     lineotn_handle_t *h_ptr,
                                                                                     UINT32 start_bit,
                                                                                     UINT32 stop_bit,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT100G_RXCLK1_INT_V_poll( otn_mld_buffer_t *b_ptr,
                                                                                     lineotn_handle_t *h_ptr,
                                                                                     UINT32 start_bit,
                                                                                     UINT32 stop_bit,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_V_poll", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_V_poll", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_RXCLK1_INT_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000103b4 bits 7:0) bits 0:7 use field HADAPT100G_RXCLK1_INT_V of register PMC_OTN_MLD_REG_HADAPT100G_INT_VAL */
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
        if (stop_bit < 7) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 7;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000103b4 bits 7:0) bits 0:7 use field HADAPT100G_RXCLK1_INT_V of register PMC_OTN_MLD_REG_HADAPT100G_INT_VAL */
        return otn_mld_reg_HADAPT100G_INT_VAL_poll( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK1_INT_V_OFF + subfield_offset),
                                                    value << (OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK1_INT_V_OFF + subfield_offset),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT100G_RXCLK1_INT_V_poll( otn_mld_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT100G_RXCLK1_INT_V_poll( otn_mld_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "otn_mld_field_HADAPT100G_RXCLK1_INT_V_poll", value );

    /* (0x000103b4 bits 7:0) bits 0:7 use field HADAPT100G_RXCLK1_INT_V of register PMC_OTN_MLD_REG_HADAPT100G_INT_VAL */
    return otn_mld_reg_HADAPT100G_INT_VAL_poll( b_ptr,
                                                h_ptr,
                                                OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK1_INT_V_MSK,
                                                (value<<OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK1_INT_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 otn_mld_field_INT_SUMMARY_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_INT_SUMMARY_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001010c bits 10:0) bits 0:10 use field INT_SUMMARY of register PMC_OTN_MLD_REG_INT_SUMMARY */
    reg_value = otn_mld_reg_INT_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_INT_SUMMARY_BIT_INT_SUMMARY_MSK) >> OTN_MLD_REG_INT_SUMMARY_BIT_INT_SUMMARY_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_INT_SUMMARY_get", value );

    return value;
}
static INLINE UINT32 otn_mld_field_range_INT_SUMMARY_get( otn_mld_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_INT_SUMMARY_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_INT_SUMMARY_get", stop_bit, start_bit );
    if (stop_bit > 10) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_INT_SUMMARY_get", stop_bit, 10 );
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
    /* (0x0001010c bits 10:0) bits 0:10 use field INT_SUMMARY of register PMC_OTN_MLD_REG_INT_SUMMARY */
    reg_value = otn_mld_reg_INT_SUMMARY_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_INT_SUMMARY_BIT_INT_SUMMARY_MSK)
                  >> OTN_MLD_REG_INT_SUMMARY_BIT_INT_SUMMARY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_INT_SUMMARY_BIT_INT_SUMMARY_MSK, OTN_MLD_REG_INT_SUMMARY_BIT_INT_SUMMARY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_INT_SUMMARY_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 otn_mld_field_HADAPT40G0_STATUS_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G0_STATUS_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001033c bits 31:0) bits 0:31 use field HADAPT40G0_STATUS of register PMC_OTN_MLD_REG_HADAPT40G0_STAT */
    reg_value = otn_mld_reg_HADAPT40G0_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G0_STAT_BIT_HADAPT40G0_STATUS_MSK) >> OTN_MLD_REG_HADAPT40G0_STAT_BIT_HADAPT40G0_STATUS_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G0_STATUS_get", value );

    return value;
}
static INLINE UINT32 otn_mld_field_range_HADAPT40G0_STATUS_get( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G0_STATUS_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G0_STATUS_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G0_STATUS_get", stop_bit, 31 );
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
    /* (0x0001033c bits 31:0) bits 0:31 use field HADAPT40G0_STATUS of register PMC_OTN_MLD_REG_HADAPT40G0_STAT */
    reg_value = otn_mld_reg_HADAPT40G0_STAT_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT40G0_STAT_BIT_HADAPT40G0_STATUS_MSK)
                  >> OTN_MLD_REG_HADAPT40G0_STAT_BIT_HADAPT40G0_STATUS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT40G0_STAT_BIT_HADAPT40G0_STATUS_MSK, OTN_MLD_REG_HADAPT40G0_STAT_BIT_HADAPT40G0_STATUS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G0_STATUS_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 otn_mld_field_HADAPT40G1_STATUS_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT40G1_STATUS_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00010364 bits 31:0) bits 0:31 use field HADAPT40G1_STATUS of register PMC_OTN_MLD_REG_HADAPT40G1_STAT */
    reg_value = otn_mld_reg_HADAPT40G1_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT40G1_STAT_BIT_HADAPT40G1_STATUS_MSK) >> OTN_MLD_REG_HADAPT40G1_STAT_BIT_HADAPT40G1_STATUS_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT40G1_STATUS_get", value );

    return value;
}
static INLINE UINT32 otn_mld_field_range_HADAPT40G1_STATUS_get( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT40G1_STATUS_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT40G1_STATUS_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT40G1_STATUS_get", stop_bit, 31 );
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
    /* (0x00010364 bits 31:0) bits 0:31 use field HADAPT40G1_STATUS of register PMC_OTN_MLD_REG_HADAPT40G1_STAT */
    reg_value = otn_mld_reg_HADAPT40G1_STAT_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT40G1_STAT_BIT_HADAPT40G1_STATUS_MSK)
                  >> OTN_MLD_REG_HADAPT40G1_STAT_BIT_HADAPT40G1_STATUS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT40G1_STAT_BIT_HADAPT40G1_STATUS_MSK, OTN_MLD_REG_HADAPT40G1_STAT_BIT_HADAPT40G1_STATUS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT40G1_STATUS_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 otn_mld_field_HADAPT100G_STATUS_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_HADAPT100G_STATUS_get( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000103b8 bits 15:0) bits 0:15 use field HADAPT100G_STATUS of register PMC_OTN_MLD_REG_HADAPT100G_STAT */
    reg_value = otn_mld_reg_HADAPT100G_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & OTN_MLD_REG_HADAPT100G_STAT_BIT_HADAPT100G_STATUS_MSK) >> OTN_MLD_REG_HADAPT100G_STAT_BIT_HADAPT100G_STATUS_OFF;
    IOLOG( "%s -> 0x%08x", "otn_mld_field_HADAPT100G_STATUS_get", value );

    return value;
}
static INLINE UINT32 otn_mld_field_range_HADAPT100G_STATUS_get( otn_mld_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 otn_mld_field_range_HADAPT100G_STATUS_get( otn_mld_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "otn_mld_field_range_HADAPT100G_STATUS_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "otn_mld_field_range_HADAPT100G_STATUS_get", stop_bit, 15 );
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
    /* (0x000103b8 bits 15:0) bits 0:15 use field HADAPT100G_STATUS of register PMC_OTN_MLD_REG_HADAPT100G_STAT */
    reg_value = otn_mld_reg_HADAPT100G_STAT_read(  b_ptr, h_ptr);
    field_value = (reg_value & OTN_MLD_REG_HADAPT100G_STAT_BIT_HADAPT100G_STATUS_MSK)
                  >> OTN_MLD_REG_HADAPT100G_STAT_BIT_HADAPT100G_STATUS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OTN_MLD_REG_HADAPT100G_STAT_BIT_HADAPT100G_STATUS_MSK, OTN_MLD_REG_HADAPT100G_STAT_BIT_HADAPT100G_STATUS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "otn_mld_field_range_HADAPT100G_STATUS_get", start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OTN_MLD_IO_INLINE_H */
