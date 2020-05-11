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
 *     and register accessor functions for the s16_csu_pcbi block
 *****************************************************************************/
#ifndef _S16_CSU_PCBI_IO_INLINE_H
#define _S16_CSU_PCBI_IO_INLINE_H

#include "s16_loc.h"
#include "s16_csu_pcbi_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define S16_CSU_PCBI_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for s16_csu_pcbi
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
    s16_handle_t * h_ptr;
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} s16_csu_pcbi_buffer_t;
static INLINE void s16_csu_pcbi_buffer_init( s16_csu_pcbi_buffer_t *b_ptr,
                                             s16_handle_t *h_ptr,
                                             UINT32 A ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_buffer_init( s16_csu_pcbi_buffer_t *b_ptr,
                                             s16_handle_t *h_ptr,
                                             UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x1000) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "s16_csu_pcbi_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void s16_csu_pcbi_buffer_flush( s16_csu_pcbi_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_buffer_flush( s16_csu_pcbi_buffer_t *b_ptr )
{
    IOLOG( "s16_csu_pcbi_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 s16_csu_pcbi_reg_read( s16_csu_pcbi_buffer_t *b_ptr,
                                            s16_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 mem_type,
                                            UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_read( s16_csu_pcbi_buffer_t *b_ptr,
                                            s16_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 mem_type,
                                            UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->A)*0x1000),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg);
    return value;
}
static INLINE void s16_csu_pcbi_reg_write( s16_csu_pcbi_buffer_t *b_ptr,
                                           s16_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_write( s16_csu_pcbi_buffer_t *b_ptr,
                                           s16_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->A)*0x1000),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                         value);
}

static INLINE void s16_csu_pcbi_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                           s16_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 mask,
                                           UINT32 unused_mask,
                                           UINT32 ofs,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                           s16_handle_t *h_ptr,
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
                          (b_ptr->base_address + (b_ptr->A)*0x1000),
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
                             (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void s16_csu_pcbi_action_on_write_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mem_type,
                                                           UINT32 reg,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_action_on_write_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
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
                                         (b_ptr->base_address + (b_ptr->A)*0x1000),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                         value<<ofs);
}

static INLINE void s16_csu_pcbi_burst_read( s16_csu_pcbi_buffer_t *b_ptr,
                                            s16_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 len,
                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_burst_read( s16_csu_pcbi_buffer_t *b_ptr,
                                            s16_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 len,
                                            UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_read( b_ptr->coalesce_handle,
                          mem_type,
                          (b_ptr->base_address + (b_ptr->A)*0x1000),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                          len,
                          value);
}

static INLINE void s16_csu_pcbi_burst_write( s16_csu_pcbi_buffer_t *b_ptr,
                                             s16_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 mem_type,
                                             UINT32 reg,
                                             UINT32 len,
                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_burst_write( s16_csu_pcbi_buffer_t *b_ptr,
                                             s16_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 mem_type,
                                             UINT32 reg,
                                             UINT32 len,
                                             UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_write( b_ptr->coalesce_handle,
                           mem_type,
                           (b_ptr->base_address + (b_ptr->A)*0x1000),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 value,
                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                      UINT32 max_count,
                                                      UINT32 *num_failed_polls,
                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
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
                           (b_ptr->base_address + (b_ptr->A)*0x1000),
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
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for s16_csu_pcbi
 * ==================================================================================
 */

static INLINE void s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1,
                            mask,
                            PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2,
                            mask,
                            PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3,
                            mask,
                            PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG,
                            mask,
                            PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_CONFIG_REG2_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_CONFIG_REG2_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_CONFIG_REG2_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_CONFIG_REG2,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_CONFIG_REG2,
                            mask,
                            PMC_PCBI_REG_CSU0_CONFIG_REG2_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_CONFIG_REG2);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_CONFIG_REG3_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_CONFIG_REG3_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_CONFIG_REG3_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_CONFIG_REG3,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_CONFIG_REG3_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_CONFIG_REG3_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_CONFIG_REG3_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_CONFIG_REG3,
                            mask,
                            PMC_PCBI_REG_CSU0_CONFIG_REG3_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_CONFIG_REG3_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_CONFIG_REG3_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_CONFIG_REG3);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_CONFIG_REG3_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3,
                            mask,
                            PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1,
                            mask,
                            PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2,
                            mask,
                            PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_RESET_CONFIG_REG,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_RESET_CONFIG_REG,
                            mask,
                            PMC_PCBI_REG_CSU0_RESET_CONFIG_REG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_RESET_CONFIG_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU_TEST_CONFIG_REG_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU_TEST_CONFIG_REG_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU_TEST_CONFIG_REG_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU_TEST_CONFIG_REG,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU_TEST_CONFIG_REG_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU_TEST_CONFIG_REG_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU_TEST_CONFIG_REG_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU_TEST_CONFIG_REG,
                            mask,
                            PMC_PCBI_REG_CSU_TEST_CONFIG_REG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU_TEST_CONFIG_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU_TEST_CONFIG_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU_TEST_CONFIG_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU_TEST_CONFIG_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CLK_DEGLITCHER_CONFIG_REG_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CLK_DEGLITCHER_CONFIG_REG_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CLK_DEGLITCHER_CONFIG_REG_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CLK_DEGLITCHER_CONFIG_REG,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CLK_DEGLITCHER_CONFIG_REG_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CLK_DEGLITCHER_CONFIG_REG_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CLK_DEGLITCHER_CONFIG_REG_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CLK_DEGLITCHER_CONFIG_REG,
                            mask,
                            PMC_PCBI_REG_CLK_DEGLITCHER_CONFIG_REG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CLK_DEGLITCHER_CONFIG_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CLK_DEGLITCHER_CONFIG_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CLK_DEGLITCHER_CONFIG_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CLK_DEGLITCHER_CONFIG_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_GLOBAL_RESET_CONFIG_REG_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_GLOBAL_RESET_CONFIG_REG_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_GLOBAL_RESET_CONFIG_REG_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_GLOBAL_RESET_CONFIG_REG,
                            value);
}

static INLINE void s16_csu_pcbi_reg_GLOBAL_RESET_CONFIG_REG_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_GLOBAL_RESET_CONFIG_REG_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_GLOBAL_RESET_CONFIG_REG_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_GLOBAL_RESET_CONFIG_REG,
                            mask,
                            PMC_PCBI_REG_GLOBAL_RESET_CONFIG_REG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_GLOBAL_RESET_CONFIG_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_GLOBAL_RESET_CONFIG_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_GLOBAL_RESET_CONFIG_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_GLOBAL_RESET_CONFIG_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG1_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG1_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG1_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG1_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG1_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG1_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1,
                            mask,
                            PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG1_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG1_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG1_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG2_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG2_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG2_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG2_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG2_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG2_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2,
                            mask,
                            PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG2_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG2_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG2_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG3_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG3_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG3_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG3_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG3_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG3_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3,
                            mask,
                            PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG3_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG3_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG3_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                              s16_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                              s16_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG,
                            mask,
                            PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
                                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
                                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG1,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG1,
                            mask,
                            PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG1_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG1);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG2_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG2_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG2_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG2,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG2_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG2_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG2_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG2,
                            mask,
                            PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG2_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG2_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG2_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG2);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG2_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG3_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG3_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG3_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG3,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG3_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG3_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG3_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG3,
                            mask,
                            PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG3_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG3_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG3_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG3);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG3_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG4_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG4_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG4_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG4,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG4_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG4_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG4_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG4,
                            mask,
                            PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG4_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG4_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG4_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG4);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG4_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG5_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG5_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG5_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG5,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG5_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG5_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG5_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG5,
                            mask,
                            PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG5_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG5_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG5_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG5);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG5_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG6_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG6_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG6_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG6,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG6_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG6_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG6_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG6,
                            mask,
                            PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG6_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG6_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG6_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG6);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG6_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_GLOBAL_CFG_REG1,
                            value);
}

static INLINE void s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_CSU0_GLOBAL_CFG_REG1,
                            mask,
                            PMC_PCBI_REG_CSU0_GLOBAL_CFG_REG1_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_CSU0_GLOBAL_CFG_REG1);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_ACB_FT_CONFIG_REG1,
                            value);
}

static INLINE void s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_ACB_FT_CONFIG_REG1,
                            mask,
                            PMC_PCBI_REG_ACB_FT_CONFIG_REG1_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_ACB_FT_CONFIG_REG1);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_PMM_CONFIG_REG1_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_PMM_CONFIG_REG1_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_PMM_CONFIG_REG1_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_PMM_CONFIG_REG1,
                            value);
}

static INLINE void s16_csu_pcbi_reg_PMM_CONFIG_REG1_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_PMM_CONFIG_REG1_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_PMM_CONFIG_REG1_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_PMM_CONFIG_REG1,
                            mask,
                            PMC_PCBI_REG_PMM_CONFIG_REG1_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_PMM_CONFIG_REG1_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_PMM_CONFIG_REG1_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_PMM_CONFIG_REG1);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_PMM_CONFIG_REG1_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_PMM_CONFIG_REG2_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_PMM_CONFIG_REG2_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_PMM_CONFIG_REG2_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_PMM_CONFIG_REG2,
                            value);
}

static INLINE void s16_csu_pcbi_reg_PMM_CONFIG_REG2_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_PMM_CONFIG_REG2_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_PMM_CONFIG_REG2_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_PMM_CONFIG_REG2,
                            mask,
                            PMC_PCBI_REG_PMM_CONFIG_REG2_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_PMM_CONFIG_REG2_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_PMM_CONFIG_REG2_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_PMM_CONFIG_REG2);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_PMM_CONFIG_REG2_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_PMM_CONFIG_REG3_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_PMM_CONFIG_REG3_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_PMM_CONFIG_REG3_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_PMM_CONFIG_REG3,
                            value);
}

static INLINE void s16_csu_pcbi_reg_PMM_CONFIG_REG3_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_PMM_CONFIG_REG3_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_PMM_CONFIG_REG3_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_PCBI_REG_PMM_CONFIG_REG3,
                            mask,
                            PMC_PCBI_REG_PMM_CONFIG_REG3_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_PMM_CONFIG_REG3_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_PMM_CONFIG_REG3_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_PCBI_REG_PMM_CONFIG_REG3);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_PMM_CONFIG_REG3_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_csu_pcbi_reg_INT_REG_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                   s16_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_INT_REG_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                   s16_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_INT_REG_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_STATUS,
                            PMC_PCBI_REG_INT_REG,
                            value);
}

static INLINE void s16_csu_pcbi_reg_INT_REG_action_on_write_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_INT_REG_action_on_write_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_INT_REG_action_on_write_field_set", A, mask, ofs, value );
    s16_csu_pcbi_action_on_write_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            MEM_TYPE_STATUS,
                                            PMC_PCBI_REG_INT_REG,
                                            mask,
                                            ofs,
                                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_INT_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                    s16_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_INT_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                    s16_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_PCBI_REG_INT_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_INT_REG_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_reg_INT_REG_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_reg_INT_REG_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "s16_csu_pcbi_reg_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return s16_csu_pcbi_poll( b_ptr,
                              h_ptr,
                              A,
                              MEM_TYPE_STATUS,
                              PMC_PCBI_REG_INT_REG,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE void s16_csu_pcbi_reg_INT_EN_REG_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_INT_EN_REG_write( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_csu_pcbi_reg_INT_EN_REG_write", value );
    s16_csu_pcbi_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_STATUS,
                            PMC_PCBI_REG_INT_EN_REG,
                            value);
}

static INLINE void s16_csu_pcbi_reg_INT_EN_REG_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_reg_INT_EN_REG_field_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_csu_pcbi_reg_INT_EN_REG_field_set", A, mask, ofs, value );
    s16_csu_pcbi_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_STATUS,
                            PMC_PCBI_REG_INT_EN_REG,
                            mask,
                            PMC_PCBI_REG_INT_EN_REG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 s16_csu_pcbi_reg_INT_EN_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_INT_EN_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_PCBI_REG_INT_EN_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_INT_EN_REG_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 s16_csu_pcbi_reg_INT_VAL_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_INT_VAL_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_PCBI_REG_INT_VAL_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_INT_VAL_REG_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_reg_INT_VAL_REG_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_reg_INT_VAL_REG_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "s16_csu_pcbi_reg_INT_VAL_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return s16_csu_pcbi_poll( b_ptr,
                              h_ptr,
                              A,
                              MEM_TYPE_STATUS,
                              PMC_PCBI_REG_INT_VAL_REG,
                              mask,
                              value,
                              cmp,
                              max_count,
                              num_failed_polls,
                              delay_between_polls_in_microseconds);

}


static INLINE UINT32 s16_csu_pcbi_reg_CSU0_PLL_OOL_STAT_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_PLL_OOL_STAT_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_PCBI_REG_CSU0_PLL_OOL_STAT_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_PLL_OOL_STAT_REG_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_VCOC_STAT_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_VCOC_STAT_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_PCBI_REG_CSU0_VCOC_STAT_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_VCOC_STAT_REG_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_STAT_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_STAT_REG_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_STAT_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_STAT_REG_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 s16_csu_pcbi_reg_PMM_STATUS_REG1_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_PMM_STATUS_REG1_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_PCBI_REG_PMM_STATUS_REG1);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_PMM_STATUS_REG1_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 s16_csu_pcbi_reg_PMM_STATUS_REG2_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_reg_PMM_STATUS_REG2_read( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_csu_pcbi_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_PCBI_REG_PMM_STATUS_REG2);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_csu_pcbi_reg_PMM_STATUS_REG2_read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void s16_csu_pcbi_field_CSU0_SW_RESET_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_SW_RESET_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_SW_RESET_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_SW_RESET_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_SW_RESET_set", A, value );

    /* (0x00000510 bits 23) field CSU0_SW_RESET of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1 */
    s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_SW_RESET_MSK,
                                                         PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_SW_RESET_OFF,
                                                         value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_SW_RESET_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_SW_RESET_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_SW_RESET_get", A, 15);
    /* (0x00000510 bits 23) field CSU0_SW_RESET of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_SW_RESET_MSK) >> PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_SW_RESET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_SW_RESET_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_RESET_HISTORY_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_RESET_HISTORY_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_RESET_HISTORY_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_RESET_HISTORY_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_RESET_HISTORY_set", A, value );

    /* (0x00000510 bits 20) field CSU0_RESET_HISTORY of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1 */
    s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_RESET_HISTORY_MSK,
                                                         PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_RESET_HISTORY_OFF,
                                                         value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_RESET_HISTORY_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_RESET_HISTORY_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_RESET_HISTORY_get", A, 15);
    /* (0x00000510 bits 20) field CSU0_RESET_HISTORY of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_RESET_HISTORY_MSK) >> PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_RESET_HISTORY_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_RESET_HISTORY_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_CLK_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_CLK_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CLK_CNT_MAX_set", A, 15);
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_CLK_CNT_MAX_set", value, 1048575);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CLK_CNT_MAX_set", A, value );

    /* (0x00000510 bits 19:0) bits 0:19 use field CSU0_CLK_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1 */
    s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_CLK_CNT_MAX_MSK,
                                                         PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_CLK_CNT_MAX_OFF,
                                                         value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_CLK_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_CLK_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CLK_CNT_MAX_get", A, 15);
    /* (0x00000510 bits 19:0) bits 0:19 use field CSU0_CLK_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_CLK_CNT_MAX_MSK) >> PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_CLK_CNT_MAX_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CLK_CNT_MAX_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_CLK_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_CLK_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CLK_CNT_MAX_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CLK_CNT_MAX_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CLK_CNT_MAX_set", stop_bit, 19 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CLK_CNT_MAX_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000510 bits 19:0) bits 0:19 use field CSU0_CLK_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1 */
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
        /* (0x00000510 bits 19:0) bits 0:19 use field CSU0_CLK_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1 */
        s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_CLK_CNT_MAX_OFF + subfield_offset),
                                                             PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_CLK_CNT_MAX_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CLK_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CLK_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CLK_CNT_MAX_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CLK_CNT_MAX_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CLK_CNT_MAX_get", stop_bit, 19 );
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
    /* (0x00000510 bits 19:0) bits 0:19 use field CSU0_CLK_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_CLK_CNT_MAX_MSK)
                  >> PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_CLK_CNT_MAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_CLK_CNT_MAX_MSK, PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_CLK_CNT_MAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CLK_CNT_MAX_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_PASS_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_PASS_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_PASS_CNT_MAX_set", A, 15);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_PASS_CNT_MAX_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_PASS_CNT_MAX_set", A, value );

    /* (0x00000514 bits 31:16) bits 0:15 use field CSU0_PASS_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_PASS_CNT_MAX_MSK,
                                                         PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_PASS_CNT_MAX_OFF,
                                                         value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_PASS_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_PASS_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_PASS_CNT_MAX_get", A, 15);
    /* (0x00000514 bits 31:16) bits 0:15 use field CSU0_PASS_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_PASS_CNT_MAX_MSK) >> PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_PASS_CNT_MAX_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_PASS_CNT_MAX_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_PASS_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_PASS_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_PASS_CNT_MAX_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_PASS_CNT_MAX_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_PASS_CNT_MAX_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_PASS_CNT_MAX_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000514 bits 31:16) bits 0:15 use field CSU0_PASS_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2 */
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
        /* (0x00000514 bits 31:16) bits 0:15 use field CSU0_PASS_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2 */
        s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_PASS_CNT_MAX_OFF + subfield_offset),
                                                             PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_PASS_CNT_MAX_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_PASS_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_PASS_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_PASS_CNT_MAX_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_PASS_CNT_MAX_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_PASS_CNT_MAX_get", stop_bit, 15 );
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
    /* (0x00000514 bits 31:16) bits 0:15 use field CSU0_PASS_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_PASS_CNT_MAX_MSK)
                  >> PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_PASS_CNT_MAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_PASS_CNT_MAX_MSK, PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_PASS_CNT_MAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_PASS_CNT_MAX_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_FAIL_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_FAIL_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_FAIL_CNT_MAX_set", A, 15);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_FAIL_CNT_MAX_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_FAIL_CNT_MAX_set", A, value );

    /* (0x00000514 bits 15:0) bits 0:15 use field CSU0_FAIL_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_FAIL_CNT_MAX_MSK,
                                                         PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_FAIL_CNT_MAX_OFF,
                                                         value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_FAIL_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_FAIL_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_FAIL_CNT_MAX_get", A, 15);
    /* (0x00000514 bits 15:0) bits 0:15 use field CSU0_FAIL_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_FAIL_CNT_MAX_MSK) >> PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_FAIL_CNT_MAX_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_FAIL_CNT_MAX_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_FAIL_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_FAIL_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_FAIL_CNT_MAX_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_FAIL_CNT_MAX_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_FAIL_CNT_MAX_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_FAIL_CNT_MAX_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000514 bits 15:0) bits 0:15 use field CSU0_FAIL_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2 */
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
        /* (0x00000514 bits 15:0) bits 0:15 use field CSU0_FAIL_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2 */
        s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_FAIL_CNT_MAX_OFF + subfield_offset),
                                                             PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_FAIL_CNT_MAX_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_FAIL_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_FAIL_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_FAIL_CNT_MAX_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_FAIL_CNT_MAX_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_FAIL_CNT_MAX_get", stop_bit, 15 );
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
    /* (0x00000514 bits 15:0) bits 0:15 use field CSU0_FAIL_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_FAIL_CNT_MAX_MSK)
                  >> PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_FAIL_CNT_MAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_FAIL_CNT_MAX_MSK, PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_FAIL_CNT_MAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_FAIL_CNT_MAX_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_LOCK_PRECISION_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_LOCK_PRECISION_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_LOCK_PRECISION_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_PRECISION_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_PRECISION_set", A, value );

    /* (0x00000518 bits 31:28) bits 0:3 use field CSU0_LOCK_PRECISION of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3 */
    s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_PRECISION_MSK,
                                                         PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_PRECISION_OFF,
                                                         value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_LOCK_PRECISION_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_LOCK_PRECISION_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_LOCK_PRECISION_get", A, 15);
    /* (0x00000518 bits 31:28) bits 0:3 use field CSU0_LOCK_PRECISION of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_PRECISION_MSK) >> PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_PRECISION_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_PRECISION_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_LOCK_PRECISION_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_LOCK_PRECISION_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_LOCK_PRECISION_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_LOCK_PRECISION_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_LOCK_PRECISION_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_LOCK_PRECISION_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000518 bits 31:28) bits 0:3 use field CSU0_LOCK_PRECISION of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3 */
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
        /* (0x00000518 bits 31:28) bits 0:3 use field CSU0_LOCK_PRECISION of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3 */
        s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_PRECISION_OFF + subfield_offset),
                                                             PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_PRECISION_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_LOCK_PRECISION_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_LOCK_PRECISION_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_LOCK_PRECISION_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_LOCK_PRECISION_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_LOCK_PRECISION_get", stop_bit, 3 );
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
    /* (0x00000518 bits 31:28) bits 0:3 use field CSU0_LOCK_PRECISION of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_PRECISION_MSK)
                  >> PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_PRECISION_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_PRECISION_MSK, PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_PRECISION_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_LOCK_PRECISION_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_LOCK_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_LOCK_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_LOCK_CNT_MAX_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_CNT_MAX_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_CNT_MAX_set", A, value );

    /* (0x00000518 bits 27:20) bits 0:7 use field CSU0_LOCK_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3 */
    s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_CNT_MAX_MSK,
                                                         PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_CNT_MAX_OFF,
                                                         value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_LOCK_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_LOCK_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_LOCK_CNT_MAX_get", A, 15);
    /* (0x00000518 bits 27:20) bits 0:7 use field CSU0_LOCK_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_CNT_MAX_MSK) >> PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_CNT_MAX_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_CNT_MAX_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_LOCK_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_LOCK_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_LOCK_CNT_MAX_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_LOCK_CNT_MAX_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_LOCK_CNT_MAX_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_LOCK_CNT_MAX_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000518 bits 27:20) bits 0:7 use field CSU0_LOCK_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3 */
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
        /* (0x00000518 bits 27:20) bits 0:7 use field CSU0_LOCK_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3 */
        s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_CNT_MAX_OFF + subfield_offset),
                                                             PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_CNT_MAX_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_LOCK_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_LOCK_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_LOCK_CNT_MAX_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_LOCK_CNT_MAX_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_LOCK_CNT_MAX_get", stop_bit, 7 );
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
    /* (0x00000518 bits 27:20) bits 0:7 use field CSU0_LOCK_CNT_MAX of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_CNT_MAX_MSK)
                  >> PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_CNT_MAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_CNT_MAX_MSK, PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_CNT_MAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_LOCK_CNT_MAX_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_VBIAS_CSU_ATMSB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_VBIAS_CSU_ATMSB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_VBIAS_CSU_ATMSB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_VBIAS_CSU_ATMSB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_VBIAS_CSU_ATMSB_set", A, value );

    /* (0x0000051c bits 17) field VBIAS_CSU_ATMSB of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_VBIAS_CSU_ATMSB_MSK,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_VBIAS_CSU_ATMSB_OFF,
                                                          value);
}

static INLINE UINT32 s16_csu_pcbi_field_VBIAS_CSU_ATMSB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_VBIAS_CSU_ATMSB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_VBIAS_CSU_ATMSB_get", A, 15);
    /* (0x0000051c bits 17) field VBIAS_CSU_ATMSB of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_VBIAS_CSU_ATMSB_MSK) >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_VBIAS_CSU_ATMSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_VBIAS_CSU_ATMSB_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU_ENB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                   s16_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU_ENB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                   s16_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_ENB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU_ENB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU_ENB_set", A, value );

    /* (0x0000051c bits 16) field CSU_ENB of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_ENB_MSK,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_ENB_OFF,
                                                          value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU_ENB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                     s16_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU_ENB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                     s16_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_ENB_get", A, 15);
    /* (0x0000051c bits 16) field CSU_ENB of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_ENB_MSK) >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_ENB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU_ENB_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU_CTRL_15_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU_CTRL_15_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_15_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_15_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_15_set", A, value );

    /* (0x0000051c bits 15) field CSU_CTRL_15 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_15_MSK,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_15_OFF,
                                                          value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_15_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_15_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_15_get", A, 15);
    /* (0x0000051c bits 15) field CSU_CTRL_15 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_15_MSK) >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_15_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_15_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU_CTRL_14_13_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU_CTRL_14_13_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_14_13_set", A, 15);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_14_13_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_14_13_set", A, value );

    /* (0x0000051c bits 14:13) bits 0:1 use field CSU_CTRL_14_13 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_14_13_MSK,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_14_13_OFF,
                                                          value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_14_13_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_14_13_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_14_13_get", A, 15);
    /* (0x0000051c bits 14:13) bits 0:1 use field CSU_CTRL_14_13 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_14_13_MSK) >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_14_13_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_14_13_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU_CTRL_14_13_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU_CTRL_14_13_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU_CTRL_14_13_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU_CTRL_14_13_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU_CTRL_14_13_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU_CTRL_14_13_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000051c bits 14:13) bits 0:1 use field CSU_CTRL_14_13 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
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
        /* (0x0000051c bits 14:13) bits 0:1 use field CSU_CTRL_14_13 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
        s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              subfield_mask << (PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_14_13_OFF + subfield_offset),
                                                              PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_14_13_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU_CTRL_14_13_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU_CTRL_14_13_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU_CTRL_14_13_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU_CTRL_14_13_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU_CTRL_14_13_get", stop_bit, 1 );
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
    /* (0x0000051c bits 14:13) bits 0:1 use field CSU_CTRL_14_13 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    field_value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_14_13_MSK)
                  >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_14_13_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_14_13_MSK, PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_14_13_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU_CTRL_14_13_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU_CTRL_12_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU_CTRL_12_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_12_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_12_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_12_set", A, value );

    /* (0x0000051c bits 12) field CSU_CTRL_12 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_12_MSK,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_12_OFF,
                                                          value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_12_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_12_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_12_get", A, 15);
    /* (0x0000051c bits 12) field CSU_CTRL_12 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_12_MSK) >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_12_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_12_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU_CTRL_11_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU_CTRL_11_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_11_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_11_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_11_set", A, value );

    /* (0x0000051c bits 11) field CSU_CTRL_11 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_11_MSK,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_11_OFF,
                                                          value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_11_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_11_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_11_get", A, 15);
    /* (0x0000051c bits 11) field CSU_CTRL_11 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_11_MSK) >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_11_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_11_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU_CTRL_10_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU_CTRL_10_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_10_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_10_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_10_set", A, value );

    /* (0x0000051c bits 10) field CSU_CTRL_10 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_10_MSK,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_10_OFF,
                                                          value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_10_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_10_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_10_get", A, 15);
    /* (0x0000051c bits 10) field CSU_CTRL_10 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_10_MSK) >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_10_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_10_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU_CTRL_9_6_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU_CTRL_9_6_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_9_6_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_9_6_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_9_6_set", A, value );

    /* (0x0000051c bits 9:6) bits 0:3 use field CSU_CTRL_9_6 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_9_6_MSK,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_9_6_OFF,
                                                          value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_9_6_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_9_6_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_9_6_get", A, 15);
    /* (0x0000051c bits 9:6) bits 0:3 use field CSU_CTRL_9_6 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_9_6_MSK) >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_9_6_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_9_6_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU_CTRL_9_6_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU_CTRL_9_6_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU_CTRL_9_6_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU_CTRL_9_6_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU_CTRL_9_6_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU_CTRL_9_6_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000051c bits 9:6) bits 0:3 use field CSU_CTRL_9_6 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
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
        /* (0x0000051c bits 9:6) bits 0:3 use field CSU_CTRL_9_6 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
        s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              subfield_mask << (PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_9_6_OFF + subfield_offset),
                                                              PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_9_6_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU_CTRL_9_6_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU_CTRL_9_6_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU_CTRL_9_6_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU_CTRL_9_6_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU_CTRL_9_6_get", stop_bit, 3 );
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
    /* (0x0000051c bits 9:6) bits 0:3 use field CSU_CTRL_9_6 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    field_value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_9_6_MSK)
                  >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_9_6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_9_6_MSK, PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_9_6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU_CTRL_9_6_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU_CTRL_5_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU_CTRL_5_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_5_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_5_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_5_set", A, value );

    /* (0x0000051c bits 5) field CSU_CTRL_5 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_5_MSK,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_5_OFF,
                                                          value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_5_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_5_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_5_get", A, 15);
    /* (0x0000051c bits 5) field CSU_CTRL_5 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_5_MSK) >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_5_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_5_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU_CTRL_4_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU_CTRL_4_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_4_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_4_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_4_set", A, value );

    /* (0x0000051c bits 4) field CSU_CTRL_4 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_4_MSK,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_4_OFF,
                                                          value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_4_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_4_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_4_get", A, 15);
    /* (0x0000051c bits 4) field CSU_CTRL_4 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_4_MSK) >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_4_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_4_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU_CTRL_3_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU_CTRL_3_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_3_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_3_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_3_set", A, value );

    /* (0x0000051c bits 3) field CSU_CTRL_3 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_3_MSK,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_3_OFF,
                                                          value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_3_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_3_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_3_get", A, 15);
    /* (0x0000051c bits 3) field CSU_CTRL_3 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_3_MSK) >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_3_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_3_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU_CTRL_2_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU_CTRL_2_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_2_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_2_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_2_set", A, value );

    /* (0x0000051c bits 2) field CSU_CTRL_2 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_2_MSK,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_2_OFF,
                                                          value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_2_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_2_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_2_get", A, 15);
    /* (0x0000051c bits 2) field CSU_CTRL_2 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_2_MSK) >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_2_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_2_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU_CTRL_1_0_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU_CTRL_1_0_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_1_0_set", A, 15);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_1_0_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_1_0_set", A, value );

    /* (0x0000051c bits 1:0) bits 0:1 use field CSU_CTRL_1_0 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_1_0_MSK,
                                                          PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_1_0_OFF,
                                                          value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_1_0_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU_CTRL_1_0_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_CTRL_1_0_get", A, 15);
    /* (0x0000051c bits 1:0) bits 0:1 use field CSU_CTRL_1_0 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_1_0_MSK) >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_1_0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU_CTRL_1_0_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU_CTRL_1_0_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU_CTRL_1_0_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU_CTRL_1_0_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU_CTRL_1_0_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU_CTRL_1_0_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU_CTRL_1_0_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000051c bits 1:0) bits 0:1 use field CSU_CTRL_1_0 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
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
        /* (0x0000051c bits 1:0) bits 0:1 use field CSU_CTRL_1_0 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
        s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              subfield_mask << (PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_1_0_OFF + subfield_offset),
                                                              PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_1_0_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU_CTRL_1_0_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU_CTRL_1_0_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU_CTRL_1_0_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU_CTRL_1_0_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU_CTRL_1_0_get", stop_bit, 1 );
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
    /* (0x0000051c bits 1:0) bits 0:1 use field CSU_CTRL_1_0 of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    field_value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_1_0_MSK)
                  >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_1_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_1_0_MSK, PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_1_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU_CTRL_1_0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_29_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_29_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_29_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_29_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_29_set", A, value );

    /* (0x00000520 bits 29) field CSU0_CSU_MODE_29 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_29_MSK,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_29_OFF,
                                                 value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_29_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_29_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_29_get", A, 15);
    /* (0x00000520 bits 29) field CSU0_CSU_MODE_29 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_29_MSK) >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_29_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_29_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_28_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_28_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_28_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_28_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_28_set", A, value );

    /* (0x00000520 bits 28) field CSU0_CSU_MODE_28 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_28_MSK,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_28_OFF,
                                                 value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_28_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_28_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_28_get", A, 15);
    /* (0x00000520 bits 28) field CSU0_CSU_MODE_28 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_28_MSK) >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_28_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_28_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_27_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_27_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_27_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_27_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_27_set", A, value );

    /* (0x00000520 bits 27) field CSU0_CSU_MODE_27 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_27_MSK,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_27_OFF,
                                                 value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_27_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_27_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_27_get", A, 15);
    /* (0x00000520 bits 27) field CSU0_CSU_MODE_27 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_27_MSK) >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_27_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_27_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_26_25_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_26_25_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_26_25_set", A, 15);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_26_25_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_26_25_set", A, value );

    /* (0x00000520 bits 26:25) bits 0:1 use field CSU0_CSU_MODE_26_25 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_26_25_MSK,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_26_25_OFF,
                                                 value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_26_25_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_26_25_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_26_25_get", A, 15);
    /* (0x00000520 bits 26:25) bits 0:1 use field CSU0_CSU_MODE_26_25 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_26_25_MSK) >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_26_25_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_26_25_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_CSU_MODE_26_25_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_CSU_MODE_26_25_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_26_25_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_26_25_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_26_25_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_26_25_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000520 bits 26:25) bits 0:1 use field CSU0_CSU_MODE_26_25 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
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
        /* (0x00000520 bits 26:25) bits 0:1 use field CSU0_CSU_MODE_26_25 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
        s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     subfield_mask << (PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_26_25_OFF + subfield_offset),
                                                     PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_26_25_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_26_25_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_26_25_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_26_25_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_26_25_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_26_25_get", stop_bit, 1 );
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
    /* (0x00000520 bits 26:25) bits 0:1 use field CSU0_CSU_MODE_26_25 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_26_25_MSK)
                  >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_26_25_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_26_25_MSK, PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_26_25_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_26_25_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_24_23_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_24_23_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_24_23_set", A, 15);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_24_23_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_24_23_set", A, value );

    /* (0x00000520 bits 24:23) bits 0:1 use field CSU0_CSU_MODE_24_23 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_24_23_MSK,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_24_23_OFF,
                                                 value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_24_23_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_24_23_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_24_23_get", A, 15);
    /* (0x00000520 bits 24:23) bits 0:1 use field CSU0_CSU_MODE_24_23 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_24_23_MSK) >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_24_23_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_24_23_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_CSU_MODE_24_23_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_CSU_MODE_24_23_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_24_23_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_24_23_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_24_23_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_24_23_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000520 bits 24:23) bits 0:1 use field CSU0_CSU_MODE_24_23 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
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
        /* (0x00000520 bits 24:23) bits 0:1 use field CSU0_CSU_MODE_24_23 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
        s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     subfield_mask << (PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_24_23_OFF + subfield_offset),
                                                     PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_24_23_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_24_23_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_24_23_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_24_23_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_24_23_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_24_23_get", stop_bit, 1 );
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
    /* (0x00000520 bits 24:23) bits 0:1 use field CSU0_CSU_MODE_24_23 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_24_23_MSK)
                  >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_24_23_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_24_23_MSK, PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_24_23_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_24_23_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_22_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_22_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_22_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_22_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_22_set", A, value );

    /* (0x00000520 bits 22) field CSU0_CSU_MODE_22 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_22_MSK,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_22_OFF,
                                                 value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_22_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_22_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_22_get", A, 15);
    /* (0x00000520 bits 22) field CSU0_CSU_MODE_22 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_22_MSK) >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_22_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_22_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_21_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_21_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_21_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_21_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_21_set", A, value );

    /* (0x00000520 bits 21) field CSU0_CSU_MODE_21 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_21_MSK,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_21_OFF,
                                                 value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_21_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_21_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_21_get", A, 15);
    /* (0x00000520 bits 21) field CSU0_CSU_MODE_21 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_21_MSK) >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_21_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_21_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_20_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_20_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_20_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_20_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_20_set", A, value );

    /* (0x00000520 bits 20) field CSU0_CSU_MODE_20 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_20_MSK,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_20_OFF,
                                                 value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_20_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_20_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_20_get", A, 15);
    /* (0x00000520 bits 20) field CSU0_CSU_MODE_20 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_20_MSK) >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_20_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_20_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_19_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_19_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_19_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_19_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_19_set", A, value );

    /* (0x00000520 bits 19) field CSU0_CSU_MODE_19 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_19_MSK,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_19_OFF,
                                                 value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_19_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_19_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_19_get", A, 15);
    /* (0x00000520 bits 19) field CSU0_CSU_MODE_19 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_19_MSK) >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_19_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_19_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_17_16_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_17_16_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_17_16_set", A, 15);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_17_16_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_17_16_set", A, value );

    /* (0x00000520 bits 17:16) field CSU0_CSU_MODE_17_16 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_17_16_MSK,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_17_16_OFF,
                                                 value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_17_16_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_17_16_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_17_16_get", A, 15);
    /* (0x00000520 bits 17:16) field CSU0_CSU_MODE_17_16 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_17_16_MSK) >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_17_16_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_17_16_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_CSU_MODE_17_16_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_CSU_MODE_17_16_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_17_16_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_17_16_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_17_16_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_17_16_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000520 bits 17:16) field CSU0_CSU_MODE_17_16 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
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
        /* (0x00000520 bits 17:16) field CSU0_CSU_MODE_17_16 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
        s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     subfield_mask << (PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_17_16_OFF + subfield_offset),
                                                     PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_17_16_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_17_16_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_17_16_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_17_16_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_17_16_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_17_16_get", stop_bit, 1 );
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
    /* (0x00000520 bits 17:16) field CSU0_CSU_MODE_17_16 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_17_16_MSK)
                  >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_17_16_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_17_16_MSK, PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_17_16_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_17_16_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_15_10_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_15_10_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_15_10_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_15_10_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_15_10_set", A, value );

    /* (0x00000520 bits 15:10) bits 0:5 use field CSU0_CSU_MODE_15_10 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_15_10_MSK,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_15_10_OFF,
                                                 value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_15_10_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_15_10_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_15_10_get", A, 15);
    /* (0x00000520 bits 15:10) bits 0:5 use field CSU0_CSU_MODE_15_10 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_15_10_MSK) >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_15_10_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_15_10_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_CSU_MODE_15_10_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_CSU_MODE_15_10_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_15_10_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_15_10_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_15_10_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_15_10_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000520 bits 15:10) bits 0:5 use field CSU0_CSU_MODE_15_10 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
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
        /* (0x00000520 bits 15:10) bits 0:5 use field CSU0_CSU_MODE_15_10 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
        s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     subfield_mask << (PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_15_10_OFF + subfield_offset),
                                                     PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_15_10_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_15_10_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_15_10_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_15_10_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_15_10_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_15_10_get", stop_bit, 5 );
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
    /* (0x00000520 bits 15:10) bits 0:5 use field CSU0_CSU_MODE_15_10 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_15_10_MSK)
                  >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_15_10_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_15_10_MSK, PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_15_10_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_15_10_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_9_8_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_9_8_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_9_8_set", A, 15);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_9_8_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_9_8_set", A, value );

    /* (0x00000520 bits 9:8) bits 0:1 use field CSU0_CSU_MODE_9_8 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_9_8_MSK,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_9_8_OFF,
                                                 value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_9_8_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_9_8_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_9_8_get", A, 15);
    /* (0x00000520 bits 9:8) bits 0:1 use field CSU0_CSU_MODE_9_8 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_9_8_MSK) >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_9_8_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_9_8_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_CSU_MODE_9_8_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_CSU_MODE_9_8_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_9_8_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_9_8_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_9_8_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_9_8_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000520 bits 9:8) bits 0:1 use field CSU0_CSU_MODE_9_8 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
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
        /* (0x00000520 bits 9:8) bits 0:1 use field CSU0_CSU_MODE_9_8 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
        s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     subfield_mask << (PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_9_8_OFF + subfield_offset),
                                                     PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_9_8_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_9_8_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_9_8_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_9_8_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_9_8_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_9_8_get", stop_bit, 1 );
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
    /* (0x00000520 bits 9:8) bits 0:1 use field CSU0_CSU_MODE_9_8 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_9_8_MSK)
                  >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_9_8_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_9_8_MSK, PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_9_8_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_9_8_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_7_6_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_7_6_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_7_6_set", A, 15);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_7_6_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_7_6_set", A, value );

    /* (0x00000520 bits 7:6) bits 0:1 use field CSU0_CSU_MODE_7_6 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_7_6_MSK,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_7_6_OFF,
                                                 value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_7_6_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_7_6_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_7_6_get", A, 15);
    /* (0x00000520 bits 7:6) bits 0:1 use field CSU0_CSU_MODE_7_6 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_7_6_MSK) >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_7_6_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_7_6_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_CSU_MODE_7_6_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_CSU_MODE_7_6_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_7_6_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_7_6_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_7_6_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_7_6_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000520 bits 7:6) bits 0:1 use field CSU0_CSU_MODE_7_6 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
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
        /* (0x00000520 bits 7:6) bits 0:1 use field CSU0_CSU_MODE_7_6 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
        s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     subfield_mask << (PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_7_6_OFF + subfield_offset),
                                                     PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_7_6_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_7_6_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_7_6_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_7_6_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_7_6_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_7_6_get", stop_bit, 1 );
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
    /* (0x00000520 bits 7:6) bits 0:1 use field CSU0_CSU_MODE_7_6 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_7_6_MSK)
                  >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_7_6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_7_6_MSK, PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_7_6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_7_6_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_5_3_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_5_3_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_5_3_set", A, 15);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_5_3_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_5_3_set", A, value );

    /* (0x00000520 bits 5:3) bits 0:2 use field CSU0_CSU_MODE_5_3 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_5_3_MSK,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_5_3_OFF,
                                                 value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_5_3_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_5_3_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_5_3_get", A, 15);
    /* (0x00000520 bits 5:3) bits 0:2 use field CSU0_CSU_MODE_5_3 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_5_3_MSK) >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_5_3_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_5_3_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_CSU_MODE_5_3_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_CSU_MODE_5_3_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_5_3_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_5_3_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_5_3_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_5_3_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000520 bits 5:3) bits 0:2 use field CSU0_CSU_MODE_5_3 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
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
        /* (0x00000520 bits 5:3) bits 0:2 use field CSU0_CSU_MODE_5_3 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
        s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     subfield_mask << (PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_5_3_OFF + subfield_offset),
                                                     PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_5_3_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_5_3_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_5_3_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_5_3_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_5_3_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_5_3_get", stop_bit, 2 );
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
    /* (0x00000520 bits 5:3) bits 0:2 use field CSU0_CSU_MODE_5_3 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_5_3_MSK)
                  >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_5_3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_5_3_MSK, PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_5_3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_5_3_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_2_0_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_CSU_MODE_2_0_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_2_0_set", A, 15);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_2_0_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_2_0_set", A, value );

    /* (0x00000520 bits 2:0) bits 0:2 use field CSU0_CSU_MODE_2_0 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_2_0_MSK,
                                                 PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_2_0_OFF,
                                                 value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_2_0_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_2_0_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_MODE_2_0_get", A, 15);
    /* (0x00000520 bits 2:0) bits 0:2 use field CSU0_CSU_MODE_2_0 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_2_0_MSK) >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_2_0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_MODE_2_0_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_CSU_MODE_2_0_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_CSU_MODE_2_0_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_2_0_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_2_0_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_2_0_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_2_0_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000520 bits 2:0) bits 0:2 use field CSU0_CSU_MODE_2_0 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
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
        /* (0x00000520 bits 2:0) bits 0:2 use field CSU0_CSU_MODE_2_0 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
        s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     subfield_mask << (PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_2_0_OFF + subfield_offset),
                                                     PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_2_0_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_2_0_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_2_0_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_2_0_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_2_0_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_2_0_get", stop_bit, 2 );
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
    /* (0x00000520 bits 2:0) bits 0:2 use field CSU0_CSU_MODE_2_0 of register PMC_PCBI_REG_CSU0_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_2_0_MSK)
                  >> PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_2_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_2_0_MSK, PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_2_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CSU_MODE_2_0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_OVR_EN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_OVR_EN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_OVR_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_OVR_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_OVR_EN_set", A, value );

    /* (0x00000524 bits 4) field CSU0_CSU_FB_CTRL_OVR_EN of register PMC_PCBI_REG_CSU0_CONFIG_REG3 */
    s16_csu_pcbi_reg_CSU0_CONFIG_REG3_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 PCBI_REG_CSU0_CONFIG_REG3_BIT_CSU0_CSU_FB_CTRL_OVR_EN_MSK,
                                                 PCBI_REG_CSU0_CONFIG_REG3_BIT_CSU0_CSU_FB_CTRL_OVR_EN_OFF,
                                                 value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_OVR_EN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_OVR_EN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_OVR_EN_get", A, 15);
    /* (0x00000524 bits 4) field CSU0_CSU_FB_CTRL_OVR_EN of register PMC_PCBI_REG_CSU0_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG3_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & PCBI_REG_CSU0_CONFIG_REG3_BIT_CSU0_CSU_FB_CTRL_OVR_EN_MSK) >> PCBI_REG_CSU0_CONFIG_REG3_BIT_CSU0_CSU_FB_CTRL_OVR_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_OVR_EN_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_set", A, value );

    /* (0x00000524 bits 3:0) bits 0:3 use field CSU0_CSU_FB_CTRL of register PMC_PCBI_REG_CSU0_CONFIG_REG3 */
    s16_csu_pcbi_reg_CSU0_CONFIG_REG3_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 PCBI_REG_CSU0_CONFIG_REG3_BIT_CSU0_CSU_FB_CTRL_MSK,
                                                 PCBI_REG_CSU0_CONFIG_REG3_BIT_CSU0_CSU_FB_CTRL_OFF,
                                                 value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_get", A, 15);
    /* (0x00000524 bits 3:0) bits 0:3 use field CSU0_CSU_FB_CTRL of register PMC_PCBI_REG_CSU0_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG3_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & PCBI_REG_CSU0_CONFIG_REG3_BIT_CSU0_CSU_FB_CTRL_MSK) >> PCBI_REG_CSU0_CONFIG_REG3_BIT_CSU0_CSU_FB_CTRL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_CSU_FB_CTRL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_CSU_FB_CTRL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CSU_FB_CTRL_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CSU_FB_CTRL_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CSU_FB_CTRL_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CSU_FB_CTRL_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000524 bits 3:0) bits 0:3 use field CSU0_CSU_FB_CTRL of register PMC_PCBI_REG_CSU0_CONFIG_REG3 */
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
        /* (0x00000524 bits 3:0) bits 0:3 use field CSU0_CSU_FB_CTRL of register PMC_PCBI_REG_CSU0_CONFIG_REG3 */
        s16_csu_pcbi_reg_CSU0_CONFIG_REG3_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     subfield_mask << (PCBI_REG_CSU0_CONFIG_REG3_BIT_CSU0_CSU_FB_CTRL_OFF + subfield_offset),
                                                     PCBI_REG_CSU0_CONFIG_REG3_BIT_CSU0_CSU_FB_CTRL_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CSU_FB_CTRL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_CSU_FB_CTRL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_CSU_FB_CTRL_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_CSU_FB_CTRL_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_CSU_FB_CTRL_get", stop_bit, 3 );
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
    /* (0x00000524 bits 3:0) bits 0:3 use field CSU0_CSU_FB_CTRL of register PMC_PCBI_REG_CSU0_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_CSU0_CONFIG_REG3_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & PCBI_REG_CSU0_CONFIG_REG3_BIT_CSU0_CSU_FB_CTRL_MSK)
                  >> PCBI_REG_CSU0_CONFIG_REG3_BIT_CSU0_CSU_FB_CTRL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_CONFIG_REG3_BIT_CSU0_CSU_FB_CTRL_MSK, PCBI_REG_CSU0_CONFIG_REG3_BIT_CSU0_CSU_FB_CTRL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_CSU_FB_CTRL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_VC_LPF_CODE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_VC_LPF_CODE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VC_LPF_CODE_set", A, 15);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_VC_LPF_CODE_set", value, 31);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VC_LPF_CODE_set", A, value );

    /* (0x00000528 bits 8:4) bits 0:4 use field CSU0_VC_LPF_CODE of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3 */
    s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_CODE_MSK,
                                                           PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_CODE_OFF,
                                                           value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_VC_LPF_CODE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_VC_LPF_CODE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VC_LPF_CODE_get", A, 15);
    /* (0x00000528 bits 8:4) bits 0:4 use field CSU0_VC_LPF_CODE of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_CODE_MSK) >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_CODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VC_LPF_CODE_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_VC_LPF_CODE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_VC_LPF_CODE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_VC_LPF_CODE_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_VC_LPF_CODE_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_VC_LPF_CODE_set", stop_bit, 4 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_VC_LPF_CODE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000528 bits 8:4) bits 0:4 use field CSU0_VC_LPF_CODE of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3 */
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
        /* (0x00000528 bits 8:4) bits 0:4 use field CSU0_VC_LPF_CODE of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3 */
        s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_field_set( b_ptr,
                                                               h_ptr,
                                                               A,
                                                               subfield_mask << (PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_CODE_OFF + subfield_offset),
                                                               PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_CODE_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_VC_LPF_CODE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_VC_LPF_CODE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_VC_LPF_CODE_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_VC_LPF_CODE_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_VC_LPF_CODE_get", stop_bit, 4 );
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
    /* (0x00000528 bits 8:4) bits 0:4 use field CSU0_VC_LPF_CODE of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    field_value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_CODE_MSK)
                  >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_CODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_CODE_MSK, PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_CODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_VC_LPF_CODE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_VC_LPF_MODE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_VC_LPF_MODE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VC_LPF_MODE_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_VC_LPF_MODE_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VC_LPF_MODE_set", A, value );

    /* (0x00000528 bits 3:0) bits 0:3 use field CSU0_VC_LPF_MODE of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3 */
    s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_MODE_MSK,
                                                           PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_MODE_OFF,
                                                           value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_VC_LPF_MODE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_VC_LPF_MODE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VC_LPF_MODE_get", A, 15);
    /* (0x00000528 bits 3:0) bits 0:3 use field CSU0_VC_LPF_MODE of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_MODE_MSK) >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VC_LPF_MODE_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_VC_LPF_MODE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_VC_LPF_MODE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_VC_LPF_MODE_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_VC_LPF_MODE_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_VC_LPF_MODE_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_VC_LPF_MODE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000528 bits 3:0) bits 0:3 use field CSU0_VC_LPF_MODE of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3 */
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
        /* (0x00000528 bits 3:0) bits 0:3 use field CSU0_VC_LPF_MODE of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3 */
        s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_field_set( b_ptr,
                                                               h_ptr,
                                                               A,
                                                               subfield_mask << (PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_MODE_OFF + subfield_offset),
                                                               PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_MODE_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_VC_LPF_MODE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_VC_LPF_MODE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_VC_LPF_MODE_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_VC_LPF_MODE_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_VC_LPF_MODE_get", stop_bit, 3 );
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
    /* (0x00000528 bits 3:0) bits 0:3 use field CSU0_VC_LPF_MODE of register PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    field_value = (reg_value & PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_MODE_MSK)
                  >> PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_MODE_MSK, PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_VC_LPF_MODE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_MAX_THRESHOLD_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_MAX_THRESHOLD_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_MAX_THRESHOLD_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_MAX_THRESHOLD_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_MAX_THRESHOLD_set", A, value );

    /* (0x0000052c bits 17:12) field CSU0_MAX_THRESHOLD of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1 */
    s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MAX_THRESHOLD_MSK,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MAX_THRESHOLD_OFF,
                                                      value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_MAX_THRESHOLD_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_MAX_THRESHOLD_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_MAX_THRESHOLD_get", A, 15);
    /* (0x0000052c bits 17:12) field CSU0_MAX_THRESHOLD of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MAX_THRESHOLD_MSK) >> PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MAX_THRESHOLD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_MAX_THRESHOLD_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_MAX_THRESHOLD_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_MAX_THRESHOLD_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_MAX_THRESHOLD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_MAX_THRESHOLD_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_MAX_THRESHOLD_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_MAX_THRESHOLD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000052c bits 17:12) field CSU0_MAX_THRESHOLD of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1 */
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
        /* (0x0000052c bits 17:12) field CSU0_MAX_THRESHOLD of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1 */
        s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          subfield_mask << (PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MAX_THRESHOLD_OFF + subfield_offset),
                                                          PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MAX_THRESHOLD_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_MAX_THRESHOLD_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_MAX_THRESHOLD_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_MAX_THRESHOLD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_MAX_THRESHOLD_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_MAX_THRESHOLD_get", stop_bit, 5 );
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
    /* (0x0000052c bits 17:12) field CSU0_MAX_THRESHOLD of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MAX_THRESHOLD_MSK)
                  >> PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MAX_THRESHOLD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MAX_THRESHOLD_MSK, PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MAX_THRESHOLD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_MAX_THRESHOLD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_MIN_THRESHOLD_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_MIN_THRESHOLD_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_MIN_THRESHOLD_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_MIN_THRESHOLD_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_MIN_THRESHOLD_set", A, value );

    /* (0x0000052c bits 11:6) field CSU0_MIN_THRESHOLD of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1 */
    s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MIN_THRESHOLD_MSK,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MIN_THRESHOLD_OFF,
                                                      value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_MIN_THRESHOLD_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_MIN_THRESHOLD_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_MIN_THRESHOLD_get", A, 15);
    /* (0x0000052c bits 11:6) field CSU0_MIN_THRESHOLD of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MIN_THRESHOLD_MSK) >> PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MIN_THRESHOLD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_MIN_THRESHOLD_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_MIN_THRESHOLD_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_MIN_THRESHOLD_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_MIN_THRESHOLD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_MIN_THRESHOLD_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_MIN_THRESHOLD_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_MIN_THRESHOLD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000052c bits 11:6) field CSU0_MIN_THRESHOLD of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1 */
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
        /* (0x0000052c bits 11:6) field CSU0_MIN_THRESHOLD of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1 */
        s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          subfield_mask << (PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MIN_THRESHOLD_OFF + subfield_offset),
                                                          PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MIN_THRESHOLD_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_MIN_THRESHOLD_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_MIN_THRESHOLD_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_MIN_THRESHOLD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_MIN_THRESHOLD_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_MIN_THRESHOLD_get", stop_bit, 5 );
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
    /* (0x0000052c bits 11:6) field CSU0_MIN_THRESHOLD of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MIN_THRESHOLD_MSK)
                  >> PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MIN_THRESHOLD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MIN_THRESHOLD_MSK, PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MIN_THRESHOLD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_MIN_THRESHOLD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_VCOC_START_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_VCOC_START_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_START_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_START_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_START_set", A, value );

    /* (0x0000052c bits 5) field CSU0_VCOC_START of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1 */
    s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_START_MSK,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_START_OFF,
                                                      value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_START_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_START_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_START_get", A, 15);
    /* (0x0000052c bits 5) field CSU0_VCOC_START of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_START_MSK) >> PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_START_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_START_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_VCOC_LOOP_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_VCOC_LOOP_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_LOOP_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_LOOP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_LOOP_set", A, value );

    /* (0x0000052c bits 4:1) bits 0:3 use field CSU0_VCOC_LOOP of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1 */
    s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_LOOP_MSK,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_LOOP_OFF,
                                                      value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_LOOP_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_LOOP_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_LOOP_get", A, 15);
    /* (0x0000052c bits 4:1) bits 0:3 use field CSU0_VCOC_LOOP of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_LOOP_MSK) >> PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_LOOP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_LOOP_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_VCOC_LOOP_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_VCOC_LOOP_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_VCOC_LOOP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_VCOC_LOOP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_VCOC_LOOP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_VCOC_LOOP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000052c bits 4:1) bits 0:3 use field CSU0_VCOC_LOOP of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1 */
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
        /* (0x0000052c bits 4:1) bits 0:3 use field CSU0_VCOC_LOOP of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1 */
        s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          subfield_mask << (PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_LOOP_OFF + subfield_offset),
                                                          PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_LOOP_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_VCOC_LOOP_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_VCOC_LOOP_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_VCOC_LOOP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_VCOC_LOOP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_VCOC_LOOP_get", stop_bit, 3 );
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
    /* (0x0000052c bits 4:1) bits 0:3 use field CSU0_VCOC_LOOP of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_LOOP_MSK)
                  >> PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_LOOP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_LOOP_MSK, PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_LOOP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_VCOC_LOOP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_VCOC_EN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_VCOC_EN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_EN_set", A, value );

    /* (0x0000052c bits 0) field CSU0_VCOC_EN of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1 */
    s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_EN_MSK,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_EN_OFF,
                                                      value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_EN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_EN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_EN_get", A, 15);
    /* (0x0000052c bits 0) field CSU0_VCOC_EN of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_EN_MSK) >> PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_EN_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_VCOC_SETTLING_TIME_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_VCOC_SETTLING_TIME_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_SETTLING_TIME_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_SETTLING_TIME_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_SETTLING_TIME_set", A, value );

    /* (0x00000530 bits 31:24) field CSU0_VCOC_SETTLING_TIME of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_VCOC_SETTLING_TIME_MSK,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_VCOC_SETTLING_TIME_OFF,
                                                      value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_SETTLING_TIME_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_SETTLING_TIME_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_SETTLING_TIME_get", A, 15);
    /* (0x00000530 bits 31:24) field CSU0_VCOC_SETTLING_TIME of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_VCOC_SETTLING_TIME_MSK) >> PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_VCOC_SETTLING_TIME_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_SETTLING_TIME_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_VCOC_SETTLING_TIME_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_VCOC_SETTLING_TIME_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_VCOC_SETTLING_TIME_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_VCOC_SETTLING_TIME_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_VCOC_SETTLING_TIME_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_VCOC_SETTLING_TIME_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000530 bits 31:24) field CSU0_VCOC_SETTLING_TIME of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
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
        /* (0x00000530 bits 31:24) field CSU0_VCOC_SETTLING_TIME of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
        s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          subfield_mask << (PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_VCOC_SETTLING_TIME_OFF + subfield_offset),
                                                          PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_VCOC_SETTLING_TIME_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_VCOC_SETTLING_TIME_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_VCOC_SETTLING_TIME_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_VCOC_SETTLING_TIME_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_VCOC_SETTLING_TIME_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_VCOC_SETTLING_TIME_get", stop_bit, 7 );
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
    /* (0x00000530 bits 31:24) field CSU0_VCOC_SETTLING_TIME of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_VCOC_SETTLING_TIME_MSK)
                  >> PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_VCOC_SETTLING_TIME_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_VCOC_SETTLING_TIME_MSK, PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_VCOC_SETTLING_TIME_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_VCOC_SETTLING_TIME_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_MIN_MODE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_MIN_MODE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_MIN_MODE_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_MIN_MODE_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_MIN_MODE_set", A, value );

    /* (0x00000530 bits 23:18) field CSU0_MIN_MODE of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MIN_MODE_MSK,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MIN_MODE_OFF,
                                                      value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_MIN_MODE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_MIN_MODE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_MIN_MODE_get", A, 15);
    /* (0x00000530 bits 23:18) field CSU0_MIN_MODE of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MIN_MODE_MSK) >> PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MIN_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_MIN_MODE_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_MIN_MODE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_MIN_MODE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_MIN_MODE_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_MIN_MODE_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_MIN_MODE_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_MIN_MODE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000530 bits 23:18) field CSU0_MIN_MODE of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
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
        /* (0x00000530 bits 23:18) field CSU0_MIN_MODE of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
        s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          subfield_mask << (PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MIN_MODE_OFF + subfield_offset),
                                                          PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MIN_MODE_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_MIN_MODE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_MIN_MODE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_MIN_MODE_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_MIN_MODE_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_MIN_MODE_get", stop_bit, 5 );
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
    /* (0x00000530 bits 23:18) field CSU0_MIN_MODE of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MIN_MODE_MSK)
                  >> PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MIN_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MIN_MODE_MSK, PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MIN_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_MIN_MODE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_MAX_MODE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_MAX_MODE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_MAX_MODE_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_MAX_MODE_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_MAX_MODE_set", A, value );

    /* (0x00000530 bits 17:12) field CSU0_MAX_MODE of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MAX_MODE_MSK,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MAX_MODE_OFF,
                                                      value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_MAX_MODE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_MAX_MODE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_MAX_MODE_get", A, 15);
    /* (0x00000530 bits 17:12) field CSU0_MAX_MODE of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MAX_MODE_MSK) >> PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MAX_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_MAX_MODE_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_MAX_MODE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_MAX_MODE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_MAX_MODE_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_MAX_MODE_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_MAX_MODE_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_MAX_MODE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000530 bits 17:12) field CSU0_MAX_MODE of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
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
        /* (0x00000530 bits 17:12) field CSU0_MAX_MODE of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
        s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          subfield_mask << (PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MAX_MODE_OFF + subfield_offset),
                                                          PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MAX_MODE_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_MAX_MODE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_MAX_MODE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_MAX_MODE_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_MAX_MODE_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_MAX_MODE_get", stop_bit, 5 );
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
    /* (0x00000530 bits 17:12) field CSU0_MAX_MODE of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MAX_MODE_MSK)
                  >> PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MAX_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MAX_MODE_MSK, PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MAX_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_MAX_MODE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_VCOC_BIST_EN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_VCOC_BIST_EN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_BIST_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_BIST_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_BIST_EN_set", A, value );

    /* (0x00000530 bits 11) field CSU0_VCOC_BIST_EN of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_VCOC_BIST_EN_MSK,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_VCOC_BIST_EN_OFF,
                                                      value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_BIST_EN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_BIST_EN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_BIST_EN_get", A, 15);
    /* (0x00000530 bits 11) field CSU0_VCOC_BIST_EN of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_VCOC_BIST_EN_MSK) >> PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_VCOC_BIST_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_BIST_EN_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_OFF_EQ_WINDOW_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_OFF_EQ_WINDOW_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_OFF_EQ_WINDOW_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_OFF_EQ_WINDOW_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_OFF_EQ_WINDOW_set", A, value );

    /* (0x00000530 bits 10:5) field CSU0_OFF_EQ_WINDOW of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_OFF_EQ_WINDOW_MSK,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_OFF_EQ_WINDOW_OFF,
                                                      value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_OFF_EQ_WINDOW_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_OFF_EQ_WINDOW_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_OFF_EQ_WINDOW_get", A, 15);
    /* (0x00000530 bits 10:5) field CSU0_OFF_EQ_WINDOW of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_OFF_EQ_WINDOW_MSK) >> PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_OFF_EQ_WINDOW_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_OFF_EQ_WINDOW_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_OFF_EQ_WINDOW_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_OFF_EQ_WINDOW_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_OFF_EQ_WINDOW_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_OFF_EQ_WINDOW_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_OFF_EQ_WINDOW_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_OFF_EQ_WINDOW_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000530 bits 10:5) field CSU0_OFF_EQ_WINDOW of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
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
        /* (0x00000530 bits 10:5) field CSU0_OFF_EQ_WINDOW of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
        s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          subfield_mask << (PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_OFF_EQ_WINDOW_OFF + subfield_offset),
                                                          PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_OFF_EQ_WINDOW_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_OFF_EQ_WINDOW_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_OFF_EQ_WINDOW_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_OFF_EQ_WINDOW_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_OFF_EQ_WINDOW_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_OFF_EQ_WINDOW_get", stop_bit, 5 );
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
    /* (0x00000530 bits 10:5) field CSU0_OFF_EQ_WINDOW of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_OFF_EQ_WINDOW_MSK)
                  >> PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_OFF_EQ_WINDOW_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_OFF_EQ_WINDOW_MSK, PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_OFF_EQ_WINDOW_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_OFF_EQ_WINDOW_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_SWEEP_DIR_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_SWEEP_DIR_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_SWEEP_DIR_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_SWEEP_DIR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_SWEEP_DIR_set", A, value );

    /* (0x00000530 bits 4) field CSU0_SWEEP_DIR of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_SWEEP_DIR_MSK,
                                                      PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_SWEEP_DIR_OFF,
                                                      value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_SWEEP_DIR_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_SWEEP_DIR_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_SWEEP_DIR_get", A, 15);
    /* (0x00000530 bits 4) field CSU0_SWEEP_DIR of register PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_SWEEP_DIR_MSK) >> PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_SWEEP_DIR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_SWEEP_DIR_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_ADSM_SFT_RSTB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_ADSM_SFT_RSTB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_ADSM_SFT_RSTB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_ADSM_SFT_RSTB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_ADSM_SFT_RSTB_set", A, value );

    /* (0x00000534 bits 4) field CSU0_ADSM_SFT_RSTB of register PMC_PCBI_REG_CSU0_RESET_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_ADSM_SFT_RSTB_MSK,
                                                      PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_ADSM_SFT_RSTB_OFF,
                                                      value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_ADSM_SFT_RSTB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_ADSM_SFT_RSTB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_ADSM_SFT_RSTB_get", A, 15);
    /* (0x00000534 bits 4) field CSU0_ADSM_SFT_RSTB of register PMC_PCBI_REG_CSU0_RESET_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_ADSM_SFT_RSTB_MSK) >> PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_ADSM_SFT_RSTB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_ADSM_SFT_RSTB_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_ADSM_HRD_RSTB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_ADSM_HRD_RSTB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_ADSM_HRD_RSTB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_ADSM_HRD_RSTB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_ADSM_HRD_RSTB_set", A, value );

    /* (0x00000534 bits 3) field CSU0_ADSM_HRD_RSTB of register PMC_PCBI_REG_CSU0_RESET_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_ADSM_HRD_RSTB_MSK,
                                                      PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_ADSM_HRD_RSTB_OFF,
                                                      value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_ADSM_HRD_RSTB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_ADSM_HRD_RSTB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_ADSM_HRD_RSTB_get", A, 15);
    /* (0x00000534 bits 3) field CSU0_ADSM_HRD_RSTB of register PMC_PCBI_REG_CSU0_RESET_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_ADSM_HRD_RSTB_MSK) >> PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_ADSM_HRD_RSTB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_ADSM_HRD_RSTB_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_PLL_RSTB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_PLL_RSTB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_PLL_RSTB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_PLL_RSTB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_PLL_RSTB_set", A, value );

    /* (0x00000534 bits 2) field CSU0_PLL_RSTB of register PMC_PCBI_REG_CSU0_RESET_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_PLL_RSTB_MSK,
                                                      PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_PLL_RSTB_OFF,
                                                      value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_PLL_RSTB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_PLL_RSTB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_PLL_RSTB_get", A, 15);
    /* (0x00000534 bits 2) field CSU0_PLL_RSTB of register PMC_PCBI_REG_CSU0_RESET_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_PLL_RSTB_MSK) >> PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_PLL_RSTB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_PLL_RSTB_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU_ARSTB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                     s16_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU_ARSTB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                     s16_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_ARSTB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU_ARSTB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU_ARSTB_set", A, value );

    /* (0x00000534 bits 1) field CSU_ARSTB of register PMC_PCBI_REG_CSU0_RESET_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU_ARSTB_MSK,
                                                      PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU_ARSTB_OFF,
                                                      value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU_ARSTB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU_ARSTB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_ARSTB_get", A, 15);
    /* (0x00000534 bits 1) field CSU_ARSTB of register PMC_PCBI_REG_CSU0_RESET_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU_ARSTB_MSK) >> PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU_ARSTB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU_ARSTB_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_VCOC_RSTB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_VCOC_RSTB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_RSTB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_RSTB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_RSTB_set", A, value );

    /* (0x00000534 bits 0) field CSU0_VCOC_RSTB of register PMC_PCBI_REG_CSU0_RESET_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_VCOC_RSTB_MSK,
                                                      PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_VCOC_RSTB_OFF,
                                                      value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_RSTB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_RSTB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_RSTB_get", A, 15);
    /* (0x00000534 bits 0) field CSU0_VCOC_RSTB of register PMC_PCBI_REG_CSU0_RESET_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_VCOC_RSTB_MSK) >> PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_VCOC_RSTB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_RSTB_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU_ATMSB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                     s16_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU_ATMSB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                     s16_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_ATMSB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU_ATMSB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU_ATMSB_set", A, value );

    /* (0x00000538 bits 0) field CSU_ATMSB of register PMC_PCBI_REG_CSU_TEST_CONFIG_REG */
    s16_csu_pcbi_reg_CSU_TEST_CONFIG_REG_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    PCBI_REG_CSU_TEST_CONFIG_REG_BIT_CSU_ATMSB_MSK,
                                                    PCBI_REG_CSU_TEST_CONFIG_REG_BIT_CSU_ATMSB_OFF,
                                                    value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU_ATMSB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU_ATMSB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU_ATMSB_get", A, 15);
    /* (0x00000538 bits 0) field CSU_ATMSB of register PMC_PCBI_REG_CSU_TEST_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU_TEST_CONFIG_REG_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & PCBI_REG_CSU_TEST_CONFIG_REG_BIT_CSU_ATMSB_MSK) >> PCBI_REG_CSU_TEST_CONFIG_REG_BIT_CSU_ATMSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU_ATMSB_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_SEL_DIVCLK_16_OR_64_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_SEL_DIVCLK_16_OR_64_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_SEL_DIVCLK_16_OR_64_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_SEL_DIVCLK_16_OR_64_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_SEL_DIVCLK_16_OR_64_set", A, value );

    /* (0x0000053c bits 1) field SEL_DIVCLK_16_OR_64 of register PMC_PCBI_REG_CLK_DEGLITCHER_CONFIG_REG */
    s16_csu_pcbi_reg_CLK_DEGLITCHER_CONFIG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PCBI_REG_CLK_DEGLITCHER_CONFIG_REG_BIT_SEL_DIVCLK_16_OR_64_MSK,
                                                          PCBI_REG_CLK_DEGLITCHER_CONFIG_REG_BIT_SEL_DIVCLK_16_OR_64_OFF,
                                                          value);
}

static INLINE UINT32 s16_csu_pcbi_field_SEL_DIVCLK_16_OR_64_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_SEL_DIVCLK_16_OR_64_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_SEL_DIVCLK_16_OR_64_get", A, 15);
    /* (0x0000053c bits 1) field SEL_DIVCLK_16_OR_64 of register PMC_PCBI_REG_CLK_DEGLITCHER_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CLK_DEGLITCHER_CONFIG_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & PCBI_REG_CLK_DEGLITCHER_CONFIG_REG_BIT_SEL_DIVCLK_16_OR_64_MSK) >> PCBI_REG_CLK_DEGLITCHER_CONFIG_REG_BIT_SEL_DIVCLK_16_OR_64_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_SEL_DIVCLK_16_OR_64_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CLK_DEGLITCHER_EN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CLK_DEGLITCHER_EN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CLK_DEGLITCHER_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CLK_DEGLITCHER_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CLK_DEGLITCHER_EN_set", A, value );

    /* (0x0000053c bits 0) field CLK_DEGLITCHER_EN of register PMC_PCBI_REG_CLK_DEGLITCHER_CONFIG_REG */
    s16_csu_pcbi_reg_CLK_DEGLITCHER_CONFIG_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PCBI_REG_CLK_DEGLITCHER_CONFIG_REG_BIT_CLK_DEGLITCHER_EN_MSK,
                                                          PCBI_REG_CLK_DEGLITCHER_CONFIG_REG_BIT_CLK_DEGLITCHER_EN_OFF,
                                                          value);
}

static INLINE UINT32 s16_csu_pcbi_field_CLK_DEGLITCHER_EN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CLK_DEGLITCHER_EN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CLK_DEGLITCHER_EN_get", A, 15);
    /* (0x0000053c bits 0) field CLK_DEGLITCHER_EN of register PMC_PCBI_REG_CLK_DEGLITCHER_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CLK_DEGLITCHER_CONFIG_REG_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & PCBI_REG_CLK_DEGLITCHER_CONFIG_REG_BIT_CLK_DEGLITCHER_EN_MSK) >> PCBI_REG_CLK_DEGLITCHER_CONFIG_REG_BIT_CLK_DEGLITCHER_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CLK_DEGLITCHER_EN_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_GLBL_RSTB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                     s16_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_GLBL_RSTB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                     s16_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_GLBL_RSTB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_GLBL_RSTB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_GLBL_RSTB_set", A, value );

    /* (0x00000540 bits 0) field GLBL_RSTB of register PMC_PCBI_REG_GLOBAL_RESET_CONFIG_REG */
    s16_csu_pcbi_reg_GLOBAL_RESET_CONFIG_REG_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        PCBI_REG_GLOBAL_RESET_CONFIG_REG_BIT_GLBL_RSTB_MSK,
                                                        PCBI_REG_GLOBAL_RESET_CONFIG_REG_BIT_GLBL_RSTB_OFF,
                                                        value);
}

static INLINE UINT32 s16_csu_pcbi_field_GLBL_RSTB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_GLBL_RSTB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_GLBL_RSTB_get", A, 15);
    /* (0x00000540 bits 0) field GLBL_RSTB of register PMC_PCBI_REG_GLOBAL_RESET_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_GLOBAL_RESET_CONFIG_REG_read( b_ptr,
                                                               h_ptr,
                                                               A);
    value = (reg_value & PCBI_REG_GLOBAL_RESET_CONFIG_REG_BIT_GLBL_RSTB_MSK) >> PCBI_REG_GLOBAL_RESET_CONFIG_REG_BIT_GLBL_RSTB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_GLBL_RSTB_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_EXT_EN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_EXT_EN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_EXT_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_EXT_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_EXT_EN_set", A, value );

    /* (0x00000548 bits 26) field CSU0_FRAC_DIV_MOD_EXT_EN of register PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1 */
    s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG1_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1_BIT_CSU0_FRAC_DIV_MOD_EXT_EN_MSK,
                                                          PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1_BIT_CSU0_FRAC_DIV_MOD_EXT_EN_OFF,
                                                          value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_EXT_EN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_EXT_EN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_EXT_EN_get", A, 15);
    /* (0x00000548 bits 26) field CSU0_FRAC_DIV_MOD_EXT_EN of register PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG1_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1_BIT_CSU0_FRAC_DIV_MOD_EXT_EN_MSK) >> PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1_BIT_CSU0_FRAC_DIV_MOD_EXT_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_EXT_EN_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_EN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_EN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_EN_set", A, value );

    /* (0x00000548 bits 25) field CSU0_FRAC_DIV_MOD_OVR_EN of register PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1 */
    s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG1_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1_BIT_CSU0_FRAC_DIV_MOD_OVR_EN_MSK,
                                                          PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1_BIT_CSU0_FRAC_DIV_MOD_OVR_EN_OFF,
                                                          value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_EN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_EN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_EN_get", A, 15);
    /* (0x00000548 bits 25) field CSU0_FRAC_DIV_MOD_OVR_EN of register PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG1_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1_BIT_CSU0_FRAC_DIV_MOD_OVR_EN_MSK) >> PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1_BIT_CSU0_FRAC_DIV_MOD_OVR_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_EN_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_31_16_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_31_16_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_31_16_set", A, 15);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_31_16_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_31_16_set", A, value );

    /* (0x0000054c bits 15:0) bits 0:15 use field CSU0_FRAC_DIV_MOD_OVR_31_16 of register PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG2_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2_BIT_CSU0_FRAC_DIV_MOD_OVR_31_16_MSK,
                                                          PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2_BIT_CSU0_FRAC_DIV_MOD_OVR_31_16_OFF,
                                                          value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_31_16_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_31_16_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_31_16_get", A, 15);
    /* (0x0000054c bits 15:0) bits 0:15 use field CSU0_FRAC_DIV_MOD_OVR_31_16 of register PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG2_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2_BIT_CSU0_FRAC_DIV_MOD_OVR_31_16_MSK) >> PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2_BIT_CSU0_FRAC_DIV_MOD_OVR_31_16_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_31_16_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_31_16_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_31_16_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_31_16_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_31_16_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_31_16_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_31_16_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000054c bits 15:0) bits 0:15 use field CSU0_FRAC_DIV_MOD_OVR_31_16 of register PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2 */
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
        /* (0x0000054c bits 15:0) bits 0:15 use field CSU0_FRAC_DIV_MOD_OVR_31_16 of register PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2 */
        s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG2_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              subfield_mask << (PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2_BIT_CSU0_FRAC_DIV_MOD_OVR_31_16_OFF + subfield_offset),
                                                              PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2_BIT_CSU0_FRAC_DIV_MOD_OVR_31_16_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_31_16_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                               s16_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_31_16_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                               s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_31_16_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_31_16_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_31_16_get", stop_bit, 15 );
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
    /* (0x0000054c bits 15:0) bits 0:15 use field CSU0_FRAC_DIV_MOD_OVR_31_16 of register PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG2_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    field_value = (reg_value & PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2_BIT_CSU0_FRAC_DIV_MOD_OVR_31_16_MSK)
                  >> PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2_BIT_CSU0_FRAC_DIV_MOD_OVR_31_16_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2_BIT_CSU0_FRAC_DIV_MOD_OVR_31_16_MSK, PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2_BIT_CSU0_FRAC_DIV_MOD_OVR_31_16_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_31_16_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_40_32_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_40_32_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_40_32_set", A, 15);
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_40_32_set", value, 511);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_40_32_set", A, value );

    /* (0x00000550 bits 8:0) bits 0:8 use field CSU0_FRAC_DIV_MOD_OVR_40_32 of register PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3 */
    s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG3_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3_BIT_CSU0_FRAC_DIV_MOD_OVR_40_32_MSK,
                                                          PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3_BIT_CSU0_FRAC_DIV_MOD_OVR_40_32_OFF,
                                                          value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_40_32_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_40_32_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_40_32_get", A, 15);
    /* (0x00000550 bits 8:0) bits 0:8 use field CSU0_FRAC_DIV_MOD_OVR_40_32 of register PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG3_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3_BIT_CSU0_FRAC_DIV_MOD_OVR_40_32_MSK) >> PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3_BIT_CSU0_FRAC_DIV_MOD_OVR_40_32_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_40_32_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_40_32_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_40_32_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_40_32_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_40_32_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_40_32_set", stop_bit, 8 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_40_32_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000550 bits 8:0) bits 0:8 use field CSU0_FRAC_DIV_MOD_OVR_40_32 of register PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3 */
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
        /* (0x00000550 bits 8:0) bits 0:8 use field CSU0_FRAC_DIV_MOD_OVR_40_32 of register PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3 */
        s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG3_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              subfield_mask << (PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3_BIT_CSU0_FRAC_DIV_MOD_OVR_40_32_OFF + subfield_offset),
                                                              PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3_BIT_CSU0_FRAC_DIV_MOD_OVR_40_32_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_40_32_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                               s16_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_40_32_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                               s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_40_32_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_40_32_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_40_32_get", stop_bit, 8 );
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
    /* (0x00000550 bits 8:0) bits 0:8 use field CSU0_FRAC_DIV_MOD_OVR_40_32 of register PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG3_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    field_value = (reg_value & PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3_BIT_CSU0_FRAC_DIV_MOD_OVR_40_32_MSK)
                  >> PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3_BIT_CSU0_FRAC_DIV_MOD_OVR_40_32_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3_BIT_CSU0_FRAC_DIV_MOD_OVR_40_32_MSK, PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3_BIT_CSU0_FRAC_DIV_MOD_OVR_40_32_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_40_32_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_EADSM_CLK_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_EADSM_CLK_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_EADSM_CLK_CNT_MAX_set", A, 15);
    if (value > 262143)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_CLK_CNT_MAX_set", value, 262143);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_CLK_CNT_MAX_set", A, value );

    /* (0x00000554 bits 23:6) bits 0:17 use field CSU0_EADSM_CLK_CNT_MAX of register PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_field_set( b_ptr,
                                                               h_ptr,
                                                               A,
                                                               PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_CLK_CNT_MAX_MSK,
                                                               PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_CLK_CNT_MAX_OFF,
                                                               value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_EADSM_CLK_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_EADSM_CLK_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_EADSM_CLK_CNT_MAX_get", A, 15);
    /* (0x00000554 bits 23:6) bits 0:17 use field CSU0_EADSM_CLK_CNT_MAX of register PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_read( b_ptr,
                                                                      h_ptr,
                                                                      A);
    value = (reg_value & PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_CLK_CNT_MAX_MSK) >> PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_CLK_CNT_MAX_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_CLK_CNT_MAX_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_EADSM_CLK_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_EADSM_CLK_CNT_MAX_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_EADSM_CLK_CNT_MAX_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_EADSM_CLK_CNT_MAX_set", stop_bit, start_bit );
    if (stop_bit > 17) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_EADSM_CLK_CNT_MAX_set", stop_bit, 17 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_EADSM_CLK_CNT_MAX_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000554 bits 23:6) bits 0:17 use field CSU0_EADSM_CLK_CNT_MAX of register PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG */
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
        if (stop_bit < 17) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 17;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000554 bits 23:6) bits 0:17 use field CSU0_EADSM_CLK_CNT_MAX of register PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG */
        s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_field_set( b_ptr,
                                                                   h_ptr,
                                                                   A,
                                                                   subfield_mask << (PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_CLK_CNT_MAX_OFF + subfield_offset),
                                                                   PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_CLK_CNT_MAX_OFF + subfield_offset,
                                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_EADSM_CLK_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_EADSM_CLK_CNT_MAX_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_EADSM_CLK_CNT_MAX_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_EADSM_CLK_CNT_MAX_get", stop_bit, start_bit );
    if (stop_bit > 17) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_EADSM_CLK_CNT_MAX_get", stop_bit, 17 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 17) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 17;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000554 bits 23:6) bits 0:17 use field CSU0_EADSM_CLK_CNT_MAX of register PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_read( b_ptr,
                                                                      h_ptr,
                                                                      A);
    field_value = (reg_value & PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_CLK_CNT_MAX_MSK)
                  >> PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_CLK_CNT_MAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_CLK_CNT_MAX_MSK, PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_CLK_CNT_MAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_EADSM_CLK_CNT_MAX_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_EADSM_LOCK_CNT_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_EADSM_LOCK_CNT_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_EADSM_LOCK_CNT_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCK_CNT_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCK_CNT_set", A, value );

    /* (0x00000554 bits 5:0) bits 0:5 use field CSU0_EADSM_LOCK_CNT of register PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG */
    s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_field_set( b_ptr,
                                                               h_ptr,
                                                               A,
                                                               PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_LOCK_CNT_MSK,
                                                               PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_LOCK_CNT_OFF,
                                                               value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_EADSM_LOCK_CNT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_EADSM_LOCK_CNT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_EADSM_LOCK_CNT_get", A, 15);
    /* (0x00000554 bits 5:0) bits 0:5 use field CSU0_EADSM_LOCK_CNT of register PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_read( b_ptr,
                                                                      h_ptr,
                                                                      A);
    value = (reg_value & PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_LOCK_CNT_MSK) >> PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_LOCK_CNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCK_CNT_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_EADSM_LOCK_CNT_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_EADSM_LOCK_CNT_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_EADSM_LOCK_CNT_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_EADSM_LOCK_CNT_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_EADSM_LOCK_CNT_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_EADSM_LOCK_CNT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000554 bits 5:0) bits 0:5 use field CSU0_EADSM_LOCK_CNT of register PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG */
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
        /* (0x00000554 bits 5:0) bits 0:5 use field CSU0_EADSM_LOCK_CNT of register PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG */
        s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_field_set( b_ptr,
                                                                   h_ptr,
                                                                   A,
                                                                   subfield_mask << (PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_LOCK_CNT_OFF + subfield_offset),
                                                                   PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_LOCK_CNT_OFF + subfield_offset,
                                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_EADSM_LOCK_CNT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_EADSM_LOCK_CNT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_EADSM_LOCK_CNT_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_EADSM_LOCK_CNT_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_EADSM_LOCK_CNT_get", stop_bit, 5 );
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
    /* (0x00000554 bits 5:0) bits 0:5 use field CSU0_EADSM_LOCK_CNT of register PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_read( b_ptr,
                                                                      h_ptr,
                                                                      A);
    field_value = (reg_value & PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_LOCK_CNT_MSK)
                  >> PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_LOCK_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_LOCK_CNT_MSK, PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_LOCK_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_EADSM_LOCK_CNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_DITHER_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_DITHER_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_DITHER_SEL_set", A, 15);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_DITHER_SEL_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_DITHER_SEL_set", A, value );

    /* (0x00000558 bits 4:3) bits 0:1 use field CSU0_DITHER_SEL of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG1 */
    s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_DITHER_SEL_MSK,
                                                       PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_DITHER_SEL_OFF,
                                                       value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_DITHER_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_DITHER_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_DITHER_SEL_get", A, 15);
    /* (0x00000558 bits 4:3) bits 0:1 use field CSU0_DITHER_SEL of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_read( b_ptr,
                                                              h_ptr,
                                                              A);
    value = (reg_value & PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_DITHER_SEL_MSK) >> PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_DITHER_SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_DITHER_SEL_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_DITHER_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_DITHER_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_DITHER_SEL_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_DITHER_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_DITHER_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_DITHER_SEL_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000558 bits 4:3) bits 0:1 use field CSU0_DITHER_SEL of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG1 */
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
        /* (0x00000558 bits 4:3) bits 0:1 use field CSU0_DITHER_SEL of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG1 */
        s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_DITHER_SEL_OFF + subfield_offset),
                                                           PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_DITHER_SEL_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_DITHER_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_DITHER_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_DITHER_SEL_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_DITHER_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_DITHER_SEL_get", stop_bit, 1 );
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
    /* (0x00000558 bits 4:3) bits 0:1 use field CSU0_DITHER_SEL of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_DITHER_SEL_MSK)
                  >> PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_DITHER_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_DITHER_SEL_MSK, PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_DITHER_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_DITHER_SEL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_MULT2_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_MULT2_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_MULT2_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_MULT2_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_MULT2_set", A, value );

    /* (0x00000558 bits 2) field CSU0_MULT2 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG1 */
    s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_MULT2_MSK,
                                                       PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_MULT2_OFF,
                                                       value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_MULT2_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_MULT2_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_MULT2_get", A, 15);
    /* (0x00000558 bits 2) field CSU0_MULT2 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_read( b_ptr,
                                                              h_ptr,
                                                              A);
    value = (reg_value & PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_MULT2_MSK) >> PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_MULT2_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_MULT2_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_SEED_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                     s16_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_SEED_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                     s16_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_SEED_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_SEED_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_SEED_set", A, value );

    /* (0x00000558 bits 1) field CSU0_SEED of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG1 */
    s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_SEED_MSK,
                                                       PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_SEED_OFF,
                                                       value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_SEED_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_SEED_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_SEED_get", A, 15);
    /* (0x00000558 bits 1) field CSU0_SEED of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_read( b_ptr,
                                                              h_ptr,
                                                              A);
    value = (reg_value & PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_SEED_MSK) >> PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_SEED_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_SEED_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_CLKO_POLARITY_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_CLKO_POLARITY_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CLKO_POLARITY_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_CLKO_POLARITY_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CLKO_POLARITY_set", A, value );

    /* (0x00000558 bits 0) field CSU0_CLKO_POLARITY of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG1 */
    s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_CLKO_POLARITY_MSK,
                                                       PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_CLKO_POLARITY_OFF,
                                                       value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_CLKO_POLARITY_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_CLKO_POLARITY_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_CLKO_POLARITY_get", A, 15);
    /* (0x00000558 bits 0) field CSU0_CLKO_POLARITY of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_read( b_ptr,
                                                              h_ptr,
                                                              A);
    value = (reg_value & PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_CLKO_POLARITY_MSK) >> PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_CLKO_POLARITY_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_CLKO_POLARITY_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_INT_DIV_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_INT_DIV_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_INT_DIV_set", A, 15);
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_INT_DIV_set", value, 1023);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_INT_DIV_set", A, value );

    /* (0x0000055c bits 9:0) bits 0:9 use field CSU0_INT_DIV of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG2 */
    s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG2_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       PCBI_REG_CSU0_EADSM_CONFIG_REG2_BIT_CSU0_INT_DIV_MSK,
                                                       PCBI_REG_CSU0_EADSM_CONFIG_REG2_BIT_CSU0_INT_DIV_OFF,
                                                       value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_INT_DIV_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_INT_DIV_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_INT_DIV_get", A, 15);
    /* (0x0000055c bits 9:0) bits 0:9 use field CSU0_INT_DIV of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG2_read( b_ptr,
                                                              h_ptr,
                                                              A);
    value = (reg_value & PCBI_REG_CSU0_EADSM_CONFIG_REG2_BIT_CSU0_INT_DIV_MSK) >> PCBI_REG_CSU0_EADSM_CONFIG_REG2_BIT_CSU0_INT_DIV_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_INT_DIV_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_INT_DIV_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_INT_DIV_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_INT_DIV_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_INT_DIV_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_INT_DIV_set", stop_bit, 9 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_INT_DIV_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000055c bits 9:0) bits 0:9 use field CSU0_INT_DIV of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG2 */
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
        /* (0x0000055c bits 9:0) bits 0:9 use field CSU0_INT_DIV of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG2 */
        s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG2_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (PCBI_REG_CSU0_EADSM_CONFIG_REG2_BIT_CSU0_INT_DIV_OFF + subfield_offset),
                                                           PCBI_REG_CSU0_EADSM_CONFIG_REG2_BIT_CSU0_INT_DIV_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_INT_DIV_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_INT_DIV_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_INT_DIV_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_INT_DIV_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_INT_DIV_get", stop_bit, 9 );
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
    /* (0x0000055c bits 9:0) bits 0:9 use field CSU0_INT_DIV of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG2_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & PCBI_REG_CSU0_EADSM_CONFIG_REG2_BIT_CSU0_INT_DIV_MSK)
                  >> PCBI_REG_CSU0_EADSM_CONFIG_REG2_BIT_CSU0_INT_DIV_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_EADSM_CONFIG_REG2_BIT_CSU0_INT_DIV_MSK, PCBI_REG_CSU0_EADSM_CONFIG_REG2_BIT_CSU0_INT_DIV_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_INT_DIV_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_DENOMIN_31_0_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_DENOMIN_31_0_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_DENOMIN_31_0_set", A, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_DENOMIN_31_0_set", A, value );

    /* (0x00000560 bits 31:0) bits 0:31 use field CSU0_DENOMIN_31_0 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG3 */
    s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG3_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       PCBI_REG_CSU0_EADSM_CONFIG_REG3_BIT_CSU0_DENOMIN_31_0_MSK,
                                                       PCBI_REG_CSU0_EADSM_CONFIG_REG3_BIT_CSU0_DENOMIN_31_0_OFF,
                                                       value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_DENOMIN_31_0_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_DENOMIN_31_0_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_DENOMIN_31_0_get", A, 15);
    /* (0x00000560 bits 31:0) bits 0:31 use field CSU0_DENOMIN_31_0 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG3_read( b_ptr,
                                                              h_ptr,
                                                              A);
    value = (reg_value & PCBI_REG_CSU0_EADSM_CONFIG_REG3_BIT_CSU0_DENOMIN_31_0_MSK) >> PCBI_REG_CSU0_EADSM_CONFIG_REG3_BIT_CSU0_DENOMIN_31_0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_DENOMIN_31_0_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_DENOMIN_31_0_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_DENOMIN_31_0_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_DENOMIN_31_0_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_DENOMIN_31_0_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_DENOMIN_31_0_set", stop_bit, 31 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_DENOMIN_31_0_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000560 bits 31:0) bits 0:31 use field CSU0_DENOMIN_31_0 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG3 */
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
        /* (0x00000560 bits 31:0) bits 0:31 use field CSU0_DENOMIN_31_0 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG3 */
        s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG3_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (PCBI_REG_CSU0_EADSM_CONFIG_REG3_BIT_CSU0_DENOMIN_31_0_OFF + subfield_offset),
                                                           PCBI_REG_CSU0_EADSM_CONFIG_REG3_BIT_CSU0_DENOMIN_31_0_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_DENOMIN_31_0_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_DENOMIN_31_0_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_DENOMIN_31_0_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_DENOMIN_31_0_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_DENOMIN_31_0_get", stop_bit, 31 );
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
    /* (0x00000560 bits 31:0) bits 0:31 use field CSU0_DENOMIN_31_0 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG3_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & PCBI_REG_CSU0_EADSM_CONFIG_REG3_BIT_CSU0_DENOMIN_31_0_MSK)
                  >> PCBI_REG_CSU0_EADSM_CONFIG_REG3_BIT_CSU0_DENOMIN_31_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_EADSM_CONFIG_REG3_BIT_CSU0_DENOMIN_31_0_MSK, PCBI_REG_CSU0_EADSM_CONFIG_REG3_BIT_CSU0_DENOMIN_31_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_DENOMIN_31_0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_DENOMIN_39_32_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_DENOMIN_39_32_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_DENOMIN_39_32_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_DENOMIN_39_32_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_DENOMIN_39_32_set", A, value );

    /* (0x00000564 bits 7:0) bits 0:7 use field CSU0_DENOMIN_39_32 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG4 */
    s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG4_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       PCBI_REG_CSU0_EADSM_CONFIG_REG4_BIT_CSU0_DENOMIN_39_32_MSK,
                                                       PCBI_REG_CSU0_EADSM_CONFIG_REG4_BIT_CSU0_DENOMIN_39_32_OFF,
                                                       value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_DENOMIN_39_32_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_DENOMIN_39_32_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_DENOMIN_39_32_get", A, 15);
    /* (0x00000564 bits 7:0) bits 0:7 use field CSU0_DENOMIN_39_32 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG4 */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG4_read( b_ptr,
                                                              h_ptr,
                                                              A);
    value = (reg_value & PCBI_REG_CSU0_EADSM_CONFIG_REG4_BIT_CSU0_DENOMIN_39_32_MSK) >> PCBI_REG_CSU0_EADSM_CONFIG_REG4_BIT_CSU0_DENOMIN_39_32_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_DENOMIN_39_32_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_DENOMIN_39_32_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_DENOMIN_39_32_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_DENOMIN_39_32_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_DENOMIN_39_32_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_DENOMIN_39_32_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_DENOMIN_39_32_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000564 bits 7:0) bits 0:7 use field CSU0_DENOMIN_39_32 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG4 */
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
        /* (0x00000564 bits 7:0) bits 0:7 use field CSU0_DENOMIN_39_32 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG4 */
        s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG4_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (PCBI_REG_CSU0_EADSM_CONFIG_REG4_BIT_CSU0_DENOMIN_39_32_OFF + subfield_offset),
                                                           PCBI_REG_CSU0_EADSM_CONFIG_REG4_BIT_CSU0_DENOMIN_39_32_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_DENOMIN_39_32_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_DENOMIN_39_32_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_DENOMIN_39_32_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_DENOMIN_39_32_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_DENOMIN_39_32_get", stop_bit, 7 );
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
    /* (0x00000564 bits 7:0) bits 0:7 use field CSU0_DENOMIN_39_32 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG4 */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG4_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & PCBI_REG_CSU0_EADSM_CONFIG_REG4_BIT_CSU0_DENOMIN_39_32_MSK)
                  >> PCBI_REG_CSU0_EADSM_CONFIG_REG4_BIT_CSU0_DENOMIN_39_32_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_EADSM_CONFIG_REG4_BIT_CSU0_DENOMIN_39_32_MSK, PCBI_REG_CSU0_EADSM_CONFIG_REG4_BIT_CSU0_DENOMIN_39_32_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_DENOMIN_39_32_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_FRAC_DIV_31_0_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_FRAC_DIV_31_0_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_FRAC_DIV_31_0_set", A, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_FRAC_DIV_31_0_set", A, value );

    /* (0x00000568 bits 31:0) bits 0:31 use field CSU0_FRAC_DIV_31_0 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG5 */
    s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG5_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       PCBI_REG_CSU0_EADSM_CONFIG_REG5_BIT_CSU0_FRAC_DIV_31_0_MSK,
                                                       PCBI_REG_CSU0_EADSM_CONFIG_REG5_BIT_CSU0_FRAC_DIV_31_0_OFF,
                                                       value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_FRAC_DIV_31_0_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_FRAC_DIV_31_0_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_FRAC_DIV_31_0_get", A, 15);
    /* (0x00000568 bits 31:0) bits 0:31 use field CSU0_FRAC_DIV_31_0 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG5 */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG5_read( b_ptr,
                                                              h_ptr,
                                                              A);
    value = (reg_value & PCBI_REG_CSU0_EADSM_CONFIG_REG5_BIT_CSU0_FRAC_DIV_31_0_MSK) >> PCBI_REG_CSU0_EADSM_CONFIG_REG5_BIT_CSU0_FRAC_DIV_31_0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_FRAC_DIV_31_0_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_FRAC_DIV_31_0_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_FRAC_DIV_31_0_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_31_0_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_31_0_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_31_0_set", stop_bit, 31 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_31_0_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000568 bits 31:0) bits 0:31 use field CSU0_FRAC_DIV_31_0 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG5 */
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
        /* (0x00000568 bits 31:0) bits 0:31 use field CSU0_FRAC_DIV_31_0 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG5 */
        s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG5_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (PCBI_REG_CSU0_EADSM_CONFIG_REG5_BIT_CSU0_FRAC_DIV_31_0_OFF + subfield_offset),
                                                           PCBI_REG_CSU0_EADSM_CONFIG_REG5_BIT_CSU0_FRAC_DIV_31_0_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_FRAC_DIV_31_0_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_FRAC_DIV_31_0_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_31_0_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_31_0_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_31_0_get", stop_bit, 31 );
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
    /* (0x00000568 bits 31:0) bits 0:31 use field CSU0_FRAC_DIV_31_0 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG5 */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG5_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & PCBI_REG_CSU0_EADSM_CONFIG_REG5_BIT_CSU0_FRAC_DIV_31_0_MSK)
                  >> PCBI_REG_CSU0_EADSM_CONFIG_REG5_BIT_CSU0_FRAC_DIV_31_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_EADSM_CONFIG_REG5_BIT_CSU0_FRAC_DIV_31_0_MSK, PCBI_REG_CSU0_EADSM_CONFIG_REG5_BIT_CSU0_FRAC_DIV_31_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_31_0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_FRAC_DIV_39_32_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_FRAC_DIV_39_32_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_FRAC_DIV_39_32_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_FRAC_DIV_39_32_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_FRAC_DIV_39_32_set", A, value );

    /* (0x0000056c bits 7:0) bits 0:7 use field CSU0_FRAC_DIV_39_32 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG6 */
    s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG6_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       PCBI_REG_CSU0_EADSM_CONFIG_REG6_BIT_CSU0_FRAC_DIV_39_32_MSK,
                                                       PCBI_REG_CSU0_EADSM_CONFIG_REG6_BIT_CSU0_FRAC_DIV_39_32_OFF,
                                                       value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_FRAC_DIV_39_32_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_FRAC_DIV_39_32_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_FRAC_DIV_39_32_get", A, 15);
    /* (0x0000056c bits 7:0) bits 0:7 use field CSU0_FRAC_DIV_39_32 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG6 */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG6_read( b_ptr,
                                                              h_ptr,
                                                              A);
    value = (reg_value & PCBI_REG_CSU0_EADSM_CONFIG_REG6_BIT_CSU0_FRAC_DIV_39_32_MSK) >> PCBI_REG_CSU0_EADSM_CONFIG_REG6_BIT_CSU0_FRAC_DIV_39_32_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_FRAC_DIV_39_32_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_CSU0_FRAC_DIV_39_32_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_CSU0_FRAC_DIV_39_32_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_39_32_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_39_32_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_39_32_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_39_32_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000056c bits 7:0) bits 0:7 use field CSU0_FRAC_DIV_39_32 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG6 */
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
        /* (0x0000056c bits 7:0) bits 0:7 use field CSU0_FRAC_DIV_39_32 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG6 */
        s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG6_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (PCBI_REG_CSU0_EADSM_CONFIG_REG6_BIT_CSU0_FRAC_DIV_39_32_OFF + subfield_offset),
                                                           PCBI_REG_CSU0_EADSM_CONFIG_REG6_BIT_CSU0_FRAC_DIV_39_32_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_FRAC_DIV_39_32_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_FRAC_DIV_39_32_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_39_32_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_39_32_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_39_32_get", stop_bit, 7 );
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
    /* (0x0000056c bits 7:0) bits 0:7 use field CSU0_FRAC_DIV_39_32 of register PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG6 */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG6_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & PCBI_REG_CSU0_EADSM_CONFIG_REG6_BIT_CSU0_FRAC_DIV_39_32_MSK)
                  >> PCBI_REG_CSU0_EADSM_CONFIG_REG6_BIT_CSU0_FRAC_DIV_39_32_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_EADSM_CONFIG_REG6_BIT_CSU0_FRAC_DIV_39_32_MSK, PCBI_REG_CSU0_EADSM_CONFIG_REG6_BIT_CSU0_FRAC_DIV_39_32_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_FRAC_DIV_39_32_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_TRCS_MODE_23_22_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_TRCS_MODE_23_22_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_TRCS_MODE_23_22_set", A, 15);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_TRCS_MODE_23_22_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_TRCS_MODE_23_22_set", A, value );

    /* (0x0000057c bits 23:22) field TRCS_MODE_23_22 of register PMC_PCBI_REG_CSU0_GLOBAL_CFG_REG1 */
    s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_23_22_MSK,
                                                     PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_23_22_OFF,
                                                     value);
}

static INLINE UINT32 s16_csu_pcbi_field_TRCS_MODE_23_22_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_TRCS_MODE_23_22_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_TRCS_MODE_23_22_get", A, 15);
    /* (0x0000057c bits 23:22) field TRCS_MODE_23_22 of register PMC_PCBI_REG_CSU0_GLOBAL_CFG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_23_22_MSK) >> PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_23_22_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_TRCS_MODE_23_22_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_TRCS_MODE_23_22_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_TRCS_MODE_23_22_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_TRCS_MODE_23_22_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_TRCS_MODE_23_22_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_TRCS_MODE_23_22_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_TRCS_MODE_23_22_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000057c bits 23:22) field TRCS_MODE_23_22 of register PMC_PCBI_REG_CSU0_GLOBAL_CFG_REG1 */
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
        /* (0x0000057c bits 23:22) field TRCS_MODE_23_22 of register PMC_PCBI_REG_CSU0_GLOBAL_CFG_REG1 */
        s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         subfield_mask << (PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_23_22_OFF + subfield_offset),
                                                         PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_23_22_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_TRCS_MODE_23_22_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_TRCS_MODE_23_22_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_TRCS_MODE_23_22_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_TRCS_MODE_23_22_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_TRCS_MODE_23_22_get", stop_bit, 1 );
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
    /* (0x0000057c bits 23:22) field TRCS_MODE_23_22 of register PMC_PCBI_REG_CSU0_GLOBAL_CFG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_read( b_ptr,
                                                            h_ptr,
                                                            A);
    field_value = (reg_value & PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_23_22_MSK)
                  >> PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_23_22_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_23_22_MSK, PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_23_22_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_TRCS_MODE_23_22_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_TRCS_MODE_14_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_TRCS_MODE_14_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_TRCS_MODE_14_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_TRCS_MODE_14_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_TRCS_MODE_14_set", A, value );

    /* (0x0000057c bits 14) field TRCS_MODE_14 of register PMC_PCBI_REG_CSU0_GLOBAL_CFG_REG1 */
    s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_14_MSK,
                                                     PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_14_OFF,
                                                     value);
}

static INLINE UINT32 s16_csu_pcbi_field_TRCS_MODE_14_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_TRCS_MODE_14_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_TRCS_MODE_14_get", A, 15);
    /* (0x0000057c bits 14) field TRCS_MODE_14 of register PMC_PCBI_REG_CSU0_GLOBAL_CFG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_14_MSK) >> PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_14_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_TRCS_MODE_14_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_TRCS_MODE_10_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_TRCS_MODE_10_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_TRCS_MODE_10_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_TRCS_MODE_10_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_TRCS_MODE_10_set", A, value );

    /* (0x0000057c bits 10) field TRCS_MODE_10 of register PMC_PCBI_REG_CSU0_GLOBAL_CFG_REG1 */
    s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_10_MSK,
                                                     PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_10_OFF,
                                                     value);
}

static INLINE UINT32 s16_csu_pcbi_field_TRCS_MODE_10_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_TRCS_MODE_10_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_TRCS_MODE_10_get", A, 15);
    /* (0x0000057c bits 10) field TRCS_MODE_10 of register PMC_PCBI_REG_CSU0_GLOBAL_CFG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_10_MSK) >> PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_10_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_TRCS_MODE_10_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_ACB_RSTB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                    s16_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_ACB_RSTB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                    s16_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_ACB_RSTB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_ACB_RSTB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_ACB_RSTB_set", A, value );

    /* (0x00000580 bits 31) field ACB_RSTB of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_ACB_RSTB_MSK,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_ACB_RSTB_OFF,
                                                   value);
}

static INLINE UINT32 s16_csu_pcbi_field_ACB_RSTB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_ACB_RSTB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_ACB_RSTB_get", A, 15);
    /* (0x00000580 bits 31) field ACB_RSTB of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & PCBI_REG_ACB_FT_CONFIG_REG1_BIT_ACB_RSTB_MSK) >> PCBI_REG_ACB_FT_CONFIG_REG1_BIT_ACB_RSTB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_ACB_RSTB_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_PDM_PHS_DET_DEL_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_PDM_PHS_DET_DEL_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_PHS_DET_DEL_SEL_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_PDM_PHS_DET_DEL_SEL_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_PDM_PHS_DET_DEL_SEL_set", A, value );

    /* (0x00000580 bits 27) field PDM_PHS_DET_DEL_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_PHS_DET_DEL_SEL_MSK,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_PHS_DET_DEL_SEL_OFF,
                                                   value);
}

static INLINE UINT32 s16_csu_pcbi_field_PDM_PHS_DET_DEL_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PDM_PHS_DET_DEL_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_PHS_DET_DEL_SEL_get", A, 15);
    /* (0x00000580 bits 27) field PDM_PHS_DET_DEL_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_PHS_DET_DEL_SEL_MSK) >> PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_PHS_DET_DEL_SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PDM_PHS_DET_DEL_SEL_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_PDM_ACCUM_BYPASS_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_PDM_ACCUM_BYPASS_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_ACCUM_BYPASS_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_PDM_ACCUM_BYPASS_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_PDM_ACCUM_BYPASS_set", A, value );

    /* (0x00000580 bits 26) field PDM_ACCUM_BYPASS of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_ACCUM_BYPASS_MSK,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_ACCUM_BYPASS_OFF,
                                                   value);
}

static INLINE UINT32 s16_csu_pcbi_field_PDM_ACCUM_BYPASS_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PDM_ACCUM_BYPASS_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_ACCUM_BYPASS_get", A, 15);
    /* (0x00000580 bits 26) field PDM_ACCUM_BYPASS of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_ACCUM_BYPASS_MSK) >> PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_ACCUM_BYPASS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PDM_ACCUM_BYPASS_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_PDM_TCLK2_DLY_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_PDM_TCLK2_DLY_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_TCLK2_DLY_SEL_set", A, 15);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_PDM_TCLK2_DLY_SEL_set", value, 31);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_PDM_TCLK2_DLY_SEL_set", A, value );

    /* (0x00000580 bits 25:21) field PDM_TCLK2_DLY_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK2_DLY_SEL_MSK,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK2_DLY_SEL_OFF,
                                                   value);
}

static INLINE UINT32 s16_csu_pcbi_field_PDM_TCLK2_DLY_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PDM_TCLK2_DLY_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_TCLK2_DLY_SEL_get", A, 15);
    /* (0x00000580 bits 25:21) field PDM_TCLK2_DLY_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK2_DLY_SEL_MSK) >> PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK2_DLY_SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PDM_TCLK2_DLY_SEL_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_PDM_TCLK2_DLY_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_PDM_TCLK2_DLY_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_TCLK2_DLY_SEL_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_TCLK2_DLY_SEL_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_TCLK2_DLY_SEL_set", stop_bit, 4 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_TCLK2_DLY_SEL_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000580 bits 25:21) field PDM_TCLK2_DLY_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
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
        /* (0x00000580 bits 25:21) field PDM_TCLK2_DLY_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
        s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK2_DLY_SEL_OFF + subfield_offset),
                                                       PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK2_DLY_SEL_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_PDM_TCLK2_DLY_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_PDM_TCLK2_DLY_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_TCLK2_DLY_SEL_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_TCLK2_DLY_SEL_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_TCLK2_DLY_SEL_get", stop_bit, 4 );
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
    /* (0x00000580 bits 25:21) field PDM_TCLK2_DLY_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK2_DLY_SEL_MSK)
                  >> PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK2_DLY_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK2_DLY_SEL_MSK, PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK2_DLY_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_TCLK2_DLY_SEL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_PDM_TCLK1_DLY_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_PDM_TCLK1_DLY_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_TCLK1_DLY_SEL_set", A, 15);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_PDM_TCLK1_DLY_SEL_set", value, 31);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_PDM_TCLK1_DLY_SEL_set", A, value );

    /* (0x00000580 bits 20:16) field PDM_TCLK1_DLY_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK1_DLY_SEL_MSK,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK1_DLY_SEL_OFF,
                                                   value);
}

static INLINE UINT32 s16_csu_pcbi_field_PDM_TCLK1_DLY_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PDM_TCLK1_DLY_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_TCLK1_DLY_SEL_get", A, 15);
    /* (0x00000580 bits 20:16) field PDM_TCLK1_DLY_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK1_DLY_SEL_MSK) >> PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK1_DLY_SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PDM_TCLK1_DLY_SEL_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_PDM_TCLK1_DLY_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_PDM_TCLK1_DLY_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_TCLK1_DLY_SEL_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_TCLK1_DLY_SEL_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_TCLK1_DLY_SEL_set", stop_bit, 4 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_TCLK1_DLY_SEL_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000580 bits 20:16) field PDM_TCLK1_DLY_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
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
        /* (0x00000580 bits 20:16) field PDM_TCLK1_DLY_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
        s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK1_DLY_SEL_OFF + subfield_offset),
                                                       PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK1_DLY_SEL_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_PDM_TCLK1_DLY_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_PDM_TCLK1_DLY_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_TCLK1_DLY_SEL_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_TCLK1_DLY_SEL_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_TCLK1_DLY_SEL_get", stop_bit, 4 );
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
    /* (0x00000580 bits 20:16) field PDM_TCLK1_DLY_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK1_DLY_SEL_MSK)
                  >> PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK1_DLY_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK1_DLY_SEL_MSK, PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK1_DLY_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_TCLK1_DLY_SEL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_PDM_EN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                  s16_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_PDM_EN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                  s16_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_PDM_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_PDM_EN_set", A, value );

    /* (0x00000580 bits 15) field PDM_EN of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_EN_MSK,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_EN_OFF,
                                                   value);
}

static INLINE UINT32 s16_csu_pcbi_field_PDM_EN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                    s16_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PDM_EN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                    s16_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_EN_get", A, 15);
    /* (0x00000580 bits 15) field PDM_EN of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_EN_MSK) >> PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PDM_EN_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_PDM_TCLK_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_PDM_TCLK_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_TCLK_SEL_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_PDM_TCLK_SEL_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_PDM_TCLK_SEL_set", A, value );

    /* (0x00000580 bits 13:10) field PDM_TCLK_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK_SEL_MSK,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK_SEL_OFF,
                                                   value);
}

static INLINE UINT32 s16_csu_pcbi_field_PDM_TCLK_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PDM_TCLK_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_TCLK_SEL_get", A, 15);
    /* (0x00000580 bits 13:10) field PDM_TCLK_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK_SEL_MSK) >> PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK_SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PDM_TCLK_SEL_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_PDM_TCLK_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_PDM_TCLK_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_TCLK_SEL_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_TCLK_SEL_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_TCLK_SEL_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_TCLK_SEL_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000580 bits 13:10) field PDM_TCLK_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
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
        /* (0x00000580 bits 13:10) field PDM_TCLK_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
        s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK_SEL_OFF + subfield_offset),
                                                       PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK_SEL_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_PDM_TCLK_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_PDM_TCLK_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_TCLK_SEL_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_TCLK_SEL_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_TCLK_SEL_get", stop_bit, 3 );
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
    /* (0x00000580 bits 13:10) field PDM_TCLK_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK_SEL_MSK)
                  >> PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK_SEL_MSK, PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_TCLK_SEL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_FRAC_DIV_MOD_EXT_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_FRAC_DIV_MOD_EXT_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_FRAC_DIV_MOD_EXT_SEL_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_FRAC_DIV_MOD_EXT_SEL_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_FRAC_DIV_MOD_EXT_SEL_set", A, value );

    /* (0x00000580 bits 9:6) field FRAC_DIV_MOD_EXT_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_FRAC_DIV_MOD_EXT_SEL_MSK,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_FRAC_DIV_MOD_EXT_SEL_OFF,
                                                   value);
}

static INLINE UINT32 s16_csu_pcbi_field_FRAC_DIV_MOD_EXT_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_FRAC_DIV_MOD_EXT_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_FRAC_DIV_MOD_EXT_SEL_get", A, 15);
    /* (0x00000580 bits 9:6) field FRAC_DIV_MOD_EXT_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & PCBI_REG_ACB_FT_CONFIG_REG1_BIT_FRAC_DIV_MOD_EXT_SEL_MSK) >> PCBI_REG_ACB_FT_CONFIG_REG1_BIT_FRAC_DIV_MOD_EXT_SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_FRAC_DIV_MOD_EXT_SEL_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_FRAC_DIV_MOD_EXT_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_FRAC_DIV_MOD_EXT_SEL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_FRAC_DIV_MOD_EXT_SEL_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_FRAC_DIV_MOD_EXT_SEL_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_FRAC_DIV_MOD_EXT_SEL_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_FRAC_DIV_MOD_EXT_SEL_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000580 bits 9:6) field FRAC_DIV_MOD_EXT_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
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
        /* (0x00000580 bits 9:6) field FRAC_DIV_MOD_EXT_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
        s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (PCBI_REG_ACB_FT_CONFIG_REG1_BIT_FRAC_DIV_MOD_EXT_SEL_OFF + subfield_offset),
                                                       PCBI_REG_ACB_FT_CONFIG_REG1_BIT_FRAC_DIV_MOD_EXT_SEL_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_FRAC_DIV_MOD_EXT_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_FRAC_DIV_MOD_EXT_SEL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_FRAC_DIV_MOD_EXT_SEL_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_FRAC_DIV_MOD_EXT_SEL_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_FRAC_DIV_MOD_EXT_SEL_get", stop_bit, 3 );
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
    /* (0x00000580 bits 9:6) field FRAC_DIV_MOD_EXT_SEL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & PCBI_REG_ACB_FT_CONFIG_REG1_BIT_FRAC_DIV_MOD_EXT_SEL_MSK)
                  >> PCBI_REG_ACB_FT_CONFIG_REG1_BIT_FRAC_DIV_MOD_EXT_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_ACB_FT_CONFIG_REG1_BIT_FRAC_DIV_MOD_EXT_SEL_MSK, PCBI_REG_ACB_FT_CONFIG_REG1_BIT_FRAC_DIV_MOD_EXT_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_FRAC_DIV_MOD_EXT_SEL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_ACB_HOLDOVER_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_ACB_HOLDOVER_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_ACB_HOLDOVER_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_ACB_HOLDOVER_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_ACB_HOLDOVER_set", A, value );

    /* (0x00000580 bits 1) field ACB_HOLDOVER of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_ACB_HOLDOVER_MSK,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_ACB_HOLDOVER_OFF,
                                                   value);
}

static INLINE UINT32 s16_csu_pcbi_field_ACB_HOLDOVER_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_ACB_HOLDOVER_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_ACB_HOLDOVER_get", A, 15);
    /* (0x00000580 bits 1) field ACB_HOLDOVER of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & PCBI_REG_ACB_FT_CONFIG_REG1_BIT_ACB_HOLDOVER_MSK) >> PCBI_REG_ACB_FT_CONFIG_REG1_BIT_ACB_HOLDOVER_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_ACB_HOLDOVER_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_ACB_FORCE_NOMINAL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_ACB_FORCE_NOMINAL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_ACB_FORCE_NOMINAL_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_ACB_FORCE_NOMINAL_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_ACB_FORCE_NOMINAL_set", A, value );

    /* (0x00000580 bits 0) field ACB_FORCE_NOMINAL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_ACB_FORCE_NOMINAL_MSK,
                                                   PCBI_REG_ACB_FT_CONFIG_REG1_BIT_ACB_FORCE_NOMINAL_OFF,
                                                   value);
}

static INLINE UINT32 s16_csu_pcbi_field_ACB_FORCE_NOMINAL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_ACB_FORCE_NOMINAL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_ACB_FORCE_NOMINAL_get", A, 15);
    /* (0x00000580 bits 0) field ACB_FORCE_NOMINAL of register PMC_PCBI_REG_ACB_FT_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & PCBI_REG_ACB_FT_CONFIG_REG1_BIT_ACB_FORCE_NOMINAL_MSK) >> PCBI_REG_ACB_FT_CONFIG_REG1_BIT_ACB_FORCE_NOMINAL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_ACB_FORCE_NOMINAL_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_PMM_DSM_DENOM_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_PMM_DSM_DENOM_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PMM_DSM_DENOM_set", A, 15);
    if (value > 16383)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_PMM_DSM_DENOM_set", value, 16383);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_PMM_DSM_DENOM_set", A, value );

    /* (0x00000584 bits 29:16) field PMM_DSM_DENOM of register PMC_PCBI_REG_PMM_CONFIG_REG1 */
    s16_csu_pcbi_reg_PMM_CONFIG_REG1_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_DSM_DENOM_MSK,
                                                PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_DSM_DENOM_OFF,
                                                value);
}

static INLINE UINT32 s16_csu_pcbi_field_PMM_DSM_DENOM_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PMM_DSM_DENOM_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PMM_DSM_DENOM_get", A, 15);
    /* (0x00000584 bits 29:16) field PMM_DSM_DENOM of register PMC_PCBI_REG_PMM_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG1_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_DSM_DENOM_MSK) >> PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_DSM_DENOM_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PMM_DSM_DENOM_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_PMM_DSM_DENOM_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_PMM_DSM_DENOM_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PMM_DSM_DENOM_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PMM_DSM_DENOM_set", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PMM_DSM_DENOM_set", stop_bit, 13 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PMM_DSM_DENOM_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000584 bits 29:16) field PMM_DSM_DENOM of register PMC_PCBI_REG_PMM_CONFIG_REG1 */
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
        if (stop_bit < 13) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 13;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000584 bits 29:16) field PMM_DSM_DENOM of register PMC_PCBI_REG_PMM_CONFIG_REG1 */
        s16_csu_pcbi_reg_PMM_CONFIG_REG1_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_DSM_DENOM_OFF + subfield_offset),
                                                    PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_DSM_DENOM_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_PMM_DSM_DENOM_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_PMM_DSM_DENOM_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PMM_DSM_DENOM_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PMM_DSM_DENOM_get", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PMM_DSM_DENOM_get", stop_bit, 13 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 13) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 13;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000584 bits 29:16) field PMM_DSM_DENOM of register PMC_PCBI_REG_PMM_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG1_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_DSM_DENOM_MSK)
                  >> PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_DSM_DENOM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_DSM_DENOM_MSK, PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_DSM_DENOM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PMM_DSM_DENOM_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_PMM_PKT_PERIOD_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_PMM_PKT_PERIOD_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PMM_PKT_PERIOD_set", A, 15);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_PMM_PKT_PERIOD_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_PMM_PKT_PERIOD_set", A, value );

    /* (0x00000584 bits 15:0) field PMM_PKT_PERIOD of register PMC_PCBI_REG_PMM_CONFIG_REG1 */
    s16_csu_pcbi_reg_PMM_CONFIG_REG1_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_PKT_PERIOD_MSK,
                                                PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_PKT_PERIOD_OFF,
                                                value);
}

static INLINE UINT32 s16_csu_pcbi_field_PMM_PKT_PERIOD_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PMM_PKT_PERIOD_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PMM_PKT_PERIOD_get", A, 15);
    /* (0x00000584 bits 15:0) field PMM_PKT_PERIOD of register PMC_PCBI_REG_PMM_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG1_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_PKT_PERIOD_MSK) >> PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_PKT_PERIOD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PMM_PKT_PERIOD_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_PMM_PKT_PERIOD_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_PMM_PKT_PERIOD_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PMM_PKT_PERIOD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PMM_PKT_PERIOD_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PMM_PKT_PERIOD_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PMM_PKT_PERIOD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000584 bits 15:0) field PMM_PKT_PERIOD of register PMC_PCBI_REG_PMM_CONFIG_REG1 */
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
        /* (0x00000584 bits 15:0) field PMM_PKT_PERIOD of register PMC_PCBI_REG_PMM_CONFIG_REG1 */
        s16_csu_pcbi_reg_PMM_CONFIG_REG1_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_PKT_PERIOD_OFF + subfield_offset),
                                                    PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_PKT_PERIOD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_PMM_PKT_PERIOD_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_PMM_PKT_PERIOD_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PMM_PKT_PERIOD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PMM_PKT_PERIOD_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PMM_PKT_PERIOD_get", stop_bit, 15 );
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
    /* (0x00000584 bits 15:0) field PMM_PKT_PERIOD of register PMC_PCBI_REG_PMM_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG1_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_PKT_PERIOD_MSK)
                  >> PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_PKT_PERIOD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_PKT_PERIOD_MSK, PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_PKT_PERIOD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PMM_PKT_PERIOD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_ACB_DESKEW_CLRB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_ACB_DESKEW_CLRB_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_ACB_DESKEW_CLRB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_CLRB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_CLRB_set", A, value );

    /* (0x00000588 bits 19) field ACB_DESKEW_CLRB of register PMC_PCBI_REG_PMM_CONFIG_REG2 */
    s16_csu_pcbi_reg_PMM_CONFIG_REG2_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                PCBI_REG_PMM_CONFIG_REG2_BIT_ACB_DESKEW_CLRB_MSK,
                                                PCBI_REG_PMM_CONFIG_REG2_BIT_ACB_DESKEW_CLRB_OFF,
                                                value);
}

static INLINE UINT32 s16_csu_pcbi_field_ACB_DESKEW_CLRB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_ACB_DESKEW_CLRB_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_ACB_DESKEW_CLRB_get", A, 15);
    /* (0x00000588 bits 19) field ACB_DESKEW_CLRB of register PMC_PCBI_REG_PMM_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & PCBI_REG_PMM_CONFIG_REG2_BIT_ACB_DESKEW_CLRB_MSK) >> PCBI_REG_PMM_CONFIG_REG2_BIT_ACB_DESKEW_CLRB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_CLRB_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_PDM_UPDATE_RATE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_PDM_UPDATE_RATE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_UPDATE_RATE_set", A, 15);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_PDM_UPDATE_RATE_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_PDM_UPDATE_RATE_set", A, value );

    /* (0x00000588 bits 18:12) field PDM_UPDATE_RATE of register PMC_PCBI_REG_PMM_CONFIG_REG2 */
    s16_csu_pcbi_reg_PMM_CONFIG_REG2_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                PCBI_REG_PMM_CONFIG_REG2_BIT_PDM_UPDATE_RATE_MSK,
                                                PCBI_REG_PMM_CONFIG_REG2_BIT_PDM_UPDATE_RATE_OFF,
                                                value);
}

static INLINE UINT32 s16_csu_pcbi_field_PDM_UPDATE_RATE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PDM_UPDATE_RATE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_UPDATE_RATE_get", A, 15);
    /* (0x00000588 bits 18:12) field PDM_UPDATE_RATE of register PMC_PCBI_REG_PMM_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & PCBI_REG_PMM_CONFIG_REG2_BIT_PDM_UPDATE_RATE_MSK) >> PCBI_REG_PMM_CONFIG_REG2_BIT_PDM_UPDATE_RATE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PDM_UPDATE_RATE_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_PDM_UPDATE_RATE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_PDM_UPDATE_RATE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_UPDATE_RATE_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_UPDATE_RATE_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_UPDATE_RATE_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_UPDATE_RATE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000588 bits 18:12) field PDM_UPDATE_RATE of register PMC_PCBI_REG_PMM_CONFIG_REG2 */
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
        /* (0x00000588 bits 18:12) field PDM_UPDATE_RATE of register PMC_PCBI_REG_PMM_CONFIG_REG2 */
        s16_csu_pcbi_reg_PMM_CONFIG_REG2_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (PCBI_REG_PMM_CONFIG_REG2_BIT_PDM_UPDATE_RATE_OFF + subfield_offset),
                                                    PCBI_REG_PMM_CONFIG_REG2_BIT_PDM_UPDATE_RATE_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_PDM_UPDATE_RATE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_PDM_UPDATE_RATE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_UPDATE_RATE_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_UPDATE_RATE_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_UPDATE_RATE_get", stop_bit, 6 );
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
    /* (0x00000588 bits 18:12) field PDM_UPDATE_RATE of register PMC_PCBI_REG_PMM_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & PCBI_REG_PMM_CONFIG_REG2_BIT_PDM_UPDATE_RATE_MSK)
                  >> PCBI_REG_PMM_CONFIG_REG2_BIT_PDM_UPDATE_RATE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_PMM_CONFIG_REG2_BIT_PDM_UPDATE_RATE_MSK, PCBI_REG_PMM_CONFIG_REG2_BIT_PDM_UPDATE_RATE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_UPDATE_RATE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_PMM_ENABLE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_PMM_ENABLE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PMM_ENABLE_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_PMM_ENABLE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_PMM_ENABLE_set", A, value );

    /* (0x00000588 bits 11) field PMM_ENABLE of register PMC_PCBI_REG_PMM_CONFIG_REG2 */
    s16_csu_pcbi_reg_PMM_CONFIG_REG2_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_ENABLE_MSK,
                                                PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_ENABLE_OFF,
                                                value);
}

static INLINE UINT32 s16_csu_pcbi_field_PMM_ENABLE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PMM_ENABLE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PMM_ENABLE_get", A, 15);
    /* (0x00000588 bits 11) field PMM_ENABLE of register PMC_PCBI_REG_PMM_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_ENABLE_MSK) >> PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_ENABLE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PMM_ENABLE_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_PMM_PKT_SIZE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_PMM_PKT_SIZE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PMM_PKT_SIZE_set", A, 15);
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_PMM_PKT_SIZE_set", value, 511);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_PMM_PKT_SIZE_set", A, value );

    /* (0x00000588 bits 10:2) field PMM_PKT_SIZE of register PMC_PCBI_REG_PMM_CONFIG_REG2 */
    s16_csu_pcbi_reg_PMM_CONFIG_REG2_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_PKT_SIZE_MSK,
                                                PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_PKT_SIZE_OFF,
                                                value);
}

static INLINE UINT32 s16_csu_pcbi_field_PMM_PKT_SIZE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PMM_PKT_SIZE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PMM_PKT_SIZE_get", A, 15);
    /* (0x00000588 bits 10:2) field PMM_PKT_SIZE of register PMC_PCBI_REG_PMM_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_PKT_SIZE_MSK) >> PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_PKT_SIZE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PMM_PKT_SIZE_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_PMM_PKT_SIZE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_PMM_PKT_SIZE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PMM_PKT_SIZE_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PMM_PKT_SIZE_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PMM_PKT_SIZE_set", stop_bit, 8 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PMM_PKT_SIZE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000588 bits 10:2) field PMM_PKT_SIZE of register PMC_PCBI_REG_PMM_CONFIG_REG2 */
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
        /* (0x00000588 bits 10:2) field PMM_PKT_SIZE of register PMC_PCBI_REG_PMM_CONFIG_REG2 */
        s16_csu_pcbi_reg_PMM_CONFIG_REG2_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_PKT_SIZE_OFF + subfield_offset),
                                                    PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_PKT_SIZE_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_PMM_PKT_SIZE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_PMM_PKT_SIZE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PMM_PKT_SIZE_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PMM_PKT_SIZE_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PMM_PKT_SIZE_get", stop_bit, 8 );
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
    /* (0x00000588 bits 10:2) field PMM_PKT_SIZE of register PMC_PCBI_REG_PMM_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_PKT_SIZE_MSK)
                  >> PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_PKT_SIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_PKT_SIZE_MSK, PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_PKT_SIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PMM_PKT_SIZE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_PMM_DSM_TYPE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_PMM_DSM_TYPE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PMM_DSM_TYPE_set", A, 15);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_PMM_DSM_TYPE_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_PMM_DSM_TYPE_set", A, value );

    /* (0x00000588 bits 1:0) field PMM_DSM_TYPE of register PMC_PCBI_REG_PMM_CONFIG_REG2 */
    s16_csu_pcbi_reg_PMM_CONFIG_REG2_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_DSM_TYPE_MSK,
                                                PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_DSM_TYPE_OFF,
                                                value);
}

static INLINE UINT32 s16_csu_pcbi_field_PMM_DSM_TYPE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PMM_DSM_TYPE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PMM_DSM_TYPE_get", A, 15);
    /* (0x00000588 bits 1:0) field PMM_DSM_TYPE of register PMC_PCBI_REG_PMM_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_DSM_TYPE_MSK) >> PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_DSM_TYPE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PMM_DSM_TYPE_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_PMM_DSM_TYPE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_PMM_DSM_TYPE_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PMM_DSM_TYPE_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PMM_DSM_TYPE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PMM_DSM_TYPE_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PMM_DSM_TYPE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000588 bits 1:0) field PMM_DSM_TYPE of register PMC_PCBI_REG_PMM_CONFIG_REG2 */
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
        /* (0x00000588 bits 1:0) field PMM_DSM_TYPE of register PMC_PCBI_REG_PMM_CONFIG_REG2 */
        s16_csu_pcbi_reg_PMM_CONFIG_REG2_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_DSM_TYPE_OFF + subfield_offset),
                                                    PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_DSM_TYPE_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_PMM_DSM_TYPE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_PMM_DSM_TYPE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PMM_DSM_TYPE_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PMM_DSM_TYPE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PMM_DSM_TYPE_get", stop_bit, 1 );
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
    /* (0x00000588 bits 1:0) field PMM_DSM_TYPE of register PMC_PCBI_REG_PMM_CONFIG_REG2 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_DSM_TYPE_MSK)
                  >> PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_DSM_TYPE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_DSM_TYPE_MSK, PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_DSM_TYPE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PMM_DSM_TYPE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_PDM_OVERRIDE_EN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_PDM_OVERRIDE_EN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_OVERRIDE_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_PDM_OVERRIDE_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_PDM_OVERRIDE_EN_set", A, value );

    /* (0x0000058c bits 31) field PDM_OVERRIDE_EN of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
    s16_csu_pcbi_reg_PMM_CONFIG_REG3_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_OVERRIDE_EN_MSK,
                                                PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_OVERRIDE_EN_OFF,
                                                value);
}

static INLINE UINT32 s16_csu_pcbi_field_PDM_OVERRIDE_EN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PDM_OVERRIDE_EN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_OVERRIDE_EN_get", A, 15);
    /* (0x0000058c bits 31) field PDM_OVERRIDE_EN of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG3_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_OVERRIDE_EN_MSK) >> PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_OVERRIDE_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PDM_OVERRIDE_EN_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_PDM_OVERRIDE_VAL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_PDM_OVERRIDE_VAL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_OVERRIDE_VAL_set", A, 15);
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_PDM_OVERRIDE_VAL_set", value, 4095);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_PDM_OVERRIDE_VAL_set", A, value );

    /* (0x0000058c bits 28:17) field PDM_OVERRIDE_VAL of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
    s16_csu_pcbi_reg_PMM_CONFIG_REG3_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_OVERRIDE_VAL_MSK,
                                                PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_OVERRIDE_VAL_OFF,
                                                value);
}

static INLINE UINT32 s16_csu_pcbi_field_PDM_OVERRIDE_VAL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PDM_OVERRIDE_VAL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_OVERRIDE_VAL_get", A, 15);
    /* (0x0000058c bits 28:17) field PDM_OVERRIDE_VAL of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG3_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_OVERRIDE_VAL_MSK) >> PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_OVERRIDE_VAL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PDM_OVERRIDE_VAL_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_PDM_OVERRIDE_VAL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_PDM_OVERRIDE_VAL_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_OVERRIDE_VAL_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_OVERRIDE_VAL_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_OVERRIDE_VAL_set", stop_bit, 11 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_OVERRIDE_VAL_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000058c bits 28:17) field PDM_OVERRIDE_VAL of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
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
        /* (0x0000058c bits 28:17) field PDM_OVERRIDE_VAL of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
        s16_csu_pcbi_reg_PMM_CONFIG_REG3_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_OVERRIDE_VAL_OFF + subfield_offset),
                                                    PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_OVERRIDE_VAL_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_PDM_OVERRIDE_VAL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_PDM_OVERRIDE_VAL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_OVERRIDE_VAL_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_OVERRIDE_VAL_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_OVERRIDE_VAL_get", stop_bit, 11 );
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
    /* (0x0000058c bits 28:17) field PDM_OVERRIDE_VAL of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG3_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_OVERRIDE_VAL_MSK)
                  >> PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_OVERRIDE_VAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_OVERRIDE_VAL_MSK, PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_OVERRIDE_VAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_OVERRIDE_VAL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_PDM_ACCUM_CYCLES_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_PDM_ACCUM_CYCLES_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_ACCUM_CYCLES_set", A, 15);
    if (value > 2047)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_PDM_ACCUM_CYCLES_set", value, 2047);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_PDM_ACCUM_CYCLES_set", A, value );

    /* (0x0000058c bits 16:6) field PDM_ACCUM_CYCLES of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
    s16_csu_pcbi_reg_PMM_CONFIG_REG3_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ACCUM_CYCLES_MSK,
                                                PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ACCUM_CYCLES_OFF,
                                                value);
}

static INLINE UINT32 s16_csu_pcbi_field_PDM_ACCUM_CYCLES_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PDM_ACCUM_CYCLES_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_ACCUM_CYCLES_get", A, 15);
    /* (0x0000058c bits 16:6) field PDM_ACCUM_CYCLES of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG3_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ACCUM_CYCLES_MSK) >> PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ACCUM_CYCLES_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PDM_ACCUM_CYCLES_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_PDM_ACCUM_CYCLES_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_PDM_ACCUM_CYCLES_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_ACCUM_CYCLES_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_ACCUM_CYCLES_set", stop_bit, start_bit );
    if (stop_bit > 10) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_ACCUM_CYCLES_set", stop_bit, 10 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_ACCUM_CYCLES_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000058c bits 16:6) field PDM_ACCUM_CYCLES of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
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
        if (stop_bit < 10) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 10;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000058c bits 16:6) field PDM_ACCUM_CYCLES of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
        s16_csu_pcbi_reg_PMM_CONFIG_REG3_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ACCUM_CYCLES_OFF + subfield_offset),
                                                    PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ACCUM_CYCLES_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_PDM_ACCUM_CYCLES_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_PDM_ACCUM_CYCLES_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_ACCUM_CYCLES_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_ACCUM_CYCLES_get", stop_bit, start_bit );
    if (stop_bit > 10) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_ACCUM_CYCLES_get", stop_bit, 10 );
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
    /* (0x0000058c bits 16:6) field PDM_ACCUM_CYCLES of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG3_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ACCUM_CYCLES_MSK)
                  >> PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ACCUM_CYCLES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ACCUM_CYCLES_MSK, PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ACCUM_CYCLES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_ACCUM_CYCLES_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_PDM_GAIN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                    s16_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_PDM_GAIN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                    s16_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_GAIN_set", A, 15);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_PDM_GAIN_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_PDM_GAIN_set", A, value );

    /* (0x0000058c bits 5:3) field PDM_GAIN of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
    s16_csu_pcbi_reg_PMM_CONFIG_REG3_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_GAIN_MSK,
                                                PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_GAIN_OFF,
                                                value);
}

static INLINE UINT32 s16_csu_pcbi_field_PDM_GAIN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PDM_GAIN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_GAIN_get", A, 15);
    /* (0x0000058c bits 5:3) field PDM_GAIN of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG3_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_GAIN_MSK) >> PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_GAIN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PDM_GAIN_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_PDM_GAIN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_PDM_GAIN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_GAIN_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_GAIN_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_GAIN_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000058c bits 5:3) field PDM_GAIN of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
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
        /* (0x0000058c bits 5:3) field PDM_GAIN of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
        s16_csu_pcbi_reg_PMM_CONFIG_REG3_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_GAIN_OFF + subfield_offset),
                                                    PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_GAIN_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_PDM_GAIN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_PDM_GAIN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_GAIN_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_GAIN_get", stop_bit, 2 );
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
    /* (0x0000058c bits 5:3) field PDM_GAIN of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG3_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_GAIN_MSK)
                  >> PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_GAIN_MSK, PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_GAIN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_PDM_ATTEN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                     s16_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_PDM_ATTEN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                     s16_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_ATTEN_set", A, 15);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_PDM_ATTEN_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_PDM_ATTEN_set", A, value );

    /* (0x0000058c bits 2:0) field PDM_ATTEN of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
    s16_csu_pcbi_reg_PMM_CONFIG_REG3_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ATTEN_MSK,
                                                PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ATTEN_OFF,
                                                value);
}

static INLINE UINT32 s16_csu_pcbi_field_PDM_ATTEN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PDM_ATTEN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_ATTEN_get", A, 15);
    /* (0x0000058c bits 2:0) field PDM_ATTEN of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG3_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ATTEN_MSK) >> PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ATTEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PDM_ATTEN_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_range_PDM_ATTEN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_range_PDM_ATTEN_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_ATTEN_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_ATTEN_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_ATTEN_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_ATTEN_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000058c bits 2:0) field PDM_ATTEN of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
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
        /* (0x0000058c bits 2:0) field PDM_ATTEN of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
        s16_csu_pcbi_reg_PMM_CONFIG_REG3_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ATTEN_OFF + subfield_offset),
                                                    PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ATTEN_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 s16_csu_pcbi_field_range_PDM_ATTEN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_PDM_ATTEN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_ATTEN_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_ATTEN_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_ATTEN_get", stop_bit, 2 );
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
    /* (0x0000058c bits 2:0) field PDM_ATTEN of register PMC_PCBI_REG_PMM_CONFIG_REG3 */
    reg_value = s16_csu_pcbi_reg_PMM_CONFIG_REG3_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ATTEN_MSK)
                  >> PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ATTEN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ATTEN_MSK, PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ATTEN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_ATTEN_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void s16_csu_pcbi_field_CSU0_PLL_RD_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_PLL_RD_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_PLL_RD_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_PLL_RD_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_PLL_RD_set", A, value );

    /* (0x00000510 bits 22) field CSU0_PLL_RD of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1 */
    s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_PLL_RD_MSK,
                                                         PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_PLL_RD_OFF,
                                                         value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_PLL_RD_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_PLL_RD_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_PLL_RD_get", A, 15);
    /* (0x00000510 bits 22) field CSU0_PLL_RD of register PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1 */
    reg_value = s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_PLL_RD_MSK) >> PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_PLL_RD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_PLL_RD_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_rising
 * ==================================================================================
 */
static INLINE void s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_I_set_to_clear", A, value );

    /* (0x00000574 bits 14) field UPDATE_RATE_OVERFLOW_I of register PMC_PCBI_REG_INT_REG */
    s16_csu_pcbi_reg_INT_REG_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        PCBI_REG_INT_REG_BIT_UPDATE_RATE_OVERFLOW_I_MSK,
                                                        PCBI_REG_INT_REG_BIT_UPDATE_RATE_OVERFLOW_I_OFF,
                                                        value);
}

static INLINE UINT32 s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_I_get", A, 15);
    /* (0x00000574 bits 14) field UPDATE_RATE_OVERFLOW_I of register PMC_PCBI_REG_INT_REG */
    reg_value = s16_csu_pcbi_reg_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_REG_BIT_UPDATE_RATE_OVERFLOW_I_MSK) >> PCBI_REG_INT_REG_BIT_UPDATE_RATE_OVERFLOW_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                   s16_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                   s16_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_I_poll", A, value );

    /* (0x00000574 bits 14) field UPDATE_RATE_OVERFLOW_I of register PMC_PCBI_REG_INT_REG */
    return s16_csu_pcbi_reg_INT_REG_poll( b_ptr,
                                          h_ptr,
                                          A,
                                          PCBI_REG_INT_REG_BIT_UPDATE_RATE_OVERFLOW_I_MSK,
                                          (value<<PCBI_REG_INT_REG_BIT_UPDATE_RATE_OVERFLOW_I_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE void s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_I_set_to_clear", A, value );

    /* (0x00000574 bits 13) field ACB_DESKEW_OVERFLOW_I of register PMC_PCBI_REG_INT_REG */
    s16_csu_pcbi_reg_INT_REG_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        PCBI_REG_INT_REG_BIT_ACB_DESKEW_OVERFLOW_I_MSK,
                                                        PCBI_REG_INT_REG_BIT_ACB_DESKEW_OVERFLOW_I_OFF,
                                                        value);
}

static INLINE UINT32 s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_I_get", A, 15);
    /* (0x00000574 bits 13) field ACB_DESKEW_OVERFLOW_I of register PMC_PCBI_REG_INT_REG */
    reg_value = s16_csu_pcbi_reg_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_REG_BIT_ACB_DESKEW_OVERFLOW_I_MSK) >> PCBI_REG_INT_REG_BIT_ACB_DESKEW_OVERFLOW_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                  s16_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                  s16_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_I_poll", A, value );

    /* (0x00000574 bits 13) field ACB_DESKEW_OVERFLOW_I of register PMC_PCBI_REG_INT_REG */
    return s16_csu_pcbi_reg_INT_REG_poll( b_ptr,
                                          h_ptr,
                                          A,
                                          PCBI_REG_INT_REG_BIT_ACB_DESKEW_OVERFLOW_I_MSK,
                                          (value<<PCBI_REG_INT_REG_BIT_ACB_DESKEW_OVERFLOW_I_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE void s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_I_set_to_clear", A, value );

    /* (0x00000574 bits 12) field ACB_DESKEW_UNDERFLOW_I of register PMC_PCBI_REG_INT_REG */
    s16_csu_pcbi_reg_INT_REG_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        PCBI_REG_INT_REG_BIT_ACB_DESKEW_UNDERFLOW_I_MSK,
                                                        PCBI_REG_INT_REG_BIT_ACB_DESKEW_UNDERFLOW_I_OFF,
                                                        value);
}

static INLINE UINT32 s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_I_get", A, 15);
    /* (0x00000574 bits 12) field ACB_DESKEW_UNDERFLOW_I of register PMC_PCBI_REG_INT_REG */
    reg_value = s16_csu_pcbi_reg_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_REG_BIT_ACB_DESKEW_UNDERFLOW_I_MSK) >> PCBI_REG_INT_REG_BIT_ACB_DESKEW_UNDERFLOW_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                   s16_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                   s16_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_I_poll", A, value );

    /* (0x00000574 bits 12) field ACB_DESKEW_UNDERFLOW_I of register PMC_PCBI_REG_INT_REG */
    return s16_csu_pcbi_reg_INT_REG_poll( b_ptr,
                                          h_ptr,
                                          A,
                                          PCBI_REG_INT_REG_BIT_ACB_DESKEW_UNDERFLOW_I_MSK,
                                          (value<<PCBI_REG_INT_REG_BIT_ACB_DESKEW_UNDERFLOW_I_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE void s16_csu_pcbi_field_VC_LPF_OUT1_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_VC_LPF_OUT1_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_VC_LPF_OUT1_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_VC_LPF_OUT1_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_VC_LPF_OUT1_I_set_to_clear", A, value );

    /* (0x00000574 bits 11) field VC_LPF_OUT1_I of register PMC_PCBI_REG_INT_REG */
    s16_csu_pcbi_reg_INT_REG_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        PCBI_REG_INT_REG_BIT_VC_LPF_OUT1_I_MSK,
                                                        PCBI_REG_INT_REG_BIT_VC_LPF_OUT1_I_OFF,
                                                        value);
}

static INLINE UINT32 s16_csu_pcbi_field_VC_LPF_OUT1_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_VC_LPF_OUT1_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_VC_LPF_OUT1_I_get", A, 15);
    /* (0x00000574 bits 11) field VC_LPF_OUT1_I of register PMC_PCBI_REG_INT_REG */
    reg_value = s16_csu_pcbi_reg_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_REG_BIT_VC_LPF_OUT1_I_MSK) >> PCBI_REG_INT_REG_BIT_VC_LPF_OUT1_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_VC_LPF_OUT1_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_VC_LPF_OUT1_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_VC_LPF_OUT1_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_VC_LPF_OUT1_I_poll", A, value );

    /* (0x00000574 bits 11) field VC_LPF_OUT1_I of register PMC_PCBI_REG_INT_REG */
    return s16_csu_pcbi_reg_INT_REG_poll( b_ptr,
                                          h_ptr,
                                          A,
                                          PCBI_REG_INT_REG_BIT_VC_LPF_OUT1_I_MSK,
                                          (value<<PCBI_REG_INT_REG_BIT_VC_LPF_OUT1_I_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE void s16_csu_pcbi_field_VC_LPF_OUT0_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_VC_LPF_OUT0_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_VC_LPF_OUT0_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_VC_LPF_OUT0_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_VC_LPF_OUT0_I_set_to_clear", A, value );

    /* (0x00000574 bits 10) field VC_LPF_OUT0_I of register PMC_PCBI_REG_INT_REG */
    s16_csu_pcbi_reg_INT_REG_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        PCBI_REG_INT_REG_BIT_VC_LPF_OUT0_I_MSK,
                                                        PCBI_REG_INT_REG_BIT_VC_LPF_OUT0_I_OFF,
                                                        value);
}

static INLINE UINT32 s16_csu_pcbi_field_VC_LPF_OUT0_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_VC_LPF_OUT0_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_VC_LPF_OUT0_I_get", A, 15);
    /* (0x00000574 bits 10) field VC_LPF_OUT0_I of register PMC_PCBI_REG_INT_REG */
    reg_value = s16_csu_pcbi_reg_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_REG_BIT_VC_LPF_OUT0_I_MSK) >> PCBI_REG_INT_REG_BIT_VC_LPF_OUT0_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_VC_LPF_OUT0_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_VC_LPF_OUT0_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_VC_LPF_OUT0_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_VC_LPF_OUT0_I_poll", A, value );

    /* (0x00000574 bits 10) field VC_LPF_OUT0_I of register PMC_PCBI_REG_INT_REG */
    return s16_csu_pcbi_reg_INT_REG_poll( b_ptr,
                                          h_ptr,
                                          A,
                                          PCBI_REG_INT_REG_BIT_VC_LPF_OUT0_I_MSK,
                                          (value<<PCBI_REG_INT_REG_BIT_VC_LPF_OUT0_I_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE void s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                           s16_handle_t *h_ptr,
                                                                                           UINT32 A,
                                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                           s16_handle_t *h_ptr,
                                                                                           UINT32 A,
                                                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_set_to_clear", A, value );

    /* (0x00000574 bits 9) field CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I of register PMC_PCBI_REG_INT_REG */
    s16_csu_pcbi_reg_INT_REG_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        PCBI_REG_INT_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_MSK,
                                                        PCBI_REG_INT_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_OFF,
                                                        value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                    s16_handle_t *h_ptr,
                                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                    s16_handle_t *h_ptr,
                                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_get", A, 15);
    /* (0x00000574 bits 9) field CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I of register PMC_PCBI_REG_INT_REG */
    reg_value = s16_csu_pcbi_reg_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_MSK) >> PCBI_REG_INT_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                                   s16_handle_t *h_ptr,
                                                                                                   UINT32 A,
                                                                                                   UINT32 value,
                                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                                   UINT32 max_count,
                                                                                                   UINT32 *num_failed_polls,
                                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                                   s16_handle_t *h_ptr,
                                                                                                   UINT32 A,
                                                                                                   UINT32 value,
                                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                                   UINT32 max_count,
                                                                                                   UINT32 *num_failed_polls,
                                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_poll", A, value );

    /* (0x00000574 bits 9) field CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I of register PMC_PCBI_REG_INT_REG */
    return s16_csu_pcbi_reg_INT_REG_poll( b_ptr,
                                          h_ptr,
                                          A,
                                          PCBI_REG_INT_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_MSK,
                                          (value<<PCBI_REG_INT_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE void s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                              s16_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                              s16_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_I_set_to_clear", A, value );

    /* (0x00000574 bits 8) field CSU0_EADSM_LOCKDET_LOCK_I of register PMC_PCBI_REG_INT_REG */
    s16_csu_pcbi_reg_INT_REG_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        PCBI_REG_INT_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_I_MSK,
                                                        PCBI_REG_INT_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_I_OFF,
                                                        value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_I_get", A, 15);
    /* (0x00000574 bits 8) field CSU0_EADSM_LOCKDET_LOCK_I of register PMC_PCBI_REG_INT_REG */
    reg_value = s16_csu_pcbi_reg_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_I_MSK) >> PCBI_REG_INT_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                      s16_handle_t *h_ptr,
                                                                                      UINT32 A,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                      s16_handle_t *h_ptr,
                                                                                      UINT32 A,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_I_poll", A, value );

    /* (0x00000574 bits 8) field CSU0_EADSM_LOCKDET_LOCK_I of register PMC_PCBI_REG_INT_REG */
    return s16_csu_pcbi_reg_INT_REG_poll( b_ptr,
                                          h_ptr,
                                          A,
                                          PCBI_REG_INT_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_I_MSK,
                                          (value<<PCBI_REG_INT_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_I_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE void s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                   s16_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                   s16_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_set_to_clear", A, value );

    /* (0x00000574 bits 7) field CSU0_PLL_LOCKDET_PPM_OFF_UPD_I of register PMC_PCBI_REG_INT_REG */
    s16_csu_pcbi_reg_INT_REG_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        PCBI_REG_INT_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_MSK,
                                                        PCBI_REG_INT_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_OFF,
                                                        value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_get", A, 15);
    /* (0x00000574 bits 7) field CSU0_PLL_LOCKDET_PPM_OFF_UPD_I of register PMC_PCBI_REG_INT_REG */
    reg_value = s16_csu_pcbi_reg_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_MSK) >> PCBI_REG_INT_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                           s16_handle_t *h_ptr,
                                                                                           UINT32 A,
                                                                                           UINT32 value,
                                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                                           UINT32 max_count,
                                                                                           UINT32 *num_failed_polls,
                                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                           s16_handle_t *h_ptr,
                                                                                           UINT32 A,
                                                                                           UINT32 value,
                                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                                           UINT32 max_count,
                                                                                           UINT32 *num_failed_polls,
                                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_poll", A, value );

    /* (0x00000574 bits 7) field CSU0_PLL_LOCKDET_PPM_OFF_UPD_I of register PMC_PCBI_REG_INT_REG */
    return s16_csu_pcbi_reg_INT_REG_poll( b_ptr,
                                          h_ptr,
                                          A,
                                          PCBI_REG_INT_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_MSK,
                                          (value<<PCBI_REG_INT_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE void s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_I_set_to_clear", A, value );

    /* (0x00000574 bits 6) field CSU0_VCOC_BIST_ERR_I of register PMC_PCBI_REG_INT_REG */
    s16_csu_pcbi_reg_INT_REG_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        PCBI_REG_INT_REG_BIT_CSU0_VCOC_BIST_ERR_I_MSK,
                                                        PCBI_REG_INT_REG_BIT_CSU0_VCOC_BIST_ERR_I_OFF,
                                                        value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_I_get", A, 15);
    /* (0x00000574 bits 6) field CSU0_VCOC_BIST_ERR_I of register PMC_PCBI_REG_INT_REG */
    reg_value = s16_csu_pcbi_reg_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_REG_BIT_CSU0_VCOC_BIST_ERR_I_MSK) >> PCBI_REG_INT_REG_BIT_CSU0_VCOC_BIST_ERR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                 s16_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                 s16_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_I_poll", A, value );

    /* (0x00000574 bits 6) field CSU0_VCOC_BIST_ERR_I of register PMC_PCBI_REG_INT_REG */
    return s16_csu_pcbi_reg_INT_REG_poll( b_ptr,
                                          h_ptr,
                                          A,
                                          PCBI_REG_INT_REG_BIT_CSU0_VCOC_BIST_ERR_I_MSK,
                                          (value<<PCBI_REG_INT_REG_BIT_CSU0_VCOC_BIST_ERR_I_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE void s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_I_set_to_clear", A, value );

    /* (0x00000574 bits 5) field CSU0_DIVCLK_DEAD_I of register PMC_PCBI_REG_INT_REG */
    s16_csu_pcbi_reg_INT_REG_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        PCBI_REG_INT_REG_BIT_CSU0_DIVCLK_DEAD_I_MSK,
                                                        PCBI_REG_INT_REG_BIT_CSU0_DIVCLK_DEAD_I_OFF,
                                                        value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_I_get", A, 15);
    /* (0x00000574 bits 5) field CSU0_DIVCLK_DEAD_I of register PMC_PCBI_REG_INT_REG */
    reg_value = s16_csu_pcbi_reg_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_REG_BIT_CSU0_DIVCLK_DEAD_I_MSK) >> PCBI_REG_INT_REG_BIT_CSU0_DIVCLK_DEAD_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                               s16_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                               s16_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_I_poll", A, value );

    /* (0x00000574 bits 5) field CSU0_DIVCLK_DEAD_I of register PMC_PCBI_REG_INT_REG */
    return s16_csu_pcbi_reg_INT_REG_poll( b_ptr,
                                          h_ptr,
                                          A,
                                          PCBI_REG_INT_REG_BIT_CSU0_DIVCLK_DEAD_I_MSK,
                                          (value<<PCBI_REG_INT_REG_BIT_CSU0_DIVCLK_DEAD_I_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE void s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_I_set_to_clear", A, value );

    /* (0x00000574 bits 4) field CSU0_IN_RANGE_N_FAILS_I of register PMC_PCBI_REG_INT_REG */
    s16_csu_pcbi_reg_INT_REG_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        PCBI_REG_INT_REG_BIT_CSU0_IN_RANGE_N_FAILS_I_MSK,
                                                        PCBI_REG_INT_REG_BIT_CSU0_IN_RANGE_N_FAILS_I_OFF,
                                                        value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_I_get", A, 15);
    /* (0x00000574 bits 4) field CSU0_IN_RANGE_N_FAILS_I of register PMC_PCBI_REG_INT_REG */
    reg_value = s16_csu_pcbi_reg_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_REG_BIT_CSU0_IN_RANGE_N_FAILS_I_MSK) >> PCBI_REG_INT_REG_BIT_CSU0_IN_RANGE_N_FAILS_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                    s16_handle_t *h_ptr,
                                                                                    UINT32 A,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                    s16_handle_t *h_ptr,
                                                                                    UINT32 A,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_I_poll", A, value );

    /* (0x00000574 bits 4) field CSU0_IN_RANGE_N_FAILS_I of register PMC_PCBI_REG_INT_REG */
    return s16_csu_pcbi_reg_INT_REG_poll( b_ptr,
                                          h_ptr,
                                          A,
                                          PCBI_REG_INT_REG_BIT_CSU0_IN_RANGE_N_FAILS_I_MSK,
                                          (value<<PCBI_REG_INT_REG_BIT_CSU0_IN_RANGE_N_FAILS_I_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE void s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_I_set_to_clear", A, value );

    /* (0x00000574 bits 3) field CSU0_IN_RANGE_N_PASSES_I of register PMC_PCBI_REG_INT_REG */
    s16_csu_pcbi_reg_INT_REG_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        PCBI_REG_INT_REG_BIT_CSU0_IN_RANGE_N_PASSES_I_MSK,
                                                        PCBI_REG_INT_REG_BIT_CSU0_IN_RANGE_N_PASSES_I_OFF,
                                                        value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_I_get", A, 15);
    /* (0x00000574 bits 3) field CSU0_IN_RANGE_N_PASSES_I of register PMC_PCBI_REG_INT_REG */
    reg_value = s16_csu_pcbi_reg_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_REG_BIT_CSU0_IN_RANGE_N_PASSES_I_MSK) >> PCBI_REG_INT_REG_BIT_CSU0_IN_RANGE_N_PASSES_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                     s16_handle_t *h_ptr,
                                                                                     UINT32 A,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                     s16_handle_t *h_ptr,
                                                                                     UINT32 A,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_I_poll", A, value );

    /* (0x00000574 bits 3) field CSU0_IN_RANGE_N_PASSES_I of register PMC_PCBI_REG_INT_REG */
    return s16_csu_pcbi_reg_INT_REG_poll( b_ptr,
                                          h_ptr,
                                          A,
                                          PCBI_REG_INT_REG_BIT_CSU0_IN_RANGE_N_PASSES_I_MSK,
                                          (value<<PCBI_REG_INT_REG_BIT_CSU0_IN_RANGE_N_PASSES_I_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE void s16_csu_pcbi_field_CSU0_LOCK_LOSS_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_LOCK_LOSS_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_LOCK_LOSS_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_LOSS_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_LOSS_I_set_to_clear", A, value );

    /* (0x00000574 bits 2) field CSU0_LOCK_LOSS_I of register PMC_PCBI_REG_INT_REG */
    s16_csu_pcbi_reg_INT_REG_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        PCBI_REG_INT_REG_BIT_CSU0_LOCK_LOSS_I_MSK,
                                                        PCBI_REG_INT_REG_BIT_CSU0_LOCK_LOSS_I_OFF,
                                                        value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_LOCK_LOSS_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_LOCK_LOSS_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_LOCK_LOSS_I_get", A, 15);
    /* (0x00000574 bits 2) field CSU0_LOCK_LOSS_I of register PMC_PCBI_REG_INT_REG */
    reg_value = s16_csu_pcbi_reg_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_REG_BIT_CSU0_LOCK_LOSS_I_MSK) >> PCBI_REG_INT_REG_BIT_CSU0_LOCK_LOSS_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_LOSS_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_LOCK_LOSS_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_LOCK_LOSS_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_LOSS_I_poll", A, value );

    /* (0x00000574 bits 2) field CSU0_LOCK_LOSS_I of register PMC_PCBI_REG_INT_REG */
    return s16_csu_pcbi_reg_INT_REG_poll( b_ptr,
                                          h_ptr,
                                          A,
                                          PCBI_REG_INT_REG_BIT_CSU0_LOCK_LOSS_I_MSK,
                                          (value<<PCBI_REG_INT_REG_BIT_CSU0_LOCK_LOSS_I_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE void s16_csu_pcbi_field_CSU0_LOCK_GAIN_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_LOCK_GAIN_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_LOCK_GAIN_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_GAIN_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_GAIN_I_set_to_clear", A, value );

    /* (0x00000574 bits 1) field CSU0_LOCK_GAIN_I of register PMC_PCBI_REG_INT_REG */
    s16_csu_pcbi_reg_INT_REG_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        PCBI_REG_INT_REG_BIT_CSU0_LOCK_GAIN_I_MSK,
                                                        PCBI_REG_INT_REG_BIT_CSU0_LOCK_GAIN_I_OFF,
                                                        value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_LOCK_GAIN_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_LOCK_GAIN_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_LOCK_GAIN_I_get", A, 15);
    /* (0x00000574 bits 1) field CSU0_LOCK_GAIN_I of register PMC_PCBI_REG_INT_REG */
    reg_value = s16_csu_pcbi_reg_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_REG_BIT_CSU0_LOCK_GAIN_I_MSK) >> PCBI_REG_INT_REG_BIT_CSU0_LOCK_GAIN_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_GAIN_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_LOCK_GAIN_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_LOCK_GAIN_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_GAIN_I_poll", A, value );

    /* (0x00000574 bits 1) field CSU0_LOCK_GAIN_I of register PMC_PCBI_REG_INT_REG */
    return s16_csu_pcbi_reg_INT_REG_poll( b_ptr,
                                          h_ptr,
                                          A,
                                          PCBI_REG_INT_REG_BIT_CSU0_LOCK_GAIN_I_MSK,
                                          (value<<PCBI_REG_INT_REG_BIT_CSU0_LOCK_GAIN_I_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE void s16_csu_pcbi_field_CSU0_VCOC_DONE_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_VCOC_DONE_I_set_to_clear( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_DONE_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_DONE_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_DONE_I_set_to_clear", A, value );

    /* (0x00000574 bits 0) field CSU0_VCOC_DONE_I of register PMC_PCBI_REG_INT_REG */
    s16_csu_pcbi_reg_INT_REG_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        PCBI_REG_INT_REG_BIT_CSU0_VCOC_DONE_I_MSK,
                                                        PCBI_REG_INT_REG_BIT_CSU0_VCOC_DONE_I_OFF,
                                                        value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_DONE_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_DONE_I_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_DONE_I_get", A, 15);
    /* (0x00000574 bits 0) field CSU0_VCOC_DONE_I of register PMC_PCBI_REG_INT_REG */
    reg_value = s16_csu_pcbi_reg_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_REG_BIT_CSU0_VCOC_DONE_I_MSK) >> PCBI_REG_INT_REG_BIT_CSU0_VCOC_DONE_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_DONE_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_VCOC_DONE_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_VCOC_DONE_I_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_DONE_I_poll", A, value );

    /* (0x00000574 bits 0) field CSU0_VCOC_DONE_I of register PMC_PCBI_REG_INT_REG */
    return s16_csu_pcbi_reg_INT_REG_poll( b_ptr,
                                          h_ptr,
                                          A,
                                          PCBI_REG_INT_REG_BIT_CSU0_VCOC_DONE_I_MSK,
                                          (value<<PCBI_REG_INT_REG_BIT_CSU0_VCOC_DONE_I_OFF),
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
static INLINE void s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_E_set", A, value );

    /* (0x00000570 bits 14) field UPDATE_RATE_OVERFLOW_E of register PMC_PCBI_REG_INT_EN_REG */
    s16_csu_pcbi_reg_INT_EN_REG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           PCBI_REG_INT_EN_REG_BIT_UPDATE_RATE_OVERFLOW_E_MSK,
                                           PCBI_REG_INT_EN_REG_BIT_UPDATE_RATE_OVERFLOW_E_OFF,
                                           value);
}

static INLINE UINT32 s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_E_get", A, 15);
    /* (0x00000570 bits 14) field UPDATE_RATE_OVERFLOW_E of register PMC_PCBI_REG_INT_EN_REG */
    reg_value = s16_csu_pcbi_reg_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_EN_REG_BIT_UPDATE_RATE_OVERFLOW_E_MSK) >> PCBI_REG_INT_EN_REG_BIT_UPDATE_RATE_OVERFLOW_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_E_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_E_set", A, value );

    /* (0x00000570 bits 13) field ACB_DESKEW_OVERFLOW_E of register PMC_PCBI_REG_INT_EN_REG */
    s16_csu_pcbi_reg_INT_EN_REG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           PCBI_REG_INT_EN_REG_BIT_ACB_DESKEW_OVERFLOW_E_MSK,
                                           PCBI_REG_INT_EN_REG_BIT_ACB_DESKEW_OVERFLOW_E_OFF,
                                           value);
}

static INLINE UINT32 s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_E_get", A, 15);
    /* (0x00000570 bits 13) field ACB_DESKEW_OVERFLOW_E of register PMC_PCBI_REG_INT_EN_REG */
    reg_value = s16_csu_pcbi_reg_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_EN_REG_BIT_ACB_DESKEW_OVERFLOW_E_MSK) >> PCBI_REG_INT_EN_REG_BIT_ACB_DESKEW_OVERFLOW_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_E_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_E_set", A, value );

    /* (0x00000570 bits 12) field ACB_DESKEW_UNDERFLOW_E of register PMC_PCBI_REG_INT_EN_REG */
    s16_csu_pcbi_reg_INT_EN_REG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           PCBI_REG_INT_EN_REG_BIT_ACB_DESKEW_UNDERFLOW_E_MSK,
                                           PCBI_REG_INT_EN_REG_BIT_ACB_DESKEW_UNDERFLOW_E_OFF,
                                           value);
}

static INLINE UINT32 s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_E_get", A, 15);
    /* (0x00000570 bits 12) field ACB_DESKEW_UNDERFLOW_E of register PMC_PCBI_REG_INT_EN_REG */
    reg_value = s16_csu_pcbi_reg_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_EN_REG_BIT_ACB_DESKEW_UNDERFLOW_E_MSK) >> PCBI_REG_INT_EN_REG_BIT_ACB_DESKEW_UNDERFLOW_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_E_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_VC_LPF_OUT1_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_VC_LPF_OUT1_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_VC_LPF_OUT1_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_VC_LPF_OUT1_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_VC_LPF_OUT1_E_set", A, value );

    /* (0x00000570 bits 11) field VC_LPF_OUT1_E of register PMC_PCBI_REG_INT_EN_REG */
    s16_csu_pcbi_reg_INT_EN_REG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           PCBI_REG_INT_EN_REG_BIT_VC_LPF_OUT1_E_MSK,
                                           PCBI_REG_INT_EN_REG_BIT_VC_LPF_OUT1_E_OFF,
                                           value);
}

static INLINE UINT32 s16_csu_pcbi_field_VC_LPF_OUT1_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_VC_LPF_OUT1_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_VC_LPF_OUT1_E_get", A, 15);
    /* (0x00000570 bits 11) field VC_LPF_OUT1_E of register PMC_PCBI_REG_INT_EN_REG */
    reg_value = s16_csu_pcbi_reg_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_EN_REG_BIT_VC_LPF_OUT1_E_MSK) >> PCBI_REG_INT_EN_REG_BIT_VC_LPF_OUT1_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_VC_LPF_OUT1_E_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_VC_LPF_OUT0_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_VC_LPF_OUT0_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_VC_LPF_OUT0_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_VC_LPF_OUT0_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_VC_LPF_OUT0_E_set", A, value );

    /* (0x00000570 bits 10) field VC_LPF_OUT0_E of register PMC_PCBI_REG_INT_EN_REG */
    s16_csu_pcbi_reg_INT_EN_REG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           PCBI_REG_INT_EN_REG_BIT_VC_LPF_OUT0_E_MSK,
                                           PCBI_REG_INT_EN_REG_BIT_VC_LPF_OUT0_E_OFF,
                                           value);
}

static INLINE UINT32 s16_csu_pcbi_field_VC_LPF_OUT0_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_VC_LPF_OUT0_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_VC_LPF_OUT0_E_get", A, 15);
    /* (0x00000570 bits 10) field VC_LPF_OUT0_E of register PMC_PCBI_REG_INT_EN_REG */
    reg_value = s16_csu_pcbi_reg_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_EN_REG_BIT_VC_LPF_OUT0_E_MSK) >> PCBI_REG_INT_EN_REG_BIT_VC_LPF_OUT0_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_VC_LPF_OUT0_E_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                  s16_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                  s16_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E_set", A, value );

    /* (0x00000570 bits 9) field CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E of register PMC_PCBI_REG_INT_EN_REG */
    s16_csu_pcbi_reg_INT_EN_REG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E_MSK,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E_OFF,
                                           value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                    s16_handle_t *h_ptr,
                                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                    s16_handle_t *h_ptr,
                                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E_get", A, 15);
    /* (0x00000570 bits 9) field CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E of register PMC_PCBI_REG_INT_EN_REG */
    reg_value = s16_csu_pcbi_reg_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_EN_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E_MSK) >> PCBI_REG_INT_EN_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_E_set", A, value );

    /* (0x00000570 bits 8) field CSU0_EADSM_LOCKDET_LOCK_E of register PMC_PCBI_REG_INT_EN_REG */
    s16_csu_pcbi_reg_INT_EN_REG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_E_MSK,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_E_OFF,
                                           value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_E_get", A, 15);
    /* (0x00000570 bits 8) field CSU0_EADSM_LOCKDET_LOCK_E of register PMC_PCBI_REG_INT_EN_REG */
    reg_value = s16_csu_pcbi_reg_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_EN_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_E_MSK) >> PCBI_REG_INT_EN_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_E_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_E_set", A, value );

    /* (0x00000570 bits 7) field CSU0_PLL_LOCKDET_PPM_OFF_UPD_E of register PMC_PCBI_REG_INT_EN_REG */
    s16_csu_pcbi_reg_INT_EN_REG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_E_MSK,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_E_OFF,
                                           value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_E_get", A, 15);
    /* (0x00000570 bits 7) field CSU0_PLL_LOCKDET_PPM_OFF_UPD_E of register PMC_PCBI_REG_INT_EN_REG */
    reg_value = s16_csu_pcbi_reg_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_EN_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_E_MSK) >> PCBI_REG_INT_EN_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_E_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_E_set", A, value );

    /* (0x00000570 bits 6) field CSU0_VCOC_BIST_ERR_E of register PMC_PCBI_REG_INT_EN_REG */
    s16_csu_pcbi_reg_INT_EN_REG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_VCOC_BIST_ERR_E_MSK,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_VCOC_BIST_ERR_E_OFF,
                                           value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_E_get", A, 15);
    /* (0x00000570 bits 6) field CSU0_VCOC_BIST_ERR_E of register PMC_PCBI_REG_INT_EN_REG */
    reg_value = s16_csu_pcbi_reg_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_EN_REG_BIT_CSU0_VCOC_BIST_ERR_E_MSK) >> PCBI_REG_INT_EN_REG_BIT_CSU0_VCOC_BIST_ERR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_E_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_E_set", A, value );

    /* (0x00000570 bits 5) field CSU0_DIVCLK_DEAD_E of register PMC_PCBI_REG_INT_EN_REG */
    s16_csu_pcbi_reg_INT_EN_REG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_DIVCLK_DEAD_E_MSK,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_DIVCLK_DEAD_E_OFF,
                                           value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_E_get", A, 15);
    /* (0x00000570 bits 5) field CSU0_DIVCLK_DEAD_E of register PMC_PCBI_REG_INT_EN_REG */
    reg_value = s16_csu_pcbi_reg_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_EN_REG_BIT_CSU0_DIVCLK_DEAD_E_MSK) >> PCBI_REG_INT_EN_REG_BIT_CSU0_DIVCLK_DEAD_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_E_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_E_set", A, value );

    /* (0x00000570 bits 4) field CSU0_IN_RANGE_N_FAILS_E of register PMC_PCBI_REG_INT_EN_REG */
    s16_csu_pcbi_reg_INT_EN_REG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_IN_RANGE_N_FAILS_E_MSK,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_IN_RANGE_N_FAILS_E_OFF,
                                           value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_E_get", A, 15);
    /* (0x00000570 bits 4) field CSU0_IN_RANGE_N_FAILS_E of register PMC_PCBI_REG_INT_EN_REG */
    reg_value = s16_csu_pcbi_reg_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_EN_REG_BIT_CSU0_IN_RANGE_N_FAILS_E_MSK) >> PCBI_REG_INT_EN_REG_BIT_CSU0_IN_RANGE_N_FAILS_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_E_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_E_set", A, value );

    /* (0x00000570 bits 3) field CSU0_IN_RANGE_N_PASSES_E of register PMC_PCBI_REG_INT_EN_REG */
    s16_csu_pcbi_reg_INT_EN_REG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_IN_RANGE_N_PASSES_E_MSK,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_IN_RANGE_N_PASSES_E_OFF,
                                           value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_E_get", A, 15);
    /* (0x00000570 bits 3) field CSU0_IN_RANGE_N_PASSES_E of register PMC_PCBI_REG_INT_EN_REG */
    reg_value = s16_csu_pcbi_reg_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_EN_REG_BIT_CSU0_IN_RANGE_N_PASSES_E_MSK) >> PCBI_REG_INT_EN_REG_BIT_CSU0_IN_RANGE_N_PASSES_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_E_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_LOCK_LOSS_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_LOCK_LOSS_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_LOCK_LOSS_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_LOSS_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_LOSS_E_set", A, value );

    /* (0x00000570 bits 2) field CSU0_LOCK_LOSS_E of register PMC_PCBI_REG_INT_EN_REG */
    s16_csu_pcbi_reg_INT_EN_REG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_LOCK_LOSS_E_MSK,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_LOCK_LOSS_E_OFF,
                                           value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_LOCK_LOSS_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_LOCK_LOSS_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_LOCK_LOSS_E_get", A, 15);
    /* (0x00000570 bits 2) field CSU0_LOCK_LOSS_E of register PMC_PCBI_REG_INT_EN_REG */
    reg_value = s16_csu_pcbi_reg_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_EN_REG_BIT_CSU0_LOCK_LOSS_E_MSK) >> PCBI_REG_INT_EN_REG_BIT_CSU0_LOCK_LOSS_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_LOSS_E_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_LOCK_GAIN_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_LOCK_GAIN_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_LOCK_GAIN_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_GAIN_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_GAIN_E_set", A, value );

    /* (0x00000570 bits 1) field CSU0_LOCK_GAIN_E of register PMC_PCBI_REG_INT_EN_REG */
    s16_csu_pcbi_reg_INT_EN_REG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_LOCK_GAIN_E_MSK,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_LOCK_GAIN_E_OFF,
                                           value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_LOCK_GAIN_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_LOCK_GAIN_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_LOCK_GAIN_E_get", A, 15);
    /* (0x00000570 bits 1) field CSU0_LOCK_GAIN_E of register PMC_PCBI_REG_INT_EN_REG */
    reg_value = s16_csu_pcbi_reg_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_EN_REG_BIT_CSU0_LOCK_GAIN_E_MSK) >> PCBI_REG_INT_EN_REG_BIT_CSU0_LOCK_GAIN_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_GAIN_E_get", A, value );

    return value;
}
static INLINE void s16_csu_pcbi_field_CSU0_VCOC_DONE_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_csu_pcbi_field_CSU0_VCOC_DONE_E_set( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_DONE_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_DONE_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_DONE_E_set", A, value );

    /* (0x00000570 bits 0) field CSU0_VCOC_DONE_E of register PMC_PCBI_REG_INT_EN_REG */
    s16_csu_pcbi_reg_INT_EN_REG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_VCOC_DONE_E_MSK,
                                           PCBI_REG_INT_EN_REG_BIT_CSU0_VCOC_DONE_E_OFF,
                                           value);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_DONE_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_DONE_E_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_DONE_E_get", A, 15);
    /* (0x00000570 bits 0) field CSU0_VCOC_DONE_E of register PMC_PCBI_REG_INT_EN_REG */
    reg_value = s16_csu_pcbi_reg_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCBI_REG_INT_EN_REG_BIT_CSU0_VCOC_DONE_E_MSK) >> PCBI_REG_INT_EN_REG_BIT_CSU0_VCOC_DONE_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_DONE_E_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value
 * ==================================================================================
 */
static INLINE UINT32 s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_V_get", A, 15);
    /* (0x00000578 bits 14) field UPDATE_RATE_OVERFLOW_V of register PMC_PCBI_REG_INT_VAL_REG */
    reg_value = s16_csu_pcbi_reg_INT_VAL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PCBI_REG_INT_VAL_REG_BIT_UPDATE_RATE_OVERFLOW_V_MSK) >> PCBI_REG_INT_VAL_REG_BIT_UPDATE_RATE_OVERFLOW_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                   s16_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                   s16_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_V_poll", A, value );

    /* (0x00000578 bits 14) field UPDATE_RATE_OVERFLOW_V of register PMC_PCBI_REG_INT_VAL_REG */
    return s16_csu_pcbi_reg_INT_VAL_REG_poll( b_ptr,
                                              h_ptr,
                                              A,
                                              PCBI_REG_INT_VAL_REG_BIT_UPDATE_RATE_OVERFLOW_V_MSK,
                                              (value<<PCBI_REG_INT_VAL_REG_BIT_UPDATE_RATE_OVERFLOW_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_V_get", A, 15);
    /* (0x00000578 bits 13) field ACB_DESKEW_OVERFLOW_V of register PMC_PCBI_REG_INT_VAL_REG */
    reg_value = s16_csu_pcbi_reg_INT_VAL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PCBI_REG_INT_VAL_REG_BIT_ACB_DESKEW_OVERFLOW_V_MSK) >> PCBI_REG_INT_VAL_REG_BIT_ACB_DESKEW_OVERFLOW_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                  s16_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                  s16_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_V_poll", A, value );

    /* (0x00000578 bits 13) field ACB_DESKEW_OVERFLOW_V of register PMC_PCBI_REG_INT_VAL_REG */
    return s16_csu_pcbi_reg_INT_VAL_REG_poll( b_ptr,
                                              h_ptr,
                                              A,
                                              PCBI_REG_INT_VAL_REG_BIT_ACB_DESKEW_OVERFLOW_V_MSK,
                                              (value<<PCBI_REG_INT_VAL_REG_BIT_ACB_DESKEW_OVERFLOW_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_V_get", A, 15);
    /* (0x00000578 bits 12) field ACB_DESKEW_UNDERFLOW_V of register PMC_PCBI_REG_INT_VAL_REG */
    reg_value = s16_csu_pcbi_reg_INT_VAL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PCBI_REG_INT_VAL_REG_BIT_ACB_DESKEW_UNDERFLOW_V_MSK) >> PCBI_REG_INT_VAL_REG_BIT_ACB_DESKEW_UNDERFLOW_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                   s16_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                   s16_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_V_poll", A, value );

    /* (0x00000578 bits 12) field ACB_DESKEW_UNDERFLOW_V of register PMC_PCBI_REG_INT_VAL_REG */
    return s16_csu_pcbi_reg_INT_VAL_REG_poll( b_ptr,
                                              h_ptr,
                                              A,
                                              PCBI_REG_INT_VAL_REG_BIT_ACB_DESKEW_UNDERFLOW_V_MSK,
                                              (value<<PCBI_REG_INT_VAL_REG_BIT_ACB_DESKEW_UNDERFLOW_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 s16_csu_pcbi_field_VC_LPF_OUT1_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_VC_LPF_OUT1_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_VC_LPF_OUT1_V_get", A, 15);
    /* (0x00000578 bits 11) field VC_LPF_OUT1_V of register PMC_PCBI_REG_INT_VAL_REG */
    reg_value = s16_csu_pcbi_reg_INT_VAL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PCBI_REG_INT_VAL_REG_BIT_VC_LPF_OUT1_V_MSK) >> PCBI_REG_INT_VAL_REG_BIT_VC_LPF_OUT1_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_VC_LPF_OUT1_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_VC_LPF_OUT1_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_VC_LPF_OUT1_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_VC_LPF_OUT1_V_poll", A, value );

    /* (0x00000578 bits 11) field VC_LPF_OUT1_V of register PMC_PCBI_REG_INT_VAL_REG */
    return s16_csu_pcbi_reg_INT_VAL_REG_poll( b_ptr,
                                              h_ptr,
                                              A,
                                              PCBI_REG_INT_VAL_REG_BIT_VC_LPF_OUT1_V_MSK,
                                              (value<<PCBI_REG_INT_VAL_REG_BIT_VC_LPF_OUT1_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 s16_csu_pcbi_field_VC_LPF_OUT0_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_VC_LPF_OUT0_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_VC_LPF_OUT0_V_get", A, 15);
    /* (0x00000578 bits 10) field VC_LPF_OUT0_V of register PMC_PCBI_REG_INT_VAL_REG */
    reg_value = s16_csu_pcbi_reg_INT_VAL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PCBI_REG_INT_VAL_REG_BIT_VC_LPF_OUT0_V_MSK) >> PCBI_REG_INT_VAL_REG_BIT_VC_LPF_OUT0_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_VC_LPF_OUT0_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_VC_LPF_OUT0_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_VC_LPF_OUT0_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_VC_LPF_OUT0_V_poll", A, value );

    /* (0x00000578 bits 10) field VC_LPF_OUT0_V of register PMC_PCBI_REG_INT_VAL_REG */
    return s16_csu_pcbi_reg_INT_VAL_REG_poll( b_ptr,
                                              h_ptr,
                                              A,
                                              PCBI_REG_INT_VAL_REG_BIT_VC_LPF_OUT0_V_MSK,
                                              (value<<PCBI_REG_INT_VAL_REG_BIT_VC_LPF_OUT0_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                    s16_handle_t *h_ptr,
                                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                    s16_handle_t *h_ptr,
                                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_V_get", A, 15);
    /* (0x00000578 bits 9) field CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_V of register PMC_PCBI_REG_INT_VAL_REG */
    reg_value = s16_csu_pcbi_reg_INT_VAL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PCBI_REG_INT_VAL_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_V_MSK) >> PCBI_REG_INT_VAL_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                                   s16_handle_t *h_ptr,
                                                                                                   UINT32 A,
                                                                                                   UINT32 value,
                                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                                   UINT32 max_count,
                                                                                                   UINT32 *num_failed_polls,
                                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                                   s16_handle_t *h_ptr,
                                                                                                   UINT32 A,
                                                                                                   UINT32 value,
                                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                                   UINT32 max_count,
                                                                                                   UINT32 *num_failed_polls,
                                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_V_poll", A, value );

    /* (0x00000578 bits 9) field CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_V of register PMC_PCBI_REG_INT_VAL_REG */
    return s16_csu_pcbi_reg_INT_VAL_REG_poll( b_ptr,
                                              h_ptr,
                                              A,
                                              PCBI_REG_INT_VAL_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_V_MSK,
                                              (value<<PCBI_REG_INT_VAL_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_V_get", A, 15);
    /* (0x00000578 bits 8) field CSU0_EADSM_LOCKDET_LOCK_V of register PMC_PCBI_REG_INT_VAL_REG */
    reg_value = s16_csu_pcbi_reg_INT_VAL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PCBI_REG_INT_VAL_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_V_MSK) >> PCBI_REG_INT_VAL_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                      s16_handle_t *h_ptr,
                                                                                      UINT32 A,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                      s16_handle_t *h_ptr,
                                                                                      UINT32 A,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_V_poll", A, value );

    /* (0x00000578 bits 8) field CSU0_EADSM_LOCKDET_LOCK_V of register PMC_PCBI_REG_INT_VAL_REG */
    return s16_csu_pcbi_reg_INT_VAL_REG_poll( b_ptr,
                                              h_ptr,
                                              A,
                                              PCBI_REG_INT_VAL_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_V_MSK,
                                              (value<<PCBI_REG_INT_VAL_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_V_get", A, 15);
    /* (0x00000578 bits 7) field CSU0_PLL_LOCKDET_PPM_OFF_UPD_V of register PMC_PCBI_REG_INT_VAL_REG */
    reg_value = s16_csu_pcbi_reg_INT_VAL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PCBI_REG_INT_VAL_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_V_MSK) >> PCBI_REG_INT_VAL_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                           s16_handle_t *h_ptr,
                                                                                           UINT32 A,
                                                                                           UINT32 value,
                                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                                           UINT32 max_count,
                                                                                           UINT32 *num_failed_polls,
                                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                           s16_handle_t *h_ptr,
                                                                                           UINT32 A,
                                                                                           UINT32 value,
                                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                                           UINT32 max_count,
                                                                                           UINT32 *num_failed_polls,
                                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_V_poll", A, value );

    /* (0x00000578 bits 7) field CSU0_PLL_LOCKDET_PPM_OFF_UPD_V of register PMC_PCBI_REG_INT_VAL_REG */
    return s16_csu_pcbi_reg_INT_VAL_REG_poll( b_ptr,
                                              h_ptr,
                                              A,
                                              PCBI_REG_INT_VAL_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_V_MSK,
                                              (value<<PCBI_REG_INT_VAL_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_V_get", A, 15);
    /* (0x00000578 bits 6) field CSU0_VCOC_BIST_ERR_V of register PMC_PCBI_REG_INT_VAL_REG */
    reg_value = s16_csu_pcbi_reg_INT_VAL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PCBI_REG_INT_VAL_REG_BIT_CSU0_VCOC_BIST_ERR_V_MSK) >> PCBI_REG_INT_VAL_REG_BIT_CSU0_VCOC_BIST_ERR_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                 s16_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                 s16_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_V_poll", A, value );

    /* (0x00000578 bits 6) field CSU0_VCOC_BIST_ERR_V of register PMC_PCBI_REG_INT_VAL_REG */
    return s16_csu_pcbi_reg_INT_VAL_REG_poll( b_ptr,
                                              h_ptr,
                                              A,
                                              PCBI_REG_INT_VAL_REG_BIT_CSU0_VCOC_BIST_ERR_V_MSK,
                                              (value<<PCBI_REG_INT_VAL_REG_BIT_CSU0_VCOC_BIST_ERR_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_V_get", A, 15);
    /* (0x00000578 bits 5) field CSU0_DIVCLK_DEAD_V of register PMC_PCBI_REG_INT_VAL_REG */
    reg_value = s16_csu_pcbi_reg_INT_VAL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PCBI_REG_INT_VAL_REG_BIT_CSU0_DIVCLK_DEAD_V_MSK) >> PCBI_REG_INT_VAL_REG_BIT_CSU0_DIVCLK_DEAD_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                               s16_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                               s16_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_V_poll", A, value );

    /* (0x00000578 bits 5) field CSU0_DIVCLK_DEAD_V of register PMC_PCBI_REG_INT_VAL_REG */
    return s16_csu_pcbi_reg_INT_VAL_REG_poll( b_ptr,
                                              h_ptr,
                                              A,
                                              PCBI_REG_INT_VAL_REG_BIT_CSU0_DIVCLK_DEAD_V_MSK,
                                              (value<<PCBI_REG_INT_VAL_REG_BIT_CSU0_DIVCLK_DEAD_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_V_get", A, 15);
    /* (0x00000578 bits 4) field CSU0_IN_RANGE_N_FAILS_V of register PMC_PCBI_REG_INT_VAL_REG */
    reg_value = s16_csu_pcbi_reg_INT_VAL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PCBI_REG_INT_VAL_REG_BIT_CSU0_IN_RANGE_N_FAILS_V_MSK) >> PCBI_REG_INT_VAL_REG_BIT_CSU0_IN_RANGE_N_FAILS_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                    s16_handle_t *h_ptr,
                                                                                    UINT32 A,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                    s16_handle_t *h_ptr,
                                                                                    UINT32 A,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_V_poll", A, value );

    /* (0x00000578 bits 4) field CSU0_IN_RANGE_N_FAILS_V of register PMC_PCBI_REG_INT_VAL_REG */
    return s16_csu_pcbi_reg_INT_VAL_REG_poll( b_ptr,
                                              h_ptr,
                                              A,
                                              PCBI_REG_INT_VAL_REG_BIT_CSU0_IN_RANGE_N_FAILS_V_MSK,
                                              (value<<PCBI_REG_INT_VAL_REG_BIT_CSU0_IN_RANGE_N_FAILS_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_V_get", A, 15);
    /* (0x00000578 bits 3) field CSU0_IN_RANGE_N_PASSES_V of register PMC_PCBI_REG_INT_VAL_REG */
    reg_value = s16_csu_pcbi_reg_INT_VAL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PCBI_REG_INT_VAL_REG_BIT_CSU0_IN_RANGE_N_PASSES_V_MSK) >> PCBI_REG_INT_VAL_REG_BIT_CSU0_IN_RANGE_N_PASSES_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                     s16_handle_t *h_ptr,
                                                                                     UINT32 A,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                                     s16_handle_t *h_ptr,
                                                                                     UINT32 A,
                                                                                     UINT32 value,
                                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                                     UINT32 max_count,
                                                                                     UINT32 *num_failed_polls,
                                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_V_poll", A, value );

    /* (0x00000578 bits 3) field CSU0_IN_RANGE_N_PASSES_V of register PMC_PCBI_REG_INT_VAL_REG */
    return s16_csu_pcbi_reg_INT_VAL_REG_poll( b_ptr,
                                              h_ptr,
                                              A,
                                              PCBI_REG_INT_VAL_REG_BIT_CSU0_IN_RANGE_N_PASSES_V_MSK,
                                              (value<<PCBI_REG_INT_VAL_REG_BIT_CSU0_IN_RANGE_N_PASSES_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_LOCK_LOSS_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_LOCK_LOSS_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_LOCK_LOSS_V_get", A, 15);
    /* (0x00000578 bits 2) field CSU0_LOCK_LOSS_V of register PMC_PCBI_REG_INT_VAL_REG */
    reg_value = s16_csu_pcbi_reg_INT_VAL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PCBI_REG_INT_VAL_REG_BIT_CSU0_LOCK_LOSS_V_MSK) >> PCBI_REG_INT_VAL_REG_BIT_CSU0_LOCK_LOSS_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_LOSS_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_LOCK_LOSS_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_LOCK_LOSS_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_LOSS_V_poll", A, value );

    /* (0x00000578 bits 2) field CSU0_LOCK_LOSS_V of register PMC_PCBI_REG_INT_VAL_REG */
    return s16_csu_pcbi_reg_INT_VAL_REG_poll( b_ptr,
                                              h_ptr,
                                              A,
                                              PCBI_REG_INT_VAL_REG_BIT_CSU0_LOCK_LOSS_V_MSK,
                                              (value<<PCBI_REG_INT_VAL_REG_BIT_CSU0_LOCK_LOSS_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_LOCK_GAIN_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_LOCK_GAIN_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_LOCK_GAIN_V_get", A, 15);
    /* (0x00000578 bits 1) field CSU0_LOCK_GAIN_V of register PMC_PCBI_REG_INT_VAL_REG */
    reg_value = s16_csu_pcbi_reg_INT_VAL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PCBI_REG_INT_VAL_REG_BIT_CSU0_LOCK_GAIN_V_MSK) >> PCBI_REG_INT_VAL_REG_BIT_CSU0_LOCK_GAIN_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_GAIN_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_LOCK_GAIN_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_LOCK_GAIN_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_LOCK_GAIN_V_poll", A, value );

    /* (0x00000578 bits 1) field CSU0_LOCK_GAIN_V of register PMC_PCBI_REG_INT_VAL_REG */
    return s16_csu_pcbi_reg_INT_VAL_REG_poll( b_ptr,
                                              h_ptr,
                                              A,
                                              PCBI_REG_INT_VAL_REG_BIT_CSU0_LOCK_GAIN_V_MSK,
                                              (value<<PCBI_REG_INT_VAL_REG_BIT_CSU0_LOCK_GAIN_V_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_DONE_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_DONE_V_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_DONE_V_get", A, 15);
    /* (0x00000578 bits 0) field CSU0_VCOC_DONE_V of register PMC_PCBI_REG_INT_VAL_REG */
    reg_value = s16_csu_pcbi_reg_INT_VAL_REG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PCBI_REG_INT_VAL_REG_BIT_CSU0_VCOC_DONE_V_MSK) >> PCBI_REG_INT_VAL_REG_BIT_CSU0_VCOC_DONE_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_DONE_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_VCOC_DONE_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_VCOC_DONE_V_poll( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_DONE_V_poll", A, value );

    /* (0x00000578 bits 0) field CSU0_VCOC_DONE_V of register PMC_PCBI_REG_INT_VAL_REG */
    return s16_csu_pcbi_reg_INT_VAL_REG_poll( b_ptr,
                                              h_ptr,
                                              A,
                                              PCBI_REG_INT_VAL_REG_BIT_CSU0_VCOC_DONE_V_MSK,
                                              (value<<PCBI_REG_INT_VAL_REG_BIT_CSU0_VCOC_DONE_V_OFF),
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
static INLINE UINT32 s16_csu_pcbi_field_CSU0_PPM_OFF_RD_VAL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_PPM_OFF_RD_VAL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_PPM_OFF_RD_VAL_get", A, 15);
    /* (0x00000500 bits 20:0) bits 0:20 use field CSU0_PPM_OFF_RD_VAL of register PMC_PCBI_REG_CSU0_PLL_OOL_STAT_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_PLL_OOL_STAT_REG_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & PCBI_REG_CSU0_PLL_OOL_STAT_REG_BIT_CSU0_PPM_OFF_RD_VAL_MSK) >> PCBI_REG_CSU0_PLL_OOL_STAT_REG_BIT_CSU0_PPM_OFF_RD_VAL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_PPM_OFF_RD_VAL_get", A, value );

    return value;
}
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_PPM_OFF_RD_VAL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_PPM_OFF_RD_VAL_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_PPM_OFF_RD_VAL_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_PPM_OFF_RD_VAL_get", stop_bit, start_bit );
    if (stop_bit > 20) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_PPM_OFF_RD_VAL_get", stop_bit, 20 );
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
    /* (0x00000500 bits 20:0) bits 0:20 use field CSU0_PPM_OFF_RD_VAL of register PMC_PCBI_REG_CSU0_PLL_OOL_STAT_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_PLL_OOL_STAT_REG_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & PCBI_REG_CSU0_PLL_OOL_STAT_REG_BIT_CSU0_PPM_OFF_RD_VAL_MSK)
                  >> PCBI_REG_CSU0_PLL_OOL_STAT_REG_BIT_CSU0_PPM_OFF_RD_VAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_PLL_OOL_STAT_REG_BIT_CSU0_PPM_OFF_RD_VAL_MSK, PCBI_REG_CSU0_PLL_OOL_STAT_REG_BIT_CSU0_PPM_OFF_RD_VAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_PPM_OFF_RD_VAL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_csu_pcbi_field_VCOC_MODE_IN_RANGE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_VCOC_MODE_IN_RANGE_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_VCOC_MODE_IN_RANGE_get", A, 15);
    /* (0x00000508 bits 15) field VCOC_MODE_IN_RANGE of register PMC_PCBI_REG_CSU0_VCOC_STAT_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_STAT_REG_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & PCBI_REG_CSU0_VCOC_STAT_REG_BIT_VCOC_MODE_IN_RANGE_MSK) >> PCBI_REG_CSU0_VCOC_STAT_REG_BIT_VCOC_MODE_IN_RANGE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_VCOC_MODE_IN_RANGE_get", A, value );

    return value;
}
static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_BIST_MODE_COUNT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_BIST_MODE_COUNT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_BIST_MODE_COUNT_get", A, 15);
    /* (0x00000508 bits 14:7) bits 0:7 use field CSU0_VCOC_BIST_MODE_COUNT of register PMC_PCBI_REG_CSU0_VCOC_STAT_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_STAT_REG_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & PCBI_REG_CSU0_VCOC_STAT_REG_BIT_CSU0_VCOC_BIST_MODE_COUNT_MSK) >> PCBI_REG_CSU0_VCOC_STAT_REG_BIT_CSU0_VCOC_BIST_MODE_COUNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_BIST_MODE_COUNT_get", A, value );

    return value;
}
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_VCOC_BIST_MODE_COUNT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_VCOC_BIST_MODE_COUNT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_VCOC_BIST_MODE_COUNT_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_VCOC_BIST_MODE_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_VCOC_BIST_MODE_COUNT_get", stop_bit, 7 );
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
    /* (0x00000508 bits 14:7) bits 0:7 use field CSU0_VCOC_BIST_MODE_COUNT of register PMC_PCBI_REG_CSU0_VCOC_STAT_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_STAT_REG_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & PCBI_REG_CSU0_VCOC_STAT_REG_BIT_CSU0_VCOC_BIST_MODE_COUNT_MSK)
                  >> PCBI_REG_CSU0_VCOC_STAT_REG_BIT_CSU0_VCOC_BIST_MODE_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_VCOC_STAT_REG_BIT_CSU0_VCOC_BIST_MODE_COUNT_MSK, PCBI_REG_CSU0_VCOC_STAT_REG_BIT_CSU0_VCOC_BIST_MODE_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_VCOC_BIST_MODE_COUNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_MODE_OUT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_VCOC_MODE_OUT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_VCOC_MODE_OUT_get", A, 15);
    /* (0x00000508 bits 6:1) bits 0:5 use field CSU0_VCOC_MODE_OUT of register PMC_PCBI_REG_CSU0_VCOC_STAT_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_STAT_REG_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & PCBI_REG_CSU0_VCOC_STAT_REG_BIT_CSU0_VCOC_MODE_OUT_MSK) >> PCBI_REG_CSU0_VCOC_STAT_REG_BIT_CSU0_VCOC_MODE_OUT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_VCOC_MODE_OUT_get", A, value );

    return value;
}
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_VCOC_MODE_OUT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_VCOC_MODE_OUT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_VCOC_MODE_OUT_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_VCOC_MODE_OUT_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_VCOC_MODE_OUT_get", stop_bit, 5 );
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
    /* (0x00000508 bits 6:1) bits 0:5 use field CSU0_VCOC_MODE_OUT of register PMC_PCBI_REG_CSU0_VCOC_STAT_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_VCOC_STAT_REG_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & PCBI_REG_CSU0_VCOC_STAT_REG_BIT_CSU0_VCOC_MODE_OUT_MSK)
                  >> PCBI_REG_CSU0_VCOC_STAT_REG_BIT_CSU0_VCOC_MODE_OUT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_VCOC_STAT_REG_BIT_CSU0_VCOC_MODE_OUT_MSK, PCBI_REG_CSU0_VCOC_STAT_REG_BIT_CSU0_VCOC_MODE_OUT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_VCOC_MODE_OUT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_csu_pcbi_field_CSU0_EADSM_SYSCLK_CNT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_CSU0_EADSM_SYSCLK_CNT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_CSU0_EADSM_SYSCLK_CNT_get", A, 15);
    /* (0x0000050c bits 17:0) bits 0:17 use field CSU0_EADSM_SYSCLK_CNT of register PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_STAT_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_STAT_REG_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & PCBI_REG_CSU0_EADSM_LOCK_DET_STAT_REG_BIT_CSU0_EADSM_SYSCLK_CNT_MSK) >> PCBI_REG_CSU0_EADSM_LOCK_DET_STAT_REG_BIT_CSU0_EADSM_SYSCLK_CNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_CSU0_EADSM_SYSCLK_CNT_get", A, value );

    return value;
}
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_EADSM_SYSCLK_CNT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_CSU0_EADSM_SYSCLK_CNT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_CSU0_EADSM_SYSCLK_CNT_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_CSU0_EADSM_SYSCLK_CNT_get", stop_bit, start_bit );
    if (stop_bit > 17) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_CSU0_EADSM_SYSCLK_CNT_get", stop_bit, 17 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 17) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 17;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000050c bits 17:0) bits 0:17 use field CSU0_EADSM_SYSCLK_CNT of register PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_STAT_REG */
    reg_value = s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_STAT_REG_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    field_value = (reg_value & PCBI_REG_CSU0_EADSM_LOCK_DET_STAT_REG_BIT_CSU0_EADSM_SYSCLK_CNT_MSK)
                  >> PCBI_REG_CSU0_EADSM_LOCK_DET_STAT_REG_BIT_CSU0_EADSM_SYSCLK_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_CSU0_EADSM_LOCK_DET_STAT_REG_BIT_CSU0_EADSM_SYSCLK_CNT_MSK, PCBI_REG_CSU0_EADSM_LOCK_DET_STAT_REG_BIT_CSU0_EADSM_SYSCLK_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_CSU0_EADSM_SYSCLK_CNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_csu_pcbi_field_PDM_ACCUM_ATTEN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PDM_ACCUM_ATTEN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_ACCUM_ATTEN_get", A, 15);
    /* (0x00000590 bits 28:17) bits 0:11 use field PDM_ACCUM_ATTEN of register PMC_PCBI_REG_PMM_STATUS_REG1 */
    reg_value = s16_csu_pcbi_reg_PMM_STATUS_REG1_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & PCBI_REG_PMM_STATUS_REG1_BIT_PDM_ACCUM_ATTEN_MSK) >> PCBI_REG_PMM_STATUS_REG1_BIT_PDM_ACCUM_ATTEN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PDM_ACCUM_ATTEN_get", A, value );

    return value;
}
static INLINE UINT32 s16_csu_pcbi_field_range_PDM_ACCUM_ATTEN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_PDM_ACCUM_ATTEN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_ACCUM_ATTEN_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_ACCUM_ATTEN_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_ACCUM_ATTEN_get", stop_bit, 11 );
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
    /* (0x00000590 bits 28:17) bits 0:11 use field PDM_ACCUM_ATTEN of register PMC_PCBI_REG_PMM_STATUS_REG1 */
    reg_value = s16_csu_pcbi_reg_PMM_STATUS_REG1_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & PCBI_REG_PMM_STATUS_REG1_BIT_PDM_ACCUM_ATTEN_MSK)
                  >> PCBI_REG_PMM_STATUS_REG1_BIT_PDM_ACCUM_ATTEN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_PMM_STATUS_REG1_BIT_PDM_ACCUM_ATTEN_MSK, PCBI_REG_PMM_STATUS_REG1_BIT_PDM_ACCUM_ATTEN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_ACCUM_ATTEN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_csu_pcbi_field_PDM_ACCUM_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PDM_ACCUM_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_ACCUM_get", A, 15);
    /* (0x00000590 bits 11:0) bits 0:11 use field PDM_ACCUM of register PMC_PCBI_REG_PMM_STATUS_REG1 */
    reg_value = s16_csu_pcbi_reg_PMM_STATUS_REG1_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & PCBI_REG_PMM_STATUS_REG1_BIT_PDM_ACCUM_MSK) >> PCBI_REG_PMM_STATUS_REG1_BIT_PDM_ACCUM_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PDM_ACCUM_get", A, value );

    return value;
}
static INLINE UINT32 s16_csu_pcbi_field_range_PDM_ACCUM_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_PDM_ACCUM_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_ACCUM_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_ACCUM_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_ACCUM_get", stop_bit, 11 );
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
    /* (0x00000590 bits 11:0) bits 0:11 use field PDM_ACCUM of register PMC_PCBI_REG_PMM_STATUS_REG1 */
    reg_value = s16_csu_pcbi_reg_PMM_STATUS_REG1_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & PCBI_REG_PMM_STATUS_REG1_BIT_PDM_ACCUM_MSK)
                  >> PCBI_REG_PMM_STATUS_REG1_BIT_PDM_ACCUM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_PMM_STATUS_REG1_BIT_PDM_ACCUM_MSK, PCBI_REG_PMM_STATUS_REG1_BIT_PDM_ACCUM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_ACCUM_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_csu_pcbi_field_PDM_ALIGN_OUT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PDM_ALIGN_OUT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_ALIGN_OUT_get", A, 15);
    /* (0x00000594 bits 28:17) bits 0:11 use field PDM_ALIGN_OUT of register PMC_PCBI_REG_PMM_STATUS_REG2 */
    reg_value = s16_csu_pcbi_reg_PMM_STATUS_REG2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & PCBI_REG_PMM_STATUS_REG2_BIT_PDM_ALIGN_OUT_MSK) >> PCBI_REG_PMM_STATUS_REG2_BIT_PDM_ALIGN_OUT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PDM_ALIGN_OUT_get", A, value );

    return value;
}
static INLINE UINT32 s16_csu_pcbi_field_range_PDM_ALIGN_OUT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_PDM_ALIGN_OUT_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_ALIGN_OUT_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_ALIGN_OUT_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_ALIGN_OUT_get", stop_bit, 11 );
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
    /* (0x00000594 bits 28:17) bits 0:11 use field PDM_ALIGN_OUT of register PMC_PCBI_REG_PMM_STATUS_REG2 */
    reg_value = s16_csu_pcbi_reg_PMM_STATUS_REG2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & PCBI_REG_PMM_STATUS_REG2_BIT_PDM_ALIGN_OUT_MSK)
                  >> PCBI_REG_PMM_STATUS_REG2_BIT_PDM_ALIGN_OUT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_PMM_STATUS_REG2_BIT_PDM_ALIGN_OUT_MSK, PCBI_REG_PMM_STATUS_REG2_BIT_PDM_ALIGN_OUT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_ALIGN_OUT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_csu_pcbi_field_PDM_ACCUM_GAIN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_PDM_ACCUM_GAIN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_PDM_ACCUM_GAIN_get", A, 15);
    /* (0x00000594 bits 11:0) bits 0:11 use field PDM_ACCUM_GAIN of register PMC_PCBI_REG_PMM_STATUS_REG2 */
    reg_value = s16_csu_pcbi_reg_PMM_STATUS_REG2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & PCBI_REG_PMM_STATUS_REG2_BIT_PDM_ACCUM_GAIN_MSK) >> PCBI_REG_PMM_STATUS_REG2_BIT_PDM_ACCUM_GAIN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_csu_pcbi_field_PDM_ACCUM_GAIN_get", A, value );

    return value;
}
static INLINE UINT32 s16_csu_pcbi_field_range_PDM_ACCUM_GAIN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_csu_pcbi_field_range_PDM_ACCUM_GAIN_get( s16_csu_pcbi_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
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

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_csu_pcbi_field_range_PDM_ACCUM_GAIN_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_csu_pcbi_field_range_PDM_ACCUM_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_csu_pcbi_field_range_PDM_ACCUM_GAIN_get", stop_bit, 11 );
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
    /* (0x00000594 bits 11:0) bits 0:11 use field PDM_ACCUM_GAIN of register PMC_PCBI_REG_PMM_STATUS_REG2 */
    reg_value = s16_csu_pcbi_reg_PMM_STATUS_REG2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & PCBI_REG_PMM_STATUS_REG2_BIT_PDM_ACCUM_GAIN_MSK)
                  >> PCBI_REG_PMM_STATUS_REG2_BIT_PDM_ACCUM_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCBI_REG_PMM_STATUS_REG2_BIT_PDM_ACCUM_GAIN_MSK, PCBI_REG_PMM_STATUS_REG2_BIT_PDM_ACCUM_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_csu_pcbi_field_range_PDM_ACCUM_GAIN_get", A, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _S16_CSU_PCBI_IO_INLINE_H */
