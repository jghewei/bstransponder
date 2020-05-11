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
 *     and register accessor functions for the sifd_dft block
 *****************************************************************************/
#ifndef _SIFD_DFT_IO_INLINE_H
#define _SIFD_DFT_IO_INLINE_H

#include "sifd_api.h"
#include "sifd_dft_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SIFD_DFT_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for sifd_dft
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
    sifd_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} sifd_dft_buffer_t;
static INLINE void sifd_dft_buffer_init( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void sifd_dft_buffer_init( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "sifd_dft_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void sifd_dft_buffer_flush( sifd_dft_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sifd_dft_buffer_flush( sifd_dft_buffer_t *b_ptr )
{
    IOLOG( "sifd_dft_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 sifd_dft_reg_read( sifd_dft_buffer_t *b_ptr,
                                        sifd_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_reg_read( sifd_dft_buffer_t *b_ptr,
                                        sifd_handle_t *h_ptr,
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
static INLINE void sifd_dft_reg_write( sifd_dft_buffer_t *b_ptr,
                                       sifd_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_write( sifd_dft_buffer_t *b_ptr,
                                       sifd_handle_t *h_ptr,
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

static INLINE void sifd_dft_field_set( sifd_dft_buffer_t *b_ptr,
                                       sifd_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 mask,
                                       UINT32 unused_mask,
                                       UINT32 ofs,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_set( sifd_dft_buffer_t *b_ptr,
                                       sifd_handle_t *h_ptr,
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

static INLINE void sifd_dft_action_on_write_field_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_action_on_write_field_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
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

static INLINE void sifd_dft_burst_read( sifd_dft_buffer_t *b_ptr,
                                        sifd_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void sifd_dft_burst_read( sifd_dft_buffer_t *b_ptr,
                                        sifd_handle_t *h_ptr,
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

static INLINE void sifd_dft_burst_write( sifd_dft_buffer_t *b_ptr,
                                         sifd_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void sifd_dft_burst_write( sifd_dft_buffer_t *b_ptr,
                                         sifd_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE sifd_dft_poll( sifd_dft_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 mem_type,
                                                  UINT32 reg,
                                                  UINT32 mask,
                                                  UINT32 value,
                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                  UINT32 max_count,
                                                  UINT32 *num_failed_polls,
                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sifd_dft_poll( sifd_dft_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
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
 *  register access functions for sifd_dft
 * ==================================================================================
 */

static INLINE void sifd_dft_reg_SOFT_RESET_write( sifd_dft_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_SOFT_RESET_write( sifd_dft_buffer_t *b_ptr,
                                                  sifd_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sifd_dft_reg_SOFT_RESET_write", value );
    sifd_dft_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_SOFT_RESET,
                        value);
}

static INLINE void sifd_dft_reg_SOFT_RESET_field_set( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_SOFT_RESET_field_set( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sifd_dft_reg_SOFT_RESET_field_set", mask, ofs, value );
    sifd_dft_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_SOFT_RESET,
                        mask,
                        PMC_SIFD_DFT_REG_SOFT_RESET_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sifd_dft_reg_SOFT_RESET_read( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_reg_SOFT_RESET_read( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sifd_dft_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SIFD_DFT_REG_SOFT_RESET);

    IOLOG( "%s -> 0x%08x;", "sifd_dft_reg_SOFT_RESET_read", reg_value);
    return reg_value;
}

static INLINE void sifd_dft_reg_LOW_POWER_CFG_write( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_LOW_POWER_CFG_write( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sifd_dft_reg_LOW_POWER_CFG_write", value );
    sifd_dft_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_LOW_POWER_CFG,
                        value);
}

static INLINE void sifd_dft_reg_LOW_POWER_CFG_field_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_LOW_POWER_CFG_field_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sifd_dft_reg_LOW_POWER_CFG_field_set", mask, ofs, value );
    sifd_dft_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_LOW_POWER_CFG,
                        mask,
                        PMC_SIFD_DFT_REG_LOW_POWER_CFG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sifd_dft_reg_LOW_POWER_CFG_read( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_reg_LOW_POWER_CFG_read( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sifd_dft_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SIFD_DFT_REG_LOW_POWER_CFG);

    IOLOG( "%s -> 0x%08x;", "sifd_dft_reg_LOW_POWER_CFG_read", reg_value);
    return reg_value;
}

static INLINE void sifd_dft_reg_RAM_LOW_POWER_CFG_write( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_RAM_LOW_POWER_CFG_write( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sifd_dft_reg_RAM_LOW_POWER_CFG_write", value );
    sifd_dft_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_RAM_LOW_POWER_CFG,
                        value);
}

static INLINE void sifd_dft_reg_RAM_LOW_POWER_CFG_field_set( sifd_dft_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_RAM_LOW_POWER_CFG_field_set( sifd_dft_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sifd_dft_reg_RAM_LOW_POWER_CFG_field_set", mask, ofs, value );
    sifd_dft_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_RAM_LOW_POWER_CFG,
                        mask,
                        PMC_SIFD_DFT_REG_RAM_LOW_POWER_CFG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sifd_dft_reg_RAM_LOW_POWER_CFG_read( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_reg_RAM_LOW_POWER_CFG_read( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sifd_dft_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SIFD_DFT_REG_RAM_LOW_POWER_CFG);

    IOLOG( "%s -> 0x%08x;", "sifd_dft_reg_RAM_LOW_POWER_CFG_read", reg_value);
    return reg_value;
}

static INLINE void sifd_dft_reg_FIC_SERDES_LANE_SELECT_write( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_FIC_SERDES_LANE_SELECT_write( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sifd_dft_reg_FIC_SERDES_LANE_SELECT_write", value );
    sifd_dft_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT,
                        value);
}

static INLINE void sifd_dft_reg_FIC_SERDES_LANE_SELECT_field_set( sifd_dft_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_FIC_SERDES_LANE_SELECT_field_set( sifd_dft_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sifd_dft_reg_FIC_SERDES_LANE_SELECT_field_set", mask, ofs, value );
    sifd_dft_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT,
                        mask,
                        PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sifd_dft_reg_FIC_SERDES_LANE_SELECT_read( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_reg_FIC_SERDES_LANE_SELECT_read( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sifd_dft_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT);

    IOLOG( "%s -> 0x%08x;", "sifd_dft_reg_FIC_SERDES_LANE_SELECT_read", reg_value);
    return reg_value;
}

static INLINE void sifd_dft_reg_COPI_FIC_DPI_SEL_write( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_COPI_FIC_DPI_SEL_write( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sifd_dft_reg_COPI_FIC_DPI_SEL_write", value );
    sifd_dft_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_COPI_FIC_DPI_SEL,
                        value);
}

static INLINE void sifd_dft_reg_COPI_FIC_DPI_SEL_field_set( sifd_dft_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_COPI_FIC_DPI_SEL_field_set( sifd_dft_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sifd_dft_reg_COPI_FIC_DPI_SEL_field_set", mask, ofs, value );
    sifd_dft_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_COPI_FIC_DPI_SEL,
                        mask,
                        PMC_SIFD_DFT_REG_COPI_FIC_DPI_SEL_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sifd_dft_reg_COPI_FIC_DPI_SEL_read( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_reg_COPI_FIC_DPI_SEL_read( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sifd_dft_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SIFD_DFT_REG_COPI_FIC_DPI_SEL);

    IOLOG( "%s -> 0x%08x;", "sifd_dft_reg_COPI_FIC_DPI_SEL_read", reg_value);
    return reg_value;
}

static INLINE void sifd_dft_reg_SYSTEM_N1_SERDES_ACB_MUX_SEL_write( sifd_dft_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_SYSTEM_N1_SERDES_ACB_MUX_SEL_write( sifd_dft_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sifd_dft_reg_SYSTEM_N1_SERDES_ACB_MUX_SEL_write", value );
    sifd_dft_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL,
                        value);
}

static INLINE void sifd_dft_reg_SYSTEM_N1_SERDES_ACB_MUX_SEL_field_set( sifd_dft_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_SYSTEM_N1_SERDES_ACB_MUX_SEL_field_set( sifd_dft_buffer_t *b_ptr,
                                                                        sifd_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sifd_dft_reg_SYSTEM_N1_SERDES_ACB_MUX_SEL_field_set", mask, ofs, value );
    sifd_dft_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL,
                        mask,
                        PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sifd_dft_reg_SYSTEM_N1_SERDES_ACB_MUX_SEL_read( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_reg_SYSTEM_N1_SERDES_ACB_MUX_SEL_read( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sifd_dft_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL);

    IOLOG( "%s -> 0x%08x;", "sifd_dft_reg_SYSTEM_N1_SERDES_ACB_MUX_SEL_read", reg_value);
    return reg_value;
}

static INLINE void sifd_dft_reg_DSIS_CFGS_array_write( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_DSIS_CFGS_array_write( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sifd_dft_reg_DSIS_CFGS_array_write", value );
    sifd_dft_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_DSIS_CFGS(N),
                        value);
}

static INLINE void sifd_dft_reg_DSIS_CFGS_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_DSIS_CFGS_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sifd_dft_reg_DSIS_CFGS_array_field_set", N, mask, ofs, value );
    sifd_dft_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_DSIS_CFGS(N),
                        mask,
                        PMC_SIFD_DFT_REG_DSIS_CFGS_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sifd_dft_reg_DSIS_CFGS_array_read( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_reg_DSIS_CFGS_array_read( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = sifd_dft_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SIFD_DFT_REG_DSIS_CFGS(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sifd_dft_reg_DSIS_CFGS_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_write( sifd_dft_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_write( sifd_dft_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_write", value );
    sifd_dft_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS(N),
                        value);
}

static INLINE void sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_field_set", N, mask, ofs, value );
    sifd_dft_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS(N),
                        mask,
                        PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_read( sifd_dft_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_read( sifd_dft_buffer_t *b_ptr,
                                                                    sifd_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = sifd_dft_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sifd_dft_reg_INTERLAKEN_1_CFGS_array_write( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_INTERLAKEN_1_CFGS_array_write( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sifd_dft_reg_INTERLAKEN_1_CFGS_array_write", value );
    sifd_dft_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS(N),
                        value);
}

static INLINE void sifd_dft_reg_INTERLAKEN_1_CFGS_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_INTERLAKEN_1_CFGS_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sifd_dft_reg_INTERLAKEN_1_CFGS_array_field_set", N, mask, ofs, value );
    sifd_dft_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS(N),
                        mask,
                        PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sifd_dft_reg_INTERLAKEN_1_CFGS_array_read( sifd_dft_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_reg_INTERLAKEN_1_CFGS_array_read( sifd_dft_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = sifd_dft_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sifd_dft_reg_INTERLAKEN_1_CFGS_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sifd_dft_reg_INTERLAKEN_2_CFGS_array_write( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_INTERLAKEN_2_CFGS_array_write( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sifd_dft_reg_INTERLAKEN_2_CFGS_array_write", value );
    sifd_dft_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS(N),
                        value);
}

static INLINE void sifd_dft_reg_INTERLAKEN_2_CFGS_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_INTERLAKEN_2_CFGS_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sifd_dft_reg_INTERLAKEN_2_CFGS_array_field_set", N, mask, ofs, value );
    sifd_dft_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS(N),
                        mask,
                        PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sifd_dft_reg_INTERLAKEN_2_CFGS_array_read( sifd_dft_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_reg_INTERLAKEN_2_CFGS_array_read( sifd_dft_buffer_t *b_ptr,
                                                                sifd_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = sifd_dft_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sifd_dft_reg_INTERLAKEN_2_CFGS_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sifd_dft_reg_FIC_WORK_CFGS_array_write( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_FIC_WORK_CFGS_array_write( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sifd_dft_reg_FIC_WORK_CFGS_array_write", value );
    sifd_dft_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_FIC_WORK_CFGS(N),
                        value);
}

static INLINE void sifd_dft_reg_FIC_WORK_CFGS_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_FIC_WORK_CFGS_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sifd_dft_reg_FIC_WORK_CFGS_array_field_set", N, mask, ofs, value );
    sifd_dft_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_FIC_WORK_CFGS(N),
                        mask,
                        PMC_SIFD_DFT_REG_FIC_WORK_CFGS_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sifd_dft_reg_FIC_WORK_CFGS_array_read( sifd_dft_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_reg_FIC_WORK_CFGS_array_read( sifd_dft_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = sifd_dft_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SIFD_DFT_REG_FIC_WORK_CFGS(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sifd_dft_reg_FIC_WORK_CFGS_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sifd_dft_reg_FIC_PROTECT_CFGS_array_write( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_FIC_PROTECT_CFGS_array_write( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sifd_dft_reg_FIC_PROTECT_CFGS_array_write", value );
    sifd_dft_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_FIC_PROTECT_CFGS(N),
                        value);
}

static INLINE void sifd_dft_reg_FIC_PROTECT_CFGS_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_FIC_PROTECT_CFGS_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                                  sifd_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sifd_dft_reg_FIC_PROTECT_CFGS_array_field_set", N, mask, ofs, value );
    sifd_dft_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_FIC_PROTECT_CFGS(N),
                        mask,
                        PMC_SIFD_DFT_REG_FIC_PROTECT_CFGS_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sifd_dft_reg_FIC_PROTECT_CFGS_array_read( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_reg_FIC_PROTECT_CFGS_array_read( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = sifd_dft_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SIFD_DFT_REG_FIC_PROTECT_CFGS(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sifd_dft_reg_FIC_PROTECT_CFGS_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sifd_dft_reg_SIFD_INT_EN_array_write( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_SIFD_INT_EN_array_write( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sifd_dft_reg_SIFD_INT_EN_array_write", value );
    sifd_dft_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_SIFD_INT_EN(N),
                        value);
}

static INLINE void sifd_dft_reg_SIFD_INT_EN_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_SIFD_INT_EN_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sifd_dft_reg_SIFD_INT_EN_array_field_set", N, mask, ofs, value );
    sifd_dft_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_SIFD_INT_EN(N),
                        mask,
                        PMC_SIFD_DFT_REG_SIFD_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sifd_dft_reg_SIFD_INT_EN_array_read( sifd_dft_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_reg_SIFD_INT_EN_array_read( sifd_dft_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 reg_value;

    reg_value = sifd_dft_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SIFD_DFT_REG_SIFD_INT_EN(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sifd_dft_reg_SIFD_INT_EN_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_write( sifd_dft_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_write( sifd_dft_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_write", value );
    sifd_dft_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS(N),
                        value);
}

static INLINE void sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                                             sifd_handle_t *h_ptr,
                                                                             UINT32  N,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                                             sifd_handle_t *h_ptr,
                                                                             UINT32  N,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_field_set", N, mask, ofs, value );
    sifd_dft_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS(N),
                        mask,
                        PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_read( sifd_dft_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_read( sifd_dft_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32  N )
{
    UINT32 reg_value;

    reg_value = sifd_dft_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sifd_dft_reg_LINE_OTN_CFGS_array_write( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_LINE_OTN_CFGS_array_write( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sifd_dft_reg_LINE_OTN_CFGS_array_write", value );
    sifd_dft_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_LINE_OTN_CFGS(N),
                        value);
}

static INLINE void sifd_dft_reg_LINE_OTN_CFGS_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_LINE_OTN_CFGS_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sifd_dft_reg_LINE_OTN_CFGS_array_field_set", N, mask, ofs, value );
    sifd_dft_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_LINE_OTN_CFGS(N),
                        mask,
                        PMC_SIFD_DFT_REG_LINE_OTN_CFGS_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sifd_dft_reg_LINE_OTN_CFGS_array_read( sifd_dft_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_reg_LINE_OTN_CFGS_array_read( sifd_dft_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = sifd_dft_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SIFD_DFT_REG_LINE_OTN_CFGS(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sifd_dft_reg_LINE_OTN_CFGS_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_write( sifd_dft_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_write( sifd_dft_buffer_t *b_ptr,
                                                                         sifd_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_write", value );
    sifd_dft_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS(N),
                        value);
}

static INLINE void sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                                             sifd_handle_t *h_ptr,
                                                                             UINT32  N,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_field_set( sifd_dft_buffer_t *b_ptr,
                                                                             sifd_handle_t *h_ptr,
                                                                             UINT32  N,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_field_set", N, mask, ofs, value );
    sifd_dft_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS(N),
                        mask,
                        PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_read( sifd_dft_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_read( sifd_dft_buffer_t *b_ptr,
                                                                          sifd_handle_t *h_ptr,
                                                                          UINT32  N )
{
    UINT32 reg_value;

    reg_value = sifd_dft_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 sifd_dft_reg_SIFD_SUBSYSTEM_INT_STAT_read( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_reg_SIFD_SUBSYSTEM_INT_STAT_read( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sifd_dft_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "sifd_dft_reg_SIFD_SUBSYSTEM_INT_STAT_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void sifd_dft_field_FIC_SOFT_RST_set( sifd_dft_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_SOFT_RST_set( sifd_dft_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_SOFT_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_FIC_SOFT_RST_set", value );

    /* (0x00000000 bits 4) field FIC_SOFT_RST of register PMC_SIFD_DFT_REG_SOFT_RESET */
    sifd_dft_reg_SOFT_RESET_field_set( b_ptr,
                                       h_ptr,
                                       SIFD_DFT_REG_SOFT_RESET_BIT_FIC_SOFT_RST_MSK,
                                       SIFD_DFT_REG_SOFT_RESET_BIT_FIC_SOFT_RST_OFF,
                                       value);
}

static INLINE UINT32 sifd_dft_field_FIC_SOFT_RST_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_SOFT_RST_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 4) field FIC_SOFT_RST of register PMC_SIFD_DFT_REG_SOFT_RESET */
    reg_value = sifd_dft_reg_SOFT_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & SIFD_DFT_REG_SOFT_RESET_BIT_FIC_SOFT_RST_MSK) >> SIFD_DFT_REG_SOFT_RESET_BIT_FIC_SOFT_RST_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_FIC_SOFT_RST_get", value );

    return value;
}
static INLINE void sifd_dft_field_DSIS_SOFT_RST_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_DSIS_SOFT_RST_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_DSIS_SOFT_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_DSIS_SOFT_RST_set", value );

    /* (0x00000000 bits 3) field DSIS_SOFT_RST of register PMC_SIFD_DFT_REG_SOFT_RESET */
    sifd_dft_reg_SOFT_RESET_field_set( b_ptr,
                                       h_ptr,
                                       SIFD_DFT_REG_SOFT_RESET_BIT_DSIS_SOFT_RST_MSK,
                                       SIFD_DFT_REG_SOFT_RESET_BIT_DSIS_SOFT_RST_OFF,
                                       value);
}

static INLINE UINT32 sifd_dft_field_DSIS_SOFT_RST_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_DSIS_SOFT_RST_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 3) field DSIS_SOFT_RST of register PMC_SIFD_DFT_REG_SOFT_RESET */
    reg_value = sifd_dft_reg_SOFT_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & SIFD_DFT_REG_SOFT_RESET_BIT_DSIS_SOFT_RST_MSK) >> SIFD_DFT_REG_SOFT_RESET_BIT_DSIS_SOFT_RST_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_DSIS_SOFT_RST_get", value );

    return value;
}
static INLINE void sifd_dft_field_COPI_SOFT_RST_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_COPI_SOFT_RST_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_COPI_SOFT_RST_set", value, 3);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_COPI_SOFT_RST_set", value );

    /* (0x00000000 bits 2:1) bits 0:1 use field COPI_SOFT_RST of register PMC_SIFD_DFT_REG_SOFT_RESET */
    sifd_dft_reg_SOFT_RESET_field_set( b_ptr,
                                       h_ptr,
                                       SIFD_DFT_REG_SOFT_RESET_BIT_COPI_SOFT_RST_MSK,
                                       SIFD_DFT_REG_SOFT_RESET_BIT_COPI_SOFT_RST_OFF,
                                       value);
}

static INLINE UINT32 sifd_dft_field_COPI_SOFT_RST_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_COPI_SOFT_RST_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 2:1) bits 0:1 use field COPI_SOFT_RST of register PMC_SIFD_DFT_REG_SOFT_RESET */
    reg_value = sifd_dft_reg_SOFT_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & SIFD_DFT_REG_SOFT_RESET_BIT_COPI_SOFT_RST_MSK) >> SIFD_DFT_REG_SOFT_RESET_BIT_COPI_SOFT_RST_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_COPI_SOFT_RST_get", value );

    return value;
}
static INLINE void sifd_dft_field_range_COPI_SOFT_RST_set( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_range_COPI_SOFT_RST_set( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_COPI_SOFT_RST_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_COPI_SOFT_RST_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_COPI_SOFT_RST_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000000 bits 2:1) bits 0:1 use field COPI_SOFT_RST of register PMC_SIFD_DFT_REG_SOFT_RESET */
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
        /* (0x00000000 bits 2:1) bits 0:1 use field COPI_SOFT_RST of register PMC_SIFD_DFT_REG_SOFT_RESET */
        sifd_dft_reg_SOFT_RESET_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (SIFD_DFT_REG_SOFT_RESET_BIT_COPI_SOFT_RST_OFF + subfield_offset),
                                           SIFD_DFT_REG_SOFT_RESET_BIT_COPI_SOFT_RST_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 sifd_dft_field_range_COPI_SOFT_RST_get( sifd_dft_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_range_COPI_SOFT_RST_get( sifd_dft_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_COPI_SOFT_RST_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_COPI_SOFT_RST_get", stop_bit, 1 );
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
    /* (0x00000000 bits 2:1) bits 0:1 use field COPI_SOFT_RST of register PMC_SIFD_DFT_REG_SOFT_RESET */
    reg_value = sifd_dft_reg_SOFT_RESET_read(  b_ptr, h_ptr);
    field_value = (reg_value & SIFD_DFT_REG_SOFT_RESET_BIT_COPI_SOFT_RST_MSK)
                  >> SIFD_DFT_REG_SOFT_RESET_BIT_COPI_SOFT_RST_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SIFD_DFT_REG_SOFT_RESET_BIT_COPI_SOFT_RST_MSK, SIFD_DFT_REG_SOFT_RESET_BIT_COPI_SOFT_RST_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_COPI_SOFT_RST_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sifd_dft_field_SIFD_DFT_SOFT_RST_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_SIFD_DFT_SOFT_RST_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_SIFD_DFT_SOFT_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_SIFD_DFT_SOFT_RST_set", value );

    /* (0x00000000 bits 0) field SIFD_DFT_SOFT_RST of register PMC_SIFD_DFT_REG_SOFT_RESET */
    sifd_dft_reg_SOFT_RESET_field_set( b_ptr,
                                       h_ptr,
                                       SIFD_DFT_REG_SOFT_RESET_BIT_SIFD_DFT_SOFT_RST_MSK,
                                       SIFD_DFT_REG_SOFT_RESET_BIT_SIFD_DFT_SOFT_RST_OFF,
                                       value);
}

static INLINE UINT32 sifd_dft_field_SIFD_DFT_SOFT_RST_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_SIFD_DFT_SOFT_RST_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field SIFD_DFT_SOFT_RST of register PMC_SIFD_DFT_REG_SOFT_RESET */
    reg_value = sifd_dft_reg_SOFT_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & SIFD_DFT_REG_SOFT_RESET_BIT_SIFD_DFT_SOFT_RST_MSK) >> SIFD_DFT_REG_SOFT_RESET_BIT_SIFD_DFT_SOFT_RST_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_SIFD_DFT_SOFT_RST_get", value );

    return value;
}
static INLINE void sifd_dft_field_FIC_LOWPWR_CFG_set( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_LOWPWR_CFG_set( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_LOWPWR_CFG_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_FIC_LOWPWR_CFG_set", value );

    /* (0x00000004 bits 4) field FIC_LOWPWR_CFG of register PMC_SIFD_DFT_REG_LOW_POWER_CFG */
    sifd_dft_reg_LOW_POWER_CFG_field_set( b_ptr,
                                          h_ptr,
                                          SIFD_DFT_REG_LOW_POWER_CFG_BIT_FIC_LOWPWR_CFG_MSK,
                                          SIFD_DFT_REG_LOW_POWER_CFG_BIT_FIC_LOWPWR_CFG_OFF,
                                          value);
}

static INLINE UINT32 sifd_dft_field_FIC_LOWPWR_CFG_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_LOWPWR_CFG_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 4) field FIC_LOWPWR_CFG of register PMC_SIFD_DFT_REG_LOW_POWER_CFG */
    reg_value = sifd_dft_reg_LOW_POWER_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & SIFD_DFT_REG_LOW_POWER_CFG_BIT_FIC_LOWPWR_CFG_MSK) >> SIFD_DFT_REG_LOW_POWER_CFG_BIT_FIC_LOWPWR_CFG_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_FIC_LOWPWR_CFG_get", value );

    return value;
}
static INLINE void sifd_dft_field_DSIS_LOWPWR_CFG_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_DSIS_LOWPWR_CFG_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_DSIS_LOWPWR_CFG_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_DSIS_LOWPWR_CFG_set", value );

    /* (0x00000004 bits 3) field DSIS_LOWPWR_CFG of register PMC_SIFD_DFT_REG_LOW_POWER_CFG */
    sifd_dft_reg_LOW_POWER_CFG_field_set( b_ptr,
                                          h_ptr,
                                          SIFD_DFT_REG_LOW_POWER_CFG_BIT_DSIS_LOWPWR_CFG_MSK,
                                          SIFD_DFT_REG_LOW_POWER_CFG_BIT_DSIS_LOWPWR_CFG_OFF,
                                          value);
}

static INLINE UINT32 sifd_dft_field_DSIS_LOWPWR_CFG_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_DSIS_LOWPWR_CFG_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 3) field DSIS_LOWPWR_CFG of register PMC_SIFD_DFT_REG_LOW_POWER_CFG */
    reg_value = sifd_dft_reg_LOW_POWER_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & SIFD_DFT_REG_LOW_POWER_CFG_BIT_DSIS_LOWPWR_CFG_MSK) >> SIFD_DFT_REG_LOW_POWER_CFG_BIT_DSIS_LOWPWR_CFG_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_DSIS_LOWPWR_CFG_get", value );

    return value;
}
static INLINE void sifd_dft_field_COPI_LOWPWR_CFG_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_COPI_LOWPWR_CFG_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_COPI_LOWPWR_CFG_set", value, 3);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_COPI_LOWPWR_CFG_set", value );

    /* (0x00000004 bits 2:1) bits 0:1 use field COPI_LOWPWR_CFG of register PMC_SIFD_DFT_REG_LOW_POWER_CFG */
    sifd_dft_reg_LOW_POWER_CFG_field_set( b_ptr,
                                          h_ptr,
                                          SIFD_DFT_REG_LOW_POWER_CFG_BIT_COPI_LOWPWR_CFG_MSK,
                                          SIFD_DFT_REG_LOW_POWER_CFG_BIT_COPI_LOWPWR_CFG_OFF,
                                          value);
}

static INLINE UINT32 sifd_dft_field_COPI_LOWPWR_CFG_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_COPI_LOWPWR_CFG_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 2:1) bits 0:1 use field COPI_LOWPWR_CFG of register PMC_SIFD_DFT_REG_LOW_POWER_CFG */
    reg_value = sifd_dft_reg_LOW_POWER_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & SIFD_DFT_REG_LOW_POWER_CFG_BIT_COPI_LOWPWR_CFG_MSK) >> SIFD_DFT_REG_LOW_POWER_CFG_BIT_COPI_LOWPWR_CFG_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_COPI_LOWPWR_CFG_get", value );

    return value;
}
static INLINE void sifd_dft_field_range_COPI_LOWPWR_CFG_set( sifd_dft_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_range_COPI_LOWPWR_CFG_set( sifd_dft_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_COPI_LOWPWR_CFG_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_COPI_LOWPWR_CFG_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_COPI_LOWPWR_CFG_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000004 bits 2:1) bits 0:1 use field COPI_LOWPWR_CFG of register PMC_SIFD_DFT_REG_LOW_POWER_CFG */
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
        /* (0x00000004 bits 2:1) bits 0:1 use field COPI_LOWPWR_CFG of register PMC_SIFD_DFT_REG_LOW_POWER_CFG */
        sifd_dft_reg_LOW_POWER_CFG_field_set( b_ptr,
                                              h_ptr,
                                              subfield_mask << (SIFD_DFT_REG_LOW_POWER_CFG_BIT_COPI_LOWPWR_CFG_OFF + subfield_offset),
                                              SIFD_DFT_REG_LOW_POWER_CFG_BIT_COPI_LOWPWR_CFG_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 sifd_dft_field_range_COPI_LOWPWR_CFG_get( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_range_COPI_LOWPWR_CFG_get( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_COPI_LOWPWR_CFG_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_COPI_LOWPWR_CFG_get", stop_bit, 1 );
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
    /* (0x00000004 bits 2:1) bits 0:1 use field COPI_LOWPWR_CFG of register PMC_SIFD_DFT_REG_LOW_POWER_CFG */
    reg_value = sifd_dft_reg_LOW_POWER_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & SIFD_DFT_REG_LOW_POWER_CFG_BIT_COPI_LOWPWR_CFG_MSK)
                  >> SIFD_DFT_REG_LOW_POWER_CFG_BIT_COPI_LOWPWR_CFG_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SIFD_DFT_REG_LOW_POWER_CFG_BIT_COPI_LOWPWR_CFG_MSK, SIFD_DFT_REG_LOW_POWER_CFG_BIT_COPI_LOWPWR_CFG_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_COPI_LOWPWR_CFG_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sifd_dft_field_SIFD_DFT_LOWPWR_CFG_set( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_SIFD_DFT_LOWPWR_CFG_set( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_SIFD_DFT_LOWPWR_CFG_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_SIFD_DFT_LOWPWR_CFG_set", value );

    /* (0x00000004 bits 0) field SIFD_DFT_LOWPWR_CFG of register PMC_SIFD_DFT_REG_LOW_POWER_CFG */
    sifd_dft_reg_LOW_POWER_CFG_field_set( b_ptr,
                                          h_ptr,
                                          SIFD_DFT_REG_LOW_POWER_CFG_BIT_SIFD_DFT_LOWPWR_CFG_MSK,
                                          SIFD_DFT_REG_LOW_POWER_CFG_BIT_SIFD_DFT_LOWPWR_CFG_OFF,
                                          value);
}

static INLINE UINT32 sifd_dft_field_SIFD_DFT_LOWPWR_CFG_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_SIFD_DFT_LOWPWR_CFG_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 0) field SIFD_DFT_LOWPWR_CFG of register PMC_SIFD_DFT_REG_LOW_POWER_CFG */
    reg_value = sifd_dft_reg_LOW_POWER_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & SIFD_DFT_REG_LOW_POWER_CFG_BIT_SIFD_DFT_LOWPWR_CFG_MSK) >> SIFD_DFT_REG_LOW_POWER_CFG_BIT_SIFD_DFT_LOWPWR_CFG_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_SIFD_DFT_LOWPWR_CFG_get", value );

    return value;
}
static INLINE void sifd_dft_field_FIC_RAM_LOWPWR_CFG_set( sifd_dft_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_RAM_LOWPWR_CFG_set( sifd_dft_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_RAM_LOWPWR_CFG_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_FIC_RAM_LOWPWR_CFG_set", value );

    /* (0x0000000c bits 2) field FIC_RAM_LOWPWR_CFG of register PMC_SIFD_DFT_REG_RAM_LOW_POWER_CFG */
    sifd_dft_reg_RAM_LOW_POWER_CFG_field_set( b_ptr,
                                              h_ptr,
                                              SIFD_DFT_REG_RAM_LOW_POWER_CFG_BIT_FIC_RAM_LOWPWR_CFG_MSK,
                                              SIFD_DFT_REG_RAM_LOW_POWER_CFG_BIT_FIC_RAM_LOWPWR_CFG_OFF,
                                              value);
}

static INLINE UINT32 sifd_dft_field_FIC_RAM_LOWPWR_CFG_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_RAM_LOWPWR_CFG_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 2) field FIC_RAM_LOWPWR_CFG of register PMC_SIFD_DFT_REG_RAM_LOW_POWER_CFG */
    reg_value = sifd_dft_reg_RAM_LOW_POWER_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & SIFD_DFT_REG_RAM_LOW_POWER_CFG_BIT_FIC_RAM_LOWPWR_CFG_MSK) >> SIFD_DFT_REG_RAM_LOW_POWER_CFG_BIT_FIC_RAM_LOWPWR_CFG_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_FIC_RAM_LOWPWR_CFG_get", value );

    return value;
}
static INLINE void sifd_dft_field_COPI_RAM_LOWPWR_CFG_set( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_COPI_RAM_LOWPWR_CFG_set( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_COPI_RAM_LOWPWR_CFG_set", value, 3);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_COPI_RAM_LOWPWR_CFG_set", value );

    /* (0x0000000c bits 1:0) bits 0:1 use field COPI_RAM_LOWPWR_CFG of register PMC_SIFD_DFT_REG_RAM_LOW_POWER_CFG */
    sifd_dft_reg_RAM_LOW_POWER_CFG_field_set( b_ptr,
                                              h_ptr,
                                              SIFD_DFT_REG_RAM_LOW_POWER_CFG_BIT_COPI_RAM_LOWPWR_CFG_MSK,
                                              SIFD_DFT_REG_RAM_LOW_POWER_CFG_BIT_COPI_RAM_LOWPWR_CFG_OFF,
                                              value);
}

static INLINE UINT32 sifd_dft_field_COPI_RAM_LOWPWR_CFG_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_COPI_RAM_LOWPWR_CFG_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 1:0) bits 0:1 use field COPI_RAM_LOWPWR_CFG of register PMC_SIFD_DFT_REG_RAM_LOW_POWER_CFG */
    reg_value = sifd_dft_reg_RAM_LOW_POWER_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & SIFD_DFT_REG_RAM_LOW_POWER_CFG_BIT_COPI_RAM_LOWPWR_CFG_MSK) >> SIFD_DFT_REG_RAM_LOW_POWER_CFG_BIT_COPI_RAM_LOWPWR_CFG_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_COPI_RAM_LOWPWR_CFG_get", value );

    return value;
}
static INLINE void sifd_dft_field_range_COPI_RAM_LOWPWR_CFG_set( sifd_dft_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_range_COPI_RAM_LOWPWR_CFG_set( sifd_dft_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_COPI_RAM_LOWPWR_CFG_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_COPI_RAM_LOWPWR_CFG_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_COPI_RAM_LOWPWR_CFG_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000000c bits 1:0) bits 0:1 use field COPI_RAM_LOWPWR_CFG of register PMC_SIFD_DFT_REG_RAM_LOW_POWER_CFG */
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
        /* (0x0000000c bits 1:0) bits 0:1 use field COPI_RAM_LOWPWR_CFG of register PMC_SIFD_DFT_REG_RAM_LOW_POWER_CFG */
        sifd_dft_reg_RAM_LOW_POWER_CFG_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (SIFD_DFT_REG_RAM_LOW_POWER_CFG_BIT_COPI_RAM_LOWPWR_CFG_OFF + subfield_offset),
                                                  SIFD_DFT_REG_RAM_LOW_POWER_CFG_BIT_COPI_RAM_LOWPWR_CFG_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 sifd_dft_field_range_COPI_RAM_LOWPWR_CFG_get( sifd_dft_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_range_COPI_RAM_LOWPWR_CFG_get( sifd_dft_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_COPI_RAM_LOWPWR_CFG_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_COPI_RAM_LOWPWR_CFG_get", stop_bit, 1 );
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
    /* (0x0000000c bits 1:0) bits 0:1 use field COPI_RAM_LOWPWR_CFG of register PMC_SIFD_DFT_REG_RAM_LOW_POWER_CFG */
    reg_value = sifd_dft_reg_RAM_LOW_POWER_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & SIFD_DFT_REG_RAM_LOW_POWER_CFG_BIT_COPI_RAM_LOWPWR_CFG_MSK)
                  >> SIFD_DFT_REG_RAM_LOW_POWER_CFG_BIT_COPI_RAM_LOWPWR_CFG_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SIFD_DFT_REG_RAM_LOW_POWER_CFG_BIT_COPI_RAM_LOWPWR_CFG_MSK, SIFD_DFT_REG_RAM_LOW_POWER_CFG_BIT_COPI_RAM_LOWPWR_CFG_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_COPI_RAM_LOWPWR_CFG_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sifd_dft_field_FIC_P_TX_SERDES_SEL_19_set( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_P_TX_SERDES_SEL_19_set( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_P_TX_SERDES_SEL_19_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_FIC_P_TX_SERDES_SEL_19_set", value );

    /* (0x0000002c bits 7) field FIC_P_TX_SERDES_SEL_19 of register PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT */
    sifd_dft_reg_FIC_SERDES_LANE_SELECT_field_set( b_ptr,
                                                   h_ptr,
                                                   SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_TX_SERDES_SEL_19_MSK,
                                                   SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_TX_SERDES_SEL_19_OFF,
                                                   value);
}

static INLINE UINT32 sifd_dft_field_FIC_P_TX_SERDES_SEL_19_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_P_TX_SERDES_SEL_19_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 7) field FIC_P_TX_SERDES_SEL_19 of register PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT */
    reg_value = sifd_dft_reg_FIC_SERDES_LANE_SELECT_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_TX_SERDES_SEL_19_MSK) >> SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_TX_SERDES_SEL_19_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_FIC_P_TX_SERDES_SEL_19_get", value );

    return value;
}
static INLINE void sifd_dft_field_FIC_P_RX_SERDES_SEL_19_set( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_P_RX_SERDES_SEL_19_set( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_P_RX_SERDES_SEL_19_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_FIC_P_RX_SERDES_SEL_19_set", value );

    /* (0x0000002c bits 6) field FIC_P_RX_SERDES_SEL_19 of register PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT */
    sifd_dft_reg_FIC_SERDES_LANE_SELECT_field_set( b_ptr,
                                                   h_ptr,
                                                   SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_RX_SERDES_SEL_19_MSK,
                                                   SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_RX_SERDES_SEL_19_OFF,
                                                   value);
}

static INLINE UINT32 sifd_dft_field_FIC_P_RX_SERDES_SEL_19_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_P_RX_SERDES_SEL_19_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 6) field FIC_P_RX_SERDES_SEL_19 of register PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT */
    reg_value = sifd_dft_reg_FIC_SERDES_LANE_SELECT_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_RX_SERDES_SEL_19_MSK) >> SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_RX_SERDES_SEL_19_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_FIC_P_RX_SERDES_SEL_19_get", value );

    return value;
}
static INLINE void sifd_dft_field_FIC_P_TX_SERDES_SEL_18_set( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_P_TX_SERDES_SEL_18_set( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_P_TX_SERDES_SEL_18_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_FIC_P_TX_SERDES_SEL_18_set", value );

    /* (0x0000002c bits 5) field FIC_P_TX_SERDES_SEL_18 of register PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT */
    sifd_dft_reg_FIC_SERDES_LANE_SELECT_field_set( b_ptr,
                                                   h_ptr,
                                                   SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_TX_SERDES_SEL_18_MSK,
                                                   SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_TX_SERDES_SEL_18_OFF,
                                                   value);
}

static INLINE UINT32 sifd_dft_field_FIC_P_TX_SERDES_SEL_18_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_P_TX_SERDES_SEL_18_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 5) field FIC_P_TX_SERDES_SEL_18 of register PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT */
    reg_value = sifd_dft_reg_FIC_SERDES_LANE_SELECT_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_TX_SERDES_SEL_18_MSK) >> SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_TX_SERDES_SEL_18_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_FIC_P_TX_SERDES_SEL_18_get", value );

    return value;
}
static INLINE void sifd_dft_field_FIC_P_RX_SERDES_SEL_18_set( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_P_RX_SERDES_SEL_18_set( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_P_RX_SERDES_SEL_18_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_FIC_P_RX_SERDES_SEL_18_set", value );

    /* (0x0000002c bits 4) field FIC_P_RX_SERDES_SEL_18 of register PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT */
    sifd_dft_reg_FIC_SERDES_LANE_SELECT_field_set( b_ptr,
                                                   h_ptr,
                                                   SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_RX_SERDES_SEL_18_MSK,
                                                   SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_RX_SERDES_SEL_18_OFF,
                                                   value);
}

static INLINE UINT32 sifd_dft_field_FIC_P_RX_SERDES_SEL_18_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_P_RX_SERDES_SEL_18_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 4) field FIC_P_RX_SERDES_SEL_18 of register PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT */
    reg_value = sifd_dft_reg_FIC_SERDES_LANE_SELECT_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_RX_SERDES_SEL_18_MSK) >> SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_RX_SERDES_SEL_18_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_FIC_P_RX_SERDES_SEL_18_get", value );

    return value;
}
static INLINE void sifd_dft_field_FIC_W_TX_SERDES_SEL_19_set( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_W_TX_SERDES_SEL_19_set( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_W_TX_SERDES_SEL_19_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_FIC_W_TX_SERDES_SEL_19_set", value );

    /* (0x0000002c bits 3) field FIC_W_TX_SERDES_SEL_19 of register PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT */
    sifd_dft_reg_FIC_SERDES_LANE_SELECT_field_set( b_ptr,
                                                   h_ptr,
                                                   SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_TX_SERDES_SEL_19_MSK,
                                                   SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_TX_SERDES_SEL_19_OFF,
                                                   value);
}

static INLINE UINT32 sifd_dft_field_FIC_W_TX_SERDES_SEL_19_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_W_TX_SERDES_SEL_19_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 3) field FIC_W_TX_SERDES_SEL_19 of register PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT */
    reg_value = sifd_dft_reg_FIC_SERDES_LANE_SELECT_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_TX_SERDES_SEL_19_MSK) >> SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_TX_SERDES_SEL_19_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_FIC_W_TX_SERDES_SEL_19_get", value );

    return value;
}
static INLINE void sifd_dft_field_FIC_W_RX_SERDES_SEL_19_set( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_W_RX_SERDES_SEL_19_set( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_W_RX_SERDES_SEL_19_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_FIC_W_RX_SERDES_SEL_19_set", value );

    /* (0x0000002c bits 2) field FIC_W_RX_SERDES_SEL_19 of register PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT */
    sifd_dft_reg_FIC_SERDES_LANE_SELECT_field_set( b_ptr,
                                                   h_ptr,
                                                   SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_RX_SERDES_SEL_19_MSK,
                                                   SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_RX_SERDES_SEL_19_OFF,
                                                   value);
}

static INLINE UINT32 sifd_dft_field_FIC_W_RX_SERDES_SEL_19_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_W_RX_SERDES_SEL_19_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 2) field FIC_W_RX_SERDES_SEL_19 of register PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT */
    reg_value = sifd_dft_reg_FIC_SERDES_LANE_SELECT_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_RX_SERDES_SEL_19_MSK) >> SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_RX_SERDES_SEL_19_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_FIC_W_RX_SERDES_SEL_19_get", value );

    return value;
}
static INLINE void sifd_dft_field_FIC_W_TX_SERDES_SEL_18_set( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_W_TX_SERDES_SEL_18_set( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_W_TX_SERDES_SEL_18_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_FIC_W_TX_SERDES_SEL_18_set", value );

    /* (0x0000002c bits 1) field FIC_W_TX_SERDES_SEL_18 of register PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT */
    sifd_dft_reg_FIC_SERDES_LANE_SELECT_field_set( b_ptr,
                                                   h_ptr,
                                                   SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_TX_SERDES_SEL_18_MSK,
                                                   SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_TX_SERDES_SEL_18_OFF,
                                                   value);
}

static INLINE UINT32 sifd_dft_field_FIC_W_TX_SERDES_SEL_18_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_W_TX_SERDES_SEL_18_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 1) field FIC_W_TX_SERDES_SEL_18 of register PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT */
    reg_value = sifd_dft_reg_FIC_SERDES_LANE_SELECT_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_TX_SERDES_SEL_18_MSK) >> SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_TX_SERDES_SEL_18_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_FIC_W_TX_SERDES_SEL_18_get", value );

    return value;
}
static INLINE void sifd_dft_field_FIC_W_RX_SERDES_SEL_18_set( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_W_RX_SERDES_SEL_18_set( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_W_RX_SERDES_SEL_18_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_FIC_W_RX_SERDES_SEL_18_set", value );

    /* (0x0000002c bits 0) field FIC_W_RX_SERDES_SEL_18 of register PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT */
    sifd_dft_reg_FIC_SERDES_LANE_SELECT_field_set( b_ptr,
                                                   h_ptr,
                                                   SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_RX_SERDES_SEL_18_MSK,
                                                   SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_RX_SERDES_SEL_18_OFF,
                                                   value);
}

static INLINE UINT32 sifd_dft_field_FIC_W_RX_SERDES_SEL_18_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_W_RX_SERDES_SEL_18_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 0) field FIC_W_RX_SERDES_SEL_18 of register PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT */
    reg_value = sifd_dft_reg_FIC_SERDES_LANE_SELECT_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_RX_SERDES_SEL_18_MSK) >> SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_RX_SERDES_SEL_18_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_FIC_W_RX_SERDES_SEL_18_get", value );

    return value;
}
static INLINE void sifd_dft_field_COPI_FIC_DPI_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_COPI_FIC_DPI_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_COPI_FIC_DPI_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_COPI_FIC_DPI_SEL_set", value );

    /* (0x00000030 bits 0) field COPI_FIC_DPI_SEL of register PMC_SIFD_DFT_REG_COPI_FIC_DPI_SEL */
    sifd_dft_reg_COPI_FIC_DPI_SEL_field_set( b_ptr,
                                             h_ptr,
                                             SIFD_DFT_REG_COPI_FIC_DPI_SEL_BIT_COPI_FIC_DPI_SEL_MSK,
                                             SIFD_DFT_REG_COPI_FIC_DPI_SEL_BIT_COPI_FIC_DPI_SEL_OFF,
                                             value);
}

static INLINE UINT32 sifd_dft_field_COPI_FIC_DPI_SEL_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_COPI_FIC_DPI_SEL_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 0) field COPI_FIC_DPI_SEL of register PMC_SIFD_DFT_REG_COPI_FIC_DPI_SEL */
    reg_value = sifd_dft_reg_COPI_FIC_DPI_SEL_read(  b_ptr, h_ptr);
    value = (reg_value & SIFD_DFT_REG_COPI_FIC_DPI_SEL_BIT_COPI_FIC_DPI_SEL_MSK) >> SIFD_DFT_REG_COPI_FIC_DPI_SEL_BIT_COPI_FIC_DPI_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_COPI_FIC_DPI_SEL_get", value );

    return value;
}
static INLINE void sifd_dft_field_N1_SERD_ACB_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_N1_SERD_ACB_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_N1_SERD_ACB_SEL_set", value, 255);
    IOLOG( "%s <= 0x%08x", "sifd_dft_field_N1_SERD_ACB_SEL_set", value );

    /* (0x00000034 bits 31:24) field N1_SERD_ACB_SEL of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL */
    sifd_dft_reg_SYSTEM_N1_SERDES_ACB_MUX_SEL_field_set( b_ptr,
                                                         h_ptr,
                                                         SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL_BIT_N1_SERD_ACB_SEL_MSK,
                                                         SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL_BIT_N1_SERD_ACB_SEL_OFF,
                                                         value);
}

static INLINE UINT32 sifd_dft_field_N1_SERD_ACB_SEL_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_N1_SERD_ACB_SEL_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 31:24) field N1_SERD_ACB_SEL of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL */
    reg_value = sifd_dft_reg_SYSTEM_N1_SERDES_ACB_MUX_SEL_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL_BIT_N1_SERD_ACB_SEL_MSK) >> SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL_BIT_N1_SERD_ACB_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_N1_SERD_ACB_SEL_get", value );

    return value;
}
static INLINE void sifd_dft_field_range_N1_SERD_ACB_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_range_N1_SERD_ACB_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                             sifd_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_N1_SERD_ACB_SEL_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_N1_SERD_ACB_SEL_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_N1_SERD_ACB_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000034 bits 31:24) field N1_SERD_ACB_SEL of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL */
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
        /* (0x00000034 bits 31:24) field N1_SERD_ACB_SEL of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL */
        sifd_dft_reg_SYSTEM_N1_SERDES_ACB_MUX_SEL_field_set( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL_BIT_N1_SERD_ACB_SEL_OFF + subfield_offset),
                                                             SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL_BIT_N1_SERD_ACB_SEL_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 sifd_dft_field_range_N1_SERD_ACB_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_range_N1_SERD_ACB_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_N1_SERD_ACB_SEL_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_N1_SERD_ACB_SEL_get", stop_bit, 7 );
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
    /* (0x00000034 bits 31:24) field N1_SERD_ACB_SEL of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL */
    reg_value = sifd_dft_reg_SYSTEM_N1_SERDES_ACB_MUX_SEL_read( b_ptr,
                                                                h_ptr);
    field_value = (reg_value & SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL_BIT_N1_SERD_ACB_SEL_MSK)
                  >> SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL_BIT_N1_SERD_ACB_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL_BIT_N1_SERD_ACB_SEL_MSK, SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL_BIT_N1_SERD_ACB_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_N1_SERD_ACB_SEL_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size12
 * ==================================================================================
 */
static INLINE void sifd_dft_field_DSIS_LOS_POL_INV_set( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_DSIS_LOS_POL_INV_set( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_DSIS_LOS_POL_INV_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_DSIS_LOS_POL_INV_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_DSIS_LOS_POL_INV_set", N, value );

    /* ((0x00000100 + (N) * 0x4) bits 6) field DSIS_LOS_POL_INV of register PMC_SIFD_DFT_REG_DSIS_CFGS index N=0..11 */
    sifd_dft_reg_DSIS_CFGS_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_LOS_POL_INV_MSK,
                                            SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_LOS_POL_INV_OFF,
                                            value);
}

static INLINE UINT32 sifd_dft_field_DSIS_LOS_POL_INV_get( sifd_dft_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_DSIS_LOS_POL_INV_get( sifd_dft_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_DSIS_LOS_POL_INV_get", N, 11);
    /* ((0x00000100 + (N) * 0x4) bits 6) field DSIS_LOS_POL_INV of register PMC_SIFD_DFT_REG_DSIS_CFGS index N=0..11 */
    reg_value = sifd_dft_reg_DSIS_CFGS_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_LOS_POL_INV_MSK) >> SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_LOS_POL_INV_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_DSIS_LOS_POL_INV_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_DSIS_DLOS_INV_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_DSIS_DLOS_INV_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_DSIS_DLOS_INV_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_DSIS_DLOS_INV_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_DSIS_DLOS_INV_set", N, value );

    /* ((0x00000100 + (N) * 0x4) bits 5) field DSIS_DLOS_INV of register PMC_SIFD_DFT_REG_DSIS_CFGS index N=0..11 */
    sifd_dft_reg_DSIS_CFGS_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_DLOS_INV_MSK,
                                            SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_DLOS_INV_OFF,
                                            value);
}

static INLINE UINT32 sifd_dft_field_DSIS_DLOS_INV_get( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_DSIS_DLOS_INV_get( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_DSIS_DLOS_INV_get", N, 11);
    /* ((0x00000100 + (N) * 0x4) bits 5) field DSIS_DLOS_INV of register PMC_SIFD_DFT_REG_DSIS_CFGS index N=0..11 */
    reg_value = sifd_dft_reg_DSIS_CFGS_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_DLOS_INV_MSK) >> SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_DLOS_INV_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_DSIS_DLOS_INV_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_DSIS_DLOS_EN_set( sifd_dft_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_DSIS_DLOS_EN_set( sifd_dft_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_DSIS_DLOS_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_DSIS_DLOS_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_DSIS_DLOS_EN_set", N, value );

    /* ((0x00000100 + (N) * 0x4) bits 4) field DSIS_DLOS_EN of register PMC_SIFD_DFT_REG_DSIS_CFGS index N=0..11 */
    sifd_dft_reg_DSIS_CFGS_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_DLOS_EN_MSK,
                                            SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_DLOS_EN_OFF,
                                            value);
}

static INLINE UINT32 sifd_dft_field_DSIS_DLOS_EN_get( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_DSIS_DLOS_EN_get( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_DSIS_DLOS_EN_get", N, 11);
    /* ((0x00000100 + (N) * 0x4) bits 4) field DSIS_DLOS_EN of register PMC_SIFD_DFT_REG_DSIS_CFGS index N=0..11 */
    reg_value = sifd_dft_reg_DSIS_CFGS_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_DLOS_EN_MSK) >> SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_DLOS_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_DSIS_DLOS_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_DSIS_PIN_LOS_INV_set( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_DSIS_PIN_LOS_INV_set( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_DSIS_PIN_LOS_INV_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_DSIS_PIN_LOS_INV_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_DSIS_PIN_LOS_INV_set", N, value );

    /* ((0x00000100 + (N) * 0x4) bits 3) field DSIS_PIN_LOS_INV of register PMC_SIFD_DFT_REG_DSIS_CFGS index N=0..11 */
    sifd_dft_reg_DSIS_CFGS_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_PIN_LOS_INV_MSK,
                                            SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_PIN_LOS_INV_OFF,
                                            value);
}

static INLINE UINT32 sifd_dft_field_DSIS_PIN_LOS_INV_get( sifd_dft_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_DSIS_PIN_LOS_INV_get( sifd_dft_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_DSIS_PIN_LOS_INV_get", N, 11);
    /* ((0x00000100 + (N) * 0x4) bits 3) field DSIS_PIN_LOS_INV of register PMC_SIFD_DFT_REG_DSIS_CFGS index N=0..11 */
    reg_value = sifd_dft_reg_DSIS_CFGS_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_PIN_LOS_INV_MSK) >> SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_PIN_LOS_INV_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_DSIS_PIN_LOS_INV_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_DSIS_PIN_LOS_EN_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_DSIS_PIN_LOS_EN_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_DSIS_PIN_LOS_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_DSIS_PIN_LOS_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_DSIS_PIN_LOS_EN_set", N, value );

    /* ((0x00000100 + (N) * 0x4) bits 2) field DSIS_PIN_LOS_EN of register PMC_SIFD_DFT_REG_DSIS_CFGS index N=0..11 */
    sifd_dft_reg_DSIS_CFGS_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_PIN_LOS_EN_MSK,
                                            SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_PIN_LOS_EN_OFF,
                                            value);
}

static INLINE UINT32 sifd_dft_field_DSIS_PIN_LOS_EN_get( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_DSIS_PIN_LOS_EN_get( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_DSIS_PIN_LOS_EN_get", N, 11);
    /* ((0x00000100 + (N) * 0x4) bits 2) field DSIS_PIN_LOS_EN of register PMC_SIFD_DFT_REG_DSIS_CFGS index N=0..11 */
    reg_value = sifd_dft_reg_DSIS_CFGS_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_PIN_LOS_EN_MSK) >> SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_PIN_LOS_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_DSIS_PIN_LOS_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_DSIS_TX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_DSIS_TX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_DSIS_TX_LANE_DIS_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_DSIS_TX_LANE_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_DSIS_TX_LANE_DIS_set", N, value );

    /* ((0x00000100 + (N) * 0x4) bits 1) field DSIS_TX_LANE_DIS of register PMC_SIFD_DFT_REG_DSIS_CFGS index N=0..11 */
    sifd_dft_reg_DSIS_CFGS_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_TX_LANE_DIS_MSK,
                                            SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_TX_LANE_DIS_OFF,
                                            value);
}

static INLINE UINT32 sifd_dft_field_DSIS_TX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_DSIS_TX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_DSIS_TX_LANE_DIS_get", N, 11);
    /* ((0x00000100 + (N) * 0x4) bits 1) field DSIS_TX_LANE_DIS of register PMC_SIFD_DFT_REG_DSIS_CFGS index N=0..11 */
    reg_value = sifd_dft_reg_DSIS_CFGS_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_TX_LANE_DIS_MSK) >> SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_TX_LANE_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_DSIS_TX_LANE_DIS_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_DSIS_RX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_DSIS_RX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_DSIS_RX_LANE_DIS_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_DSIS_RX_LANE_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_DSIS_RX_LANE_DIS_set", N, value );

    /* ((0x00000100 + (N) * 0x4) bits 0) field DSIS_RX_LANE_DIS of register PMC_SIFD_DFT_REG_DSIS_CFGS index N=0..11 */
    sifd_dft_reg_DSIS_CFGS_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_RX_LANE_DIS_MSK,
                                            SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_RX_LANE_DIS_OFF,
                                            value);
}

static INLINE UINT32 sifd_dft_field_DSIS_RX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_DSIS_RX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_DSIS_RX_LANE_DIS_get", N, 11);
    /* ((0x00000100 + (N) * 0x4) bits 0) field DSIS_RX_LANE_DIS of register PMC_SIFD_DFT_REG_DSIS_CFGS index N=0..11 */
    reg_value = sifd_dft_reg_DSIS_CFGS_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_RX_LANE_DIS_MSK) >> SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_RX_LANE_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_DSIS_RX_LANE_DIS_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size16
 * ==================================================================================
 */
static INLINE void sifd_dft_field_N1_TX_LANE_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_N1_TX_LANE_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 15)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_N1_TX_LANE_SEL_set", N, 15);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_N1_TX_LANE_SEL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_N1_TX_LANE_SEL_set", N, value );

    /* ((0x000002c8 + (N) * 0x4) bits 25:24) field N1_TX_LANE_SEL of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS index N=0..15 */
    sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_LANE_SEL_MSK,
                                                        SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_LANE_SEL_OFF,
                                                        value);
}

static INLINE UINT32 sifd_dft_field_N1_TX_LANE_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_N1_TX_LANE_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 15)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_N1_TX_LANE_SEL_get", N, 15);
    /* ((0x000002c8 + (N) * 0x4) bits 25:24) field N1_TX_LANE_SEL of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS index N=0..15 */
    reg_value = sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_LANE_SEL_MSK) >> SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_LANE_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_N1_TX_LANE_SEL_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_range_N1_TX_LANE_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_range_N1_TX_LANE_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 15)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_range_N1_TX_LANE_SEL_set", N, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_N1_TX_LANE_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_N1_TX_LANE_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_N1_TX_LANE_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000002c8 + (N) * 0x4) bits 25:24) field N1_TX_LANE_SEL of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS index N=0..15 */
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
        /* ((0x000002c8 + (N) * 0x4) bits 25:24) field N1_TX_LANE_SEL of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS index N=0..15 */
        sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            subfield_mask << (SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_LANE_SEL_OFF + subfield_offset),
                                                            SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_LANE_SEL_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 sifd_dft_field_range_N1_TX_LANE_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_range_N1_TX_LANE_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
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

    if (N > 15)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_range_N1_TX_LANE_SEL_get", N, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_N1_TX_LANE_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_N1_TX_LANE_SEL_get", stop_bit, 1 );
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
    /* ((0x000002c8 + (N) * 0x4) bits 25:24) field N1_TX_LANE_SEL of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS index N=0..15 */
    reg_value = sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    field_value = (reg_value & SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_LANE_SEL_MSK)
                  >> SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_LANE_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_LANE_SEL_MSK, SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_LANE_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_N1_TX_LANE_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sifd_dft_field_N1_TX_DESKEW_CONST_DELAY_EN_set( sifd_dft_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_N1_TX_DESKEW_CONST_DELAY_EN_set( sifd_dft_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value )
{
    if (N > 15)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_N1_TX_DESKEW_CONST_DELAY_EN_set", N, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_N1_TX_DESKEW_CONST_DELAY_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_N1_TX_DESKEW_CONST_DELAY_EN_set", N, value );

    /* ((0x000002c8 + (N) * 0x4) bits 18) field N1_TX_DESKEW_CONST_DELAY_EN of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS index N=0..15 */
    sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_DESKEW_CONST_DELAY_EN_MSK,
                                                        SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_DESKEW_CONST_DELAY_EN_OFF,
                                                        value);
}

static INLINE UINT32 sifd_dft_field_N1_TX_DESKEW_CONST_DELAY_EN_get( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_N1_TX_DESKEW_CONST_DELAY_EN_get( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 15)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_N1_TX_DESKEW_CONST_DELAY_EN_get", N, 15);
    /* ((0x000002c8 + (N) * 0x4) bits 18) field N1_TX_DESKEW_CONST_DELAY_EN of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS index N=0..15 */
    reg_value = sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_DESKEW_CONST_DELAY_EN_MSK) >> SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_DESKEW_CONST_DELAY_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_N1_TX_DESKEW_CONST_DELAY_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_N1_TX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_N1_TX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 15)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_N1_TX_NUM_DESKEW_STAGES_set", N, 15);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_N1_TX_NUM_DESKEW_STAGES_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_N1_TX_NUM_DESKEW_STAGES_set", N, value );

    /* ((0x000002c8 + (N) * 0x4) bits 17:16) field N1_TX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS index N=0..15 */
    sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_NUM_DESKEW_STAGES_MSK,
                                                        SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_NUM_DESKEW_STAGES_OFF,
                                                        value);
}

static INLINE UINT32 sifd_dft_field_N1_TX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_N1_TX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 15)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_N1_TX_NUM_DESKEW_STAGES_get", N, 15);
    /* ((0x000002c8 + (N) * 0x4) bits 17:16) field N1_TX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS index N=0..15 */
    reg_value = sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_NUM_DESKEW_STAGES_MSK) >> SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_NUM_DESKEW_STAGES_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_N1_TX_NUM_DESKEW_STAGES_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_range_N1_TX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_range_N1_TX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (N > 15)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_range_N1_TX_NUM_DESKEW_STAGES_set", N, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_N1_TX_NUM_DESKEW_STAGES_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_N1_TX_NUM_DESKEW_STAGES_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_N1_TX_NUM_DESKEW_STAGES_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000002c8 + (N) * 0x4) bits 17:16) field N1_TX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS index N=0..15 */
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
        /* ((0x000002c8 + (N) * 0x4) bits 17:16) field N1_TX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS index N=0..15 */
        sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            subfield_mask << (SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_NUM_DESKEW_STAGES_OFF + subfield_offset),
                                                            SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_NUM_DESKEW_STAGES_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 sifd_dft_field_range_N1_TX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_range_N1_TX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
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

    if (N > 15)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_range_N1_TX_NUM_DESKEW_STAGES_get", N, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_N1_TX_NUM_DESKEW_STAGES_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_N1_TX_NUM_DESKEW_STAGES_get", stop_bit, 1 );
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
    /* ((0x000002c8 + (N) * 0x4) bits 17:16) field N1_TX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS index N=0..15 */
    reg_value = sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    field_value = (reg_value & SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_NUM_DESKEW_STAGES_MSK)
                  >> SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_NUM_DESKEW_STAGES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_NUM_DESKEW_STAGES_MSK, SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_NUM_DESKEW_STAGES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_N1_TX_NUM_DESKEW_STAGES_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sifd_dft_field_N1_RX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_N1_RX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 15)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_N1_RX_NUM_DESKEW_STAGES_set", N, 15);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_N1_RX_NUM_DESKEW_STAGES_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_N1_RX_NUM_DESKEW_STAGES_set", N, value );

    /* ((0x000002c8 + (N) * 0x4) bits 9:8) field N1_RX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS index N=0..15 */
    sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_RX_NUM_DESKEW_STAGES_MSK,
                                                        SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_RX_NUM_DESKEW_STAGES_OFF,
                                                        value);
}

static INLINE UINT32 sifd_dft_field_N1_RX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_N1_RX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 15)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_N1_RX_NUM_DESKEW_STAGES_get", N, 15);
    /* ((0x000002c8 + (N) * 0x4) bits 9:8) field N1_RX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS index N=0..15 */
    reg_value = sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_RX_NUM_DESKEW_STAGES_MSK) >> SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_RX_NUM_DESKEW_STAGES_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_N1_RX_NUM_DESKEW_STAGES_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_range_N1_RX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_range_N1_RX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (N > 15)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_range_N1_RX_NUM_DESKEW_STAGES_set", N, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_N1_RX_NUM_DESKEW_STAGES_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_N1_RX_NUM_DESKEW_STAGES_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_N1_RX_NUM_DESKEW_STAGES_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x000002c8 + (N) * 0x4) bits 9:8) field N1_RX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS index N=0..15 */
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
        /* ((0x000002c8 + (N) * 0x4) bits 9:8) field N1_RX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS index N=0..15 */
        sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            subfield_mask << (SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_RX_NUM_DESKEW_STAGES_OFF + subfield_offset),
                                                            SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_RX_NUM_DESKEW_STAGES_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 sifd_dft_field_range_N1_RX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_range_N1_RX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
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

    if (N > 15)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_range_N1_RX_NUM_DESKEW_STAGES_get", N, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_N1_RX_NUM_DESKEW_STAGES_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_N1_RX_NUM_DESKEW_STAGES_get", stop_bit, 1 );
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
    /* ((0x000002c8 + (N) * 0x4) bits 9:8) field N1_RX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS index N=0..15 */
    reg_value = sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    field_value = (reg_value & SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_RX_NUM_DESKEW_STAGES_MSK)
                  >> SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_RX_NUM_DESKEW_STAGES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_RX_NUM_DESKEW_STAGES_MSK, SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_RX_NUM_DESKEW_STAGES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_N1_RX_NUM_DESKEW_STAGES_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size24
 * ==================================================================================
 */
static INLINE void sifd_dft_field_IL1_TX_SERDES_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL1_TX_SERDES_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_TX_SERDES_SEL_set", N, 23);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL1_TX_SERDES_SEL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL1_TX_SERDES_SEL_set", N, value );

    /* ((0x00000040 + (N) * 0x4) bits 5) field IL1_TX_SERDES_SEL of register PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS index N=0..23 */
    sifd_dft_reg_INTERLAKEN_1_CFGS_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_TX_SERDES_SEL_MSK,
                                                    SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_TX_SERDES_SEL_OFF,
                                                    value);
}

static INLINE UINT32 sifd_dft_field_IL1_TX_SERDES_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL1_TX_SERDES_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_TX_SERDES_SEL_get", N, 23);
    /* ((0x00000040 + (N) * 0x4) bits 5) field IL1_TX_SERDES_SEL of register PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS index N=0..23 */
    reg_value = sifd_dft_reg_INTERLAKEN_1_CFGS_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_TX_SERDES_SEL_MSK) >> SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_TX_SERDES_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL1_TX_SERDES_SEL_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_IL1_RX_SERDES_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL1_RX_SERDES_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_RX_SERDES_SEL_set", N, 23);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL1_RX_SERDES_SEL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL1_RX_SERDES_SEL_set", N, value );

    /* ((0x00000040 + (N) * 0x4) bits 4) field IL1_RX_SERDES_SEL of register PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS index N=0..23 */
    sifd_dft_reg_INTERLAKEN_1_CFGS_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_RX_SERDES_SEL_MSK,
                                                    SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_RX_SERDES_SEL_OFF,
                                                    value);
}

static INLINE UINT32 sifd_dft_field_IL1_RX_SERDES_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL1_RX_SERDES_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_RX_SERDES_SEL_get", N, 23);
    /* ((0x00000040 + (N) * 0x4) bits 4) field IL1_RX_SERDES_SEL of register PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS index N=0..23 */
    reg_value = sifd_dft_reg_INTERLAKEN_1_CFGS_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_RX_SERDES_SEL_MSK) >> SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_RX_SERDES_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL1_RX_SERDES_SEL_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_IL1_DLOS_INV_set( sifd_dft_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL1_DLOS_INV_set( sifd_dft_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_DLOS_INV_set", N, 23);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL1_DLOS_INV_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL1_DLOS_INV_set", N, value );

    /* ((0x00000040 + (N) * 0x4) bits 3) field IL1_DLOS_INV of register PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS index N=0..23 */
    sifd_dft_reg_INTERLAKEN_1_CFGS_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_DLOS_INV_MSK,
                                                    SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_DLOS_INV_OFF,
                                                    value);
}

static INLINE UINT32 sifd_dft_field_IL1_DLOS_INV_get( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL1_DLOS_INV_get( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_DLOS_INV_get", N, 23);
    /* ((0x00000040 + (N) * 0x4) bits 3) field IL1_DLOS_INV of register PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS index N=0..23 */
    reg_value = sifd_dft_reg_INTERLAKEN_1_CFGS_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_DLOS_INV_MSK) >> SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_DLOS_INV_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL1_DLOS_INV_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_IL1_DLOS_EN_set( sifd_dft_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL1_DLOS_EN_set( sifd_dft_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_DLOS_EN_set", N, 23);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL1_DLOS_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL1_DLOS_EN_set", N, value );

    /* ((0x00000040 + (N) * 0x4) bits 2) field IL1_DLOS_EN of register PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS index N=0..23 */
    sifd_dft_reg_INTERLAKEN_1_CFGS_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_DLOS_EN_MSK,
                                                    SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_DLOS_EN_OFF,
                                                    value);
}

static INLINE UINT32 sifd_dft_field_IL1_DLOS_EN_get( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL1_DLOS_EN_get( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_DLOS_EN_get", N, 23);
    /* ((0x00000040 + (N) * 0x4) bits 2) field IL1_DLOS_EN of register PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS index N=0..23 */
    reg_value = sifd_dft_reg_INTERLAKEN_1_CFGS_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_DLOS_EN_MSK) >> SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_DLOS_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL1_DLOS_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_IL1_TX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL1_TX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_TX_LANE_DIS_set", N, 23);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL1_TX_LANE_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL1_TX_LANE_DIS_set", N, value );

    /* ((0x00000040 + (N) * 0x4) bits 1) field IL1_TX_LANE_DIS of register PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS index N=0..23 */
    sifd_dft_reg_INTERLAKEN_1_CFGS_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_TX_LANE_DIS_MSK,
                                                    SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_TX_LANE_DIS_OFF,
                                                    value);
}

static INLINE UINT32 sifd_dft_field_IL1_TX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL1_TX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_TX_LANE_DIS_get", N, 23);
    /* ((0x00000040 + (N) * 0x4) bits 1) field IL1_TX_LANE_DIS of register PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS index N=0..23 */
    reg_value = sifd_dft_reg_INTERLAKEN_1_CFGS_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_TX_LANE_DIS_MSK) >> SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_TX_LANE_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL1_TX_LANE_DIS_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_IL1_RX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL1_RX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_RX_LANE_DIS_set", N, 23);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL1_RX_LANE_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL1_RX_LANE_DIS_set", N, value );

    /* ((0x00000040 + (N) * 0x4) bits 0) field IL1_RX_LANE_DIS of register PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS index N=0..23 */
    sifd_dft_reg_INTERLAKEN_1_CFGS_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_RX_LANE_DIS_MSK,
                                                    SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_RX_LANE_DIS_OFF,
                                                    value);
}

static INLINE UINT32 sifd_dft_field_IL1_RX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL1_RX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_RX_LANE_DIS_get", N, 23);
    /* ((0x00000040 + (N) * 0x4) bits 0) field IL1_RX_LANE_DIS of register PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS index N=0..23 */
    reg_value = sifd_dft_reg_INTERLAKEN_1_CFGS_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_RX_LANE_DIS_MSK) >> SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_RX_LANE_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL1_RX_LANE_DIS_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_IL2_TX_SERDES_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL2_TX_SERDES_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_TX_SERDES_SEL_set", N, 23);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL2_TX_SERDES_SEL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL2_TX_SERDES_SEL_set", N, value );

    /* ((0x000000a0 + (N) * 0x4) bits 5) field IL2_TX_SERDES_SEL of register PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS index N=0..23 */
    sifd_dft_reg_INTERLAKEN_2_CFGS_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_TX_SERDES_SEL_MSK,
                                                    SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_TX_SERDES_SEL_OFF,
                                                    value);
}

static INLINE UINT32 sifd_dft_field_IL2_TX_SERDES_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL2_TX_SERDES_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_TX_SERDES_SEL_get", N, 23);
    /* ((0x000000a0 + (N) * 0x4) bits 5) field IL2_TX_SERDES_SEL of register PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS index N=0..23 */
    reg_value = sifd_dft_reg_INTERLAKEN_2_CFGS_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_TX_SERDES_SEL_MSK) >> SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_TX_SERDES_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL2_TX_SERDES_SEL_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_IL2_RX_SERDES_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL2_RX_SERDES_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_RX_SERDES_SEL_set", N, 23);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL2_RX_SERDES_SEL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL2_RX_SERDES_SEL_set", N, value );

    /* ((0x000000a0 + (N) * 0x4) bits 4) field IL2_RX_SERDES_SEL of register PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS index N=0..23 */
    sifd_dft_reg_INTERLAKEN_2_CFGS_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_RX_SERDES_SEL_MSK,
                                                    SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_RX_SERDES_SEL_OFF,
                                                    value);
}

static INLINE UINT32 sifd_dft_field_IL2_RX_SERDES_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL2_RX_SERDES_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_RX_SERDES_SEL_get", N, 23);
    /* ((0x000000a0 + (N) * 0x4) bits 4) field IL2_RX_SERDES_SEL of register PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS index N=0..23 */
    reg_value = sifd_dft_reg_INTERLAKEN_2_CFGS_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_RX_SERDES_SEL_MSK) >> SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_RX_SERDES_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL2_RX_SERDES_SEL_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_IL2_DLOS_INV_set( sifd_dft_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL2_DLOS_INV_set( sifd_dft_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_DLOS_INV_set", N, 23);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL2_DLOS_INV_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL2_DLOS_INV_set", N, value );

    /* ((0x000000a0 + (N) * 0x4) bits 3) field IL2_DLOS_INV of register PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS index N=0..23 */
    sifd_dft_reg_INTERLAKEN_2_CFGS_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_DLOS_INV_MSK,
                                                    SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_DLOS_INV_OFF,
                                                    value);
}

static INLINE UINT32 sifd_dft_field_IL2_DLOS_INV_get( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL2_DLOS_INV_get( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_DLOS_INV_get", N, 23);
    /* ((0x000000a0 + (N) * 0x4) bits 3) field IL2_DLOS_INV of register PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS index N=0..23 */
    reg_value = sifd_dft_reg_INTERLAKEN_2_CFGS_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_DLOS_INV_MSK) >> SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_DLOS_INV_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL2_DLOS_INV_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_IL2_DLOS_EN_set( sifd_dft_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL2_DLOS_EN_set( sifd_dft_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_DLOS_EN_set", N, 23);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL2_DLOS_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL2_DLOS_EN_set", N, value );

    /* ((0x000000a0 + (N) * 0x4) bits 2) field IL2_DLOS_EN of register PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS index N=0..23 */
    sifd_dft_reg_INTERLAKEN_2_CFGS_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_DLOS_EN_MSK,
                                                    SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_DLOS_EN_OFF,
                                                    value);
}

static INLINE UINT32 sifd_dft_field_IL2_DLOS_EN_get( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL2_DLOS_EN_get( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_DLOS_EN_get", N, 23);
    /* ((0x000000a0 + (N) * 0x4) bits 2) field IL2_DLOS_EN of register PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS index N=0..23 */
    reg_value = sifd_dft_reg_INTERLAKEN_2_CFGS_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_DLOS_EN_MSK) >> SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_DLOS_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL2_DLOS_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_IL2_TX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL2_TX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_TX_LANE_DIS_set", N, 23);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL2_TX_LANE_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL2_TX_LANE_DIS_set", N, value );

    /* ((0x000000a0 + (N) * 0x4) bits 1) field IL2_TX_LANE_DIS of register PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS index N=0..23 */
    sifd_dft_reg_INTERLAKEN_2_CFGS_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_TX_LANE_DIS_MSK,
                                                    SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_TX_LANE_DIS_OFF,
                                                    value);
}

static INLINE UINT32 sifd_dft_field_IL2_TX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL2_TX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_TX_LANE_DIS_get", N, 23);
    /* ((0x000000a0 + (N) * 0x4) bits 1) field IL2_TX_LANE_DIS of register PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS index N=0..23 */
    reg_value = sifd_dft_reg_INTERLAKEN_2_CFGS_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_TX_LANE_DIS_MSK) >> SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_TX_LANE_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL2_TX_LANE_DIS_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_IL2_RX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL2_RX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_RX_LANE_DIS_set", N, 23);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL2_RX_LANE_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL2_RX_LANE_DIS_set", N, value );

    /* ((0x000000a0 + (N) * 0x4) bits 0) field IL2_RX_LANE_DIS of register PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS index N=0..23 */
    sifd_dft_reg_INTERLAKEN_2_CFGS_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_RX_LANE_DIS_MSK,
                                                    SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_RX_LANE_DIS_OFF,
                                                    value);
}

static INLINE UINT32 sifd_dft_field_IL2_RX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL2_RX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 23)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_RX_LANE_DIS_get", N, 23);
    /* ((0x000000a0 + (N) * 0x4) bits 0) field IL2_RX_LANE_DIS of register PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS index N=0..23 */
    reg_value = sifd_dft_reg_INTERLAKEN_2_CFGS_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_RX_LANE_DIS_MSK) >> SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_RX_LANE_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL2_RX_LANE_DIS_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size27
 * ==================================================================================
 */
static INLINE void sifd_dft_field_FIC_W_DLOS_INV_set( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_W_DLOS_INV_set( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 26)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_W_DLOS_INV_set", N, 26);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_W_DLOS_INV_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_FIC_W_DLOS_INV_set", N, value );

    /* ((0x00000150 + (N) * 0x4) bits 3) field FIC_W_DLOS_INV of register PMC_SIFD_DFT_REG_FIC_WORK_CFGS index N=0..26 */
    sifd_dft_reg_FIC_WORK_CFGS_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_DLOS_INV_MSK,
                                                SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_DLOS_INV_OFF,
                                                value);
}

static INLINE UINT32 sifd_dft_field_FIC_W_DLOS_INV_get( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_W_DLOS_INV_get( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 26)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_W_DLOS_INV_get", N, 26);
    /* ((0x00000150 + (N) * 0x4) bits 3) field FIC_W_DLOS_INV of register PMC_SIFD_DFT_REG_FIC_WORK_CFGS index N=0..26 */
    reg_value = sifd_dft_reg_FIC_WORK_CFGS_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_DLOS_INV_MSK) >> SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_DLOS_INV_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_FIC_W_DLOS_INV_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_FIC_W_DLOS_EN_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_W_DLOS_EN_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 26)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_W_DLOS_EN_set", N, 26);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_W_DLOS_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_FIC_W_DLOS_EN_set", N, value );

    /* ((0x00000150 + (N) * 0x4) bits 2) field FIC_W_DLOS_EN of register PMC_SIFD_DFT_REG_FIC_WORK_CFGS index N=0..26 */
    sifd_dft_reg_FIC_WORK_CFGS_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_DLOS_EN_MSK,
                                                SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_DLOS_EN_OFF,
                                                value);
}

static INLINE UINT32 sifd_dft_field_FIC_W_DLOS_EN_get( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_W_DLOS_EN_get( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 26)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_W_DLOS_EN_get", N, 26);
    /* ((0x00000150 + (N) * 0x4) bits 2) field FIC_W_DLOS_EN of register PMC_SIFD_DFT_REG_FIC_WORK_CFGS index N=0..26 */
    reg_value = sifd_dft_reg_FIC_WORK_CFGS_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_DLOS_EN_MSK) >> SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_DLOS_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_FIC_W_DLOS_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_FIC_W_TX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_W_TX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 26)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_W_TX_LANE_DIS_set", N, 26);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_W_TX_LANE_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_FIC_W_TX_LANE_DIS_set", N, value );

    /* ((0x00000150 + (N) * 0x4) bits 1) field FIC_W_TX_LANE_DIS of register PMC_SIFD_DFT_REG_FIC_WORK_CFGS index N=0..26 */
    sifd_dft_reg_FIC_WORK_CFGS_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_TX_LANE_DIS_MSK,
                                                SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_TX_LANE_DIS_OFF,
                                                value);
}

static INLINE UINT32 sifd_dft_field_FIC_W_TX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_W_TX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 26)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_W_TX_LANE_DIS_get", N, 26);
    /* ((0x00000150 + (N) * 0x4) bits 1) field FIC_W_TX_LANE_DIS of register PMC_SIFD_DFT_REG_FIC_WORK_CFGS index N=0..26 */
    reg_value = sifd_dft_reg_FIC_WORK_CFGS_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_TX_LANE_DIS_MSK) >> SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_TX_LANE_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_FIC_W_TX_LANE_DIS_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_FIC_W_RX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_W_RX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 26)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_W_RX_LANE_DIS_set", N, 26);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_W_RX_LANE_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_FIC_W_RX_LANE_DIS_set", N, value );

    /* ((0x00000150 + (N) * 0x4) bits 0) field FIC_W_RX_LANE_DIS of register PMC_SIFD_DFT_REG_FIC_WORK_CFGS index N=0..26 */
    sifd_dft_reg_FIC_WORK_CFGS_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_RX_LANE_DIS_MSK,
                                                SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_RX_LANE_DIS_OFF,
                                                value);
}

static INLINE UINT32 sifd_dft_field_FIC_W_RX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_W_RX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 26)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_W_RX_LANE_DIS_get", N, 26);
    /* ((0x00000150 + (N) * 0x4) bits 0) field FIC_W_RX_LANE_DIS of register PMC_SIFD_DFT_REG_FIC_WORK_CFGS index N=0..26 */
    reg_value = sifd_dft_reg_FIC_WORK_CFGS_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_RX_LANE_DIS_MSK) >> SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_RX_LANE_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_FIC_W_RX_LANE_DIS_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_FIC_P_DLOS_INV_set( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_P_DLOS_INV_set( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 26)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_P_DLOS_INV_set", N, 26);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_P_DLOS_INV_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_FIC_P_DLOS_INV_set", N, value );

    /* ((0x000001bc + (N) * 0x4) bits 3) field FIC_P_DLOS_INV of register PMC_SIFD_DFT_REG_FIC_PROTECT_CFGS index N=0..26 */
    sifd_dft_reg_FIC_PROTECT_CFGS_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_DLOS_INV_MSK,
                                                   SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_DLOS_INV_OFF,
                                                   value);
}

static INLINE UINT32 sifd_dft_field_FIC_P_DLOS_INV_get( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_P_DLOS_INV_get( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 26)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_P_DLOS_INV_get", N, 26);
    /* ((0x000001bc + (N) * 0x4) bits 3) field FIC_P_DLOS_INV of register PMC_SIFD_DFT_REG_FIC_PROTECT_CFGS index N=0..26 */
    reg_value = sifd_dft_reg_FIC_PROTECT_CFGS_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_DLOS_INV_MSK) >> SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_DLOS_INV_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_FIC_P_DLOS_INV_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_FIC_P_DLOS_EN_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_P_DLOS_EN_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 26)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_P_DLOS_EN_set", N, 26);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_P_DLOS_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_FIC_P_DLOS_EN_set", N, value );

    /* ((0x000001bc + (N) * 0x4) bits 2) field FIC_P_DLOS_EN of register PMC_SIFD_DFT_REG_FIC_PROTECT_CFGS index N=0..26 */
    sifd_dft_reg_FIC_PROTECT_CFGS_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_DLOS_EN_MSK,
                                                   SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_DLOS_EN_OFF,
                                                   value);
}

static INLINE UINT32 sifd_dft_field_FIC_P_DLOS_EN_get( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_P_DLOS_EN_get( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 26)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_P_DLOS_EN_get", N, 26);
    /* ((0x000001bc + (N) * 0x4) bits 2) field FIC_P_DLOS_EN of register PMC_SIFD_DFT_REG_FIC_PROTECT_CFGS index N=0..26 */
    reg_value = sifd_dft_reg_FIC_PROTECT_CFGS_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_DLOS_EN_MSK) >> SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_DLOS_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_FIC_P_DLOS_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_FIC_P_TX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_P_TX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 26)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_P_TX_LANE_DIS_set", N, 26);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_P_TX_LANE_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_FIC_P_TX_LANE_DIS_set", N, value );

    /* ((0x000001bc + (N) * 0x4) bits 1) field FIC_P_TX_LANE_DIS of register PMC_SIFD_DFT_REG_FIC_PROTECT_CFGS index N=0..26 */
    sifd_dft_reg_FIC_PROTECT_CFGS_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_TX_LANE_DIS_MSK,
                                                   SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_TX_LANE_DIS_OFF,
                                                   value);
}

static INLINE UINT32 sifd_dft_field_FIC_P_TX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_P_TX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 26)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_P_TX_LANE_DIS_get", N, 26);
    /* ((0x000001bc + (N) * 0x4) bits 1) field FIC_P_TX_LANE_DIS of register PMC_SIFD_DFT_REG_FIC_PROTECT_CFGS index N=0..26 */
    reg_value = sifd_dft_reg_FIC_PROTECT_CFGS_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_TX_LANE_DIS_MSK) >> SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_TX_LANE_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_FIC_P_TX_LANE_DIS_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_FIC_P_RX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_P_RX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 26)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_P_RX_LANE_DIS_set", N, 26);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_P_RX_LANE_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_FIC_P_RX_LANE_DIS_set", N, value );

    /* ((0x000001bc + (N) * 0x4) bits 0) field FIC_P_RX_LANE_DIS of register PMC_SIFD_DFT_REG_FIC_PROTECT_CFGS index N=0..26 */
    sifd_dft_reg_FIC_PROTECT_CFGS_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_RX_LANE_DIS_MSK,
                                                   SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_RX_LANE_DIS_OFF,
                                                   value);
}

static INLINE UINT32 sifd_dft_field_FIC_P_RX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_P_RX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 26)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_P_RX_LANE_DIS_get", N, 26);
    /* ((0x000001bc + (N) * 0x4) bits 0) field FIC_P_RX_LANE_DIS of register PMC_SIFD_DFT_REG_FIC_PROTECT_CFGS index N=0..26 */
    reg_value = sifd_dft_reg_FIC_PROTECT_CFGS_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_RX_LANE_DIS_MSK) >> SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_RX_LANE_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_FIC_P_RX_LANE_DIS_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size4
 * ==================================================================================
 */
static INLINE void sifd_dft_field_MPMA_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_MPMA_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_MPMA_IRQ_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_MPMA_IRQ_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_MPMA_IRQ_EN_set", N, value );

    /* ((0x0000001c + (N) * 0x4) bits 14) field MPMA_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    sifd_dft_reg_SIFD_INT_EN_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_MPMA_IRQ_EN_MSK,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_MPMA_IRQ_EN_OFF,
                                              value);
}

static INLINE UINT32 sifd_dft_field_MPMA_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_MPMA_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_MPMA_IRQ_EN_get", N, 3);
    /* ((0x0000001c + (N) * 0x4) bits 14) field MPMA_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    reg_value = sifd_dft_reg_SIFD_INT_EN_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & SIFD_DFT_REG_SIFD_INT_EN_BIT_MPMA_IRQ_EN_MSK) >> SIFD_DFT_REG_SIFD_INT_EN_BIT_MPMA_IRQ_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_MPMA_IRQ_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_MPMO_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_MPMO_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_MPMO_IRQ_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_MPMO_IRQ_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_MPMO_IRQ_EN_set", N, value );

    /* ((0x0000001c + (N) * 0x4) bits 13) field MPMO_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    sifd_dft_reg_SIFD_INT_EN_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_MPMO_IRQ_EN_MSK,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_MPMO_IRQ_EN_OFF,
                                              value);
}

static INLINE UINT32 sifd_dft_field_MPMO_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_MPMO_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_MPMO_IRQ_EN_get", N, 3);
    /* ((0x0000001c + (N) * 0x4) bits 13) field MPMO_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    reg_value = sifd_dft_reg_SIFD_INT_EN_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & SIFD_DFT_REG_SIFD_INT_EN_BIT_MPMO_IRQ_EN_MSK) >> SIFD_DFT_REG_SIFD_INT_EN_BIT_MPMO_IRQ_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_MPMO_IRQ_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_FIC_WRAP_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_WRAP_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_WRAP_IRQ_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_WRAP_IRQ_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_FIC_WRAP_IRQ_EN_set", N, value );

    /* ((0x0000001c + (N) * 0x4) bits 12) field FIC_WRAP_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    sifd_dft_reg_SIFD_INT_EN_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_FIC_WRAP_IRQ_EN_MSK,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_FIC_WRAP_IRQ_EN_OFF,
                                              value);
}

static INLINE UINT32 sifd_dft_field_FIC_WRAP_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_WRAP_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_WRAP_IRQ_EN_get", N, 3);
    /* ((0x0000001c + (N) * 0x4) bits 12) field FIC_WRAP_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    reg_value = sifd_dft_reg_SIFD_INT_EN_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & SIFD_DFT_REG_SIFD_INT_EN_BIT_FIC_WRAP_IRQ_EN_MSK) >> SIFD_DFT_REG_SIFD_INT_EN_BIT_FIC_WRAP_IRQ_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_FIC_WRAP_IRQ_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_FIC_IP_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_FIC_IP_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_IP_IRQ_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_FIC_IP_IRQ_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_FIC_IP_IRQ_EN_set", N, value );

    /* ((0x0000001c + (N) * 0x4) bits 11) field FIC_IP_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    sifd_dft_reg_SIFD_INT_EN_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_FIC_IP_IRQ_EN_MSK,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_FIC_IP_IRQ_EN_OFF,
                                              value);
}

static INLINE UINT32 sifd_dft_field_FIC_IP_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_IP_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_FIC_IP_IRQ_EN_get", N, 3);
    /* ((0x0000001c + (N) * 0x4) bits 11) field FIC_IP_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    reg_value = sifd_dft_reg_SIFD_INT_EN_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & SIFD_DFT_REG_SIFD_INT_EN_BIT_FIC_IP_IRQ_EN_MSK) >> SIFD_DFT_REG_SIFD_INT_EN_BIT_FIC_IP_IRQ_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_FIC_IP_IRQ_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_DSIS_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_DSIS_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                   sifd_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_DSIS_IRQ_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_DSIS_IRQ_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_DSIS_IRQ_EN_set", N, value );

    /* ((0x0000001c + (N) * 0x4) bits 10) field DSIS_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    sifd_dft_reg_SIFD_INT_EN_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_DSIS_IRQ_EN_MSK,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_DSIS_IRQ_EN_OFF,
                                              value);
}

static INLINE UINT32 sifd_dft_field_DSIS_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_DSIS_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_DSIS_IRQ_EN_get", N, 3);
    /* ((0x0000001c + (N) * 0x4) bits 10) field DSIS_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    reg_value = sifd_dft_reg_SIFD_INT_EN_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & SIFD_DFT_REG_SIFD_INT_EN_BIT_DSIS_IRQ_EN_MSK) >> SIFD_DFT_REG_SIFD_INT_EN_BIT_DSIS_IRQ_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_DSIS_IRQ_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_IL2_RX_IRQ_2ND_EN_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL2_RX_IRQ_2ND_EN_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_RX_IRQ_2ND_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL2_RX_IRQ_2ND_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL2_RX_IRQ_2ND_EN_set", N, value );

    /* ((0x0000001c + (N) * 0x4) bits 9) field IL2_RX_IRQ_2ND_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    sifd_dft_reg_SIFD_INT_EN_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_RX_IRQ_2ND_EN_MSK,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_RX_IRQ_2ND_EN_OFF,
                                              value);
}

static INLINE UINT32 sifd_dft_field_IL2_RX_IRQ_2ND_EN_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL2_RX_IRQ_2ND_EN_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_RX_IRQ_2ND_EN_get", N, 3);
    /* ((0x0000001c + (N) * 0x4) bits 9) field IL2_RX_IRQ_2ND_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    reg_value = sifd_dft_reg_SIFD_INT_EN_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_RX_IRQ_2ND_EN_MSK) >> SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_RX_IRQ_2ND_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL2_RX_IRQ_2ND_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_IL2_RX_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL2_RX_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_RX_IRQ_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL2_RX_IRQ_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL2_RX_IRQ_EN_set", N, value );

    /* ((0x0000001c + (N) * 0x4) bits 8) field IL2_RX_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    sifd_dft_reg_SIFD_INT_EN_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_RX_IRQ_EN_MSK,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_RX_IRQ_EN_OFF,
                                              value);
}

static INLINE UINT32 sifd_dft_field_IL2_RX_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL2_RX_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_RX_IRQ_EN_get", N, 3);
    /* ((0x0000001c + (N) * 0x4) bits 8) field IL2_RX_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    reg_value = sifd_dft_reg_SIFD_INT_EN_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_RX_IRQ_EN_MSK) >> SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_RX_IRQ_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL2_RX_IRQ_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_IL2_TX_IRQ_2ND_EN_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL2_TX_IRQ_2ND_EN_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_TX_IRQ_2ND_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL2_TX_IRQ_2ND_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL2_TX_IRQ_2ND_EN_set", N, value );

    /* ((0x0000001c + (N) * 0x4) bits 7) field IL2_TX_IRQ_2ND_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    sifd_dft_reg_SIFD_INT_EN_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_TX_IRQ_2ND_EN_MSK,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_TX_IRQ_2ND_EN_OFF,
                                              value);
}

static INLINE UINT32 sifd_dft_field_IL2_TX_IRQ_2ND_EN_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL2_TX_IRQ_2ND_EN_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_TX_IRQ_2ND_EN_get", N, 3);
    /* ((0x0000001c + (N) * 0x4) bits 7) field IL2_TX_IRQ_2ND_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    reg_value = sifd_dft_reg_SIFD_INT_EN_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_TX_IRQ_2ND_EN_MSK) >> SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_TX_IRQ_2ND_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL2_TX_IRQ_2ND_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_IL2_TX_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL2_TX_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_TX_IRQ_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL2_TX_IRQ_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL2_TX_IRQ_EN_set", N, value );

    /* ((0x0000001c + (N) * 0x4) bits 6) field IL2_TX_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    sifd_dft_reg_SIFD_INT_EN_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_TX_IRQ_EN_MSK,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_TX_IRQ_EN_OFF,
                                              value);
}

static INLINE UINT32 sifd_dft_field_IL2_TX_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL2_TX_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL2_TX_IRQ_EN_get", N, 3);
    /* ((0x0000001c + (N) * 0x4) bits 6) field IL2_TX_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    reg_value = sifd_dft_reg_SIFD_INT_EN_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_TX_IRQ_EN_MSK) >> SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_TX_IRQ_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL2_TX_IRQ_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_COPI2_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_COPI2_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_COPI2_IRQ_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_COPI2_IRQ_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_COPI2_IRQ_EN_set", N, value );

    /* ((0x0000001c + (N) * 0x4) bits 5) field COPI2_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    sifd_dft_reg_SIFD_INT_EN_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_COPI2_IRQ_EN_MSK,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_COPI2_IRQ_EN_OFF,
                                              value);
}

static INLINE UINT32 sifd_dft_field_COPI2_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_COPI2_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_COPI2_IRQ_EN_get", N, 3);
    /* ((0x0000001c + (N) * 0x4) bits 5) field COPI2_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    reg_value = sifd_dft_reg_SIFD_INT_EN_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & SIFD_DFT_REG_SIFD_INT_EN_BIT_COPI2_IRQ_EN_MSK) >> SIFD_DFT_REG_SIFD_INT_EN_BIT_COPI2_IRQ_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_COPI2_IRQ_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_IL1_RX_IRQ_2ND_EN_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL1_RX_IRQ_2ND_EN_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_RX_IRQ_2ND_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL1_RX_IRQ_2ND_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL1_RX_IRQ_2ND_EN_set", N, value );

    /* ((0x0000001c + (N) * 0x4) bits 4) field IL1_RX_IRQ_2ND_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    sifd_dft_reg_SIFD_INT_EN_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_RX_IRQ_2ND_EN_MSK,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_RX_IRQ_2ND_EN_OFF,
                                              value);
}

static INLINE UINT32 sifd_dft_field_IL1_RX_IRQ_2ND_EN_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL1_RX_IRQ_2ND_EN_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_RX_IRQ_2ND_EN_get", N, 3);
    /* ((0x0000001c + (N) * 0x4) bits 4) field IL1_RX_IRQ_2ND_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    reg_value = sifd_dft_reg_SIFD_INT_EN_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_RX_IRQ_2ND_EN_MSK) >> SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_RX_IRQ_2ND_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL1_RX_IRQ_2ND_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_IL1_RX_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL1_RX_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_RX_IRQ_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL1_RX_IRQ_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL1_RX_IRQ_EN_set", N, value );

    /* ((0x0000001c + (N) * 0x4) bits 3) field IL1_RX_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    sifd_dft_reg_SIFD_INT_EN_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_RX_IRQ_EN_MSK,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_RX_IRQ_EN_OFF,
                                              value);
}

static INLINE UINT32 sifd_dft_field_IL1_RX_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL1_RX_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_RX_IRQ_EN_get", N, 3);
    /* ((0x0000001c + (N) * 0x4) bits 3) field IL1_RX_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    reg_value = sifd_dft_reg_SIFD_INT_EN_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_RX_IRQ_EN_MSK) >> SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_RX_IRQ_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL1_RX_IRQ_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_IL1_TX_IRQ_2ND_EN_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL1_TX_IRQ_2ND_EN_set( sifd_dft_buffer_t *b_ptr,
                                                         sifd_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_TX_IRQ_2ND_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL1_TX_IRQ_2ND_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL1_TX_IRQ_2ND_EN_set", N, value );

    /* ((0x0000001c + (N) * 0x4) bits 2) field IL1_TX_IRQ_2ND_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    sifd_dft_reg_SIFD_INT_EN_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_TX_IRQ_2ND_EN_MSK,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_TX_IRQ_2ND_EN_OFF,
                                              value);
}

static INLINE UINT32 sifd_dft_field_IL1_TX_IRQ_2ND_EN_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL1_TX_IRQ_2ND_EN_get( sifd_dft_buffer_t *b_ptr,
                                                           sifd_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_TX_IRQ_2ND_EN_get", N, 3);
    /* ((0x0000001c + (N) * 0x4) bits 2) field IL1_TX_IRQ_2ND_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    reg_value = sifd_dft_reg_SIFD_INT_EN_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_TX_IRQ_2ND_EN_MSK) >> SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_TX_IRQ_2ND_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL1_TX_IRQ_2ND_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_IL1_TX_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_IL1_TX_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                     sifd_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_TX_IRQ_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_IL1_TX_IRQ_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_IL1_TX_IRQ_EN_set", N, value );

    /* ((0x0000001c + (N) * 0x4) bits 1) field IL1_TX_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    sifd_dft_reg_SIFD_INT_EN_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_TX_IRQ_EN_MSK,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_TX_IRQ_EN_OFF,
                                              value);
}

static INLINE UINT32 sifd_dft_field_IL1_TX_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL1_TX_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                       sifd_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_IL1_TX_IRQ_EN_get", N, 3);
    /* ((0x0000001c + (N) * 0x4) bits 1) field IL1_TX_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    reg_value = sifd_dft_reg_SIFD_INT_EN_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_TX_IRQ_EN_MSK) >> SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_TX_IRQ_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_IL1_TX_IRQ_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_COPI1_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_COPI1_IRQ_EN_set( sifd_dft_buffer_t *b_ptr,
                                                    sifd_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_COPI1_IRQ_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_COPI1_IRQ_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_COPI1_IRQ_EN_set", N, value );

    /* ((0x0000001c + (N) * 0x4) bits 0) field COPI1_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    sifd_dft_reg_SIFD_INT_EN_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_COPI1_IRQ_EN_MSK,
                                              SIFD_DFT_REG_SIFD_INT_EN_BIT_COPI1_IRQ_EN_OFF,
                                              value);
}

static INLINE UINT32 sifd_dft_field_COPI1_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_COPI1_IRQ_EN_get( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_COPI1_IRQ_EN_get", N, 3);
    /* ((0x0000001c + (N) * 0x4) bits 0) field COPI1_IRQ_EN of register PMC_SIFD_DFT_REG_SIFD_INT_EN index N=0..3 */
    reg_value = sifd_dft_reg_SIFD_INT_EN_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & SIFD_DFT_REG_SIFD_INT_EN_BIT_COPI1_IRQ_EN_MSK) >> SIFD_DFT_REG_SIFD_INT_EN_BIT_COPI1_IRQ_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_COPI1_IRQ_EN_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size40
 * ==================================================================================
 */
static INLINE void sifd_dft_field_E1_TX_LANE_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_E1_TX_LANE_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 39)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_E1_TX_LANE_SEL_set", N, 39);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_E1_TX_LANE_SEL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_E1_TX_LANE_SEL_set", N, value );

    /* ((0x00000228 + (N) * 0x4) bits 25:24) field E1_TX_LANE_SEL of register PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS index N=0..39 */
    sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_field_set( b_ptr,
                                                              h_ptr,
                                                              N,
                                                              SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_LANE_SEL_MSK,
                                                              SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_LANE_SEL_OFF,
                                                              value);
}

static INLINE UINT32 sifd_dft_field_E1_TX_LANE_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_E1_TX_LANE_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 39)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_E1_TX_LANE_SEL_get", N, 39);
    /* ((0x00000228 + (N) * 0x4) bits 25:24) field E1_TX_LANE_SEL of register PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS index N=0..39 */
    reg_value = sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_read( b_ptr,
                                                                     h_ptr,
                                                                     N);
    value = (reg_value & SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_LANE_SEL_MSK) >> SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_LANE_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_E1_TX_LANE_SEL_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_range_E1_TX_LANE_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_range_E1_TX_LANE_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 39)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_range_E1_TX_LANE_SEL_set", N, 39);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_E1_TX_LANE_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_E1_TX_LANE_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_E1_TX_LANE_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000228 + (N) * 0x4) bits 25:24) field E1_TX_LANE_SEL of register PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS index N=0..39 */
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
        /* ((0x00000228 + (N) * 0x4) bits 25:24) field E1_TX_LANE_SEL of register PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS index N=0..39 */
        sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_field_set( b_ptr,
                                                                  h_ptr,
                                                                  N,
                                                                  subfield_mask << (SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_LANE_SEL_OFF + subfield_offset),
                                                                  SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_LANE_SEL_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 sifd_dft_field_range_E1_TX_LANE_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_range_E1_TX_LANE_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
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

    if (N > 39)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_range_E1_TX_LANE_SEL_get", N, 39);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_E1_TX_LANE_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_E1_TX_LANE_SEL_get", stop_bit, 1 );
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
    /* ((0x00000228 + (N) * 0x4) bits 25:24) field E1_TX_LANE_SEL of register PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS index N=0..39 */
    reg_value = sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_read( b_ptr,
                                                                     h_ptr,
                                                                     N);
    field_value = (reg_value & SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_LANE_SEL_MSK)
                  >> SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_LANE_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_LANE_SEL_MSK, SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_LANE_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_E1_TX_LANE_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sifd_dft_field_E1_TX_DESKEW_CONST_DELAY_EN_set( sifd_dft_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_E1_TX_DESKEW_CONST_DELAY_EN_set( sifd_dft_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value )
{
    if (N > 39)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_E1_TX_DESKEW_CONST_DELAY_EN_set", N, 39);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_E1_TX_DESKEW_CONST_DELAY_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_E1_TX_DESKEW_CONST_DELAY_EN_set", N, value );

    /* ((0x00000228 + (N) * 0x4) bits 20) field E1_TX_DESKEW_CONST_DELAY_EN of register PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS index N=0..39 */
    sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_field_set( b_ptr,
                                                              h_ptr,
                                                              N,
                                                              SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_DESKEW_CONST_DELAY_EN_MSK,
                                                              SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_DESKEW_CONST_DELAY_EN_OFF,
                                                              value);
}

static INLINE UINT32 sifd_dft_field_E1_TX_DESKEW_CONST_DELAY_EN_get( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_E1_TX_DESKEW_CONST_DELAY_EN_get( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 39)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_E1_TX_DESKEW_CONST_DELAY_EN_get", N, 39);
    /* ((0x00000228 + (N) * 0x4) bits 20) field E1_TX_DESKEW_CONST_DELAY_EN of register PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS index N=0..39 */
    reg_value = sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_read( b_ptr,
                                                                     h_ptr,
                                                                     N);
    value = (reg_value & SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_DESKEW_CONST_DELAY_EN_MSK) >> SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_DESKEW_CONST_DELAY_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_E1_TX_DESKEW_CONST_DELAY_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_E1_TX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_E1_TX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 39)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_E1_TX_NUM_DESKEW_STAGES_set", N, 39);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_E1_TX_NUM_DESKEW_STAGES_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_E1_TX_NUM_DESKEW_STAGES_set", N, value );

    /* ((0x00000228 + (N) * 0x4) bits 19:16) field E1_TX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS index N=0..39 */
    sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_field_set( b_ptr,
                                                              h_ptr,
                                                              N,
                                                              SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_NUM_DESKEW_STAGES_MSK,
                                                              SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_NUM_DESKEW_STAGES_OFF,
                                                              value);
}

static INLINE UINT32 sifd_dft_field_E1_TX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_E1_TX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 39)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_E1_TX_NUM_DESKEW_STAGES_get", N, 39);
    /* ((0x00000228 + (N) * 0x4) bits 19:16) field E1_TX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS index N=0..39 */
    reg_value = sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_read( b_ptr,
                                                                     h_ptr,
                                                                     N);
    value = (reg_value & SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_NUM_DESKEW_STAGES_MSK) >> SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_NUM_DESKEW_STAGES_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_E1_TX_NUM_DESKEW_STAGES_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_range_E1_TX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_range_E1_TX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (N > 39)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_range_E1_TX_NUM_DESKEW_STAGES_set", N, 39);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_E1_TX_NUM_DESKEW_STAGES_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_E1_TX_NUM_DESKEW_STAGES_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_E1_TX_NUM_DESKEW_STAGES_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000228 + (N) * 0x4) bits 19:16) field E1_TX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS index N=0..39 */
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
        /* ((0x00000228 + (N) * 0x4) bits 19:16) field E1_TX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS index N=0..39 */
        sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_field_set( b_ptr,
                                                                  h_ptr,
                                                                  N,
                                                                  subfield_mask << (SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_NUM_DESKEW_STAGES_OFF + subfield_offset),
                                                                  SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_NUM_DESKEW_STAGES_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 sifd_dft_field_range_E1_TX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_range_E1_TX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
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

    if (N > 39)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_range_E1_TX_NUM_DESKEW_STAGES_get", N, 39);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_E1_TX_NUM_DESKEW_STAGES_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_E1_TX_NUM_DESKEW_STAGES_get", stop_bit, 3 );
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
    /* ((0x00000228 + (N) * 0x4) bits 19:16) field E1_TX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS index N=0..39 */
    reg_value = sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_read( b_ptr,
                                                                     h_ptr,
                                                                     N);
    field_value = (reg_value & SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_NUM_DESKEW_STAGES_MSK)
                  >> SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_NUM_DESKEW_STAGES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_NUM_DESKEW_STAGES_MSK, SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_NUM_DESKEW_STAGES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_E1_TX_NUM_DESKEW_STAGES_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sifd_dft_field_E1_RX_DESKEW_CONST_DELAY_EN_set( sifd_dft_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_E1_RX_DESKEW_CONST_DELAY_EN_set( sifd_dft_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value )
{
    if (N > 39)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_E1_RX_DESKEW_CONST_DELAY_EN_set", N, 39);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_E1_RX_DESKEW_CONST_DELAY_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_E1_RX_DESKEW_CONST_DELAY_EN_set", N, value );

    /* ((0x00000228 + (N) * 0x4) bits 12) field E1_RX_DESKEW_CONST_DELAY_EN of register PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS index N=0..39 */
    sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_field_set( b_ptr,
                                                              h_ptr,
                                                              N,
                                                              SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_RX_DESKEW_CONST_DELAY_EN_MSK,
                                                              SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_RX_DESKEW_CONST_DELAY_EN_OFF,
                                                              value);
}

static INLINE UINT32 sifd_dft_field_E1_RX_DESKEW_CONST_DELAY_EN_get( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_E1_RX_DESKEW_CONST_DELAY_EN_get( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 39)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_E1_RX_DESKEW_CONST_DELAY_EN_get", N, 39);
    /* ((0x00000228 + (N) * 0x4) bits 12) field E1_RX_DESKEW_CONST_DELAY_EN of register PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS index N=0..39 */
    reg_value = sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_read( b_ptr,
                                                                     h_ptr,
                                                                     N);
    value = (reg_value & SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_RX_DESKEW_CONST_DELAY_EN_MSK) >> SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_RX_DESKEW_CONST_DELAY_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_E1_RX_DESKEW_CONST_DELAY_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_E1_RX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_E1_RX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 39)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_E1_RX_NUM_DESKEW_STAGES_set", N, 39);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_E1_RX_NUM_DESKEW_STAGES_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_E1_RX_NUM_DESKEW_STAGES_set", N, value );

    /* ((0x00000228 + (N) * 0x4) bits 11:8) field E1_RX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS index N=0..39 */
    sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_field_set( b_ptr,
                                                              h_ptr,
                                                              N,
                                                              SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_RX_NUM_DESKEW_STAGES_MSK,
                                                              SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_RX_NUM_DESKEW_STAGES_OFF,
                                                              value);
}

static INLINE UINT32 sifd_dft_field_E1_RX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_E1_RX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 39)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_E1_RX_NUM_DESKEW_STAGES_get", N, 39);
    /* ((0x00000228 + (N) * 0x4) bits 11:8) field E1_RX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS index N=0..39 */
    reg_value = sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_read( b_ptr,
                                                                     h_ptr,
                                                                     N);
    value = (reg_value & SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_RX_NUM_DESKEW_STAGES_MSK) >> SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_RX_NUM_DESKEW_STAGES_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_E1_RX_NUM_DESKEW_STAGES_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_range_E1_RX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_range_E1_RX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (N > 39)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_range_E1_RX_NUM_DESKEW_STAGES_set", N, 39);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_E1_RX_NUM_DESKEW_STAGES_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_E1_RX_NUM_DESKEW_STAGES_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_E1_RX_NUM_DESKEW_STAGES_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000228 + (N) * 0x4) bits 11:8) field E1_RX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS index N=0..39 */
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
        /* ((0x00000228 + (N) * 0x4) bits 11:8) field E1_RX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS index N=0..39 */
        sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_field_set( b_ptr,
                                                                  h_ptr,
                                                                  N,
                                                                  subfield_mask << (SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_RX_NUM_DESKEW_STAGES_OFF + subfield_offset),
                                                                  SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_RX_NUM_DESKEW_STAGES_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 sifd_dft_field_range_E1_RX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_range_E1_RX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
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

    if (N > 39)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_range_E1_RX_NUM_DESKEW_STAGES_get", N, 39);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_E1_RX_NUM_DESKEW_STAGES_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_E1_RX_NUM_DESKEW_STAGES_get", stop_bit, 3 );
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
    /* ((0x00000228 + (N) * 0x4) bits 11:8) field E1_RX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS index N=0..39 */
    reg_value = sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_read( b_ptr,
                                                                     h_ptr,
                                                                     N);
    field_value = (reg_value & SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_RX_NUM_DESKEW_STAGES_MSK)
                  >> SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_RX_NUM_DESKEW_STAGES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_RX_NUM_DESKEW_STAGES_MSK, SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_RX_NUM_DESKEW_STAGES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_E1_RX_NUM_DESKEW_STAGES_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size8
 * ==================================================================================
 */
static INLINE void sifd_dft_field_LOTN_TX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_LOTN_TX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_LOTN_TX_LANE_DIS_set", N, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_LOTN_TX_LANE_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_LOTN_TX_LANE_DIS_set", N, value );

    /* ((0x00000130 + (N) * 0x4) bits 1) field LOTN_TX_LANE_DIS of register PMC_SIFD_DFT_REG_LINE_OTN_CFGS index N=0..7 */
    sifd_dft_reg_LINE_OTN_CFGS_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                SIFD_DFT_REG_LINE_OTN_CFGS_BIT_LOTN_TX_LANE_DIS_MSK,
                                                SIFD_DFT_REG_LINE_OTN_CFGS_BIT_LOTN_TX_LANE_DIS_OFF,
                                                value);
}

static INLINE UINT32 sifd_dft_field_LOTN_TX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_LOTN_TX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_LOTN_TX_LANE_DIS_get", N, 7);
    /* ((0x00000130 + (N) * 0x4) bits 1) field LOTN_TX_LANE_DIS of register PMC_SIFD_DFT_REG_LINE_OTN_CFGS index N=0..7 */
    reg_value = sifd_dft_reg_LINE_OTN_CFGS_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & SIFD_DFT_REG_LINE_OTN_CFGS_BIT_LOTN_TX_LANE_DIS_MSK) >> SIFD_DFT_REG_LINE_OTN_CFGS_BIT_LOTN_TX_LANE_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_LOTN_TX_LANE_DIS_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_LOTN_RX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_LOTN_RX_LANE_DIS_set( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_LOTN_RX_LANE_DIS_set", N, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_LOTN_RX_LANE_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_LOTN_RX_LANE_DIS_set", N, value );

    /* ((0x00000130 + (N) * 0x4) bits 0) field LOTN_RX_LANE_DIS of register PMC_SIFD_DFT_REG_LINE_OTN_CFGS index N=0..7 */
    sifd_dft_reg_LINE_OTN_CFGS_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                SIFD_DFT_REG_LINE_OTN_CFGS_BIT_LOTN_RX_LANE_DIS_MSK,
                                                SIFD_DFT_REG_LINE_OTN_CFGS_BIT_LOTN_RX_LANE_DIS_OFF,
                                                value);
}

static INLINE UINT32 sifd_dft_field_LOTN_RX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_LOTN_RX_LANE_DIS_get( sifd_dft_buffer_t *b_ptr,
                                                          sifd_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_LOTN_RX_LANE_DIS_get", N, 7);
    /* ((0x00000130 + (N) * 0x4) bits 0) field LOTN_RX_LANE_DIS of register PMC_SIFD_DFT_REG_LINE_OTN_CFGS index N=0..7 */
    reg_value = sifd_dft_reg_LINE_OTN_CFGS_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & SIFD_DFT_REG_LINE_OTN_CFGS_BIT_LOTN_RX_LANE_DIS_MSK) >> SIFD_DFT_REG_LINE_OTN_CFGS_BIT_LOTN_RX_LANE_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_LOTN_RX_LANE_DIS_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_N2_TX_LANE_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_N2_TX_LANE_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                      sifd_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_N2_TX_LANE_SEL_set", N, 7);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_N2_TX_LANE_SEL_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_N2_TX_LANE_SEL_set", N, value );

    /* ((0x00000308 + (N) * 0x4) bits 25:24) field N2_TX_LANE_SEL of register PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS index N=0..7 */
    sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_field_set( b_ptr,
                                                              h_ptr,
                                                              N,
                                                              SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_LANE_SEL_MSK,
                                                              SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_LANE_SEL_OFF,
                                                              value);
}

static INLINE UINT32 sifd_dft_field_N2_TX_LANE_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_N2_TX_LANE_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                        sifd_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_N2_TX_LANE_SEL_get", N, 7);
    /* ((0x00000308 + (N) * 0x4) bits 25:24) field N2_TX_LANE_SEL of register PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS index N=0..7 */
    reg_value = sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_read( b_ptr,
                                                                     h_ptr,
                                                                     N);
    value = (reg_value & SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_LANE_SEL_MSK) >> SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_LANE_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_N2_TX_LANE_SEL_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_range_N2_TX_LANE_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_range_N2_TX_LANE_SEL_set( sifd_dft_buffer_t *b_ptr,
                                                            sifd_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_range_N2_TX_LANE_SEL_set", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_N2_TX_LANE_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_N2_TX_LANE_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_N2_TX_LANE_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000308 + (N) * 0x4) bits 25:24) field N2_TX_LANE_SEL of register PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS index N=0..7 */
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
        /* ((0x00000308 + (N) * 0x4) bits 25:24) field N2_TX_LANE_SEL of register PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS index N=0..7 */
        sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_field_set( b_ptr,
                                                                  h_ptr,
                                                                  N,
                                                                  subfield_mask << (SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_LANE_SEL_OFF + subfield_offset),
                                                                  SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_LANE_SEL_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 sifd_dft_field_range_N2_TX_LANE_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_range_N2_TX_LANE_SEL_get( sifd_dft_buffer_t *b_ptr,
                                                              sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_range_N2_TX_LANE_SEL_get", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_N2_TX_LANE_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_N2_TX_LANE_SEL_get", stop_bit, 1 );
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
    /* ((0x00000308 + (N) * 0x4) bits 25:24) field N2_TX_LANE_SEL of register PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS index N=0..7 */
    reg_value = sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_read( b_ptr,
                                                                     h_ptr,
                                                                     N);
    field_value = (reg_value & SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_LANE_SEL_MSK)
                  >> SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_LANE_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_LANE_SEL_MSK, SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_LANE_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_N2_TX_LANE_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sifd_dft_field_N2_TX_DESKEW_CONST_DELAY_EN_set( sifd_dft_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_N2_TX_DESKEW_CONST_DELAY_EN_set( sifd_dft_buffer_t *b_ptr,
                                                                   sifd_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value )
{
    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_N2_TX_DESKEW_CONST_DELAY_EN_set", N, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_N2_TX_DESKEW_CONST_DELAY_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_N2_TX_DESKEW_CONST_DELAY_EN_set", N, value );

    /* ((0x00000308 + (N) * 0x4) bits 19) field N2_TX_DESKEW_CONST_DELAY_EN of register PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS index N=0..7 */
    sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_field_set( b_ptr,
                                                              h_ptr,
                                                              N,
                                                              SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_DESKEW_CONST_DELAY_EN_MSK,
                                                              SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_DESKEW_CONST_DELAY_EN_OFF,
                                                              value);
}

static INLINE UINT32 sifd_dft_field_N2_TX_DESKEW_CONST_DELAY_EN_get( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_N2_TX_DESKEW_CONST_DELAY_EN_get( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_N2_TX_DESKEW_CONST_DELAY_EN_get", N, 7);
    /* ((0x00000308 + (N) * 0x4) bits 19) field N2_TX_DESKEW_CONST_DELAY_EN of register PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS index N=0..7 */
    reg_value = sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_read( b_ptr,
                                                                     h_ptr,
                                                                     N);
    value = (reg_value & SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_DESKEW_CONST_DELAY_EN_MSK) >> SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_DESKEW_CONST_DELAY_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_N2_TX_DESKEW_CONST_DELAY_EN_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_N2_TX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_N2_TX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_N2_TX_NUM_DESKEW_STAGES_set", N, 7);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_N2_TX_NUM_DESKEW_STAGES_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_N2_TX_NUM_DESKEW_STAGES_set", N, value );

    /* ((0x00000308 + (N) * 0x4) bits 18:16) field N2_TX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS index N=0..7 */
    sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_field_set( b_ptr,
                                                              h_ptr,
                                                              N,
                                                              SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_NUM_DESKEW_STAGES_MSK,
                                                              SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_NUM_DESKEW_STAGES_OFF,
                                                              value);
}

static INLINE UINT32 sifd_dft_field_N2_TX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_N2_TX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_N2_TX_NUM_DESKEW_STAGES_get", N, 7);
    /* ((0x00000308 + (N) * 0x4) bits 18:16) field N2_TX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS index N=0..7 */
    reg_value = sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_read( b_ptr,
                                                                     h_ptr,
                                                                     N);
    value = (reg_value & SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_NUM_DESKEW_STAGES_MSK) >> SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_NUM_DESKEW_STAGES_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_N2_TX_NUM_DESKEW_STAGES_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_range_N2_TX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_range_N2_TX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_range_N2_TX_NUM_DESKEW_STAGES_set", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_N2_TX_NUM_DESKEW_STAGES_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_N2_TX_NUM_DESKEW_STAGES_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_N2_TX_NUM_DESKEW_STAGES_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000308 + (N) * 0x4) bits 18:16) field N2_TX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS index N=0..7 */
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
        /* ((0x00000308 + (N) * 0x4) bits 18:16) field N2_TX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS index N=0..7 */
        sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_field_set( b_ptr,
                                                                  h_ptr,
                                                                  N,
                                                                  subfield_mask << (SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_NUM_DESKEW_STAGES_OFF + subfield_offset),
                                                                  SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_NUM_DESKEW_STAGES_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 sifd_dft_field_range_N2_TX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_range_N2_TX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_range_N2_TX_NUM_DESKEW_STAGES_get", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_N2_TX_NUM_DESKEW_STAGES_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_N2_TX_NUM_DESKEW_STAGES_get", stop_bit, 2 );
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
    /* ((0x00000308 + (N) * 0x4) bits 18:16) field N2_TX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS index N=0..7 */
    reg_value = sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_read( b_ptr,
                                                                     h_ptr,
                                                                     N);
    field_value = (reg_value & SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_NUM_DESKEW_STAGES_MSK)
                  >> SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_NUM_DESKEW_STAGES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_NUM_DESKEW_STAGES_MSK, SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_NUM_DESKEW_STAGES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_N2_TX_NUM_DESKEW_STAGES_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void sifd_dft_field_N2_RX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_N2_RX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                               sifd_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_N2_RX_NUM_DESKEW_STAGES_set", N, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sifd_dft_field_N2_RX_NUM_DESKEW_STAGES_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "sifd_dft_field_N2_RX_NUM_DESKEW_STAGES_set", N, value );

    /* ((0x00000308 + (N) * 0x4) bits 11:8) field N2_RX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS index N=0..7 */
    sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_field_set( b_ptr,
                                                              h_ptr,
                                                              N,
                                                              SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_RX_NUM_DESKEW_STAGES_MSK,
                                                              SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_RX_NUM_DESKEW_STAGES_OFF,
                                                              value);
}

static INLINE UINT32 sifd_dft_field_N2_RX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_N2_RX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                 sifd_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_N2_RX_NUM_DESKEW_STAGES_get", N, 7);
    /* ((0x00000308 + (N) * 0x4) bits 11:8) field N2_RX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS index N=0..7 */
    reg_value = sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_read( b_ptr,
                                                                     h_ptr,
                                                                     N);
    value = (reg_value & SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_RX_NUM_DESKEW_STAGES_MSK) >> SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_RX_NUM_DESKEW_STAGES_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sifd_dft_field_N2_RX_NUM_DESKEW_STAGES_get", N, value );

    return value;
}
static INLINE void sifd_dft_field_range_N2_RX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void sifd_dft_field_range_N2_RX_NUM_DESKEW_STAGES_set( sifd_dft_buffer_t *b_ptr,
                                                                     sifd_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_range_N2_RX_NUM_DESKEW_STAGES_set", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_N2_RX_NUM_DESKEW_STAGES_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_N2_RX_NUM_DESKEW_STAGES_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_N2_RX_NUM_DESKEW_STAGES_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000308 + (N) * 0x4) bits 11:8) field N2_RX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS index N=0..7 */
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
        /* ((0x00000308 + (N) * 0x4) bits 11:8) field N2_RX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS index N=0..7 */
        sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_field_set( b_ptr,
                                                                  h_ptr,
                                                                  N,
                                                                  subfield_mask << (SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_RX_NUM_DESKEW_STAGES_OFF + subfield_offset),
                                                                  SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_RX_NUM_DESKEW_STAGES_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 sifd_dft_field_range_N2_RX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_range_N2_RX_NUM_DESKEW_STAGES_get( sifd_dft_buffer_t *b_ptr,
                                                                       sifd_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "sifd_dft_field_range_N2_RX_NUM_DESKEW_STAGES_get", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sifd_dft_field_range_N2_RX_NUM_DESKEW_STAGES_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sifd_dft_field_range_N2_RX_NUM_DESKEW_STAGES_get", stop_bit, 3 );
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
    /* ((0x00000308 + (N) * 0x4) bits 11:8) field N2_RX_NUM_DESKEW_STAGES of register PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS index N=0..7 */
    reg_value = sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_read( b_ptr,
                                                                     h_ptr,
                                                                     N);
    field_value = (reg_value & SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_RX_NUM_DESKEW_STAGES_MSK)
                  >> SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_RX_NUM_DESKEW_STAGES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_RX_NUM_DESKEW_STAGES_MSK, SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_RX_NUM_DESKEW_STAGES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "sifd_dft_field_range_N2_RX_NUM_DESKEW_STAGES_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 sifd_dft_field_MPMA_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_MPMA_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 14) field MPMA_IRQ of register PMC_SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT */
    reg_value = sifd_dft_reg_SIFD_SUBSYSTEM_INT_STAT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_MPMA_IRQ_MSK) >> SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_MPMA_IRQ_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_MPMA_IRQ_get", value );

    return value;
}
static INLINE UINT32 sifd_dft_field_MPMO_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_MPMO_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 13) field MPMO_IRQ of register PMC_SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT */
    reg_value = sifd_dft_reg_SIFD_SUBSYSTEM_INT_STAT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_MPMO_IRQ_MSK) >> SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_MPMO_IRQ_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_MPMO_IRQ_get", value );

    return value;
}
static INLINE UINT32 sifd_dft_field_FIC_WRAP_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_WRAP_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 12) field FIC_WRAP_IRQ of register PMC_SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT */
    reg_value = sifd_dft_reg_SIFD_SUBSYSTEM_INT_STAT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_FIC_WRAP_IRQ_MSK) >> SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_FIC_WRAP_IRQ_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_FIC_WRAP_IRQ_get", value );

    return value;
}
static INLINE UINT32 sifd_dft_field_FIC_IP_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_FIC_IP_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 11) field FIC_IP_IRQ of register PMC_SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT */
    reg_value = sifd_dft_reg_SIFD_SUBSYSTEM_INT_STAT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_FIC_IP_IRQ_MSK) >> SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_FIC_IP_IRQ_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_FIC_IP_IRQ_get", value );

    return value;
}
static INLINE UINT32 sifd_dft_field_DSIS_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_DSIS_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 10) field DSIS_IRQ of register PMC_SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT */
    reg_value = sifd_dft_reg_SIFD_SUBSYSTEM_INT_STAT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_DSIS_IRQ_MSK) >> SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_DSIS_IRQ_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_DSIS_IRQ_get", value );

    return value;
}
static INLINE UINT32 sifd_dft_field_IL2_RX_IRQ_2ND_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL2_RX_IRQ_2ND_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 9) field IL2_RX_IRQ_2ND of register PMC_SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT */
    reg_value = sifd_dft_reg_SIFD_SUBSYSTEM_INT_STAT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL2_RX_IRQ_2ND_MSK) >> SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL2_RX_IRQ_2ND_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_IL2_RX_IRQ_2ND_get", value );

    return value;
}
static INLINE UINT32 sifd_dft_field_IL2_RX_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL2_RX_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 8) field IL2_RX_IRQ of register PMC_SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT */
    reg_value = sifd_dft_reg_SIFD_SUBSYSTEM_INT_STAT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL2_RX_IRQ_MSK) >> SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL2_RX_IRQ_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_IL2_RX_IRQ_get", value );

    return value;
}
static INLINE UINT32 sifd_dft_field_IL2_TX_IRQ_2ND_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL2_TX_IRQ_2ND_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 7) field IL2_TX_IRQ_2ND of register PMC_SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT */
    reg_value = sifd_dft_reg_SIFD_SUBSYSTEM_INT_STAT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL2_TX_IRQ_2ND_MSK) >> SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL2_TX_IRQ_2ND_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_IL2_TX_IRQ_2ND_get", value );

    return value;
}
static INLINE UINT32 sifd_dft_field_IL2_TX_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL2_TX_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 6) field IL2_TX_IRQ of register PMC_SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT */
    reg_value = sifd_dft_reg_SIFD_SUBSYSTEM_INT_STAT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL2_TX_IRQ_MSK) >> SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL2_TX_IRQ_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_IL2_TX_IRQ_get", value );

    return value;
}
static INLINE UINT32 sifd_dft_field_COPI2_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_COPI2_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 5) field COPI2_IRQ of register PMC_SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT */
    reg_value = sifd_dft_reg_SIFD_SUBSYSTEM_INT_STAT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_COPI2_IRQ_MSK) >> SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_COPI2_IRQ_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_COPI2_IRQ_get", value );

    return value;
}
static INLINE UINT32 sifd_dft_field_IL1_RX_IRQ_2ND_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL1_RX_IRQ_2ND_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 4) field IL1_RX_IRQ_2ND of register PMC_SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT */
    reg_value = sifd_dft_reg_SIFD_SUBSYSTEM_INT_STAT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL1_RX_IRQ_2ND_MSK) >> SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL1_RX_IRQ_2ND_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_IL1_RX_IRQ_2ND_get", value );

    return value;
}
static INLINE UINT32 sifd_dft_field_IL1_RX_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL1_RX_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 3) field IL1_RX_IRQ of register PMC_SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT */
    reg_value = sifd_dft_reg_SIFD_SUBSYSTEM_INT_STAT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL1_RX_IRQ_MSK) >> SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL1_RX_IRQ_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_IL1_RX_IRQ_get", value );

    return value;
}
static INLINE UINT32 sifd_dft_field_IL1_TX_IRQ_2ND_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL1_TX_IRQ_2ND_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 2) field IL1_TX_IRQ_2ND of register PMC_SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT */
    reg_value = sifd_dft_reg_SIFD_SUBSYSTEM_INT_STAT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL1_TX_IRQ_2ND_MSK) >> SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL1_TX_IRQ_2ND_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_IL1_TX_IRQ_2ND_get", value );

    return value;
}
static INLINE UINT32 sifd_dft_field_IL1_TX_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_IL1_TX_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 1) field IL1_TX_IRQ of register PMC_SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT */
    reg_value = sifd_dft_reg_SIFD_SUBSYSTEM_INT_STAT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL1_TX_IRQ_MSK) >> SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL1_TX_IRQ_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_IL1_TX_IRQ_get", value );

    return value;
}
static INLINE UINT32 sifd_dft_field_COPI1_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sifd_dft_field_COPI1_IRQ_get( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 0) field COPI1_IRQ of register PMC_SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT */
    reg_value = sifd_dft_reg_SIFD_SUBSYSTEM_INT_STAT_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_COPI1_IRQ_MSK) >> SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_COPI1_IRQ_OFF;
    IOLOG( "%s -> 0x%08x", "sifd_dft_field_COPI1_IRQ_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SIFD_DFT_IO_INLINE_H */
