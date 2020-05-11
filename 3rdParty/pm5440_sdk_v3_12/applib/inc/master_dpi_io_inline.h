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
 *     and register accessor functions for the master_dpi block
 *****************************************************************************/
#ifndef _MASTER_DPI_IO_INLINE_H
#define _MASTER_DPI_IO_INLINE_H

#include "cpb_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "master_dpi_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MASTER_DPI_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for master_dpi
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
} master_dpi_buffer_t;
static INLINE void master_dpi_buffer_init( master_dpi_buffer_t *b_ptr,
                                           cpb_handle_t *h_ptr,
                                           UINT32 A ) ALWAYS_INLINE;
static INLINE void master_dpi_buffer_init( master_dpi_buffer_t *b_ptr,
                                           cpb_handle_t *h_ptr,
                                           UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x4000) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "master_dpi_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void master_dpi_buffer_flush( master_dpi_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void master_dpi_buffer_flush( master_dpi_buffer_t *b_ptr )
{
    IOLOG( "master_dpi_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 master_dpi_reg_read( master_dpi_buffer_t *b_ptr,
                                          cpb_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 mem_type,
                                          UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 master_dpi_reg_read( master_dpi_buffer_t *b_ptr,
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
static INLINE void master_dpi_reg_write( master_dpi_buffer_t *b_ptr,
                                         cpb_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_reg_write( master_dpi_buffer_t *b_ptr,
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

static INLINE void master_dpi_field_set( master_dpi_buffer_t *b_ptr,
                                         cpb_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 mask,
                                         UINT32 unused_mask,
                                         UINT32 ofs,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_field_set( master_dpi_buffer_t *b_ptr,
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

static INLINE void master_dpi_action_on_write_field_set( master_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mem_type,
                                                         UINT32 reg,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_action_on_write_field_set( master_dpi_buffer_t *b_ptr,
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

static INLINE void master_dpi_burst_read( master_dpi_buffer_t *b_ptr,
                                          cpb_handle_t *h_ptr,
                                          UINT32 A,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void master_dpi_burst_read( master_dpi_buffer_t *b_ptr,
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

static INLINE void master_dpi_burst_write( master_dpi_buffer_t *b_ptr,
                                           cpb_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 len,
                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void master_dpi_burst_write( master_dpi_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE master_dpi_poll( master_dpi_buffer_t *b_ptr,
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
static INLINE PMC_POLL_RETURN_TYPE master_dpi_poll( master_dpi_buffer_t *b_ptr,
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
 *  register access functions for master_dpi
 * ==================================================================================
 */

static INLINE void master_dpi_reg_ZONE_CONFIG_write( master_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_reg_ZONE_CONFIG_write( master_dpi_buffer_t *b_ptr,
                                                     cpb_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "master_dpi_reg_ZONE_CONFIG_write", value );
    master_dpi_reg_write( b_ptr,
                          h_ptr,
                          A,
                          MEM_TYPE_CONFIG,
                          PMC_MASTER_DPI_REG_ZONE_CONFIG,
                          value);
}

static INLINE void master_dpi_reg_ZONE_CONFIG_field_set( master_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_reg_ZONE_CONFIG_field_set( master_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "master_dpi_reg_ZONE_CONFIG_field_set", A, mask, ofs, value );
    master_dpi_field_set( b_ptr,
                          h_ptr,
                          A,
                          MEM_TYPE_CONFIG,
                          PMC_MASTER_DPI_REG_ZONE_CONFIG,
                          mask,
                          PMC_MASTER_DPI_REG_ZONE_CONFIG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 master_dpi_reg_ZONE_CONFIG_read( master_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 master_dpi_reg_ZONE_CONFIG_read( master_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = master_dpi_reg_read( b_ptr,
                                     h_ptr,
                                     A,
                                     MEM_TYPE_CONFIG,
                                     PMC_MASTER_DPI_REG_ZONE_CONFIG);

    IOLOG( "%s -> 0x%08x; A=%d", "master_dpi_reg_ZONE_CONFIG_read", reg_value, A);
    return reg_value;
}

static INLINE void master_dpi_reg_CH_CONFIG_array_write( master_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_reg_CH_CONFIG_array_write( master_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "master_dpi_reg_CH_CONFIG_array_write", value );
    master_dpi_reg_write( b_ptr,
                          h_ptr,
                          A,
                          MEM_TYPE_STATUS,
                          PMC_MASTER_DPI_REG_CH_CONFIG(N),
                          value);
}

static INLINE void master_dpi_reg_CH_CONFIG_array_field_set( master_dpi_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_reg_CH_CONFIG_array_field_set( master_dpi_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "master_dpi_reg_CH_CONFIG_array_field_set", A, N, mask, ofs, value );
    master_dpi_field_set( b_ptr,
                          h_ptr,
                          A,
                          MEM_TYPE_STATUS,
                          PMC_MASTER_DPI_REG_CH_CONFIG(N),
                          mask,
                          PMC_MASTER_DPI_REG_CH_CONFIG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 master_dpi_reg_CH_CONFIG_array_read( master_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 master_dpi_reg_CH_CONFIG_array_read( master_dpi_buffer_t *b_ptr,
                                                          cpb_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N )
{
    UINT32 reg_value;

    reg_value = master_dpi_reg_read( b_ptr,
                                     h_ptr,
                                     A,
                                     MEM_TYPE_STATUS,
                                     PMC_MASTER_DPI_REG_CH_CONFIG(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "master_dpi_reg_CH_CONFIG_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void master_dpi_reg_ZONE_0_1_array_write( master_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_reg_ZONE_0_1_array_write( master_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "master_dpi_reg_ZONE_0_1_array_write", value );
    master_dpi_reg_write( b_ptr,
                          h_ptr,
                          A,
                          MEM_TYPE_STATUS,
                          PMC_MASTER_DPI_REG_ZONE_0_1(N),
                          value);
}

static INLINE void master_dpi_reg_ZONE_0_1_array_field_set( master_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_reg_ZONE_0_1_array_field_set( master_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "master_dpi_reg_ZONE_0_1_array_field_set", A, N, mask, ofs, value );
    master_dpi_field_set( b_ptr,
                          h_ptr,
                          A,
                          MEM_TYPE_STATUS,
                          PMC_MASTER_DPI_REG_ZONE_0_1(N),
                          mask,
                          PMC_MASTER_DPI_REG_ZONE_0_1_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 master_dpi_reg_ZONE_0_1_array_read( master_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 master_dpi_reg_ZONE_0_1_array_read( master_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = master_dpi_reg_read( b_ptr,
                                     h_ptr,
                                     A,
                                     MEM_TYPE_STATUS,
                                     PMC_MASTER_DPI_REG_ZONE_0_1(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "master_dpi_reg_ZONE_0_1_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void master_dpi_reg_ZONE_1_2_array_write( master_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_reg_ZONE_1_2_array_write( master_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "master_dpi_reg_ZONE_1_2_array_write", value );
    master_dpi_reg_write( b_ptr,
                          h_ptr,
                          A,
                          MEM_TYPE_STATUS,
                          PMC_MASTER_DPI_REG_ZONE_1_2(N),
                          value);
}

static INLINE void master_dpi_reg_ZONE_1_2_array_field_set( master_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_reg_ZONE_1_2_array_field_set( master_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "master_dpi_reg_ZONE_1_2_array_field_set", A, N, mask, ofs, value );
    master_dpi_field_set( b_ptr,
                          h_ptr,
                          A,
                          MEM_TYPE_STATUS,
                          PMC_MASTER_DPI_REG_ZONE_1_2(N),
                          mask,
                          PMC_MASTER_DPI_REG_ZONE_1_2_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 master_dpi_reg_ZONE_1_2_array_read( master_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 master_dpi_reg_ZONE_1_2_array_read( master_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = master_dpi_reg_read( b_ptr,
                                     h_ptr,
                                     A,
                                     MEM_TYPE_STATUS,
                                     PMC_MASTER_DPI_REG_ZONE_1_2(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "master_dpi_reg_ZONE_1_2_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void master_dpi_reg_ZONE_2_3_array_write( master_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_reg_ZONE_2_3_array_write( master_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "master_dpi_reg_ZONE_2_3_array_write", value );
    master_dpi_reg_write( b_ptr,
                          h_ptr,
                          A,
                          MEM_TYPE_STATUS,
                          PMC_MASTER_DPI_REG_ZONE_2_3(N),
                          value);
}

static INLINE void master_dpi_reg_ZONE_2_3_array_field_set( master_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_reg_ZONE_2_3_array_field_set( master_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "master_dpi_reg_ZONE_2_3_array_field_set", A, N, mask, ofs, value );
    master_dpi_field_set( b_ptr,
                          h_ptr,
                          A,
                          MEM_TYPE_STATUS,
                          PMC_MASTER_DPI_REG_ZONE_2_3(N),
                          mask,
                          PMC_MASTER_DPI_REG_ZONE_2_3_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 master_dpi_reg_ZONE_2_3_array_read( master_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 master_dpi_reg_ZONE_2_3_array_read( master_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = master_dpi_reg_read( b_ptr,
                                     h_ptr,
                                     A,
                                     MEM_TYPE_STATUS,
                                     PMC_MASTER_DPI_REG_ZONE_2_3(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "master_dpi_reg_ZONE_2_3_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void master_dpi_reg_ZONE_OVRD_CONFIG_array_write( master_dpi_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_reg_ZONE_OVRD_CONFIG_array_write( master_dpi_buffer_t *b_ptr,
                                                                cpb_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "master_dpi_reg_ZONE_OVRD_CONFIG_array_write", value );
    master_dpi_reg_write( b_ptr,
                          h_ptr,
                          A,
                          MEM_TYPE_STATUS,
                          PMC_MASTER_DPI_REG_ZONE_OVRD_CONFIG(N),
                          value);
}

static INLINE void master_dpi_reg_ZONE_OVRD_CONFIG_array_field_set( master_dpi_buffer_t *b_ptr,
                                                                    cpb_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_reg_ZONE_OVRD_CONFIG_array_field_set( master_dpi_buffer_t *b_ptr,
                                                                    cpb_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "master_dpi_reg_ZONE_OVRD_CONFIG_array_field_set", A, N, mask, ofs, value );
    master_dpi_field_set( b_ptr,
                          h_ptr,
                          A,
                          MEM_TYPE_STATUS,
                          PMC_MASTER_DPI_REG_ZONE_OVRD_CONFIG(N),
                          mask,
                          PMC_MASTER_DPI_REG_ZONE_OVRD_CONFIG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 master_dpi_reg_ZONE_OVRD_CONFIG_array_read( master_dpi_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 master_dpi_reg_ZONE_OVRD_CONFIG_array_read( master_dpi_buffer_t *b_ptr,
                                                                 cpb_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32  N )
{
    UINT32 reg_value;

    reg_value = master_dpi_reg_read( b_ptr,
                                     h_ptr,
                                     A,
                                     MEM_TYPE_STATUS,
                                     PMC_MASTER_DPI_REG_ZONE_OVRD_CONFIG(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "master_dpi_reg_ZONE_OVRD_CONFIG_array_read", reg_value, A, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void master_dpi_field_ZONE_EN_set( master_dpi_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_field_ZONE_EN_set( master_dpi_buffer_t *b_ptr,
                                                 cpb_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_ZONE_EN_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "master_dpi_field_ZONE_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "master_dpi_field_ZONE_EN_set", A, value );

    /* (0x00020000 bits 17) field ZONE_EN of register PMC_MASTER_DPI_REG_ZONE_CONFIG */
    master_dpi_reg_ZONE_CONFIG_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          MASTER_DPI_REG_ZONE_CONFIG_BIT_ZONE_EN_MSK,
                                          MASTER_DPI_REG_ZONE_CONFIG_BIT_ZONE_EN_OFF,
                                          value);
}

static INLINE UINT32 master_dpi_field_ZONE_EN_get( master_dpi_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 master_dpi_field_ZONE_EN_get( master_dpi_buffer_t *b_ptr,
                                                   cpb_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_ZONE_EN_get", A, 7);
    /* (0x00020000 bits 17) field ZONE_EN of register PMC_MASTER_DPI_REG_ZONE_CONFIG */
    reg_value = master_dpi_reg_ZONE_CONFIG_read(  b_ptr, h_ptr, A);
    value = (reg_value & MASTER_DPI_REG_ZONE_CONFIG_BIT_ZONE_EN_MSK) >> MASTER_DPI_REG_ZONE_CONFIG_BIT_ZONE_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "master_dpi_field_ZONE_EN_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size256
 * ==================================================================================
 */
static INLINE void master_dpi_field_EOP_HOLE_SIZE_set( master_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_field_EOP_HOLE_SIZE_set( master_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_EOP_HOLE_SIZE_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_EOP_HOLE_SIZE_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "master_dpi_field_EOP_HOLE_SIZE_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "master_dpi_field_EOP_HOLE_SIZE_set", A, N, value );

    /* ((0x00020400 + (N) * 0x4) bits 16) field EOP_HOLE_SIZE of register PMC_MASTER_DPI_REG_CH_CONFIG index N=0..255 */
    master_dpi_reg_CH_CONFIG_array_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              N,
                                              MASTER_DPI_REG_CH_CONFIG_BIT_EOP_HOLE_SIZE_MSK,
                                              MASTER_DPI_REG_CH_CONFIG_BIT_EOP_HOLE_SIZE_OFF,
                                              value);
}

static INLINE UINT32 master_dpi_field_EOP_HOLE_SIZE_get( master_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 master_dpi_field_EOP_HOLE_SIZE_get( master_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_EOP_HOLE_SIZE_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_EOP_HOLE_SIZE_get", N, 255);
    /* ((0x00020400 + (N) * 0x4) bits 16) field EOP_HOLE_SIZE of register PMC_MASTER_DPI_REG_CH_CONFIG index N=0..255 */
    reg_value = master_dpi_reg_CH_CONFIG_array_read( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     N);
    value = (reg_value & MASTER_DPI_REG_CH_CONFIG_BIT_EOP_HOLE_SIZE_MSK) >> MASTER_DPI_REG_CH_CONFIG_BIT_EOP_HOLE_SIZE_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "master_dpi_field_EOP_HOLE_SIZE_get", A, N, value );

    return value;
}
static INLINE void master_dpi_field_SOP_HOLE_SIZE_set( master_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_field_SOP_HOLE_SIZE_set( master_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_SOP_HOLE_SIZE_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_SOP_HOLE_SIZE_set", N, 255);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "master_dpi_field_SOP_HOLE_SIZE_set", value, 127);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "master_dpi_field_SOP_HOLE_SIZE_set", A, N, value );

    /* ((0x00020400 + (N) * 0x4) bits 6:0) field SOP_HOLE_SIZE of register PMC_MASTER_DPI_REG_CH_CONFIG index N=0..255 */
    master_dpi_reg_CH_CONFIG_array_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              N,
                                              MASTER_DPI_REG_CH_CONFIG_BIT_SOP_HOLE_SIZE_MSK,
                                              MASTER_DPI_REG_CH_CONFIG_BIT_SOP_HOLE_SIZE_OFF,
                                              value);
}

static INLINE UINT32 master_dpi_field_SOP_HOLE_SIZE_get( master_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 master_dpi_field_SOP_HOLE_SIZE_get( master_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_SOP_HOLE_SIZE_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_SOP_HOLE_SIZE_get", N, 255);
    /* ((0x00020400 + (N) * 0x4) bits 6:0) field SOP_HOLE_SIZE of register PMC_MASTER_DPI_REG_CH_CONFIG index N=0..255 */
    reg_value = master_dpi_reg_CH_CONFIG_array_read( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     N);
    value = (reg_value & MASTER_DPI_REG_CH_CONFIG_BIT_SOP_HOLE_SIZE_MSK) >> MASTER_DPI_REG_CH_CONFIG_BIT_SOP_HOLE_SIZE_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "master_dpi_field_SOP_HOLE_SIZE_get", A, N, value );

    return value;
}
static INLINE void master_dpi_field_range_SOP_HOLE_SIZE_set( master_dpi_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_field_range_SOP_HOLE_SIZE_set( master_dpi_buffer_t *b_ptr,
                                                             cpb_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_range_SOP_HOLE_SIZE_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_range_SOP_HOLE_SIZE_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "master_dpi_field_range_SOP_HOLE_SIZE_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "master_dpi_field_range_SOP_HOLE_SIZE_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "master_dpi_field_range_SOP_HOLE_SIZE_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00020400 + (N) * 0x4) bits 6:0) field SOP_HOLE_SIZE of register PMC_MASTER_DPI_REG_CH_CONFIG index N=0..255 */
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
        /* ((0x00020400 + (N) * 0x4) bits 6:0) field SOP_HOLE_SIZE of register PMC_MASTER_DPI_REG_CH_CONFIG index N=0..255 */
        master_dpi_reg_CH_CONFIG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  N,
                                                  subfield_mask << (MASTER_DPI_REG_CH_CONFIG_BIT_SOP_HOLE_SIZE_OFF + subfield_offset),
                                                  MASTER_DPI_REG_CH_CONFIG_BIT_SOP_HOLE_SIZE_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 master_dpi_field_range_SOP_HOLE_SIZE_get( master_dpi_buffer_t *b_ptr,
                                                               cpb_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 master_dpi_field_range_SOP_HOLE_SIZE_get( master_dpi_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_range_SOP_HOLE_SIZE_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_range_SOP_HOLE_SIZE_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "master_dpi_field_range_SOP_HOLE_SIZE_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "master_dpi_field_range_SOP_HOLE_SIZE_get", stop_bit, 6 );
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
    /* ((0x00020400 + (N) * 0x4) bits 6:0) field SOP_HOLE_SIZE of register PMC_MASTER_DPI_REG_CH_CONFIG index N=0..255 */
    reg_value = master_dpi_reg_CH_CONFIG_array_read( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     N);
    field_value = (reg_value & MASTER_DPI_REG_CH_CONFIG_BIT_SOP_HOLE_SIZE_MSK)
                  >> MASTER_DPI_REG_CH_CONFIG_BIT_SOP_HOLE_SIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MASTER_DPI_REG_CH_CONFIG_BIT_SOP_HOLE_SIZE_MSK, MASTER_DPI_REG_CH_CONFIG_BIT_SOP_HOLE_SIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "master_dpi_field_range_SOP_HOLE_SIZE_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void master_dpi_field_ZONE_THR_0_1_set( master_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_field_ZONE_THR_0_1_set( master_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_ZONE_THR_0_1_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_ZONE_THR_0_1_set", N, 255);
    if (value > 131071)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "master_dpi_field_ZONE_THR_0_1_set", value, 131071);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "master_dpi_field_ZONE_THR_0_1_set", A, N, value );

    /* ((0x00020800 + (N) * 0x4) bits 16:0) field ZONE_THR_0_1 of register PMC_MASTER_DPI_REG_ZONE_0_1 index N=0..255 */
    master_dpi_reg_ZONE_0_1_array_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             N,
                                             MASTER_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_MSK,
                                             MASTER_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_OFF,
                                             value);
}

static INLINE UINT32 master_dpi_field_ZONE_THR_0_1_get( master_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 master_dpi_field_ZONE_THR_0_1_get( master_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_ZONE_THR_0_1_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_ZONE_THR_0_1_get", N, 255);
    /* ((0x00020800 + (N) * 0x4) bits 16:0) field ZONE_THR_0_1 of register PMC_MASTER_DPI_REG_ZONE_0_1 index N=0..255 */
    reg_value = master_dpi_reg_ZONE_0_1_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    value = (reg_value & MASTER_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_MSK) >> MASTER_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "master_dpi_field_ZONE_THR_0_1_get", A, N, value );

    return value;
}
static INLINE void master_dpi_field_range_ZONE_THR_0_1_set( master_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_field_range_ZONE_THR_0_1_set( master_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_range_ZONE_THR_0_1_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_range_ZONE_THR_0_1_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "master_dpi_field_range_ZONE_THR_0_1_set", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "master_dpi_field_range_ZONE_THR_0_1_set", stop_bit, 16 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "master_dpi_field_range_ZONE_THR_0_1_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00020800 + (N) * 0x4) bits 16:0) field ZONE_THR_0_1 of register PMC_MASTER_DPI_REG_ZONE_0_1 index N=0..255 */
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
        /* ((0x00020800 + (N) * 0x4) bits 16:0) field ZONE_THR_0_1 of register PMC_MASTER_DPI_REG_ZONE_0_1 index N=0..255 */
        master_dpi_reg_ZONE_0_1_array_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 subfield_mask << (MASTER_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_OFF + subfield_offset),
                                                 MASTER_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 master_dpi_field_range_ZONE_THR_0_1_get( master_dpi_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 master_dpi_field_range_ZONE_THR_0_1_get( master_dpi_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_range_ZONE_THR_0_1_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_range_ZONE_THR_0_1_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "master_dpi_field_range_ZONE_THR_0_1_get", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "master_dpi_field_range_ZONE_THR_0_1_get", stop_bit, 16 );
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
    /* ((0x00020800 + (N) * 0x4) bits 16:0) field ZONE_THR_0_1 of register PMC_MASTER_DPI_REG_ZONE_0_1 index N=0..255 */
    reg_value = master_dpi_reg_ZONE_0_1_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    field_value = (reg_value & MASTER_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_MSK)
                  >> MASTER_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MASTER_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_MSK, MASTER_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "master_dpi_field_range_ZONE_THR_0_1_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void master_dpi_field_ZONE_THR_1_2_set( master_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_field_ZONE_THR_1_2_set( master_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_ZONE_THR_1_2_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_ZONE_THR_1_2_set", N, 255);
    if (value > 131071)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "master_dpi_field_ZONE_THR_1_2_set", value, 131071);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "master_dpi_field_ZONE_THR_1_2_set", A, N, value );

    /* ((0x00020c00 + (N) * 0x4) bits 16:0) field ZONE_THR_1_2 of register PMC_MASTER_DPI_REG_ZONE_1_2 index N=0..255 */
    master_dpi_reg_ZONE_1_2_array_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             N,
                                             MASTER_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_MSK,
                                             MASTER_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_OFF,
                                             value);
}

static INLINE UINT32 master_dpi_field_ZONE_THR_1_2_get( master_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 master_dpi_field_ZONE_THR_1_2_get( master_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_ZONE_THR_1_2_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_ZONE_THR_1_2_get", N, 255);
    /* ((0x00020c00 + (N) * 0x4) bits 16:0) field ZONE_THR_1_2 of register PMC_MASTER_DPI_REG_ZONE_1_2 index N=0..255 */
    reg_value = master_dpi_reg_ZONE_1_2_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    value = (reg_value & MASTER_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_MSK) >> MASTER_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "master_dpi_field_ZONE_THR_1_2_get", A, N, value );

    return value;
}
static INLINE void master_dpi_field_range_ZONE_THR_1_2_set( master_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_field_range_ZONE_THR_1_2_set( master_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_range_ZONE_THR_1_2_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_range_ZONE_THR_1_2_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "master_dpi_field_range_ZONE_THR_1_2_set", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "master_dpi_field_range_ZONE_THR_1_2_set", stop_bit, 16 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "master_dpi_field_range_ZONE_THR_1_2_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00020c00 + (N) * 0x4) bits 16:0) field ZONE_THR_1_2 of register PMC_MASTER_DPI_REG_ZONE_1_2 index N=0..255 */
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
        /* ((0x00020c00 + (N) * 0x4) bits 16:0) field ZONE_THR_1_2 of register PMC_MASTER_DPI_REG_ZONE_1_2 index N=0..255 */
        master_dpi_reg_ZONE_1_2_array_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 subfield_mask << (MASTER_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_OFF + subfield_offset),
                                                 MASTER_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 master_dpi_field_range_ZONE_THR_1_2_get( master_dpi_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 master_dpi_field_range_ZONE_THR_1_2_get( master_dpi_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_range_ZONE_THR_1_2_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_range_ZONE_THR_1_2_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "master_dpi_field_range_ZONE_THR_1_2_get", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "master_dpi_field_range_ZONE_THR_1_2_get", stop_bit, 16 );
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
    /* ((0x00020c00 + (N) * 0x4) bits 16:0) field ZONE_THR_1_2 of register PMC_MASTER_DPI_REG_ZONE_1_2 index N=0..255 */
    reg_value = master_dpi_reg_ZONE_1_2_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    field_value = (reg_value & MASTER_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_MSK)
                  >> MASTER_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MASTER_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_MSK, MASTER_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "master_dpi_field_range_ZONE_THR_1_2_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void master_dpi_field_ZONE_THR_2_3_set( master_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_field_ZONE_THR_2_3_set( master_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_ZONE_THR_2_3_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_ZONE_THR_2_3_set", N, 255);
    if (value > 131071)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "master_dpi_field_ZONE_THR_2_3_set", value, 131071);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "master_dpi_field_ZONE_THR_2_3_set", A, N, value );

    /* ((0x00021000 + (N) * 0x4) bits 16:0) field ZONE_THR_2_3 of register PMC_MASTER_DPI_REG_ZONE_2_3 index N=0..255 */
    master_dpi_reg_ZONE_2_3_array_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             N,
                                             MASTER_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_MSK,
                                             MASTER_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_OFF,
                                             value);
}

static INLINE UINT32 master_dpi_field_ZONE_THR_2_3_get( master_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 master_dpi_field_ZONE_THR_2_3_get( master_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_ZONE_THR_2_3_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_ZONE_THR_2_3_get", N, 255);
    /* ((0x00021000 + (N) * 0x4) bits 16:0) field ZONE_THR_2_3 of register PMC_MASTER_DPI_REG_ZONE_2_3 index N=0..255 */
    reg_value = master_dpi_reg_ZONE_2_3_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    value = (reg_value & MASTER_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_MSK) >> MASTER_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "master_dpi_field_ZONE_THR_2_3_get", A, N, value );

    return value;
}
static INLINE void master_dpi_field_range_ZONE_THR_2_3_set( master_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_field_range_ZONE_THR_2_3_set( master_dpi_buffer_t *b_ptr,
                                                            cpb_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_range_ZONE_THR_2_3_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_range_ZONE_THR_2_3_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "master_dpi_field_range_ZONE_THR_2_3_set", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "master_dpi_field_range_ZONE_THR_2_3_set", stop_bit, 16 );
    IOLOG( "%s <= A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "master_dpi_field_range_ZONE_THR_2_3_set", A, N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00021000 + (N) * 0x4) bits 16:0) field ZONE_THR_2_3 of register PMC_MASTER_DPI_REG_ZONE_2_3 index N=0..255 */
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
        /* ((0x00021000 + (N) * 0x4) bits 16:0) field ZONE_THR_2_3 of register PMC_MASTER_DPI_REG_ZONE_2_3 index N=0..255 */
        master_dpi_reg_ZONE_2_3_array_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 N,
                                                 subfield_mask << (MASTER_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_OFF + subfield_offset),
                                                 MASTER_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 master_dpi_field_range_ZONE_THR_2_3_get( master_dpi_buffer_t *b_ptr,
                                                              cpb_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 master_dpi_field_range_ZONE_THR_2_3_get( master_dpi_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_range_ZONE_THR_2_3_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_range_ZONE_THR_2_3_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "master_dpi_field_range_ZONE_THR_2_3_get", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "master_dpi_field_range_ZONE_THR_2_3_get", stop_bit, 16 );
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
    /* ((0x00021000 + (N) * 0x4) bits 16:0) field ZONE_THR_2_3 of register PMC_MASTER_DPI_REG_ZONE_2_3 index N=0..255 */
    reg_value = master_dpi_reg_ZONE_2_3_array_read( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    N);
    field_value = (reg_value & MASTER_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_MSK)
                  >> MASTER_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MASTER_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_MSK, MASTER_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "master_dpi_field_range_ZONE_THR_2_3_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE void master_dpi_field_ZONE_OVRD_EN_set( master_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_field_ZONE_OVRD_EN_set( master_dpi_buffer_t *b_ptr,
                                                      cpb_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_ZONE_OVRD_EN_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_ZONE_OVRD_EN_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "master_dpi_field_ZONE_OVRD_EN_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "master_dpi_field_ZONE_OVRD_EN_set", A, N, value );

    /* ((0x00021c00 + (N) * 0x4) bits 25) field ZONE_OVRD_EN of register PMC_MASTER_DPI_REG_ZONE_OVRD_CONFIG index N=0..255 */
    master_dpi_reg_ZONE_OVRD_CONFIG_array_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     N,
                                                     MASTER_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_EN_MSK,
                                                     MASTER_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_EN_OFF,
                                                     value);
}

static INLINE UINT32 master_dpi_field_ZONE_OVRD_EN_get( master_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 master_dpi_field_ZONE_OVRD_EN_get( master_dpi_buffer_t *b_ptr,
                                                        cpb_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_ZONE_OVRD_EN_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_ZONE_OVRD_EN_get", N, 255);
    /* ((0x00021c00 + (N) * 0x4) bits 25) field ZONE_OVRD_EN of register PMC_MASTER_DPI_REG_ZONE_OVRD_CONFIG index N=0..255 */
    reg_value = master_dpi_reg_ZONE_OVRD_CONFIG_array_read( b_ptr,
                                                            h_ptr,
                                                            A,
                                                            N);
    value = (reg_value & MASTER_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_EN_MSK) >> MASTER_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_EN_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "master_dpi_field_ZONE_OVRD_EN_get", A, N, value );

    return value;
}
static INLINE void master_dpi_field_ZONE_OVRD_CFG_set( master_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void master_dpi_field_ZONE_OVRD_CFG_set( master_dpi_buffer_t *b_ptr,
                                                       cpb_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_ZONE_OVRD_CFG_set", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_ZONE_OVRD_CFG_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "master_dpi_field_ZONE_OVRD_CFG_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "master_dpi_field_ZONE_OVRD_CFG_set", A, N, value );

    /* ((0x00021c00 + (N) * 0x4) bits 24) field ZONE_OVRD_CFG of register PMC_MASTER_DPI_REG_ZONE_OVRD_CONFIG index N=0..255 */
    master_dpi_reg_ZONE_OVRD_CONFIG_array_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     N,
                                                     MASTER_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_CFG_MSK,
                                                     MASTER_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_CFG_OFF,
                                                     value);
}

static INLINE UINT32 master_dpi_field_ZONE_OVRD_CFG_get( master_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 master_dpi_field_ZONE_OVRD_CFG_get( master_dpi_buffer_t *b_ptr,
                                                         cpb_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "master_dpi_field_ZONE_OVRD_CFG_get", A, 7);
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "master_dpi_field_ZONE_OVRD_CFG_get", N, 255);
    /* ((0x00021c00 + (N) * 0x4) bits 24) field ZONE_OVRD_CFG of register PMC_MASTER_DPI_REG_ZONE_OVRD_CONFIG index N=0..255 */
    reg_value = master_dpi_reg_ZONE_OVRD_CONFIG_array_read( b_ptr,
                                                            h_ptr,
                                                            A,
                                                            N);
    value = (reg_value & MASTER_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_CFG_MSK) >> MASTER_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_CFG_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "master_dpi_field_ZONE_OVRD_CFG_get", A, N, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MASTER_DPI_IO_INLINE_H */
