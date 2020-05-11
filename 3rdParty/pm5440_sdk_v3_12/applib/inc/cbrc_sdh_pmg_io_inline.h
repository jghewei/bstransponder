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
 *     and register accessor functions for the cbrc_sdh_pmg block
 *****************************************************************************/
#ifndef _CBRC_SDH_PMG_IO_INLINE_H
#define _CBRC_SDH_PMG_IO_INLINE_H

#include "cbrc_api.h"
#include "cbrc_sdh_pmg_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CBRC_SDH_PMG_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for cbrc_sdh_pmg
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
    cbrc_handle_t * h_ptr;
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} cbrc_sdh_pmg_buffer_t;
static INLINE void cbrc_sdh_pmg_buffer_init( cbrc_sdh_pmg_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_buffer_init( cbrc_sdh_pmg_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x2000) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "cbrc_sdh_pmg_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void cbrc_sdh_pmg_buffer_flush( cbrc_sdh_pmg_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_buffer_flush( cbrc_sdh_pmg_buffer_t *b_ptr )
{
    IOLOG( "cbrc_sdh_pmg_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 cbrc_sdh_pmg_reg_read( cbrc_sdh_pmg_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 mem_type,
                                            UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_reg_read( cbrc_sdh_pmg_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
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
static INLINE void cbrc_sdh_pmg_reg_write( cbrc_sdh_pmg_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_reg_write( cbrc_sdh_pmg_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
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

static INLINE void cbrc_sdh_pmg_field_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 mask,
                                           UINT32 unused_mask,
                                           UINT32 ofs,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                           cbrc_handle_t *h_ptr,
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

static INLINE void cbrc_sdh_pmg_action_on_write_field_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mem_type,
                                                           UINT32 reg,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_action_on_write_field_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
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

static INLINE void cbrc_sdh_pmg_burst_read( cbrc_sdh_pmg_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 len,
                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_burst_read( cbrc_sdh_pmg_buffer_t *b_ptr,
                                            cbrc_handle_t *h_ptr,
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

static INLINE void cbrc_sdh_pmg_burst_write( cbrc_sdh_pmg_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 mem_type,
                                             UINT32 reg,
                                             UINT32 len,
                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_burst_write( cbrc_sdh_pmg_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE cbrc_sdh_pmg_poll( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 value,
                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                      UINT32 max_count,
                                                      UINT32 *num_failed_polls,
                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE cbrc_sdh_pmg_poll( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
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
 *  register access functions for cbrc_sdh_pmg
 * ==================================================================================
 */

static INLINE void cbrc_sdh_pmg_reg_CFG_write( cbrc_sdh_pmg_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_reg_CFG_write( cbrc_sdh_pmg_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cbrc_sdh_pmg_reg_CFG_write", value );
    cbrc_sdh_pmg_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_CBRC_SDH_PMG_REG_CFG,
                            value);
}

static INLINE void cbrc_sdh_pmg_reg_CFG_field_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_reg_CFG_field_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "cbrc_sdh_pmg_reg_CFG_field_set", A, mask, ofs, value );
    cbrc_sdh_pmg_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_CBRC_SDH_PMG_REG_CFG,
                            mask,
                            PMC_CBRC_SDH_PMG_REG_CFG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 cbrc_sdh_pmg_reg_CFG_read( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_reg_CFG_read( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = cbrc_sdh_pmg_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_CBRC_SDH_PMG_REG_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "cbrc_sdh_pmg_reg_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void cbrc_sdh_pmg_reg_INT_PIN_EN_write( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_reg_INT_PIN_EN_write( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cbrc_sdh_pmg_reg_INT_PIN_EN_write", value );
    cbrc_sdh_pmg_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_CBRC_SDH_PMG_REG_INT_PIN_EN,
                            value);
}

static INLINE void cbrc_sdh_pmg_reg_INT_PIN_EN_field_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_reg_INT_PIN_EN_field_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "cbrc_sdh_pmg_reg_INT_PIN_EN_field_set", A, mask, ofs, value );
    cbrc_sdh_pmg_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_CBRC_SDH_PMG_REG_INT_PIN_EN,
                            mask,
                            PMC_CBRC_SDH_PMG_REG_INT_PIN_EN_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 cbrc_sdh_pmg_reg_INT_PIN_EN_read( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_reg_INT_PIN_EN_read( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = cbrc_sdh_pmg_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_CBRC_SDH_PMG_REG_INT_PIN_EN);

    IOLOG( "%s -> 0x%08x; A=%d", "cbrc_sdh_pmg_reg_INT_PIN_EN_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 cbrc_sdh_pmg_reg_INT_SUMMARY_read( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_reg_INT_SUMMARY_read( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = cbrc_sdh_pmg_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_CBRC_SDH_PMG_REG_INT_SUMMARY);

    IOLOG( "%s -> 0x%08x; A=%d", "cbrc_sdh_pmg_reg_INT_SUMMARY_read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_2_LOWPWR_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_2_LOWPWR_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_2_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_2_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_2_LOWPWR_set", A, value );

    /* (0x00002000 bits 10) field SDH_PMG_2_LOWPWR of register PMC_CBRC_SDH_PMG_REG_CFG */
    cbrc_sdh_pmg_reg_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_2_LOWPWR_MSK,
                                    CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_2_LOWPWR_OFF,
                                    value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_LOWPWR_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_LOWPWR_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_2_LOWPWR_get", A, 1);
    /* (0x00002000 bits 10) field SDH_PMG_2_LOWPWR of register PMC_CBRC_SDH_PMG_REG_CFG */
    reg_value = cbrc_sdh_pmg_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_2_LOWPWR_MSK) >> CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_2_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_2_LOWPWR_get", A, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_1_LOWPWR_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_1_LOWPWR_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_1_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_1_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_1_LOWPWR_set", A, value );

    /* (0x00002000 bits 9) field SDH_PMG_1_LOWPWR of register PMC_CBRC_SDH_PMG_REG_CFG */
    cbrc_sdh_pmg_reg_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_1_LOWPWR_MSK,
                                    CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_1_LOWPWR_OFF,
                                    value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_LOWPWR_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_LOWPWR_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_1_LOWPWR_get", A, 1);
    /* (0x00002000 bits 9) field SDH_PMG_1_LOWPWR of register PMC_CBRC_SDH_PMG_REG_CFG */
    reg_value = cbrc_sdh_pmg_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_1_LOWPWR_MSK) >> CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_1_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_1_LOWPWR_get", A, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_0_LOWPWR_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_0_LOWPWR_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_0_LOWPWR_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_0_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_0_LOWPWR_set", A, value );

    /* (0x00002000 bits 8) field SDH_PMG_0_LOWPWR of register PMC_CBRC_SDH_PMG_REG_CFG */
    cbrc_sdh_pmg_reg_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_0_LOWPWR_MSK,
                                    CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_0_LOWPWR_OFF,
                                    value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_LOWPWR_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_LOWPWR_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_0_LOWPWR_get", A, 1);
    /* (0x00002000 bits 8) field SDH_PMG_0_LOWPWR of register PMC_CBRC_SDH_PMG_REG_CFG */
    reg_value = cbrc_sdh_pmg_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_0_LOWPWR_MSK) >> CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_0_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_0_LOWPWR_get", A, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_2_SW_RST_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_2_SW_RST_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_2_SW_RST_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_2_SW_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_2_SW_RST_set", A, value );

    /* (0x00002000 bits 7) field SDH_PMG_2_SW_RST of register PMC_CBRC_SDH_PMG_REG_CFG */
    cbrc_sdh_pmg_reg_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_2_SW_RST_MSK,
                                    CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_2_SW_RST_OFF,
                                    value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_SW_RST_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_SW_RST_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_2_SW_RST_get", A, 1);
    /* (0x00002000 bits 7) field SDH_PMG_2_SW_RST of register PMC_CBRC_SDH_PMG_REG_CFG */
    reg_value = cbrc_sdh_pmg_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_2_SW_RST_MSK) >> CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_2_SW_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_2_SW_RST_get", A, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_1_SW_RST_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_1_SW_RST_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_1_SW_RST_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_1_SW_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_1_SW_RST_set", A, value );

    /* (0x00002000 bits 6) field SDH_PMG_1_SW_RST of register PMC_CBRC_SDH_PMG_REG_CFG */
    cbrc_sdh_pmg_reg_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_1_SW_RST_MSK,
                                    CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_1_SW_RST_OFF,
                                    value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_SW_RST_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_SW_RST_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_1_SW_RST_get", A, 1);
    /* (0x00002000 bits 6) field SDH_PMG_1_SW_RST of register PMC_CBRC_SDH_PMG_REG_CFG */
    reg_value = cbrc_sdh_pmg_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_1_SW_RST_MSK) >> CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_1_SW_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_1_SW_RST_get", A, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_0_SW_RST_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_0_SW_RST_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_0_SW_RST_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_0_SW_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_0_SW_RST_set", A, value );

    /* (0x00002000 bits 5) field SDH_PMG_0_SW_RST of register PMC_CBRC_SDH_PMG_REG_CFG */
    cbrc_sdh_pmg_reg_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_0_SW_RST_MSK,
                                    CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_0_SW_RST_OFF,
                                    value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_SW_RST_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_SW_RST_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                              cbrc_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_0_SW_RST_get", A, 1);
    /* (0x00002000 bits 5) field SDH_PMG_0_SW_RST of register PMC_CBRC_SDH_PMG_REG_CFG */
    reg_value = cbrc_sdh_pmg_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_0_SW_RST_MSK) >> CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_0_SW_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_0_SW_RST_get", A, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                               cbrc_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_set", A, value );

    /* (0x00002000 bits 4) field CBRC_SDH_PMG_SW_RST of register PMC_CBRC_SDH_PMG_REG_CFG */
    cbrc_sdh_pmg_reg_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    CBRC_SDH_PMG_REG_CFG_BIT_CBRC_SDH_PMG_SW_RST_MSK,
                                    CBRC_SDH_PMG_REG_CFG_BIT_CBRC_SDH_PMG_SW_RST_OFF,
                                    value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_get", A, 1);
    /* (0x00002000 bits 4) field CBRC_SDH_PMG_SW_RST of register PMC_CBRC_SDH_PMG_REG_CFG */
    reg_value = cbrc_sdh_pmg_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_CFG_BIT_CBRC_SDH_PMG_SW_RST_MSK) >> CBRC_SDH_PMG_REG_CFG_BIT_CBRC_SDH_PMG_SW_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_CBRC_SDH_PMG_SW_RST_get", A, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_PRBS_MON_CH_SEL_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_PRBS_MON_CH_SEL_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_PRBS_MON_CH_SEL_set", A, 1);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_PRBS_MON_CH_SEL_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_PRBS_MON_CH_SEL_set", A, value );

    /* (0x00002000 bits 3:0) field PRBS_MON_CH_SEL of register PMC_CBRC_SDH_PMG_REG_CFG */
    cbrc_sdh_pmg_reg_CFG_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    CBRC_SDH_PMG_REG_CFG_BIT_PRBS_MON_CH_SEL_MSK,
                                    CBRC_SDH_PMG_REG_CFG_BIT_PRBS_MON_CH_SEL_OFF,
                                    value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_PRBS_MON_CH_SEL_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_PRBS_MON_CH_SEL_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_PRBS_MON_CH_SEL_get", A, 1);
    /* (0x00002000 bits 3:0) field PRBS_MON_CH_SEL of register PMC_CBRC_SDH_PMG_REG_CFG */
    reg_value = cbrc_sdh_pmg_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_CFG_BIT_PRBS_MON_CH_SEL_MSK) >> CBRC_SDH_PMG_REG_CFG_BIT_PRBS_MON_CH_SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_PRBS_MON_CH_SEL_get", A, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_range_PRBS_MON_CH_SEL_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_range_PRBS_MON_CH_SEL_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_range_PRBS_MON_CH_SEL_set", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbrc_sdh_pmg_field_range_PRBS_MON_CH_SEL_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbrc_sdh_pmg_field_range_PRBS_MON_CH_SEL_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "cbrc_sdh_pmg_field_range_PRBS_MON_CH_SEL_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002000 bits 3:0) field PRBS_MON_CH_SEL of register PMC_CBRC_SDH_PMG_REG_CFG */
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
        /* (0x00002000 bits 3:0) field PRBS_MON_CH_SEL of register PMC_CBRC_SDH_PMG_REG_CFG */
        cbrc_sdh_pmg_reg_CFG_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        subfield_mask << (CBRC_SDH_PMG_REG_CFG_BIT_PRBS_MON_CH_SEL_OFF + subfield_offset),
                                        CBRC_SDH_PMG_REG_CFG_BIT_PRBS_MON_CH_SEL_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 cbrc_sdh_pmg_field_range_PRBS_MON_CH_SEL_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_range_PRBS_MON_CH_SEL_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                                   cbrc_handle_t *h_ptr,
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

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_range_PRBS_MON_CH_SEL_get", A, 1);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbrc_sdh_pmg_field_range_PRBS_MON_CH_SEL_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbrc_sdh_pmg_field_range_PRBS_MON_CH_SEL_get", stop_bit, 3 );
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
    /* (0x00002000 bits 3:0) field PRBS_MON_CH_SEL of register PMC_CBRC_SDH_PMG_REG_CFG */
    reg_value = cbrc_sdh_pmg_reg_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & CBRC_SDH_PMG_REG_CFG_BIT_PRBS_MON_CH_SEL_MSK)
                  >> CBRC_SDH_PMG_REG_CFG_BIT_PRBS_MON_CH_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CBRC_SDH_PMG_REG_CFG_BIT_PRBS_MON_CH_SEL_MSK, CBRC_SDH_PMG_REG_CFG_BIT_PRBS_MON_CH_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "cbrc_sdh_pmg_field_range_PRBS_MON_CH_SEL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_2_S_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_2_S_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_2_S_E_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_2_S_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_2_S_E_set", A, value );

    /* (0x00002008 bits 11) field SDH_PMG_2_S_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    cbrc_sdh_pmg_reg_INT_PIN_EN_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_S_E_MSK,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_S_E_OFF,
                                           value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_S_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_S_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_2_S_E_get", A, 1);
    /* (0x00002008 bits 11) field SDH_PMG_2_S_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    reg_value = cbrc_sdh_pmg_reg_INT_PIN_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_S_E_MSK) >> CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_S_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_2_S_E_get", A, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_1_S_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_1_S_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_1_S_E_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_1_S_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_1_S_E_set", A, value );

    /* (0x00002008 bits 10) field SDH_PMG_1_S_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    cbrc_sdh_pmg_reg_INT_PIN_EN_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_S_E_MSK,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_S_E_OFF,
                                           value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_S_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_S_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_1_S_E_get", A, 1);
    /* (0x00002008 bits 10) field SDH_PMG_1_S_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    reg_value = cbrc_sdh_pmg_reg_INT_PIN_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_S_E_MSK) >> CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_S_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_1_S_E_get", A, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_0_S_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_0_S_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_0_S_E_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_0_S_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_0_S_E_set", A, value );

    /* (0x00002008 bits 9) field SDH_PMG_0_S_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    cbrc_sdh_pmg_reg_INT_PIN_EN_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_S_E_MSK,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_S_E_OFF,
                                           value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_S_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_S_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_0_S_E_get", A, 1);
    /* (0x00002008 bits 9) field SDH_PMG_0_S_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    reg_value = cbrc_sdh_pmg_reg_INT_PIN_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_S_E_MSK) >> CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_S_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_0_S_E_get", A, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_2_D_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_2_D_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_2_D_E_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_2_D_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_2_D_E_set", A, value );

    /* (0x00002008 bits 8) field SDH_PMG_2_D_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    cbrc_sdh_pmg_reg_INT_PIN_EN_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_D_E_MSK,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_D_E_OFF,
                                           value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_D_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_D_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_2_D_E_get", A, 1);
    /* (0x00002008 bits 8) field SDH_PMG_2_D_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    reg_value = cbrc_sdh_pmg_reg_INT_PIN_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_D_E_MSK) >> CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_D_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_2_D_E_get", A, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_1_D_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_1_D_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_1_D_E_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_1_D_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_1_D_E_set", A, value );

    /* (0x00002008 bits 7) field SDH_PMG_1_D_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    cbrc_sdh_pmg_reg_INT_PIN_EN_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_D_E_MSK,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_D_E_OFF,
                                           value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_D_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_D_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_1_D_E_get", A, 1);
    /* (0x00002008 bits 7) field SDH_PMG_1_D_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    reg_value = cbrc_sdh_pmg_reg_INT_PIN_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_D_E_MSK) >> CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_D_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_1_D_E_get", A, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_0_D_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_0_D_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_0_D_E_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_0_D_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_0_D_E_set", A, value );

    /* (0x00002008 bits 6) field SDH_PMG_0_D_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    cbrc_sdh_pmg_reg_INT_PIN_EN_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_D_E_MSK,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_D_E_OFF,
                                           value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_D_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_D_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_0_D_E_get", A, 1);
    /* (0x00002008 bits 6) field SDH_PMG_0_D_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    reg_value = cbrc_sdh_pmg_reg_INT_PIN_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_D_E_MSK) >> CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_D_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_0_D_E_get", A, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_2_M_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_2_M_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_2_M_E_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_2_M_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_2_M_E_set", A, value );

    /* (0x00002008 bits 5) field SDH_PMG_2_M_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    cbrc_sdh_pmg_reg_INT_PIN_EN_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_M_E_MSK,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_M_E_OFF,
                                           value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_M_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_M_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_2_M_E_get", A, 1);
    /* (0x00002008 bits 5) field SDH_PMG_2_M_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    reg_value = cbrc_sdh_pmg_reg_INT_PIN_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_M_E_MSK) >> CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_M_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_2_M_E_get", A, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_1_M_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_1_M_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_1_M_E_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_1_M_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_1_M_E_set", A, value );

    /* (0x00002008 bits 4) field SDH_PMG_1_M_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    cbrc_sdh_pmg_reg_INT_PIN_EN_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_M_E_MSK,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_M_E_OFF,
                                           value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_M_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_M_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_1_M_E_get", A, 1);
    /* (0x00002008 bits 4) field SDH_PMG_1_M_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    reg_value = cbrc_sdh_pmg_reg_INT_PIN_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_M_E_MSK) >> CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_M_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_1_M_E_get", A, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_0_M_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_0_M_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_0_M_E_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_0_M_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_0_M_E_set", A, value );

    /* (0x00002008 bits 3) field SDH_PMG_0_M_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    cbrc_sdh_pmg_reg_INT_PIN_EN_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_M_E_MSK,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_M_E_OFF,
                                           value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_M_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_M_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_0_M_E_get", A, 1);
    /* (0x00002008 bits 3) field SDH_PMG_0_M_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    reg_value = cbrc_sdh_pmg_reg_INT_PIN_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_M_E_MSK) >> CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_M_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_0_M_E_get", A, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_2_P_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_2_P_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_2_P_E_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_2_P_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_2_P_E_set", A, value );

    /* (0x00002008 bits 2) field SDH_PMG_2_P_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    cbrc_sdh_pmg_reg_INT_PIN_EN_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_P_E_MSK,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_P_E_OFF,
                                           value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_P_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_P_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_2_P_E_get", A, 1);
    /* (0x00002008 bits 2) field SDH_PMG_2_P_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    reg_value = cbrc_sdh_pmg_reg_INT_PIN_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_P_E_MSK) >> CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_P_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_2_P_E_get", A, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_1_P_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_1_P_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_1_P_E_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_1_P_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_1_P_E_set", A, value );

    /* (0x00002008 bits 1) field SDH_PMG_1_P_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    cbrc_sdh_pmg_reg_INT_PIN_EN_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_P_E_MSK,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_P_E_OFF,
                                           value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_P_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_P_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_1_P_E_get", A, 1);
    /* (0x00002008 bits 1) field SDH_PMG_1_P_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    reg_value = cbrc_sdh_pmg_reg_INT_PIN_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_P_E_MSK) >> CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_P_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_1_P_E_get", A, value );

    return value;
}
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_0_P_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cbrc_sdh_pmg_field_SDH_PMG_0_P_E_set( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_0_P_E_set", A, 1);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_0_P_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_0_P_E_set", A, value );

    /* (0x00002008 bits 0) field SDH_PMG_0_P_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    cbrc_sdh_pmg_reg_INT_PIN_EN_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_P_E_MSK,
                                           CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_P_E_OFF,
                                           value);
}

static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_P_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_P_E_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_0_P_E_get", A, 1);
    /* (0x00002008 bits 0) field SDH_PMG_0_P_E of register PMC_CBRC_SDH_PMG_REG_INT_PIN_EN */
    reg_value = cbrc_sdh_pmg_reg_INT_PIN_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_P_E_MSK) >> CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_P_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_0_P_E_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_INT_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_2_INT_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_2_INT_get", A, 1);
    /* (0x00002004 bits 2) field SDH_PMG_2_INT of register PMC_CBRC_SDH_PMG_REG_INT_SUMMARY */
    reg_value = cbrc_sdh_pmg_reg_INT_SUMMARY_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & CBRC_SDH_PMG_REG_INT_SUMMARY_BIT_SDH_PMG_2_INT_MSK) >> CBRC_SDH_PMG_REG_INT_SUMMARY_BIT_SDH_PMG_2_INT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_2_INT_get", A, value );

    return value;
}
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_INT_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_1_INT_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_1_INT_get", A, 1);
    /* (0x00002004 bits 1) field SDH_PMG_1_INT of register PMC_CBRC_SDH_PMG_REG_INT_SUMMARY */
    reg_value = cbrc_sdh_pmg_reg_INT_SUMMARY_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & CBRC_SDH_PMG_REG_INT_SUMMARY_BIT_SDH_PMG_1_INT_MSK) >> CBRC_SDH_PMG_REG_INT_SUMMARY_BIT_SDH_PMG_1_INT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_1_INT_get", A, value );

    return value;
}
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_INT_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbrc_sdh_pmg_field_SDH_PMG_0_INT_get( cbrc_sdh_pmg_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 1)
        IO_RANGE_CHECK("%s A is %d but max is %d", "cbrc_sdh_pmg_field_SDH_PMG_0_INT_get", A, 1);
    /* (0x00002004 bits 0) field SDH_PMG_0_INT of register PMC_CBRC_SDH_PMG_REG_INT_SUMMARY */
    reg_value = cbrc_sdh_pmg_reg_INT_SUMMARY_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & CBRC_SDH_PMG_REG_INT_SUMMARY_BIT_SDH_PMG_0_INT_MSK) >> CBRC_SDH_PMG_REG_INT_SUMMARY_BIT_SDH_PMG_0_INT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "cbrc_sdh_pmg_field_SDH_PMG_0_INT_get", A, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CBRC_SDH_PMG_IO_INLINE_H */
