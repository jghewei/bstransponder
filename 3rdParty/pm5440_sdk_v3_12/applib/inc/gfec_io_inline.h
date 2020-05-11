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
 *     and register accessor functions for the gfec block
 *****************************************************************************/
#ifndef _GFEC_IO_INLINE_H
#define _GFEC_IO_INLINE_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "gfec_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GFEC_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for gfec
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
} gfec_buffer_t;
static INLINE void gfec_buffer_init( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void gfec_buffer_init( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "gfec_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void gfec_buffer_flush( gfec_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void gfec_buffer_flush( gfec_buffer_t *b_ptr )
{
    IOLOG( "gfec_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 gfec_reg_read( gfec_buffer_t *b_ptr,
                                    lineotn_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 gfec_reg_read( gfec_buffer_t *b_ptr,
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
static INLINE void gfec_reg_write( gfec_buffer_t *b_ptr,
                                   lineotn_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_reg_write( gfec_buffer_t *b_ptr,
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

static INLINE void gfec_field_set( gfec_buffer_t *b_ptr,
                                   lineotn_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 mask,
                                   UINT32 unused_mask,
                                   UINT32 ofs,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_field_set( gfec_buffer_t *b_ptr,
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

static INLINE void gfec_action_on_write_field_set( gfec_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_action_on_write_field_set( gfec_buffer_t *b_ptr,
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

static INLINE void gfec_burst_read( gfec_buffer_t *b_ptr,
                                    lineotn_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void gfec_burst_read( gfec_buffer_t *b_ptr,
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

static INLINE void gfec_burst_write( gfec_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void gfec_burst_write( gfec_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE gfec_poll( gfec_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 mask,
                                              UINT32 value,
                                              PMC_POLL_COMPARISON_TYPE cmp,
                                              UINT32 max_count,
                                              UINT32 *num_failed_polls,
                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE gfec_poll( gfec_buffer_t *b_ptr,
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
 *  register access functions for gfec
 * ==================================================================================
 */

static INLINE void gfec_reg_RS_FEC_MODE_write( gfec_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_reg_RS_FEC_MODE_write( gfec_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "gfec_reg_RS_FEC_MODE_write", value );
    gfec_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_GFEC_REG_RS_FEC_MODE,
                    value);
}

static INLINE void gfec_reg_RS_FEC_MODE_field_set( gfec_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_reg_RS_FEC_MODE_field_set( gfec_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "gfec_reg_RS_FEC_MODE_field_set", mask, ofs, value );
    gfec_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_GFEC_REG_RS_FEC_MODE,
                    mask,
                    PMC_GFEC_REG_RS_FEC_MODE_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 gfec_reg_RS_FEC_MODE_read( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_reg_RS_FEC_MODE_read( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gfec_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_GFEC_REG_RS_FEC_MODE);

    IOLOG( "%s -> 0x%08x;", "gfec_reg_RS_FEC_MODE_read", reg_value);
    return reg_value;
}

static INLINE void gfec_reg_LPA_STATISTICS_MODE_CFG_write( gfec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_reg_LPA_STATISTICS_MODE_CFG_write( gfec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "gfec_reg_LPA_STATISTICS_MODE_CFG_write", value );
    gfec_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_GFEC_REG_LPA_STATISTICS_MODE_CFG,
                    value);
}

static INLINE void gfec_reg_LPA_STATISTICS_MODE_CFG_field_set( gfec_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_reg_LPA_STATISTICS_MODE_CFG_field_set( gfec_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "gfec_reg_LPA_STATISTICS_MODE_CFG_field_set", mask, ofs, value );
    gfec_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_GFEC_REG_LPA_STATISTICS_MODE_CFG,
                    mask,
                    PMC_GFEC_REG_LPA_STATISTICS_MODE_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 gfec_reg_LPA_STATISTICS_MODE_CFG_read( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_reg_LPA_STATISTICS_MODE_CFG_read( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = gfec_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_GFEC_REG_LPA_STATISTICS_MODE_CFG);

    IOLOG( "%s -> 0x%08x;", "gfec_reg_LPA_STATISTICS_MODE_CFG_read", reg_value);
    return reg_value;
}

static INLINE void gfec_reg_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read( gfec_buffer_t *b_ptr,
                                                                                                            lineotn_handle_t *h_ptr,
                                                                                                            UINT32 ofs,
                                                                                                            UINT32 len,
                                                                                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void gfec_reg_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read( gfec_buffer_t *b_ptr,
                                                                                                            lineotn_handle_t *h_ptr,
                                                                                                            UINT32 ofs,
                                                                                                            UINT32 len,
                                                                                                            UINT32 *value )
{
    if (len + ofs > 12)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "gfec_reg_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read", ofs, len, 12 );
    gfec_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_GFEC_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "gfec_reg_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read", 12, ofs, len);
}

static INLINE UINT32 gfec_reg_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read( gfec_buffer_t *b_ptr,
                                                                                                        lineotn_handle_t *h_ptr,
                                                                                                        UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_reg_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read( gfec_buffer_t *b_ptr,
                                                                                                        lineotn_handle_t *h_ptr,
                                                                                                        UINT32 N )
{
    UINT32 reg_value;

    reg_value = gfec_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_GFEC_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11(N));

    IOLOG( "%s -> 0x%08x;", "gfec_reg_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read", reg_value);
    return reg_value;
}

static INLINE void gfec_reg_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read( gfec_buffer_t *b_ptr,
                                                                                                                  lineotn_handle_t *h_ptr,
                                                                                                                  UINT32 ofs,
                                                                                                                  UINT32 len,
                                                                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void gfec_reg_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read( gfec_buffer_t *b_ptr,
                                                                                                                  lineotn_handle_t *h_ptr,
                                                                                                                  UINT32 ofs,
                                                                                                                  UINT32 len,
                                                                                                                  UINT32 *value )
{
    if (len + ofs > 12)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "gfec_reg_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read", ofs, len, 12 );
    gfec_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_GFEC_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "gfec_reg_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read", 12, ofs, len);
}

static INLINE UINT32 gfec_reg_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read( gfec_buffer_t *b_ptr,
                                                                                                              lineotn_handle_t *h_ptr,
                                                                                                              UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_reg_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read( gfec_buffer_t *b_ptr,
                                                                                                              lineotn_handle_t *h_ptr,
                                                                                                              UINT32 N )
{
    UINT32 reg_value;

    reg_value = gfec_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_GFEC_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11(N));

    IOLOG( "%s -> 0x%08x;", "gfec_reg_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read", reg_value);
    return reg_value;
}

static INLINE void gfec_reg_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read( gfec_buffer_t *b_ptr,
                                                                                                         lineotn_handle_t *h_ptr,
                                                                                                         UINT32 ofs,
                                                                                                         UINT32 len,
                                                                                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void gfec_reg_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read( gfec_buffer_t *b_ptr,
                                                                                                         lineotn_handle_t *h_ptr,
                                                                                                         UINT32 ofs,
                                                                                                         UINT32 len,
                                                                                                         UINT32 *value )
{
    if (len + ofs > 12)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "gfec_reg_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read", ofs, len, 12 );
    gfec_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_GFEC_REG_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "gfec_reg_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read", 12, ofs, len);
}

static INLINE UINT32 gfec_reg_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read( gfec_buffer_t *b_ptr,
                                                                                                     lineotn_handle_t *h_ptr,
                                                                                                     UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_reg_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read( gfec_buffer_t *b_ptr,
                                                                                                     lineotn_handle_t *h_ptr,
                                                                                                     UINT32 N )
{
    UINT32 reg_value;

    reg_value = gfec_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_GFEC_REG_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11(N));

    IOLOG( "%s -> 0x%08x;", "gfec_reg_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read", reg_value);
    return reg_value;
}

static INLINE void gfec_reg_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read( gfec_buffer_t *b_ptr,
                                                                                                         lineotn_handle_t *h_ptr,
                                                                                                         UINT32 ofs,
                                                                                                         UINT32 len,
                                                                                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void gfec_reg_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read( gfec_buffer_t *b_ptr,
                                                                                                         lineotn_handle_t *h_ptr,
                                                                                                         UINT32 ofs,
                                                                                                         UINT32 len,
                                                                                                         UINT32 *value )
{
    if (len + ofs > 12)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "gfec_reg_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read", ofs, len, 12 );
    gfec_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_GFEC_REG_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "gfec_reg_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read", 12, ofs, len);
}

static INLINE UINT32 gfec_reg_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read( gfec_buffer_t *b_ptr,
                                                                                                     lineotn_handle_t *h_ptr,
                                                                                                     UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_reg_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read( gfec_buffer_t *b_ptr,
                                                                                                     lineotn_handle_t *h_ptr,
                                                                                                     UINT32 N )
{
    UINT32 reg_value;

    reg_value = gfec_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_GFEC_REG_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11(N));

    IOLOG( "%s -> 0x%08x;", "gfec_reg_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void gfec_field_REG_TX_ENCODE_TYPE_set( gfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_field_REG_TX_ENCODE_TYPE_set( gfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gfec_field_REG_TX_ENCODE_TYPE_set", value, 15);
    IOLOG( "%s <= 0x%08x", "gfec_field_REG_TX_ENCODE_TYPE_set", value );

    /* (0x00026000 bits 31:28) field REG_TX_ENCODE_TYPE of register PMC_GFEC_REG_RS_FEC_MODE */
    gfec_reg_RS_FEC_MODE_field_set( b_ptr,
                                    h_ptr,
                                    GFEC_REG_RS_FEC_MODE_BIT_REG_TX_ENCODE_TYPE_MSK,
                                    GFEC_REG_RS_FEC_MODE_BIT_REG_TX_ENCODE_TYPE_OFF,
                                    value);
}

static INLINE UINT32 gfec_field_REG_TX_ENCODE_TYPE_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_field_REG_TX_ENCODE_TYPE_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00026000 bits 31:28) field REG_TX_ENCODE_TYPE of register PMC_GFEC_REG_RS_FEC_MODE */
    reg_value = gfec_reg_RS_FEC_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & GFEC_REG_RS_FEC_MODE_BIT_REG_TX_ENCODE_TYPE_MSK) >> GFEC_REG_RS_FEC_MODE_BIT_REG_TX_ENCODE_TYPE_OFF;
    IOLOG( "%s -> 0x%08x", "gfec_field_REG_TX_ENCODE_TYPE_get", value );

    return value;
}
static INLINE void gfec_field_range_REG_TX_ENCODE_TYPE_set( gfec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_field_range_REG_TX_ENCODE_TYPE_set( gfec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gfec_field_range_REG_TX_ENCODE_TYPE_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gfec_field_range_REG_TX_ENCODE_TYPE_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "gfec_field_range_REG_TX_ENCODE_TYPE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00026000 bits 31:28) field REG_TX_ENCODE_TYPE of register PMC_GFEC_REG_RS_FEC_MODE */
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
        /* (0x00026000 bits 31:28) field REG_TX_ENCODE_TYPE of register PMC_GFEC_REG_RS_FEC_MODE */
        gfec_reg_RS_FEC_MODE_field_set( b_ptr,
                                        h_ptr,
                                        subfield_mask << (GFEC_REG_RS_FEC_MODE_BIT_REG_TX_ENCODE_TYPE_OFF + subfield_offset),
                                        GFEC_REG_RS_FEC_MODE_BIT_REG_TX_ENCODE_TYPE_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 gfec_field_range_REG_TX_ENCODE_TYPE_get( gfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_field_range_REG_TX_ENCODE_TYPE_get( gfec_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gfec_field_range_REG_TX_ENCODE_TYPE_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gfec_field_range_REG_TX_ENCODE_TYPE_get", stop_bit, 3 );
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
    /* (0x00026000 bits 31:28) field REG_TX_ENCODE_TYPE of register PMC_GFEC_REG_RS_FEC_MODE */
    reg_value = gfec_reg_RS_FEC_MODE_read(  b_ptr, h_ptr);
    field_value = (reg_value & GFEC_REG_RS_FEC_MODE_BIT_REG_TX_ENCODE_TYPE_MSK)
                  >> GFEC_REG_RS_FEC_MODE_BIT_REG_TX_ENCODE_TYPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GFEC_REG_RS_FEC_MODE_BIT_REG_TX_ENCODE_TYPE_MSK, GFEC_REG_RS_FEC_MODE_BIT_REG_TX_ENCODE_TYPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gfec_field_range_REG_TX_ENCODE_TYPE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void gfec_field_REG_RX_DECODE_TYPE_set( gfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_field_REG_RX_DECODE_TYPE_set( gfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gfec_field_REG_RX_DECODE_TYPE_set", value, 15);
    IOLOG( "%s <= 0x%08x", "gfec_field_REG_RX_DECODE_TYPE_set", value );

    /* (0x00026000 bits 27:24) field REG_RX_DECODE_TYPE of register PMC_GFEC_REG_RS_FEC_MODE */
    gfec_reg_RS_FEC_MODE_field_set( b_ptr,
                                    h_ptr,
                                    GFEC_REG_RS_FEC_MODE_BIT_REG_RX_DECODE_TYPE_MSK,
                                    GFEC_REG_RS_FEC_MODE_BIT_REG_RX_DECODE_TYPE_OFF,
                                    value);
}

static INLINE UINT32 gfec_field_REG_RX_DECODE_TYPE_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_field_REG_RX_DECODE_TYPE_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00026000 bits 27:24) field REG_RX_DECODE_TYPE of register PMC_GFEC_REG_RS_FEC_MODE */
    reg_value = gfec_reg_RS_FEC_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & GFEC_REG_RS_FEC_MODE_BIT_REG_RX_DECODE_TYPE_MSK) >> GFEC_REG_RS_FEC_MODE_BIT_REG_RX_DECODE_TYPE_OFF;
    IOLOG( "%s -> 0x%08x", "gfec_field_REG_RX_DECODE_TYPE_get", value );

    return value;
}
static INLINE void gfec_field_range_REG_RX_DECODE_TYPE_set( gfec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_field_range_REG_RX_DECODE_TYPE_set( gfec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gfec_field_range_REG_RX_DECODE_TYPE_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gfec_field_range_REG_RX_DECODE_TYPE_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "gfec_field_range_REG_RX_DECODE_TYPE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00026000 bits 27:24) field REG_RX_DECODE_TYPE of register PMC_GFEC_REG_RS_FEC_MODE */
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
        /* (0x00026000 bits 27:24) field REG_RX_DECODE_TYPE of register PMC_GFEC_REG_RS_FEC_MODE */
        gfec_reg_RS_FEC_MODE_field_set( b_ptr,
                                        h_ptr,
                                        subfield_mask << (GFEC_REG_RS_FEC_MODE_BIT_REG_RX_DECODE_TYPE_OFF + subfield_offset),
                                        GFEC_REG_RS_FEC_MODE_BIT_REG_RX_DECODE_TYPE_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 gfec_field_range_REG_RX_DECODE_TYPE_get( gfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_field_range_REG_RX_DECODE_TYPE_get( gfec_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gfec_field_range_REG_RX_DECODE_TYPE_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gfec_field_range_REG_RX_DECODE_TYPE_get", stop_bit, 3 );
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
    /* (0x00026000 bits 27:24) field REG_RX_DECODE_TYPE of register PMC_GFEC_REG_RS_FEC_MODE */
    reg_value = gfec_reg_RS_FEC_MODE_read(  b_ptr, h_ptr);
    field_value = (reg_value & GFEC_REG_RS_FEC_MODE_BIT_REG_RX_DECODE_TYPE_MSK)
                  >> GFEC_REG_RS_FEC_MODE_BIT_REG_RX_DECODE_TYPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GFEC_REG_RS_FEC_MODE_BIT_REG_RX_DECODE_TYPE_MSK, GFEC_REG_RS_FEC_MODE_BIT_REG_RX_DECODE_TYPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gfec_field_range_REG_RX_DECODE_TYPE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void gfec_field_REG_DISABLE_DEC_set( gfec_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_field_REG_DISABLE_DEC_set( gfec_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gfec_field_REG_DISABLE_DEC_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "gfec_field_REG_DISABLE_DEC_set", value );

    /* (0x00026000 bits 23:12) field REG_DISABLE_DEC of register PMC_GFEC_REG_RS_FEC_MODE */
    gfec_reg_RS_FEC_MODE_field_set( b_ptr,
                                    h_ptr,
                                    GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_DEC_MSK,
                                    GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_DEC_OFF,
                                    value);
}

static INLINE UINT32 gfec_field_REG_DISABLE_DEC_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_field_REG_DISABLE_DEC_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00026000 bits 23:12) field REG_DISABLE_DEC of register PMC_GFEC_REG_RS_FEC_MODE */
    reg_value = gfec_reg_RS_FEC_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_DEC_MSK) >> GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_DEC_OFF;
    IOLOG( "%s -> 0x%08x", "gfec_field_REG_DISABLE_DEC_get", value );

    return value;
}
static INLINE void gfec_field_range_REG_DISABLE_DEC_set( gfec_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_field_range_REG_DISABLE_DEC_set( gfec_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gfec_field_range_REG_DISABLE_DEC_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gfec_field_range_REG_DISABLE_DEC_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "gfec_field_range_REG_DISABLE_DEC_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00026000 bits 23:12) field REG_DISABLE_DEC of register PMC_GFEC_REG_RS_FEC_MODE */
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
        /* (0x00026000 bits 23:12) field REG_DISABLE_DEC of register PMC_GFEC_REG_RS_FEC_MODE */
        gfec_reg_RS_FEC_MODE_field_set( b_ptr,
                                        h_ptr,
                                        subfield_mask << (GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_DEC_OFF + subfield_offset),
                                        GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_DEC_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 gfec_field_range_REG_DISABLE_DEC_get( gfec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_field_range_REG_DISABLE_DEC_get( gfec_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gfec_field_range_REG_DISABLE_DEC_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gfec_field_range_REG_DISABLE_DEC_get", stop_bit, 11 );
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
    /* (0x00026000 bits 23:12) field REG_DISABLE_DEC of register PMC_GFEC_REG_RS_FEC_MODE */
    reg_value = gfec_reg_RS_FEC_MODE_read(  b_ptr, h_ptr);
    field_value = (reg_value & GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_DEC_MSK)
                  >> GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_DEC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_DEC_MSK, GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_DEC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gfec_field_range_REG_DISABLE_DEC_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void gfec_field_REG_SHUT_DOWN_set( gfec_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_field_REG_SHUT_DOWN_set( gfec_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gfec_field_REG_SHUT_DOWN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gfec_field_REG_SHUT_DOWN_set", value );

    /* (0x00026000 bits 7) field REG_SHUT_DOWN of register PMC_GFEC_REG_RS_FEC_MODE */
    gfec_reg_RS_FEC_MODE_field_set( b_ptr,
                                    h_ptr,
                                    GFEC_REG_RS_FEC_MODE_BIT_REG_SHUT_DOWN_MSK,
                                    GFEC_REG_RS_FEC_MODE_BIT_REG_SHUT_DOWN_OFF,
                                    value);
}

static INLINE UINT32 gfec_field_REG_SHUT_DOWN_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_field_REG_SHUT_DOWN_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00026000 bits 7) field REG_SHUT_DOWN of register PMC_GFEC_REG_RS_FEC_MODE */
    reg_value = gfec_reg_RS_FEC_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & GFEC_REG_RS_FEC_MODE_BIT_REG_SHUT_DOWN_MSK) >> GFEC_REG_RS_FEC_MODE_BIT_REG_SHUT_DOWN_OFF;
    IOLOG( "%s -> 0x%08x", "gfec_field_REG_SHUT_DOWN_get", value );

    return value;
}
static INLINE void gfec_field_REG_DEEP_SLEEP_set( gfec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_field_REG_DEEP_SLEEP_set( gfec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gfec_field_REG_DEEP_SLEEP_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gfec_field_REG_DEEP_SLEEP_set", value );

    /* (0x00026000 bits 6) field REG_DEEP_SLEEP of register PMC_GFEC_REG_RS_FEC_MODE */
    gfec_reg_RS_FEC_MODE_field_set( b_ptr,
                                    h_ptr,
                                    GFEC_REG_RS_FEC_MODE_BIT_REG_DEEP_SLEEP_MSK,
                                    GFEC_REG_RS_FEC_MODE_BIT_REG_DEEP_SLEEP_OFF,
                                    value);
}

static INLINE UINT32 gfec_field_REG_DEEP_SLEEP_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_field_REG_DEEP_SLEEP_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00026000 bits 6) field REG_DEEP_SLEEP of register PMC_GFEC_REG_RS_FEC_MODE */
    reg_value = gfec_reg_RS_FEC_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & GFEC_REG_RS_FEC_MODE_BIT_REG_DEEP_SLEEP_MSK) >> GFEC_REG_RS_FEC_MODE_BIT_REG_DEEP_SLEEP_OFF;
    IOLOG( "%s -> 0x%08x", "gfec_field_REG_DEEP_SLEEP_get", value );

    return value;
}
static INLINE void gfec_field_REG_DISABLE_UNCORR_set( gfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_field_REG_DISABLE_UNCORR_set( gfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gfec_field_REG_DISABLE_UNCORR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gfec_field_REG_DISABLE_UNCORR_set", value );

    /* (0x00026000 bits 5) field REG_DISABLE_UNCORR of register PMC_GFEC_REG_RS_FEC_MODE */
    gfec_reg_RS_FEC_MODE_field_set( b_ptr,
                                    h_ptr,
                                    GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_UNCORR_MSK,
                                    GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_UNCORR_OFF,
                                    value);
}

static INLINE UINT32 gfec_field_REG_DISABLE_UNCORR_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_field_REG_DISABLE_UNCORR_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00026000 bits 5) field REG_DISABLE_UNCORR of register PMC_GFEC_REG_RS_FEC_MODE */
    reg_value = gfec_reg_RS_FEC_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_UNCORR_MSK) >> GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_UNCORR_OFF;
    IOLOG( "%s -> 0x%08x", "gfec_field_REG_DISABLE_UNCORR_get", value );

    return value;
}
static INLINE void gfec_field_REG_DISABLE_SCRAM_set( gfec_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_field_REG_DISABLE_SCRAM_set( gfec_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gfec_field_REG_DISABLE_SCRAM_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gfec_field_REG_DISABLE_SCRAM_set", value );

    /* (0x00026000 bits 4) field REG_DISABLE_SCRAM of register PMC_GFEC_REG_RS_FEC_MODE */
    gfec_reg_RS_FEC_MODE_field_set( b_ptr,
                                    h_ptr,
                                    GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_SCRAM_MSK,
                                    GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_SCRAM_OFF,
                                    value);
}

static INLINE UINT32 gfec_field_REG_DISABLE_SCRAM_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_field_REG_DISABLE_SCRAM_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00026000 bits 4) field REG_DISABLE_SCRAM of register PMC_GFEC_REG_RS_FEC_MODE */
    reg_value = gfec_reg_RS_FEC_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_SCRAM_MSK) >> GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_SCRAM_OFF;
    IOLOG( "%s -> 0x%08x", "gfec_field_REG_DISABLE_SCRAM_get", value );

    return value;
}
static INLINE void gfec_field_FEC_SW_RST_set( gfec_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_field_FEC_SW_RST_set( gfec_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gfec_field_FEC_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gfec_field_FEC_SW_RST_set", value );

    /* (0x00026000 bits 3) field FEC_SW_RST of register PMC_GFEC_REG_RS_FEC_MODE */
    gfec_reg_RS_FEC_MODE_field_set( b_ptr,
                                    h_ptr,
                                    GFEC_REG_RS_FEC_MODE_BIT_FEC_SW_RST_MSK,
                                    GFEC_REG_RS_FEC_MODE_BIT_FEC_SW_RST_OFF,
                                    value);
}

static INLINE UINT32 gfec_field_FEC_SW_RST_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_field_FEC_SW_RST_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00026000 bits 3) field FEC_SW_RST of register PMC_GFEC_REG_RS_FEC_MODE */
    reg_value = gfec_reg_RS_FEC_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & GFEC_REG_RS_FEC_MODE_BIT_FEC_SW_RST_MSK) >> GFEC_REG_RS_FEC_MODE_BIT_FEC_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "gfec_field_FEC_SW_RST_get", value );

    return value;
}
static INLINE void gfec_field_GFEC_LOWPWR_set( gfec_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_field_GFEC_LOWPWR_set( gfec_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gfec_field_GFEC_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "gfec_field_GFEC_LOWPWR_set", value );

    /* (0x00026000 bits 0) field GFEC_LOWPWR of register PMC_GFEC_REG_RS_FEC_MODE */
    gfec_reg_RS_FEC_MODE_field_set( b_ptr,
                                    h_ptr,
                                    GFEC_REG_RS_FEC_MODE_BIT_GFEC_LOWPWR_MSK,
                                    GFEC_REG_RS_FEC_MODE_BIT_GFEC_LOWPWR_OFF,
                                    value);
}

static INLINE UINT32 gfec_field_GFEC_LOWPWR_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_field_GFEC_LOWPWR_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00026000 bits 0) field GFEC_LOWPWR of register PMC_GFEC_REG_RS_FEC_MODE */
    reg_value = gfec_reg_RS_FEC_MODE_read(  b_ptr, h_ptr);
    value = (reg_value & GFEC_REG_RS_FEC_MODE_BIT_GFEC_LOWPWR_MSK) >> GFEC_REG_RS_FEC_MODE_BIT_GFEC_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "gfec_field_GFEC_LOWPWR_get", value );

    return value;
}
static INLINE void gfec_field_LPA_STATS_COUNTER_MODE_set( gfec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_field_LPA_STATS_COUNTER_MODE_set( gfec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "gfec_field_LPA_STATS_COUNTER_MODE_set", value, 3);
    IOLOG( "%s <= 0x%08x", "gfec_field_LPA_STATS_COUNTER_MODE_set", value );

    /* (0x00026008 bits 1:0) bits 0:1 use field LPA_STATS_COUNTER_MODE of register PMC_GFEC_REG_LPA_STATISTICS_MODE_CFG */
    gfec_reg_LPA_STATISTICS_MODE_CFG_field_set( b_ptr,
                                                h_ptr,
                                                GFEC_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_MSK,
                                                GFEC_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_OFF,
                                                value);
}

static INLINE UINT32 gfec_field_LPA_STATS_COUNTER_MODE_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_field_LPA_STATS_COUNTER_MODE_get( gfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00026008 bits 1:0) bits 0:1 use field LPA_STATS_COUNTER_MODE of register PMC_GFEC_REG_LPA_STATISTICS_MODE_CFG */
    reg_value = gfec_reg_LPA_STATISTICS_MODE_CFG_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & GFEC_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_MSK) >> GFEC_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "gfec_field_LPA_STATS_COUNTER_MODE_get", value );

    return value;
}
static INLINE void gfec_field_range_LPA_STATS_COUNTER_MODE_set( gfec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void gfec_field_range_LPA_STATS_COUNTER_MODE_set( gfec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gfec_field_range_LPA_STATS_COUNTER_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gfec_field_range_LPA_STATS_COUNTER_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "gfec_field_range_LPA_STATS_COUNTER_MODE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00026008 bits 1:0) bits 0:1 use field LPA_STATS_COUNTER_MODE of register PMC_GFEC_REG_LPA_STATISTICS_MODE_CFG */
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
        /* (0x00026008 bits 1:0) bits 0:1 use field LPA_STATS_COUNTER_MODE of register PMC_GFEC_REG_LPA_STATISTICS_MODE_CFG */
        gfec_reg_LPA_STATISTICS_MODE_CFG_field_set( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (GFEC_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_OFF + subfield_offset),
                                                    GFEC_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 gfec_field_range_LPA_STATS_COUNTER_MODE_get( gfec_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 gfec_field_range_LPA_STATS_COUNTER_MODE_get( gfec_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "gfec_field_range_LPA_STATS_COUNTER_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "gfec_field_range_LPA_STATS_COUNTER_MODE_get", stop_bit, 1 );
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
    /* (0x00026008 bits 1:0) bits 0:1 use field LPA_STATS_COUNTER_MODE of register PMC_GFEC_REG_LPA_STATISTICS_MODE_CFG */
    reg_value = gfec_reg_LPA_STATISTICS_MODE_CFG_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & GFEC_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_MSK)
                  >> GFEC_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, GFEC_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_MSK, GFEC_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "gfec_field_range_LPA_STATS_COUNTER_MODE_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status_N_burstsize12
 * ==================================================================================
 */
static INLINE void gfec_lfield_LPA_STATS_FP_get( gfec_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 value[12] ) ALWAYS_INLINE;
static INLINE void gfec_lfield_LPA_STATS_FP_get( gfec_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 value[12] )
{
    IOLOG( "%s ", "gfec_lfield_LPA_STATS_FP_get");
    gfec_reg_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read( b_ptr,
                                                                                             h_ptr,
                                                                                             0,
                                                                                             12,
                                                                                             (UINT32 *)value);
}

static INLINE UINT32 gfec_lfield_range_LPA_STATS_FP_get( gfec_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 gfec_lfield_range_LPA_STATS_FP_get( gfec_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "gfec_lfield_range_LPA_STATS_FP_get", start_bit, stop_bit );
    }
    reg_value = gfec_reg_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read( b_ptr,
                                                                                                   h_ptr,
                                                                                                   word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "gfec_lfield_range_LPA_STATS_FP_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void gfec_lfield_LPA_STATS_UNCORR_CWDS_get( gfec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value[12] ) ALWAYS_INLINE;
static INLINE void gfec_lfield_LPA_STATS_UNCORR_CWDS_get( gfec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value[12] )
{
    IOLOG( "%s ", "gfec_lfield_LPA_STATS_UNCORR_CWDS_get");
    gfec_reg_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read( b_ptr,
                                                                                                   h_ptr,
                                                                                                   0,
                                                                                                   12,
                                                                                                   (UINT32 *)value);
}

static INLINE UINT32 gfec_lfield_range_LPA_STATS_UNCORR_CWDS_get( gfec_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 gfec_lfield_range_LPA_STATS_UNCORR_CWDS_get( gfec_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "gfec_lfield_range_LPA_STATS_UNCORR_CWDS_get", start_bit, stop_bit );
    }
    reg_value = gfec_reg_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read( b_ptr,
                                                                                                         h_ptr,
                                                                                                         word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "gfec_lfield_range_LPA_STATS_UNCORR_CWDS_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void gfec_lfield_LPA_STATS_CORR_1S_get( gfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value[12] ) ALWAYS_INLINE;
static INLINE void gfec_lfield_LPA_STATS_CORR_1S_get( gfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value[12] )
{
    IOLOG( "%s ", "gfec_lfield_LPA_STATS_CORR_1S_get");
    gfec_reg_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read( b_ptr,
                                                                                          h_ptr,
                                                                                          0,
                                                                                          12,
                                                                                          (UINT32 *)value);
}

static INLINE UINT32 gfec_lfield_range_LPA_STATS_CORR_1S_get( gfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 gfec_lfield_range_LPA_STATS_CORR_1S_get( gfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "gfec_lfield_range_LPA_STATS_CORR_1S_get", start_bit, stop_bit );
    }
    reg_value = gfec_reg_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read( b_ptr,
                                                                                                h_ptr,
                                                                                                word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "gfec_lfield_range_LPA_STATS_CORR_1S_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void gfec_lfield_LPA_STATS_CORR_0S_get( gfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value[12] ) ALWAYS_INLINE;
static INLINE void gfec_lfield_LPA_STATS_CORR_0S_get( gfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value[12] )
{
    IOLOG( "%s ", "gfec_lfield_LPA_STATS_CORR_0S_get");
    gfec_reg_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_burst_read( b_ptr,
                                                                                          h_ptr,
                                                                                          0,
                                                                                          12,
                                                                                          (UINT32 *)value);
}

static INLINE UINT32 gfec_lfield_range_LPA_STATS_CORR_0S_get( gfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 gfec_lfield_range_LPA_STATS_CORR_0S_get( gfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "gfec_lfield_range_LPA_STATS_CORR_0S_get", start_bit, stop_bit );
    }
    reg_value = gfec_reg_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11_array_read( b_ptr,
                                                                                                h_ptr,
                                                                                                word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "gfec_lfield_range_LPA_STATS_CORR_0S_get", start_bit, stop_bit, value );
    return value;
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _GFEC_IO_INLINE_H */
