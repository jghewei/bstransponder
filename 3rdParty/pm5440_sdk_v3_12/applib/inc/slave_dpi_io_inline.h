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
 *     and register accessor functions for the slave_dpi block
 *****************************************************************************/
#ifndef _SLAVE_DPI_IO_INLINE_H
#define _SLAVE_DPI_IO_INLINE_H

#include "cpb_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "slave_dpi_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SLAVE_DPI_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for slave_dpi
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
} slave_dpi_buffer_t;
static INLINE void slave_dpi_buffer_init( slave_dpi_buffer_t *b_ptr,
                                          cpb_handle_t *h_ptr,
                                          UINT32 A ) ALWAYS_INLINE;
static INLINE void slave_dpi_buffer_init( slave_dpi_buffer_t *b_ptr,
                                          cpb_handle_t *h_ptr,
                                          UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x4000) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "slave_dpi_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void slave_dpi_buffer_flush( slave_dpi_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void slave_dpi_buffer_flush( slave_dpi_buffer_t *b_ptr )
{
    IOLOG( "slave_dpi_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 slave_dpi_reg_read( slave_dpi_buffer_t *b_ptr,
                                         cpb_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 mem_type,
                                         UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_read( slave_dpi_buffer_t *b_ptr,
                                         cpb_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 mem_type,
                                         UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->A)*0x4000),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (A)*0x4000) + reg);
    return value;
}
static INLINE void slave_dpi_reg_write( slave_dpi_buffer_t *b_ptr,
                                        cpb_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_write( slave_dpi_buffer_t *b_ptr,
                                        cpb_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->A)*0x4000),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x4000) + reg,
                         value);
}

static INLINE void slave_dpi_field_set( slave_dpi_buffer_t *b_ptr,
                                        cpb_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 mask,
                                        UINT32 unused_mask,
                                        UINT32 ofs,
                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_set( slave_dpi_buffer_t *b_ptr,
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
                          (b_ptr->base_address + (b_ptr->A)*0x4000),
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
                             (((pmc_handle_t *)h_ptr)->base_address + (A)*0x4000) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x4000) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void slave_dpi_action_on_write_field_set( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mem_type,
                                                        UINT32 reg,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_action_on_write_field_set( slave_dpi_buffer_t *b_ptr,
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
                                         (b_ptr->base_address + (b_ptr->A)*0x4000),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x4000) + reg,
                         value<<ofs);
}

static INLINE void slave_dpi_burst_read( slave_dpi_buffer_t *b_ptr,
                                         cpb_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void slave_dpi_burst_read( slave_dpi_buffer_t *b_ptr,
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
                          (b_ptr->base_address + (b_ptr->A)*0x4000),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (A)*0x4000) + reg,
                          len,
                          value);
}

static INLINE void slave_dpi_burst_write( slave_dpi_buffer_t *b_ptr,
                                          cpb_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void slave_dpi_burst_write( slave_dpi_buffer_t *b_ptr,
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
                           (b_ptr->base_address + (b_ptr->A)*0x4000),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x4000) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE slave_dpi_poll( slave_dpi_buffer_t *b_ptr,
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
static INLINE PMC_POLL_RETURN_TYPE slave_dpi_poll( slave_dpi_buffer_t *b_ptr,
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
                           (b_ptr->base_address + (b_ptr->A)*0x4000),
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
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x4000) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for slave_dpi
 * ==================================================================================
 */

static INLINE void slave_dpi_reg_ZONE_CONFIG_write( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_ZONE_CONFIG_write( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "slave_dpi_reg_ZONE_CONFIG_write", value );
    slave_dpi_reg_write( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_CONFIG,
                         PMC_SLAVE_DPI_REG_ZONE_CONFIG,
                         value);
}

static INLINE void slave_dpi_reg_ZONE_CONFIG_field_set( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_ZONE_CONFIG_field_set( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "slave_dpi_reg_ZONE_CONFIG_field_set", A, mask, ofs, value );
    slave_dpi_field_set( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_CONFIG,
                         PMC_SLAVE_DPI_REG_ZONE_CONFIG,
                         mask,
                         PMC_SLAVE_DPI_REG_ZONE_CONFIG_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 slave_dpi_reg_ZONE_CONFIG_read( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_ZONE_CONFIG_read( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_CONFIG,
                                    PMC_SLAVE_DPI_REG_ZONE_CONFIG);

    IOLOG( "%s -> 0x%08x; A=%d", "slave_dpi_reg_ZONE_CONFIG_read", reg_value, A);
    return reg_value;
}

static INLINE void slave_dpi_reg_BURST_CONFIG_write( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_BURST_CONFIG_write( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "slave_dpi_reg_BURST_CONFIG_write", value );
    slave_dpi_reg_write( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_CONFIG,
                         PMC_SLAVE_DPI_REG_BURST_CONFIG,
                         value);
}

static INLINE void slave_dpi_reg_BURST_CONFIG_field_set( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_BURST_CONFIG_field_set( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "slave_dpi_reg_BURST_CONFIG_field_set", A, mask, ofs, value );
    slave_dpi_field_set( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_CONFIG,
                         PMC_SLAVE_DPI_REG_BURST_CONFIG,
                         mask,
                         PMC_SLAVE_DPI_REG_BURST_CONFIG_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 slave_dpi_reg_BURST_CONFIG_read( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_BURST_CONFIG_read( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_CONFIG,
                                    PMC_SLAVE_DPI_REG_BURST_CONFIG);

    IOLOG( "%s -> 0x%08x; A=%d", "slave_dpi_reg_BURST_CONFIG_read", reg_value, A);
    return reg_value;
}

static INLINE void slave_dpi_reg_HRD_CONFIG_write( slave_dpi_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_HRD_CONFIG_write( slave_dpi_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "slave_dpi_reg_HRD_CONFIG_write", value );
    slave_dpi_reg_write( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_CONFIG,
                         PMC_SLAVE_DPI_REG_HRD_CONFIG,
                         value);
}

static INLINE void slave_dpi_reg_HRD_CONFIG_field_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_HRD_CONFIG_field_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "slave_dpi_reg_HRD_CONFIG_field_set", A, mask, ofs, value );
    slave_dpi_field_set( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_CONFIG,
                         PMC_SLAVE_DPI_REG_HRD_CONFIG,
                         mask,
                         PMC_SLAVE_DPI_REG_HRD_CONFIG_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 slave_dpi_reg_HRD_CONFIG_read( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_HRD_CONFIG_read( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_CONFIG,
                                    PMC_SLAVE_DPI_REG_HRD_CONFIG);

    IOLOG( "%s -> 0x%08x; A=%d", "slave_dpi_reg_HRD_CONFIG_read", reg_value, A);
    return reg_value;
}

static INLINE void slave_dpi_reg_PMON_CNT_TRIG_write( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_PMON_CNT_TRIG_write( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "slave_dpi_reg_PMON_CNT_TRIG_write", value );
    slave_dpi_reg_write( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_CONFIG,
                         PMC_SLAVE_DPI_REG_PMON_CNT_TRIG,
                         value);
}

static INLINE void slave_dpi_reg_PMON_CNT_TRIG_field_set( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_PMON_CNT_TRIG_field_set( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "slave_dpi_reg_PMON_CNT_TRIG_field_set", A, mask, ofs, value );
    slave_dpi_field_set( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_CONFIG,
                         PMC_SLAVE_DPI_REG_PMON_CNT_TRIG,
                         mask,
                         PMC_SLAVE_DPI_REG_PMON_CNT_TRIG_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 slave_dpi_reg_PMON_CNT_TRIG_read( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_PMON_CNT_TRIG_read( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_CONFIG,
                                    PMC_SLAVE_DPI_REG_PMON_CNT_TRIG);

    IOLOG( "%s -> 0x%08x; A=%d", "slave_dpi_reg_PMON_CNT_TRIG_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE slave_dpi_reg_PMON_CNT_TRIG_poll( slave_dpi_buffer_t *b_ptr,
                                                                     cpb_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE slave_dpi_reg_PMON_CNT_TRIG_poll( slave_dpi_buffer_t *b_ptr,
                                                                     cpb_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "slave_dpi_reg_PMON_CNT_TRIG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return slave_dpi_poll( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_CONFIG,
                           PMC_SLAVE_DPI_REG_PMON_CNT_TRIG,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);

}


static INLINE void slave_dpi_reg_PMON_MODE_TRIG_write( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_PMON_MODE_TRIG_write( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "slave_dpi_reg_PMON_MODE_TRIG_write", value );
    slave_dpi_reg_write( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_CONFIG,
                         PMC_SLAVE_DPI_REG_PMON_MODE_TRIG,
                         value);
}

static INLINE void slave_dpi_reg_PMON_MODE_TRIG_field_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_PMON_MODE_TRIG_field_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "slave_dpi_reg_PMON_MODE_TRIG_field_set", A, mask, ofs, value );
    slave_dpi_field_set( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_CONFIG,
                         PMC_SLAVE_DPI_REG_PMON_MODE_TRIG,
                         mask,
                         PMC_SLAVE_DPI_REG_PMON_MODE_TRIG_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 slave_dpi_reg_PMON_MODE_TRIG_read( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_PMON_MODE_TRIG_read( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_CONFIG,
                                    PMC_SLAVE_DPI_REG_PMON_MODE_TRIG);

    IOLOG( "%s -> 0x%08x; A=%d", "slave_dpi_reg_PMON_MODE_TRIG_read", reg_value, A);
    return reg_value;
}

static INLINE void slave_dpi_reg_PMON_PKT_TRIG_write( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_PMON_PKT_TRIG_write( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "slave_dpi_reg_PMON_PKT_TRIG_write", value );
    slave_dpi_reg_write( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_CONFIG,
                         PMC_SLAVE_DPI_REG_PMON_PKT_TRIG,
                         value);
}

static INLINE void slave_dpi_reg_PMON_PKT_TRIG_field_set( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_PMON_PKT_TRIG_field_set( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "slave_dpi_reg_PMON_PKT_TRIG_field_set", A, mask, ofs, value );
    slave_dpi_field_set( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_CONFIG,
                         PMC_SLAVE_DPI_REG_PMON_PKT_TRIG,
                         mask,
                         PMC_SLAVE_DPI_REG_PMON_PKT_TRIG_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 slave_dpi_reg_PMON_PKT_TRIG_read( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_PMON_PKT_TRIG_read( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_CONFIG,
                                    PMC_SLAVE_DPI_REG_PMON_PKT_TRIG);

    IOLOG( "%s -> 0x%08x; A=%d", "slave_dpi_reg_PMON_PKT_TRIG_read", reg_value, A);
    return reg_value;
}

static INLINE void slave_dpi_reg_CH_ENABLE_array_write( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_CH_ENABLE_array_write( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "slave_dpi_reg_CH_ENABLE_array_write", value );
    slave_dpi_reg_write( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_CONFIG,
                         PMC_SLAVE_DPI_REG_CH_ENABLE(N),
                         value);
}

static INLINE void slave_dpi_reg_CH_ENABLE_array_field_set( slave_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_CH_ENABLE_array_field_set( slave_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "slave_dpi_reg_CH_ENABLE_array_field_set", A, N, mask, ofs, value );
    slave_dpi_field_set( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_CONFIG,
                         PMC_SLAVE_DPI_REG_CH_ENABLE(N),
                         mask,
                         PMC_SLAVE_DPI_REG_CH_ENABLE_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 slave_dpi_reg_CH_ENABLE_array_read( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_CH_ENABLE_array_read( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_CONFIG,
                                    PMC_SLAVE_DPI_REG_CH_ENABLE(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "slave_dpi_reg_CH_ENABLE_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void slave_dpi_reg_DROP_CNT_TRIG_REG_write( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_DROP_CNT_TRIG_REG_write( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "slave_dpi_reg_DROP_CNT_TRIG_REG_write", value );
    slave_dpi_reg_write( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_SLAVE_DPI_REG_DROP_CNT_TRIG_REG,
                         value);
}

static INLINE void slave_dpi_reg_DROP_CNT_TRIG_REG_field_set( slave_dpi_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_DROP_CNT_TRIG_REG_field_set( slave_dpi_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "slave_dpi_reg_DROP_CNT_TRIG_REG_field_set", A, mask, ofs, value );
    slave_dpi_field_set( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_SLAVE_DPI_REG_DROP_CNT_TRIG_REG,
                         mask,
                         PMC_SLAVE_DPI_REG_DROP_CNT_TRIG_REG_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 slave_dpi_reg_DROP_CNT_TRIG_REG_read( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_DROP_CNT_TRIG_REG_read( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_SLAVE_DPI_REG_DROP_CNT_TRIG_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "slave_dpi_reg_DROP_CNT_TRIG_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void slave_dpi_reg_CH_DROP_INT_array_write( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_CH_DROP_INT_array_write( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "slave_dpi_reg_CH_DROP_INT_array_write", value );
    slave_dpi_reg_write( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_SLAVE_DPI_REG_CH_DROP_INT(N),
                         value);
}

static INLINE void slave_dpi_reg_CH_DROP_INT_array_action_on_write_field_set( slave_dpi_buffer_t *b_ptr,
                                                                              cpb_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32  N,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_CH_DROP_INT_array_action_on_write_field_set( slave_dpi_buffer_t *b_ptr,
                                                                              cpb_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32  N,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "slave_dpi_reg_CH_DROP_INT_array_action_on_write_field_set", A, N, mask, ofs, value );
    slave_dpi_action_on_write_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         MEM_TYPE_STATUS,
                                         PMC_SLAVE_DPI_REG_CH_DROP_INT(N),
                                         mask,
                                         ofs,
                                         value);

}

static INLINE UINT32 slave_dpi_reg_CH_DROP_INT_array_read( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_CH_DROP_INT_array_read( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_SLAVE_DPI_REG_CH_DROP_INT(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "slave_dpi_reg_CH_DROP_INT_array_read", reg_value, A, N);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE slave_dpi_reg_CH_DROP_INT_array_poll( slave_dpi_buffer_t *b_ptr,
                                                                         cpb_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32  N,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE slave_dpi_reg_CH_DROP_INT_array_poll( slave_dpi_buffer_t *b_ptr,
                                                                         cpb_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32  N,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "slave_dpi_reg_CH_DROP_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return slave_dpi_poll( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_STATUS,
                           PMC_SLAVE_DPI_REG_CH_DROP_INT(N),
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);

}


static INLINE void slave_dpi_reg_CH_DROP_COUNT_array_burst_write( slave_dpi_buffer_t *b_ptr,
                                                                  cpb_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_CH_DROP_COUNT_array_burst_write( slave_dpi_buffer_t *b_ptr,
                                                                  cpb_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 150)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "slave_dpi_reg_CH_DROP_COUNT_array_burst_write", ofs, len, 150 );
    IOLOG( "%s of %d words A=%d ofs=%d len=%d", "slave_dpi_reg_CH_DROP_COUNT_array_burst_write", 150, A, ofs, len);
    slave_dpi_burst_write( b_ptr,
                           h_ptr,
                           A,
                           MEM_TYPE_STATUS,
                           PMC_SLAVE_DPI_REG_CH_DROP_COUNT(ofs),
                           len,
                           value);

}

static INLINE void slave_dpi_reg_CH_DROP_COUNT_array_field_set( slave_dpi_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_CH_DROP_COUNT_array_field_set( slave_dpi_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "slave_dpi_reg_CH_DROP_COUNT_array_field_set", A, mask, ofs, value );
    slave_dpi_field_set( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_SLAVE_DPI_REG_CH_DROP_COUNT(N),
                         mask,
                         PMC_SLAVE_DPI_REG_CH_DROP_COUNT_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE void slave_dpi_reg_CH_DROP_COUNT_array_burst_read( slave_dpi_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_CH_DROP_COUNT_array_burst_read( slave_dpi_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value )
{
    if (len + ofs > 150)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "slave_dpi_reg_CH_DROP_COUNT_array_burst_read", ofs, len, 150 );
    slave_dpi_burst_read( b_ptr,
                          h_ptr,
                          A,
                          MEM_TYPE_STATUS,
                          PMC_SLAVE_DPI_REG_CH_DROP_COUNT(ofs),
                          len,
                          value);

    IOLOG( "%s of %d words A=%d ofs=%d len=%d", "slave_dpi_reg_CH_DROP_COUNT_array_burst_read", 150, A, ofs, len);
}

static INLINE UINT32 slave_dpi_reg_CH_DROP_COUNT_array_read( slave_dpi_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_CH_DROP_COUNT_array_read( slave_dpi_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 N )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_SLAVE_DPI_REG_CH_DROP_COUNT(N));

    IOLOG( "%s -> 0x%08x; A=%d", "slave_dpi_reg_CH_DROP_COUNT_array_read", reg_value, A);
    return reg_value;
}

static INLINE void slave_dpi_reg_CH_CONFIG_array_write( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_CH_CONFIG_array_write( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "slave_dpi_reg_CH_CONFIG_array_write", value );
    slave_dpi_reg_write( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_SLAVE_DPI_REG_CH_CONFIG(N),
                         value);
}

static INLINE void slave_dpi_reg_CH_CONFIG_array_field_set( slave_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_CH_CONFIG_array_field_set( slave_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "slave_dpi_reg_CH_CONFIG_array_field_set", A, N, mask, ofs, value );
    slave_dpi_field_set( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_SLAVE_DPI_REG_CH_CONFIG(N),
                         mask,
                         PMC_SLAVE_DPI_REG_CH_CONFIG_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 slave_dpi_reg_CH_CONFIG_array_read( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_CH_CONFIG_array_read( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_SLAVE_DPI_REG_CH_CONFIG(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "slave_dpi_reg_CH_CONFIG_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void slave_dpi_reg_CH_Q_MAP_array_write( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_CH_Q_MAP_array_write( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "slave_dpi_reg_CH_Q_MAP_array_write", value );
    slave_dpi_reg_write( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_SLAVE_DPI_REG_CH_Q_MAP(N),
                         value);
}

static INLINE void slave_dpi_reg_CH_Q_MAP_array_field_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_CH_Q_MAP_array_field_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "slave_dpi_reg_CH_Q_MAP_array_field_set", A, N, mask, ofs, value );
    slave_dpi_field_set( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_SLAVE_DPI_REG_CH_Q_MAP(N),
                         mask,
                         PMC_SLAVE_DPI_REG_CH_Q_MAP_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 slave_dpi_reg_CH_Q_MAP_array_read( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_CH_Q_MAP_array_read( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_SLAVE_DPI_REG_CH_Q_MAP(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "slave_dpi_reg_CH_Q_MAP_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void slave_dpi_reg_ZONE_0_1_array_write( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_ZONE_0_1_array_write( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "slave_dpi_reg_ZONE_0_1_array_write", value );
    slave_dpi_reg_write( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_SLAVE_DPI_REG_ZONE_0_1(N),
                         value);
}

static INLINE void slave_dpi_reg_ZONE_0_1_array_field_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_ZONE_0_1_array_field_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "slave_dpi_reg_ZONE_0_1_array_field_set", A, N, mask, ofs, value );
    slave_dpi_field_set( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_SLAVE_DPI_REG_ZONE_0_1(N),
                         mask,
                         PMC_SLAVE_DPI_REG_ZONE_0_1_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 slave_dpi_reg_ZONE_0_1_array_read( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_ZONE_0_1_array_read( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_SLAVE_DPI_REG_ZONE_0_1(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "slave_dpi_reg_ZONE_0_1_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void slave_dpi_reg_ZONE_1_2_array_write( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_ZONE_1_2_array_write( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "slave_dpi_reg_ZONE_1_2_array_write", value );
    slave_dpi_reg_write( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_SLAVE_DPI_REG_ZONE_1_2(N),
                         value);
}

static INLINE void slave_dpi_reg_ZONE_1_2_array_field_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_ZONE_1_2_array_field_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "slave_dpi_reg_ZONE_1_2_array_field_set", A, N, mask, ofs, value );
    slave_dpi_field_set( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_SLAVE_DPI_REG_ZONE_1_2(N),
                         mask,
                         PMC_SLAVE_DPI_REG_ZONE_1_2_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 slave_dpi_reg_ZONE_1_2_array_read( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_ZONE_1_2_array_read( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_SLAVE_DPI_REG_ZONE_1_2(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "slave_dpi_reg_ZONE_1_2_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void slave_dpi_reg_ZONE_2_3_array_write( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_ZONE_2_3_array_write( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "slave_dpi_reg_ZONE_2_3_array_write", value );
    slave_dpi_reg_write( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_SLAVE_DPI_REG_ZONE_2_3(N),
                         value);
}

static INLINE void slave_dpi_reg_ZONE_2_3_array_field_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_ZONE_2_3_array_field_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "slave_dpi_reg_ZONE_2_3_array_field_set", A, N, mask, ofs, value );
    slave_dpi_field_set( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_SLAVE_DPI_REG_ZONE_2_3(N),
                         mask,
                         PMC_SLAVE_DPI_REG_ZONE_2_3_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 slave_dpi_reg_ZONE_2_3_array_read( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_ZONE_2_3_array_read( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_SLAVE_DPI_REG_ZONE_2_3(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "slave_dpi_reg_ZONE_2_3_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void slave_dpi_reg_ZONE_OVRD_CONFIG_array_write( slave_dpi_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_ZONE_OVRD_CONFIG_array_write( slave_dpi_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "slave_dpi_reg_ZONE_OVRD_CONFIG_array_write", value );
    slave_dpi_reg_write( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG(N),
                         value);
}

static INLINE void slave_dpi_reg_ZONE_OVRD_CONFIG_array_field_set( slave_dpi_buffer_t *b_ptr,
                                                                   cpb_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_reg_ZONE_OVRD_CONFIG_array_field_set( slave_dpi_buffer_t *b_ptr,
                                                                   cpb_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "slave_dpi_reg_ZONE_OVRD_CONFIG_array_field_set", A, N, mask, ofs, value );
    slave_dpi_field_set( b_ptr,
                         h_ptr,
                         A,
                         MEM_TYPE_STATUS,
                         PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG(N),
                         mask,
                         PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG_UNUSED_MASK,
                         ofs,
                         value);

}

static INLINE UINT32 slave_dpi_reg_ZONE_OVRD_CONFIG_array_read( slave_dpi_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_ZONE_OVRD_CONFIG_array_read( slave_dpi_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "slave_dpi_reg_ZONE_OVRD_CONFIG_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE UINT32 slave_dpi_reg_PMON_CNT0_REG_read( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_PMON_CNT0_REG_read( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_SLAVE_DPI_REG_PMON_CNT0_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "slave_dpi_reg_PMON_CNT0_REG_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 slave_dpi_reg_PMON_CNT1_REG_read( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_PMON_CNT1_REG_read( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_SLAVE_DPI_REG_PMON_CNT1_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "slave_dpi_reg_PMON_CNT1_REG_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 slave_dpi_reg_PMON_CNT2_REG_read( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_PMON_CNT2_REG_read( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_SLAVE_DPI_REG_PMON_CNT2_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "slave_dpi_reg_PMON_CNT2_REG_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 slave_dpi_reg_PMON_CNT3_REG_read( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_reg_PMON_CNT3_REG_read( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = slave_dpi_reg_read( b_ptr,
                                    h_ptr,
                                    A,
                                    MEM_TYPE_STATUS,
                                    PMC_SLAVE_DPI_REG_PMON_CNT3_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "slave_dpi_reg_PMON_CNT3_REG_read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void slave_dpi_field_ZONE_OVRD_VAL_set( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_ZONE_OVRD_VAL_set( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_OVRD_VAL_set", A, 7);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_ZONE_OVRD_VAL_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_ZONE_OVRD_VAL_set", A, value );

    /* (0x00000000 bits 25:24) bits 0:1 use field ZONE_OVRD_VAL of register PMC_SLAVE_DPI_REG_ZONE_CONFIG */
    slave_dpi_reg_ZONE_CONFIG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_OVRD_VAL_MSK,
                                         SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_OVRD_VAL_OFF,
                                         value);
}

static INLINE UINT32 slave_dpi_field_ZONE_OVRD_VAL_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_ZONE_OVRD_VAL_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_OVRD_VAL_get", A, 7);
    /* (0x00000000 bits 25:24) bits 0:1 use field ZONE_OVRD_VAL of register PMC_SLAVE_DPI_REG_ZONE_CONFIG */
    reg_value = slave_dpi_reg_ZONE_CONFIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_OVRD_VAL_MSK) >> SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_OVRD_VAL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_ZONE_OVRD_VAL_get", A, value );

    return value;
}
static INLINE void slave_dpi_field_range_ZONE_OVRD_VAL_set( slave_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_range_ZONE_OVRD_VAL_set( slave_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_ZONE_OVRD_VAL_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_ZONE_OVRD_VAL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_ZONE_OVRD_VAL_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_ZONE_OVRD_VAL_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000000 bits 25:24) bits 0:1 use field ZONE_OVRD_VAL of register PMC_SLAVE_DPI_REG_ZONE_CONFIG */
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
        /* (0x00000000 bits 25:24) bits 0:1 use field ZONE_OVRD_VAL of register PMC_SLAVE_DPI_REG_ZONE_CONFIG */
        slave_dpi_reg_ZONE_CONFIG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             subfield_mask << (SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_OVRD_VAL_OFF + subfield_offset),
                                             SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_OVRD_VAL_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 slave_dpi_field_range_ZONE_OVRD_VAL_get( slave_dpi_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_ZONE_OVRD_VAL_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_ZONE_OVRD_VAL_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_ZONE_OVRD_VAL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_ZONE_OVRD_VAL_get", stop_bit, 1 );
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
    /* (0x00000000 bits 25:24) bits 0:1 use field ZONE_OVRD_VAL of register PMC_SLAVE_DPI_REG_ZONE_CONFIG */
    reg_value = slave_dpi_reg_ZONE_CONFIG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_OVRD_VAL_MSK)
                  >> SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_OVRD_VAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_OVRD_VAL_MSK, SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_OVRD_VAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_ZONE_OVRD_VAL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void slave_dpi_field_ZONE_EN_set( slave_dpi_buffer_t *b_ptr,
                                                cpb_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_ZONE_EN_set( slave_dpi_buffer_t *b_ptr,
                                                cpb_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_EN_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_ZONE_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_ZONE_EN_set", A, value );

    /* (0x00000000 bits 17) field ZONE_EN of register PMC_SLAVE_DPI_REG_ZONE_CONFIG */
    slave_dpi_reg_ZONE_CONFIG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_EN_MSK,
                                         SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_EN_OFF,
                                         value);
}

static INLINE UINT32 slave_dpi_field_ZONE_EN_get( slave_dpi_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_ZONE_EN_get( slave_dpi_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_EN_get", A, 7);
    /* (0x00000000 bits 17) field ZONE_EN of register PMC_SLAVE_DPI_REG_ZONE_CONFIG */
    reg_value = slave_dpi_reg_ZONE_CONFIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_EN_MSK) >> SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_ZONE_EN_get", A, value );

    return value;
}
static INLINE void slave_dpi_field_ZONE_CFG_set( slave_dpi_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_ZONE_CFG_set( slave_dpi_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_CFG_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_ZONE_CFG_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_ZONE_CFG_set", A, value );

    /* (0x00000000 bits 16) field ZONE_CFG of register PMC_SLAVE_DPI_REG_ZONE_CONFIG */
    slave_dpi_reg_ZONE_CONFIG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_CFG_MSK,
                                         SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_CFG_OFF,
                                         value);
}

static INLINE UINT32 slave_dpi_field_ZONE_CFG_get( slave_dpi_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_ZONE_CFG_get( slave_dpi_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_CFG_get", A, 7);
    /* (0x00000000 bits 16) field ZONE_CFG of register PMC_SLAVE_DPI_REG_ZONE_CONFIG */
    reg_value = slave_dpi_reg_ZONE_CONFIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_CFG_MSK) >> SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_CFG_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_ZONE_CFG_get", A, value );

    return value;
}
static INLINE void slave_dpi_field_ZONE_MAX_CH_set( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_ZONE_MAX_CH_set( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_MAX_CH_set", A, 7);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_ZONE_MAX_CH_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_ZONE_MAX_CH_set", A, value );

    /* (0x00000000 bits 7:0) field ZONE_MAX_CH of register PMC_SLAVE_DPI_REG_ZONE_CONFIG */
    slave_dpi_reg_ZONE_CONFIG_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_MAX_CH_MSK,
                                         SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_MAX_CH_OFF,
                                         value);
}

static INLINE UINT32 slave_dpi_field_ZONE_MAX_CH_get( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_ZONE_MAX_CH_get( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_MAX_CH_get", A, 7);
    /* (0x00000000 bits 7:0) field ZONE_MAX_CH of register PMC_SLAVE_DPI_REG_ZONE_CONFIG */
    reg_value = slave_dpi_reg_ZONE_CONFIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_MAX_CH_MSK) >> SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_MAX_CH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_ZONE_MAX_CH_get", A, value );

    return value;
}
static INLINE void slave_dpi_field_range_ZONE_MAX_CH_set( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_range_ZONE_MAX_CH_set( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_ZONE_MAX_CH_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_ZONE_MAX_CH_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_ZONE_MAX_CH_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_ZONE_MAX_CH_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000000 bits 7:0) field ZONE_MAX_CH of register PMC_SLAVE_DPI_REG_ZONE_CONFIG */
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
        /* (0x00000000 bits 7:0) field ZONE_MAX_CH of register PMC_SLAVE_DPI_REG_ZONE_CONFIG */
        slave_dpi_reg_ZONE_CONFIG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             subfield_mask << (SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_MAX_CH_OFF + subfield_offset),
                                             SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_MAX_CH_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 slave_dpi_field_range_ZONE_MAX_CH_get( slave_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_ZONE_MAX_CH_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_ZONE_MAX_CH_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_ZONE_MAX_CH_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_ZONE_MAX_CH_get", stop_bit, 7 );
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
    /* (0x00000000 bits 7:0) field ZONE_MAX_CH of register PMC_SLAVE_DPI_REG_ZONE_CONFIG */
    reg_value = slave_dpi_reg_ZONE_CONFIG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_MAX_CH_MSK)
                  >> SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_MAX_CH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_MAX_CH_MSK, SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_MAX_CH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_ZONE_MAX_CH_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void slave_dpi_field_BURST_EN_set( slave_dpi_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_BURST_EN_set( slave_dpi_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_BURST_EN_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_BURST_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_BURST_EN_set", A, value );

    /* (0x00000010 bits 0) field BURST_EN of register PMC_SLAVE_DPI_REG_BURST_CONFIG */
    slave_dpi_reg_BURST_CONFIG_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          SLAVE_DPI_REG_BURST_CONFIG_BIT_BURST_EN_MSK,
                                          SLAVE_DPI_REG_BURST_CONFIG_BIT_BURST_EN_OFF,
                                          value);
}

static INLINE UINT32 slave_dpi_field_BURST_EN_get( slave_dpi_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_BURST_EN_get( slave_dpi_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_BURST_EN_get", A, 7);
    /* (0x00000010 bits 0) field BURST_EN of register PMC_SLAVE_DPI_REG_BURST_CONFIG */
    reg_value = slave_dpi_reg_BURST_CONFIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_BURST_CONFIG_BIT_BURST_EN_MSK) >> SLAVE_DPI_REG_BURST_CONFIG_BIT_BURST_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_BURST_EN_get", A, value );

    return value;
}
static INLINE void slave_dpi_field_HDR_SIZE_set( slave_dpi_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_HDR_SIZE_set( slave_dpi_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_HDR_SIZE_set", A, 7);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_HDR_SIZE_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_HDR_SIZE_set", A, value );

    /* (0x00000020 bits 5:0) field HDR_SIZE of register PMC_SLAVE_DPI_REG_HRD_CONFIG */
    slave_dpi_reg_HRD_CONFIG_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        SLAVE_DPI_REG_HRD_CONFIG_BIT_HDR_SIZE_MSK,
                                        SLAVE_DPI_REG_HRD_CONFIG_BIT_HDR_SIZE_OFF,
                                        value);
}

static INLINE UINT32 slave_dpi_field_HDR_SIZE_get( slave_dpi_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_HDR_SIZE_get( slave_dpi_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_HDR_SIZE_get", A, 7);
    /* (0x00000020 bits 5:0) field HDR_SIZE of register PMC_SLAVE_DPI_REG_HRD_CONFIG */
    reg_value = slave_dpi_reg_HRD_CONFIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_HRD_CONFIG_BIT_HDR_SIZE_MSK) >> SLAVE_DPI_REG_HRD_CONFIG_BIT_HDR_SIZE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_HDR_SIZE_get", A, value );

    return value;
}
static INLINE void slave_dpi_field_range_HDR_SIZE_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_range_HDR_SIZE_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_HDR_SIZE_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_HDR_SIZE_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_HDR_SIZE_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_HDR_SIZE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000020 bits 5:0) field HDR_SIZE of register PMC_SLAVE_DPI_REG_HRD_CONFIG */
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
        /* (0x00000020 bits 5:0) field HDR_SIZE of register PMC_SLAVE_DPI_REG_HRD_CONFIG */
        slave_dpi_reg_HRD_CONFIG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            subfield_mask << (SLAVE_DPI_REG_HRD_CONFIG_BIT_HDR_SIZE_OFF + subfield_offset),
                                            SLAVE_DPI_REG_HRD_CONFIG_BIT_HDR_SIZE_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 slave_dpi_field_range_HDR_SIZE_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_HDR_SIZE_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_HDR_SIZE_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_HDR_SIZE_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_HDR_SIZE_get", stop_bit, 5 );
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
    /* (0x00000020 bits 5:0) field HDR_SIZE of register PMC_SLAVE_DPI_REG_HRD_CONFIG */
    reg_value = slave_dpi_reg_HRD_CONFIG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & SLAVE_DPI_REG_HRD_CONFIG_BIT_HDR_SIZE_MSK)
                  >> SLAVE_DPI_REG_HRD_CONFIG_BIT_HDR_SIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_HRD_CONFIG_BIT_HDR_SIZE_MSK, SLAVE_DPI_REG_HRD_CONFIG_BIT_HDR_SIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_HDR_SIZE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void slave_dpi_field_PMON_CNT3_RST_set( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT3_RST_set( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT3_RST_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT3_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_PMON_CNT3_RST_set", A, value );

    /* (0x00000100 bits 25) field PMON_CNT3_RST of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    slave_dpi_reg_PMON_CNT_TRIG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT3_RST_MSK,
                                           SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT3_RST_OFF,
                                           value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT3_RST_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT3_RST_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT3_RST_get", A, 7);
    /* (0x00000100 bits 25) field PMON_CNT3_RST of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    reg_value = slave_dpi_reg_PMON_CNT_TRIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT3_RST_MSK) >> SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT3_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT3_RST_get", A, value );

    return value;
}
static INLINE void slave_dpi_field_PMON_CNT2_RST_set( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT2_RST_set( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT2_RST_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT2_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_PMON_CNT2_RST_set", A, value );

    /* (0x00000100 bits 17) field PMON_CNT2_RST of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    slave_dpi_reg_PMON_CNT_TRIG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT2_RST_MSK,
                                           SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT2_RST_OFF,
                                           value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT2_RST_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT2_RST_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT2_RST_get", A, 7);
    /* (0x00000100 bits 17) field PMON_CNT2_RST of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    reg_value = slave_dpi_reg_PMON_CNT_TRIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT2_RST_MSK) >> SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT2_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT2_RST_get", A, value );

    return value;
}
static INLINE void slave_dpi_field_PMON_CNT1_RST_set( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT1_RST_set( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT1_RST_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT1_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_PMON_CNT1_RST_set", A, value );

    /* (0x00000100 bits 9) field PMON_CNT1_RST of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    slave_dpi_reg_PMON_CNT_TRIG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT1_RST_MSK,
                                           SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT1_RST_OFF,
                                           value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT1_RST_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT1_RST_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT1_RST_get", A, 7);
    /* (0x00000100 bits 9) field PMON_CNT1_RST of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    reg_value = slave_dpi_reg_PMON_CNT_TRIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT1_RST_MSK) >> SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT1_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT1_RST_get", A, value );

    return value;
}
static INLINE void slave_dpi_field_PMON_CNT0_RST_set( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT0_RST_set( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT0_RST_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT0_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_PMON_CNT0_RST_set", A, value );

    /* (0x00000100 bits 1) field PMON_CNT0_RST of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    slave_dpi_reg_PMON_CNT_TRIG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT0_RST_MSK,
                                           SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT0_RST_OFF,
                                           value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT0_RST_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT0_RST_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT0_RST_get", A, 7);
    /* (0x00000100 bits 1) field PMON_CNT0_RST of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    reg_value = slave_dpi_reg_PMON_CNT_TRIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT0_RST_MSK) >> SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT0_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT0_RST_get", A, value );

    return value;
}
static INLINE void slave_dpi_field_PMON_CNT3_MODE_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT3_MODE_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT3_MODE_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT3_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_PMON_CNT3_MODE_set", A, value );

    /* (0x00000104 bits 24) field PMON_CNT3_MODE of register PMC_SLAVE_DPI_REG_PMON_MODE_TRIG */
    slave_dpi_reg_PMON_MODE_TRIG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT3_MODE_MSK,
                                            SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT3_MODE_OFF,
                                            value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT3_MODE_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT3_MODE_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT3_MODE_get", A, 7);
    /* (0x00000104 bits 24) field PMON_CNT3_MODE of register PMC_SLAVE_DPI_REG_PMON_MODE_TRIG */
    reg_value = slave_dpi_reg_PMON_MODE_TRIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT3_MODE_MSK) >> SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT3_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT3_MODE_get", A, value );

    return value;
}
static INLINE void slave_dpi_field_PMON_CNT2_MODE_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT2_MODE_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT2_MODE_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT2_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_PMON_CNT2_MODE_set", A, value );

    /* (0x00000104 bits 16) field PMON_CNT2_MODE of register PMC_SLAVE_DPI_REG_PMON_MODE_TRIG */
    slave_dpi_reg_PMON_MODE_TRIG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT2_MODE_MSK,
                                            SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT2_MODE_OFF,
                                            value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT2_MODE_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT2_MODE_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT2_MODE_get", A, 7);
    /* (0x00000104 bits 16) field PMON_CNT2_MODE of register PMC_SLAVE_DPI_REG_PMON_MODE_TRIG */
    reg_value = slave_dpi_reg_PMON_MODE_TRIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT2_MODE_MSK) >> SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT2_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT2_MODE_get", A, value );

    return value;
}
static INLINE void slave_dpi_field_PMON_CNT1_MODE_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT1_MODE_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT1_MODE_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT1_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_PMON_CNT1_MODE_set", A, value );

    /* (0x00000104 bits 8) field PMON_CNT1_MODE of register PMC_SLAVE_DPI_REG_PMON_MODE_TRIG */
    slave_dpi_reg_PMON_MODE_TRIG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT1_MODE_MSK,
                                            SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT1_MODE_OFF,
                                            value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT1_MODE_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT1_MODE_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT1_MODE_get", A, 7);
    /* (0x00000104 bits 8) field PMON_CNT1_MODE of register PMC_SLAVE_DPI_REG_PMON_MODE_TRIG */
    reg_value = slave_dpi_reg_PMON_MODE_TRIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT1_MODE_MSK) >> SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT1_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT1_MODE_get", A, value );

    return value;
}
static INLINE void slave_dpi_field_PMON_CNT0_MODE_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT0_MODE_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT0_MODE_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT0_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_PMON_CNT0_MODE_set", A, value );

    /* (0x00000104 bits 0) field PMON_CNT0_MODE of register PMC_SLAVE_DPI_REG_PMON_MODE_TRIG */
    slave_dpi_reg_PMON_MODE_TRIG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT0_MODE_MSK,
                                            SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT0_MODE_OFF,
                                            value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT0_MODE_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT0_MODE_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT0_MODE_get", A, 7);
    /* (0x00000104 bits 0) field PMON_CNT0_MODE of register PMC_SLAVE_DPI_REG_PMON_MODE_TRIG */
    reg_value = slave_dpi_reg_PMON_MODE_TRIG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT0_MODE_MSK) >> SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT0_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT0_MODE_get", A, value );

    return value;
}
static INLINE void slave_dpi_field_PMON_CNT3_PKT_FILT_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT3_PKT_FILT_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT3_PKT_FILT_set", A, 7);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT3_PKT_FILT_set", value, 31);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_PMON_CNT3_PKT_FILT_set", A, value );

    /* (0x00000108 bits 28:24) bits 0:4 use field PMON_CNT3_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
    slave_dpi_reg_PMON_PKT_TRIG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT3_PKT_FILT_MSK,
                                           SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT3_PKT_FILT_OFF,
                                           value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT3_PKT_FILT_get( slave_dpi_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT3_PKT_FILT_get( slave_dpi_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT3_PKT_FILT_get", A, 7);
    /* (0x00000108 bits 28:24) bits 0:4 use field PMON_CNT3_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
    reg_value = slave_dpi_reg_PMON_PKT_TRIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT3_PKT_FILT_MSK) >> SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT3_PKT_FILT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT3_PKT_FILT_get", A, value );

    return value;
}
static INLINE void slave_dpi_field_range_PMON_CNT3_PKT_FILT_set( slave_dpi_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_range_PMON_CNT3_PKT_FILT_set( slave_dpi_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_PMON_CNT3_PKT_FILT_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_PMON_CNT3_PKT_FILT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_PMON_CNT3_PKT_FILT_set", stop_bit, 4 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_PMON_CNT3_PKT_FILT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000108 bits 28:24) bits 0:4 use field PMON_CNT3_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
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
        /* (0x00000108 bits 28:24) bits 0:4 use field PMON_CNT3_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
        slave_dpi_reg_PMON_PKT_TRIG_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               subfield_mask << (SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT3_PKT_FILT_OFF + subfield_offset),
                                               SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT3_PKT_FILT_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 slave_dpi_field_range_PMON_CNT3_PKT_FILT_get( slave_dpi_buffer_t *b_ptr,
                                                                   cpb_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_PMON_CNT3_PKT_FILT_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_PMON_CNT3_PKT_FILT_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_PMON_CNT3_PKT_FILT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_PMON_CNT3_PKT_FILT_get", stop_bit, 4 );
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
    /* (0x00000108 bits 28:24) bits 0:4 use field PMON_CNT3_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
    reg_value = slave_dpi_reg_PMON_PKT_TRIG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT3_PKT_FILT_MSK)
                  >> SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT3_PKT_FILT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT3_PKT_FILT_MSK, SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT3_PKT_FILT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_PMON_CNT3_PKT_FILT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void slave_dpi_field_PMON_CNT2_PKT_FILT_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT2_PKT_FILT_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT2_PKT_FILT_set", A, 7);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT2_PKT_FILT_set", value, 31);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_PMON_CNT2_PKT_FILT_set", A, value );

    /* (0x00000108 bits 20:16) bits 0:4 use field PMON_CNT2_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
    slave_dpi_reg_PMON_PKT_TRIG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT2_PKT_FILT_MSK,
                                           SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT2_PKT_FILT_OFF,
                                           value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT2_PKT_FILT_get( slave_dpi_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT2_PKT_FILT_get( slave_dpi_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT2_PKT_FILT_get", A, 7);
    /* (0x00000108 bits 20:16) bits 0:4 use field PMON_CNT2_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
    reg_value = slave_dpi_reg_PMON_PKT_TRIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT2_PKT_FILT_MSK) >> SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT2_PKT_FILT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT2_PKT_FILT_get", A, value );

    return value;
}
static INLINE void slave_dpi_field_range_PMON_CNT2_PKT_FILT_set( slave_dpi_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_range_PMON_CNT2_PKT_FILT_set( slave_dpi_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_PMON_CNT2_PKT_FILT_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_PMON_CNT2_PKT_FILT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_PMON_CNT2_PKT_FILT_set", stop_bit, 4 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_PMON_CNT2_PKT_FILT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000108 bits 20:16) bits 0:4 use field PMON_CNT2_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
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
        /* (0x00000108 bits 20:16) bits 0:4 use field PMON_CNT2_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
        slave_dpi_reg_PMON_PKT_TRIG_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               subfield_mask << (SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT2_PKT_FILT_OFF + subfield_offset),
                                               SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT2_PKT_FILT_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 slave_dpi_field_range_PMON_CNT2_PKT_FILT_get( slave_dpi_buffer_t *b_ptr,
                                                                   cpb_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_PMON_CNT2_PKT_FILT_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_PMON_CNT2_PKT_FILT_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_PMON_CNT2_PKT_FILT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_PMON_CNT2_PKT_FILT_get", stop_bit, 4 );
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
    /* (0x00000108 bits 20:16) bits 0:4 use field PMON_CNT2_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
    reg_value = slave_dpi_reg_PMON_PKT_TRIG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT2_PKT_FILT_MSK)
                  >> SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT2_PKT_FILT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT2_PKT_FILT_MSK, SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT2_PKT_FILT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_PMON_CNT2_PKT_FILT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void slave_dpi_field_PMON_CNT1_PKT_FILT_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT1_PKT_FILT_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT1_PKT_FILT_set", A, 7);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT1_PKT_FILT_set", value, 31);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_PMON_CNT1_PKT_FILT_set", A, value );

    /* (0x00000108 bits 12:8) bits 0:4 use field PMON_CNT1_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
    slave_dpi_reg_PMON_PKT_TRIG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT1_PKT_FILT_MSK,
                                           SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT1_PKT_FILT_OFF,
                                           value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT1_PKT_FILT_get( slave_dpi_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT1_PKT_FILT_get( slave_dpi_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT1_PKT_FILT_get", A, 7);
    /* (0x00000108 bits 12:8) bits 0:4 use field PMON_CNT1_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
    reg_value = slave_dpi_reg_PMON_PKT_TRIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT1_PKT_FILT_MSK) >> SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT1_PKT_FILT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT1_PKT_FILT_get", A, value );

    return value;
}
static INLINE void slave_dpi_field_range_PMON_CNT1_PKT_FILT_set( slave_dpi_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_range_PMON_CNT1_PKT_FILT_set( slave_dpi_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_PMON_CNT1_PKT_FILT_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_PMON_CNT1_PKT_FILT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_PMON_CNT1_PKT_FILT_set", stop_bit, 4 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_PMON_CNT1_PKT_FILT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000108 bits 12:8) bits 0:4 use field PMON_CNT1_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
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
        /* (0x00000108 bits 12:8) bits 0:4 use field PMON_CNT1_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
        slave_dpi_reg_PMON_PKT_TRIG_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               subfield_mask << (SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT1_PKT_FILT_OFF + subfield_offset),
                                               SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT1_PKT_FILT_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 slave_dpi_field_range_PMON_CNT1_PKT_FILT_get( slave_dpi_buffer_t *b_ptr,
                                                                   cpb_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_PMON_CNT1_PKT_FILT_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_PMON_CNT1_PKT_FILT_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_PMON_CNT1_PKT_FILT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_PMON_CNT1_PKT_FILT_get", stop_bit, 4 );
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
    /* (0x00000108 bits 12:8) bits 0:4 use field PMON_CNT1_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
    reg_value = slave_dpi_reg_PMON_PKT_TRIG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT1_PKT_FILT_MSK)
                  >> SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT1_PKT_FILT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT1_PKT_FILT_MSK, SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT1_PKT_FILT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_PMON_CNT1_PKT_FILT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void slave_dpi_field_PMON_CNT0_PKT_FILT_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT0_PKT_FILT_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT0_PKT_FILT_set", A, 7);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT0_PKT_FILT_set", value, 31);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_PMON_CNT0_PKT_FILT_set", A, value );

    /* (0x00000108 bits 4:0) bits 0:4 use field PMON_CNT0_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
    slave_dpi_reg_PMON_PKT_TRIG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT0_PKT_FILT_MSK,
                                           SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT0_PKT_FILT_OFF,
                                           value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT0_PKT_FILT_get( slave_dpi_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT0_PKT_FILT_get( slave_dpi_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT0_PKT_FILT_get", A, 7);
    /* (0x00000108 bits 4:0) bits 0:4 use field PMON_CNT0_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
    reg_value = slave_dpi_reg_PMON_PKT_TRIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT0_PKT_FILT_MSK) >> SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT0_PKT_FILT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT0_PKT_FILT_get", A, value );

    return value;
}
static INLINE void slave_dpi_field_range_PMON_CNT0_PKT_FILT_set( slave_dpi_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_range_PMON_CNT0_PKT_FILT_set( slave_dpi_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_PMON_CNT0_PKT_FILT_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_PMON_CNT0_PKT_FILT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_PMON_CNT0_PKT_FILT_set", stop_bit, 4 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_PMON_CNT0_PKT_FILT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000108 bits 4:0) bits 0:4 use field PMON_CNT0_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
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
        /* (0x00000108 bits 4:0) bits 0:4 use field PMON_CNT0_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
        slave_dpi_reg_PMON_PKT_TRIG_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               subfield_mask << (SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT0_PKT_FILT_OFF + subfield_offset),
                                               SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT0_PKT_FILT_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 slave_dpi_field_range_PMON_CNT0_PKT_FILT_get( slave_dpi_buffer_t *b_ptr,
                                                                   cpb_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_PMON_CNT0_PKT_FILT_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_PMON_CNT0_PKT_FILT_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_PMON_CNT0_PKT_FILT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_PMON_CNT0_PKT_FILT_get", stop_bit, 4 );
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
    /* (0x00000108 bits 4:0) bits 0:4 use field PMON_CNT0_PKT_FILT of register PMC_SLAVE_DPI_REG_PMON_PKT_TRIG */
    reg_value = slave_dpi_reg_PMON_PKT_TRIG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT0_PKT_FILT_MSK)
                  >> SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT0_PKT_FILT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT0_PKT_FILT_MSK, SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT0_PKT_FILT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_PMON_CNT0_PKT_FILT_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size150
 * ==================================================================================
 */
static INLINE void slave_dpi_field_HDR_EN_set( slave_dpi_buffer_t *b_ptr,
                                               cpb_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_HDR_EN_set( slave_dpi_buffer_t *b_ptr,
                                               cpb_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32  N,
                                               UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_HDR_EN_set", A, 7);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_HDR_EN_set", N, 149);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_HDR_EN_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_HDR_EN_set", A, N, value );

    /* ((0x00000400 + (N) * 0x4) bits 8) field HDR_EN of register PMC_SLAVE_DPI_REG_CH_ENABLE index N=0..149 */
    slave_dpi_reg_CH_ENABLE_array_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             N,
                                             SLAVE_DPI_REG_CH_ENABLE_BIT_HDR_EN_MSK,
                                             SLAVE_DPI_REG_CH_ENABLE_BIT_HDR_EN_OFF,
                                             value);
}

static INLINE UINT32 slave_dpi_field_HDR_EN_get( slave_dpi_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_HDR_EN_get( slave_dpi_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_HDR_EN_get", A, 7);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_HDR_EN_get", N, 149);
    /* ((0x00000400 + (N) * 0x4) bits 8) field HDR_EN of register PMC_SLAVE_DPI_REG_CH_ENABLE index N=0..149 */
    reg_value = slave_dpi_reg_CH_ENABLE_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    value = (reg_value & SLAVE_DPI_REG_CH_ENABLE_BIT_HDR_EN_MSK) >> SLAVE_DPI_REG_CH_ENABLE_BIT_HDR_EN_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_HDR_EN_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_QS3_CH_EN_set( slave_dpi_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_QS3_CH_EN_set( slave_dpi_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS3_CH_EN_set", A, 7);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS3_CH_EN_set", N, 149);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_QS3_CH_EN_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_QS3_CH_EN_set", A, N, value );

    /* ((0x00000400 + (N) * 0x4) bits 3) field QS3_CH_EN of register PMC_SLAVE_DPI_REG_CH_ENABLE index N=0..149 */
    slave_dpi_reg_CH_ENABLE_array_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             N,
                                             SLAVE_DPI_REG_CH_ENABLE_BIT_QS3_CH_EN_MSK,
                                             SLAVE_DPI_REG_CH_ENABLE_BIT_QS3_CH_EN_OFF,
                                             value);
}

static INLINE UINT32 slave_dpi_field_QS3_CH_EN_get( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_QS3_CH_EN_get( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS3_CH_EN_get", A, 7);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS3_CH_EN_get", N, 149);
    /* ((0x00000400 + (N) * 0x4) bits 3) field QS3_CH_EN of register PMC_SLAVE_DPI_REG_CH_ENABLE index N=0..149 */
    reg_value = slave_dpi_reg_CH_ENABLE_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    value = (reg_value & SLAVE_DPI_REG_CH_ENABLE_BIT_QS3_CH_EN_MSK) >> SLAVE_DPI_REG_CH_ENABLE_BIT_QS3_CH_EN_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_QS3_CH_EN_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_QS2_CH_EN_set( slave_dpi_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_QS2_CH_EN_set( slave_dpi_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS2_CH_EN_set", A, 7);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS2_CH_EN_set", N, 149);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_QS2_CH_EN_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_QS2_CH_EN_set", A, N, value );

    /* ((0x00000400 + (N) * 0x4) bits 2) field QS2_CH_EN of register PMC_SLAVE_DPI_REG_CH_ENABLE index N=0..149 */
    slave_dpi_reg_CH_ENABLE_array_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             N,
                                             SLAVE_DPI_REG_CH_ENABLE_BIT_QS2_CH_EN_MSK,
                                             SLAVE_DPI_REG_CH_ENABLE_BIT_QS2_CH_EN_OFF,
                                             value);
}

static INLINE UINT32 slave_dpi_field_QS2_CH_EN_get( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_QS2_CH_EN_get( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS2_CH_EN_get", A, 7);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS2_CH_EN_get", N, 149);
    /* ((0x00000400 + (N) * 0x4) bits 2) field QS2_CH_EN of register PMC_SLAVE_DPI_REG_CH_ENABLE index N=0..149 */
    reg_value = slave_dpi_reg_CH_ENABLE_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    value = (reg_value & SLAVE_DPI_REG_CH_ENABLE_BIT_QS2_CH_EN_MSK) >> SLAVE_DPI_REG_CH_ENABLE_BIT_QS2_CH_EN_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_QS2_CH_EN_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_QS1_CH_EN_set( slave_dpi_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_QS1_CH_EN_set( slave_dpi_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS1_CH_EN_set", A, 7);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS1_CH_EN_set", N, 149);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_QS1_CH_EN_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_QS1_CH_EN_set", A, N, value );

    /* ((0x00000400 + (N) * 0x4) bits 1) field QS1_CH_EN of register PMC_SLAVE_DPI_REG_CH_ENABLE index N=0..149 */
    slave_dpi_reg_CH_ENABLE_array_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             N,
                                             SLAVE_DPI_REG_CH_ENABLE_BIT_QS1_CH_EN_MSK,
                                             SLAVE_DPI_REG_CH_ENABLE_BIT_QS1_CH_EN_OFF,
                                             value);
}

static INLINE UINT32 slave_dpi_field_QS1_CH_EN_get( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_QS1_CH_EN_get( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS1_CH_EN_get", A, 7);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS1_CH_EN_get", N, 149);
    /* ((0x00000400 + (N) * 0x4) bits 1) field QS1_CH_EN of register PMC_SLAVE_DPI_REG_CH_ENABLE index N=0..149 */
    reg_value = slave_dpi_reg_CH_ENABLE_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    value = (reg_value & SLAVE_DPI_REG_CH_ENABLE_BIT_QS1_CH_EN_MSK) >> SLAVE_DPI_REG_CH_ENABLE_BIT_QS1_CH_EN_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_QS1_CH_EN_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_QS0_CH_EN_set( slave_dpi_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_QS0_CH_EN_set( slave_dpi_buffer_t *b_ptr,
                                                  cpb_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS0_CH_EN_set", A, 7);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS0_CH_EN_set", N, 149);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_QS0_CH_EN_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_QS0_CH_EN_set", A, N, value );

    /* ((0x00000400 + (N) * 0x4) bits 0) field QS0_CH_EN of register PMC_SLAVE_DPI_REG_CH_ENABLE index N=0..149 */
    slave_dpi_reg_CH_ENABLE_array_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             N,
                                             SLAVE_DPI_REG_CH_ENABLE_BIT_QS0_CH_EN_MSK,
                                             SLAVE_DPI_REG_CH_ENABLE_BIT_QS0_CH_EN_OFF,
                                             value);
}

static INLINE UINT32 slave_dpi_field_QS0_CH_EN_get( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_QS0_CH_EN_get( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS0_CH_EN_get", A, 7);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS0_CH_EN_get", N, 149);
    /* ((0x00000400 + (N) * 0x4) bits 0) field QS0_CH_EN of register PMC_SLAVE_DPI_REG_CH_ENABLE index N=0..149 */
    reg_value = slave_dpi_reg_CH_ENABLE_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    value = (reg_value & SLAVE_DPI_REG_CH_ENABLE_BIT_QS0_CH_EN_MSK) >> SLAVE_DPI_REG_CH_ENABLE_BIT_QS0_CH_EN_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_QS0_CH_EN_get", A, N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void slave_dpi_field_CH_DROP_CNT_TRIG_set( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_CH_DROP_CNT_TRIG_set( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_CH_DROP_CNT_TRIG_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_CH_DROP_CNT_TRIG_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_CH_DROP_CNT_TRIG_set", A, value );

    /* (0x00000084 bits 0) field CH_DROP_CNT_TRIG of register PMC_SLAVE_DPI_REG_DROP_CNT_TRIG_REG */
    slave_dpi_reg_DROP_CNT_TRIG_REG_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               SLAVE_DPI_REG_DROP_CNT_TRIG_REG_BIT_CH_DROP_CNT_TRIG_MSK,
                                               SLAVE_DPI_REG_DROP_CNT_TRIG_REG_BIT_CH_DROP_CNT_TRIG_OFF,
                                               value);
}

static INLINE UINT32 slave_dpi_field_CH_DROP_CNT_TRIG_get( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_CH_DROP_CNT_TRIG_get( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_CH_DROP_CNT_TRIG_get", A, 7);
    /* (0x00000084 bits 0) field CH_DROP_CNT_TRIG of register PMC_SLAVE_DPI_REG_DROP_CNT_TRIG_REG */
    reg_value = slave_dpi_reg_DROP_CNT_TRIG_REG_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & SLAVE_DPI_REG_DROP_CNT_TRIG_REG_BIT_CH_DROP_CNT_TRIG_MSK) >> SLAVE_DPI_REG_DROP_CNT_TRIG_REG_BIT_CH_DROP_CNT_TRIG_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_CH_DROP_CNT_TRIG_get", A, value );

    return value;
}
static INLINE void slave_dpi_field_PMON_CNT3_TRIG_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT3_TRIG_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT3_TRIG_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT3_TRIG_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_PMON_CNT3_TRIG_set", A, value );

    /* (0x00000100 bits 24) field PMON_CNT3_TRIG of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    slave_dpi_reg_PMON_CNT_TRIG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT3_TRIG_MSK,
                                           SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT3_TRIG_OFF,
                                           value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT3_TRIG_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT3_TRIG_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT3_TRIG_get", A, 7);
    /* (0x00000100 bits 24) field PMON_CNT3_TRIG of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    reg_value = slave_dpi_reg_PMON_CNT_TRIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT3_TRIG_MSK) >> SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT3_TRIG_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT3_TRIG_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE slave_dpi_field_PMON_CNT3_TRIG_poll( slave_dpi_buffer_t *b_ptr,
                                                                        cpb_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE slave_dpi_field_PMON_CNT3_TRIG_poll( slave_dpi_buffer_t *b_ptr,
                                                                        cpb_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "slave_dpi_field_PMON_CNT3_TRIG_poll", A, value );

    /* (0x00000100 bits 24) field PMON_CNT3_TRIG of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    return slave_dpi_reg_PMON_CNT_TRIG_poll( b_ptr,
                                             h_ptr,
                                             A,
                                             SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT3_TRIG_MSK,
                                             (value<<SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT3_TRIG_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void slave_dpi_field_PMON_CNT2_TRIG_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT2_TRIG_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT2_TRIG_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT2_TRIG_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_PMON_CNT2_TRIG_set", A, value );

    /* (0x00000100 bits 16) field PMON_CNT2_TRIG of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    slave_dpi_reg_PMON_CNT_TRIG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT2_TRIG_MSK,
                                           SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT2_TRIG_OFF,
                                           value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT2_TRIG_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT2_TRIG_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT2_TRIG_get", A, 7);
    /* (0x00000100 bits 16) field PMON_CNT2_TRIG of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    reg_value = slave_dpi_reg_PMON_CNT_TRIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT2_TRIG_MSK) >> SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT2_TRIG_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT2_TRIG_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE slave_dpi_field_PMON_CNT2_TRIG_poll( slave_dpi_buffer_t *b_ptr,
                                                                        cpb_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE slave_dpi_field_PMON_CNT2_TRIG_poll( slave_dpi_buffer_t *b_ptr,
                                                                        cpb_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "slave_dpi_field_PMON_CNT2_TRIG_poll", A, value );

    /* (0x00000100 bits 16) field PMON_CNT2_TRIG of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    return slave_dpi_reg_PMON_CNT_TRIG_poll( b_ptr,
                                             h_ptr,
                                             A,
                                             SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT2_TRIG_MSK,
                                             (value<<SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT2_TRIG_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void slave_dpi_field_PMON_CNT1_TRIG_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT1_TRIG_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT1_TRIG_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT1_TRIG_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_PMON_CNT1_TRIG_set", A, value );

    /* (0x00000100 bits 8) field PMON_CNT1_TRIG of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    slave_dpi_reg_PMON_CNT_TRIG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT1_TRIG_MSK,
                                           SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT1_TRIG_OFF,
                                           value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT1_TRIG_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT1_TRIG_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT1_TRIG_get", A, 7);
    /* (0x00000100 bits 8) field PMON_CNT1_TRIG of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    reg_value = slave_dpi_reg_PMON_CNT_TRIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT1_TRIG_MSK) >> SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT1_TRIG_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT1_TRIG_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE slave_dpi_field_PMON_CNT1_TRIG_poll( slave_dpi_buffer_t *b_ptr,
                                                                        cpb_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE slave_dpi_field_PMON_CNT1_TRIG_poll( slave_dpi_buffer_t *b_ptr,
                                                                        cpb_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "slave_dpi_field_PMON_CNT1_TRIG_poll", A, value );

    /* (0x00000100 bits 8) field PMON_CNT1_TRIG of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    return slave_dpi_reg_PMON_CNT_TRIG_poll( b_ptr,
                                             h_ptr,
                                             A,
                                             SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT1_TRIG_MSK,
                                             (value<<SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT1_TRIG_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void slave_dpi_field_PMON_CNT0_TRIG_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT0_TRIG_set( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT0_TRIG_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT0_TRIG_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "slave_dpi_field_PMON_CNT0_TRIG_set", A, value );

    /* (0x00000100 bits 0) field PMON_CNT0_TRIG of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    slave_dpi_reg_PMON_CNT_TRIG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT0_TRIG_MSK,
                                           SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT0_TRIG_OFF,
                                           value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT0_TRIG_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT0_TRIG_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT0_TRIG_get", A, 7);
    /* (0x00000100 bits 0) field PMON_CNT0_TRIG of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    reg_value = slave_dpi_reg_PMON_CNT_TRIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT0_TRIG_MSK) >> SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT0_TRIG_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT0_TRIG_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE slave_dpi_field_PMON_CNT0_TRIG_poll( slave_dpi_buffer_t *b_ptr,
                                                                        cpb_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE slave_dpi_field_PMON_CNT0_TRIG_poll( slave_dpi_buffer_t *b_ptr,
                                                                        cpb_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "slave_dpi_field_PMON_CNT0_TRIG_poll", A, value );

    /* (0x00000100 bits 0) field PMON_CNT0_TRIG of register PMC_SLAVE_DPI_REG_PMON_CNT_TRIG */
    return slave_dpi_reg_PMON_CNT_TRIG_poll( b_ptr,
                                             h_ptr,
                                             A,
                                             SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT0_TRIG_MSK,
                                             (value<<SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT0_TRIG_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync_N_size150
 * ==================================================================================
 */
static INLINE void slave_dpi_field_CH_DROP_FULL_I_set_to_clear( slave_dpi_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_CH_DROP_FULL_I_set_to_clear( slave_dpi_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_CH_DROP_FULL_I_set_to_clear", A, 7);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_CH_DROP_FULL_I_set_to_clear", N, 149);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_CH_DROP_FULL_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_CH_DROP_FULL_I_set_to_clear", A, N, value );

    /* ((0x00002000 + (N) * 0x4) bits 16) field CH_DROP_FULL_I of register PMC_SLAVE_DPI_REG_CH_DROP_INT index N=0..149 */
    slave_dpi_reg_CH_DROP_INT_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               A,
                                                               N,
                                                               SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_FULL_I_MSK,
                                                               SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_FULL_I_OFF,
                                                               value);
}

static INLINE UINT32 slave_dpi_field_CH_DROP_FULL_I_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_CH_DROP_FULL_I_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_CH_DROP_FULL_I_get", A, 7);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_CH_DROP_FULL_I_get", N, 149);
    /* ((0x00002000 + (N) * 0x4) bits 16) field CH_DROP_FULL_I of register PMC_SLAVE_DPI_REG_CH_DROP_INT index N=0..149 */
    reg_value = slave_dpi_reg_CH_DROP_INT_array_read( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N);
    value = (reg_value & SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_FULL_I_MSK) >> SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_FULL_I_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_CH_DROP_FULL_I_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE slave_dpi_field_CH_DROP_FULL_I_poll( slave_dpi_buffer_t *b_ptr,
                                                                        cpb_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE slave_dpi_field_CH_DROP_FULL_I_poll( slave_dpi_buffer_t *b_ptr,
                                                                        cpb_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "slave_dpi_field_CH_DROP_FULL_I_poll", A, N, value );

    /* ((0x00002000 + (N) * 0x4) bits 16) field CH_DROP_FULL_I of register PMC_SLAVE_DPI_REG_CH_DROP_INT index N=0..149 */
    return slave_dpi_reg_CH_DROP_INT_array_poll( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_FULL_I_MSK,
                                                 (value<<SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_FULL_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void slave_dpi_field_CH_DROP_CLIENT_I_set_to_clear( slave_dpi_buffer_t *b_ptr,
                                                                  cpb_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32  N,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_CH_DROP_CLIENT_I_set_to_clear( slave_dpi_buffer_t *b_ptr,
                                                                  cpb_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32  N,
                                                                  UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_CH_DROP_CLIENT_I_set_to_clear", A, 7);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_CH_DROP_CLIENT_I_set_to_clear", N, 149);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_CH_DROP_CLIENT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_CH_DROP_CLIENT_I_set_to_clear", A, N, value );

    /* ((0x00002000 + (N) * 0x4) bits 8) field CH_DROP_CLIENT_I of register PMC_SLAVE_DPI_REG_CH_DROP_INT index N=0..149 */
    slave_dpi_reg_CH_DROP_INT_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               A,
                                                               N,
                                                               SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_CLIENT_I_MSK,
                                                               SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_CLIENT_I_OFF,
                                                               value);
}

static INLINE UINT32 slave_dpi_field_CH_DROP_CLIENT_I_get( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_CH_DROP_CLIENT_I_get( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_CH_DROP_CLIENT_I_get", A, 7);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_CH_DROP_CLIENT_I_get", N, 149);
    /* ((0x00002000 + (N) * 0x4) bits 8) field CH_DROP_CLIENT_I of register PMC_SLAVE_DPI_REG_CH_DROP_INT index N=0..149 */
    reg_value = slave_dpi_reg_CH_DROP_INT_array_read( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N);
    value = (reg_value & SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_CLIENT_I_MSK) >> SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_CLIENT_I_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_CH_DROP_CLIENT_I_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE slave_dpi_field_CH_DROP_CLIENT_I_poll( slave_dpi_buffer_t *b_ptr,
                                                                          cpb_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32  N,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE slave_dpi_field_CH_DROP_CLIENT_I_poll( slave_dpi_buffer_t *b_ptr,
                                                                          cpb_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32  N,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "slave_dpi_field_CH_DROP_CLIENT_I_poll", A, N, value );

    /* ((0x00002000 + (N) * 0x4) bits 8) field CH_DROP_CLIENT_I of register PMC_SLAVE_DPI_REG_CH_DROP_INT index N=0..149 */
    return slave_dpi_reg_CH_DROP_INT_array_poll( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_CLIENT_I_MSK,
                                                 (value<<SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_CLIENT_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void slave_dpi_field_CH_DROP_OVRSIZE_I_set_to_clear( slave_dpi_buffer_t *b_ptr,
                                                                   cpb_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32  N,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_CH_DROP_OVRSIZE_I_set_to_clear( slave_dpi_buffer_t *b_ptr,
                                                                   cpb_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32  N,
                                                                   UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_CH_DROP_OVRSIZE_I_set_to_clear", A, 7);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_CH_DROP_OVRSIZE_I_set_to_clear", N, 149);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_CH_DROP_OVRSIZE_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_CH_DROP_OVRSIZE_I_set_to_clear", A, N, value );

    /* ((0x00002000 + (N) * 0x4) bits 0) field CH_DROP_OVRSIZE_I of register PMC_SLAVE_DPI_REG_CH_DROP_INT index N=0..149 */
    slave_dpi_reg_CH_DROP_INT_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               A,
                                                               N,
                                                               SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_OVRSIZE_I_MSK,
                                                               SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_OVRSIZE_I_OFF,
                                                               value);
}

static INLINE UINT32 slave_dpi_field_CH_DROP_OVRSIZE_I_get( slave_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_CH_DROP_OVRSIZE_I_get( slave_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_CH_DROP_OVRSIZE_I_get", A, 7);
    if (N > 149)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_CH_DROP_OVRSIZE_I_get", N, 149);
    /* ((0x00002000 + (N) * 0x4) bits 0) field CH_DROP_OVRSIZE_I of register PMC_SLAVE_DPI_REG_CH_DROP_INT index N=0..149 */
    reg_value = slave_dpi_reg_CH_DROP_INT_array_read( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N);
    value = (reg_value & SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_OVRSIZE_I_MSK) >> SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_OVRSIZE_I_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_CH_DROP_OVRSIZE_I_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE slave_dpi_field_CH_DROP_OVRSIZE_I_poll( slave_dpi_buffer_t *b_ptr,
                                                                           cpb_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32  N,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE slave_dpi_field_CH_DROP_OVRSIZE_I_poll( slave_dpi_buffer_t *b_ptr,
                                                                           cpb_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32  N,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "slave_dpi_field_CH_DROP_OVRSIZE_I_poll", A, N, value );

    /* ((0x00002000 + (N) * 0x4) bits 0) field CH_DROP_OVRSIZE_I of register PMC_SLAVE_DPI_REG_CH_DROP_INT index N=0..149 */
    return slave_dpi_reg_CH_DROP_INT_array_poll( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_OVRSIZE_I_MSK,
                                                 (value<<SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_OVRSIZE_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_burstsize150
 * ==================================================================================
 */
static INLINE void slave_dpi_lfield_CH_DROP_CNT_VAL_set( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value[150] ) ALWAYS_INLINE;
static INLINE void slave_dpi_lfield_CH_DROP_CNT_VAL_set( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value[150] )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_lfield_CH_DROP_CNT_VAL_set", A, 7);
    IOLOG( "%s  A=%d", "slave_dpi_lfield_CH_DROP_CNT_VAL_set", A);
    slave_dpi_reg_CH_DROP_COUNT_array_burst_write( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   0,
                                                   150,
                                                   value);
}

static INLINE void slave_dpi_lfield_CH_DROP_CNT_VAL_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value[150] ) ALWAYS_INLINE;
static INLINE void slave_dpi_lfield_CH_DROP_CNT_VAL_get( slave_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value[150] )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_lfield_CH_DROP_CNT_VAL_get", A, 7);
    IOLOG( "%s  A=%d", "slave_dpi_lfield_CH_DROP_CNT_VAL_get", A);
    slave_dpi_reg_CH_DROP_COUNT_array_burst_read( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  0,
                                                  150,
                                                  (UINT32 *)value);
}

static INLINE void slave_dpi_lfield_range_CH_DROP_CNT_VAL_set( slave_dpi_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_lfield_range_CH_DROP_CNT_VAL_set( slave_dpi_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_lfield_range_CH_DROP_CNT_VAL_set", A, 7);
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "slave_dpi_lfield_range_CH_DROP_CNT_VAL_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d value=0x%08x", "slave_dpi_lfield_range_CH_DROP_CNT_VAL_set", A, start_bit, stop_bit, value);
    slave_dpi_reg_CH_DROP_COUNT_array_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 word_number,
                                                 mask,
                                                 field_ofs,
                                                 value);
}

static INLINE UINT32 slave_dpi_lfield_range_CH_DROP_CNT_VAL_get( slave_dpi_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_lfield_range_CH_DROP_CNT_VAL_get( slave_dpi_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_lfield_range_CH_DROP_CNT_VAL_get", A, 7);
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "slave_dpi_lfield_range_CH_DROP_CNT_VAL_get", start_bit, stop_bit );
    }
    reg_value = slave_dpi_reg_CH_DROP_COUNT_array_read( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d value=0x%08x", "slave_dpi_lfield_range_CH_DROP_CNT_VAL_get", A, start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size256
 * ==================================================================================
 */
static INLINE void slave_dpi_field_DCPB_IMUX_SEL_set( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_DCPB_IMUX_SEL_set( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_DCPB_IMUX_SEL_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_DCPB_IMUX_SEL_set", N, 255);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_DCPB_IMUX_SEL_set", value, 3);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_DCPB_IMUX_SEL_set", A, N, value );

    /* ((0x00000800 + (N) * 0x4) bits 17:16) field DCPB_IMUX_SEL of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
    slave_dpi_reg_CH_CONFIG_array_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             N,
                                             SLAVE_DPI_REG_CH_CONFIG_BIT_DCPB_IMUX_SEL_MSK,
                                             SLAVE_DPI_REG_CH_CONFIG_BIT_DCPB_IMUX_SEL_OFF,
                                             value);
}

static INLINE UINT32 slave_dpi_field_DCPB_IMUX_SEL_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_DCPB_IMUX_SEL_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_DCPB_IMUX_SEL_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_DCPB_IMUX_SEL_get", N, 255);
    /* ((0x00000800 + (N) * 0x4) bits 17:16) field DCPB_IMUX_SEL of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
    reg_value = slave_dpi_reg_CH_CONFIG_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    value = (reg_value & SLAVE_DPI_REG_CH_CONFIG_BIT_DCPB_IMUX_SEL_MSK) >> SLAVE_DPI_REG_CH_CONFIG_BIT_DCPB_IMUX_SEL_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_DCPB_IMUX_SEL_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_range_DCPB_IMUX_SEL_set( slave_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_range_DCPB_IMUX_SEL_set( slave_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_DCPB_IMUX_SEL_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_DCPB_IMUX_SEL_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_DCPB_IMUX_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_DCPB_IMUX_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_DCPB_IMUX_SEL_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000800 + (N) * 0x4) bits 17:16) field DCPB_IMUX_SEL of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
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
        /* ((0x00000800 + (N) * 0x4) bits 17:16) field DCPB_IMUX_SEL of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
        slave_dpi_reg_CH_CONFIG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 subfield_mask << (SLAVE_DPI_REG_CH_CONFIG_BIT_DCPB_IMUX_SEL_OFF + subfield_offset),
                                                 SLAVE_DPI_REG_CH_CONFIG_BIT_DCPB_IMUX_SEL_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 slave_dpi_field_range_DCPB_IMUX_SEL_get( slave_dpi_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_DCPB_IMUX_SEL_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_DCPB_IMUX_SEL_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_DCPB_IMUX_SEL_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_DCPB_IMUX_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_DCPB_IMUX_SEL_get", stop_bit, 1 );
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
    /* ((0x00000800 + (N) * 0x4) bits 17:16) field DCPB_IMUX_SEL of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
    reg_value = slave_dpi_reg_CH_CONFIG_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    field_value = (reg_value & SLAVE_DPI_REG_CH_CONFIG_BIT_DCPB_IMUX_SEL_MSK)
                  >> SLAVE_DPI_REG_CH_CONFIG_BIT_DCPB_IMUX_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_CH_CONFIG_BIT_DCPB_IMUX_SEL_MSK, SLAVE_DPI_REG_CH_CONFIG_BIT_DCPB_IMUX_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_DCPB_IMUX_SEL_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void slave_dpi_field_PMON_CNT3_EN_set( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT3_EN_set( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT3_EN_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_PMON_CNT3_EN_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT3_EN_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_PMON_CNT3_EN_set", A, N, value );

    /* ((0x00000800 + (N) * 0x4) bits 11) field PMON_CNT3_EN of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
    slave_dpi_reg_CH_CONFIG_array_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             N,
                                             SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT3_EN_MSK,
                                             SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT3_EN_OFF,
                                             value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT3_EN_get( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT3_EN_get( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT3_EN_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_PMON_CNT3_EN_get", N, 255);
    /* ((0x00000800 + (N) * 0x4) bits 11) field PMON_CNT3_EN of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
    reg_value = slave_dpi_reg_CH_CONFIG_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    value = (reg_value & SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT3_EN_MSK) >> SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT3_EN_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_PMON_CNT3_EN_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_PMON_CNT2_EN_set( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT2_EN_set( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT2_EN_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_PMON_CNT2_EN_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT2_EN_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_PMON_CNT2_EN_set", A, N, value );

    /* ((0x00000800 + (N) * 0x4) bits 10) field PMON_CNT2_EN of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
    slave_dpi_reg_CH_CONFIG_array_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             N,
                                             SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT2_EN_MSK,
                                             SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT2_EN_OFF,
                                             value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT2_EN_get( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT2_EN_get( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT2_EN_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_PMON_CNT2_EN_get", N, 255);
    /* ((0x00000800 + (N) * 0x4) bits 10) field PMON_CNT2_EN of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
    reg_value = slave_dpi_reg_CH_CONFIG_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    value = (reg_value & SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT2_EN_MSK) >> SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT2_EN_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_PMON_CNT2_EN_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_PMON_CNT1_EN_set( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT1_EN_set( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT1_EN_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_PMON_CNT1_EN_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT1_EN_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_PMON_CNT1_EN_set", A, N, value );

    /* ((0x00000800 + (N) * 0x4) bits 9) field PMON_CNT1_EN of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
    slave_dpi_reg_CH_CONFIG_array_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             N,
                                             SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT1_EN_MSK,
                                             SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT1_EN_OFF,
                                             value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT1_EN_get( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT1_EN_get( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT1_EN_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_PMON_CNT1_EN_get", N, 255);
    /* ((0x00000800 + (N) * 0x4) bits 9) field PMON_CNT1_EN of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
    reg_value = slave_dpi_reg_CH_CONFIG_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    value = (reg_value & SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT1_EN_MSK) >> SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT1_EN_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_PMON_CNT1_EN_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_PMON_CNT0_EN_set( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_PMON_CNT0_EN_set( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT0_EN_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_PMON_CNT0_EN_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_PMON_CNT0_EN_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_PMON_CNT0_EN_set", A, N, value );

    /* ((0x00000800 + (N) * 0x4) bits 8) field PMON_CNT0_EN of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
    slave_dpi_reg_CH_CONFIG_array_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             N,
                                             SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT0_EN_MSK,
                                             SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT0_EN_OFF,
                                             value);
}

static INLINE UINT32 slave_dpi_field_PMON_CNT0_EN_get( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT0_EN_get( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT0_EN_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_PMON_CNT0_EN_get", N, 255);
    /* ((0x00000800 + (N) * 0x4) bits 8) field PMON_CNT0_EN of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
    reg_value = slave_dpi_reg_CH_CONFIG_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    value = (reg_value & SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT0_EN_MSK) >> SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT0_EN_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_PMON_CNT0_EN_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_QS3_CH_MST_set( slave_dpi_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_QS3_CH_MST_set( slave_dpi_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS3_CH_MST_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS3_CH_MST_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_QS3_CH_MST_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_QS3_CH_MST_set", A, N, value );

    /* ((0x00000800 + (N) * 0x4) bits 3) field QS3_CH_MST of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
    slave_dpi_reg_CH_CONFIG_array_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             N,
                                             SLAVE_DPI_REG_CH_CONFIG_BIT_QS3_CH_MST_MSK,
                                             SLAVE_DPI_REG_CH_CONFIG_BIT_QS3_CH_MST_OFF,
                                             value);
}

static INLINE UINT32 slave_dpi_field_QS3_CH_MST_get( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_QS3_CH_MST_get( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS3_CH_MST_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS3_CH_MST_get", N, 255);
    /* ((0x00000800 + (N) * 0x4) bits 3) field QS3_CH_MST of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
    reg_value = slave_dpi_reg_CH_CONFIG_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    value = (reg_value & SLAVE_DPI_REG_CH_CONFIG_BIT_QS3_CH_MST_MSK) >> SLAVE_DPI_REG_CH_CONFIG_BIT_QS3_CH_MST_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_QS3_CH_MST_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_QS2_CH_MST_set( slave_dpi_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_QS2_CH_MST_set( slave_dpi_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS2_CH_MST_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS2_CH_MST_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_QS2_CH_MST_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_QS2_CH_MST_set", A, N, value );

    /* ((0x00000800 + (N) * 0x4) bits 2) field QS2_CH_MST of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
    slave_dpi_reg_CH_CONFIG_array_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             N,
                                             SLAVE_DPI_REG_CH_CONFIG_BIT_QS2_CH_MST_MSK,
                                             SLAVE_DPI_REG_CH_CONFIG_BIT_QS2_CH_MST_OFF,
                                             value);
}

static INLINE UINT32 slave_dpi_field_QS2_CH_MST_get( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_QS2_CH_MST_get( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS2_CH_MST_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS2_CH_MST_get", N, 255);
    /* ((0x00000800 + (N) * 0x4) bits 2) field QS2_CH_MST of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
    reg_value = slave_dpi_reg_CH_CONFIG_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    value = (reg_value & SLAVE_DPI_REG_CH_CONFIG_BIT_QS2_CH_MST_MSK) >> SLAVE_DPI_REG_CH_CONFIG_BIT_QS2_CH_MST_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_QS2_CH_MST_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_QS1_CH_MST_set( slave_dpi_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_QS1_CH_MST_set( slave_dpi_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS1_CH_MST_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS1_CH_MST_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_QS1_CH_MST_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_QS1_CH_MST_set", A, N, value );

    /* ((0x00000800 + (N) * 0x4) bits 1) field QS1_CH_MST of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
    slave_dpi_reg_CH_CONFIG_array_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             N,
                                             SLAVE_DPI_REG_CH_CONFIG_BIT_QS1_CH_MST_MSK,
                                             SLAVE_DPI_REG_CH_CONFIG_BIT_QS1_CH_MST_OFF,
                                             value);
}

static INLINE UINT32 slave_dpi_field_QS1_CH_MST_get( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_QS1_CH_MST_get( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS1_CH_MST_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS1_CH_MST_get", N, 255);
    /* ((0x00000800 + (N) * 0x4) bits 1) field QS1_CH_MST of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
    reg_value = slave_dpi_reg_CH_CONFIG_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    value = (reg_value & SLAVE_DPI_REG_CH_CONFIG_BIT_QS1_CH_MST_MSK) >> SLAVE_DPI_REG_CH_CONFIG_BIT_QS1_CH_MST_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_QS1_CH_MST_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_QS0_CH_MST_set( slave_dpi_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_QS0_CH_MST_set( slave_dpi_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS0_CH_MST_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS0_CH_MST_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_QS0_CH_MST_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_QS0_CH_MST_set", A, N, value );

    /* ((0x00000800 + (N) * 0x4) bits 0) field QS0_CH_MST of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
    slave_dpi_reg_CH_CONFIG_array_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             N,
                                             SLAVE_DPI_REG_CH_CONFIG_BIT_QS0_CH_MST_MSK,
                                             SLAVE_DPI_REG_CH_CONFIG_BIT_QS0_CH_MST_OFF,
                                             value);
}

static INLINE UINT32 slave_dpi_field_QS0_CH_MST_get( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_QS0_CH_MST_get( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS0_CH_MST_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS0_CH_MST_get", N, 255);
    /* ((0x00000800 + (N) * 0x4) bits 0) field QS0_CH_MST of register PMC_SLAVE_DPI_REG_CH_CONFIG index N=0..255 */
    reg_value = slave_dpi_reg_CH_CONFIG_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    value = (reg_value & SLAVE_DPI_REG_CH_CONFIG_BIT_QS0_CH_MST_MSK) >> SLAVE_DPI_REG_CH_CONFIG_BIT_QS0_CH_MST_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_QS0_CH_MST_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_QS3_POP_QID_set( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_QS3_POP_QID_set( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS3_POP_QID_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS3_POP_QID_set", N, 255);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_QS3_POP_QID_set", value, 127);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_QS3_POP_QID_set", A, N, value );

    /* ((0x00000c00 + (N) * 0x4) bits 30:24) field QS3_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
    slave_dpi_reg_CH_Q_MAP_array_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            N,
                                            SLAVE_DPI_REG_CH_Q_MAP_BIT_QS3_POP_QID_MSK,
                                            SLAVE_DPI_REG_CH_Q_MAP_BIT_QS3_POP_QID_OFF,
                                            value);
}

static INLINE UINT32 slave_dpi_field_QS3_POP_QID_get( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_QS3_POP_QID_get( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS3_POP_QID_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS3_POP_QID_get", N, 255);
    /* ((0x00000c00 + (N) * 0x4) bits 30:24) field QS3_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
    reg_value = slave_dpi_reg_CH_Q_MAP_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    value = (reg_value & SLAVE_DPI_REG_CH_Q_MAP_BIT_QS3_POP_QID_MSK) >> SLAVE_DPI_REG_CH_Q_MAP_BIT_QS3_POP_QID_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_QS3_POP_QID_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_range_QS3_POP_QID_set( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_range_QS3_POP_QID_set( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_QS3_POP_QID_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_QS3_POP_QID_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_QS3_POP_QID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_QS3_POP_QID_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_QS3_POP_QID_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000c00 + (N) * 0x4) bits 30:24) field QS3_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
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
        /* ((0x00000c00 + (N) * 0x4) bits 30:24) field QS3_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
        slave_dpi_reg_CH_Q_MAP_array_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                N,
                                                subfield_mask << (SLAVE_DPI_REG_CH_Q_MAP_BIT_QS3_POP_QID_OFF + subfield_offset),
                                                SLAVE_DPI_REG_CH_Q_MAP_BIT_QS3_POP_QID_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 slave_dpi_field_range_QS3_POP_QID_get( slave_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_QS3_POP_QID_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_QS3_POP_QID_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_QS3_POP_QID_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_QS3_POP_QID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_QS3_POP_QID_get", stop_bit, 6 );
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
    /* ((0x00000c00 + (N) * 0x4) bits 30:24) field QS3_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
    reg_value = slave_dpi_reg_CH_Q_MAP_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    field_value = (reg_value & SLAVE_DPI_REG_CH_Q_MAP_BIT_QS3_POP_QID_MSK)
                  >> SLAVE_DPI_REG_CH_Q_MAP_BIT_QS3_POP_QID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_CH_Q_MAP_BIT_QS3_POP_QID_MSK, SLAVE_DPI_REG_CH_Q_MAP_BIT_QS3_POP_QID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_QS3_POP_QID_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void slave_dpi_field_QS2_POP_QID_set( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_QS2_POP_QID_set( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS2_POP_QID_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS2_POP_QID_set", N, 255);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_QS2_POP_QID_set", value, 127);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_QS2_POP_QID_set", A, N, value );

    /* ((0x00000c00 + (N) * 0x4) bits 22:16) field QS2_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
    slave_dpi_reg_CH_Q_MAP_array_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            N,
                                            SLAVE_DPI_REG_CH_Q_MAP_BIT_QS2_POP_QID_MSK,
                                            SLAVE_DPI_REG_CH_Q_MAP_BIT_QS2_POP_QID_OFF,
                                            value);
}

static INLINE UINT32 slave_dpi_field_QS2_POP_QID_get( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_QS2_POP_QID_get( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS2_POP_QID_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS2_POP_QID_get", N, 255);
    /* ((0x00000c00 + (N) * 0x4) bits 22:16) field QS2_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
    reg_value = slave_dpi_reg_CH_Q_MAP_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    value = (reg_value & SLAVE_DPI_REG_CH_Q_MAP_BIT_QS2_POP_QID_MSK) >> SLAVE_DPI_REG_CH_Q_MAP_BIT_QS2_POP_QID_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_QS2_POP_QID_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_range_QS2_POP_QID_set( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_range_QS2_POP_QID_set( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_QS2_POP_QID_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_QS2_POP_QID_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_QS2_POP_QID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_QS2_POP_QID_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_QS2_POP_QID_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000c00 + (N) * 0x4) bits 22:16) field QS2_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
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
        /* ((0x00000c00 + (N) * 0x4) bits 22:16) field QS2_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
        slave_dpi_reg_CH_Q_MAP_array_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                N,
                                                subfield_mask << (SLAVE_DPI_REG_CH_Q_MAP_BIT_QS2_POP_QID_OFF + subfield_offset),
                                                SLAVE_DPI_REG_CH_Q_MAP_BIT_QS2_POP_QID_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 slave_dpi_field_range_QS2_POP_QID_get( slave_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_QS2_POP_QID_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_QS2_POP_QID_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_QS2_POP_QID_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_QS2_POP_QID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_QS2_POP_QID_get", stop_bit, 6 );
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
    /* ((0x00000c00 + (N) * 0x4) bits 22:16) field QS2_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
    reg_value = slave_dpi_reg_CH_Q_MAP_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    field_value = (reg_value & SLAVE_DPI_REG_CH_Q_MAP_BIT_QS2_POP_QID_MSK)
                  >> SLAVE_DPI_REG_CH_Q_MAP_BIT_QS2_POP_QID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_CH_Q_MAP_BIT_QS2_POP_QID_MSK, SLAVE_DPI_REG_CH_Q_MAP_BIT_QS2_POP_QID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_QS2_POP_QID_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void slave_dpi_field_QS1_POP_QID_set( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_QS1_POP_QID_set( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS1_POP_QID_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS1_POP_QID_set", N, 255);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_QS1_POP_QID_set", value, 127);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_QS1_POP_QID_set", A, N, value );

    /* ((0x00000c00 + (N) * 0x4) bits 14:8) field QS1_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
    slave_dpi_reg_CH_Q_MAP_array_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            N,
                                            SLAVE_DPI_REG_CH_Q_MAP_BIT_QS1_POP_QID_MSK,
                                            SLAVE_DPI_REG_CH_Q_MAP_BIT_QS1_POP_QID_OFF,
                                            value);
}

static INLINE UINT32 slave_dpi_field_QS1_POP_QID_get( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_QS1_POP_QID_get( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS1_POP_QID_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS1_POP_QID_get", N, 255);
    /* ((0x00000c00 + (N) * 0x4) bits 14:8) field QS1_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
    reg_value = slave_dpi_reg_CH_Q_MAP_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    value = (reg_value & SLAVE_DPI_REG_CH_Q_MAP_BIT_QS1_POP_QID_MSK) >> SLAVE_DPI_REG_CH_Q_MAP_BIT_QS1_POP_QID_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_QS1_POP_QID_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_range_QS1_POP_QID_set( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_range_QS1_POP_QID_set( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_QS1_POP_QID_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_QS1_POP_QID_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_QS1_POP_QID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_QS1_POP_QID_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_QS1_POP_QID_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000c00 + (N) * 0x4) bits 14:8) field QS1_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
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
        /* ((0x00000c00 + (N) * 0x4) bits 14:8) field QS1_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
        slave_dpi_reg_CH_Q_MAP_array_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                N,
                                                subfield_mask << (SLAVE_DPI_REG_CH_Q_MAP_BIT_QS1_POP_QID_OFF + subfield_offset),
                                                SLAVE_DPI_REG_CH_Q_MAP_BIT_QS1_POP_QID_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 slave_dpi_field_range_QS1_POP_QID_get( slave_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_QS1_POP_QID_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_QS1_POP_QID_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_QS1_POP_QID_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_QS1_POP_QID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_QS1_POP_QID_get", stop_bit, 6 );
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
    /* ((0x00000c00 + (N) * 0x4) bits 14:8) field QS1_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
    reg_value = slave_dpi_reg_CH_Q_MAP_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    field_value = (reg_value & SLAVE_DPI_REG_CH_Q_MAP_BIT_QS1_POP_QID_MSK)
                  >> SLAVE_DPI_REG_CH_Q_MAP_BIT_QS1_POP_QID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_CH_Q_MAP_BIT_QS1_POP_QID_MSK, SLAVE_DPI_REG_CH_Q_MAP_BIT_QS1_POP_QID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_QS1_POP_QID_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void slave_dpi_field_QS0_POP_QID_set( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_QS0_POP_QID_set( slave_dpi_buffer_t *b_ptr,
                                                    cpb_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS0_POP_QID_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS0_POP_QID_set", N, 255);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_QS0_POP_QID_set", value, 127);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_QS0_POP_QID_set", A, N, value );

    /* ((0x00000c00 + (N) * 0x4) bits 6:0) field QS0_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
    slave_dpi_reg_CH_Q_MAP_array_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            N,
                                            SLAVE_DPI_REG_CH_Q_MAP_BIT_QS0_POP_QID_MSK,
                                            SLAVE_DPI_REG_CH_Q_MAP_BIT_QS0_POP_QID_OFF,
                                            value);
}

static INLINE UINT32 slave_dpi_field_QS0_POP_QID_get( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_QS0_POP_QID_get( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_QS0_POP_QID_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_QS0_POP_QID_get", N, 255);
    /* ((0x00000c00 + (N) * 0x4) bits 6:0) field QS0_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
    reg_value = slave_dpi_reg_CH_Q_MAP_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    value = (reg_value & SLAVE_DPI_REG_CH_Q_MAP_BIT_QS0_POP_QID_MSK) >> SLAVE_DPI_REG_CH_Q_MAP_BIT_QS0_POP_QID_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_QS0_POP_QID_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_range_QS0_POP_QID_set( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_range_QS0_POP_QID_set( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_QS0_POP_QID_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_QS0_POP_QID_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_QS0_POP_QID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_QS0_POP_QID_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_QS0_POP_QID_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000c00 + (N) * 0x4) bits 6:0) field QS0_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
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
        /* ((0x00000c00 + (N) * 0x4) bits 6:0) field QS0_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
        slave_dpi_reg_CH_Q_MAP_array_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                N,
                                                subfield_mask << (SLAVE_DPI_REG_CH_Q_MAP_BIT_QS0_POP_QID_OFF + subfield_offset),
                                                SLAVE_DPI_REG_CH_Q_MAP_BIT_QS0_POP_QID_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 slave_dpi_field_range_QS0_POP_QID_get( slave_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_QS0_POP_QID_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_QS0_POP_QID_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_QS0_POP_QID_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_QS0_POP_QID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_QS0_POP_QID_get", stop_bit, 6 );
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
    /* ((0x00000c00 + (N) * 0x4) bits 6:0) field QS0_POP_QID of register PMC_SLAVE_DPI_REG_CH_Q_MAP index N=0..255 */
    reg_value = slave_dpi_reg_CH_Q_MAP_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    field_value = (reg_value & SLAVE_DPI_REG_CH_Q_MAP_BIT_QS0_POP_QID_MSK)
                  >> SLAVE_DPI_REG_CH_Q_MAP_BIT_QS0_POP_QID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_CH_Q_MAP_BIT_QS0_POP_QID_MSK, SLAVE_DPI_REG_CH_Q_MAP_BIT_QS0_POP_QID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_QS0_POP_QID_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void slave_dpi_field_ZONE_THR_0_1_set( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_ZONE_THR_0_1_set( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_THR_0_1_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_ZONE_THR_0_1_set", N, 255);
    if (value > 131071)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_ZONE_THR_0_1_set", value, 131071);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_ZONE_THR_0_1_set", A, N, value );

    /* ((0x00001000 + (N) * 0x4) bits 16:0) field ZONE_THR_0_1 of register PMC_SLAVE_DPI_REG_ZONE_0_1 index N=0..255 */
    slave_dpi_reg_ZONE_0_1_array_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            N,
                                            SLAVE_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_MSK,
                                            SLAVE_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_OFF,
                                            value);
}

static INLINE UINT32 slave_dpi_field_ZONE_THR_0_1_get( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_ZONE_THR_0_1_get( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_THR_0_1_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_ZONE_THR_0_1_get", N, 255);
    /* ((0x00001000 + (N) * 0x4) bits 16:0) field ZONE_THR_0_1 of register PMC_SLAVE_DPI_REG_ZONE_0_1 index N=0..255 */
    reg_value = slave_dpi_reg_ZONE_0_1_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    value = (reg_value & SLAVE_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_MSK) >> SLAVE_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_ZONE_THR_0_1_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_range_ZONE_THR_0_1_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_range_ZONE_THR_0_1_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_ZONE_THR_0_1_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_ZONE_THR_0_1_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_ZONE_THR_0_1_set", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_ZONE_THR_0_1_set", stop_bit, 16 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_ZONE_THR_0_1_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001000 + (N) * 0x4) bits 16:0) field ZONE_THR_0_1 of register PMC_SLAVE_DPI_REG_ZONE_0_1 index N=0..255 */
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
        /* ((0x00001000 + (N) * 0x4) bits 16:0) field ZONE_THR_0_1 of register PMC_SLAVE_DPI_REG_ZONE_0_1 index N=0..255 */
        slave_dpi_reg_ZONE_0_1_array_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                N,
                                                subfield_mask << (SLAVE_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_OFF + subfield_offset),
                                                SLAVE_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 slave_dpi_field_range_ZONE_THR_0_1_get( slave_dpi_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_ZONE_THR_0_1_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_ZONE_THR_0_1_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_ZONE_THR_0_1_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_ZONE_THR_0_1_get", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_ZONE_THR_0_1_get", stop_bit, 16 );
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
    /* ((0x00001000 + (N) * 0x4) bits 16:0) field ZONE_THR_0_1 of register PMC_SLAVE_DPI_REG_ZONE_0_1 index N=0..255 */
    reg_value = slave_dpi_reg_ZONE_0_1_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    field_value = (reg_value & SLAVE_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_MSK)
                  >> SLAVE_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_MSK, SLAVE_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_ZONE_THR_0_1_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void slave_dpi_field_ZONE_THR_1_2_set( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_ZONE_THR_1_2_set( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_THR_1_2_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_ZONE_THR_1_2_set", N, 255);
    if (value > 131071)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_ZONE_THR_1_2_set", value, 131071);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_ZONE_THR_1_2_set", A, N, value );

    /* ((0x00001400 + (N) * 0x4) bits 16:0) field ZONE_THR_1_2 of register PMC_SLAVE_DPI_REG_ZONE_1_2 index N=0..255 */
    slave_dpi_reg_ZONE_1_2_array_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            N,
                                            SLAVE_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_MSK,
                                            SLAVE_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_OFF,
                                            value);
}

static INLINE UINT32 slave_dpi_field_ZONE_THR_1_2_get( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_ZONE_THR_1_2_get( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_THR_1_2_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_ZONE_THR_1_2_get", N, 255);
    /* ((0x00001400 + (N) * 0x4) bits 16:0) field ZONE_THR_1_2 of register PMC_SLAVE_DPI_REG_ZONE_1_2 index N=0..255 */
    reg_value = slave_dpi_reg_ZONE_1_2_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    value = (reg_value & SLAVE_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_MSK) >> SLAVE_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_ZONE_THR_1_2_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_range_ZONE_THR_1_2_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_range_ZONE_THR_1_2_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_ZONE_THR_1_2_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_ZONE_THR_1_2_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_ZONE_THR_1_2_set", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_ZONE_THR_1_2_set", stop_bit, 16 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_ZONE_THR_1_2_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001400 + (N) * 0x4) bits 16:0) field ZONE_THR_1_2 of register PMC_SLAVE_DPI_REG_ZONE_1_2 index N=0..255 */
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
        /* ((0x00001400 + (N) * 0x4) bits 16:0) field ZONE_THR_1_2 of register PMC_SLAVE_DPI_REG_ZONE_1_2 index N=0..255 */
        slave_dpi_reg_ZONE_1_2_array_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                N,
                                                subfield_mask << (SLAVE_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_OFF + subfield_offset),
                                                SLAVE_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 slave_dpi_field_range_ZONE_THR_1_2_get( slave_dpi_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_ZONE_THR_1_2_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_ZONE_THR_1_2_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_ZONE_THR_1_2_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_ZONE_THR_1_2_get", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_ZONE_THR_1_2_get", stop_bit, 16 );
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
    /* ((0x00001400 + (N) * 0x4) bits 16:0) field ZONE_THR_1_2 of register PMC_SLAVE_DPI_REG_ZONE_1_2 index N=0..255 */
    reg_value = slave_dpi_reg_ZONE_1_2_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    field_value = (reg_value & SLAVE_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_MSK)
                  >> SLAVE_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_MSK, SLAVE_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_ZONE_THR_1_2_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void slave_dpi_field_ZONE_THR_2_3_set( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_ZONE_THR_2_3_set( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_THR_2_3_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_ZONE_THR_2_3_set", N, 255);
    if (value > 131071)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_ZONE_THR_2_3_set", value, 131071);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_ZONE_THR_2_3_set", A, N, value );

    /* ((0x00001800 + (N) * 0x4) bits 16:0) field ZONE_THR_2_3 of register PMC_SLAVE_DPI_REG_ZONE_2_3 index N=0..255 */
    slave_dpi_reg_ZONE_2_3_array_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            N,
                                            SLAVE_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_MSK,
                                            SLAVE_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_OFF,
                                            value);
}

static INLINE UINT32 slave_dpi_field_ZONE_THR_2_3_get( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_ZONE_THR_2_3_get( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_THR_2_3_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_ZONE_THR_2_3_get", N, 255);
    /* ((0x00001800 + (N) * 0x4) bits 16:0) field ZONE_THR_2_3 of register PMC_SLAVE_DPI_REG_ZONE_2_3 index N=0..255 */
    reg_value = slave_dpi_reg_ZONE_2_3_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    value = (reg_value & SLAVE_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_MSK) >> SLAVE_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_ZONE_THR_2_3_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_range_ZONE_THR_2_3_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_range_ZONE_THR_2_3_set( slave_dpi_buffer_t *b_ptr,
                                                           cpb_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_ZONE_THR_2_3_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_ZONE_THR_2_3_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_ZONE_THR_2_3_set", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_ZONE_THR_2_3_set", stop_bit, 16 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_ZONE_THR_2_3_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001800 + (N) * 0x4) bits 16:0) field ZONE_THR_2_3 of register PMC_SLAVE_DPI_REG_ZONE_2_3 index N=0..255 */
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
        /* ((0x00001800 + (N) * 0x4) bits 16:0) field ZONE_THR_2_3 of register PMC_SLAVE_DPI_REG_ZONE_2_3 index N=0..255 */
        slave_dpi_reg_ZONE_2_3_array_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                N,
                                                subfield_mask << (SLAVE_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_OFF + subfield_offset),
                                                SLAVE_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 slave_dpi_field_range_ZONE_THR_2_3_get( slave_dpi_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_ZONE_THR_2_3_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_ZONE_THR_2_3_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_ZONE_THR_2_3_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_ZONE_THR_2_3_get", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_ZONE_THR_2_3_get", stop_bit, 16 );
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
    /* ((0x00001800 + (N) * 0x4) bits 16:0) field ZONE_THR_2_3 of register PMC_SLAVE_DPI_REG_ZONE_2_3 index N=0..255 */
    reg_value = slave_dpi_reg_ZONE_2_3_array_read( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N);
    field_value = (reg_value & SLAVE_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_MSK)
                  >> SLAVE_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_MSK, SLAVE_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_ZONE_THR_2_3_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void slave_dpi_field_ZONE_OVRD_EN_set( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_ZONE_OVRD_EN_set( slave_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_OVRD_EN_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_ZONE_OVRD_EN_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_ZONE_OVRD_EN_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_ZONE_OVRD_EN_set", A, N, value );

    /* ((0x00001c00 + (N) * 0x4) bits 25) field ZONE_OVRD_EN of register PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG index N=0..255 */
    slave_dpi_reg_ZONE_OVRD_CONFIG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N,
                                                    SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_EN_MSK,
                                                    SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_EN_OFF,
                                                    value);
}

static INLINE UINT32 slave_dpi_field_ZONE_OVRD_EN_get( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_ZONE_OVRD_EN_get( slave_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_OVRD_EN_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_ZONE_OVRD_EN_get", N, 255);
    /* ((0x00001c00 + (N) * 0x4) bits 25) field ZONE_OVRD_EN of register PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG index N=0..255 */
    reg_value = slave_dpi_reg_ZONE_OVRD_CONFIG_array_read( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           N);
    value = (reg_value & SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_EN_MSK) >> SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_EN_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_ZONE_OVRD_EN_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_ZONE_OVRD_CFG_set( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_ZONE_OVRD_CFG_set( slave_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_OVRD_CFG_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_ZONE_OVRD_CFG_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_ZONE_OVRD_CFG_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_ZONE_OVRD_CFG_set", A, N, value );

    /* ((0x00001c00 + (N) * 0x4) bits 24) field ZONE_OVRD_CFG of register PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG index N=0..255 */
    slave_dpi_reg_ZONE_OVRD_CONFIG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N,
                                                    SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_CFG_MSK,
                                                    SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_CFG_OFF,
                                                    value);
}

static INLINE UINT32 slave_dpi_field_ZONE_OVRD_CFG_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_ZONE_OVRD_CFG_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_OVRD_CFG_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_ZONE_OVRD_CFG_get", N, 255);
    /* ((0x00001c00 + (N) * 0x4) bits 24) field ZONE_OVRD_CFG of register PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG index N=0..255 */
    reg_value = slave_dpi_reg_ZONE_OVRD_CONFIG_array_read( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           N);
    value = (reg_value & SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_CFG_MSK) >> SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_CFG_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_ZONE_OVRD_CFG_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_ZONE_OVRD_DPI_MST_set( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_ZONE_OVRD_DPI_MST_set( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_OVRD_DPI_MST_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_ZONE_OVRD_DPI_MST_set", N, 255);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_ZONE_OVRD_DPI_MST_set", value, 7);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_ZONE_OVRD_DPI_MST_set", A, N, value );

    /* ((0x00001c00 + (N) * 0x4) bits 10:8) field ZONE_OVRD_DPI_MST of register PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG index N=0..255 */
    slave_dpi_reg_ZONE_OVRD_CONFIG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N,
                                                    SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_DPI_MST_MSK,
                                                    SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_DPI_MST_OFF,
                                                    value);
}

static INLINE UINT32 slave_dpi_field_ZONE_OVRD_DPI_MST_get( slave_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_ZONE_OVRD_DPI_MST_get( slave_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_OVRD_DPI_MST_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_ZONE_OVRD_DPI_MST_get", N, 255);
    /* ((0x00001c00 + (N) * 0x4) bits 10:8) field ZONE_OVRD_DPI_MST of register PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG index N=0..255 */
    reg_value = slave_dpi_reg_ZONE_OVRD_CONFIG_array_read( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           N);
    value = (reg_value & SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_DPI_MST_MSK) >> SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_DPI_MST_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_ZONE_OVRD_DPI_MST_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_range_ZONE_OVRD_DPI_MST_set( slave_dpi_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_range_ZONE_OVRD_DPI_MST_set( slave_dpi_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_ZONE_OVRD_DPI_MST_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_ZONE_OVRD_DPI_MST_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_ZONE_OVRD_DPI_MST_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_ZONE_OVRD_DPI_MST_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_ZONE_OVRD_DPI_MST_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001c00 + (N) * 0x4) bits 10:8) field ZONE_OVRD_DPI_MST of register PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG index N=0..255 */
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
        /* ((0x00001c00 + (N) * 0x4) bits 10:8) field ZONE_OVRD_DPI_MST of register PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG index N=0..255 */
        slave_dpi_reg_ZONE_OVRD_CONFIG_array_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        N,
                                                        subfield_mask << (SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_DPI_MST_OFF + subfield_offset),
                                                        SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_DPI_MST_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 slave_dpi_field_range_ZONE_OVRD_DPI_MST_get( slave_dpi_buffer_t *b_ptr,
                                                                  cpb_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32  N,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_ZONE_OVRD_DPI_MST_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_ZONE_OVRD_DPI_MST_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_ZONE_OVRD_DPI_MST_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_ZONE_OVRD_DPI_MST_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_ZONE_OVRD_DPI_MST_get", stop_bit, 2 );
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
    /* ((0x00001c00 + (N) * 0x4) bits 10:8) field ZONE_OVRD_DPI_MST of register PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG index N=0..255 */
    reg_value = slave_dpi_reg_ZONE_OVRD_CONFIG_array_read( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           N);
    field_value = (reg_value & SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_DPI_MST_MSK)
                  >> SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_DPI_MST_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_DPI_MST_MSK, SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_DPI_MST_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_ZONE_OVRD_DPI_MST_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void slave_dpi_field_ZONE_OVRD_XOFF_CH_set( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_ZONE_OVRD_XOFF_CH_set( slave_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_OVRD_XOFF_CH_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_ZONE_OVRD_XOFF_CH_set", N, 255);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "slave_dpi_field_ZONE_OVRD_XOFF_CH_set", value, 255);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "slave_dpi_field_ZONE_OVRD_XOFF_CH_set", A, N, value );

    /* ((0x00001c00 + (N) * 0x4) bits 7:0) field ZONE_OVRD_XOFF_CH of register PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG index N=0..255 */
    slave_dpi_reg_ZONE_OVRD_CONFIG_array_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N,
                                                    SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_XOFF_CH_MSK,
                                                    SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_XOFF_CH_OFF,
                                                    value);
}

static INLINE UINT32 slave_dpi_field_ZONE_OVRD_XOFF_CH_get( slave_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_ZONE_OVRD_XOFF_CH_get( slave_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_ZONE_OVRD_XOFF_CH_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_ZONE_OVRD_XOFF_CH_get", N, 255);
    /* ((0x00001c00 + (N) * 0x4) bits 7:0) field ZONE_OVRD_XOFF_CH of register PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG index N=0..255 */
    reg_value = slave_dpi_reg_ZONE_OVRD_CONFIG_array_read( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           N);
    value = (reg_value & SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_XOFF_CH_MSK) >> SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_XOFF_CH_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "slave_dpi_field_ZONE_OVRD_XOFF_CH_get", A, N, value );

    return value;
}
static INLINE void slave_dpi_field_range_ZONE_OVRD_XOFF_CH_set( slave_dpi_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void slave_dpi_field_range_ZONE_OVRD_XOFF_CH_set( slave_dpi_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_ZONE_OVRD_XOFF_CH_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_ZONE_OVRD_XOFF_CH_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_ZONE_OVRD_XOFF_CH_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_ZONE_OVRD_XOFF_CH_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_ZONE_OVRD_XOFF_CH_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001c00 + (N) * 0x4) bits 7:0) field ZONE_OVRD_XOFF_CH of register PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG index N=0..255 */
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
        /* ((0x00001c00 + (N) * 0x4) bits 7:0) field ZONE_OVRD_XOFF_CH of register PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG index N=0..255 */
        slave_dpi_reg_ZONE_OVRD_CONFIG_array_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        N,
                                                        subfield_mask << (SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_XOFF_CH_OFF + subfield_offset),
                                                        SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_XOFF_CH_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 slave_dpi_field_range_ZONE_OVRD_XOFF_CH_get( slave_dpi_buffer_t *b_ptr,
                                                                  cpb_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32  N,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_ZONE_OVRD_XOFF_CH_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_ZONE_OVRD_XOFF_CH_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "slave_dpi_field_range_ZONE_OVRD_XOFF_CH_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_ZONE_OVRD_XOFF_CH_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_ZONE_OVRD_XOFF_CH_get", stop_bit, 7 );
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
    /* ((0x00001c00 + (N) * 0x4) bits 7:0) field ZONE_OVRD_XOFF_CH of register PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG index N=0..255 */
    reg_value = slave_dpi_reg_ZONE_OVRD_CONFIG_array_read( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           N);
    field_value = (reg_value & SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_XOFF_CH_MSK)
                  >> SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_XOFF_CH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_XOFF_CH_MSK, SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_XOFF_CH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_ZONE_OVRD_XOFF_CH_get", A, N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 slave_dpi_field_PMON_CNT0_VAL_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT0_VAL_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT0_VAL_get", A, 7);
    /* (0x00000110 bits 31:0) bits 0:31 use field PMON_CNT0_VAL of register PMC_SLAVE_DPI_REG_PMON_CNT0_REG */
    reg_value = slave_dpi_reg_PMON_CNT0_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_PMON_CNT0_REG_BIT_PMON_CNT0_VAL_MSK) >> SLAVE_DPI_REG_PMON_CNT0_REG_BIT_PMON_CNT0_VAL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT0_VAL_get", A, value );

    return value;
}
static INLINE UINT32 slave_dpi_field_range_PMON_CNT0_VAL_get( slave_dpi_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_PMON_CNT0_VAL_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_PMON_CNT0_VAL_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_PMON_CNT0_VAL_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_PMON_CNT0_VAL_get", stop_bit, 31 );
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
    /* (0x00000110 bits 31:0) bits 0:31 use field PMON_CNT0_VAL of register PMC_SLAVE_DPI_REG_PMON_CNT0_REG */
    reg_value = slave_dpi_reg_PMON_CNT0_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & SLAVE_DPI_REG_PMON_CNT0_REG_BIT_PMON_CNT0_VAL_MSK)
                  >> SLAVE_DPI_REG_PMON_CNT0_REG_BIT_PMON_CNT0_VAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_PMON_CNT0_REG_BIT_PMON_CNT0_VAL_MSK, SLAVE_DPI_REG_PMON_CNT0_REG_BIT_PMON_CNT0_VAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_PMON_CNT0_VAL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 slave_dpi_field_PMON_CNT1_VAL_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT1_VAL_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT1_VAL_get", A, 7);
    /* (0x00000114 bits 31:0) bits 0:31 use field PMON_CNT1_VAL of register PMC_SLAVE_DPI_REG_PMON_CNT1_REG */
    reg_value = slave_dpi_reg_PMON_CNT1_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_PMON_CNT1_REG_BIT_PMON_CNT1_VAL_MSK) >> SLAVE_DPI_REG_PMON_CNT1_REG_BIT_PMON_CNT1_VAL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT1_VAL_get", A, value );

    return value;
}
static INLINE UINT32 slave_dpi_field_range_PMON_CNT1_VAL_get( slave_dpi_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_PMON_CNT1_VAL_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_PMON_CNT1_VAL_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_PMON_CNT1_VAL_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_PMON_CNT1_VAL_get", stop_bit, 31 );
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
    /* (0x00000114 bits 31:0) bits 0:31 use field PMON_CNT1_VAL of register PMC_SLAVE_DPI_REG_PMON_CNT1_REG */
    reg_value = slave_dpi_reg_PMON_CNT1_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & SLAVE_DPI_REG_PMON_CNT1_REG_BIT_PMON_CNT1_VAL_MSK)
                  >> SLAVE_DPI_REG_PMON_CNT1_REG_BIT_PMON_CNT1_VAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_PMON_CNT1_REG_BIT_PMON_CNT1_VAL_MSK, SLAVE_DPI_REG_PMON_CNT1_REG_BIT_PMON_CNT1_VAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_PMON_CNT1_VAL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 slave_dpi_field_PMON_CNT2_VAL_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT2_VAL_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT2_VAL_get", A, 7);
    /* (0x00000118 bits 31:0) bits 0:31 use field PMON_CNT2_VAL of register PMC_SLAVE_DPI_REG_PMON_CNT2_REG */
    reg_value = slave_dpi_reg_PMON_CNT2_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_PMON_CNT2_REG_BIT_PMON_CNT2_VAL_MSK) >> SLAVE_DPI_REG_PMON_CNT2_REG_BIT_PMON_CNT2_VAL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT2_VAL_get", A, value );

    return value;
}
static INLINE UINT32 slave_dpi_field_range_PMON_CNT2_VAL_get( slave_dpi_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_PMON_CNT2_VAL_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_PMON_CNT2_VAL_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_PMON_CNT2_VAL_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_PMON_CNT2_VAL_get", stop_bit, 31 );
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
    /* (0x00000118 bits 31:0) bits 0:31 use field PMON_CNT2_VAL of register PMC_SLAVE_DPI_REG_PMON_CNT2_REG */
    reg_value = slave_dpi_reg_PMON_CNT2_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & SLAVE_DPI_REG_PMON_CNT2_REG_BIT_PMON_CNT2_VAL_MSK)
                  >> SLAVE_DPI_REG_PMON_CNT2_REG_BIT_PMON_CNT2_VAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_PMON_CNT2_REG_BIT_PMON_CNT2_VAL_MSK, SLAVE_DPI_REG_PMON_CNT2_REG_BIT_PMON_CNT2_VAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_PMON_CNT2_VAL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 slave_dpi_field_PMON_CNT3_VAL_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_PMON_CNT3_VAL_get( slave_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_PMON_CNT3_VAL_get", A, 7);
    /* (0x0000011c bits 31:0) bits 0:31 use field PMON_CNT3_VAL of register PMC_SLAVE_DPI_REG_PMON_CNT3_REG */
    reg_value = slave_dpi_reg_PMON_CNT3_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & SLAVE_DPI_REG_PMON_CNT3_REG_BIT_PMON_CNT3_VAL_MSK) >> SLAVE_DPI_REG_PMON_CNT3_REG_BIT_PMON_CNT3_VAL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "slave_dpi_field_PMON_CNT3_VAL_get", A, value );

    return value;
}
static INLINE UINT32 slave_dpi_field_range_PMON_CNT3_VAL_get( slave_dpi_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 slave_dpi_field_range_PMON_CNT3_VAL_get( slave_dpi_buffer_t *b_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "slave_dpi_field_range_PMON_CNT3_VAL_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "slave_dpi_field_range_PMON_CNT3_VAL_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "slave_dpi_field_range_PMON_CNT3_VAL_get", stop_bit, 31 );
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
    /* (0x0000011c bits 31:0) bits 0:31 use field PMON_CNT3_VAL of register PMC_SLAVE_DPI_REG_PMON_CNT3_REG */
    reg_value = slave_dpi_reg_PMON_CNT3_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & SLAVE_DPI_REG_PMON_CNT3_REG_BIT_PMON_CNT3_VAL_MSK)
                  >> SLAVE_DPI_REG_PMON_CNT3_REG_BIT_PMON_CNT3_VAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SLAVE_DPI_REG_PMON_CNT3_REG_BIT_PMON_CNT3_VAL_MSK, SLAVE_DPI_REG_PMON_CNT3_REG_BIT_PMON_CNT3_VAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "slave_dpi_field_range_PMON_CNT3_VAL_get", A, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SLAVE_DPI_IO_INLINE_H */
