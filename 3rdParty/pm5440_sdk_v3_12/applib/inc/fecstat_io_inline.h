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
 *     and register accessor functions for the fecstat block
 *****************************************************************************/
#ifndef _FECSTAT_IO_INLINE_H
#define _FECSTAT_IO_INLINE_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "fecstat_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define FECSTAT_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for fecstat
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
} fecstat_buffer_t;
static INLINE void fecstat_buffer_init( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void fecstat_buffer_init( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "fecstat_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void fecstat_buffer_flush( fecstat_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void fecstat_buffer_flush( fecstat_buffer_t *b_ptr )
{
    IOLOG( "fecstat_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 fecstat_reg_read( fecstat_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 fecstat_reg_read( fecstat_buffer_t *b_ptr,
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
static INLINE void fecstat_reg_write( fecstat_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_write( fecstat_buffer_t *b_ptr,
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

static INLINE void fecstat_field_set( fecstat_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 mask,
                                      UINT32 unused_mask,
                                      UINT32 ofs,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_set( fecstat_buffer_t *b_ptr,
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

static INLINE void fecstat_action_on_write_field_set( fecstat_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_action_on_write_field_set( fecstat_buffer_t *b_ptr,
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

static INLINE void fecstat_burst_read( fecstat_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void fecstat_burst_read( fecstat_buffer_t *b_ptr,
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

static INLINE void fecstat_burst_write( fecstat_buffer_t *b_ptr,
                                        lineotn_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void fecstat_burst_write( fecstat_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE fecstat_poll( fecstat_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 mask,
                                                 UINT32 value,
                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                 UINT32 max_count,
                                                 UINT32 *num_failed_polls,
                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fecstat_poll( fecstat_buffer_t *b_ptr,
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
 *  register access functions for fecstat
 * ==================================================================================
 */

static INLINE void fecstat_reg_FECSTAT_EN_write( fecstat_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_FECSTAT_EN_write( fecstat_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fecstat_reg_FECSTAT_EN_write", value );
    fecstat_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_FECSTAT_EN,
                       value);
}

static INLINE void fecstat_reg_FECSTAT_EN_field_set( fecstat_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_FECSTAT_EN_field_set( fecstat_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "fecstat_reg_FECSTAT_EN_field_set", mask, ofs, value );
    fecstat_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_FECSTAT_EN,
                       mask,
                       PMC_FECSTAT_REG_FECSTAT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fecstat_reg_FECSTAT_EN_read( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_reg_FECSTAT_EN_read( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = fecstat_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_FECSTAT_REG_FECSTAT_EN);

    IOLOG( "%s -> 0x%08x;", "fecstat_reg_FECSTAT_EN_read", reg_value);
    return reg_value;
}

static INLINE void fecstat_reg_FWI_CLK_DIVIDER_write( fecstat_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_FWI_CLK_DIVIDER_write( fecstat_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fecstat_reg_FWI_CLK_DIVIDER_write", value );
    fecstat_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_FWI_CLK_DIVIDER,
                       value);
}

static INLINE void fecstat_reg_FWI_CLK_DIVIDER_field_set( fecstat_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_FWI_CLK_DIVIDER_field_set( fecstat_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "fecstat_reg_FWI_CLK_DIVIDER_field_set", mask, ofs, value );
    fecstat_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_FWI_CLK_DIVIDER,
                       mask,
                       PMC_FECSTAT_REG_FWI_CLK_DIVIDER_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fecstat_reg_FWI_CLK_DIVIDER_read( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_reg_FWI_CLK_DIVIDER_read( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = fecstat_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_FECSTAT_REG_FWI_CLK_DIVIDER);

    IOLOG( "%s -> 0x%08x;", "fecstat_reg_FWI_CLK_DIVIDER_read", reg_value);
    return reg_value;
}

static INLINE void fecstat_reg_FEC_COLLECTION_PERIOD_CNT_write( fecstat_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_FEC_COLLECTION_PERIOD_CNT_write( fecstat_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fecstat_reg_FEC_COLLECTION_PERIOD_CNT_write", value );
    fecstat_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT,
                       value);
}

static INLINE void fecstat_reg_FEC_COLLECTION_PERIOD_CNT_field_set( fecstat_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_FEC_COLLECTION_PERIOD_CNT_field_set( fecstat_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "fecstat_reg_FEC_COLLECTION_PERIOD_CNT_field_set", mask, ofs, value );
    fecstat_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT,
                       mask,
                       PMC_FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fecstat_reg_FEC_COLLECTION_PERIOD_CNT_read( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_reg_FEC_COLLECTION_PERIOD_CNT_read( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = fecstat_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT);

    IOLOG( "%s -> 0x%08x;", "fecstat_reg_FEC_COLLECTION_PERIOD_CNT_read", reg_value);
    return reg_value;
}

static INLINE void fecstat_reg_FEC_ROW_EN_LOWER_write( fecstat_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_FEC_ROW_EN_LOWER_write( fecstat_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fecstat_reg_FEC_ROW_EN_LOWER_write", value );
    fecstat_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_FEC_ROW_EN_LOWER,
                       value);
}

static INLINE void fecstat_reg_FEC_ROW_EN_LOWER_field_set( fecstat_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_FEC_ROW_EN_LOWER_field_set( fecstat_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "fecstat_reg_FEC_ROW_EN_LOWER_field_set", mask, ofs, value );
    fecstat_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_FEC_ROW_EN_LOWER,
                       mask,
                       PMC_FECSTAT_REG_FEC_ROW_EN_LOWER_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fecstat_reg_FEC_ROW_EN_LOWER_read( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_reg_FEC_ROW_EN_LOWER_read( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = fecstat_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_FECSTAT_REG_FEC_ROW_EN_LOWER);

    IOLOG( "%s -> 0x%08x;", "fecstat_reg_FEC_ROW_EN_LOWER_read", reg_value);
    return reg_value;
}

static INLINE void fecstat_reg_FEC_ROW_EN_UPPER_write( fecstat_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_FEC_ROW_EN_UPPER_write( fecstat_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fecstat_reg_FEC_ROW_EN_UPPER_write", value );
    fecstat_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_FEC_ROW_EN_UPPER,
                       value);
}

static INLINE void fecstat_reg_FEC_ROW_EN_UPPER_field_set( fecstat_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_FEC_ROW_EN_UPPER_field_set( fecstat_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "fecstat_reg_FEC_ROW_EN_UPPER_field_set", mask, ofs, value );
    fecstat_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_FEC_ROW_EN_UPPER,
                       mask,
                       PMC_FECSTAT_REG_FEC_ROW_EN_UPPER_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fecstat_reg_FEC_ROW_EN_UPPER_read( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_reg_FEC_ROW_EN_UPPER_read( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = fecstat_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_FECSTAT_REG_FEC_ROW_EN_UPPER);

    IOLOG( "%s -> 0x%08x;", "fecstat_reg_FEC_ROW_EN_UPPER_read", reg_value);
    return reg_value;
}

static INLINE void fecstat_reg_STAT_SOURCE_EN_array_write( fecstat_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_STAT_SOURCE_EN_array_write( fecstat_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fecstat_reg_STAT_SOURCE_EN_array_write", value );
    fecstat_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_STAT_SOURCE_EN(N),
                       value);
}

static INLINE void fecstat_reg_STAT_SOURCE_EN_array_field_set( fecstat_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_STAT_SOURCE_EN_array_field_set( fecstat_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "fecstat_reg_STAT_SOURCE_EN_array_field_set", N, mask, ofs, value );
    fecstat_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_STAT_SOURCE_EN(N),
                       mask,
                       PMC_FECSTAT_REG_STAT_SOURCE_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fecstat_reg_STAT_SOURCE_EN_array_read( fecstat_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_reg_STAT_SOURCE_EN_array_read( fecstat_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = fecstat_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_FECSTAT_REG_STAT_SOURCE_EN(N));

    IOLOG( "%s -> 0x%08x; N=%d", "fecstat_reg_STAT_SOURCE_EN_array_read", reg_value, N);
    return reg_value;
}

static INLINE void fecstat_reg_STAT_SOURCE_0_ADR_array_write( fecstat_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_STAT_SOURCE_0_ADR_array_write( fecstat_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fecstat_reg_STAT_SOURCE_0_ADR_array_write", value );
    fecstat_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_STAT_SOURCE_0_ADR(N),
                       value);
}

static INLINE void fecstat_reg_STAT_SOURCE_0_ADR_array_field_set( fecstat_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_STAT_SOURCE_0_ADR_array_field_set( fecstat_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "fecstat_reg_STAT_SOURCE_0_ADR_array_field_set", N, mask, ofs, value );
    fecstat_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_STAT_SOURCE_0_ADR(N),
                       mask,
                       PMC_FECSTAT_REG_STAT_SOURCE_0_ADR_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fecstat_reg_STAT_SOURCE_0_ADR_array_read( fecstat_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_reg_STAT_SOURCE_0_ADR_array_read( fecstat_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = fecstat_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_FECSTAT_REG_STAT_SOURCE_0_ADR(N));

    IOLOG( "%s -> 0x%08x; N=%d", "fecstat_reg_STAT_SOURCE_0_ADR_array_read", reg_value, N);
    return reg_value;
}

static INLINE void fecstat_reg_STAT_SOURCE_1_ADR_array_write( fecstat_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_STAT_SOURCE_1_ADR_array_write( fecstat_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fecstat_reg_STAT_SOURCE_1_ADR_array_write", value );
    fecstat_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_STAT_SOURCE_1_ADR(N),
                       value);
}

static INLINE void fecstat_reg_STAT_SOURCE_1_ADR_array_field_set( fecstat_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_STAT_SOURCE_1_ADR_array_field_set( fecstat_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "fecstat_reg_STAT_SOURCE_1_ADR_array_field_set", N, mask, ofs, value );
    fecstat_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_STAT_SOURCE_1_ADR(N),
                       mask,
                       PMC_FECSTAT_REG_STAT_SOURCE_1_ADR_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fecstat_reg_STAT_SOURCE_1_ADR_array_read( fecstat_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_reg_STAT_SOURCE_1_ADR_array_read( fecstat_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = fecstat_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_FECSTAT_REG_STAT_SOURCE_1_ADR(N));

    IOLOG( "%s -> 0x%08x; N=%d", "fecstat_reg_STAT_SOURCE_1_ADR_array_read", reg_value, N);
    return reg_value;
}

static INLINE void fecstat_reg_STAT_SOURCE_2_ADR_array_write( fecstat_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_STAT_SOURCE_2_ADR_array_write( fecstat_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fecstat_reg_STAT_SOURCE_2_ADR_array_write", value );
    fecstat_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_STAT_SOURCE_2_ADR(N),
                       value);
}

static INLINE void fecstat_reg_STAT_SOURCE_2_ADR_array_field_set( fecstat_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_STAT_SOURCE_2_ADR_array_field_set( fecstat_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "fecstat_reg_STAT_SOURCE_2_ADR_array_field_set", N, mask, ofs, value );
    fecstat_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_STAT_SOURCE_2_ADR(N),
                       mask,
                       PMC_FECSTAT_REG_STAT_SOURCE_2_ADR_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fecstat_reg_STAT_SOURCE_2_ADR_array_read( fecstat_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_reg_STAT_SOURCE_2_ADR_array_read( fecstat_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = fecstat_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_FECSTAT_REG_STAT_SOURCE_2_ADR(N));

    IOLOG( "%s -> 0x%08x; N=%d", "fecstat_reg_STAT_SOURCE_2_ADR_array_read", reg_value, N);
    return reg_value;
}

static INLINE void fecstat_reg_LOGICAL_LANE_ID_array_write( fecstat_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_LOGICAL_LANE_ID_array_write( fecstat_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fecstat_reg_LOGICAL_LANE_ID_array_write", value );
    fecstat_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_LOGICAL_LANE_ID(N),
                       value);
}

static INLINE void fecstat_reg_LOGICAL_LANE_ID_array_field_set( fecstat_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_LOGICAL_LANE_ID_array_field_set( fecstat_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "fecstat_reg_LOGICAL_LANE_ID_array_field_set", N, mask, ofs, value );
    fecstat_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_LOGICAL_LANE_ID(N),
                       mask,
                       PMC_FECSTAT_REG_LOGICAL_LANE_ID_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fecstat_reg_LOGICAL_LANE_ID_array_read( fecstat_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_reg_LOGICAL_LANE_ID_array_read( fecstat_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 reg_value;

    reg_value = fecstat_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_FECSTAT_REG_LOGICAL_LANE_ID(N));

    IOLOG( "%s -> 0x%08x; N=%d", "fecstat_reg_LOGICAL_LANE_ID_array_read", reg_value, N);
    return reg_value;
}

static INLINE void fecstat_reg_PHYSICAL_LANE_INDEX_array_write( fecstat_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_PHYSICAL_LANE_INDEX_array_write( fecstat_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fecstat_reg_PHYSICAL_LANE_INDEX_array_write", value );
    fecstat_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_PHYSICAL_LANE_INDEX(N),
                       value);
}

static INLINE void fecstat_reg_PHYSICAL_LANE_INDEX_array_field_set( fecstat_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_PHYSICAL_LANE_INDEX_array_field_set( fecstat_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "fecstat_reg_PHYSICAL_LANE_INDEX_array_field_set", N, mask, ofs, value );
    fecstat_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_FECSTAT_REG_PHYSICAL_LANE_INDEX(N),
                       mask,
                       PMC_FECSTAT_REG_PHYSICAL_LANE_INDEX_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fecstat_reg_PHYSICAL_LANE_INDEX_array_read( fecstat_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_reg_PHYSICAL_LANE_INDEX_array_read( fecstat_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 reg_value;

    reg_value = fecstat_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_FECSTAT_REG_PHYSICAL_LANE_INDEX(N));

    IOLOG( "%s -> 0x%08x; N=%d", "fecstat_reg_PHYSICAL_LANE_INDEX_array_read", reg_value, N);
    return reg_value;
}

static INLINE void fecstat_reg_AIR_ERR_INT_EN_write( fecstat_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_AIR_ERR_INT_EN_write( fecstat_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fecstat_reg_AIR_ERR_INT_EN_write", value );
    fecstat_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_FECSTAT_REG_AIR_ERR_INT_EN,
                       value);
}

static INLINE void fecstat_reg_AIR_ERR_INT_EN_field_set( fecstat_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_AIR_ERR_INT_EN_field_set( fecstat_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "fecstat_reg_AIR_ERR_INT_EN_field_set", mask, ofs, value );
    fecstat_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_FECSTAT_REG_AIR_ERR_INT_EN,
                       mask,
                       PMC_FECSTAT_REG_AIR_ERR_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 fecstat_reg_AIR_ERR_INT_EN_read( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_reg_AIR_ERR_INT_EN_read( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = fecstat_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_FECSTAT_REG_AIR_ERR_INT_EN);

    IOLOG( "%s -> 0x%08x;", "fecstat_reg_AIR_ERR_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void fecstat_reg_AIR_ERR_INT_REQUEST_write( fecstat_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_AIR_ERR_INT_REQUEST_write( fecstat_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fecstat_reg_AIR_ERR_INT_REQUEST_write", value );
    fecstat_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_FECSTAT_REG_AIR_ERR_INT_REQUEST,
                       value);
}

static INLINE void fecstat_reg_AIR_ERR_INT_REQUEST_action_on_write_field_set( fecstat_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_reg_AIR_ERR_INT_REQUEST_action_on_write_field_set( fecstat_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "fecstat_reg_AIR_ERR_INT_REQUEST_action_on_write_field_set", mask, ofs, value );
    fecstat_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       MEM_TYPE_STATUS,
                                       PMC_FECSTAT_REG_AIR_ERR_INT_REQUEST,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 fecstat_reg_AIR_ERR_INT_REQUEST_read( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_reg_AIR_ERR_INT_REQUEST_read( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = fecstat_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_FECSTAT_REG_AIR_ERR_INT_REQUEST);

    IOLOG( "%s -> 0x%08x;", "fecstat_reg_AIR_ERR_INT_REQUEST_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE fecstat_reg_AIR_ERR_INT_REQUEST_poll( fecstat_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fecstat_reg_AIR_ERR_INT_REQUEST_poll( fecstat_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "fecstat_reg_AIR_ERR_INT_REQUEST_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return fecstat_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_FECSTAT_REG_AIR_ERR_INT_REQUEST,
                         mask,
                         value,
                         cmp,
                         max_count,
                         num_failed_polls,
                         delay_between_polls_in_microseconds);

}


static INLINE UINT32 fecstat_reg_FEC_READ_ERR_VECTOR_read( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_reg_FEC_READ_ERR_VECTOR_read( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = fecstat_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_FECSTAT_REG_FEC_READ_ERR_VECTOR);

    IOLOG( "%s -> 0x%08x;", "fecstat_reg_FEC_READ_ERR_VECTOR_read", reg_value);
    return reg_value;
}

static INLINE UINT32 fecstat_reg_FWI_WRITE_ERR_VECTOR_read( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_reg_FWI_WRITE_ERR_VECTOR_read( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = fecstat_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_FECSTAT_REG_FWI_WRITE_ERR_VECTOR);

    IOLOG( "%s -> 0x%08x;", "fecstat_reg_FWI_WRITE_ERR_VECTOR_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void fecstat_field_FECSTAT_EN_set( fecstat_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_FECSTAT_EN_set( fecstat_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fecstat_field_FECSTAT_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "fecstat_field_FECSTAT_EN_set", value );

    /* (0x00006000 bits 0) field FECSTAT_EN of register PMC_FECSTAT_REG_FECSTAT_EN */
    fecstat_reg_FECSTAT_EN_field_set( b_ptr,
                                      h_ptr,
                                      FECSTAT_REG_FECSTAT_EN_BIT_FECSTAT_EN_MSK,
                                      FECSTAT_REG_FECSTAT_EN_BIT_FECSTAT_EN_OFF,
                                      value);
}

static INLINE UINT32 fecstat_field_FECSTAT_EN_get( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_FECSTAT_EN_get( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00006000 bits 0) field FECSTAT_EN of register PMC_FECSTAT_REG_FECSTAT_EN */
    reg_value = fecstat_reg_FECSTAT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & FECSTAT_REG_FECSTAT_EN_BIT_FECSTAT_EN_MSK) >> FECSTAT_REG_FECSTAT_EN_BIT_FECSTAT_EN_OFF;
    IOLOG( "%s -> 0x%08x", "fecstat_field_FECSTAT_EN_get", value );

    return value;
}
static INLINE void fecstat_field_FWI_INV_GL_CLK_set( fecstat_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_FWI_INV_GL_CLK_set( fecstat_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fecstat_field_FWI_INV_GL_CLK_set", value, 1);
    IOLOG( "%s <= 0x%08x", "fecstat_field_FWI_INV_GL_CLK_set", value );

    /* (0x00006604 bits 4) field FWI_INV_GL_CLK of register PMC_FECSTAT_REG_FWI_CLK_DIVIDER */
    fecstat_reg_FWI_CLK_DIVIDER_field_set( b_ptr,
                                           h_ptr,
                                           FECSTAT_REG_FWI_CLK_DIVIDER_BIT_FWI_INV_GL_CLK_MSK,
                                           FECSTAT_REG_FWI_CLK_DIVIDER_BIT_FWI_INV_GL_CLK_OFF,
                                           value);
}

static INLINE UINT32 fecstat_field_FWI_INV_GL_CLK_get( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_FWI_INV_GL_CLK_get( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00006604 bits 4) field FWI_INV_GL_CLK of register PMC_FECSTAT_REG_FWI_CLK_DIVIDER */
    reg_value = fecstat_reg_FWI_CLK_DIVIDER_read(  b_ptr, h_ptr);
    value = (reg_value & FECSTAT_REG_FWI_CLK_DIVIDER_BIT_FWI_INV_GL_CLK_MSK) >> FECSTAT_REG_FWI_CLK_DIVIDER_BIT_FWI_INV_GL_CLK_OFF;
    IOLOG( "%s -> 0x%08x", "fecstat_field_FWI_INV_GL_CLK_get", value );

    return value;
}
static INLINE void fecstat_field_FEC_COLLCT_CNT_set( fecstat_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_FEC_COLLCT_CNT_set( fecstat_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 268435455)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fecstat_field_FEC_COLLCT_CNT_set", value, 268435455);
    IOLOG( "%s <= 0x%08x", "fecstat_field_FEC_COLLCT_CNT_set", value );

    /* (0x00006608 bits 27:0) field FEC_COLLCT_CNT of register PMC_FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT */
    fecstat_reg_FEC_COLLECTION_PERIOD_CNT_field_set( b_ptr,
                                                     h_ptr,
                                                     FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT_BIT_FEC_COLLCT_CNT_MSK,
                                                     FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT_BIT_FEC_COLLCT_CNT_OFF,
                                                     value);
}

static INLINE UINT32 fecstat_field_FEC_COLLCT_CNT_get( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_FEC_COLLCT_CNT_get( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00006608 bits 27:0) field FEC_COLLCT_CNT of register PMC_FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT */
    reg_value = fecstat_reg_FEC_COLLECTION_PERIOD_CNT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT_BIT_FEC_COLLCT_CNT_MSK) >> FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT_BIT_FEC_COLLCT_CNT_OFF;
    IOLOG( "%s -> 0x%08x", "fecstat_field_FEC_COLLCT_CNT_get", value );

    return value;
}
static INLINE void fecstat_field_range_FEC_COLLCT_CNT_set( fecstat_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_range_FEC_COLLCT_CNT_set( fecstat_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_FEC_COLLCT_CNT_set", stop_bit, start_bit );
    if (stop_bit > 27) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_FEC_COLLCT_CNT_set", stop_bit, 27 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_FEC_COLLCT_CNT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00006608 bits 27:0) field FEC_COLLCT_CNT of register PMC_FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT */
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
        if (stop_bit < 27) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 27;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00006608 bits 27:0) field FEC_COLLCT_CNT of register PMC_FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT */
        fecstat_reg_FEC_COLLECTION_PERIOD_CNT_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT_BIT_FEC_COLLCT_CNT_OFF + subfield_offset),
                                                         FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT_BIT_FEC_COLLCT_CNT_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 fecstat_field_range_FEC_COLLCT_CNT_get( fecstat_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_range_FEC_COLLCT_CNT_get( fecstat_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_FEC_COLLCT_CNT_get", stop_bit, start_bit );
    if (stop_bit > 27) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_FEC_COLLCT_CNT_get", stop_bit, 27 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 27) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 27;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00006608 bits 27:0) field FEC_COLLCT_CNT of register PMC_FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT */
    reg_value = fecstat_reg_FEC_COLLECTION_PERIOD_CNT_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT_BIT_FEC_COLLCT_CNT_MSK)
                  >> FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT_BIT_FEC_COLLCT_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT_BIT_FEC_COLLCT_CNT_MSK, FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT_BIT_FEC_COLLCT_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_FEC_COLLCT_CNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void fecstat_lfield_FEC_ROW_EN_set( fecstat_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void fecstat_lfield_FEC_ROW_EN_set( fecstat_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value[2] )
{
    IOLOG( "%s <= 0x%08x 0x%08x", "fecstat_lfield_FEC_ROW_EN_set", value[1] , value[0] );

    /* (0x0000660c bits 31:0) bits 0:31 use field FEC_ROW_EN of register PMC_FECSTAT_REG_FEC_ROW_EN_LOWER */
    fecstat_reg_FEC_ROW_EN_LOWER_field_set( b_ptr,
                                            h_ptr,
                                            FECSTAT_REG_FEC_ROW_EN_LOWER_BIT_FEC_ROW_EN_MSK,
                                            FECSTAT_REG_FEC_ROW_EN_LOWER_BIT_FEC_ROW_EN_OFF,
                                            value[0]);

    /* (0x00006610 bits 15:0) bits 32:47 use field FEC_ROW_EN of register PMC_FECSTAT_REG_FEC_ROW_EN_UPPER */
    fecstat_reg_FEC_ROW_EN_UPPER_field_set( b_ptr,
                                            h_ptr,
                                            FECSTAT_REG_FEC_ROW_EN_UPPER_BIT_FEC_ROW_EN_MSK,
                                            FECSTAT_REG_FEC_ROW_EN_UPPER_BIT_FEC_ROW_EN_OFF,
                                            value[1]);
}

static INLINE void fecstat_lfield_FEC_ROW_EN_get( fecstat_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void fecstat_lfield_FEC_ROW_EN_get( fecstat_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x0000660c bits 31:0) bits 0:31 use field FEC_ROW_EN of register PMC_FECSTAT_REG_FEC_ROW_EN_LOWER */
    reg_value = fecstat_reg_FEC_ROW_EN_LOWER_read(  b_ptr, h_ptr);
    field_value = (reg_value & FECSTAT_REG_FEC_ROW_EN_LOWER_BIT_FEC_ROW_EN_MSK) >> FECSTAT_REG_FEC_ROW_EN_LOWER_BIT_FEC_ROW_EN_OFF;
    value[0] |= field_value;

    /* (0x00006610 bits 15:0) bits 32:47 use field FEC_ROW_EN of register PMC_FECSTAT_REG_FEC_ROW_EN_UPPER */
    reg_value = fecstat_reg_FEC_ROW_EN_UPPER_read(  b_ptr, h_ptr);
    field_value = (reg_value & FECSTAT_REG_FEC_ROW_EN_UPPER_BIT_FEC_ROW_EN_MSK) >> FECSTAT_REG_FEC_ROW_EN_UPPER_BIT_FEC_ROW_EN_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "fecstat_lfield_FEC_ROW_EN_get", value[1] , value[0] );

}
static INLINE void fecstat_lfield_range_FEC_ROW_EN_set( fecstat_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_lfield_range_FEC_ROW_EN_set( fecstat_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_lfield_range_FEC_ROW_EN_set", stop_bit, start_bit );
    if (stop_bit > 47) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_lfield_range_FEC_ROW_EN_set", stop_bit, 47 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "fecstat_lfield_range_FEC_ROW_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000660c bits 31:0) bits 0:31 use field FEC_ROW_EN of register PMC_FECSTAT_REG_FEC_ROW_EN_LOWER */
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
        /* (0x0000660c bits 31:0) bits 0:31 use field FEC_ROW_EN of register PMC_FECSTAT_REG_FEC_ROW_EN_LOWER */
        fecstat_reg_FEC_ROW_EN_LOWER_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (FECSTAT_REG_FEC_ROW_EN_LOWER_BIT_FEC_ROW_EN_OFF + subfield_offset),
                                                FECSTAT_REG_FEC_ROW_EN_LOWER_BIT_FEC_ROW_EN_OFF + subfield_offset,
                                                value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00006610 bits 15:0) bits 32:47 use field FEC_ROW_EN of register PMC_FECSTAT_REG_FEC_ROW_EN_UPPER */
    if ((start_bit <= 47) && (stop_bit >= 32)) {
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
        if (stop_bit < 47) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 47;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00006610 bits 15:0) bits 32:47 use field FEC_ROW_EN of register PMC_FECSTAT_REG_FEC_ROW_EN_UPPER */
        fecstat_reg_FEC_ROW_EN_UPPER_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (FECSTAT_REG_FEC_ROW_EN_UPPER_BIT_FEC_ROW_EN_OFF + subfield_offset),
                                                FECSTAT_REG_FEC_ROW_EN_UPPER_BIT_FEC_ROW_EN_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 fecstat_lfield_range_FEC_ROW_EN_get( fecstat_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_lfield_range_FEC_ROW_EN_get( fecstat_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_lfield_range_FEC_ROW_EN_get", stop_bit, start_bit );
    if (stop_bit > 47) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_lfield_range_FEC_ROW_EN_get", stop_bit, 47 );
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
    /* (0x0000660c bits 31:0) bits 0:31 use field FEC_ROW_EN of register PMC_FECSTAT_REG_FEC_ROW_EN_LOWER */
        reg_value = fecstat_reg_FEC_ROW_EN_LOWER_read(  b_ptr, h_ptr);
        field_value = (reg_value & FECSTAT_REG_FEC_ROW_EN_LOWER_BIT_FEC_ROW_EN_MSK)
                      >> FECSTAT_REG_FEC_ROW_EN_LOWER_BIT_FEC_ROW_EN_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FECSTAT_REG_FEC_ROW_EN_LOWER_BIT_FEC_ROW_EN_MSK, FECSTAT_REG_FEC_ROW_EN_LOWER_BIT_FEC_ROW_EN_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 47) && (stop_bit >= 32)) {
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
        if (stop_bit < 47) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 47;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00006610 bits 15:0) bits 32:47 use field FEC_ROW_EN of register PMC_FECSTAT_REG_FEC_ROW_EN_UPPER */
        reg_value = fecstat_reg_FEC_ROW_EN_UPPER_read(  b_ptr, h_ptr);
        field_value = (reg_value & FECSTAT_REG_FEC_ROW_EN_UPPER_BIT_FEC_ROW_EN_MSK)
                      >> FECSTAT_REG_FEC_ROW_EN_UPPER_BIT_FEC_ROW_EN_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FECSTAT_REG_FEC_ROW_EN_UPPER_BIT_FEC_ROW_EN_MSK, FECSTAT_REG_FEC_ROW_EN_UPPER_BIT_FEC_ROW_EN_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "fecstat_lfield_range_FEC_ROW_EN_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size48
 * ==================================================================================
 */
static INLINE void fecstat_field_STAT_SOURCE_EN_set( fecstat_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_STAT_SOURCE_EN_set( fecstat_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_STAT_SOURCE_EN_set", N, 47);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fecstat_field_STAT_SOURCE_EN_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "fecstat_field_STAT_SOURCE_EN_set", N, value );

    /* ((0x00006004 + (N) * 0x20) bits 2:0) field STAT_SOURCE_EN of register PMC_FECSTAT_REG_STAT_SOURCE_EN index N=0..47 */
    fecstat_reg_STAT_SOURCE_EN_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                FECSTAT_REG_STAT_SOURCE_EN_BIT_STAT_SOURCE_EN_MSK,
                                                FECSTAT_REG_STAT_SOURCE_EN_BIT_STAT_SOURCE_EN_OFF,
                                                value);
}

static INLINE UINT32 fecstat_field_STAT_SOURCE_EN_get( fecstat_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_STAT_SOURCE_EN_get( fecstat_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_STAT_SOURCE_EN_get", N, 47);
    /* ((0x00006004 + (N) * 0x20) bits 2:0) field STAT_SOURCE_EN of register PMC_FECSTAT_REG_STAT_SOURCE_EN index N=0..47 */
    reg_value = fecstat_reg_STAT_SOURCE_EN_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & FECSTAT_REG_STAT_SOURCE_EN_BIT_STAT_SOURCE_EN_MSK) >> FECSTAT_REG_STAT_SOURCE_EN_BIT_STAT_SOURCE_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "fecstat_field_STAT_SOURCE_EN_get", N, value );

    return value;
}
static INLINE void fecstat_field_range_STAT_SOURCE_EN_set( fecstat_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_range_STAT_SOURCE_EN_set( fecstat_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_range_STAT_SOURCE_EN_set", N, 47);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_STAT_SOURCE_EN_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_STAT_SOURCE_EN_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_STAT_SOURCE_EN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00006004 + (N) * 0x20) bits 2:0) field STAT_SOURCE_EN of register PMC_FECSTAT_REG_STAT_SOURCE_EN index N=0..47 */
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
        /* ((0x00006004 + (N) * 0x20) bits 2:0) field STAT_SOURCE_EN of register PMC_FECSTAT_REG_STAT_SOURCE_EN index N=0..47 */
        fecstat_reg_STAT_SOURCE_EN_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (FECSTAT_REG_STAT_SOURCE_EN_BIT_STAT_SOURCE_EN_OFF + subfield_offset),
                                                    FECSTAT_REG_STAT_SOURCE_EN_BIT_STAT_SOURCE_EN_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 fecstat_field_range_STAT_SOURCE_EN_get( fecstat_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_range_STAT_SOURCE_EN_get( fecstat_buffer_t *b_ptr,
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

    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_range_STAT_SOURCE_EN_get", N, 47);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_STAT_SOURCE_EN_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_STAT_SOURCE_EN_get", stop_bit, 2 );
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
    /* ((0x00006004 + (N) * 0x20) bits 2:0) field STAT_SOURCE_EN of register PMC_FECSTAT_REG_STAT_SOURCE_EN index N=0..47 */
    reg_value = fecstat_reg_STAT_SOURCE_EN_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & FECSTAT_REG_STAT_SOURCE_EN_BIT_STAT_SOURCE_EN_MSK)
                  >> FECSTAT_REG_STAT_SOURCE_EN_BIT_STAT_SOURCE_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FECSTAT_REG_STAT_SOURCE_EN_BIT_STAT_SOURCE_EN_MSK, FECSTAT_REG_STAT_SOURCE_EN_BIT_STAT_SOURCE_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_STAT_SOURCE_EN_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void fecstat_field_STAT_SOURCE0_ADDR_set( fecstat_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_STAT_SOURCE0_ADDR_set( fecstat_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_STAT_SOURCE0_ADDR_set", N, 47);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fecstat_field_STAT_SOURCE0_ADDR_set", value, 31);
    IOLOG( "%s <= N=%d 0x%08x", "fecstat_field_STAT_SOURCE0_ADDR_set", N, value );

    /* ((0x00006008 + (N) * 0x20) bits 4:0) field STAT_SOURCE0_ADDR of register PMC_FECSTAT_REG_STAT_SOURCE_0_ADR index N=0..47 */
    fecstat_reg_STAT_SOURCE_0_ADR_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   FECSTAT_REG_STAT_SOURCE_0_ADR_BIT_STAT_SOURCE0_ADDR_MSK,
                                                   FECSTAT_REG_STAT_SOURCE_0_ADR_BIT_STAT_SOURCE0_ADDR_OFF,
                                                   value);
}

static INLINE UINT32 fecstat_field_STAT_SOURCE0_ADDR_get( fecstat_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_STAT_SOURCE0_ADDR_get( fecstat_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_STAT_SOURCE0_ADDR_get", N, 47);
    /* ((0x00006008 + (N) * 0x20) bits 4:0) field STAT_SOURCE0_ADDR of register PMC_FECSTAT_REG_STAT_SOURCE_0_ADR index N=0..47 */
    reg_value = fecstat_reg_STAT_SOURCE_0_ADR_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & FECSTAT_REG_STAT_SOURCE_0_ADR_BIT_STAT_SOURCE0_ADDR_MSK) >> FECSTAT_REG_STAT_SOURCE_0_ADR_BIT_STAT_SOURCE0_ADDR_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "fecstat_field_STAT_SOURCE0_ADDR_get", N, value );

    return value;
}
static INLINE void fecstat_field_range_STAT_SOURCE0_ADDR_set( fecstat_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_range_STAT_SOURCE0_ADDR_set( fecstat_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_range_STAT_SOURCE0_ADDR_set", N, 47);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_STAT_SOURCE0_ADDR_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_STAT_SOURCE0_ADDR_set", stop_bit, 4 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_STAT_SOURCE0_ADDR_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00006008 + (N) * 0x20) bits 4:0) field STAT_SOURCE0_ADDR of register PMC_FECSTAT_REG_STAT_SOURCE_0_ADR index N=0..47 */
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
        /* ((0x00006008 + (N) * 0x20) bits 4:0) field STAT_SOURCE0_ADDR of register PMC_FECSTAT_REG_STAT_SOURCE_0_ADR index N=0..47 */
        fecstat_reg_STAT_SOURCE_0_ADR_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       subfield_mask << (FECSTAT_REG_STAT_SOURCE_0_ADR_BIT_STAT_SOURCE0_ADDR_OFF + subfield_offset),
                                                       FECSTAT_REG_STAT_SOURCE_0_ADR_BIT_STAT_SOURCE0_ADDR_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 fecstat_field_range_STAT_SOURCE0_ADDR_get( fecstat_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_range_STAT_SOURCE0_ADDR_get( fecstat_buffer_t *b_ptr,
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

    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_range_STAT_SOURCE0_ADDR_get", N, 47);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_STAT_SOURCE0_ADDR_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_STAT_SOURCE0_ADDR_get", stop_bit, 4 );
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
    /* ((0x00006008 + (N) * 0x20) bits 4:0) field STAT_SOURCE0_ADDR of register PMC_FECSTAT_REG_STAT_SOURCE_0_ADR index N=0..47 */
    reg_value = fecstat_reg_STAT_SOURCE_0_ADR_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    field_value = (reg_value & FECSTAT_REG_STAT_SOURCE_0_ADR_BIT_STAT_SOURCE0_ADDR_MSK)
                  >> FECSTAT_REG_STAT_SOURCE_0_ADR_BIT_STAT_SOURCE0_ADDR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FECSTAT_REG_STAT_SOURCE_0_ADR_BIT_STAT_SOURCE0_ADDR_MSK, FECSTAT_REG_STAT_SOURCE_0_ADR_BIT_STAT_SOURCE0_ADDR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_STAT_SOURCE0_ADDR_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void fecstat_field_STAT_SOURCE1_ADDR_set( fecstat_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_STAT_SOURCE1_ADDR_set( fecstat_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_STAT_SOURCE1_ADDR_set", N, 47);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fecstat_field_STAT_SOURCE1_ADDR_set", value, 31);
    IOLOG( "%s <= N=%d 0x%08x", "fecstat_field_STAT_SOURCE1_ADDR_set", N, value );

    /* ((0x0000600c + (N) * 0x20) bits 4:0) field STAT_SOURCE1_ADDR of register PMC_FECSTAT_REG_STAT_SOURCE_1_ADR index N=0..47 */
    fecstat_reg_STAT_SOURCE_1_ADR_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   FECSTAT_REG_STAT_SOURCE_1_ADR_BIT_STAT_SOURCE1_ADDR_MSK,
                                                   FECSTAT_REG_STAT_SOURCE_1_ADR_BIT_STAT_SOURCE1_ADDR_OFF,
                                                   value);
}

static INLINE UINT32 fecstat_field_STAT_SOURCE1_ADDR_get( fecstat_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_STAT_SOURCE1_ADDR_get( fecstat_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_STAT_SOURCE1_ADDR_get", N, 47);
    /* ((0x0000600c + (N) * 0x20) bits 4:0) field STAT_SOURCE1_ADDR of register PMC_FECSTAT_REG_STAT_SOURCE_1_ADR index N=0..47 */
    reg_value = fecstat_reg_STAT_SOURCE_1_ADR_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & FECSTAT_REG_STAT_SOURCE_1_ADR_BIT_STAT_SOURCE1_ADDR_MSK) >> FECSTAT_REG_STAT_SOURCE_1_ADR_BIT_STAT_SOURCE1_ADDR_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "fecstat_field_STAT_SOURCE1_ADDR_get", N, value );

    return value;
}
static INLINE void fecstat_field_range_STAT_SOURCE1_ADDR_set( fecstat_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_range_STAT_SOURCE1_ADDR_set( fecstat_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_range_STAT_SOURCE1_ADDR_set", N, 47);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_STAT_SOURCE1_ADDR_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_STAT_SOURCE1_ADDR_set", stop_bit, 4 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_STAT_SOURCE1_ADDR_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000600c + (N) * 0x20) bits 4:0) field STAT_SOURCE1_ADDR of register PMC_FECSTAT_REG_STAT_SOURCE_1_ADR index N=0..47 */
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
        /* ((0x0000600c + (N) * 0x20) bits 4:0) field STAT_SOURCE1_ADDR of register PMC_FECSTAT_REG_STAT_SOURCE_1_ADR index N=0..47 */
        fecstat_reg_STAT_SOURCE_1_ADR_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       subfield_mask << (FECSTAT_REG_STAT_SOURCE_1_ADR_BIT_STAT_SOURCE1_ADDR_OFF + subfield_offset),
                                                       FECSTAT_REG_STAT_SOURCE_1_ADR_BIT_STAT_SOURCE1_ADDR_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 fecstat_field_range_STAT_SOURCE1_ADDR_get( fecstat_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_range_STAT_SOURCE1_ADDR_get( fecstat_buffer_t *b_ptr,
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

    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_range_STAT_SOURCE1_ADDR_get", N, 47);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_STAT_SOURCE1_ADDR_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_STAT_SOURCE1_ADDR_get", stop_bit, 4 );
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
    /* ((0x0000600c + (N) * 0x20) bits 4:0) field STAT_SOURCE1_ADDR of register PMC_FECSTAT_REG_STAT_SOURCE_1_ADR index N=0..47 */
    reg_value = fecstat_reg_STAT_SOURCE_1_ADR_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    field_value = (reg_value & FECSTAT_REG_STAT_SOURCE_1_ADR_BIT_STAT_SOURCE1_ADDR_MSK)
                  >> FECSTAT_REG_STAT_SOURCE_1_ADR_BIT_STAT_SOURCE1_ADDR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FECSTAT_REG_STAT_SOURCE_1_ADR_BIT_STAT_SOURCE1_ADDR_MSK, FECSTAT_REG_STAT_SOURCE_1_ADR_BIT_STAT_SOURCE1_ADDR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_STAT_SOURCE1_ADDR_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void fecstat_field_STAT_SOURCE2_ADDR_set( fecstat_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_STAT_SOURCE2_ADDR_set( fecstat_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_STAT_SOURCE2_ADDR_set", N, 47);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fecstat_field_STAT_SOURCE2_ADDR_set", value, 31);
    IOLOG( "%s <= N=%d 0x%08x", "fecstat_field_STAT_SOURCE2_ADDR_set", N, value );

    /* ((0x00006010 + (N) * 0x20) bits 4:0) field STAT_SOURCE2_ADDR of register PMC_FECSTAT_REG_STAT_SOURCE_2_ADR index N=0..47 */
    fecstat_reg_STAT_SOURCE_2_ADR_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   FECSTAT_REG_STAT_SOURCE_2_ADR_BIT_STAT_SOURCE2_ADDR_MSK,
                                                   FECSTAT_REG_STAT_SOURCE_2_ADR_BIT_STAT_SOURCE2_ADDR_OFF,
                                                   value);
}

static INLINE UINT32 fecstat_field_STAT_SOURCE2_ADDR_get( fecstat_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_STAT_SOURCE2_ADDR_get( fecstat_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_STAT_SOURCE2_ADDR_get", N, 47);
    /* ((0x00006010 + (N) * 0x20) bits 4:0) field STAT_SOURCE2_ADDR of register PMC_FECSTAT_REG_STAT_SOURCE_2_ADR index N=0..47 */
    reg_value = fecstat_reg_STAT_SOURCE_2_ADR_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & FECSTAT_REG_STAT_SOURCE_2_ADR_BIT_STAT_SOURCE2_ADDR_MSK) >> FECSTAT_REG_STAT_SOURCE_2_ADR_BIT_STAT_SOURCE2_ADDR_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "fecstat_field_STAT_SOURCE2_ADDR_get", N, value );

    return value;
}
static INLINE void fecstat_field_range_STAT_SOURCE2_ADDR_set( fecstat_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_range_STAT_SOURCE2_ADDR_set( fecstat_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_range_STAT_SOURCE2_ADDR_set", N, 47);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_STAT_SOURCE2_ADDR_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_STAT_SOURCE2_ADDR_set", stop_bit, 4 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_STAT_SOURCE2_ADDR_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00006010 + (N) * 0x20) bits 4:0) field STAT_SOURCE2_ADDR of register PMC_FECSTAT_REG_STAT_SOURCE_2_ADR index N=0..47 */
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
        /* ((0x00006010 + (N) * 0x20) bits 4:0) field STAT_SOURCE2_ADDR of register PMC_FECSTAT_REG_STAT_SOURCE_2_ADR index N=0..47 */
        fecstat_reg_STAT_SOURCE_2_ADR_array_field_set( b_ptr,
                                                       h_ptr,
                                                       N,
                                                       subfield_mask << (FECSTAT_REG_STAT_SOURCE_2_ADR_BIT_STAT_SOURCE2_ADDR_OFF + subfield_offset),
                                                       FECSTAT_REG_STAT_SOURCE_2_ADR_BIT_STAT_SOURCE2_ADDR_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 fecstat_field_range_STAT_SOURCE2_ADDR_get( fecstat_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_range_STAT_SOURCE2_ADDR_get( fecstat_buffer_t *b_ptr,
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

    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_range_STAT_SOURCE2_ADDR_get", N, 47);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_STAT_SOURCE2_ADDR_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_STAT_SOURCE2_ADDR_get", stop_bit, 4 );
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
    /* ((0x00006010 + (N) * 0x20) bits 4:0) field STAT_SOURCE2_ADDR of register PMC_FECSTAT_REG_STAT_SOURCE_2_ADR index N=0..47 */
    reg_value = fecstat_reg_STAT_SOURCE_2_ADR_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    field_value = (reg_value & FECSTAT_REG_STAT_SOURCE_2_ADR_BIT_STAT_SOURCE2_ADDR_MSK)
                  >> FECSTAT_REG_STAT_SOURCE_2_ADR_BIT_STAT_SOURCE2_ADDR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FECSTAT_REG_STAT_SOURCE_2_ADR_BIT_STAT_SOURCE2_ADDR_MSK, FECSTAT_REG_STAT_SOURCE_2_ADR_BIT_STAT_SOURCE2_ADDR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_STAT_SOURCE2_ADDR_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void fecstat_field_LLID_set( fecstat_buffer_t *b_ptr,
                                           lineotn_handle_t *h_ptr,
                                           UINT32  N,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_LLID_set( fecstat_buffer_t *b_ptr,
                                           lineotn_handle_t *h_ptr,
                                           UINT32  N,
                                           UINT32 value )
{
    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_LLID_set", N, 47);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fecstat_field_LLID_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "fecstat_field_LLID_set", N, value );

    /* ((0x00006014 + (N) * 0x20) bits 7:0) field LLID of register PMC_FECSTAT_REG_LOGICAL_LANE_ID index N=0..47 */
    fecstat_reg_LOGICAL_LANE_ID_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 FECSTAT_REG_LOGICAL_LANE_ID_BIT_LLID_MSK,
                                                 FECSTAT_REG_LOGICAL_LANE_ID_BIT_LLID_OFF,
                                                 value);
}

static INLINE UINT32 fecstat_field_LLID_get( fecstat_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_LLID_get( fecstat_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_LLID_get", N, 47);
    /* ((0x00006014 + (N) * 0x20) bits 7:0) field LLID of register PMC_FECSTAT_REG_LOGICAL_LANE_ID index N=0..47 */
    reg_value = fecstat_reg_LOGICAL_LANE_ID_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & FECSTAT_REG_LOGICAL_LANE_ID_BIT_LLID_MSK) >> FECSTAT_REG_LOGICAL_LANE_ID_BIT_LLID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "fecstat_field_LLID_get", N, value );

    return value;
}
static INLINE void fecstat_field_range_LLID_set( fecstat_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_range_LLID_set( fecstat_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value )
{
    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_range_LLID_set", N, 47);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_LLID_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_LLID_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_LLID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00006014 + (N) * 0x20) bits 7:0) field LLID of register PMC_FECSTAT_REG_LOGICAL_LANE_ID index N=0..47 */
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
        /* ((0x00006014 + (N) * 0x20) bits 7:0) field LLID of register PMC_FECSTAT_REG_LOGICAL_LANE_ID index N=0..47 */
        fecstat_reg_LOGICAL_LANE_ID_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     subfield_mask << (FECSTAT_REG_LOGICAL_LANE_ID_BIT_LLID_OFF + subfield_offset),
                                                     FECSTAT_REG_LOGICAL_LANE_ID_BIT_LLID_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 fecstat_field_range_LLID_get( fecstat_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_range_LLID_get( fecstat_buffer_t *b_ptr,
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

    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_range_LLID_get", N, 47);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_LLID_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_LLID_get", stop_bit, 7 );
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
    /* ((0x00006014 + (N) * 0x20) bits 7:0) field LLID of register PMC_FECSTAT_REG_LOGICAL_LANE_ID index N=0..47 */
    reg_value = fecstat_reg_LOGICAL_LANE_ID_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    field_value = (reg_value & FECSTAT_REG_LOGICAL_LANE_ID_BIT_LLID_MSK)
                  >> FECSTAT_REG_LOGICAL_LANE_ID_BIT_LLID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FECSTAT_REG_LOGICAL_LANE_ID_BIT_LLID_MSK, FECSTAT_REG_LOGICAL_LANE_ID_BIT_LLID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_LLID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void fecstat_field_PHY_LN_set( fecstat_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_PHY_LN_set( fecstat_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_PHY_LN_set", N, 47);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fecstat_field_PHY_LN_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "fecstat_field_PHY_LN_set", N, value );

    /* ((0x00006018 + (N) * 0x20) bits 7:0) field PHY_LN of register PMC_FECSTAT_REG_PHYSICAL_LANE_INDEX index N=0..47 */
    fecstat_reg_PHYSICAL_LANE_INDEX_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     FECSTAT_REG_PHYSICAL_LANE_INDEX_BIT_PHY_LN_MSK,
                                                     FECSTAT_REG_PHYSICAL_LANE_INDEX_BIT_PHY_LN_OFF,
                                                     value);
}

static INLINE UINT32 fecstat_field_PHY_LN_get( fecstat_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_PHY_LN_get( fecstat_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_PHY_LN_get", N, 47);
    /* ((0x00006018 + (N) * 0x20) bits 7:0) field PHY_LN of register PMC_FECSTAT_REG_PHYSICAL_LANE_INDEX index N=0..47 */
    reg_value = fecstat_reg_PHYSICAL_LANE_INDEX_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & FECSTAT_REG_PHYSICAL_LANE_INDEX_BIT_PHY_LN_MSK) >> FECSTAT_REG_PHYSICAL_LANE_INDEX_BIT_PHY_LN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "fecstat_field_PHY_LN_get", N, value );

    return value;
}
static INLINE void fecstat_field_range_PHY_LN_set( fecstat_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_range_PHY_LN_set( fecstat_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_range_PHY_LN_set", N, 47);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_PHY_LN_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_PHY_LN_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_PHY_LN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00006018 + (N) * 0x20) bits 7:0) field PHY_LN of register PMC_FECSTAT_REG_PHYSICAL_LANE_INDEX index N=0..47 */
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
        /* ((0x00006018 + (N) * 0x20) bits 7:0) field PHY_LN of register PMC_FECSTAT_REG_PHYSICAL_LANE_INDEX index N=0..47 */
        fecstat_reg_PHYSICAL_LANE_INDEX_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         subfield_mask << (FECSTAT_REG_PHYSICAL_LANE_INDEX_BIT_PHY_LN_OFF + subfield_offset),
                                                         FECSTAT_REG_PHYSICAL_LANE_INDEX_BIT_PHY_LN_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 fecstat_field_range_PHY_LN_get( fecstat_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_range_PHY_LN_get( fecstat_buffer_t *b_ptr,
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

    if (N > 47)
        IO_RANGE_CHECK("%s N is %d but max is %d", "fecstat_field_range_PHY_LN_get", N, 47);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_PHY_LN_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_PHY_LN_get", stop_bit, 7 );
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
    /* ((0x00006018 + (N) * 0x20) bits 7:0) field PHY_LN of register PMC_FECSTAT_REG_PHYSICAL_LANE_INDEX index N=0..47 */
    reg_value = fecstat_reg_PHYSICAL_LANE_INDEX_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    field_value = (reg_value & FECSTAT_REG_PHYSICAL_LANE_INDEX_BIT_PHY_LN_MSK)
                  >> FECSTAT_REG_PHYSICAL_LANE_INDEX_BIT_PHY_LN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FECSTAT_REG_PHYSICAL_LANE_INDEX_BIT_PHY_LN_MSK, FECSTAT_REG_PHYSICAL_LANE_INDEX_BIT_PHY_LN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_PHY_LN_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void fecstat_field_AIR_ERR_INT_E_set( fecstat_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_AIR_ERR_INT_E_set( fecstat_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fecstat_field_AIR_ERR_INT_E_set", value, 7);
    IOLOG( "%s <= 0x%08x", "fecstat_field_AIR_ERR_INT_E_set", value );

    /* (0x00006614 bits 2:0) field AIR_ERR_INT_E of register PMC_FECSTAT_REG_AIR_ERR_INT_EN */
    fecstat_reg_AIR_ERR_INT_EN_field_set( b_ptr,
                                          h_ptr,
                                          FECSTAT_REG_AIR_ERR_INT_EN_BIT_AIR_ERR_INT_E_MSK,
                                          FECSTAT_REG_AIR_ERR_INT_EN_BIT_AIR_ERR_INT_E_OFF,
                                          value);
}

static INLINE UINT32 fecstat_field_AIR_ERR_INT_E_get( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_AIR_ERR_INT_E_get( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00006614 bits 2:0) field AIR_ERR_INT_E of register PMC_FECSTAT_REG_AIR_ERR_INT_EN */
    reg_value = fecstat_reg_AIR_ERR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & FECSTAT_REG_AIR_ERR_INT_EN_BIT_AIR_ERR_INT_E_MSK) >> FECSTAT_REG_AIR_ERR_INT_EN_BIT_AIR_ERR_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "fecstat_field_AIR_ERR_INT_E_get", value );

    return value;
}
static INLINE void fecstat_field_range_AIR_ERR_INT_E_set( fecstat_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_range_AIR_ERR_INT_E_set( fecstat_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_AIR_ERR_INT_E_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_AIR_ERR_INT_E_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_AIR_ERR_INT_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00006614 bits 2:0) field AIR_ERR_INT_E of register PMC_FECSTAT_REG_AIR_ERR_INT_EN */
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
        /* (0x00006614 bits 2:0) field AIR_ERR_INT_E of register PMC_FECSTAT_REG_AIR_ERR_INT_EN */
        fecstat_reg_AIR_ERR_INT_EN_field_set( b_ptr,
                                              h_ptr,
                                              subfield_mask << (FECSTAT_REG_AIR_ERR_INT_EN_BIT_AIR_ERR_INT_E_OFF + subfield_offset),
                                              FECSTAT_REG_AIR_ERR_INT_EN_BIT_AIR_ERR_INT_E_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 fecstat_field_range_AIR_ERR_INT_E_get( fecstat_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_range_AIR_ERR_INT_E_get( fecstat_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_AIR_ERR_INT_E_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_AIR_ERR_INT_E_get", stop_bit, 2 );
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
    /* (0x00006614 bits 2:0) field AIR_ERR_INT_E of register PMC_FECSTAT_REG_AIR_ERR_INT_EN */
    reg_value = fecstat_reg_AIR_ERR_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & FECSTAT_REG_AIR_ERR_INT_EN_BIT_AIR_ERR_INT_E_MSK)
                  >> FECSTAT_REG_AIR_ERR_INT_EN_BIT_AIR_ERR_INT_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FECSTAT_REG_AIR_ERR_INT_EN_BIT_AIR_ERR_INT_E_MSK, FECSTAT_REG_AIR_ERR_INT_EN_BIT_AIR_ERR_INT_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_AIR_ERR_INT_E_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void fecstat_field_AIR_ERR_INT_I_set_to_clear( fecstat_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_AIR_ERR_INT_I_set_to_clear( fecstat_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fecstat_field_AIR_ERR_INT_I_set_to_clear", value, 7);
    IOLOG( "%s <= 0x%08x", "fecstat_field_AIR_ERR_INT_I_set_to_clear", value );

    /* (0x00006618 bits 2:0) field AIR_ERR_INT_I of register PMC_FECSTAT_REG_AIR_ERR_INT_REQUEST */
    fecstat_reg_AIR_ERR_INT_REQUEST_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               FECSTAT_REG_AIR_ERR_INT_REQUEST_BIT_AIR_ERR_INT_I_MSK,
                                                               FECSTAT_REG_AIR_ERR_INT_REQUEST_BIT_AIR_ERR_INT_I_OFF,
                                                               value);
}

static INLINE UINT32 fecstat_field_AIR_ERR_INT_I_get( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_AIR_ERR_INT_I_get( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00006618 bits 2:0) field AIR_ERR_INT_I of register PMC_FECSTAT_REG_AIR_ERR_INT_REQUEST */
    reg_value = fecstat_reg_AIR_ERR_INT_REQUEST_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & FECSTAT_REG_AIR_ERR_INT_REQUEST_BIT_AIR_ERR_INT_I_MSK) >> FECSTAT_REG_AIR_ERR_INT_REQUEST_BIT_AIR_ERR_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "fecstat_field_AIR_ERR_INT_I_get", value );

    return value;
}
static INLINE void fecstat_field_range_AIR_ERR_INT_I_set_to_clear( fecstat_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void fecstat_field_range_AIR_ERR_INT_I_set_to_clear( fecstat_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_AIR_ERR_INT_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_AIR_ERR_INT_I_set_to_clear", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_AIR_ERR_INT_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00006618 bits 2:0) field AIR_ERR_INT_I of register PMC_FECSTAT_REG_AIR_ERR_INT_REQUEST */
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
        /* (0x00006618 bits 2:0) field AIR_ERR_INT_I of register PMC_FECSTAT_REG_AIR_ERR_INT_REQUEST */
        fecstat_reg_AIR_ERR_INT_REQUEST_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   subfield_mask << (FECSTAT_REG_AIR_ERR_INT_REQUEST_BIT_AIR_ERR_INT_I_OFF + subfield_offset),
                                                                   FECSTAT_REG_AIR_ERR_INT_REQUEST_BIT_AIR_ERR_INT_I_OFF + subfield_offset,
                                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 fecstat_field_range_AIR_ERR_INT_I_get( fecstat_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_range_AIR_ERR_INT_I_get( fecstat_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_AIR_ERR_INT_I_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_AIR_ERR_INT_I_get", stop_bit, 2 );
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
    /* (0x00006618 bits 2:0) field AIR_ERR_INT_I of register PMC_FECSTAT_REG_AIR_ERR_INT_REQUEST */
    reg_value = fecstat_reg_AIR_ERR_INT_REQUEST_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & FECSTAT_REG_AIR_ERR_INT_REQUEST_BIT_AIR_ERR_INT_I_MSK)
                  >> FECSTAT_REG_AIR_ERR_INT_REQUEST_BIT_AIR_ERR_INT_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FECSTAT_REG_AIR_ERR_INT_REQUEST_BIT_AIR_ERR_INT_I_MSK, FECSTAT_REG_AIR_ERR_INT_REQUEST_BIT_AIR_ERR_INT_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_AIR_ERR_INT_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fecstat_field_range_AIR_ERR_INT_I_poll( fecstat_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fecstat_field_range_AIR_ERR_INT_I_poll( fecstat_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_AIR_ERR_INT_I_poll", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_AIR_ERR_INT_I_poll", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_AIR_ERR_INT_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00006618 bits 2:0) field AIR_ERR_INT_I of register PMC_FECSTAT_REG_AIR_ERR_INT_REQUEST */
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
        /* (0x00006618 bits 2:0) field AIR_ERR_INT_I of register PMC_FECSTAT_REG_AIR_ERR_INT_REQUEST */
        return fecstat_reg_AIR_ERR_INT_REQUEST_poll( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (FECSTAT_REG_AIR_ERR_INT_REQUEST_BIT_AIR_ERR_INT_I_OFF + subfield_offset),
                                                     value << (FECSTAT_REG_AIR_ERR_INT_REQUEST_BIT_AIR_ERR_INT_I_OFF + subfield_offset),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE fecstat_field_AIR_ERR_INT_I_poll( fecstat_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fecstat_field_AIR_ERR_INT_I_poll( fecstat_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "fecstat_field_AIR_ERR_INT_I_poll", value );

    /* (0x00006618 bits 2:0) field AIR_ERR_INT_I of register PMC_FECSTAT_REG_AIR_ERR_INT_REQUEST */
    return fecstat_reg_AIR_ERR_INT_REQUEST_poll( b_ptr,
                                                 h_ptr,
                                                 FECSTAT_REG_AIR_ERR_INT_REQUEST_BIT_AIR_ERR_INT_I_MSK,
                                                 (value<<FECSTAT_REG_AIR_ERR_INT_REQUEST_BIT_AIR_ERR_INT_I_OFF),
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
static INLINE UINT32 fecstat_field_FEC_RD_ERR_VECTOR_get( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_FEC_RD_ERR_VECTOR_get( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000661c bits 15:0) field FEC_RD_ERR_VECTOR of register PMC_FECSTAT_REG_FEC_READ_ERR_VECTOR */
    reg_value = fecstat_reg_FEC_READ_ERR_VECTOR_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & FECSTAT_REG_FEC_READ_ERR_VECTOR_BIT_FEC_RD_ERR_VECTOR_MSK) >> FECSTAT_REG_FEC_READ_ERR_VECTOR_BIT_FEC_RD_ERR_VECTOR_OFF;
    IOLOG( "%s -> 0x%08x", "fecstat_field_FEC_RD_ERR_VECTOR_get", value );

    return value;
}
static INLINE UINT32 fecstat_field_range_FEC_RD_ERR_VECTOR_get( fecstat_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_range_FEC_RD_ERR_VECTOR_get( fecstat_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_FEC_RD_ERR_VECTOR_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_FEC_RD_ERR_VECTOR_get", stop_bit, 15 );
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
    /* (0x0000661c bits 15:0) field FEC_RD_ERR_VECTOR of register PMC_FECSTAT_REG_FEC_READ_ERR_VECTOR */
    reg_value = fecstat_reg_FEC_READ_ERR_VECTOR_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & FECSTAT_REG_FEC_READ_ERR_VECTOR_BIT_FEC_RD_ERR_VECTOR_MSK)
                  >> FECSTAT_REG_FEC_READ_ERR_VECTOR_BIT_FEC_RD_ERR_VECTOR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FECSTAT_REG_FEC_READ_ERR_VECTOR_BIT_FEC_RD_ERR_VECTOR_MSK, FECSTAT_REG_FEC_READ_ERR_VECTOR_BIT_FEC_RD_ERR_VECTOR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_FEC_RD_ERR_VECTOR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 fecstat_field_FWI_WR_ERR_VECTOR_get( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_FWI_WR_ERR_VECTOR_get( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00006620 bits 15:0) field FWI_WR_ERR_VECTOR of register PMC_FECSTAT_REG_FWI_WRITE_ERR_VECTOR */
    reg_value = fecstat_reg_FWI_WRITE_ERR_VECTOR_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & FECSTAT_REG_FWI_WRITE_ERR_VECTOR_BIT_FWI_WR_ERR_VECTOR_MSK) >> FECSTAT_REG_FWI_WRITE_ERR_VECTOR_BIT_FWI_WR_ERR_VECTOR_OFF;
    IOLOG( "%s -> 0x%08x", "fecstat_field_FWI_WR_ERR_VECTOR_get", value );

    return value;
}
static INLINE UINT32 fecstat_field_range_FWI_WR_ERR_VECTOR_get( fecstat_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fecstat_field_range_FWI_WR_ERR_VECTOR_get( fecstat_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fecstat_field_range_FWI_WR_ERR_VECTOR_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fecstat_field_range_FWI_WR_ERR_VECTOR_get", stop_bit, 15 );
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
    /* (0x00006620 bits 15:0) field FWI_WR_ERR_VECTOR of register PMC_FECSTAT_REG_FWI_WRITE_ERR_VECTOR */
    reg_value = fecstat_reg_FWI_WRITE_ERR_VECTOR_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & FECSTAT_REG_FWI_WRITE_ERR_VECTOR_BIT_FWI_WR_ERR_VECTOR_MSK)
                  >> FECSTAT_REG_FWI_WRITE_ERR_VECTOR_BIT_FWI_WR_ERR_VECTOR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FECSTAT_REG_FWI_WRITE_ERR_VECTOR_BIT_FWI_WR_ERR_VECTOR_MSK, FECSTAT_REG_FWI_WRITE_ERR_VECTOR_BIT_FWI_WR_ERR_VECTOR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "fecstat_field_range_FWI_WR_ERR_VECTOR_get", start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FECSTAT_IO_INLINE_H */
