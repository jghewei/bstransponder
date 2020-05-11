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
 *     and register accessor functions for the enh_8g_csu block
 *****************************************************************************/
#ifndef _ENH_8G_CSU_IO_INLINE_H
#define _ENH_8G_CSU_IO_INLINE_H

#include "sfi51_d8_tx_18x.h"
#include "enh_8g_csu_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ENH_8G_CSU_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for enh_8g_csu
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
    sfi51_d8_tx_18x_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} enh_8g_csu_buffer_t;
static INLINE void enh_8g_csu_buffer_init( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_buffer_init( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "enh_8g_csu_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void enh_8g_csu_buffer_flush( enh_8g_csu_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_buffer_flush( enh_8g_csu_buffer_t *b_ptr )
{
    IOLOG( "enh_8g_csu_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 enh_8g_csu_reg_read( enh_8g_csu_buffer_t *b_ptr,
                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_read( enh_8g_csu_buffer_t *b_ptr,
                                          sfi51_d8_tx_18x_handle_t *h_ptr,
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
static INLINE void enh_8g_csu_reg_write( enh_8g_csu_buffer_t *b_ptr,
                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_write( enh_8g_csu_buffer_t *b_ptr,
                                         sfi51_d8_tx_18x_handle_t *h_ptr,
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

static INLINE void enh_8g_csu_field_set( enh_8g_csu_buffer_t *b_ptr,
                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 mask,
                                         UINT32 unused_mask,
                                         UINT32 ofs,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_set( enh_8g_csu_buffer_t *b_ptr,
                                         sfi51_d8_tx_18x_handle_t *h_ptr,
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

static INLINE void enh_8g_csu_action_on_write_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                         UINT32 mem_type,
                                                         UINT32 reg,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_action_on_write_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
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

static INLINE void enh_8g_csu_burst_read( enh_8g_csu_buffer_t *b_ptr,
                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                          UINT32 mem_type,
                                          UINT32 reg,
                                          UINT32 len,
                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_burst_read( enh_8g_csu_buffer_t *b_ptr,
                                          sfi51_d8_tx_18x_handle_t *h_ptr,
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

static INLINE void enh_8g_csu_burst_write( enh_8g_csu_buffer_t *b_ptr,
                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 len,
                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_burst_write( enh_8g_csu_buffer_t *b_ptr,
                                           sfi51_d8_tx_18x_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_poll( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 mem_type,
                                                    UINT32 reg,
                                                    UINT32 mask,
                                                    UINT32 value,
                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                    UINT32 max_count,
                                                    UINT32 *num_failed_polls,
                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_poll( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
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
 *  register access functions for enh_8g_csu
 * ==================================================================================
 */

static INLINE void enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG,
                          value);
}

static INLINE void enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG,
                          mask,
                          PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_MABC_RST_CNFG_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_MABC_RST_CNFG_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_MABC_RST_CNFG_REG_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_MABC_RST_CNFG_REG,
                          value);
}

static INLINE void enh_8g_csu_reg_MABC_RST_CNFG_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_MABC_RST_CNFG_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_MABC_RST_CNFG_REG_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_MABC_RST_CNFG_REG,
                          mask,
                          PMC_ENH_8G_CSU_REG_MABC_RST_CNFG_REG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_MABC_RST_CNFG_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_MABC_RST_CNFG_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_MABC_RST_CNFG_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_MABC_RST_CNFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_CLK_DGLCH_CNFG_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_CLK_DGLCH_CNFG_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_CLK_DGLCH_CNFG_REG_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG,
                          value);
}

static INLINE void enh_8g_csu_reg_CLK_DGLCH_CNFG_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_CLK_DGLCH_CNFG_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_CLK_DGLCH_CNFG_REG_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG,
                          mask,
                          PMC_ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_CLK_DGLCH_CNFG_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_CLK_DGLCH_CNFG_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_CLK_DGLCH_CNFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_PLL_CNFG_REG1_write( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_PLL_CNFG_REG1_write( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_PLL_CNFG_REG1_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_PLL_CNFG_REG1,
                          value);
}

static INLINE void enh_8g_csu_reg_PLL_CNFG_REG1_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_PLL_CNFG_REG1_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_PLL_CNFG_REG1_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_PLL_CNFG_REG1,
                          mask,
                          PMC_ENH_8G_CSU_REG_PLL_CNFG_REG1_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_PLL_CNFG_REG1_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_PLL_CNFG_REG1_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_PLL_CNFG_REG1);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_PLL_CNFG_REG1_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_PLL_CLK_CNTR_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_PLL_CLK_CNTR_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_PLL_CLK_CNTR_REG_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_PLL_CLK_CNTR_REG,
                          value);
}

static INLINE void enh_8g_csu_reg_PLL_CLK_CNTR_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_PLL_CLK_CNTR_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_PLL_CLK_CNTR_REG_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_PLL_CLK_CNTR_REG,
                          mask,
                          PMC_ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_PLL_CLK_CNTR_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_PLL_CLK_CNTR_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_PLL_CLK_CNTR_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_PLL_CLK_CNTR_REG_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_PLL_CNTR_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_PLL_CNTR_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_PLL_CNTR_REG_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_PLL_CNTR_REG,
                          value);
}

static INLINE void enh_8g_csu_reg_PLL_CNTR_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_PLL_CNTR_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_PLL_CNTR_REG_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_PLL_CNTR_REG,
                          mask,
                          PMC_ENH_8G_CSU_REG_PLL_CNTR_REG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_PLL_CNTR_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_PLL_CNTR_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_PLL_CNTR_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_PLL_CNTR_REG_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_write( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_write( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1,
                          value);
}

static INLINE void enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1,
                          mask,
                          PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_write( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_write( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2,
                          value);
}

static INLINE void enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2,
                          mask,
                          PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_write( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_write( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1,
                          value);
}

static INLINE void enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1,
                          mask,
                          PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_write( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_write( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2,
                          value);
}

static INLINE void enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2,
                          mask,
                          PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_write( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_write( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3,
                          value);
}

static INLINE void enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3,
                          mask,
                          PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_CLK_DIV_CNFG_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_CLK_DIV_CNFG_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_CLK_DIV_CNFG_REG_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_CLK_DIV_CNFG_REG,
                          value);
}

static INLINE void enh_8g_csu_reg_CLK_DIV_CNFG_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_CLK_DIV_CNFG_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_CLK_DIV_CNFG_REG_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_CLK_DIV_CNFG_REG,
                          mask,
                          PMC_ENH_8G_CSU_REG_CLK_DIV_CNFG_REG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_CLK_DIV_CNFG_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_CLK_DIV_CNFG_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_CLK_DIV_CNFG_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_CLK_DIV_CNFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_MABC_TC_CNFG_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_MABC_TC_CNFG_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_MABC_TC_CNFG_REG_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_MABC_TC_CNFG_REG,
                          value);
}

static INLINE void enh_8g_csu_reg_MABC_TC_CNFG_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_MABC_TC_CNFG_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_MABC_TC_CNFG_REG_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_MABC_TC_CNFG_REG,
                          mask,
                          PMC_ENH_8G_CSU_REG_MABC_TC_CNFG_REG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_MABC_TC_CNFG_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_MABC_TC_CNFG_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_MABC_TC_CNFG_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_MABC_TC_CNFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_MABC_TEST_CON_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_MABC_TEST_CON_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_MABC_TEST_CON_REG_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_MABC_TEST_CON_REG,
                          value);
}

static INLINE void enh_8g_csu_reg_MABC_TEST_CON_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_MABC_TEST_CON_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_MABC_TEST_CON_REG_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_MABC_TEST_CON_REG,
                          mask,
                          PMC_ENH_8G_CSU_REG_MABC_TEST_CON_REG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_MABC_TEST_CON_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_MABC_TEST_CON_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_MABC_TEST_CON_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_MABC_TEST_CON_REG_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_MABC_ENB_CON_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_MABC_ENB_CON_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_MABC_ENB_CON_REG_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_MABC_ENB_CON_REG,
                          value);
}

static INLINE void enh_8g_csu_reg_MABC_ENB_CON_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_MABC_ENB_CON_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_MABC_ENB_CON_REG_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_MABC_ENB_CON_REG,
                          mask,
                          PMC_ENH_8G_CSU_REG_MABC_ENB_CON_REG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_MABC_ENB_CON_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_MABC_ENB_CON_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_MABC_ENB_CON_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_MABC_ENB_CON_REG_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG,
                          value);
}

static INLINE void enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG,
                          mask,
                          PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG,
                          value);
}

static INLINE void enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG,
                          mask,
                          PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_MABC_CNTL_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_MABC_CNTL_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_MABC_CNTL_REG_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_MABC_CNTL_REG,
                          value);
}

static INLINE void enh_8g_csu_reg_MABC_CNTL_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_MABC_CNTL_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_MABC_CNTL_REG_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_MABC_CNTL_REG,
                          mask,
                          PMC_ENH_8G_CSU_REG_MABC_CNTL_REG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_MABC_CNTL_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_MABC_CNTL_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_MABC_CNTL_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_MABC_CNTL_REG_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_E_ADSM_CNFG_REG_1_write( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_E_ADSM_CNFG_REG_1_write( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_E_ADSM_CNFG_REG_1_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1,
                          value);
}

static INLINE void enh_8g_csu_reg_E_ADSM_CNFG_REG_1_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_E_ADSM_CNFG_REG_1_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_E_ADSM_CNFG_REG_1_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1,
                          mask,
                          PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_E_ADSM_CNFG_REG_1_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_E_ADSM_CNFG_REG_1_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_E_ADSM_CNFG_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_E_ADSM_CNFG_REG_2_write( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_E_ADSM_CNFG_REG_2_write( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_E_ADSM_CNFG_REG_2_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2,
                          value);
}

static INLINE void enh_8g_csu_reg_E_ADSM_CNFG_REG_2_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_E_ADSM_CNFG_REG_2_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_E_ADSM_CNFG_REG_2_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2,
                          mask,
                          PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_E_ADSM_CNFG_REG_2_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_E_ADSM_CNFG_REG_2_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_E_ADSM_CNFG_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_E_ADSM_CNFG_REG_3_write( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_E_ADSM_CNFG_REG_3_write( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_E_ADSM_CNFG_REG_3_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3,
                          value);
}

static INLINE void enh_8g_csu_reg_E_ADSM_CNFG_REG_3_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_E_ADSM_CNFG_REG_3_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_E_ADSM_CNFG_REG_3_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3,
                          mask,
                          PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_E_ADSM_CNFG_REG_3_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_E_ADSM_CNFG_REG_3_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_E_ADSM_CNFG_REG_3_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_E_ADSM_DENOMIN_LOW_CNFG_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_E_ADSM_DENOMIN_LOW_CNFG_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_E_ADSM_DENOMIN_LOW_CNFG_REG_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG,
                          value);
}

static INLINE void enh_8g_csu_reg_E_ADSM_DENOMIN_LOW_CNFG_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_E_ADSM_DENOMIN_LOW_CNFG_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_E_ADSM_DENOMIN_LOW_CNFG_REG_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG,
                          mask,
                          PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_E_ADSM_DENOMIN_LOW_CNFG_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_E_ADSM_DENOMIN_LOW_CNFG_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_E_ADSM_DENOMIN_LOW_CNFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_E_ADSM_DENOMIN_UP_CNFG_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_E_ADSM_DENOMIN_UP_CNFG_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_E_ADSM_DENOMIN_UP_CNFG_REG_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG,
                          value);
}

static INLINE void enh_8g_csu_reg_E_ADSM_DENOMIN_UP_CNFG_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_E_ADSM_DENOMIN_UP_CNFG_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_E_ADSM_DENOMIN_UP_CNFG_REG_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG,
                          mask,
                          PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_E_ADSM_DENOMIN_UP_CNFG_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_E_ADSM_DENOMIN_UP_CNFG_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_E_ADSM_DENOMIN_UP_CNFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG,
                          value);
}

static INLINE void enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG,
                          mask,
                          PMC_ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_OVR_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_OVR_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_OVR_REG_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG,
                          value);
}

static INLINE void enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_OVR_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_OVR_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_OVR_REG_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG,
                          mask,
                          PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_OVR_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_OVR_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_OVR_REG_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_UP_OVR_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_UP_OVR_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_UP_OVR_REG_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG,
                          value);
}

static INLINE void enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_UP_OVR_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_UP_OVR_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_UP_OVR_REG_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG,
                          mask,
                          PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_UP_OVR_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_UP_OVR_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_UP_OVR_REG_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_write( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_write( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1,
                          value);
}

static INLINE void enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1,
                          mask,
                          PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_write( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_write( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2,
                          value);
}

static INLINE void enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2,
                          mask,
                          PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_ACB_CNFG_REG_1_write( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_ACB_CNFG_REG_1_write( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_ACB_CNFG_REG_1_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_ACB_CNFG_REG_1,
                          value);
}

static INLINE void enh_8g_csu_reg_ACB_CNFG_REG_1_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_ACB_CNFG_REG_1_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_ACB_CNFG_REG_1_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_CONFIG,
                          PMC_ENH_8G_CSU_REG_ACB_CNFG_REG_1,
                          mask,
                          PMC_ENH_8G_CSU_REG_ACB_CNFG_REG_1_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_ACB_CNFG_REG_1_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_ACB_CNFG_REG_1_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_CONFIG,
                                     PMC_ENH_8G_CSU_REG_ACB_CNFG_REG_1);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_ACB_CNFG_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG,
                          value);
}

static INLINE void enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_action_on_write_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_action_on_write_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_action_on_write_field_set", mask, ofs, value );
    enh_8g_csu_action_on_write_field_set( b_ptr,
                                          h_ptr,
                                          MEM_TYPE_STATUS,
                                          PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG,
                                          mask,
                                          ofs,
                                          value);

}

static INLINE UINT32 enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return enh_8g_csu_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE void enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_write( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_write", value );
    enh_8g_csu_reg_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG,
                          value);
}

static INLINE void enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_field_set( enh_8g_csu_buffer_t *b_ptr,
                                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_field_set", mask, ofs, value );
    enh_8g_csu_field_set( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG,
                          mask,
                          PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_UNUSED_MASK,
                          ofs,
                          value);

}

static INLINE UINT32 enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return enh_8g_csu_poll( b_ptr,
                            h_ptr,
                            MEM_TYPE_STATUS,
                            PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG,
                            mask,
                            value,
                            cmp,
                            max_count,
                            num_failed_polls,
                            delay_between_polls_in_microseconds);

}


static INLINE UINT32 enh_8g_csu_reg_PLL_STA_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_PLL_STA_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_ENH_8G_CSU_REG_PLL_STA_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_PLL_STA_REG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 enh_8g_csu_reg_PLL_LOCK_DET_PPM_OFF_STA_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_PLL_LOCK_DET_PPM_OFF_STA_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_ENH_8G_CSU_REG_PLL_LOCK_DET_PPM_OFF_STA_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_PLL_LOCK_DET_PPM_OFF_STA_REG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 enh_8g_csu_reg_VCO_CAL_STA_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_VCO_CAL_STA_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_ENH_8G_CSU_REG_VCO_CAL_STA_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_VCO_CAL_STA_REG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 enh_8g_csu_reg_OCC_TXRX_STA_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_OCC_TXRX_STA_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_ENH_8G_CSU_REG_OCC_TXRX_STA_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_OCC_TXRX_STA_REG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 enh_8g_csu_reg_REF_DBLR_CALIB_STA_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_reg_REF_DBLR_CALIB_STA_REG_read( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = enh_8g_csu_reg_read( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_ENH_8G_CSU_REG_REF_DBLR_CALIB_STA_REG);

    IOLOG( "%s -> 0x%08x;", "enh_8g_csu_reg_REF_DBLR_CALIB_STA_REG_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void enh_8g_csu_field_ACB_RSTB_set( enh_8g_csu_buffer_t *b_ptr,
                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_ACB_RSTB_set( enh_8g_csu_buffer_t *b_ptr,
                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_ACB_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_ACB_RSTB_set", value );

    /* (0x00000400 bits 7) field ACB_RSTB of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_ACB_RSTB_MSK,
                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_ACB_RSTB_OFF,
                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_ACB_RSTB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_ACB_RSTB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000400 bits 7) field ACB_RSTB of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_ACB_RSTB_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_ACB_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_ACB_RSTB_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_PLL_LOCK_DET_SW_RST_set( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_PLL_LOCK_DET_SW_RST_set( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_PLL_LOCK_DET_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_PLL_LOCK_DET_SW_RST_set", value );

    /* (0x00000400 bits 6) field PLL_LOCK_DET_SW_RST of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_PLL_LOCK_DET_SW_RST_MSK,
                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_PLL_LOCK_DET_SW_RST_OFF,
                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_PLL_LOCK_DET_SW_RST_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_PLL_LOCK_DET_SW_RST_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000400 bits 6) field PLL_LOCK_DET_SW_RST of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_PLL_LOCK_DET_SW_RST_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_PLL_LOCK_DET_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_PLL_LOCK_DET_SW_RST_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CNI_UNI_CSU_GLB_RSTB_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CNI_UNI_CSU_GLB_RSTB_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CNI_UNI_CSU_GLB_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CNI_UNI_CSU_GLB_RSTB_set", value );

    /* (0x00000400 bits 5) field CNI_UNI_CSU_GLB_RSTB of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_CNI_UNI_CSU_GLB_RSTB_MSK,
                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_CNI_UNI_CSU_GLB_RSTB_OFF,
                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_CNI_UNI_CSU_GLB_RSTB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CNI_UNI_CSU_GLB_RSTB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000400 bits 5) field CNI_UNI_CSU_GLB_RSTB of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_CNI_UNI_CSU_GLB_RSTB_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_CNI_UNI_CSU_GLB_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CNI_UNI_CSU_GLB_RSTB_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_GCOC_RSTB_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_GCOC_RSTB_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_GCOC_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_GCOC_RSTB_set", value );

    /* (0x00000400 bits 4) field GCOC_RSTB of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_GCOC_RSTB_MSK,
                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_GCOC_RSTB_OFF,
                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_GCOC_RSTB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_GCOC_RSTB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000400 bits 4) field GCOC_RSTB of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_GCOC_RSTB_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_GCOC_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_GCOC_RSTB_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_EADSM_RSTB_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_EADSM_RSTB_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_EADSM_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_EADSM_RSTB_set", value );

    /* (0x00000400 bits 3) field EADSM_RSTB of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_EADSM_RSTB_MSK,
                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_EADSM_RSTB_OFF,
                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_EADSM_RSTB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_EADSM_RSTB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000400 bits 3) field EADSM_RSTB of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_EADSM_RSTB_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_EADSM_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_EADSM_RSTB_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_OCC_TXRX_RSTB_set( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_OCC_TXRX_RSTB_set( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_OCC_TXRX_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_OCC_TXRX_RSTB_set", value );

    /* (0x00000400 bits 2) field OCC_TXRX_RSTB of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_OCC_TXRX_RSTB_MSK,
                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_OCC_TXRX_RSTB_OFF,
                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_OCC_TXRX_RSTB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OCC_TXRX_RSTB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000400 bits 2) field OCC_TXRX_RSTB of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_OCC_TXRX_RSTB_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_OCC_TXRX_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OCC_TXRX_RSTB_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_VCO_CAL_RSTB_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_VCO_CAL_RSTB_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_VCO_CAL_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_VCO_CAL_RSTB_set", value );

    /* (0x00000400 bits 1) field VCO_CAL_RSTB of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_VCO_CAL_RSTB_MSK,
                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_VCO_CAL_RSTB_OFF,
                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_VCO_CAL_RSTB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCO_CAL_RSTB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000400 bits 1) field VCO_CAL_RSTB of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_VCO_CAL_RSTB_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_VCO_CAL_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCO_CAL_RSTB_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_PLL_LOCK_DET_RSTB_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_PLL_LOCK_DET_RSTB_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_PLL_LOCK_DET_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_PLL_LOCK_DET_RSTB_set", value );

    /* (0x00000400 bits 0) field PLL_LOCK_DET_RSTB of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_PLL_LOCK_DET_RSTB_MSK,
                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_PLL_LOCK_DET_RSTB_OFF,
                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_PLL_LOCK_DET_RSTB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_PLL_LOCK_DET_RSTB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000400 bits 0) field PLL_LOCK_DET_RSTB of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_PLL_LOCK_DET_RSTB_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_PLL_LOCK_DET_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_PLL_LOCK_DET_RSTB_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_ARSTB_set( enh_8g_csu_buffer_t *b_ptr,
                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_ARSTB_set( enh_8g_csu_buffer_t *b_ptr,
                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_ARSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_ARSTB_set", value );

    /* (0x00000404 bits 1) field ARSTB of register PMC_ENH_8G_CSU_REG_MABC_RST_CNFG_REG */
    enh_8g_csu_reg_MABC_RST_CNFG_REG_field_set( b_ptr,
                                                h_ptr,
                                                ENH_8G_CSU_REG_MABC_RST_CNFG_REG_BIT_ARSTB_MSK,
                                                ENH_8G_CSU_REG_MABC_RST_CNFG_REG_BIT_ARSTB_OFF,
                                                value);
}

static INLINE UINT32 enh_8g_csu_field_ARSTB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_ARSTB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000404 bits 1) field ARSTB of register PMC_ENH_8G_CSU_REG_MABC_RST_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_RST_CNFG_REG_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_RST_CNFG_REG_BIT_ARSTB_MSK) >> ENH_8G_CSU_REG_MABC_RST_CNFG_REG_BIT_ARSTB_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_ARSTB_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_RST_SYNC_set( enh_8g_csu_buffer_t *b_ptr,
                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_RST_SYNC_set( enh_8g_csu_buffer_t *b_ptr,
                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_RST_SYNC_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_RST_SYNC_set", value );

    /* (0x00000404 bits 0) field RST_SYNC of register PMC_ENH_8G_CSU_REG_MABC_RST_CNFG_REG */
    enh_8g_csu_reg_MABC_RST_CNFG_REG_field_set( b_ptr,
                                                h_ptr,
                                                ENH_8G_CSU_REG_MABC_RST_CNFG_REG_BIT_RST_SYNC_MSK,
                                                ENH_8G_CSU_REG_MABC_RST_CNFG_REG_BIT_RST_SYNC_OFF,
                                                value);
}

static INLINE UINT32 enh_8g_csu_field_RST_SYNC_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_RST_SYNC_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000404 bits 0) field RST_SYNC of register PMC_ENH_8G_CSU_REG_MABC_RST_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_RST_CNFG_REG_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_RST_CNFG_REG_BIT_RST_SYNC_MSK) >> ENH_8G_CSU_REG_MABC_RST_CNFG_REG_BIT_RST_SYNC_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_RST_SYNC_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_HRCLKDIV4OR5_EN_set( enh_8g_csu_buffer_t *b_ptr,
                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_HRCLKDIV4OR5_EN_set( enh_8g_csu_buffer_t *b_ptr,
                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_HRCLKDIV4OR5_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_HRCLKDIV4OR5_EN_set", value );

    /* (0x00000408 bits 1) field HRCLKDIV4OR5_EN of register PMC_ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG */
    enh_8g_csu_reg_CLK_DGLCH_CNFG_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG_BIT_HRCLKDIV4OR5_EN_MSK,
                                                 ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG_BIT_HRCLKDIV4OR5_EN_OFF,
                                                 value);
}

static INLINE UINT32 enh_8g_csu_field_HRCLKDIV4OR5_EN_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_HRCLKDIV4OR5_EN_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000408 bits 1) field HRCLKDIV4OR5_EN of register PMC_ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG */
    reg_value = enh_8g_csu_reg_CLK_DGLCH_CNFG_REG_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG_BIT_HRCLKDIV4OR5_EN_MSK) >> ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG_BIT_HRCLKDIV4OR5_EN_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_HRCLKDIV4OR5_EN_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_HRCLKDIV4_EN_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_HRCLKDIV4_EN_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_HRCLKDIV4_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_HRCLKDIV4_EN_set", value );

    /* (0x00000408 bits 0) field HRCLKDIV4_EN of register PMC_ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG */
    enh_8g_csu_reg_CLK_DGLCH_CNFG_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG_BIT_HRCLKDIV4_EN_MSK,
                                                 ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG_BIT_HRCLKDIV4_EN_OFF,
                                                 value);
}

static INLINE UINT32 enh_8g_csu_field_HRCLKDIV4_EN_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_HRCLKDIV4_EN_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000408 bits 0) field HRCLKDIV4_EN of register PMC_ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG */
    reg_value = enh_8g_csu_reg_CLK_DGLCH_CNFG_REG_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG_BIT_HRCLKDIV4_EN_MSK) >> ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG_BIT_HRCLKDIV4_EN_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_HRCLKDIV4_EN_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_RESET_HISTORY_set( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_RESET_HISTORY_set( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_RESET_HISTORY_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_RESET_HISTORY_set", value );

    /* (0x00000410 bits 4) field RESET_HISTORY of register PMC_ENH_8G_CSU_REG_PLL_CNFG_REG1 */
    enh_8g_csu_reg_PLL_CNFG_REG1_field_set( b_ptr,
                                            h_ptr,
                                            ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_RESET_HISTORY_MSK,
                                            ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_RESET_HISTORY_OFF,
                                            value);
}

static INLINE UINT32 enh_8g_csu_field_RESET_HISTORY_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_RESET_HISTORY_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000410 bits 4) field RESET_HISTORY of register PMC_ENH_8G_CSU_REG_PLL_CNFG_REG1 */
    reg_value = enh_8g_csu_reg_PLL_CNFG_REG1_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_RESET_HISTORY_MSK) >> ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_RESET_HISTORY_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_RESET_HISTORY_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_LOCK_PRECISION_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_LOCK_PRECISION_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_LOCK_PRECISION_set", value, 15);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_LOCK_PRECISION_set", value );

    /* (0x00000410 bits 3:0) field LOCK_PRECISION of register PMC_ENH_8G_CSU_REG_PLL_CNFG_REG1 */
    enh_8g_csu_reg_PLL_CNFG_REG1_field_set( b_ptr,
                                            h_ptr,
                                            ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_LOCK_PRECISION_MSK,
                                            ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_LOCK_PRECISION_OFF,
                                            value);
}

static INLINE UINT32 enh_8g_csu_field_LOCK_PRECISION_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_LOCK_PRECISION_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000410 bits 3:0) field LOCK_PRECISION of register PMC_ENH_8G_CSU_REG_PLL_CNFG_REG1 */
    reg_value = enh_8g_csu_reg_PLL_CNFG_REG1_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_LOCK_PRECISION_MSK) >> ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_LOCK_PRECISION_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_LOCK_PRECISION_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_LOCK_PRECISION_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_LOCK_PRECISION_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_LOCK_PRECISION_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_LOCK_PRECISION_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_LOCK_PRECISION_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000410 bits 3:0) field LOCK_PRECISION of register PMC_ENH_8G_CSU_REG_PLL_CNFG_REG1 */
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
        /* (0x00000410 bits 3:0) field LOCK_PRECISION of register PMC_ENH_8G_CSU_REG_PLL_CNFG_REG1 */
        enh_8g_csu_reg_PLL_CNFG_REG1_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_LOCK_PRECISION_OFF + subfield_offset),
                                                ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_LOCK_PRECISION_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_LOCK_PRECISION_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_LOCK_PRECISION_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_LOCK_PRECISION_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_LOCK_PRECISION_get", stop_bit, 3 );
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
    /* (0x00000410 bits 3:0) field LOCK_PRECISION of register PMC_ENH_8G_CSU_REG_PLL_CNFG_REG1 */
    reg_value = enh_8g_csu_reg_PLL_CNFG_REG1_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_LOCK_PRECISION_MSK)
                  >> ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_LOCK_PRECISION_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_LOCK_PRECISION_MSK, ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_LOCK_PRECISION_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_LOCK_PRECISION_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_LOCK_CNT_MAX_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_LOCK_CNT_MAX_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_LOCK_CNT_MAX_set", value, 255);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_LOCK_CNT_MAX_set", value );

    /* (0x00000414 bits 27:20) field LOCK_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CLK_CNTR_REG */
    enh_8g_csu_reg_PLL_CLK_CNTR_REG_field_set( b_ptr,
                                               h_ptr,
                                               ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_LOCK_CNT_MAX_MSK,
                                               ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_LOCK_CNT_MAX_OFF,
                                               value);
}

static INLINE UINT32 enh_8g_csu_field_LOCK_CNT_MAX_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_LOCK_CNT_MAX_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000414 bits 27:20) field LOCK_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CLK_CNTR_REG */
    reg_value = enh_8g_csu_reg_PLL_CLK_CNTR_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_LOCK_CNT_MAX_MSK) >> ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_LOCK_CNT_MAX_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_LOCK_CNT_MAX_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_LOCK_CNT_MAX_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_LOCK_CNT_MAX_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_LOCK_CNT_MAX_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_LOCK_CNT_MAX_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_LOCK_CNT_MAX_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000414 bits 27:20) field LOCK_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CLK_CNTR_REG */
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
        /* (0x00000414 bits 27:20) field LOCK_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CLK_CNTR_REG */
        enh_8g_csu_reg_PLL_CLK_CNTR_REG_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_LOCK_CNT_MAX_OFF + subfield_offset),
                                                   ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_LOCK_CNT_MAX_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_LOCK_CNT_MAX_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_LOCK_CNT_MAX_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_LOCK_CNT_MAX_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_LOCK_CNT_MAX_get", stop_bit, 7 );
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
    /* (0x00000414 bits 27:20) field LOCK_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CLK_CNTR_REG */
    reg_value = enh_8g_csu_reg_PLL_CLK_CNTR_REG_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_LOCK_CNT_MAX_MSK)
                  >> ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_LOCK_CNT_MAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_LOCK_CNT_MAX_MSK, ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_LOCK_CNT_MAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_LOCK_CNT_MAX_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_CLK_CNT_MAX_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CLK_CNT_MAX_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CLK_CNT_MAX_set", value, 1048575);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CLK_CNT_MAX_set", value );

    /* (0x00000414 bits 19:0) field CLK_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CLK_CNTR_REG */
    enh_8g_csu_reg_PLL_CLK_CNTR_REG_field_set( b_ptr,
                                               h_ptr,
                                               ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_CLK_CNT_MAX_MSK,
                                               ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_CLK_CNT_MAX_OFF,
                                               value);
}

static INLINE UINT32 enh_8g_csu_field_CLK_CNT_MAX_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CLK_CNT_MAX_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000414 bits 19:0) field CLK_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CLK_CNTR_REG */
    reg_value = enh_8g_csu_reg_PLL_CLK_CNTR_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_CLK_CNT_MAX_MSK) >> ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_CLK_CNT_MAX_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CLK_CNT_MAX_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_CLK_CNT_MAX_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_CLK_CNT_MAX_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CLK_CNT_MAX_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CLK_CNT_MAX_set", stop_bit, 19 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CLK_CNT_MAX_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000414 bits 19:0) field CLK_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CLK_CNTR_REG */
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
        if (stop_bit < 19) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 19;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000414 bits 19:0) field CLK_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CLK_CNTR_REG */
        enh_8g_csu_reg_PLL_CLK_CNTR_REG_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_CLK_CNT_MAX_OFF + subfield_offset),
                                                   ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_CLK_CNT_MAX_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_CLK_CNT_MAX_get( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_CLK_CNT_MAX_get( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CLK_CNT_MAX_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CLK_CNT_MAX_get", stop_bit, 19 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 19) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 19;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000414 bits 19:0) field CLK_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CLK_CNTR_REG */
    reg_value = enh_8g_csu_reg_PLL_CLK_CNTR_REG_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_CLK_CNT_MAX_MSK)
                  >> ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_CLK_CNT_MAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_CLK_CNT_MAX_MSK, ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_CLK_CNT_MAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CLK_CNT_MAX_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_FAIL_CNT_MAX_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_FAIL_CNT_MAX_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_FAIL_CNT_MAX_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_FAIL_CNT_MAX_set", value );

    /* (0x00000418 bits 31:16) field FAIL_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CNTR_REG */
    enh_8g_csu_reg_PLL_CNTR_REG_field_set( b_ptr,
                                           h_ptr,
                                           ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_FAIL_CNT_MAX_MSK,
                                           ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_FAIL_CNT_MAX_OFF,
                                           value);
}

static INLINE UINT32 enh_8g_csu_field_FAIL_CNT_MAX_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_FAIL_CNT_MAX_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000418 bits 31:16) field FAIL_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CNTR_REG */
    reg_value = enh_8g_csu_reg_PLL_CNTR_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_FAIL_CNT_MAX_MSK) >> ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_FAIL_CNT_MAX_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_FAIL_CNT_MAX_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_FAIL_CNT_MAX_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_FAIL_CNT_MAX_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_FAIL_CNT_MAX_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_FAIL_CNT_MAX_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_FAIL_CNT_MAX_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000418 bits 31:16) field FAIL_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CNTR_REG */
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
        /* (0x00000418 bits 31:16) field FAIL_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CNTR_REG */
        enh_8g_csu_reg_PLL_CNTR_REG_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_FAIL_CNT_MAX_OFF + subfield_offset),
                                               ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_FAIL_CNT_MAX_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_FAIL_CNT_MAX_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_FAIL_CNT_MAX_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_FAIL_CNT_MAX_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_FAIL_CNT_MAX_get", stop_bit, 15 );
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
    /* (0x00000418 bits 31:16) field FAIL_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CNTR_REG */
    reg_value = enh_8g_csu_reg_PLL_CNTR_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_FAIL_CNT_MAX_MSK)
                  >> ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_FAIL_CNT_MAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_FAIL_CNT_MAX_MSK, ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_FAIL_CNT_MAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_FAIL_CNT_MAX_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_PASS_CNT_MAX_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_PASS_CNT_MAX_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_PASS_CNT_MAX_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_PASS_CNT_MAX_set", value );

    /* (0x00000418 bits 15:0) field PASS_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CNTR_REG */
    enh_8g_csu_reg_PLL_CNTR_REG_field_set( b_ptr,
                                           h_ptr,
                                           ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_PASS_CNT_MAX_MSK,
                                           ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_PASS_CNT_MAX_OFF,
                                           value);
}

static INLINE UINT32 enh_8g_csu_field_PASS_CNT_MAX_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_PASS_CNT_MAX_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000418 bits 15:0) field PASS_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CNTR_REG */
    reg_value = enh_8g_csu_reg_PLL_CNTR_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_PASS_CNT_MAX_MSK) >> ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_PASS_CNT_MAX_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_PASS_CNT_MAX_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_PASS_CNT_MAX_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_PASS_CNT_MAX_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_PASS_CNT_MAX_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_PASS_CNT_MAX_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_PASS_CNT_MAX_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000418 bits 15:0) field PASS_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CNTR_REG */
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
        /* (0x00000418 bits 15:0) field PASS_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CNTR_REG */
        enh_8g_csu_reg_PLL_CNTR_REG_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_PASS_CNT_MAX_OFF + subfield_offset),
                                               ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_PASS_CNT_MAX_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_PASS_CNT_MAX_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_PASS_CNT_MAX_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_PASS_CNT_MAX_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_PASS_CNT_MAX_get", stop_bit, 15 );
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
    /* (0x00000418 bits 15:0) field PASS_CNT_MAX of register PMC_ENH_8G_CSU_REG_PLL_CNTR_REG */
    reg_value = enh_8g_csu_reg_PLL_CNTR_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_PASS_CNT_MAX_MSK)
                  >> ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_PASS_CNT_MAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_PASS_CNT_MAX_MSK, ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_PASS_CNT_MAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_PASS_CNT_MAX_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_VCOC_MAX_THRESHOLD_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_VCOC_MAX_THRESHOLD_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_VCOC_MAX_THRESHOLD_set", value, 63);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_VCOC_MAX_THRESHOLD_set", value );

    /* (0x00000428 bits 28:23) field VCOC_MAX_THRESHOLD of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_field_set( b_ptr,
                                                 h_ptr,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MAX_THRESHOLD_MSK,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MAX_THRESHOLD_OFF,
                                                 value);
}

static INLINE UINT32 enh_8g_csu_field_VCOC_MAX_THRESHOLD_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCOC_MAX_THRESHOLD_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000428 bits 28:23) field VCOC_MAX_THRESHOLD of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MAX_THRESHOLD_MSK) >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MAX_THRESHOLD_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCOC_MAX_THRESHOLD_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_VCOC_MAX_THRESHOLD_set( enh_8g_csu_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_VCOC_MAX_THRESHOLD_set( enh_8g_csu_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_VCOC_MAX_THRESHOLD_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_VCOC_MAX_THRESHOLD_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_VCOC_MAX_THRESHOLD_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000428 bits 28:23) field VCOC_MAX_THRESHOLD of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
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
        /* (0x00000428 bits 28:23) field VCOC_MAX_THRESHOLD of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
        enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MAX_THRESHOLD_OFF + subfield_offset),
                                                     ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MAX_THRESHOLD_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_VCOC_MAX_THRESHOLD_get( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_VCOC_MAX_THRESHOLD_get( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_VCOC_MAX_THRESHOLD_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_VCOC_MAX_THRESHOLD_get", stop_bit, 5 );
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
    /* (0x00000428 bits 28:23) field VCOC_MAX_THRESHOLD of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MAX_THRESHOLD_MSK)
                  >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MAX_THRESHOLD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MAX_THRESHOLD_MSK, ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MAX_THRESHOLD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_VCOC_MAX_THRESHOLD_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_VCOC_MIN_THRESHOLD_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_VCOC_MIN_THRESHOLD_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_VCOC_MIN_THRESHOLD_set", value, 63);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_VCOC_MIN_THRESHOLD_set", value );

    /* (0x00000428 bits 22:17) field VCOC_MIN_THRESHOLD of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_field_set( b_ptr,
                                                 h_ptr,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MIN_THRESHOLD_MSK,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MIN_THRESHOLD_OFF,
                                                 value);
}

static INLINE UINT32 enh_8g_csu_field_VCOC_MIN_THRESHOLD_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCOC_MIN_THRESHOLD_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000428 bits 22:17) field VCOC_MIN_THRESHOLD of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MIN_THRESHOLD_MSK) >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MIN_THRESHOLD_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCOC_MIN_THRESHOLD_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_VCOC_MIN_THRESHOLD_set( enh_8g_csu_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_VCOC_MIN_THRESHOLD_set( enh_8g_csu_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_VCOC_MIN_THRESHOLD_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_VCOC_MIN_THRESHOLD_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_VCOC_MIN_THRESHOLD_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000428 bits 22:17) field VCOC_MIN_THRESHOLD of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
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
        /* (0x00000428 bits 22:17) field VCOC_MIN_THRESHOLD of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
        enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MIN_THRESHOLD_OFF + subfield_offset),
                                                     ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MIN_THRESHOLD_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_VCOC_MIN_THRESHOLD_get( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_VCOC_MIN_THRESHOLD_get( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_VCOC_MIN_THRESHOLD_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_VCOC_MIN_THRESHOLD_get", stop_bit, 5 );
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
    /* (0x00000428 bits 22:17) field VCOC_MIN_THRESHOLD of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MIN_THRESHOLD_MSK)
                  >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MIN_THRESHOLD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MIN_THRESHOLD_MSK, ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MIN_THRESHOLD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_VCOC_MIN_THRESHOLD_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_PPM_OFFSET_POLARITY_INV_set( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_PPM_OFFSET_POLARITY_INV_set( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_PPM_OFFSET_POLARITY_INV_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_PPM_OFFSET_POLARITY_INV_set", value );

    /* (0x00000428 bits 16) field PPM_OFFSET_POLARITY_INV of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_field_set( b_ptr,
                                                 h_ptr,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_PPM_OFFSET_POLARITY_INV_MSK,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_PPM_OFFSET_POLARITY_INV_OFF,
                                                 value);
}

static INLINE UINT32 enh_8g_csu_field_PPM_OFFSET_POLARITY_INV_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_PPM_OFFSET_POLARITY_INV_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000428 bits 16) field PPM_OFFSET_POLARITY_INV of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_PPM_OFFSET_POLARITY_INV_MSK) >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_PPM_OFFSET_POLARITY_INV_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_PPM_OFFSET_POLARITY_INV_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_LOAD_COUNT_OVR_EN_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_LOAD_COUNT_OVR_EN_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_LOAD_COUNT_OVR_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_LOAD_COUNT_OVR_EN_set", value );

    /* (0x00000428 bits 15) field LOAD_COUNT_OVR_EN of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_field_set( b_ptr,
                                                 h_ptr,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_LOAD_COUNT_OVR_EN_MSK,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_LOAD_COUNT_OVR_EN_OFF,
                                                 value);
}

static INLINE UINT32 enh_8g_csu_field_LOAD_COUNT_OVR_EN_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_LOAD_COUNT_OVR_EN_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000428 bits 15) field LOAD_COUNT_OVR_EN of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_LOAD_COUNT_OVR_EN_MSK) >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_LOAD_COUNT_OVR_EN_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_LOAD_COUNT_OVR_EN_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_LOAD_COUNT_OVR_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_LOAD_COUNT_OVR_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_LOAD_COUNT_OVR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_LOAD_COUNT_OVR_set", value );

    /* (0x00000428 bits 14) field LOAD_COUNT_OVR of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_field_set( b_ptr,
                                                 h_ptr,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_LOAD_COUNT_OVR_MSK,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_LOAD_COUNT_OVR_OFF,
                                                 value);
}

static INLINE UINT32 enh_8g_csu_field_LOAD_COUNT_OVR_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_LOAD_COUNT_OVR_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000428 bits 14) field LOAD_COUNT_OVR of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_LOAD_COUNT_OVR_MSK) >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_LOAD_COUNT_OVR_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_LOAD_COUNT_OVR_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_MIN_MODE_set( enh_8g_csu_buffer_t *b_ptr,
                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_MIN_MODE_set( enh_8g_csu_buffer_t *b_ptr,
                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_MIN_MODE_set", value, 63);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_MIN_MODE_set", value );

    /* (0x00000428 bits 13:8) field MIN_MODE of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_field_set( b_ptr,
                                                 h_ptr,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MIN_MODE_MSK,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MIN_MODE_OFF,
                                                 value);
}

static INLINE UINT32 enh_8g_csu_field_MIN_MODE_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_MIN_MODE_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000428 bits 13:8) field MIN_MODE of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MIN_MODE_MSK) >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MIN_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_MIN_MODE_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_MIN_MODE_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_MIN_MODE_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_MIN_MODE_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_MIN_MODE_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_MIN_MODE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000428 bits 13:8) field MIN_MODE of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
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
        /* (0x00000428 bits 13:8) field MIN_MODE of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
        enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MIN_MODE_OFF + subfield_offset),
                                                     ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MIN_MODE_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_MIN_MODE_get( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_MIN_MODE_get( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_MIN_MODE_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_MIN_MODE_get", stop_bit, 5 );
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
    /* (0x00000428 bits 13:8) field MIN_MODE of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MIN_MODE_MSK)
                  >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MIN_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MIN_MODE_MSK, ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MIN_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_MIN_MODE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_MAX_MODE_set( enh_8g_csu_buffer_t *b_ptr,
                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_MAX_MODE_set( enh_8g_csu_buffer_t *b_ptr,
                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_MAX_MODE_set", value, 63);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_MAX_MODE_set", value );

    /* (0x00000428 bits 7:2) field MAX_MODE of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_field_set( b_ptr,
                                                 h_ptr,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MAX_MODE_MSK,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MAX_MODE_OFF,
                                                 value);
}

static INLINE UINT32 enh_8g_csu_field_MAX_MODE_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_MAX_MODE_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000428 bits 7:2) field MAX_MODE of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MAX_MODE_MSK) >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MAX_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_MAX_MODE_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_MAX_MODE_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_MAX_MODE_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_MAX_MODE_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_MAX_MODE_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_MAX_MODE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000428 bits 7:2) field MAX_MODE of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
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
        /* (0x00000428 bits 7:2) field MAX_MODE of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
        enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MAX_MODE_OFF + subfield_offset),
                                                     ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MAX_MODE_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_MAX_MODE_get( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_MAX_MODE_get( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_MAX_MODE_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_MAX_MODE_get", stop_bit, 5 );
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
    /* (0x00000428 bits 7:2) field MAX_MODE of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MAX_MODE_MSK)
                  >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MAX_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MAX_MODE_MSK, ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MAX_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_MAX_MODE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_VCOC_START_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_VCOC_START_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_VCOC_START_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_VCOC_START_set", value );

    /* (0x00000428 bits 1) field VCOC_START of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_field_set( b_ptr,
                                                 h_ptr,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_START_MSK,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_START_OFF,
                                                 value);
}

static INLINE UINT32 enh_8g_csu_field_VCOC_START_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCOC_START_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000428 bits 1) field VCOC_START of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_START_MSK) >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_START_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCOC_START_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_VCOC_EN_set( enh_8g_csu_buffer_t *b_ptr,
                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_VCOC_EN_set( enh_8g_csu_buffer_t *b_ptr,
                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_VCOC_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_VCOC_EN_set", value );

    /* (0x00000428 bits 0) field VCOC_EN of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_field_set( b_ptr,
                                                 h_ptr,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_EN_MSK,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_EN_OFF,
                                                 value);
}

static INLINE UINT32 enh_8g_csu_field_VCOC_EN_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCOC_EN_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000428 bits 0) field VCOC_EN of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_EN_MSK) >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_EN_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCOC_EN_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_VCO_SETTLING_TIME_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_VCO_SETTLING_TIME_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_VCO_SETTLING_TIME_set", value, 255);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_VCO_SETTLING_TIME_set", value );

    /* (0x0000042c bits 23:16) field VCO_SETTLING_TIME of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2 */
    enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_field_set( b_ptr,
                                                 h_ptr,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCO_SETTLING_TIME_MSK,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCO_SETTLING_TIME_OFF,
                                                 value);
}

static INLINE UINT32 enh_8g_csu_field_VCO_SETTLING_TIME_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCO_SETTLING_TIME_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000042c bits 23:16) field VCO_SETTLING_TIME of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCO_SETTLING_TIME_MSK) >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCO_SETTLING_TIME_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCO_SETTLING_TIME_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_VCO_SETTLING_TIME_set( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_VCO_SETTLING_TIME_set( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_VCO_SETTLING_TIME_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_VCO_SETTLING_TIME_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_VCO_SETTLING_TIME_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000042c bits 23:16) field VCO_SETTLING_TIME of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2 */
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
        /* (0x0000042c bits 23:16) field VCO_SETTLING_TIME of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2 */
        enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCO_SETTLING_TIME_OFF + subfield_offset),
                                                     ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCO_SETTLING_TIME_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_VCO_SETTLING_TIME_get( enh_8g_csu_buffer_t *b_ptr,
                                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_VCO_SETTLING_TIME_get( enh_8g_csu_buffer_t *b_ptr,
                                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_VCO_SETTLING_TIME_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_VCO_SETTLING_TIME_get", stop_bit, 7 );
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
    /* (0x0000042c bits 23:16) field VCO_SETTLING_TIME of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCO_SETTLING_TIME_MSK)
                  >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCO_SETTLING_TIME_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCO_SETTLING_TIME_MSK, ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCO_SETTLING_TIME_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_VCO_SETTLING_TIME_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_VCOC_LOOP_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_VCOC_LOOP_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_VCOC_LOOP_set", value, 15);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_VCOC_LOOP_set", value );

    /* (0x0000042c bits 11:8) field VCOC_LOOP of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2 */
    enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_field_set( b_ptr,
                                                 h_ptr,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCOC_LOOP_MSK,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCOC_LOOP_OFF,
                                                 value);
}

static INLINE UINT32 enh_8g_csu_field_VCOC_LOOP_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCOC_LOOP_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000042c bits 11:8) field VCOC_LOOP of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCOC_LOOP_MSK) >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCOC_LOOP_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCOC_LOOP_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_VCOC_LOOP_set( enh_8g_csu_buffer_t *b_ptr,
                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_VCOC_LOOP_set( enh_8g_csu_buffer_t *b_ptr,
                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_VCOC_LOOP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_VCOC_LOOP_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_VCOC_LOOP_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000042c bits 11:8) field VCOC_LOOP of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2 */
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
        /* (0x0000042c bits 11:8) field VCOC_LOOP of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2 */
        enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCOC_LOOP_OFF + subfield_offset),
                                                     ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCOC_LOOP_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_VCOC_LOOP_get( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_VCOC_LOOP_get( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_VCOC_LOOP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_VCOC_LOOP_get", stop_bit, 3 );
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
    /* (0x0000042c bits 11:8) field VCOC_LOOP of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCOC_LOOP_MSK)
                  >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCOC_LOOP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCOC_LOOP_MSK, ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCOC_LOOP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_VCOC_LOOP_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_OFF_EQ_WINDOW_set( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_OFF_EQ_WINDOW_set( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_OFF_EQ_WINDOW_set", value, 63);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_OFF_EQ_WINDOW_set", value );

    /* (0x0000042c bits 6:1) field OFF_EQ_WINDOW of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2 */
    enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_field_set( b_ptr,
                                                 h_ptr,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_OFF_EQ_WINDOW_MSK,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_OFF_EQ_WINDOW_OFF,
                                                 value);
}

static INLINE UINT32 enh_8g_csu_field_OFF_EQ_WINDOW_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OFF_EQ_WINDOW_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000042c bits 6:1) field OFF_EQ_WINDOW of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_OFF_EQ_WINDOW_MSK) >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_OFF_EQ_WINDOW_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OFF_EQ_WINDOW_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_OFF_EQ_WINDOW_set( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_OFF_EQ_WINDOW_set( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_OFF_EQ_WINDOW_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_OFF_EQ_WINDOW_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_OFF_EQ_WINDOW_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000042c bits 6:1) field OFF_EQ_WINDOW of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2 */
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
        /* (0x0000042c bits 6:1) field OFF_EQ_WINDOW of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2 */
        enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_OFF_EQ_WINDOW_OFF + subfield_offset),
                                                     ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_OFF_EQ_WINDOW_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_OFF_EQ_WINDOW_get( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_OFF_EQ_WINDOW_get( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_OFF_EQ_WINDOW_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_OFF_EQ_WINDOW_get", stop_bit, 5 );
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
    /* (0x0000042c bits 6:1) field OFF_EQ_WINDOW of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_OFF_EQ_WINDOW_MSK)
                  >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_OFF_EQ_WINDOW_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_OFF_EQ_WINDOW_MSK, ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_OFF_EQ_WINDOW_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_OFF_EQ_WINDOW_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_VCOC_BIST_EN_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_VCOC_BIST_EN_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_VCOC_BIST_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_VCOC_BIST_EN_set", value );

    /* (0x0000042c bits 0) field VCOC_BIST_EN of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2 */
    enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_field_set( b_ptr,
                                                 h_ptr,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCOC_BIST_EN_MSK,
                                                 ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCOC_BIST_EN_OFF,
                                                 value);
}

static INLINE UINT32 enh_8g_csu_field_VCOC_BIST_EN_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCOC_BIST_EN_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000042c bits 0) field VCOC_BIST_EN of register PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2 */
    reg_value = enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCOC_BIST_EN_MSK) >> ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCOC_BIST_EN_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCOC_BIST_EN_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_OCC_START_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_OCC_START_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_OCC_START_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_OCC_START_set", value );

    /* (0x00000438 bits 21) field OCC_START of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1 */
    enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_field_set( b_ptr,
                                                  h_ptr,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_START_MSK,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_START_OFF,
                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_OCC_START_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OCC_START_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 21) field OCC_START of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_START_MSK) >> ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_START_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OCC_START_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_OCC_CONT_set( enh_8g_csu_buffer_t *b_ptr,
                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_OCC_CONT_set( enh_8g_csu_buffer_t *b_ptr,
                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_OCC_CONT_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_OCC_CONT_set", value );

    /* (0x00000438 bits 20) field OCC_CONT of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1 */
    enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_field_set( b_ptr,
                                                  h_ptr,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_CONT_MSK,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_CONT_OFF,
                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_OCC_CONT_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OCC_CONT_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 20) field OCC_CONT of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_CONT_MSK) >> ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_CONT_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OCC_CONT_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CLR_OCDAC_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CLR_OCDAC_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CLR_OCDAC_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CLR_OCDAC_set", value );

    /* (0x00000438 bits 19) field CLR_OCDAC of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1 */
    enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_field_set( b_ptr,
                                                  h_ptr,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_CLR_OCDAC_MSK,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_CLR_OCDAC_OFF,
                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_CLR_OCDAC_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CLR_OCDAC_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 19) field CLR_OCDAC of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_CLR_OCDAC_MSK) >> ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_CLR_OCDAC_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CLR_OCDAC_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_OCC_ENABLE_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_OCC_ENABLE_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_OCC_ENABLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_OCC_ENABLE_set", value );

    /* (0x00000438 bits 18) field OCC_ENABLE of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1 */
    enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_field_set( b_ptr,
                                                  h_ptr,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_ENABLE_MSK,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_ENABLE_OFF,
                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_OCC_ENABLE_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OCC_ENABLE_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 18) field OCC_ENABLE of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_ENABLE_MSK) >> ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_ENABLE_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OCC_ENABLE_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_OC_OVERRIDE_ENB_set( enh_8g_csu_buffer_t *b_ptr,
                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_OC_OVERRIDE_ENB_set( enh_8g_csu_buffer_t *b_ptr,
                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_OC_OVERRIDE_ENB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_OC_OVERRIDE_ENB_set", value );

    /* (0x00000438 bits 17) field OC_OVERRIDE_ENB of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1 */
    enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_field_set( b_ptr,
                                                  h_ptr,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OC_OVERRIDE_ENB_MSK,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OC_OVERRIDE_ENB_OFF,
                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_OC_OVERRIDE_ENB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OC_OVERRIDE_ENB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 17) field OC_OVERRIDE_ENB of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OC_OVERRIDE_ENB_MSK) >> ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OC_OVERRIDE_ENB_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OC_OVERRIDE_ENB_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_OC_INVERT_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_OC_INVERT_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_OC_INVERT_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_OC_INVERT_set", value );

    /* (0x00000438 bits 16) field OC_INVERT of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1 */
    enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_field_set( b_ptr,
                                                  h_ptr,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OC_INVERT_MSK,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OC_INVERT_OFF,
                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_OC_INVERT_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OC_INVERT_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 16) field OC_INVERT of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OC_INVERT_MSK) >> ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OC_INVERT_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OC_INVERT_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_OCC_DONE_GEN_SEL_set( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_OCC_DONE_GEN_SEL_set( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_OCC_DONE_GEN_SEL_set", value, 3);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_OCC_DONE_GEN_SEL_set", value );

    /* (0x00000438 bits 15:14) field OCC_DONE_GEN_SEL of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1 */
    enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_field_set( b_ptr,
                                                  h_ptr,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_DONE_GEN_SEL_MSK,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_DONE_GEN_SEL_OFF,
                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_OCC_DONE_GEN_SEL_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OCC_DONE_GEN_SEL_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000438 bits 15:14) field OCC_DONE_GEN_SEL of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_DONE_GEN_SEL_MSK) >> ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_DONE_GEN_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OCC_DONE_GEN_SEL_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_OCC_DONE_GEN_SEL_set( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_OCC_DONE_GEN_SEL_set( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_OCC_DONE_GEN_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_OCC_DONE_GEN_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_OCC_DONE_GEN_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000438 bits 15:14) field OCC_DONE_GEN_SEL of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1 */
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
        /* (0x00000438 bits 15:14) field OCC_DONE_GEN_SEL of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1 */
        enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_DONE_GEN_SEL_OFF + subfield_offset),
                                                      ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_DONE_GEN_SEL_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_OCC_DONE_GEN_SEL_get( enh_8g_csu_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_OCC_DONE_GEN_SEL_get( enh_8g_csu_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_OCC_DONE_GEN_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_OCC_DONE_GEN_SEL_get", stop_bit, 1 );
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
    /* (0x00000438 bits 15:14) field OCC_DONE_GEN_SEL of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_DONE_GEN_SEL_MSK)
                  >> ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_DONE_GEN_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_DONE_GEN_SEL_MSK, ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_DONE_GEN_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_OCC_DONE_GEN_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_OC_OVERRIDE_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_OC_OVERRIDE_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_OC_OVERRIDE_set", value, 31);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_OC_OVERRIDE_set", value );

    /* (0x00000440 bits 24:20) field OC_OVERRIDE of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2 */
    enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_field_set( b_ptr,
                                                  h_ptr,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_OVERRIDE_MSK,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_OVERRIDE_OFF,
                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_OC_OVERRIDE_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OC_OVERRIDE_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000440 bits 24:20) field OC_OVERRIDE of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2 */
    reg_value = enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_OVERRIDE_MSK) >> ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_OVERRIDE_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OC_OVERRIDE_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_OC_OVERRIDE_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_OC_OVERRIDE_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_OC_OVERRIDE_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_OC_OVERRIDE_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_OC_OVERRIDE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000440 bits 24:20) field OC_OVERRIDE of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2 */
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
        /* (0x00000440 bits 24:20) field OC_OVERRIDE of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2 */
        enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_OVERRIDE_OFF + subfield_offset),
                                                      ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_OVERRIDE_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_OC_OVERRIDE_get( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_OC_OVERRIDE_get( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_OC_OVERRIDE_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_OC_OVERRIDE_get", stop_bit, 4 );
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
    /* (0x00000440 bits 24:20) field OC_OVERRIDE of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2 */
    reg_value = enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_OVERRIDE_MSK)
                  >> ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_OVERRIDE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_OVERRIDE_MSK, ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_OVERRIDE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_OC_OVERRIDE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_OC_TOLERANCE_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_OC_TOLERANCE_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 32767)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_OC_TOLERANCE_set", value, 32767);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_OC_TOLERANCE_set", value );

    /* (0x00000440 bits 14:0) field OC_TOLERANCE of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2 */
    enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_field_set( b_ptr,
                                                  h_ptr,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_TOLERANCE_MSK,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_TOLERANCE_OFF,
                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_OC_TOLERANCE_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OC_TOLERANCE_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000440 bits 14:0) field OC_TOLERANCE of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2 */
    reg_value = enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_TOLERANCE_MSK) >> ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_TOLERANCE_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OC_TOLERANCE_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_OC_TOLERANCE_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_OC_TOLERANCE_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_OC_TOLERANCE_set", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_OC_TOLERANCE_set", stop_bit, 14 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_OC_TOLERANCE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000440 bits 14:0) field OC_TOLERANCE of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2 */
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
        if (stop_bit < 14) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 14;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000440 bits 14:0) field OC_TOLERANCE of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2 */
        enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_TOLERANCE_OFF + subfield_offset),
                                                      ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_TOLERANCE_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_OC_TOLERANCE_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_OC_TOLERANCE_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_OC_TOLERANCE_get", stop_bit, start_bit );
    if (stop_bit > 14) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_OC_TOLERANCE_get", stop_bit, 14 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 14) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 14;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000440 bits 14:0) field OC_TOLERANCE of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2 */
    reg_value = enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_TOLERANCE_MSK)
                  >> ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_TOLERANCE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_TOLERANCE_MSK, ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_TOLERANCE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_OC_TOLERANCE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_OC_THRESHOLD_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_OC_THRESHOLD_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_OC_THRESHOLD_set", value, 15);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_OC_THRESHOLD_set", value );

    /* (0x00000444 bits 19:16) field OC_THRESHOLD of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3 */
    enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_field_set( b_ptr,
                                                  h_ptr,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_THRESHOLD_MSK,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_THRESHOLD_OFF,
                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_OC_THRESHOLD_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OC_THRESHOLD_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000444 bits 19:16) field OC_THRESHOLD of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3 */
    reg_value = enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_THRESHOLD_MSK) >> ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_THRESHOLD_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OC_THRESHOLD_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_OC_THRESHOLD_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_OC_THRESHOLD_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_OC_THRESHOLD_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_OC_THRESHOLD_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_OC_THRESHOLD_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000444 bits 19:16) field OC_THRESHOLD of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3 */
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
        /* (0x00000444 bits 19:16) field OC_THRESHOLD of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3 */
        enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_THRESHOLD_OFF + subfield_offset),
                                                      ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_THRESHOLD_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_OC_THRESHOLD_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_OC_THRESHOLD_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_OC_THRESHOLD_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_OC_THRESHOLD_get", stop_bit, 3 );
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
    /* (0x00000444 bits 19:16) field OC_THRESHOLD of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3 */
    reg_value = enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_THRESHOLD_MSK)
                  >> ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_THRESHOLD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_THRESHOLD_MSK, ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_THRESHOLD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_OC_THRESHOLD_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_OC_INTERVAL_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_OC_INTERVAL_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_OC_INTERVAL_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_OC_INTERVAL_set", value );

    /* (0x00000444 bits 15:0) field OC_INTERVAL of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3 */
    enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_field_set( b_ptr,
                                                  h_ptr,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_INTERVAL_MSK,
                                                  ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_INTERVAL_OFF,
                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_OC_INTERVAL_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OC_INTERVAL_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000444 bits 15:0) field OC_INTERVAL of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3 */
    reg_value = enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_INTERVAL_MSK) >> ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_INTERVAL_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OC_INTERVAL_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_OC_INTERVAL_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_OC_INTERVAL_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_OC_INTERVAL_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_OC_INTERVAL_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_OC_INTERVAL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000444 bits 15:0) field OC_INTERVAL of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3 */
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
        /* (0x00000444 bits 15:0) field OC_INTERVAL of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3 */
        enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_INTERVAL_OFF + subfield_offset),
                                                      ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_INTERVAL_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_OC_INTERVAL_get( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_OC_INTERVAL_get( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_OC_INTERVAL_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_OC_INTERVAL_get", stop_bit, 15 );
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
    /* (0x00000444 bits 15:0) field OC_INTERVAL of register PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3 */
    reg_value = enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_INTERVAL_MSK)
                  >> ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_INTERVAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_INTERVAL_MSK, ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_INTERVAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_OC_INTERVAL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_DIV_CLK_FACT_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_DIV_CLK_FACT_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_DIV_CLK_FACT_set", value, 15);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_DIV_CLK_FACT_set", value );

    /* (0x00000454 bits 3:0) field DIV_CLK_FACT of register PMC_ENH_8G_CSU_REG_CLK_DIV_CNFG_REG */
    enh_8g_csu_reg_CLK_DIV_CNFG_REG_field_set( b_ptr,
                                               h_ptr,
                                               ENH_8G_CSU_REG_CLK_DIV_CNFG_REG_BIT_DIV_CLK_FACT_MSK,
                                               ENH_8G_CSU_REG_CLK_DIV_CNFG_REG_BIT_DIV_CLK_FACT_OFF,
                                               value);
}

static INLINE UINT32 enh_8g_csu_field_DIV_CLK_FACT_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_DIV_CLK_FACT_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000454 bits 3:0) field DIV_CLK_FACT of register PMC_ENH_8G_CSU_REG_CLK_DIV_CNFG_REG */
    reg_value = enh_8g_csu_reg_CLK_DIV_CNFG_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CLK_DIV_CNFG_REG_BIT_DIV_CLK_FACT_MSK) >> ENH_8G_CSU_REG_CLK_DIV_CNFG_REG_BIT_DIV_CLK_FACT_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_DIV_CLK_FACT_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_DIV_CLK_FACT_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_DIV_CLK_FACT_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_DIV_CLK_FACT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_DIV_CLK_FACT_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_DIV_CLK_FACT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000454 bits 3:0) field DIV_CLK_FACT of register PMC_ENH_8G_CSU_REG_CLK_DIV_CNFG_REG */
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
        /* (0x00000454 bits 3:0) field DIV_CLK_FACT of register PMC_ENH_8G_CSU_REG_CLK_DIV_CNFG_REG */
        enh_8g_csu_reg_CLK_DIV_CNFG_REG_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (ENH_8G_CSU_REG_CLK_DIV_CNFG_REG_BIT_DIV_CLK_FACT_OFF + subfield_offset),
                                                   ENH_8G_CSU_REG_CLK_DIV_CNFG_REG_BIT_DIV_CLK_FACT_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_DIV_CLK_FACT_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_DIV_CLK_FACT_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_DIV_CLK_FACT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_DIV_CLK_FACT_get", stop_bit, 3 );
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
    /* (0x00000454 bits 3:0) field DIV_CLK_FACT of register PMC_ENH_8G_CSU_REG_CLK_DIV_CNFG_REG */
    reg_value = enh_8g_csu_reg_CLK_DIV_CNFG_REG_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_CLK_DIV_CNFG_REG_BIT_DIV_CLK_FACT_MSK)
                  >> ENH_8G_CSU_REG_CLK_DIV_CNFG_REG_BIT_DIV_CLK_FACT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_CLK_DIV_CNFG_REG_BIT_DIV_CLK_FACT_MSK, ENH_8G_CSU_REG_CLK_DIV_CNFG_REG_BIT_DIV_CLK_FACT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_DIV_CLK_FACT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_TERM_OVR_set( enh_8g_csu_buffer_t *b_ptr,
                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_TERM_OVR_set( enh_8g_csu_buffer_t *b_ptr,
                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_TERM_OVR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_TERM_OVR_set", value );

    /* (0x0000045c bits 16) field TERM_OVR of register PMC_ENH_8G_CSU_REG_MABC_TC_CNFG_REG */
    enh_8g_csu_reg_MABC_TC_CNFG_REG_field_set( b_ptr,
                                               h_ptr,
                                               ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERM_OVR_MSK,
                                               ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERM_OVR_OFF,
                                               value);
}

static INLINE UINT32 enh_8g_csu_field_TERM_OVR_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_TERM_OVR_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000045c bits 16) field TERM_OVR of register PMC_ENH_8G_CSU_REG_MABC_TC_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_TC_CNFG_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERM_OVR_MSK) >> ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERM_OVR_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_TERM_OVR_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_TERMB0_ENB_DIG_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_TERMB0_ENB_DIG_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_TERMB0_ENB_DIG_set", value, 15);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_TERMB0_ENB_DIG_set", value );

    /* (0x0000045c bits 11:8) field TERMB0_ENB_DIG of register PMC_ENH_8G_CSU_REG_MABC_TC_CNFG_REG */
    enh_8g_csu_reg_MABC_TC_CNFG_REG_field_set( b_ptr,
                                               h_ptr,
                                               ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMB0_ENB_DIG_MSK,
                                               ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMB0_ENB_DIG_OFF,
                                               value);
}

static INLINE UINT32 enh_8g_csu_field_TERMB0_ENB_DIG_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_TERMB0_ENB_DIG_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000045c bits 11:8) field TERMB0_ENB_DIG of register PMC_ENH_8G_CSU_REG_MABC_TC_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_TC_CNFG_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMB0_ENB_DIG_MSK) >> ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMB0_ENB_DIG_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_TERMB0_ENB_DIG_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_TERMB0_ENB_DIG_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_TERMB0_ENB_DIG_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_TERMB0_ENB_DIG_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_TERMB0_ENB_DIG_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_TERMB0_ENB_DIG_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000045c bits 11:8) field TERMB0_ENB_DIG of register PMC_ENH_8G_CSU_REG_MABC_TC_CNFG_REG */
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
        /* (0x0000045c bits 11:8) field TERMB0_ENB_DIG of register PMC_ENH_8G_CSU_REG_MABC_TC_CNFG_REG */
        enh_8g_csu_reg_MABC_TC_CNFG_REG_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMB0_ENB_DIG_OFF + subfield_offset),
                                                   ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMB0_ENB_DIG_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_TERMB0_ENB_DIG_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_TERMB0_ENB_DIG_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_TERMB0_ENB_DIG_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_TERMB0_ENB_DIG_get", stop_bit, 3 );
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
    /* (0x0000045c bits 11:8) field TERMB0_ENB_DIG of register PMC_ENH_8G_CSU_REG_MABC_TC_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_TC_CNFG_REG_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMB0_ENB_DIG_MSK)
                  >> ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMB0_ENB_DIG_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMB0_ENB_DIG_MSK, ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMB0_ENB_DIG_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_TERMB0_ENB_DIG_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_TERMA0_ENB_DIG_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_TERMA0_ENB_DIG_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_TERMA0_ENB_DIG_set", value, 15);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_TERMA0_ENB_DIG_set", value );

    /* (0x0000045c bits 3:0) field TERMA0_ENB_DIG of register PMC_ENH_8G_CSU_REG_MABC_TC_CNFG_REG */
    enh_8g_csu_reg_MABC_TC_CNFG_REG_field_set( b_ptr,
                                               h_ptr,
                                               ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMA0_ENB_DIG_MSK,
                                               ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMA0_ENB_DIG_OFF,
                                               value);
}

static INLINE UINT32 enh_8g_csu_field_TERMA0_ENB_DIG_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_TERMA0_ENB_DIG_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000045c bits 3:0) field TERMA0_ENB_DIG of register PMC_ENH_8G_CSU_REG_MABC_TC_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_TC_CNFG_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMA0_ENB_DIG_MSK) >> ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMA0_ENB_DIG_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_TERMA0_ENB_DIG_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_TERMA0_ENB_DIG_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_TERMA0_ENB_DIG_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_TERMA0_ENB_DIG_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_TERMA0_ENB_DIG_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_TERMA0_ENB_DIG_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000045c bits 3:0) field TERMA0_ENB_DIG of register PMC_ENH_8G_CSU_REG_MABC_TC_CNFG_REG */
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
        /* (0x0000045c bits 3:0) field TERMA0_ENB_DIG of register PMC_ENH_8G_CSU_REG_MABC_TC_CNFG_REG */
        enh_8g_csu_reg_MABC_TC_CNFG_REG_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMA0_ENB_DIG_OFF + subfield_offset),
                                                   ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMA0_ENB_DIG_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_TERMA0_ENB_DIG_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_TERMA0_ENB_DIG_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_TERMA0_ENB_DIG_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_TERMA0_ENB_DIG_get", stop_bit, 3 );
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
    /* (0x0000045c bits 3:0) field TERMA0_ENB_DIG of register PMC_ENH_8G_CSU_REG_MABC_TC_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_TC_CNFG_REG_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMA0_ENB_DIG_MSK)
                  >> ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMA0_ENB_DIG_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMA0_ENB_DIG_MSK, ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMA0_ENB_DIG_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_TERMA0_ENB_DIG_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_ATMSB_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_ATMSB_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_ATMSB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_ATMSB_set", value );

    /* (0x00000460 bits 6) field CSU_ATMSB of register PMC_ENH_8G_CSU_REG_MABC_TEST_CON_REG */
    enh_8g_csu_reg_MABC_TEST_CON_REG_field_set( b_ptr,
                                                h_ptr,
                                                ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_CSU_ATMSB_MSK,
                                                ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_CSU_ATMSB_OFF,
                                                value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_ATMSB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_ATMSB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000460 bits 6) field CSU_ATMSB of register PMC_ENH_8G_CSU_REG_MABC_TEST_CON_REG */
    reg_value = enh_8g_csu_reg_MABC_TEST_CON_REG_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_CSU_ATMSB_MSK) >> ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_CSU_ATMSB_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_ATMSB_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CS_ATMSB_set( enh_8g_csu_buffer_t *b_ptr,
                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CS_ATMSB_set( enh_8g_csu_buffer_t *b_ptr,
                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CS_ATMSB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CS_ATMSB_set", value );

    /* (0x00000460 bits 5) field CS_ATMSB of register PMC_ENH_8G_CSU_REG_MABC_TEST_CON_REG */
    enh_8g_csu_reg_MABC_TEST_CON_REG_field_set( b_ptr,
                                                h_ptr,
                                                ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_CS_ATMSB_MSK,
                                                ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_CS_ATMSB_OFF,
                                                value);
}

static INLINE UINT32 enh_8g_csu_field_CS_ATMSB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CS_ATMSB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000460 bits 5) field CS_ATMSB of register PMC_ENH_8G_CSU_REG_MABC_TEST_CON_REG */
    reg_value = enh_8g_csu_reg_MABC_TEST_CON_REG_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_CS_ATMSB_MSK) >> ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_CS_ATMSB_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CS_ATMSB_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_ATIN_set( enh_8g_csu_buffer_t *b_ptr,
                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_ATIN_set( enh_8g_csu_buffer_t *b_ptr,
                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_ATIN_set", value, 31);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_ATIN_set", value );

    /* (0x00000460 bits 4:0) field ATIN of register PMC_ENH_8G_CSU_REG_MABC_TEST_CON_REG */
    enh_8g_csu_reg_MABC_TEST_CON_REG_field_set( b_ptr,
                                                h_ptr,
                                                ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_ATIN_MSK,
                                                ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_ATIN_OFF,
                                                value);
}

static INLINE UINT32 enh_8g_csu_field_ATIN_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_ATIN_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000460 bits 4:0) field ATIN of register PMC_ENH_8G_CSU_REG_MABC_TEST_CON_REG */
    reg_value = enh_8g_csu_reg_MABC_TEST_CON_REG_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_ATIN_MSK) >> ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_ATIN_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_ATIN_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_ATIN_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_ATIN_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_ATIN_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_ATIN_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_ATIN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000460 bits 4:0) field ATIN of register PMC_ENH_8G_CSU_REG_MABC_TEST_CON_REG */
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
        /* (0x00000460 bits 4:0) field ATIN of register PMC_ENH_8G_CSU_REG_MABC_TEST_CON_REG */
        enh_8g_csu_reg_MABC_TEST_CON_REG_field_set( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_ATIN_OFF + subfield_offset),
                                                    ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_ATIN_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_ATIN_get( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_ATIN_get( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_ATIN_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_ATIN_get", stop_bit, 4 );
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
    /* (0x00000460 bits 4:0) field ATIN of register PMC_ENH_8G_CSU_REG_MABC_TEST_CON_REG */
    reg_value = enh_8g_csu_reg_MABC_TEST_CON_REG_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_ATIN_MSK)
                  >> ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_ATIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_ATIN_MSK, ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_ATIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_ATIN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_ENB_set( enh_8g_csu_buffer_t *b_ptr,
                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_ENB_set( enh_8g_csu_buffer_t *b_ptr,
                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_ENB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_ENB_set", value );

    /* (0x00000464 bits 0) field ENB of register PMC_ENH_8G_CSU_REG_MABC_ENB_CON_REG */
    enh_8g_csu_reg_MABC_ENB_CON_REG_field_set( b_ptr,
                                               h_ptr,
                                               ENH_8G_CSU_REG_MABC_ENB_CON_REG_BIT_ENB_MSK,
                                               ENH_8G_CSU_REG_MABC_ENB_CON_REG_BIT_ENB_OFF,
                                               value);
}

static INLINE UINT32 enh_8g_csu_field_ENB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_ENB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000464 bits 0) field ENB of register PMC_ENH_8G_CSU_REG_MABC_ENB_CON_REG */
    reg_value = enh_8g_csu_reg_MABC_ENB_CON_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_ENB_CON_REG_BIT_ENB_MSK) >> ENH_8G_CSU_REG_MABC_ENB_CON_REG_BIT_ENB_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_ENB_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CS_DIVMODE_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CS_DIVMODE_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CS_DIVMODE_set", value, 3);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CS_DIVMODE_set", value );

    /* (0x00000468 bits 21:20) field CS_DIVMODE of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_DIVMODE_MSK,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_DIVMODE_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_CS_DIVMODE_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CS_DIVMODE_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000468 bits 21:20) field CS_DIVMODE of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    reg_value = enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_DIVMODE_MSK) >> ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_DIVMODE_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CS_DIVMODE_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_CS_DIVMODE_set( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_CS_DIVMODE_set( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CS_DIVMODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CS_DIVMODE_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CS_DIVMODE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000468 bits 21:20) field CS_DIVMODE of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
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
        /* (0x00000468 bits 21:20) field CS_DIVMODE of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
        enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_DIVMODE_OFF + subfield_offset),
                                                          ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_DIVMODE_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_CS_DIVMODE_get( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_CS_DIVMODE_get( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CS_DIVMODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CS_DIVMODE_get", stop_bit, 1 );
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
    /* (0x00000468 bits 21:20) field CS_DIVMODE of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    reg_value = enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_DIVMODE_MSK)
                  >> ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_DIVMODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_DIVMODE_MSK, ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_DIVMODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CS_DIVMODE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_CS_CTRL_7_3_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CS_CTRL_7_3_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CS_CTRL_7_3_set", value, 31);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CS_CTRL_7_3_set", value );

    /* (0x00000468 bits 17:13) field CS_CTRL_7_3 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_7_3_MSK,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_7_3_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_CS_CTRL_7_3_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CS_CTRL_7_3_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000468 bits 17:13) field CS_CTRL_7_3 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    reg_value = enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_7_3_MSK) >> ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_7_3_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CS_CTRL_7_3_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_CS_CTRL_7_3_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_CS_CTRL_7_3_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CS_CTRL_7_3_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CS_CTRL_7_3_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CS_CTRL_7_3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000468 bits 17:13) field CS_CTRL_7_3 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
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
        /* (0x00000468 bits 17:13) field CS_CTRL_7_3 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
        enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_7_3_OFF + subfield_offset),
                                                          ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_7_3_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_CS_CTRL_7_3_get( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_CS_CTRL_7_3_get( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CS_CTRL_7_3_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CS_CTRL_7_3_get", stop_bit, 4 );
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
    /* (0x00000468 bits 17:13) field CS_CTRL_7_3 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    reg_value = enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_7_3_MSK)
                  >> ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_7_3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_7_3_MSK, ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_7_3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CS_CTRL_7_3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_CS_CTRL_2_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CS_CTRL_2_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CS_CTRL_2_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CS_CTRL_2_set", value );

    /* (0x00000468 bits 12) field CS_CTRL_2 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_2_MSK,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_2_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_CS_CTRL_2_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CS_CTRL_2_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000468 bits 12) field CS_CTRL_2 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    reg_value = enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_2_MSK) >> ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_2_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CS_CTRL_2_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CS_CTRL_1_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CS_CTRL_1_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CS_CTRL_1_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CS_CTRL_1_set", value );

    /* (0x00000468 bits 11) field CS_CTRL_1 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_1_MSK,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_1_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_CS_CTRL_1_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CS_CTRL_1_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000468 bits 11) field CS_CTRL_1 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    reg_value = enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_1_MSK) >> ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_1_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CS_CTRL_1_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CS_CTRL_0_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CS_CTRL_0_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CS_CTRL_0_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CS_CTRL_0_set", value );

    /* (0x00000468 bits 10) field CS_CTRL_0 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_0_MSK,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_0_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_CS_CTRL_0_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CS_CTRL_0_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000468 bits 10) field CS_CTRL_0 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    reg_value = enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_0_MSK) >> ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_0_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CS_CTRL_0_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CS_MODE_7_6_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CS_MODE_7_6_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CS_MODE_7_6_set", value, 3);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CS_MODE_7_6_set", value );

    /* (0x00000468 bits 7:6) field CS_MODE_7_6 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_7_6_MSK,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_7_6_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_CS_MODE_7_6_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CS_MODE_7_6_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000468 bits 7:6) field CS_MODE_7_6 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    reg_value = enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_7_6_MSK) >> ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_7_6_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CS_MODE_7_6_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_CS_MODE_7_6_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_CS_MODE_7_6_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CS_MODE_7_6_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CS_MODE_7_6_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CS_MODE_7_6_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000468 bits 7:6) field CS_MODE_7_6 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
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
        /* (0x00000468 bits 7:6) field CS_MODE_7_6 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
        enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_7_6_OFF + subfield_offset),
                                                          ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_7_6_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_CS_MODE_7_6_get( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_CS_MODE_7_6_get( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CS_MODE_7_6_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CS_MODE_7_6_get", stop_bit, 1 );
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
    /* (0x00000468 bits 7:6) field CS_MODE_7_6 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    reg_value = enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_7_6_MSK)
                  >> ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_7_6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_7_6_MSK, ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_7_6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CS_MODE_7_6_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_CS_MODE_5_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CS_MODE_5_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CS_MODE_5_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CS_MODE_5_set", value );

    /* (0x00000468 bits 5) field CS_MODE_5 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_5_MSK,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_5_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_CS_MODE_5_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CS_MODE_5_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000468 bits 5) field CS_MODE_5 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    reg_value = enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_5_MSK) >> ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_5_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CS_MODE_5_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CS_MODE_4_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CS_MODE_4_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CS_MODE_4_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CS_MODE_4_set", value );

    /* (0x00000468 bits 4) field CS_MODE_4 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_4_MSK,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_4_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_CS_MODE_4_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CS_MODE_4_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000468 bits 4) field CS_MODE_4 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    reg_value = enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_4_MSK) >> ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_4_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CS_MODE_4_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CS_MODE_3_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CS_MODE_3_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CS_MODE_3_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CS_MODE_3_set", value );

    /* (0x00000468 bits 3) field CS_MODE_3 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_3_MSK,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_3_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_CS_MODE_3_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CS_MODE_3_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000468 bits 3) field CS_MODE_3 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    reg_value = enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_3_MSK) >> ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_3_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CS_MODE_3_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CS_MODE_2_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CS_MODE_2_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CS_MODE_2_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CS_MODE_2_set", value );

    /* (0x00000468 bits 2) field CS_MODE_2 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_2_MSK,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_2_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_CS_MODE_2_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CS_MODE_2_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000468 bits 2) field CS_MODE_2 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    reg_value = enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_2_MSK) >> ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_2_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CS_MODE_2_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CS_MODE_1_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CS_MODE_1_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CS_MODE_1_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CS_MODE_1_set", value );

    /* (0x00000468 bits 1) field CS_MODE_1 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_1_MSK,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_1_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_CS_MODE_1_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CS_MODE_1_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000468 bits 1) field CS_MODE_1 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    reg_value = enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_1_MSK) >> ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_1_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CS_MODE_1_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CS_MODE_0_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CS_MODE_0_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CS_MODE_0_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CS_MODE_0_set", value );

    /* (0x00000468 bits 0) field CS_MODE_0 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_0_MSK,
                                                      ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_0_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_CS_MODE_0_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CS_MODE_0_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000468 bits 0) field CS_MODE_0 of register PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG */
    reg_value = enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_0_MSK) >> ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_0_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CS_MODE_0_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_MODE_31_28_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_MODE_31_28_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_MODE_31_28_set", value, 15);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_MODE_31_28_set", value );

    /* (0x00000470 bits 31:28) field CSU_MODE_31_28 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_31_28_MSK,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_31_28_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_MODE_31_28_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_MODE_31_28_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000470 bits 31:28) field CSU_MODE_31_28 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_31_28_MSK) >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_31_28_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_MODE_31_28_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_CSU_MODE_31_28_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_CSU_MODE_31_28_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_MODE_31_28_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_MODE_31_28_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_MODE_31_28_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000470 bits 31:28) field CSU_MODE_31_28 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
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
        /* (0x00000470 bits 31:28) field CSU_MODE_31_28 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
        enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_31_28_OFF + subfield_offset),
                                                         ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_31_28_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_CSU_MODE_31_28_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_CSU_MODE_31_28_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_MODE_31_28_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_MODE_31_28_get", stop_bit, 3 );
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
    /* (0x00000470 bits 31:28) field CSU_MODE_31_28 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_31_28_MSK)
                  >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_31_28_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_31_28_MSK, ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_31_28_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_MODE_31_28_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_MODE_27_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_MODE_27_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_MODE_27_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_MODE_27_set", value );

    /* (0x00000470 bits 27) field CSU_MODE_27 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_27_MSK,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_27_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_MODE_27_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_MODE_27_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000470 bits 27) field CSU_MODE_27 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_27_MSK) >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_27_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_MODE_27_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_MODE_26_25_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_MODE_26_25_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_MODE_26_25_set", value, 3);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_MODE_26_25_set", value );

    /* (0x00000470 bits 26:25) field CSU_MODE_26_25 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_26_25_MSK,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_26_25_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_MODE_26_25_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_MODE_26_25_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000470 bits 26:25) field CSU_MODE_26_25 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_26_25_MSK) >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_26_25_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_MODE_26_25_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_CSU_MODE_26_25_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_CSU_MODE_26_25_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_MODE_26_25_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_MODE_26_25_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_MODE_26_25_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000470 bits 26:25) field CSU_MODE_26_25 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
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
        /* (0x00000470 bits 26:25) field CSU_MODE_26_25 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
        enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_26_25_OFF + subfield_offset),
                                                         ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_26_25_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_CSU_MODE_26_25_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_CSU_MODE_26_25_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_MODE_26_25_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_MODE_26_25_get", stop_bit, 1 );
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
    /* (0x00000470 bits 26:25) field CSU_MODE_26_25 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_26_25_MSK)
                  >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_26_25_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_26_25_MSK, ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_26_25_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_MODE_26_25_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_MODE_24_22_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_MODE_24_22_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_MODE_24_22_set", value, 7);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_MODE_24_22_set", value );

    /* (0x00000470 bits 24:22) field CSU_MODE_24_22 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_24_22_MSK,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_24_22_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_MODE_24_22_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_MODE_24_22_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000470 bits 24:22) field CSU_MODE_24_22 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_24_22_MSK) >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_24_22_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_MODE_24_22_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_CSU_MODE_24_22_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_CSU_MODE_24_22_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_MODE_24_22_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_MODE_24_22_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_MODE_24_22_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000470 bits 24:22) field CSU_MODE_24_22 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
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
        /* (0x00000470 bits 24:22) field CSU_MODE_24_22 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
        enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_24_22_OFF + subfield_offset),
                                                         ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_24_22_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_CSU_MODE_24_22_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_CSU_MODE_24_22_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_MODE_24_22_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_MODE_24_22_get", stop_bit, 2 );
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
    /* (0x00000470 bits 24:22) field CSU_MODE_24_22 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_24_22_MSK)
                  >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_24_22_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_24_22_MSK, ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_24_22_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_MODE_24_22_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_MODE_21_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_MODE_21_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_MODE_21_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_MODE_21_set", value );

    /* (0x00000470 bits 21) field CSU_MODE_21 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_21_MSK,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_21_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_MODE_21_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_MODE_21_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000470 bits 21) field CSU_MODE_21 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_21_MSK) >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_21_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_MODE_21_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_MODE_20_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_MODE_20_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_MODE_20_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_MODE_20_set", value );

    /* (0x00000470 bits 20) field CSU_MODE_20 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_20_MSK,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_20_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_MODE_20_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_MODE_20_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000470 bits 20) field CSU_MODE_20 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_20_MSK) >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_20_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_MODE_20_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_MODE_19_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_MODE_19_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_MODE_19_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_MODE_19_set", value );

    /* (0x00000470 bits 19) field CSU_MODE_19 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_19_MSK,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_19_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_MODE_19_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_MODE_19_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000470 bits 19) field CSU_MODE_19 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_19_MSK) >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_19_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_MODE_19_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_MODE_18_16_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_MODE_18_16_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_MODE_18_16_set", value, 7);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_MODE_18_16_set", value );

    /* (0x00000470 bits 18:16) field CSU_MODE_18_16 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_18_16_MSK,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_18_16_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_MODE_18_16_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_MODE_18_16_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000470 bits 18:16) field CSU_MODE_18_16 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_18_16_MSK) >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_18_16_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_MODE_18_16_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_CSU_MODE_18_16_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_CSU_MODE_18_16_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_MODE_18_16_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_MODE_18_16_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_MODE_18_16_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000470 bits 18:16) field CSU_MODE_18_16 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
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
        /* (0x00000470 bits 18:16) field CSU_MODE_18_16 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
        enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_18_16_OFF + subfield_offset),
                                                         ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_18_16_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_CSU_MODE_18_16_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_CSU_MODE_18_16_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_MODE_18_16_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_MODE_18_16_get", stop_bit, 2 );
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
    /* (0x00000470 bits 18:16) field CSU_MODE_18_16 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_18_16_MSK)
                  >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_18_16_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_18_16_MSK, ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_18_16_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_MODE_18_16_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_MODE_15_10_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_MODE_15_10_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_MODE_15_10_set", value, 63);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_MODE_15_10_set", value );

    /* (0x00000470 bits 15:10) field CSU_MODE_15_10 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_15_10_MSK,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_15_10_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_MODE_15_10_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_MODE_15_10_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000470 bits 15:10) field CSU_MODE_15_10 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_15_10_MSK) >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_15_10_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_MODE_15_10_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_CSU_MODE_15_10_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_CSU_MODE_15_10_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_MODE_15_10_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_MODE_15_10_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_MODE_15_10_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000470 bits 15:10) field CSU_MODE_15_10 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
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
        /* (0x00000470 bits 15:10) field CSU_MODE_15_10 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
        enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_15_10_OFF + subfield_offset),
                                                         ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_15_10_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_CSU_MODE_15_10_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_CSU_MODE_15_10_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_MODE_15_10_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_MODE_15_10_get", stop_bit, 5 );
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
    /* (0x00000470 bits 15:10) field CSU_MODE_15_10 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_15_10_MSK)
                  >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_15_10_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_15_10_MSK, ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_15_10_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_MODE_15_10_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_MODE_9_8_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_MODE_9_8_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_MODE_9_8_set", value, 3);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_MODE_9_8_set", value );

    /* (0x00000470 bits 9:8) field CSU_MODE_9_8 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_9_8_MSK,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_9_8_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_MODE_9_8_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_MODE_9_8_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000470 bits 9:8) field CSU_MODE_9_8 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_9_8_MSK) >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_9_8_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_MODE_9_8_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_CSU_MODE_9_8_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_CSU_MODE_9_8_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_MODE_9_8_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_MODE_9_8_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_MODE_9_8_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000470 bits 9:8) field CSU_MODE_9_8 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
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
        /* (0x00000470 bits 9:8) field CSU_MODE_9_8 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
        enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_9_8_OFF + subfield_offset),
                                                         ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_9_8_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_CSU_MODE_9_8_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_CSU_MODE_9_8_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_MODE_9_8_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_MODE_9_8_get", stop_bit, 1 );
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
    /* (0x00000470 bits 9:8) field CSU_MODE_9_8 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_9_8_MSK)
                  >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_9_8_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_9_8_MSK, ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_9_8_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_MODE_9_8_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_MODE_7_6_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_MODE_7_6_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_MODE_7_6_set", value, 3);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_MODE_7_6_set", value );

    /* (0x00000470 bits 7:6) field CSU_MODE_7_6 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_7_6_MSK,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_7_6_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_MODE_7_6_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_MODE_7_6_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000470 bits 7:6) field CSU_MODE_7_6 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_7_6_MSK) >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_7_6_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_MODE_7_6_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_CSU_MODE_7_6_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_CSU_MODE_7_6_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_MODE_7_6_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_MODE_7_6_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_MODE_7_6_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000470 bits 7:6) field CSU_MODE_7_6 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
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
        /* (0x00000470 bits 7:6) field CSU_MODE_7_6 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
        enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_7_6_OFF + subfield_offset),
                                                         ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_7_6_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_CSU_MODE_7_6_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_CSU_MODE_7_6_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_MODE_7_6_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_MODE_7_6_get", stop_bit, 1 );
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
    /* (0x00000470 bits 7:6) field CSU_MODE_7_6 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_7_6_MSK)
                  >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_7_6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_7_6_MSK, ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_7_6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_MODE_7_6_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_MODE_5_3_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_MODE_5_3_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_MODE_5_3_set", value, 7);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_MODE_5_3_set", value );

    /* (0x00000470 bits 5:3) field CSU_MODE_5_3 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_5_3_MSK,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_5_3_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_MODE_5_3_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_MODE_5_3_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000470 bits 5:3) field CSU_MODE_5_3 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_5_3_MSK) >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_5_3_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_MODE_5_3_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_CSU_MODE_5_3_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_CSU_MODE_5_3_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_MODE_5_3_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_MODE_5_3_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_MODE_5_3_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000470 bits 5:3) field CSU_MODE_5_3 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
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
        /* (0x00000470 bits 5:3) field CSU_MODE_5_3 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
        enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_5_3_OFF + subfield_offset),
                                                         ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_5_3_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_CSU_MODE_5_3_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_CSU_MODE_5_3_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_MODE_5_3_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_MODE_5_3_get", stop_bit, 2 );
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
    /* (0x00000470 bits 5:3) field CSU_MODE_5_3 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_5_3_MSK)
                  >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_5_3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_5_3_MSK, ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_5_3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_MODE_5_3_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_MODE_2_0_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_MODE_2_0_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_MODE_2_0_set", value, 7);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_MODE_2_0_set", value );

    /* (0x00000470 bits 2:0) field CSU_MODE_2_0 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_2_0_MSK,
                                                     ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_2_0_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_MODE_2_0_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_MODE_2_0_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000470 bits 2:0) field CSU_MODE_2_0 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_2_0_MSK) >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_2_0_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_MODE_2_0_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_CSU_MODE_2_0_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_CSU_MODE_2_0_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_MODE_2_0_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_MODE_2_0_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_MODE_2_0_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000470 bits 2:0) field CSU_MODE_2_0 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
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
        /* (0x00000470 bits 2:0) field CSU_MODE_2_0 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
        enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_2_0_OFF + subfield_offset),
                                                         ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_2_0_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_CSU_MODE_2_0_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_CSU_MODE_2_0_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_MODE_2_0_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_MODE_2_0_get", stop_bit, 2 );
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
    /* (0x00000470 bits 2:0) field CSU_MODE_2_0 of register PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG */
    reg_value = enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_2_0_MSK)
                  >> ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_2_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_2_0_MSK, ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_2_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_MODE_2_0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_CTRL_19_17_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_CTRL_19_17_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_CTRL_19_17_set", value, 7);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_CTRL_19_17_set", value );

    /* (0x00000474 bits 19:17) field CSU_CTRL_19_17 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    enh_8g_csu_reg_MABC_CNTL_REG_field_set( b_ptr,
                                            h_ptr,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_19_17_MSK,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_19_17_OFF,
                                            value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_19_17_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_19_17_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000474 bits 19:17) field CSU_CTRL_19_17 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    reg_value = enh_8g_csu_reg_MABC_CNTL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_19_17_MSK) >> ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_19_17_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_CTRL_19_17_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_CSU_CTRL_19_17_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_CSU_CTRL_19_17_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_CTRL_19_17_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_CTRL_19_17_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_CTRL_19_17_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000474 bits 19:17) field CSU_CTRL_19_17 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
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
        /* (0x00000474 bits 19:17) field CSU_CTRL_19_17 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
        enh_8g_csu_reg_MABC_CNTL_REG_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_19_17_OFF + subfield_offset),
                                                ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_19_17_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_CSU_CTRL_19_17_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_CSU_CTRL_19_17_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_CTRL_19_17_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_CTRL_19_17_get", stop_bit, 2 );
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
    /* (0x00000474 bits 19:17) field CSU_CTRL_19_17 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    reg_value = enh_8g_csu_reg_MABC_CNTL_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_19_17_MSK)
                  >> ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_19_17_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_19_17_MSK, ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_19_17_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_CTRL_19_17_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_CTRL_16_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_CTRL_16_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_CTRL_16_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_CTRL_16_set", value );

    /* (0x00000474 bits 16) field CSU_CTRL_16 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    enh_8g_csu_reg_MABC_CNTL_REG_field_set( b_ptr,
                                            h_ptr,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_16_MSK,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_16_OFF,
                                            value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_16_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_16_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000474 bits 16) field CSU_CTRL_16 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    reg_value = enh_8g_csu_reg_MABC_CNTL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_16_MSK) >> ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_16_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_CTRL_16_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_CTRL_15_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_CTRL_15_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_CTRL_15_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_CTRL_15_set", value );

    /* (0x00000474 bits 15) field CSU_CTRL_15 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    enh_8g_csu_reg_MABC_CNTL_REG_field_set( b_ptr,
                                            h_ptr,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_15_MSK,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_15_OFF,
                                            value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_15_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_15_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000474 bits 15) field CSU_CTRL_15 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    reg_value = enh_8g_csu_reg_MABC_CNTL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_15_MSK) >> ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_15_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_CTRL_15_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_CTRL_14_13_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_CTRL_14_13_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_CTRL_14_13_set", value, 3);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_CTRL_14_13_set", value );

    /* (0x00000474 bits 14:13) field CSU_CTRL_14_13 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    enh_8g_csu_reg_MABC_CNTL_REG_field_set( b_ptr,
                                            h_ptr,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_14_13_MSK,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_14_13_OFF,
                                            value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_14_13_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_14_13_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000474 bits 14:13) field CSU_CTRL_14_13 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    reg_value = enh_8g_csu_reg_MABC_CNTL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_14_13_MSK) >> ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_14_13_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_CTRL_14_13_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_CSU_CTRL_14_13_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_CSU_CTRL_14_13_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_CTRL_14_13_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_CTRL_14_13_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_CTRL_14_13_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000474 bits 14:13) field CSU_CTRL_14_13 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
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
        /* (0x00000474 bits 14:13) field CSU_CTRL_14_13 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
        enh_8g_csu_reg_MABC_CNTL_REG_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_14_13_OFF + subfield_offset),
                                                ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_14_13_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_CSU_CTRL_14_13_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_CSU_CTRL_14_13_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_CTRL_14_13_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_CTRL_14_13_get", stop_bit, 1 );
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
    /* (0x00000474 bits 14:13) field CSU_CTRL_14_13 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    reg_value = enh_8g_csu_reg_MABC_CNTL_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_14_13_MSK)
                  >> ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_14_13_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_14_13_MSK, ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_14_13_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_CTRL_14_13_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_CTRL_12_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_CTRL_12_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_CTRL_12_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_CTRL_12_set", value );

    /* (0x00000474 bits 12) field CSU_CTRL_12 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    enh_8g_csu_reg_MABC_CNTL_REG_field_set( b_ptr,
                                            h_ptr,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_12_MSK,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_12_OFF,
                                            value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_12_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_12_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000474 bits 12) field CSU_CTRL_12 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    reg_value = enh_8g_csu_reg_MABC_CNTL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_12_MSK) >> ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_12_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_CTRL_12_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_CTRL_11_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_CTRL_11_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_CTRL_11_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_CTRL_11_set", value );

    /* (0x00000474 bits 11) field CSU_CTRL_11 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    enh_8g_csu_reg_MABC_CNTL_REG_field_set( b_ptr,
                                            h_ptr,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_11_MSK,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_11_OFF,
                                            value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_11_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_11_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000474 bits 11) field CSU_CTRL_11 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    reg_value = enh_8g_csu_reg_MABC_CNTL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_11_MSK) >> ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_11_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_CTRL_11_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_CTRL_10_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_CTRL_10_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_CTRL_10_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_CTRL_10_set", value );

    /* (0x00000474 bits 10) field CSU_CTRL_10 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    enh_8g_csu_reg_MABC_CNTL_REG_field_set( b_ptr,
                                            h_ptr,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_10_MSK,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_10_OFF,
                                            value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_10_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_10_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000474 bits 10) field CSU_CTRL_10 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    reg_value = enh_8g_csu_reg_MABC_CNTL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_10_MSK) >> ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_10_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_CTRL_10_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_CTRL_9_6_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_CTRL_9_6_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_CTRL_9_6_set", value, 15);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_CTRL_9_6_set", value );

    /* (0x00000474 bits 9:6) field CSU_CTRL_9_6 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    enh_8g_csu_reg_MABC_CNTL_REG_field_set( b_ptr,
                                            h_ptr,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_9_6_MSK,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_9_6_OFF,
                                            value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_9_6_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_9_6_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000474 bits 9:6) field CSU_CTRL_9_6 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    reg_value = enh_8g_csu_reg_MABC_CNTL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_9_6_MSK) >> ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_9_6_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_CTRL_9_6_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_CSU_CTRL_9_6_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_CSU_CTRL_9_6_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_CTRL_9_6_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_CTRL_9_6_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_CTRL_9_6_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000474 bits 9:6) field CSU_CTRL_9_6 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
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
        /* (0x00000474 bits 9:6) field CSU_CTRL_9_6 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
        enh_8g_csu_reg_MABC_CNTL_REG_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_9_6_OFF + subfield_offset),
                                                ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_9_6_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_CSU_CTRL_9_6_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_CSU_CTRL_9_6_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_CTRL_9_6_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_CTRL_9_6_get", stop_bit, 3 );
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
    /* (0x00000474 bits 9:6) field CSU_CTRL_9_6 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    reg_value = enh_8g_csu_reg_MABC_CNTL_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_9_6_MSK)
                  >> ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_9_6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_9_6_MSK, ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_9_6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_CTRL_9_6_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_CTRL_5_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_CTRL_5_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_CTRL_5_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_CTRL_5_set", value );

    /* (0x00000474 bits 5) field CSU_CTRL_5 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    enh_8g_csu_reg_MABC_CNTL_REG_field_set( b_ptr,
                                            h_ptr,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_5_MSK,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_5_OFF,
                                            value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_5_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_5_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000474 bits 5) field CSU_CTRL_5 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    reg_value = enh_8g_csu_reg_MABC_CNTL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_5_MSK) >> ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_5_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_CTRL_5_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_CTRL_4_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_CTRL_4_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_CTRL_4_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_CTRL_4_set", value );

    /* (0x00000474 bits 4) field CSU_CTRL_4 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    enh_8g_csu_reg_MABC_CNTL_REG_field_set( b_ptr,
                                            h_ptr,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_4_MSK,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_4_OFF,
                                            value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_4_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_4_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000474 bits 4) field CSU_CTRL_4 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    reg_value = enh_8g_csu_reg_MABC_CNTL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_4_MSK) >> ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_4_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_CTRL_4_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_CTRL_3_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_CTRL_3_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_CTRL_3_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_CTRL_3_set", value );

    /* (0x00000474 bits 3) field CSU_CTRL_3 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    enh_8g_csu_reg_MABC_CNTL_REG_field_set( b_ptr,
                                            h_ptr,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_3_MSK,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_3_OFF,
                                            value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_3_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_3_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000474 bits 3) field CSU_CTRL_3 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    reg_value = enh_8g_csu_reg_MABC_CNTL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_3_MSK) >> ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_3_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_CTRL_3_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_CTRL_2_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_CTRL_2_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_CTRL_2_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_CTRL_2_set", value );

    /* (0x00000474 bits 2) field CSU_CTRL_2 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    enh_8g_csu_reg_MABC_CNTL_REG_field_set( b_ptr,
                                            h_ptr,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_2_MSK,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_2_OFF,
                                            value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_2_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_2_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000474 bits 2) field CSU_CTRL_2 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    reg_value = enh_8g_csu_reg_MABC_CNTL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_2_MSK) >> ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_2_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_CTRL_2_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CSU_CTRL_1_0_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CSU_CTRL_1_0_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CSU_CTRL_1_0_set", value, 3);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CSU_CTRL_1_0_set", value );

    /* (0x00000474 bits 1:0) field CSU_CTRL_1_0 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    enh_8g_csu_reg_MABC_CNTL_REG_field_set( b_ptr,
                                            h_ptr,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_1_0_MSK,
                                            ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_1_0_OFF,
                                            value);
}

static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_1_0_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CSU_CTRL_1_0_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000474 bits 1:0) field CSU_CTRL_1_0 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    reg_value = enh_8g_csu_reg_MABC_CNTL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_1_0_MSK) >> ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_1_0_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CSU_CTRL_1_0_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_CSU_CTRL_1_0_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_CSU_CTRL_1_0_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_CTRL_1_0_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_CTRL_1_0_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_CTRL_1_0_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000474 bits 1:0) field CSU_CTRL_1_0 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
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
        /* (0x00000474 bits 1:0) field CSU_CTRL_1_0 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
        enh_8g_csu_reg_MABC_CNTL_REG_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_1_0_OFF + subfield_offset),
                                                ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_1_0_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_CSU_CTRL_1_0_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_CSU_CTRL_1_0_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_CSU_CTRL_1_0_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_CSU_CTRL_1_0_get", stop_bit, 1 );
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
    /* (0x00000474 bits 1:0) field CSU_CTRL_1_0 of register PMC_ENH_8G_CSU_REG_MABC_CNTL_REG */
    reg_value = enh_8g_csu_reg_MABC_CNTL_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_1_0_MSK)
                  >> ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_1_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_1_0_MSK, ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_1_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_CSU_CTRL_1_0_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_CLKO_POLARITY_set( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CLKO_POLARITY_set( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CLKO_POLARITY_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CLKO_POLARITY_set", value );

    /* (0x00000480 bits 3) field CLKO_POLARITY of register PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1 */
    enh_8g_csu_reg_E_ADSM_CNFG_REG_1_field_set( b_ptr,
                                                h_ptr,
                                                ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_CLKO_POLARITY_MSK,
                                                ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_CLKO_POLARITY_OFF,
                                                value);
}

static INLINE UINT32 enh_8g_csu_field_CLKO_POLARITY_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CLKO_POLARITY_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000480 bits 3) field CLKO_POLARITY of register PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_E_ADSM_CNFG_REG_1_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_CLKO_POLARITY_MSK) >> ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_CLKO_POLARITY_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CLKO_POLARITY_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_DSM_RSTB_set( enh_8g_csu_buffer_t *b_ptr,
                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_DSM_RSTB_set( enh_8g_csu_buffer_t *b_ptr,
                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_DSM_RSTB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_DSM_RSTB_set", value );

    /* (0x00000480 bits 2) field DSM_RSTB of register PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1 */
    enh_8g_csu_reg_E_ADSM_CNFG_REG_1_field_set( b_ptr,
                                                h_ptr,
                                                ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_DSM_RSTB_MSK,
                                                ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_DSM_RSTB_OFF,
                                                value);
}

static INLINE UINT32 enh_8g_csu_field_DSM_RSTB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_DSM_RSTB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000480 bits 2) field DSM_RSTB of register PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_E_ADSM_CNFG_REG_1_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_DSM_RSTB_MSK) >> ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_DSM_RSTB_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_DSM_RSTB_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_MULT2_set( enh_8g_csu_buffer_t *b_ptr,
                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_MULT2_set( enh_8g_csu_buffer_t *b_ptr,
                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_MULT2_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_MULT2_set", value );

    /* (0x00000480 bits 1) field MULT2 of register PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1 */
    enh_8g_csu_reg_E_ADSM_CNFG_REG_1_field_set( b_ptr,
                                                h_ptr,
                                                ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_MULT2_MSK,
                                                ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_MULT2_OFF,
                                                value);
}

static INLINE UINT32 enh_8g_csu_field_MULT2_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_MULT2_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000480 bits 1) field MULT2 of register PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_E_ADSM_CNFG_REG_1_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_MULT2_MSK) >> ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_MULT2_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_MULT2_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_SEED_set( enh_8g_csu_buffer_t *b_ptr,
                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_SEED_set( enh_8g_csu_buffer_t *b_ptr,
                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_SEED_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_SEED_set", value );

    /* (0x00000480 bits 0) field SEED of register PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1 */
    enh_8g_csu_reg_E_ADSM_CNFG_REG_1_field_set( b_ptr,
                                                h_ptr,
                                                ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_SEED_MSK,
                                                ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_SEED_OFF,
                                                value);
}

static INLINE UINT32 enh_8g_csu_field_SEED_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_SEED_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000480 bits 0) field SEED of register PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_E_ADSM_CNFG_REG_1_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_SEED_MSK) >> ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_SEED_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_SEED_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_FRAC_DIV_LOWER_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_FRAC_DIV_LOWER_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_FRAC_DIV_LOWER_set", value );

    /* (0x00000484 bits 31:0) field FRAC_DIV_LOWER of register PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2 */
    enh_8g_csu_reg_E_ADSM_CNFG_REG_2_field_set( b_ptr,
                                                h_ptr,
                                                ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2_BIT_FRAC_DIV_LOWER_MSK,
                                                ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2_BIT_FRAC_DIV_LOWER_OFF,
                                                value);
}

static INLINE UINT32 enh_8g_csu_field_FRAC_DIV_LOWER_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_FRAC_DIV_LOWER_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000484 bits 31:0) field FRAC_DIV_LOWER of register PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2 */
    reg_value = enh_8g_csu_reg_E_ADSM_CNFG_REG_2_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2_BIT_FRAC_DIV_LOWER_MSK) >> ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2_BIT_FRAC_DIV_LOWER_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_FRAC_DIV_LOWER_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_FRAC_DIV_LOWER_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_FRAC_DIV_LOWER_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_FRAC_DIV_LOWER_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_FRAC_DIV_LOWER_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_FRAC_DIV_LOWER_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000484 bits 31:0) field FRAC_DIV_LOWER of register PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2 */
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
        /* (0x00000484 bits 31:0) field FRAC_DIV_LOWER of register PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2 */
        enh_8g_csu_reg_E_ADSM_CNFG_REG_2_field_set( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2_BIT_FRAC_DIV_LOWER_OFF + subfield_offset),
                                                    ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2_BIT_FRAC_DIV_LOWER_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_FRAC_DIV_LOWER_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_FRAC_DIV_LOWER_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_FRAC_DIV_LOWER_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_FRAC_DIV_LOWER_get", stop_bit, 31 );
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
    /* (0x00000484 bits 31:0) field FRAC_DIV_LOWER of register PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2 */
    reg_value = enh_8g_csu_reg_E_ADSM_CNFG_REG_2_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2_BIT_FRAC_DIV_LOWER_MSK)
                  >> ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2_BIT_FRAC_DIV_LOWER_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2_BIT_FRAC_DIV_LOWER_MSK, ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2_BIT_FRAC_DIV_LOWER_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_FRAC_DIV_LOWER_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_FRAC_DIV_UPPER_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_FRAC_DIV_UPPER_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_FRAC_DIV_UPPER_set", value, 255);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_FRAC_DIV_UPPER_set", value );

    /* (0x00000488 bits 7:0) field FRAC_DIV_UPPER of register PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3 */
    enh_8g_csu_reg_E_ADSM_CNFG_REG_3_field_set( b_ptr,
                                                h_ptr,
                                                ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3_BIT_FRAC_DIV_UPPER_MSK,
                                                ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3_BIT_FRAC_DIV_UPPER_OFF,
                                                value);
}

static INLINE UINT32 enh_8g_csu_field_FRAC_DIV_UPPER_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_FRAC_DIV_UPPER_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000488 bits 7:0) field FRAC_DIV_UPPER of register PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3 */
    reg_value = enh_8g_csu_reg_E_ADSM_CNFG_REG_3_read( b_ptr,
                                                       h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3_BIT_FRAC_DIV_UPPER_MSK) >> ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3_BIT_FRAC_DIV_UPPER_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_FRAC_DIV_UPPER_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_FRAC_DIV_UPPER_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_FRAC_DIV_UPPER_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_FRAC_DIV_UPPER_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_FRAC_DIV_UPPER_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_FRAC_DIV_UPPER_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000488 bits 7:0) field FRAC_DIV_UPPER of register PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3 */
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
        /* (0x00000488 bits 7:0) field FRAC_DIV_UPPER of register PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3 */
        enh_8g_csu_reg_E_ADSM_CNFG_REG_3_field_set( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3_BIT_FRAC_DIV_UPPER_OFF + subfield_offset),
                                                    ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3_BIT_FRAC_DIV_UPPER_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_FRAC_DIV_UPPER_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_FRAC_DIV_UPPER_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_FRAC_DIV_UPPER_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_FRAC_DIV_UPPER_get", stop_bit, 7 );
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
    /* (0x00000488 bits 7:0) field FRAC_DIV_UPPER of register PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3 */
    reg_value = enh_8g_csu_reg_E_ADSM_CNFG_REG_3_read( b_ptr,
                                                       h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3_BIT_FRAC_DIV_UPPER_MSK)
                  >> ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3_BIT_FRAC_DIV_UPPER_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3_BIT_FRAC_DIV_UPPER_MSK, ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3_BIT_FRAC_DIV_UPPER_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_FRAC_DIV_UPPER_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_DENOMIN_LOWER_set( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_DENOMIN_LOWER_set( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_DENOMIN_LOWER_set", value );

    /* (0x0000048c bits 31:0) field DENOMIN_LOWER of register PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG */
    enh_8g_csu_reg_E_ADSM_DENOMIN_LOW_CNFG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG_BIT_DENOMIN_LOWER_MSK,
                                                          ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG_BIT_DENOMIN_LOWER_OFF,
                                                          value);
}

static INLINE UINT32 enh_8g_csu_field_DENOMIN_LOWER_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_DENOMIN_LOWER_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000048c bits 31:0) field DENOMIN_LOWER of register PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG */
    reg_value = enh_8g_csu_reg_E_ADSM_DENOMIN_LOW_CNFG_REG_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG_BIT_DENOMIN_LOWER_MSK) >> ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG_BIT_DENOMIN_LOWER_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_DENOMIN_LOWER_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_DENOMIN_LOWER_set( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_DENOMIN_LOWER_set( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_DENOMIN_LOWER_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_DENOMIN_LOWER_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_DENOMIN_LOWER_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000048c bits 31:0) field DENOMIN_LOWER of register PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG */
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
        /* (0x0000048c bits 31:0) field DENOMIN_LOWER of register PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG */
        enh_8g_csu_reg_E_ADSM_DENOMIN_LOW_CNFG_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG_BIT_DENOMIN_LOWER_OFF + subfield_offset),
                                                              ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG_BIT_DENOMIN_LOWER_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_DENOMIN_LOWER_get( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_DENOMIN_LOWER_get( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_DENOMIN_LOWER_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_DENOMIN_LOWER_get", stop_bit, 31 );
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
    /* (0x0000048c bits 31:0) field DENOMIN_LOWER of register PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG */
    reg_value = enh_8g_csu_reg_E_ADSM_DENOMIN_LOW_CNFG_REG_read( b_ptr,
                                                                 h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG_BIT_DENOMIN_LOWER_MSK)
                  >> ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG_BIT_DENOMIN_LOWER_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG_BIT_DENOMIN_LOWER_MSK, ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG_BIT_DENOMIN_LOWER_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_DENOMIN_LOWER_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_DENOMIN_UPPER_set( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_DENOMIN_UPPER_set( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_DENOMIN_UPPER_set", value, 255);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_DENOMIN_UPPER_set", value );

    /* (0x00000490 bits 7:0) field DENOMIN_UPPER of register PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG */
    enh_8g_csu_reg_E_ADSM_DENOMIN_UP_CNFG_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG_BIT_DENOMIN_UPPER_MSK,
                                                         ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG_BIT_DENOMIN_UPPER_OFF,
                                                         value);
}

static INLINE UINT32 enh_8g_csu_field_DENOMIN_UPPER_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_DENOMIN_UPPER_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000490 bits 7:0) field DENOMIN_UPPER of register PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG */
    reg_value = enh_8g_csu_reg_E_ADSM_DENOMIN_UP_CNFG_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG_BIT_DENOMIN_UPPER_MSK) >> ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG_BIT_DENOMIN_UPPER_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_DENOMIN_UPPER_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_DENOMIN_UPPER_set( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_DENOMIN_UPPER_set( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_DENOMIN_UPPER_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_DENOMIN_UPPER_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_DENOMIN_UPPER_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000490 bits 7:0) field DENOMIN_UPPER of register PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG */
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
        /* (0x00000490 bits 7:0) field DENOMIN_UPPER of register PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG */
        enh_8g_csu_reg_E_ADSM_DENOMIN_UP_CNFG_REG_field_set( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG_BIT_DENOMIN_UPPER_OFF + subfield_offset),
                                                             ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG_BIT_DENOMIN_UPPER_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_DENOMIN_UPPER_get( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_DENOMIN_UPPER_get( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_DENOMIN_UPPER_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_DENOMIN_UPPER_get", stop_bit, 7 );
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
    /* (0x00000490 bits 7:0) field DENOMIN_UPPER of register PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG */
    reg_value = enh_8g_csu_reg_E_ADSM_DENOMIN_UP_CNFG_REG_read( b_ptr,
                                                                h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG_BIT_DENOMIN_UPPER_MSK)
                  >> ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG_BIT_DENOMIN_UPPER_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG_BIT_DENOMIN_UPPER_MSK, ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG_BIT_DENOMIN_UPPER_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_DENOMIN_UPPER_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_DITHER_SEL_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_DITHER_SEL_set( enh_8g_csu_buffer_t *b_ptr,
                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_DITHER_SEL_set", value, 3);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_DITHER_SEL_set", value );

    /* (0x00000494 bits 11:10) field DITHER_SEL of register PMC_ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG */
    enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_field_set( b_ptr,
                                                    h_ptr,
                                                    ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_DITHER_SEL_MSK,
                                                    ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_DITHER_SEL_OFF,
                                                    value);
}

static INLINE UINT32 enh_8g_csu_field_DITHER_SEL_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_DITHER_SEL_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000494 bits 11:10) field DITHER_SEL of register PMC_ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG */
    reg_value = enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_DITHER_SEL_MSK) >> ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_DITHER_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_DITHER_SEL_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_DITHER_SEL_set( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_DITHER_SEL_set( enh_8g_csu_buffer_t *b_ptr,
                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_DITHER_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_DITHER_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_DITHER_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000494 bits 11:10) field DITHER_SEL of register PMC_ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG */
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
        /* (0x00000494 bits 11:10) field DITHER_SEL of register PMC_ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG */
        enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_DITHER_SEL_OFF + subfield_offset),
                                                        ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_DITHER_SEL_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_DITHER_SEL_get( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_DITHER_SEL_get( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_DITHER_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_DITHER_SEL_get", stop_bit, 1 );
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
    /* (0x00000494 bits 11:10) field DITHER_SEL of register PMC_ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG */
    reg_value = enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_DITHER_SEL_MSK)
                  >> ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_DITHER_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_DITHER_SEL_MSK, ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_DITHER_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_DITHER_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_INT_DIV_set( enh_8g_csu_buffer_t *b_ptr,
                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_INT_DIV_set( enh_8g_csu_buffer_t *b_ptr,
                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_INT_DIV_set", value, 1023);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_INT_DIV_set", value );

    /* (0x00000494 bits 9:0) field INT_DIV of register PMC_ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG */
    enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_field_set( b_ptr,
                                                    h_ptr,
                                                    ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_INT_DIV_MSK,
                                                    ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_INT_DIV_OFF,
                                                    value);
}

static INLINE UINT32 enh_8g_csu_field_INT_DIV_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_INT_DIV_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000494 bits 9:0) field INT_DIV of register PMC_ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG */
    reg_value = enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_INT_DIV_MSK) >> ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_INT_DIV_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_INT_DIV_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_INT_DIV_set( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_INT_DIV_set( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_INT_DIV_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_INT_DIV_set", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_INT_DIV_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000494 bits 9:0) field INT_DIV of register PMC_ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG */
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
        /* (0x00000494 bits 9:0) field INT_DIV of register PMC_ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG */
        enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_INT_DIV_OFF + subfield_offset),
                                                        ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_INT_DIV_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_INT_DIV_get( enh_8g_csu_buffer_t *b_ptr,
                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_INT_DIV_get( enh_8g_csu_buffer_t *b_ptr,
                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_INT_DIV_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_INT_DIV_get", stop_bit, 9 );
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
    /* (0x00000494 bits 9:0) field INT_DIV of register PMC_ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG */
    reg_value = enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_INT_DIV_MSK)
                  >> ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_INT_DIV_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_INT_DIV_MSK, ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_INT_DIV_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_INT_DIV_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_FRAC_DIV_MOD_OVR_ENB_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_FRAC_DIV_MOD_OVR_ENB_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_FRAC_DIV_MOD_OVR_ENB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_FRAC_DIV_MOD_OVR_ENB_set", value );

    /* (0x00000498 bits 29) field FRAC_DIV_MOD_OVR_ENB of register PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG */
    enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_OVR_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_BIT_FRAC_DIV_MOD_OVR_ENB_MSK,
                                                         ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_BIT_FRAC_DIV_MOD_OVR_ENB_OFF,
                                                         value);
}

static INLINE UINT32 enh_8g_csu_field_FRAC_DIV_MOD_OVR_ENB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_FRAC_DIV_MOD_OVR_ENB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000498 bits 29) field FRAC_DIV_MOD_OVR_ENB of register PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG */
    reg_value = enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_OVR_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_BIT_FRAC_DIV_MOD_OVR_ENB_MSK) >> ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_BIT_FRAC_DIV_MOD_OVR_ENB_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_FRAC_DIV_MOD_OVR_ENB_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_FRAC_DIV_MOD_LOW_OVR_VAL_set( enh_8g_csu_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_FRAC_DIV_MOD_LOW_OVR_VAL_set( enh_8g_csu_buffer_t *b_ptr,
                                                                  sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 33554431)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_FRAC_DIV_MOD_LOW_OVR_VAL_set", value, 33554431);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_FRAC_DIV_MOD_LOW_OVR_VAL_set", value );

    /* (0x00000498 bits 24:0) field FRAC_DIV_MOD_LOW_OVR_VAL of register PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG */
    enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_OVR_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_BIT_FRAC_DIV_MOD_LOW_OVR_VAL_MSK,
                                                         ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_BIT_FRAC_DIV_MOD_LOW_OVR_VAL_OFF,
                                                         value);
}

static INLINE UINT32 enh_8g_csu_field_FRAC_DIV_MOD_LOW_OVR_VAL_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_FRAC_DIV_MOD_LOW_OVR_VAL_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000498 bits 24:0) field FRAC_DIV_MOD_LOW_OVR_VAL of register PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG */
    reg_value = enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_OVR_REG_read( b_ptr,
                                                                h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_BIT_FRAC_DIV_MOD_LOW_OVR_VAL_MSK) >> ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_BIT_FRAC_DIV_MOD_LOW_OVR_VAL_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_FRAC_DIV_MOD_LOW_OVR_VAL_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_FRAC_DIV_MOD_LOW_OVR_VAL_set( enh_8g_csu_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_FRAC_DIV_MOD_LOW_OVR_VAL_set( enh_8g_csu_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_FRAC_DIV_MOD_LOW_OVR_VAL_set", stop_bit, start_bit );
    if (stop_bit > 24) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_FRAC_DIV_MOD_LOW_OVR_VAL_set", stop_bit, 24 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_FRAC_DIV_MOD_LOW_OVR_VAL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000498 bits 24:0) field FRAC_DIV_MOD_LOW_OVR_VAL of register PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG */
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
        if (stop_bit < 24) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 24;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000498 bits 24:0) field FRAC_DIV_MOD_LOW_OVR_VAL of register PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG */
        enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_OVR_REG_field_set( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_BIT_FRAC_DIV_MOD_LOW_OVR_VAL_OFF + subfield_offset),
                                                             ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_BIT_FRAC_DIV_MOD_LOW_OVR_VAL_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_FRAC_DIV_MOD_LOW_OVR_VAL_get( enh_8g_csu_buffer_t *b_ptr,
                                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_FRAC_DIV_MOD_LOW_OVR_VAL_get( enh_8g_csu_buffer_t *b_ptr,
                                                                          sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_FRAC_DIV_MOD_LOW_OVR_VAL_get", stop_bit, start_bit );
    if (stop_bit > 24) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_FRAC_DIV_MOD_LOW_OVR_VAL_get", stop_bit, 24 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 24) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 24;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000498 bits 24:0) field FRAC_DIV_MOD_LOW_OVR_VAL of register PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG */
    reg_value = enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_OVR_REG_read( b_ptr,
                                                                h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_BIT_FRAC_DIV_MOD_LOW_OVR_VAL_MSK)
                  >> ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_BIT_FRAC_DIV_MOD_LOW_OVR_VAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_BIT_FRAC_DIV_MOD_LOW_OVR_VAL_MSK, ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_BIT_FRAC_DIV_MOD_LOW_OVR_VAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_FRAC_DIV_MOD_LOW_OVR_VAL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_FRAC_DIV_MOD_UP_OVR_VAL_set( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_FRAC_DIV_MOD_UP_OVR_VAL_set( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_FRAC_DIV_MOD_UP_OVR_VAL_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_FRAC_DIV_MOD_UP_OVR_VAL_set", value );

    /* (0x0000049c bits 15:0) field FRAC_DIV_MOD_UP_OVR_VAL of register PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG */
    enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_UP_OVR_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG_BIT_FRAC_DIV_MOD_UP_OVR_VAL_MSK,
                                                            ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG_BIT_FRAC_DIV_MOD_UP_OVR_VAL_OFF,
                                                            value);
}

static INLINE UINT32 enh_8g_csu_field_FRAC_DIV_MOD_UP_OVR_VAL_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_FRAC_DIV_MOD_UP_OVR_VAL_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000049c bits 15:0) field FRAC_DIV_MOD_UP_OVR_VAL of register PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG */
    reg_value = enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_UP_OVR_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG_BIT_FRAC_DIV_MOD_UP_OVR_VAL_MSK) >> ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG_BIT_FRAC_DIV_MOD_UP_OVR_VAL_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_FRAC_DIV_MOD_UP_OVR_VAL_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_FRAC_DIV_MOD_UP_OVR_VAL_set( enh_8g_csu_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_FRAC_DIV_MOD_UP_OVR_VAL_set( enh_8g_csu_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_FRAC_DIV_MOD_UP_OVR_VAL_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_FRAC_DIV_MOD_UP_OVR_VAL_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_FRAC_DIV_MOD_UP_OVR_VAL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000049c bits 15:0) field FRAC_DIV_MOD_UP_OVR_VAL of register PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG */
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
        /* (0x0000049c bits 15:0) field FRAC_DIV_MOD_UP_OVR_VAL of register PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG */
        enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_UP_OVR_REG_field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG_BIT_FRAC_DIV_MOD_UP_OVR_VAL_OFF + subfield_offset),
                                                                ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG_BIT_FRAC_DIV_MOD_UP_OVR_VAL_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_FRAC_DIV_MOD_UP_OVR_VAL_get( enh_8g_csu_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_FRAC_DIV_MOD_UP_OVR_VAL_get( enh_8g_csu_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_FRAC_DIV_MOD_UP_OVR_VAL_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_FRAC_DIV_MOD_UP_OVR_VAL_get", stop_bit, 15 );
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
    /* (0x0000049c bits 15:0) field FRAC_DIV_MOD_UP_OVR_VAL of register PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG */
    reg_value = enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_UP_OVR_REG_read( b_ptr,
                                                                   h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG_BIT_FRAC_DIV_MOD_UP_OVR_VAL_MSK)
                  >> ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG_BIT_FRAC_DIV_MOD_UP_OVR_VAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG_BIT_FRAC_DIV_MOD_UP_OVR_VAL_MSK, ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG_BIT_FRAC_DIV_MOD_UP_OVR_VAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_FRAC_DIV_MOD_UP_OVR_VAL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_DAC_DELAY_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_DAC_DELAY_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_DAC_DELAY_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_DAC_DELAY_set", value );

    /* (0x000004ac bits 29:18) field DAC_DELAY of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1 */
    enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_DAC_DELAY_MSK,
                                                     ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_DAC_DELAY_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_DAC_DELAY_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_DAC_DELAY_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004ac bits 29:18) field DAC_DELAY of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_DAC_DELAY_MSK) >> ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_DAC_DELAY_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_DAC_DELAY_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_DAC_DELAY_set( enh_8g_csu_buffer_t *b_ptr,
                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_DAC_DELAY_set( enh_8g_csu_buffer_t *b_ptr,
                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_DAC_DELAY_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_DAC_DELAY_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_DAC_DELAY_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000004ac bits 29:18) field DAC_DELAY of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1 */
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
        /* (0x000004ac bits 29:18) field DAC_DELAY of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1 */
        enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_DAC_DELAY_OFF + subfield_offset),
                                                         ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_DAC_DELAY_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_DAC_DELAY_get( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_DAC_DELAY_get( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_DAC_DELAY_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_DAC_DELAY_get", stop_bit, 11 );
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
    /* (0x000004ac bits 29:18) field DAC_DELAY of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_DAC_DELAY_MSK)
                  >> ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_DAC_DELAY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_DAC_DELAY_MSK, ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_DAC_DELAY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_DAC_DELAY_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_ACC_DELAY_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_ACC_DELAY_set( enh_8g_csu_buffer_t *b_ptr,
                                                   sfi51_d8_tx_18x_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_ACC_DELAY_set", value, 255);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_ACC_DELAY_set", value );

    /* (0x000004ac bits 17:10) field ACC_DELAY of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1 */
    enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_ACC_DELAY_MSK,
                                                     ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_ACC_DELAY_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_ACC_DELAY_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_ACC_DELAY_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004ac bits 17:10) field ACC_DELAY of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_ACC_DELAY_MSK) >> ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_ACC_DELAY_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_ACC_DELAY_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_ACC_DELAY_set( enh_8g_csu_buffer_t *b_ptr,
                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_ACC_DELAY_set( enh_8g_csu_buffer_t *b_ptr,
                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_ACC_DELAY_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_ACC_DELAY_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_ACC_DELAY_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000004ac bits 17:10) field ACC_DELAY of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1 */
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
        /* (0x000004ac bits 17:10) field ACC_DELAY of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1 */
        enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_ACC_DELAY_OFF + subfield_offset),
                                                         ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_ACC_DELAY_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_ACC_DELAY_get( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_ACC_DELAY_get( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_ACC_DELAY_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_ACC_DELAY_get", stop_bit, 7 );
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
    /* (0x000004ac bits 17:10) field ACC_DELAY of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_ACC_DELAY_MSK)
                  >> ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_ACC_DELAY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_ACC_DELAY_MSK, ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_ACC_DELAY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_ACC_DELAY_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_N_ACCUM_CYCLES_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_N_ACCUM_CYCLES_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_N_ACCUM_CYCLES_set", value, 255);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_N_ACCUM_CYCLES_set", value );

    /* (0x000004ac bits 9:2) field N_ACCUM_CYCLES of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1 */
    enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_N_ACCUM_CYCLES_MSK,
                                                     ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_N_ACCUM_CYCLES_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_N_ACCUM_CYCLES_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_N_ACCUM_CYCLES_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004ac bits 9:2) field N_ACCUM_CYCLES of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_N_ACCUM_CYCLES_MSK) >> ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_N_ACCUM_CYCLES_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_N_ACCUM_CYCLES_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_N_ACCUM_CYCLES_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_N_ACCUM_CYCLES_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_N_ACCUM_CYCLES_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_N_ACCUM_CYCLES_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_N_ACCUM_CYCLES_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000004ac bits 9:2) field N_ACCUM_CYCLES of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1 */
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
        /* (0x000004ac bits 9:2) field N_ACCUM_CYCLES of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1 */
        enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_N_ACCUM_CYCLES_OFF + subfield_offset),
                                                         ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_N_ACCUM_CYCLES_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_N_ACCUM_CYCLES_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_N_ACCUM_CYCLES_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_N_ACCUM_CYCLES_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_N_ACCUM_CYCLES_get", stop_bit, 7 );
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
    /* (0x000004ac bits 9:2) field N_ACCUM_CYCLES of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_N_ACCUM_CYCLES_MSK)
                  >> ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_N_ACCUM_CYCLES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_N_ACCUM_CYCLES_MSK, ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_N_ACCUM_CYCLES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_N_ACCUM_CYCLES_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_CALIB_PROG_EN_set( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CALIB_PROG_EN_set( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CALIB_PROG_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CALIB_PROG_EN_set", value );

    /* (0x000004ac bits 1) field CALIB_PROG_EN of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1 */
    enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_CALIB_PROG_EN_MSK,
                                                     ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_CALIB_PROG_EN_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_CALIB_PROG_EN_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CALIB_PROG_EN_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004ac bits 1) field CALIB_PROG_EN of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_CALIB_PROG_EN_MSK) >> ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_CALIB_PROG_EN_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CALIB_PROG_EN_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CALIB_set( enh_8g_csu_buffer_t *b_ptr,
                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CALIB_set( enh_8g_csu_buffer_t *b_ptr,
                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CALIB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CALIB_set", value );

    /* (0x000004ac bits 0) field CALIB of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1 */
    enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_CALIB_MSK,
                                                     ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_CALIB_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_CALIB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CALIB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004ac bits 0) field CALIB of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_CALIB_MSK) >> ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_CALIB_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CALIB_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_REF_DCD_INV_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_REF_DCD_INV_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_REF_DCD_INV_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_REF_DCD_INV_set", value );

    /* (0x000004b0 bits 8) field REF_DCD_INV of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2 */
    enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_REF_DCD_INV_MSK,
                                                     ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_REF_DCD_INV_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_REF_DCD_INV_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_REF_DCD_INV_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004b0 bits 8) field REF_DCD_INV of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2 */
    reg_value = enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_REF_DCD_INV_MSK) >> ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_REF_DCD_INV_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_REF_DCD_INV_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_DBL_CTRL_BIT5_INV_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_DBL_CTRL_BIT5_INV_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_DBL_CTRL_BIT5_INV_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_DBL_CTRL_BIT5_INV_set", value );

    /* (0x000004b0 bits 7) field DBL_CTRL_BIT5_INV of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2 */
    enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_BIT5_INV_MSK,
                                                     ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_BIT5_INV_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_DBL_CTRL_BIT5_INV_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_DBL_CTRL_BIT5_INV_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004b0 bits 7) field DBL_CTRL_BIT5_INV of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2 */
    reg_value = enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_BIT5_INV_MSK) >> ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_BIT5_INV_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_DBL_CTRL_BIT5_INV_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_DBL_CTRL_OVR_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_DBL_CTRL_OVR_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_DBL_CTRL_OVR_set", value, 63);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_DBL_CTRL_OVR_set", value );

    /* (0x000004b0 bits 6:1) field DBL_CTRL_OVR of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2 */
    enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_OVR_MSK,
                                                     ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_OVR_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_DBL_CTRL_OVR_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_DBL_CTRL_OVR_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004b0 bits 6:1) field DBL_CTRL_OVR of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2 */
    reg_value = enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_OVR_MSK) >> ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_OVR_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_DBL_CTRL_OVR_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_DBL_CTRL_OVR_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_DBL_CTRL_OVR_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_DBL_CTRL_OVR_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_DBL_CTRL_OVR_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_DBL_CTRL_OVR_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000004b0 bits 6:1) field DBL_CTRL_OVR of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2 */
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
        /* (0x000004b0 bits 6:1) field DBL_CTRL_OVR of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2 */
        enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_OVR_OFF + subfield_offset),
                                                         ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_OVR_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_DBL_CTRL_OVR_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_DBL_CTRL_OVR_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_DBL_CTRL_OVR_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_DBL_CTRL_OVR_get", stop_bit, 5 );
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
    /* (0x000004b0 bits 6:1) field DBL_CTRL_OVR of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2 */
    reg_value = enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_OVR_MSK)
                  >> ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_OVR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_OVR_MSK, ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_OVR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_DBL_CTRL_OVR_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void enh_8g_csu_field_DBL_CTRL_OVR_EN_set( enh_8g_csu_buffer_t *b_ptr,
                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_DBL_CTRL_OVR_EN_set( enh_8g_csu_buffer_t *b_ptr,
                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_DBL_CTRL_OVR_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_DBL_CTRL_OVR_EN_set", value );

    /* (0x000004b0 bits 0) field DBL_CTRL_OVR_EN of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2 */
    enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_field_set( b_ptr,
                                                     h_ptr,
                                                     ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_OVR_EN_MSK,
                                                     ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_OVR_EN_OFF,
                                                     value);
}

static INLINE UINT32 enh_8g_csu_field_DBL_CTRL_OVR_EN_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_DBL_CTRL_OVR_EN_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004b0 bits 0) field DBL_CTRL_OVR_EN of register PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2 */
    reg_value = enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_OVR_EN_MSK) >> ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_OVR_EN_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_DBL_CTRL_OVR_EN_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_PLL_S_OUT_FRAC_DIV_MOD_SEL_set( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_PLL_S_OUT_FRAC_DIV_MOD_SEL_set( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_PLL_S_OUT_FRAC_DIV_MOD_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_PLL_S_OUT_FRAC_DIV_MOD_SEL_set", value );

    /* (0x000004b4 bits 23) field PLL_S_OUT_FRAC_DIV_MOD_SEL of register PMC_ENH_8G_CSU_REG_ACB_CNFG_REG_1 */
    enh_8g_csu_reg_ACB_CNFG_REG_1_field_set( b_ptr,
                                             h_ptr,
                                             ENH_8G_CSU_REG_ACB_CNFG_REG_1_BIT_PLL_S_OUT_FRAC_DIV_MOD_SEL_MSK,
                                             ENH_8G_CSU_REG_ACB_CNFG_REG_1_BIT_PLL_S_OUT_FRAC_DIV_MOD_SEL_OFF,
                                             value);
}

static INLINE UINT32 enh_8g_csu_field_PLL_S_OUT_FRAC_DIV_MOD_SEL_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_PLL_S_OUT_FRAC_DIV_MOD_SEL_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004b4 bits 23) field PLL_S_OUT_FRAC_DIV_MOD_SEL of register PMC_ENH_8G_CSU_REG_ACB_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_ACB_CNFG_REG_1_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_ACB_CNFG_REG_1_BIT_PLL_S_OUT_FRAC_DIV_MOD_SEL_MSK) >> ENH_8G_CSU_REG_ACB_CNFG_REG_1_BIT_PLL_S_OUT_FRAC_DIV_MOD_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_PLL_S_OUT_FRAC_DIV_MOD_SEL_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_ACB_CLK_FACT_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_ACB_CLK_FACT_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_ACB_CLK_FACT_set", value, 15);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_ACB_CLK_FACT_set", value );

    /* (0x000004b4 bits 22:19) field ACB_CLK_FACT of register PMC_ENH_8G_CSU_REG_ACB_CNFG_REG_1 */
    enh_8g_csu_reg_ACB_CNFG_REG_1_field_set( b_ptr,
                                             h_ptr,
                                             ENH_8G_CSU_REG_ACB_CNFG_REG_1_BIT_ACB_CLK_FACT_MSK,
                                             ENH_8G_CSU_REG_ACB_CNFG_REG_1_BIT_ACB_CLK_FACT_OFF,
                                             value);
}

static INLINE UINT32 enh_8g_csu_field_ACB_CLK_FACT_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_ACB_CLK_FACT_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004b4 bits 22:19) field ACB_CLK_FACT of register PMC_ENH_8G_CSU_REG_ACB_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_ACB_CNFG_REG_1_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_ACB_CNFG_REG_1_BIT_ACB_CLK_FACT_MSK) >> ENH_8G_CSU_REG_ACB_CNFG_REG_1_BIT_ACB_CLK_FACT_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_ACB_CLK_FACT_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_range_ACB_CLK_FACT_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_range_ACB_CLK_FACT_set( enh_8g_csu_buffer_t *b_ptr,
                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_ACB_CLK_FACT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_ACB_CLK_FACT_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_ACB_CLK_FACT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000004b4 bits 22:19) field ACB_CLK_FACT of register PMC_ENH_8G_CSU_REG_ACB_CNFG_REG_1 */
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
        /* (0x000004b4 bits 22:19) field ACB_CLK_FACT of register PMC_ENH_8G_CSU_REG_ACB_CNFG_REG_1 */
        enh_8g_csu_reg_ACB_CNFG_REG_1_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (ENH_8G_CSU_REG_ACB_CNFG_REG_1_BIT_ACB_CLK_FACT_OFF + subfield_offset),
                                                 ENH_8G_CSU_REG_ACB_CNFG_REG_1_BIT_ACB_CLK_FACT_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 enh_8g_csu_field_range_ACB_CLK_FACT_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_ACB_CLK_FACT_get( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_ACB_CLK_FACT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_ACB_CLK_FACT_get", stop_bit, 3 );
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
    /* (0x000004b4 bits 22:19) field ACB_CLK_FACT of register PMC_ENH_8G_CSU_REG_ACB_CNFG_REG_1 */
    reg_value = enh_8g_csu_reg_ACB_CNFG_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_ACB_CNFG_REG_1_BIT_ACB_CLK_FACT_MSK)
                  >> ENH_8G_CSU_REG_ACB_CNFG_REG_1_BIT_ACB_CLK_FACT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_ACB_CNFG_REG_1_BIT_ACB_CLK_FACT_MSK, ENH_8G_CSU_REG_ACB_CNFG_REG_1_BIT_ACB_CLK_FACT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_ACB_CLK_FACT_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void enh_8g_csu_field_PPM_OFF_RD_REQ_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_PPM_OFF_RD_REQ_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_PPM_OFF_RD_REQ_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_PPM_OFF_RD_REQ_set", value );

    /* (0x00000410 bits 5) field PPM_OFF_RD_REQ of register PMC_ENH_8G_CSU_REG_PLL_CNFG_REG1 */
    enh_8g_csu_reg_PLL_CNFG_REG1_field_set( b_ptr,
                                            h_ptr,
                                            ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_PPM_OFF_RD_REQ_MSK,
                                            ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_PPM_OFF_RD_REQ_OFF,
                                            value);
}

static INLINE UINT32 enh_8g_csu_field_PPM_OFF_RD_REQ_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_PPM_OFF_RD_REQ_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000410 bits 5) field PPM_OFF_RD_REQ of register PMC_ENH_8G_CSU_REG_PLL_CNFG_REG1 */
    reg_value = enh_8g_csu_reg_PLL_CNFG_REG1_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_PPM_OFF_RD_REQ_MSK) >> ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_PPM_OFF_RD_REQ_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_PPM_OFF_RD_REQ_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_both
 * ==================================================================================
 */
static INLINE void enh_8g_csu_field_PPM_OFFSET_UPD_I8_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_PPM_OFFSET_UPD_I8_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_PPM_OFFSET_UPD_I8_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_PPM_OFFSET_UPD_I8_set_to_clear", value );

    /* (0x000004a0 bits 8) field PPM_OFFSET_UPD_I8 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_PPM_OFFSET_UPD_I8_MSK,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_PPM_OFFSET_UPD_I8_OFF,
                                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_PPM_OFFSET_UPD_I8_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_PPM_OFFSET_UPD_I8_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a0 bits 8) field PPM_OFFSET_UPD_I8 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_PPM_OFFSET_UPD_I8_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_PPM_OFFSET_UPD_I8_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_PPM_OFFSET_UPD_I8_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_PPM_OFFSET_UPD_I8_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_PPM_OFFSET_UPD_I8_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_PPM_OFFSET_UPD_I8_poll", value );

    /* (0x000004a0 bits 8) field PPM_OFFSET_UPD_I8 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_poll( b_ptr,
                                                    h_ptr,
                                                    ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_PPM_OFFSET_UPD_I8_MSK,
                                                    (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_PPM_OFFSET_UPD_I8_OFF),
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
static INLINE void enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_I12_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_I12_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_I12_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_I12_set_to_clear", value );

    /* (0x000004a0 bits 12) field OC_THRESHOLD_CHECK_FAILB_I12 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_OC_THRESHOLD_CHECK_FAILB_I12_MSK,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_OC_THRESHOLD_CHECK_FAILB_I12_OFF,
                                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_I12_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_I12_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a0 bits 12) field OC_THRESHOLD_CHECK_FAILB_I12 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_OC_THRESHOLD_CHECK_FAILB_I12_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_OC_THRESHOLD_CHECK_FAILB_I12_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_I12_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_I12_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                       UINT32 value,
                                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                                       UINT32 max_count,
                                                                                       UINT32 *num_failed_polls,
                                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_I12_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                       UINT32 value,
                                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                                       UINT32 max_count,
                                                                                       UINT32 *num_failed_polls,
                                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_I12_poll", value );

    /* (0x000004a0 bits 12) field OC_THRESHOLD_CHECK_FAILB_I12 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_poll( b_ptr,
                                                    h_ptr,
                                                    ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_OC_THRESHOLD_CHECK_FAILB_I12_MSK,
                                                    (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_OC_THRESHOLD_CHECK_FAILB_I12_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE void enh_8g_csu_field_VCOC_MODE_IN_RANGE_I11_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_VCOC_MODE_IN_RANGE_I11_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_VCOC_MODE_IN_RANGE_I11_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_VCOC_MODE_IN_RANGE_I11_set_to_clear", value );

    /* (0x000004a0 bits 11) field VCOC_MODE_IN_RANGE_I11 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_MODE_IN_RANGE_I11_MSK,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_MODE_IN_RANGE_I11_OFF,
                                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_VCOC_MODE_IN_RANGE_I11_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCOC_MODE_IN_RANGE_I11_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a0 bits 11) field VCOC_MODE_IN_RANGE_I11 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_MODE_IN_RANGE_I11_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_MODE_IN_RANGE_I11_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCOC_MODE_IN_RANGE_I11_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_VCOC_MODE_IN_RANGE_I11_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_VCOC_MODE_IN_RANGE_I11_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_VCOC_MODE_IN_RANGE_I11_poll", value );

    /* (0x000004a0 bits 11) field VCOC_MODE_IN_RANGE_I11 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_poll( b_ptr,
                                                    h_ptr,
                                                    ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_MODE_IN_RANGE_I11_MSK,
                                                    (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_MODE_IN_RANGE_I11_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE void enh_8g_csu_field_VCOC_BIST_ERR_I10_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_VCOC_BIST_ERR_I10_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                                    sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_VCOC_BIST_ERR_I10_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_VCOC_BIST_ERR_I10_set_to_clear", value );

    /* (0x000004a0 bits 10) field VCOC_BIST_ERR_I10 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_BIST_ERR_I10_MSK,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_BIST_ERR_I10_OFF,
                                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_VCOC_BIST_ERR_I10_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCOC_BIST_ERR_I10_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a0 bits 10) field VCOC_BIST_ERR_I10 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_BIST_ERR_I10_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_BIST_ERR_I10_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCOC_BIST_ERR_I10_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_VCOC_BIST_ERR_I10_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_VCOC_BIST_ERR_I10_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_VCOC_BIST_ERR_I10_poll", value );

    /* (0x000004a0 bits 10) field VCOC_BIST_ERR_I10 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_poll( b_ptr,
                                                    h_ptr,
                                                    ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_BIST_ERR_I10_MSK,
                                                    (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_BIST_ERR_I10_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE void enh_8g_csu_field_DIVCLK_DEAD_I7_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_DIVCLK_DEAD_I7_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_DIVCLK_DEAD_I7_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_DIVCLK_DEAD_I7_set_to_clear", value );

    /* (0x000004a0 bits 7) field DIVCLK_DEAD_I7 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_DIVCLK_DEAD_I7_MSK,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_DIVCLK_DEAD_I7_OFF,
                                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_DIVCLK_DEAD_I7_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_DIVCLK_DEAD_I7_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a0 bits 7) field DIVCLK_DEAD_I7 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_DIVCLK_DEAD_I7_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_DIVCLK_DEAD_I7_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_DIVCLK_DEAD_I7_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_DIVCLK_DEAD_I7_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_DIVCLK_DEAD_I7_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_DIVCLK_DEAD_I7_poll", value );

    /* (0x000004a0 bits 7) field DIVCLK_DEAD_I7 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_poll( b_ptr,
                                                    h_ptr,
                                                    ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_DIVCLK_DEAD_I7_MSK,
                                                    (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_DIVCLK_DEAD_I7_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE void enh_8g_csu_field_CALIB_DONE_I6_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CALIB_DONE_I6_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CALIB_DONE_I6_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CALIB_DONE_I6_set_to_clear", value );

    /* (0x000004a0 bits 6) field CALIB_DONE_I6 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_CALIB_DONE_I6_MSK,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_CALIB_DONE_I6_OFF,
                                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_CALIB_DONE_I6_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CALIB_DONE_I6_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a0 bits 6) field CALIB_DONE_I6 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_CALIB_DONE_I6_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_CALIB_DONE_I6_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CALIB_DONE_I6_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_CALIB_DONE_I6_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_CALIB_DONE_I6_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_CALIB_DONE_I6_poll", value );

    /* (0x000004a0 bits 6) field CALIB_DONE_I6 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_poll( b_ptr,
                                                    h_ptr,
                                                    ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_CALIB_DONE_I6_MSK,
                                                    (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_CALIB_DONE_I6_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE void enh_8g_csu_field_OCC_DONE_I5_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_OCC_DONE_I5_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_OCC_DONE_I5_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_OCC_DONE_I5_set_to_clear", value );

    /* (0x000004a0 bits 5) field OCC_DONE_I5 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_OCC_DONE_I5_MSK,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_OCC_DONE_I5_OFF,
                                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_OCC_DONE_I5_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OCC_DONE_I5_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a0 bits 5) field OCC_DONE_I5 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_OCC_DONE_I5_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_OCC_DONE_I5_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OCC_DONE_I5_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_OCC_DONE_I5_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_OCC_DONE_I5_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_OCC_DONE_I5_poll", value );

    /* (0x000004a0 bits 5) field OCC_DONE_I5 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_poll( b_ptr,
                                                    h_ptr,
                                                    ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_OCC_DONE_I5_MSK,
                                                    (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_OCC_DONE_I5_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE void enh_8g_csu_field_LOCK_LOSS_I4_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_LOCK_LOSS_I4_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_LOCK_LOSS_I4_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_LOCK_LOSS_I4_set_to_clear", value );

    /* (0x000004a0 bits 4) field LOCK_LOSS_I4 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_LOCK_LOSS_I4_MSK,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_LOCK_LOSS_I4_OFF,
                                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_LOCK_LOSS_I4_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_LOCK_LOSS_I4_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a0 bits 4) field LOCK_LOSS_I4 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_LOCK_LOSS_I4_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_LOCK_LOSS_I4_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_LOCK_LOSS_I4_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_LOCK_LOSS_I4_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_LOCK_LOSS_I4_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_LOCK_LOSS_I4_poll", value );

    /* (0x000004a0 bits 4) field LOCK_LOSS_I4 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_poll( b_ptr,
                                                    h_ptr,
                                                    ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_LOCK_LOSS_I4_MSK,
                                                    (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_LOCK_LOSS_I4_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE void enh_8g_csu_field_LOCK_GAIN_I3_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_LOCK_GAIN_I3_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_LOCK_GAIN_I3_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_LOCK_GAIN_I3_set_to_clear", value );

    /* (0x000004a0 bits 3) field LOCK_GAIN_I3 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_LOCK_GAIN_I3_MSK,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_LOCK_GAIN_I3_OFF,
                                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_LOCK_GAIN_I3_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_LOCK_GAIN_I3_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a0 bits 3) field LOCK_GAIN_I3 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_LOCK_GAIN_I3_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_LOCK_GAIN_I3_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_LOCK_GAIN_I3_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_LOCK_GAIN_I3_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_LOCK_GAIN_I3_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_LOCK_GAIN_I3_poll", value );

    /* (0x000004a0 bits 3) field LOCK_GAIN_I3 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_poll( b_ptr,
                                                    h_ptr,
                                                    ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_LOCK_GAIN_I3_MSK,
                                                    (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_LOCK_GAIN_I3_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE void enh_8g_csu_field_VCOC_DONE_I2_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_VCOC_DONE_I2_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_VCOC_DONE_I2_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_VCOC_DONE_I2_set_to_clear", value );

    /* (0x000004a0 bits 2) field VCOC_DONE_I2 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_DONE_I2_MSK,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_DONE_I2_OFF,
                                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_VCOC_DONE_I2_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCOC_DONE_I2_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a0 bits 2) field VCOC_DONE_I2 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_DONE_I2_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_DONE_I2_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCOC_DONE_I2_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_VCOC_DONE_I2_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_VCOC_DONE_I2_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_VCOC_DONE_I2_poll", value );

    /* (0x000004a0 bits 2) field VCOC_DONE_I2 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_poll( b_ptr,
                                                    h_ptr,
                                                    ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_DONE_I2_MSK,
                                                    (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_DONE_I2_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE void enh_8g_csu_field_IN_RANGE_N_FAILS_I1_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_IN_RANGE_N_FAILS_I1_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_IN_RANGE_N_FAILS_I1_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_IN_RANGE_N_FAILS_I1_set_to_clear", value );

    /* (0x000004a0 bits 1) field IN_RANGE_N_FAILS_I1 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_IN_RANGE_N_FAILS_I1_MSK,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_IN_RANGE_N_FAILS_I1_OFF,
                                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_IN_RANGE_N_FAILS_I1_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_IN_RANGE_N_FAILS_I1_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a0 bits 1) field IN_RANGE_N_FAILS_I1 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_IN_RANGE_N_FAILS_I1_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_IN_RANGE_N_FAILS_I1_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_IN_RANGE_N_FAILS_I1_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_IN_RANGE_N_FAILS_I1_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_IN_RANGE_N_FAILS_I1_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_IN_RANGE_N_FAILS_I1_poll", value );

    /* (0x000004a0 bits 1) field IN_RANGE_N_FAILS_I1 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_poll( b_ptr,
                                                    h_ptr,
                                                    ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_IN_RANGE_N_FAILS_I1_MSK,
                                                    (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_IN_RANGE_N_FAILS_I1_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE void enh_8g_csu_field_IN_RANGE_N_PASSES_I0_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_IN_RANGE_N_PASSES_I0_set_to_clear( enh_8g_csu_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_IN_RANGE_N_PASSES_I0_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_IN_RANGE_N_PASSES_I0_set_to_clear", value );

    /* (0x000004a0 bits 0) field IN_RANGE_N_PASSES_I0 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_IN_RANGE_N_PASSES_I0_MSK,
                                                                  ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_IN_RANGE_N_PASSES_I0_OFF,
                                                                  value);
}

static INLINE UINT32 enh_8g_csu_field_IN_RANGE_N_PASSES_I0_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_IN_RANGE_N_PASSES_I0_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a0 bits 0) field IN_RANGE_N_PASSES_I0 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_IN_RANGE_N_PASSES_I0_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_IN_RANGE_N_PASSES_I0_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_IN_RANGE_N_PASSES_I0_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_IN_RANGE_N_PASSES_I0_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_IN_RANGE_N_PASSES_I0_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_IN_RANGE_N_PASSES_I0_poll", value );

    /* (0x000004a0 bits 0) field IN_RANGE_N_PASSES_I0 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_poll( b_ptr,
                                                    h_ptr,
                                                    ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_IN_RANGE_N_PASSES_I0_MSK,
                                                    (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_IN_RANGE_N_PASSES_I0_OFF),
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
static INLINE void enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_E12_set( enh_8g_csu_buffer_t *b_ptr,
                                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_E12_set( enh_8g_csu_buffer_t *b_ptr,
                                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_E12_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_E12_set", value );

    /* (0x000004a4 bits 12) field OC_THRESHOLD_CHECK_FAILB_E12 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_OC_THRESHOLD_CHECK_FAILB_E12_MSK,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_OC_THRESHOLD_CHECK_FAILB_E12_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_E12_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_E12_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a4 bits 12) field OC_THRESHOLD_CHECK_FAILB_E12 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_OC_THRESHOLD_CHECK_FAILB_E12_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_OC_THRESHOLD_CHECK_FAILB_E12_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_E12_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_VCOC_MODE_IN_RANGE_E11_set( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_VCOC_MODE_IN_RANGE_E11_set( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_VCOC_MODE_IN_RANGE_E11_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_VCOC_MODE_IN_RANGE_E11_set", value );

    /* (0x000004a4 bits 11) field VCOC_MODE_IN_RANGE_E11 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_VCOC_MODE_IN_RANGE_E11_MSK,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_VCOC_MODE_IN_RANGE_E11_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_VCOC_MODE_IN_RANGE_E11_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCOC_MODE_IN_RANGE_E11_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a4 bits 11) field VCOC_MODE_IN_RANGE_E11 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_VCOC_MODE_IN_RANGE_E11_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_VCOC_MODE_IN_RANGE_E11_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCOC_MODE_IN_RANGE_E11_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_VCOC_BIST_ERR_E10_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_VCOC_BIST_ERR_E10_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_VCOC_BIST_ERR_E10_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_VCOC_BIST_ERR_E10_set", value );

    /* (0x000004a4 bits 10) field VCOC_BIST_ERR_E10 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_VCOC_BIST_ERR_E10_MSK,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_VCOC_BIST_ERR_E10_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_VCOC_BIST_ERR_E10_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCOC_BIST_ERR_E10_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a4 bits 10) field VCOC_BIST_ERR_E10 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_VCOC_BIST_ERR_E10_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_VCOC_BIST_ERR_E10_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCOC_BIST_ERR_E10_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_PPM_OFFSET_UPD_E8_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_PPM_OFFSET_UPD_E8_set( enh_8g_csu_buffer_t *b_ptr,
                                                           sfi51_d8_tx_18x_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_PPM_OFFSET_UPD_E8_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_PPM_OFFSET_UPD_E8_set", value );

    /* (0x000004a4 bits 8) field PPM_OFFSET_UPD_E8 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_PPM_OFFSET_UPD_E8_MSK,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_PPM_OFFSET_UPD_E8_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_PPM_OFFSET_UPD_E8_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_PPM_OFFSET_UPD_E8_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a4 bits 8) field PPM_OFFSET_UPD_E8 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_PPM_OFFSET_UPD_E8_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_PPM_OFFSET_UPD_E8_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_PPM_OFFSET_UPD_E8_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_DIVCLK_DEAD_E7_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_DIVCLK_DEAD_E7_set( enh_8g_csu_buffer_t *b_ptr,
                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_DIVCLK_DEAD_E7_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_DIVCLK_DEAD_E7_set", value );

    /* (0x000004a4 bits 7) field DIVCLK_DEAD_E7 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_DIVCLK_DEAD_E7_MSK,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_DIVCLK_DEAD_E7_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_DIVCLK_DEAD_E7_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_DIVCLK_DEAD_E7_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a4 bits 7) field DIVCLK_DEAD_E7 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_DIVCLK_DEAD_E7_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_DIVCLK_DEAD_E7_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_DIVCLK_DEAD_E7_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_CALIB_DONE_E6_set( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_CALIB_DONE_E6_set( enh_8g_csu_buffer_t *b_ptr,
                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_CALIB_DONE_E6_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_CALIB_DONE_E6_set", value );

    /* (0x000004a4 bits 6) field CALIB_DONE_E6 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_CALIB_DONE_E6_MSK,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_CALIB_DONE_E6_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_CALIB_DONE_E6_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CALIB_DONE_E6_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a4 bits 6) field CALIB_DONE_E6 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_CALIB_DONE_E6_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_CALIB_DONE_E6_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CALIB_DONE_E6_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_OCC_DONE_E5_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_OCC_DONE_E5_set( enh_8g_csu_buffer_t *b_ptr,
                                                     sfi51_d8_tx_18x_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_OCC_DONE_E5_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_OCC_DONE_E5_set", value );

    /* (0x000004a4 bits 5) field OCC_DONE_E5 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_OCC_DONE_E5_MSK,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_OCC_DONE_E5_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_OCC_DONE_E5_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OCC_DONE_E5_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a4 bits 5) field OCC_DONE_E5 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_OCC_DONE_E5_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_OCC_DONE_E5_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OCC_DONE_E5_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_LOCK_LOSS_E4_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_LOCK_LOSS_E4_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_LOCK_LOSS_E4_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_LOCK_LOSS_E4_set", value );

    /* (0x000004a4 bits 4) field LOCK_LOSS_E4 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_LOCK_LOSS_E4_MSK,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_LOCK_LOSS_E4_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_LOCK_LOSS_E4_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_LOCK_LOSS_E4_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a4 bits 4) field LOCK_LOSS_E4 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_LOCK_LOSS_E4_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_LOCK_LOSS_E4_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_LOCK_LOSS_E4_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_LOCK_GAIN_E3_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_LOCK_GAIN_E3_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_LOCK_GAIN_E3_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_LOCK_GAIN_E3_set", value );

    /* (0x000004a4 bits 3) field LOCK_GAIN_E3 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_LOCK_GAIN_E3_MSK,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_LOCK_GAIN_E3_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_LOCK_GAIN_E3_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_LOCK_GAIN_E3_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a4 bits 3) field LOCK_GAIN_E3 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_LOCK_GAIN_E3_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_LOCK_GAIN_E3_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_LOCK_GAIN_E3_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_VCOC_DONE_E2_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_VCOC_DONE_E2_set( enh_8g_csu_buffer_t *b_ptr,
                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_VCOC_DONE_E2_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_VCOC_DONE_E2_set", value );

    /* (0x000004a4 bits 2) field VCOC_DONE_E2 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_VCOC_DONE_E2_MSK,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_VCOC_DONE_E2_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_VCOC_DONE_E2_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCOC_DONE_E2_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a4 bits 2) field VCOC_DONE_E2 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_VCOC_DONE_E2_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_VCOC_DONE_E2_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCOC_DONE_E2_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_IN_RANGE_N_FAILS_E1_set( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_IN_RANGE_N_FAILS_E1_set( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_IN_RANGE_N_FAILS_E1_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_IN_RANGE_N_FAILS_E1_set", value );

    /* (0x000004a4 bits 1) field IN_RANGE_N_FAILS_E1 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_IN_RANGE_N_FAILS_E1_MSK,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_IN_RANGE_N_FAILS_E1_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_IN_RANGE_N_FAILS_E1_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_IN_RANGE_N_FAILS_E1_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a4 bits 1) field IN_RANGE_N_FAILS_E1 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_IN_RANGE_N_FAILS_E1_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_IN_RANGE_N_FAILS_E1_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_IN_RANGE_N_FAILS_E1_get", value );

    return value;
}
static INLINE void enh_8g_csu_field_IN_RANGE_N_PASSES_E0_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void enh_8g_csu_field_IN_RANGE_N_PASSES_E0_set( enh_8g_csu_buffer_t *b_ptr,
                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "enh_8g_csu_field_IN_RANGE_N_PASSES_E0_set", value, 1);
    IOLOG( "%s <= 0x%08x", "enh_8g_csu_field_IN_RANGE_N_PASSES_E0_set", value );

    /* (0x000004a4 bits 0) field IN_RANGE_N_PASSES_E0 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_IN_RANGE_N_PASSES_E0_MSK,
                                                      ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_IN_RANGE_N_PASSES_E0_OFF,
                                                      value);
}

static INLINE UINT32 enh_8g_csu_field_IN_RANGE_N_PASSES_E0_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_IN_RANGE_N_PASSES_E0_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a4 bits 0) field IN_RANGE_N_PASSES_E0 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_IN_RANGE_N_PASSES_E0_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_IN_RANGE_N_PASSES_E0_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_IN_RANGE_N_PASSES_E0_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value
 * ==================================================================================
 */
static INLINE UINT32 enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_V12_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_V12_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a8 bits 12) field OC_THRESHOLD_CHECK_FAILB_V12 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_OC_THRESHOLD_CHECK_FAILB_V12_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_OC_THRESHOLD_CHECK_FAILB_V12_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_V12_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_V12_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                       UINT32 value,
                                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                                       UINT32 max_count,
                                                                                       UINT32 *num_failed_polls,
                                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_V12_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                       UINT32 value,
                                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                                       UINT32 max_count,
                                                                                       UINT32 *num_failed_polls,
                                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_V12_poll", value );

    /* (0x000004a8 bits 12) field OC_THRESHOLD_CHECK_FAILB_V12 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_poll( b_ptr,
                                                        h_ptr,
                                                        ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_OC_THRESHOLD_CHECK_FAILB_V12_MSK,
                                                        (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_OC_THRESHOLD_CHECK_FAILB_V12_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 enh_8g_csu_field_VCOC_MODE_IN_RANGE_V11_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCOC_MODE_IN_RANGE_V11_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a8 bits 11) field VCOC_MODE_IN_RANGE_V11 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_VCOC_MODE_IN_RANGE_V11_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_VCOC_MODE_IN_RANGE_V11_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCOC_MODE_IN_RANGE_V11_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_VCOC_MODE_IN_RANGE_V11_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_VCOC_MODE_IN_RANGE_V11_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                                 sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_VCOC_MODE_IN_RANGE_V11_poll", value );

    /* (0x000004a8 bits 11) field VCOC_MODE_IN_RANGE_V11 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_poll( b_ptr,
                                                        h_ptr,
                                                        ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_VCOC_MODE_IN_RANGE_V11_MSK,
                                                        (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_VCOC_MODE_IN_RANGE_V11_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 enh_8g_csu_field_VCOC_BIST_ERR_V10_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCOC_BIST_ERR_V10_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a8 bits 10) field VCOC_BIST_ERR_V10 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_VCOC_BIST_ERR_V10_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_VCOC_BIST_ERR_V10_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCOC_BIST_ERR_V10_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_VCOC_BIST_ERR_V10_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_VCOC_BIST_ERR_V10_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_VCOC_BIST_ERR_V10_poll", value );

    /* (0x000004a8 bits 10) field VCOC_BIST_ERR_V10 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_poll( b_ptr,
                                                        h_ptr,
                                                        ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_VCOC_BIST_ERR_V10_MSK,
                                                        (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_VCOC_BIST_ERR_V10_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 enh_8g_csu_field_PPM_OFFSET_UPD_V8_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_PPM_OFFSET_UPD_V8_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a8 bits 8) field PPM_OFFSET_UPD_V8 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_PPM_OFFSET_UPD_V8_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_PPM_OFFSET_UPD_V8_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_PPM_OFFSET_UPD_V8_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_PPM_OFFSET_UPD_V8_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_PPM_OFFSET_UPD_V8_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                            sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_PPM_OFFSET_UPD_V8_poll", value );

    /* (0x000004a8 bits 8) field PPM_OFFSET_UPD_V8 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_poll( b_ptr,
                                                        h_ptr,
                                                        ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_PPM_OFFSET_UPD_V8_MSK,
                                                        (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_PPM_OFFSET_UPD_V8_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 enh_8g_csu_field_DIVCLK_DEAD_V7_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_DIVCLK_DEAD_V7_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a8 bits 7) field DIVCLK_DEAD_V7 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_DIVCLK_DEAD_V7_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_DIVCLK_DEAD_V7_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_DIVCLK_DEAD_V7_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_DIVCLK_DEAD_V7_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_DIVCLK_DEAD_V7_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                         sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_DIVCLK_DEAD_V7_poll", value );

    /* (0x000004a8 bits 7) field DIVCLK_DEAD_V7 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_poll( b_ptr,
                                                        h_ptr,
                                                        ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_DIVCLK_DEAD_V7_MSK,
                                                        (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_DIVCLK_DEAD_V7_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 enh_8g_csu_field_CALIB_DONE_V6_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_CALIB_DONE_V6_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a8 bits 6) field CALIB_DONE_V6 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_CALIB_DONE_V6_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_CALIB_DONE_V6_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_CALIB_DONE_V6_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_CALIB_DONE_V6_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_CALIB_DONE_V6_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                        sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_CALIB_DONE_V6_poll", value );

    /* (0x000004a8 bits 6) field CALIB_DONE_V6 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_poll( b_ptr,
                                                        h_ptr,
                                                        ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_CALIB_DONE_V6_MSK,
                                                        (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_CALIB_DONE_V6_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 enh_8g_csu_field_OCC_DONE_V5_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OCC_DONE_V5_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a8 bits 5) field OCC_DONE_V5 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_OCC_DONE_V5_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_OCC_DONE_V5_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OCC_DONE_V5_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_OCC_DONE_V5_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_OCC_DONE_V5_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_OCC_DONE_V5_poll", value );

    /* (0x000004a8 bits 5) field OCC_DONE_V5 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_poll( b_ptr,
                                                        h_ptr,
                                                        ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_OCC_DONE_V5_MSK,
                                                        (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_OCC_DONE_V5_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 enh_8g_csu_field_LOCK_LOSS_V4_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_LOCK_LOSS_V4_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a8 bits 4) field LOCK_LOSS_V4 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_LOCK_LOSS_V4_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_LOCK_LOSS_V4_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_LOCK_LOSS_V4_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_LOCK_LOSS_V4_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_LOCK_LOSS_V4_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_LOCK_LOSS_V4_poll", value );

    /* (0x000004a8 bits 4) field LOCK_LOSS_V4 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_poll( b_ptr,
                                                        h_ptr,
                                                        ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_LOCK_LOSS_V4_MSK,
                                                        (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_LOCK_LOSS_V4_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 enh_8g_csu_field_LOCK_GAIN_V3_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_LOCK_GAIN_V3_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a8 bits 3) field LOCK_GAIN_V3 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_LOCK_GAIN_V3_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_LOCK_GAIN_V3_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_LOCK_GAIN_V3_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_LOCK_GAIN_V3_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_LOCK_GAIN_V3_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_LOCK_GAIN_V3_poll", value );

    /* (0x000004a8 bits 3) field LOCK_GAIN_V3 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_poll( b_ptr,
                                                        h_ptr,
                                                        ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_LOCK_GAIN_V3_MSK,
                                                        (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_LOCK_GAIN_V3_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 enh_8g_csu_field_VCOC_DONE_V2_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCOC_DONE_V2_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a8 bits 2) field VCOC_DONE_V2 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_VCOC_DONE_V2_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_VCOC_DONE_V2_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCOC_DONE_V2_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_VCOC_DONE_V2_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_VCOC_DONE_V2_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                       sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_VCOC_DONE_V2_poll", value );

    /* (0x000004a8 bits 2) field VCOC_DONE_V2 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_poll( b_ptr,
                                                        h_ptr,
                                                        ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_VCOC_DONE_V2_MSK,
                                                        (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_VCOC_DONE_V2_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 enh_8g_csu_field_IN_RANGE_N_FAILS_V1_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_IN_RANGE_N_FAILS_V1_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a8 bits 1) field IN_RANGE_N_FAILS_V1 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_IN_RANGE_N_FAILS_V1_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_IN_RANGE_N_FAILS_V1_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_IN_RANGE_N_FAILS_V1_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_IN_RANGE_N_FAILS_V1_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_IN_RANGE_N_FAILS_V1_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                              sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_IN_RANGE_N_FAILS_V1_poll", value );

    /* (0x000004a8 bits 1) field IN_RANGE_N_FAILS_V1 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_poll( b_ptr,
                                                        h_ptr,
                                                        ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_IN_RANGE_N_FAILS_V1_MSK,
                                                        (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_IN_RANGE_N_FAILS_V1_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE UINT32 enh_8g_csu_field_IN_RANGE_N_PASSES_V0_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_IN_RANGE_N_PASSES_V0_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x000004a8 bits 0) field IN_RANGE_N_PASSES_V0 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    reg_value = enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_read( b_ptr,
                                                             h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_IN_RANGE_N_PASSES_V0_MSK) >> ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_IN_RANGE_N_PASSES_V0_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_IN_RANGE_N_PASSES_V0_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_IN_RANGE_N_PASSES_V0_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE enh_8g_csu_field_IN_RANGE_N_PASSES_V0_poll( enh_8g_csu_buffer_t *b_ptr,
                                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "enh_8g_csu_field_IN_RANGE_N_PASSES_V0_poll", value );

    /* (0x000004a8 bits 0) field IN_RANGE_N_PASSES_V0 of register PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG */
    return enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_poll( b_ptr,
                                                        h_ptr,
                                                        ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_IN_RANGE_N_PASSES_V0_MSK,
                                                        (value<<ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_IN_RANGE_N_PASSES_V0_OFF),
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
static INLINE UINT32 enh_8g_csu_field_LOCKB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_LOCKB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000041c bits 1) field LOCKB of register PMC_ENH_8G_CSU_REG_PLL_STA_REG */
    reg_value = enh_8g_csu_reg_PLL_STA_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_PLL_STA_REG_BIT_LOCKB_MSK) >> ENH_8G_CSU_REG_PLL_STA_REG_BIT_LOCKB_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_LOCKB_get", value );

    return value;
}
static INLINE UINT32 enh_8g_csu_field_PPM_OFF_RD_SIGN_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_PPM_OFF_RD_SIGN_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000420 bits 31) field PPM_OFF_RD_SIGN of register PMC_ENH_8G_CSU_REG_PLL_LOCK_DET_PPM_OFF_STA_REG */
    reg_value = enh_8g_csu_reg_PLL_LOCK_DET_PPM_OFF_STA_REG_read( b_ptr,
                                                                  h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_PLL_LOCK_DET_PPM_OFF_STA_REG_BIT_PPM_OFF_RD_SIGN_MSK) >> ENH_8G_CSU_REG_PLL_LOCK_DET_PPM_OFF_STA_REG_BIT_PPM_OFF_RD_SIGN_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_PPM_OFF_RD_SIGN_get", value );

    return value;
}
static INLINE UINT32 enh_8g_csu_field_PPM_OFF_RD_VAL_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_PPM_OFF_RD_VAL_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000420 bits 20:0) field PPM_OFF_RD_VAL of register PMC_ENH_8G_CSU_REG_PLL_LOCK_DET_PPM_OFF_STA_REG */
    reg_value = enh_8g_csu_reg_PLL_LOCK_DET_PPM_OFF_STA_REG_read( b_ptr,
                                                                  h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_PLL_LOCK_DET_PPM_OFF_STA_REG_BIT_PPM_OFF_RD_VAL_MSK) >> ENH_8G_CSU_REG_PLL_LOCK_DET_PPM_OFF_STA_REG_BIT_PPM_OFF_RD_VAL_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_PPM_OFF_RD_VAL_get", value );

    return value;
}
static INLINE UINT32 enh_8g_csu_field_range_PPM_OFF_RD_VAL_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_PPM_OFF_RD_VAL_get( enh_8g_csu_buffer_t *b_ptr,
                                                                sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_PPM_OFF_RD_VAL_get", stop_bit, start_bit );
    if (stop_bit > 20) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_PPM_OFF_RD_VAL_get", stop_bit, 20 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 20) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 20;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000420 bits 20:0) field PPM_OFF_RD_VAL of register PMC_ENH_8G_CSU_REG_PLL_LOCK_DET_PPM_OFF_STA_REG */
    reg_value = enh_8g_csu_reg_PLL_LOCK_DET_PPM_OFF_STA_REG_read( b_ptr,
                                                                  h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_PLL_LOCK_DET_PPM_OFF_STA_REG_BIT_PPM_OFF_RD_VAL_MSK)
                  >> ENH_8G_CSU_REG_PLL_LOCK_DET_PPM_OFF_STA_REG_BIT_PPM_OFF_RD_VAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_PLL_LOCK_DET_PPM_OFF_STA_REG_BIT_PPM_OFF_RD_VAL_MSK, ENH_8G_CSU_REG_PLL_LOCK_DET_PPM_OFF_STA_REG_BIT_PPM_OFF_RD_VAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_PPM_OFF_RD_VAL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 enh_8g_csu_field_VCOC_MODE_IN_RANGE_S_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCOC_MODE_IN_RANGE_S_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000430 bits 16) field VCOC_MODE_IN_RANGE_S of register PMC_ENH_8G_CSU_REG_VCO_CAL_STA_REG */
    reg_value = enh_8g_csu_reg_VCO_CAL_STA_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_MODE_IN_RANGE_S_MSK) >> ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_MODE_IN_RANGE_S_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCOC_MODE_IN_RANGE_S_get", value );

    return value;
}
static INLINE UINT32 enh_8g_csu_field_VCOC_BIST_MODE_COUNT_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCOC_BIST_MODE_COUNT_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000430 bits 15:8) field VCOC_BIST_MODE_COUNT of register PMC_ENH_8G_CSU_REG_VCO_CAL_STA_REG */
    reg_value = enh_8g_csu_reg_VCO_CAL_STA_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_BIST_MODE_COUNT_MSK) >> ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_BIST_MODE_COUNT_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCOC_BIST_MODE_COUNT_get", value );

    return value;
}
static INLINE UINT32 enh_8g_csu_field_range_VCOC_BIST_MODE_COUNT_get( enh_8g_csu_buffer_t *b_ptr,
                                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_VCOC_BIST_MODE_COUNT_get( enh_8g_csu_buffer_t *b_ptr,
                                                                      sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_VCOC_BIST_MODE_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_VCOC_BIST_MODE_COUNT_get", stop_bit, 7 );
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
    /* (0x00000430 bits 15:8) field VCOC_BIST_MODE_COUNT of register PMC_ENH_8G_CSU_REG_VCO_CAL_STA_REG */
    reg_value = enh_8g_csu_reg_VCO_CAL_STA_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_BIST_MODE_COUNT_MSK)
                  >> ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_BIST_MODE_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_BIST_MODE_COUNT_MSK, ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_BIST_MODE_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_VCOC_BIST_MODE_COUNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 enh_8g_csu_field_VCOC_MODE_OUT_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_VCOC_MODE_OUT_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000430 bits 5:0) field VCOC_MODE_OUT of register PMC_ENH_8G_CSU_REG_VCO_CAL_STA_REG */
    reg_value = enh_8g_csu_reg_VCO_CAL_STA_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_MODE_OUT_MSK) >> ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_MODE_OUT_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_VCOC_MODE_OUT_get", value );

    return value;
}
static INLINE UINT32 enh_8g_csu_field_range_VCOC_MODE_OUT_get( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_VCOC_MODE_OUT_get( enh_8g_csu_buffer_t *b_ptr,
                                                               sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_VCOC_MODE_OUT_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_VCOC_MODE_OUT_get", stop_bit, 5 );
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
    /* (0x00000430 bits 5:0) field VCOC_MODE_OUT of register PMC_ENH_8G_CSU_REG_VCO_CAL_STA_REG */
    reg_value = enh_8g_csu_reg_VCO_CAL_STA_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_MODE_OUT_MSK)
                  >> ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_MODE_OUT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_MODE_OUT_MSK, ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_MODE_OUT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_VCOC_MODE_OUT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000448 bits 6) field OC_THRESHOLD_CHECK_FAILB of register PMC_ENH_8G_CSU_REG_OCC_TXRX_STA_REG */
    reg_value = enh_8g_csu_reg_OCC_TXRX_STA_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_STA_REG_BIT_OC_THRESHOLD_CHECK_FAILB_MSK) >> ENH_8G_CSU_REG_OCC_TXRX_STA_REG_BIT_OC_THRESHOLD_CHECK_FAILB_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_get", value );

    return value;
}
static INLINE UINT32 enh_8g_csu_field_OCDAC_UNENC_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OCDAC_UNENC_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000448 bits 5:1) field OCDAC_UNENC of register PMC_ENH_8G_CSU_REG_OCC_TXRX_STA_REG */
    reg_value = enh_8g_csu_reg_OCC_TXRX_STA_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_STA_REG_BIT_OCDAC_UNENC_MSK) >> ENH_8G_CSU_REG_OCC_TXRX_STA_REG_BIT_OCDAC_UNENC_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OCDAC_UNENC_get", value );

    return value;
}
static INLINE UINT32 enh_8g_csu_field_range_OCDAC_UNENC_get( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_OCDAC_UNENC_get( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_OCDAC_UNENC_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_OCDAC_UNENC_get", stop_bit, 4 );
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
    /* (0x00000448 bits 5:1) field OCDAC_UNENC of register PMC_ENH_8G_CSU_REG_OCC_TXRX_STA_REG */
    reg_value = enh_8g_csu_reg_OCC_TXRX_STA_REG_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_STA_REG_BIT_OCDAC_UNENC_MSK)
                  >> ENH_8G_CSU_REG_OCC_TXRX_STA_REG_BIT_OCDAC_UNENC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_OCC_TXRX_STA_REG_BIT_OCDAC_UNENC_MSK, ENH_8G_CSU_REG_OCC_TXRX_STA_REG_BIT_OCDAC_UNENC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_OCDAC_UNENC_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 enh_8g_csu_field_OC_SENSE_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_OC_SENSE_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000448 bits 0) field OC_SENSE of register PMC_ENH_8G_CSU_REG_OCC_TXRX_STA_REG */
    reg_value = enh_8g_csu_reg_OCC_TXRX_STA_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_OCC_TXRX_STA_REG_BIT_OC_SENSE_MSK) >> ENH_8G_CSU_REG_OCC_TXRX_STA_REG_BIT_OC_SENSE_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_OC_SENSE_get", value );

    return value;
}
static INLINE UINT32 enh_8g_csu_field_REF_DCD_VAL_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_REF_DCD_VAL_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000478 bits 6) field REF_DCD_VAL of register PMC_ENH_8G_CSU_REG_REF_DBLR_CALIB_STA_REG */
    reg_value = enh_8g_csu_reg_REF_DBLR_CALIB_STA_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_REF_DBLR_CALIB_STA_REG_BIT_REF_DCD_VAL_MSK) >> ENH_8G_CSU_REG_REF_DBLR_CALIB_STA_REG_BIT_REF_DCD_VAL_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_REF_DCD_VAL_get", value );

    return value;
}
static INLINE UINT32 enh_8g_csu_field_DBL_CTRL_RD_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_DBL_CTRL_RD_get( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000478 bits 5:0) field DBL_CTRL_RD of register PMC_ENH_8G_CSU_REG_REF_DBLR_CALIB_STA_REG */
    reg_value = enh_8g_csu_reg_REF_DBLR_CALIB_STA_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ENH_8G_CSU_REG_REF_DBLR_CALIB_STA_REG_BIT_DBL_CTRL_RD_MSK) >> ENH_8G_CSU_REG_REF_DBLR_CALIB_STA_REG_BIT_DBL_CTRL_RD_OFF;
    IOLOG( "%s -> 0x%08x", "enh_8g_csu_field_DBL_CTRL_RD_get", value );

    return value;
}
static INLINE UINT32 enh_8g_csu_field_range_DBL_CTRL_RD_get( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 enh_8g_csu_field_range_DBL_CTRL_RD_get( enh_8g_csu_buffer_t *b_ptr,
                                                             sfi51_d8_tx_18x_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "enh_8g_csu_field_range_DBL_CTRL_RD_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "enh_8g_csu_field_range_DBL_CTRL_RD_get", stop_bit, 5 );
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
    /* (0x00000478 bits 5:0) field DBL_CTRL_RD of register PMC_ENH_8G_CSU_REG_REF_DBLR_CALIB_STA_REG */
    reg_value = enh_8g_csu_reg_REF_DBLR_CALIB_STA_REG_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & ENH_8G_CSU_REG_REF_DBLR_CALIB_STA_REG_BIT_DBL_CTRL_RD_MSK)
                  >> ENH_8G_CSU_REG_REF_DBLR_CALIB_STA_REG_BIT_DBL_CTRL_RD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ENH_8G_CSU_REG_REF_DBLR_CALIB_STA_REG_BIT_DBL_CTRL_RD_MSK, ENH_8G_CSU_REG_REF_DBLR_CALIB_STA_REG_BIT_DBL_CTRL_RD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "enh_8g_csu_field_range_DBL_CTRL_RD_get", start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ENH_8G_CSU_IO_INLINE_H */
