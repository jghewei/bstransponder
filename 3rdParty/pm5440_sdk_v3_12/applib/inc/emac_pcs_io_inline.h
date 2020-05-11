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
 *     and register accessor functions for the emac_pcs block
 *****************************************************************************/
#ifndef _EMAC_PCS_IO_INLINE_H
#define _EMAC_PCS_IO_INLINE_H

#include "emac_loc.h"
#include "emac_pcs_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define EMAC_PCS_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for emac_pcs
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
    emac_handle_t * h_ptr;
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} emac_pcs_buffer_t;
static INLINE void emac_pcs_buffer_init( emac_pcs_buffer_t *b_ptr,
                                         emac_handle_t *h_ptr,
                                         UINT32 A ) ALWAYS_INLINE;
static INLINE void emac_pcs_buffer_init( emac_pcs_buffer_t *b_ptr,
                                         emac_handle_t *h_ptr,
                                         UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*1024) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "emac_pcs_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void emac_pcs_buffer_flush( emac_pcs_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void emac_pcs_buffer_flush( emac_pcs_buffer_t *b_ptr )
{
    IOLOG( "emac_pcs_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 emac_pcs_reg_read( emac_pcs_buffer_t *b_ptr,
                                        emac_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 mem_type,
                                        UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_read( emac_pcs_buffer_t *b_ptr,
                                        emac_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 mem_type,
                                        UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->A)*1024),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (A)*1024) + reg);
    return value;
}
static INLINE void emac_pcs_reg_write( emac_pcs_buffer_t *b_ptr,
                                       emac_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_write( emac_pcs_buffer_t *b_ptr,
                                       emac_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->A)*1024),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*1024) + reg,
                         value);
}

static INLINE void emac_pcs_field_set( emac_pcs_buffer_t *b_ptr,
                                       emac_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 mask,
                                       UINT32 unused_mask,
                                       UINT32 ofs,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_set( emac_pcs_buffer_t *b_ptr,
                                       emac_handle_t *h_ptr,
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
                          (b_ptr->base_address + (b_ptr->A)*1024),
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
                             (((pmc_handle_t *)h_ptr)->base_address + (A)*1024) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (A)*1024) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void emac_pcs_action_on_write_field_set( emac_pcs_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_action_on_write_field_set( emac_pcs_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
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
                                         (b_ptr->base_address + (b_ptr->A)*1024),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*1024) + reg,
                         value<<ofs);
}

static INLINE void emac_pcs_burst_read( emac_pcs_buffer_t *b_ptr,
                                        emac_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void emac_pcs_burst_read( emac_pcs_buffer_t *b_ptr,
                                        emac_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_read( b_ptr->coalesce_handle,
                          mem_type,
                          (b_ptr->base_address + (b_ptr->A)*1024),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (A)*1024) + reg,
                          len,
                          value);
}

static INLINE void emac_pcs_burst_write( emac_pcs_buffer_t *b_ptr,
                                         emac_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void emac_pcs_burst_write( emac_pcs_buffer_t *b_ptr,
                                         emac_handle_t *h_ptr,
                                         UINT32 A,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_write( b_ptr->coalesce_handle,
                           mem_type,
                           (b_ptr->base_address + (b_ptr->A)*1024),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*1024) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE emac_pcs_poll( emac_pcs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mem_type,
                                                  UINT32 reg,
                                                  UINT32 mask,
                                                  UINT32 value,
                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                  UINT32 max_count,
                                                  UINT32 *num_failed_polls,
                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE emac_pcs_poll( emac_pcs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
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
                           (b_ptr->base_address + (b_ptr->A)*1024),
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
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*1024) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for emac_pcs
 * ==================================================================================
 */

static INLINE void emac_pcs_reg_CTRL_AND_STAT_write( emac_pcs_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_CTRL_AND_STAT_write( emac_pcs_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_pcs_reg_CTRL_AND_STAT_write", value );
    emac_pcs_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_CTRL_AND_STAT,
                        value);
}

static INLINE void emac_pcs_reg_CTRL_AND_STAT_field_set( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_CTRL_AND_STAT_field_set( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_pcs_reg_CTRL_AND_STAT_field_set", A, mask, ofs, value );
    emac_pcs_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_CTRL_AND_STAT,
                        mask,
                        PMC_PCS_REG_CTRL_AND_STAT_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_pcs_reg_CTRL_AND_STAT_read( emac_pcs_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_CTRL_AND_STAT_read( emac_pcs_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_PCS_REG_CTRL_AND_STAT);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_CTRL_AND_STAT_read", reg_value, A);
    return reg_value;
}

static INLINE void emac_pcs_reg_BASE_R_SEED_A_1_4__write( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_BASE_R_SEED_A_1_4__write( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_pcs_reg_BASE_R_SEED_A_1_4__write", value );
    emac_pcs_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_BASE_R_SEED_A_1_4_,
                        value);
}

static INLINE void emac_pcs_reg_BASE_R_SEED_A_1_4__field_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_BASE_R_SEED_A_1_4__field_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_pcs_reg_BASE_R_SEED_A_1_4__field_set", A, mask, ofs, value );
    emac_pcs_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_BASE_R_SEED_A_1_4_,
                        mask,
                        PMC_PCS_REG_BASE_R_SEED_A_1_4_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_pcs_reg_BASE_R_SEED_A_1_4__read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_BASE_R_SEED_A_1_4__read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_PCS_REG_BASE_R_SEED_A_1_4_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_BASE_R_SEED_A_1_4__read", reg_value, A);
    return reg_value;
}

static INLINE void emac_pcs_reg_BASE_R_SEED_A_2_4__write( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_BASE_R_SEED_A_2_4__write( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_pcs_reg_BASE_R_SEED_A_2_4__write", value );
    emac_pcs_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_BASE_R_SEED_A_2_4_,
                        value);
}

static INLINE void emac_pcs_reg_BASE_R_SEED_A_2_4__field_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_BASE_R_SEED_A_2_4__field_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_pcs_reg_BASE_R_SEED_A_2_4__field_set", A, mask, ofs, value );
    emac_pcs_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_BASE_R_SEED_A_2_4_,
                        mask,
                        PMC_PCS_REG_BASE_R_SEED_A_2_4_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_pcs_reg_BASE_R_SEED_A_2_4__read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_BASE_R_SEED_A_2_4__read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_PCS_REG_BASE_R_SEED_A_2_4_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_BASE_R_SEED_A_2_4__read", reg_value, A);
    return reg_value;
}

static INLINE void emac_pcs_reg_BASE_R_SEED_A_3_4__write( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_BASE_R_SEED_A_3_4__write( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_pcs_reg_BASE_R_SEED_A_3_4__write", value );
    emac_pcs_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_BASE_R_SEED_A_3_4_,
                        value);
}

static INLINE void emac_pcs_reg_BASE_R_SEED_A_3_4__field_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_BASE_R_SEED_A_3_4__field_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_pcs_reg_BASE_R_SEED_A_3_4__field_set", A, mask, ofs, value );
    emac_pcs_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_BASE_R_SEED_A_3_4_,
                        mask,
                        PMC_PCS_REG_BASE_R_SEED_A_3_4_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_pcs_reg_BASE_R_SEED_A_3_4__read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_BASE_R_SEED_A_3_4__read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_PCS_REG_BASE_R_SEED_A_3_4_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_BASE_R_SEED_A_3_4__read", reg_value, A);
    return reg_value;
}

static INLINE void emac_pcs_reg_BASE_R_SEED_A_4_4__write( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_BASE_R_SEED_A_4_4__write( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_pcs_reg_BASE_R_SEED_A_4_4__write", value );
    emac_pcs_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_BASE_R_SEED_A_4_4_,
                        value);
}

static INLINE void emac_pcs_reg_BASE_R_SEED_A_4_4__field_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_BASE_R_SEED_A_4_4__field_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_pcs_reg_BASE_R_SEED_A_4_4__field_set", A, mask, ofs, value );
    emac_pcs_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_BASE_R_SEED_A_4_4_,
                        mask,
                        PMC_PCS_REG_BASE_R_SEED_A_4_4_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_pcs_reg_BASE_R_SEED_A_4_4__read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_BASE_R_SEED_A_4_4__read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_PCS_REG_BASE_R_SEED_A_4_4_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_BASE_R_SEED_A_4_4__read", reg_value, A);
    return reg_value;
}

static INLINE void emac_pcs_reg_BASE_R_SEED_B_1_4__write( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_BASE_R_SEED_B_1_4__write( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_pcs_reg_BASE_R_SEED_B_1_4__write", value );
    emac_pcs_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_BASE_R_SEED_B_1_4_,
                        value);
}

static INLINE void emac_pcs_reg_BASE_R_SEED_B_1_4__field_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_BASE_R_SEED_B_1_4__field_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_pcs_reg_BASE_R_SEED_B_1_4__field_set", A, mask, ofs, value );
    emac_pcs_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_BASE_R_SEED_B_1_4_,
                        mask,
                        PMC_PCS_REG_BASE_R_SEED_B_1_4_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_pcs_reg_BASE_R_SEED_B_1_4__read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_BASE_R_SEED_B_1_4__read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_PCS_REG_BASE_R_SEED_B_1_4_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_BASE_R_SEED_B_1_4__read", reg_value, A);
    return reg_value;
}

static INLINE void emac_pcs_reg_BASE_R_SEED_B_2_4__write( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_BASE_R_SEED_B_2_4__write( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_pcs_reg_BASE_R_SEED_B_2_4__write", value );
    emac_pcs_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_BASE_R_SEED_B_2_4_,
                        value);
}

static INLINE void emac_pcs_reg_BASE_R_SEED_B_2_4__field_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_BASE_R_SEED_B_2_4__field_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_pcs_reg_BASE_R_SEED_B_2_4__field_set", A, mask, ofs, value );
    emac_pcs_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_BASE_R_SEED_B_2_4_,
                        mask,
                        PMC_PCS_REG_BASE_R_SEED_B_2_4_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_pcs_reg_BASE_R_SEED_B_2_4__read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_BASE_R_SEED_B_2_4__read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_PCS_REG_BASE_R_SEED_B_2_4_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_BASE_R_SEED_B_2_4__read", reg_value, A);
    return reg_value;
}

static INLINE void emac_pcs_reg_BASE_R_SEED_B_3_4__write( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_BASE_R_SEED_B_3_4__write( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_pcs_reg_BASE_R_SEED_B_3_4__write", value );
    emac_pcs_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_BASE_R_SEED_B_3_4_,
                        value);
}

static INLINE void emac_pcs_reg_BASE_R_SEED_B_3_4__field_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_BASE_R_SEED_B_3_4__field_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_pcs_reg_BASE_R_SEED_B_3_4__field_set", A, mask, ofs, value );
    emac_pcs_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_BASE_R_SEED_B_3_4_,
                        mask,
                        PMC_PCS_REG_BASE_R_SEED_B_3_4_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_pcs_reg_BASE_R_SEED_B_3_4__read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_BASE_R_SEED_B_3_4__read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_PCS_REG_BASE_R_SEED_B_3_4_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_BASE_R_SEED_B_3_4__read", reg_value, A);
    return reg_value;
}

static INLINE void emac_pcs_reg_BASE_R_SEED_B_4_4__write( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_BASE_R_SEED_B_4_4__write( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_pcs_reg_BASE_R_SEED_B_4_4__write", value );
    emac_pcs_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_BASE_R_SEED_B_4_4_,
                        value);
}

static INLINE void emac_pcs_reg_BASE_R_SEED_B_4_4__field_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_BASE_R_SEED_B_4_4__field_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_pcs_reg_BASE_R_SEED_B_4_4__field_set", A, mask, ofs, value );
    emac_pcs_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_BASE_R_SEED_B_4_4_,
                        mask,
                        PMC_PCS_REG_BASE_R_SEED_B_4_4_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_pcs_reg_BASE_R_SEED_B_4_4__read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_BASE_R_SEED_B_4_4__read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_PCS_REG_BASE_R_SEED_B_4_4_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_BASE_R_SEED_B_4_4__read", reg_value, A);
    return reg_value;
}

static INLINE void emac_pcs_reg_BASE_R_TEST_CTRL_write( emac_pcs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_BASE_R_TEST_CTRL_write( emac_pcs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "emac_pcs_reg_BASE_R_TEST_CTRL_write", value );
    emac_pcs_reg_write( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_BASE_R_TEST_CTRL,
                        value);
}

static INLINE void emac_pcs_reg_BASE_R_TEST_CTRL_field_set( emac_pcs_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_reg_BASE_R_TEST_CTRL_field_set( emac_pcs_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "emac_pcs_reg_BASE_R_TEST_CTRL_field_set", A, mask, ofs, value );
    emac_pcs_field_set( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_CONFIG,
                        PMC_PCS_REG_BASE_R_TEST_CTRL,
                        mask,
                        PMC_PCS_REG_BASE_R_TEST_CTRL_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 emac_pcs_reg_BASE_R_TEST_CTRL_read( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_BASE_R_TEST_CTRL_read( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_CONFIG,
                                   PMC_PCS_REG_BASE_R_TEST_CTRL);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_BASE_R_TEST_CTRL_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_STAT_1_2__read( emac_pcs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_STAT_1_2__read( emac_pcs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_STAT_1_2_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_STAT_1_2__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_DEVICE_ID_1_2__read( emac_pcs_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_DEVICE_ID_1_2__read( emac_pcs_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_DEVICE_ID_1_2_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_DEVICE_ID_1_2__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_DEVICE_ID_2_2__read( emac_pcs_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_DEVICE_ID_2_2__read( emac_pcs_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_DEVICE_ID_2_2_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_DEVICE_ID_2_2__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_CTRL_2_2__read( emac_pcs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_CTRL_2_2__read( emac_pcs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_CTRL_2_2_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_CTRL_2_2__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_STAT_2_2__read( emac_pcs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_STAT_2_2__read( emac_pcs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_STAT_2_2_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_STAT_2_2__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_PACKAGE_ID_1_2__read( emac_pcs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_PACKAGE_ID_1_2__read( emac_pcs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_PACKAGE_ID_1_2_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_PACKAGE_ID_1_2__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_PACKAGE_ID_2_2__read( emac_pcs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_PACKAGE_ID_2_2__read( emac_pcs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_PACKAGE_ID_2_2_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_PACKAGE_ID_2_2__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_BASE_R_STAT_1_2__read( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_BASE_R_STAT_1_2__read( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_BASE_R_STAT_1_2_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_BASE_R_STAT_1_2__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_BASE_R_STAT_2_2__read( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_BASE_R_STAT_2_2__read( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_BASE_R_STAT_2_2_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_BASE_R_STAT_2_2__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_BASE_R_TEST_ERR_CNT_read( emac_pcs_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_BASE_R_TEST_ERR_CNT_read( emac_pcs_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_BASE_R_TEST_ERR_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_BASE_R_TEST_ERR_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_BER_CNT_MSB__read( emac_pcs_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_BER_CNT_MSB__read( emac_pcs_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_BER_CNT_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_BER_CNT_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_BLOCK_ERR_CNT_MSB__read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_BLOCK_ERR_CNT_MSB__read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_BLOCK_ERR_CNT_MSB_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_BLOCK_ERR_CNT_MSB__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_PMON_BLOCK_ERR_CNT_1_3__read( emac_pcs_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_PMON_BLOCK_ERR_CNT_1_3__read( emac_pcs_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_PMON_BLOCK_ERR_CNT_1_3_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_PMON_BLOCK_ERR_CNT_1_3__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_PMON_BLOCK_ERR_CNT_2_3__read( emac_pcs_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_PMON_BLOCK_ERR_CNT_2_3__read( emac_pcs_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_PMON_BLOCK_ERR_CNT_2_3_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_PMON_BLOCK_ERR_CNT_2_3__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_PMON_BLOCK_ERR_CNT_3_3__read( emac_pcs_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_PMON_BLOCK_ERR_CNT_3_3__read( emac_pcs_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_PMON_BLOCK_ERR_CNT_3_3_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_PMON_BLOCK_ERR_CNT_3_3__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_1_4__read( emac_pcs_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_1_4__read( emac_pcs_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_1_4_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_1_4__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_2_4__read( emac_pcs_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_2_4__read( emac_pcs_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_2_4_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_2_4__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_3_4__read( emac_pcs_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_3_4__read( emac_pcs_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_3_4_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_3_4__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_4_4__read( emac_pcs_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_4_4__read( emac_pcs_buffer_t *b_ptr,
                                                                       emac_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_4_4_);

    IOLOG( "%s -> 0x%08x; A=%d", "emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_4_4__read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_LANE_BIP_ERR_CNT_array_read( emac_pcs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_LANE_BIP_ERR_CNT_array_read( emac_pcs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_LANE_BIP_ERR_CNT(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "emac_pcs_reg_LANE_BIP_ERR_CNT_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE UINT32 emac_pcs_reg_LANE_MAPPING_array_read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_reg_LANE_MAPPING_array_read( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = emac_pcs_reg_read( b_ptr,
                                   h_ptr,
                                   A,
                                   MEM_TYPE_STATUS,
                                   PMC_PCS_REG_LANE_MAPPING(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "emac_pcs_reg_LANE_MAPPING_array_read", reg_value, A, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void emac_pcs_field_RESET_set( emac_pcs_buffer_t *b_ptr,
                                             emac_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_RESET_set( emac_pcs_buffer_t *b_ptr,
                                             emac_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_RESET_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_pcs_field_RESET_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_pcs_field_RESET_set", A, value );

    /* (0x0000c000 bits 15) field RESET of register PMC_PCS_REG_CTRL_AND_STAT */
    emac_pcs_reg_CTRL_AND_STAT_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          PCS_REG_CTRL_AND_STAT_BIT_RESET_MSK,
                                          PCS_REG_CTRL_AND_STAT_BIT_RESET_OFF,
                                          value);
}

static INLINE UINT32 emac_pcs_field_RESET_get( emac_pcs_buffer_t *b_ptr,
                                               emac_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_RESET_get( emac_pcs_buffer_t *b_ptr,
                                               emac_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_RESET_get", A, 11);
    /* (0x0000c000 bits 15) field RESET of register PMC_PCS_REG_CTRL_AND_STAT */
    reg_value = emac_pcs_reg_CTRL_AND_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCS_REG_CTRL_AND_STAT_BIT_RESET_MSK) >> PCS_REG_CTRL_AND_STAT_BIT_RESET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_RESET_get", A, value );

    return value;
}
static INLINE void emac_pcs_field_LOOPBACK_set( emac_pcs_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_LOOPBACK_set( emac_pcs_buffer_t *b_ptr,
                                                emac_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_LOOPBACK_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_pcs_field_LOOPBACK_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_pcs_field_LOOPBACK_set", A, value );

    /* (0x0000c000 bits 14) field LOOPBACK of register PMC_PCS_REG_CTRL_AND_STAT */
    emac_pcs_reg_CTRL_AND_STAT_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          PCS_REG_CTRL_AND_STAT_BIT_LOOPBACK_MSK,
                                          PCS_REG_CTRL_AND_STAT_BIT_LOOPBACK_OFF,
                                          value);
}

static INLINE UINT32 emac_pcs_field_LOOPBACK_get( emac_pcs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_LOOPBACK_get( emac_pcs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_LOOPBACK_get", A, 11);
    /* (0x0000c000 bits 14) field LOOPBACK of register PMC_PCS_REG_CTRL_AND_STAT */
    reg_value = emac_pcs_reg_CTRL_AND_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCS_REG_CTRL_AND_STAT_BIT_LOOPBACK_MSK) >> PCS_REG_CTRL_AND_STAT_BIT_LOOPBACK_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_LOOPBACK_get", A, value );

    return value;
}
static INLINE void emac_pcs_field_TEST_SEED_A_15_0_set( emac_pcs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_TEST_SEED_A_15_0_set( emac_pcs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TEST_SEED_A_15_0_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_pcs_field_TEST_SEED_A_15_0_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_pcs_field_TEST_SEED_A_15_0_set", A, value );

    /* (0x0000c088 bits 15:0) bits 0:15 use field TEST_SEED_A_15_0 of register PMC_PCS_REG_BASE_R_SEED_A_1_4_ */
    emac_pcs_reg_BASE_R_SEED_A_1_4__field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               PCS_REG_BASE_R_SEED_A_1_4_BIT_TEST_SEED_A_15_0_MSK,
                                               PCS_REG_BASE_R_SEED_A_1_4_BIT_TEST_SEED_A_15_0_OFF,
                                               value);
}

static INLINE UINT32 emac_pcs_field_TEST_SEED_A_15_0_get( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_TEST_SEED_A_15_0_get( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TEST_SEED_A_15_0_get", A, 11);
    /* (0x0000c088 bits 15:0) bits 0:15 use field TEST_SEED_A_15_0 of register PMC_PCS_REG_BASE_R_SEED_A_1_4_ */
    reg_value = emac_pcs_reg_BASE_R_SEED_A_1_4__read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & PCS_REG_BASE_R_SEED_A_1_4_BIT_TEST_SEED_A_15_0_MSK) >> PCS_REG_BASE_R_SEED_A_1_4_BIT_TEST_SEED_A_15_0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_TEST_SEED_A_15_0_get", A, value );

    return value;
}
static INLINE void emac_pcs_field_range_TEST_SEED_A_15_0_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_range_TEST_SEED_A_15_0_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_TEST_SEED_A_15_0_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_TEST_SEED_A_15_0_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_TEST_SEED_A_15_0_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_TEST_SEED_A_15_0_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000c088 bits 15:0) bits 0:15 use field TEST_SEED_A_15_0 of register PMC_PCS_REG_BASE_R_SEED_A_1_4_ */
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
        /* (0x0000c088 bits 15:0) bits 0:15 use field TEST_SEED_A_15_0 of register PMC_PCS_REG_BASE_R_SEED_A_1_4_ */
        emac_pcs_reg_BASE_R_SEED_A_1_4__field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (PCS_REG_BASE_R_SEED_A_1_4_BIT_TEST_SEED_A_15_0_OFF + subfield_offset),
                                                   PCS_REG_BASE_R_SEED_A_1_4_BIT_TEST_SEED_A_15_0_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 emac_pcs_field_range_TEST_SEED_A_15_0_get( emac_pcs_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_TEST_SEED_A_15_0_get( emac_pcs_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_TEST_SEED_A_15_0_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_TEST_SEED_A_15_0_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_TEST_SEED_A_15_0_get", stop_bit, 15 );
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
    /* (0x0000c088 bits 15:0) bits 0:15 use field TEST_SEED_A_15_0 of register PMC_PCS_REG_BASE_R_SEED_A_1_4_ */
    reg_value = emac_pcs_reg_BASE_R_SEED_A_1_4__read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & PCS_REG_BASE_R_SEED_A_1_4_BIT_TEST_SEED_A_15_0_MSK)
                  >> PCS_REG_BASE_R_SEED_A_1_4_BIT_TEST_SEED_A_15_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_BASE_R_SEED_A_1_4_BIT_TEST_SEED_A_15_0_MSK, PCS_REG_BASE_R_SEED_A_1_4_BIT_TEST_SEED_A_15_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_TEST_SEED_A_15_0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_pcs_field_TEST_SEED_A_31_16_set( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_TEST_SEED_A_31_16_set( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TEST_SEED_A_31_16_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_pcs_field_TEST_SEED_A_31_16_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_pcs_field_TEST_SEED_A_31_16_set", A, value );

    /* (0x0000c08c bits 15:0) bits 0:15 use field TEST_SEED_A_31_16 of register PMC_PCS_REG_BASE_R_SEED_A_2_4_ */
    emac_pcs_reg_BASE_R_SEED_A_2_4__field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               PCS_REG_BASE_R_SEED_A_2_4_BIT_TEST_SEED_A_31_16_MSK,
                                               PCS_REG_BASE_R_SEED_A_2_4_BIT_TEST_SEED_A_31_16_OFF,
                                               value);
}

static INLINE UINT32 emac_pcs_field_TEST_SEED_A_31_16_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_TEST_SEED_A_31_16_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TEST_SEED_A_31_16_get", A, 11);
    /* (0x0000c08c bits 15:0) bits 0:15 use field TEST_SEED_A_31_16 of register PMC_PCS_REG_BASE_R_SEED_A_2_4_ */
    reg_value = emac_pcs_reg_BASE_R_SEED_A_2_4__read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & PCS_REG_BASE_R_SEED_A_2_4_BIT_TEST_SEED_A_31_16_MSK) >> PCS_REG_BASE_R_SEED_A_2_4_BIT_TEST_SEED_A_31_16_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_TEST_SEED_A_31_16_get", A, value );

    return value;
}
static INLINE void emac_pcs_field_range_TEST_SEED_A_31_16_set( emac_pcs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_range_TEST_SEED_A_31_16_set( emac_pcs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_TEST_SEED_A_31_16_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_TEST_SEED_A_31_16_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_TEST_SEED_A_31_16_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_TEST_SEED_A_31_16_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000c08c bits 15:0) bits 0:15 use field TEST_SEED_A_31_16 of register PMC_PCS_REG_BASE_R_SEED_A_2_4_ */
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
        /* (0x0000c08c bits 15:0) bits 0:15 use field TEST_SEED_A_31_16 of register PMC_PCS_REG_BASE_R_SEED_A_2_4_ */
        emac_pcs_reg_BASE_R_SEED_A_2_4__field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (PCS_REG_BASE_R_SEED_A_2_4_BIT_TEST_SEED_A_31_16_OFF + subfield_offset),
                                                   PCS_REG_BASE_R_SEED_A_2_4_BIT_TEST_SEED_A_31_16_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 emac_pcs_field_range_TEST_SEED_A_31_16_get( emac_pcs_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_TEST_SEED_A_31_16_get( emac_pcs_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_TEST_SEED_A_31_16_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_TEST_SEED_A_31_16_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_TEST_SEED_A_31_16_get", stop_bit, 15 );
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
    /* (0x0000c08c bits 15:0) bits 0:15 use field TEST_SEED_A_31_16 of register PMC_PCS_REG_BASE_R_SEED_A_2_4_ */
    reg_value = emac_pcs_reg_BASE_R_SEED_A_2_4__read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & PCS_REG_BASE_R_SEED_A_2_4_BIT_TEST_SEED_A_31_16_MSK)
                  >> PCS_REG_BASE_R_SEED_A_2_4_BIT_TEST_SEED_A_31_16_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_BASE_R_SEED_A_2_4_BIT_TEST_SEED_A_31_16_MSK, PCS_REG_BASE_R_SEED_A_2_4_BIT_TEST_SEED_A_31_16_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_TEST_SEED_A_31_16_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_pcs_field_TEST_SEED_A_47_32_set( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_TEST_SEED_A_47_32_set( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TEST_SEED_A_47_32_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_pcs_field_TEST_SEED_A_47_32_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_pcs_field_TEST_SEED_A_47_32_set", A, value );

    /* (0x0000c090 bits 15:0) bits 0:15 use field TEST_SEED_A_47_32 of register PMC_PCS_REG_BASE_R_SEED_A_3_4_ */
    emac_pcs_reg_BASE_R_SEED_A_3_4__field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               PCS_REG_BASE_R_SEED_A_3_4_BIT_TEST_SEED_A_47_32_MSK,
                                               PCS_REG_BASE_R_SEED_A_3_4_BIT_TEST_SEED_A_47_32_OFF,
                                               value);
}

static INLINE UINT32 emac_pcs_field_TEST_SEED_A_47_32_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_TEST_SEED_A_47_32_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TEST_SEED_A_47_32_get", A, 11);
    /* (0x0000c090 bits 15:0) bits 0:15 use field TEST_SEED_A_47_32 of register PMC_PCS_REG_BASE_R_SEED_A_3_4_ */
    reg_value = emac_pcs_reg_BASE_R_SEED_A_3_4__read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & PCS_REG_BASE_R_SEED_A_3_4_BIT_TEST_SEED_A_47_32_MSK) >> PCS_REG_BASE_R_SEED_A_3_4_BIT_TEST_SEED_A_47_32_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_TEST_SEED_A_47_32_get", A, value );

    return value;
}
static INLINE void emac_pcs_field_range_TEST_SEED_A_47_32_set( emac_pcs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_range_TEST_SEED_A_47_32_set( emac_pcs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_TEST_SEED_A_47_32_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_TEST_SEED_A_47_32_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_TEST_SEED_A_47_32_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_TEST_SEED_A_47_32_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000c090 bits 15:0) bits 0:15 use field TEST_SEED_A_47_32 of register PMC_PCS_REG_BASE_R_SEED_A_3_4_ */
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
        /* (0x0000c090 bits 15:0) bits 0:15 use field TEST_SEED_A_47_32 of register PMC_PCS_REG_BASE_R_SEED_A_3_4_ */
        emac_pcs_reg_BASE_R_SEED_A_3_4__field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (PCS_REG_BASE_R_SEED_A_3_4_BIT_TEST_SEED_A_47_32_OFF + subfield_offset),
                                                   PCS_REG_BASE_R_SEED_A_3_4_BIT_TEST_SEED_A_47_32_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 emac_pcs_field_range_TEST_SEED_A_47_32_get( emac_pcs_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_TEST_SEED_A_47_32_get( emac_pcs_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_TEST_SEED_A_47_32_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_TEST_SEED_A_47_32_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_TEST_SEED_A_47_32_get", stop_bit, 15 );
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
    /* (0x0000c090 bits 15:0) bits 0:15 use field TEST_SEED_A_47_32 of register PMC_PCS_REG_BASE_R_SEED_A_3_4_ */
    reg_value = emac_pcs_reg_BASE_R_SEED_A_3_4__read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & PCS_REG_BASE_R_SEED_A_3_4_BIT_TEST_SEED_A_47_32_MSK)
                  >> PCS_REG_BASE_R_SEED_A_3_4_BIT_TEST_SEED_A_47_32_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_BASE_R_SEED_A_3_4_BIT_TEST_SEED_A_47_32_MSK, PCS_REG_BASE_R_SEED_A_3_4_BIT_TEST_SEED_A_47_32_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_TEST_SEED_A_47_32_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_pcs_field_TEST_SEED_A_57_48_set( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_TEST_SEED_A_57_48_set( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TEST_SEED_A_57_48_set", A, 11);
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_pcs_field_TEST_SEED_A_57_48_set", value, 1023);
    IOLOG( "%s <= A=%d 0x%08x", "emac_pcs_field_TEST_SEED_A_57_48_set", A, value );

    /* (0x0000c094 bits 9:0) bits 0:9 use field TEST_SEED_A_57_48 of register PMC_PCS_REG_BASE_R_SEED_A_4_4_ */
    emac_pcs_reg_BASE_R_SEED_A_4_4__field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               PCS_REG_BASE_R_SEED_A_4_4_BIT_TEST_SEED_A_57_48_MSK,
                                               PCS_REG_BASE_R_SEED_A_4_4_BIT_TEST_SEED_A_57_48_OFF,
                                               value);
}

static INLINE UINT32 emac_pcs_field_TEST_SEED_A_57_48_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_TEST_SEED_A_57_48_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TEST_SEED_A_57_48_get", A, 11);
    /* (0x0000c094 bits 9:0) bits 0:9 use field TEST_SEED_A_57_48 of register PMC_PCS_REG_BASE_R_SEED_A_4_4_ */
    reg_value = emac_pcs_reg_BASE_R_SEED_A_4_4__read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & PCS_REG_BASE_R_SEED_A_4_4_BIT_TEST_SEED_A_57_48_MSK) >> PCS_REG_BASE_R_SEED_A_4_4_BIT_TEST_SEED_A_57_48_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_TEST_SEED_A_57_48_get", A, value );

    return value;
}
static INLINE void emac_pcs_field_range_TEST_SEED_A_57_48_set( emac_pcs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_range_TEST_SEED_A_57_48_set( emac_pcs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_TEST_SEED_A_57_48_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_TEST_SEED_A_57_48_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_TEST_SEED_A_57_48_set", stop_bit, 9 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_TEST_SEED_A_57_48_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000c094 bits 9:0) bits 0:9 use field TEST_SEED_A_57_48 of register PMC_PCS_REG_BASE_R_SEED_A_4_4_ */
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
        /* (0x0000c094 bits 9:0) bits 0:9 use field TEST_SEED_A_57_48 of register PMC_PCS_REG_BASE_R_SEED_A_4_4_ */
        emac_pcs_reg_BASE_R_SEED_A_4_4__field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (PCS_REG_BASE_R_SEED_A_4_4_BIT_TEST_SEED_A_57_48_OFF + subfield_offset),
                                                   PCS_REG_BASE_R_SEED_A_4_4_BIT_TEST_SEED_A_57_48_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 emac_pcs_field_range_TEST_SEED_A_57_48_get( emac_pcs_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_TEST_SEED_A_57_48_get( emac_pcs_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_TEST_SEED_A_57_48_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_TEST_SEED_A_57_48_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_TEST_SEED_A_57_48_get", stop_bit, 9 );
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
    /* (0x0000c094 bits 9:0) bits 0:9 use field TEST_SEED_A_57_48 of register PMC_PCS_REG_BASE_R_SEED_A_4_4_ */
    reg_value = emac_pcs_reg_BASE_R_SEED_A_4_4__read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & PCS_REG_BASE_R_SEED_A_4_4_BIT_TEST_SEED_A_57_48_MSK)
                  >> PCS_REG_BASE_R_SEED_A_4_4_BIT_TEST_SEED_A_57_48_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_BASE_R_SEED_A_4_4_BIT_TEST_SEED_A_57_48_MSK, PCS_REG_BASE_R_SEED_A_4_4_BIT_TEST_SEED_A_57_48_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_TEST_SEED_A_57_48_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_pcs_field_TEST_SEED_B_15_0_set( emac_pcs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_TEST_SEED_B_15_0_set( emac_pcs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TEST_SEED_B_15_0_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_pcs_field_TEST_SEED_B_15_0_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_pcs_field_TEST_SEED_B_15_0_set", A, value );

    /* (0x0000c098 bits 15:0) bits 0:15 use field TEST_SEED_B_15_0 of register PMC_PCS_REG_BASE_R_SEED_B_1_4_ */
    emac_pcs_reg_BASE_R_SEED_B_1_4__field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               PCS_REG_BASE_R_SEED_B_1_4_BIT_TEST_SEED_B_15_0_MSK,
                                               PCS_REG_BASE_R_SEED_B_1_4_BIT_TEST_SEED_B_15_0_OFF,
                                               value);
}

static INLINE UINT32 emac_pcs_field_TEST_SEED_B_15_0_get( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_TEST_SEED_B_15_0_get( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TEST_SEED_B_15_0_get", A, 11);
    /* (0x0000c098 bits 15:0) bits 0:15 use field TEST_SEED_B_15_0 of register PMC_PCS_REG_BASE_R_SEED_B_1_4_ */
    reg_value = emac_pcs_reg_BASE_R_SEED_B_1_4__read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & PCS_REG_BASE_R_SEED_B_1_4_BIT_TEST_SEED_B_15_0_MSK) >> PCS_REG_BASE_R_SEED_B_1_4_BIT_TEST_SEED_B_15_0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_TEST_SEED_B_15_0_get", A, value );

    return value;
}
static INLINE void emac_pcs_field_range_TEST_SEED_B_15_0_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_range_TEST_SEED_B_15_0_set( emac_pcs_buffer_t *b_ptr,
                                                              emac_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_TEST_SEED_B_15_0_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_TEST_SEED_B_15_0_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_TEST_SEED_B_15_0_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_TEST_SEED_B_15_0_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000c098 bits 15:0) bits 0:15 use field TEST_SEED_B_15_0 of register PMC_PCS_REG_BASE_R_SEED_B_1_4_ */
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
        /* (0x0000c098 bits 15:0) bits 0:15 use field TEST_SEED_B_15_0 of register PMC_PCS_REG_BASE_R_SEED_B_1_4_ */
        emac_pcs_reg_BASE_R_SEED_B_1_4__field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (PCS_REG_BASE_R_SEED_B_1_4_BIT_TEST_SEED_B_15_0_OFF + subfield_offset),
                                                   PCS_REG_BASE_R_SEED_B_1_4_BIT_TEST_SEED_B_15_0_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 emac_pcs_field_range_TEST_SEED_B_15_0_get( emac_pcs_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_TEST_SEED_B_15_0_get( emac_pcs_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_TEST_SEED_B_15_0_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_TEST_SEED_B_15_0_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_TEST_SEED_B_15_0_get", stop_bit, 15 );
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
    /* (0x0000c098 bits 15:0) bits 0:15 use field TEST_SEED_B_15_0 of register PMC_PCS_REG_BASE_R_SEED_B_1_4_ */
    reg_value = emac_pcs_reg_BASE_R_SEED_B_1_4__read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & PCS_REG_BASE_R_SEED_B_1_4_BIT_TEST_SEED_B_15_0_MSK)
                  >> PCS_REG_BASE_R_SEED_B_1_4_BIT_TEST_SEED_B_15_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_BASE_R_SEED_B_1_4_BIT_TEST_SEED_B_15_0_MSK, PCS_REG_BASE_R_SEED_B_1_4_BIT_TEST_SEED_B_15_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_TEST_SEED_B_15_0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_pcs_field_TEST_SEED_B_31_16_set( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_TEST_SEED_B_31_16_set( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TEST_SEED_B_31_16_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_pcs_field_TEST_SEED_B_31_16_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_pcs_field_TEST_SEED_B_31_16_set", A, value );

    /* (0x0000c09c bits 15:0) bits 0:15 use field TEST_SEED_B_31_16 of register PMC_PCS_REG_BASE_R_SEED_B_2_4_ */
    emac_pcs_reg_BASE_R_SEED_B_2_4__field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               PCS_REG_BASE_R_SEED_B_2_4_BIT_TEST_SEED_B_31_16_MSK,
                                               PCS_REG_BASE_R_SEED_B_2_4_BIT_TEST_SEED_B_31_16_OFF,
                                               value);
}

static INLINE UINT32 emac_pcs_field_TEST_SEED_B_31_16_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_TEST_SEED_B_31_16_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TEST_SEED_B_31_16_get", A, 11);
    /* (0x0000c09c bits 15:0) bits 0:15 use field TEST_SEED_B_31_16 of register PMC_PCS_REG_BASE_R_SEED_B_2_4_ */
    reg_value = emac_pcs_reg_BASE_R_SEED_B_2_4__read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & PCS_REG_BASE_R_SEED_B_2_4_BIT_TEST_SEED_B_31_16_MSK) >> PCS_REG_BASE_R_SEED_B_2_4_BIT_TEST_SEED_B_31_16_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_TEST_SEED_B_31_16_get", A, value );

    return value;
}
static INLINE void emac_pcs_field_range_TEST_SEED_B_31_16_set( emac_pcs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_range_TEST_SEED_B_31_16_set( emac_pcs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_TEST_SEED_B_31_16_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_TEST_SEED_B_31_16_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_TEST_SEED_B_31_16_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_TEST_SEED_B_31_16_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000c09c bits 15:0) bits 0:15 use field TEST_SEED_B_31_16 of register PMC_PCS_REG_BASE_R_SEED_B_2_4_ */
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
        /* (0x0000c09c bits 15:0) bits 0:15 use field TEST_SEED_B_31_16 of register PMC_PCS_REG_BASE_R_SEED_B_2_4_ */
        emac_pcs_reg_BASE_R_SEED_B_2_4__field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (PCS_REG_BASE_R_SEED_B_2_4_BIT_TEST_SEED_B_31_16_OFF + subfield_offset),
                                                   PCS_REG_BASE_R_SEED_B_2_4_BIT_TEST_SEED_B_31_16_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 emac_pcs_field_range_TEST_SEED_B_31_16_get( emac_pcs_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_TEST_SEED_B_31_16_get( emac_pcs_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_TEST_SEED_B_31_16_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_TEST_SEED_B_31_16_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_TEST_SEED_B_31_16_get", stop_bit, 15 );
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
    /* (0x0000c09c bits 15:0) bits 0:15 use field TEST_SEED_B_31_16 of register PMC_PCS_REG_BASE_R_SEED_B_2_4_ */
    reg_value = emac_pcs_reg_BASE_R_SEED_B_2_4__read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & PCS_REG_BASE_R_SEED_B_2_4_BIT_TEST_SEED_B_31_16_MSK)
                  >> PCS_REG_BASE_R_SEED_B_2_4_BIT_TEST_SEED_B_31_16_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_BASE_R_SEED_B_2_4_BIT_TEST_SEED_B_31_16_MSK, PCS_REG_BASE_R_SEED_B_2_4_BIT_TEST_SEED_B_31_16_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_TEST_SEED_B_31_16_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_pcs_field_TEST_SEED_B_47_32_set( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_TEST_SEED_B_47_32_set( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TEST_SEED_B_47_32_set", A, 11);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_pcs_field_TEST_SEED_B_47_32_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "emac_pcs_field_TEST_SEED_B_47_32_set", A, value );

    /* (0x0000c0a0 bits 15:0) bits 0:15 use field TEST_SEED_B_47_32 of register PMC_PCS_REG_BASE_R_SEED_B_3_4_ */
    emac_pcs_reg_BASE_R_SEED_B_3_4__field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               PCS_REG_BASE_R_SEED_B_3_4_BIT_TEST_SEED_B_47_32_MSK,
                                               PCS_REG_BASE_R_SEED_B_3_4_BIT_TEST_SEED_B_47_32_OFF,
                                               value);
}

static INLINE UINT32 emac_pcs_field_TEST_SEED_B_47_32_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_TEST_SEED_B_47_32_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TEST_SEED_B_47_32_get", A, 11);
    /* (0x0000c0a0 bits 15:0) bits 0:15 use field TEST_SEED_B_47_32 of register PMC_PCS_REG_BASE_R_SEED_B_3_4_ */
    reg_value = emac_pcs_reg_BASE_R_SEED_B_3_4__read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & PCS_REG_BASE_R_SEED_B_3_4_BIT_TEST_SEED_B_47_32_MSK) >> PCS_REG_BASE_R_SEED_B_3_4_BIT_TEST_SEED_B_47_32_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_TEST_SEED_B_47_32_get", A, value );

    return value;
}
static INLINE void emac_pcs_field_range_TEST_SEED_B_47_32_set( emac_pcs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_range_TEST_SEED_B_47_32_set( emac_pcs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_TEST_SEED_B_47_32_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_TEST_SEED_B_47_32_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_TEST_SEED_B_47_32_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_TEST_SEED_B_47_32_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000c0a0 bits 15:0) bits 0:15 use field TEST_SEED_B_47_32 of register PMC_PCS_REG_BASE_R_SEED_B_3_4_ */
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
        /* (0x0000c0a0 bits 15:0) bits 0:15 use field TEST_SEED_B_47_32 of register PMC_PCS_REG_BASE_R_SEED_B_3_4_ */
        emac_pcs_reg_BASE_R_SEED_B_3_4__field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (PCS_REG_BASE_R_SEED_B_3_4_BIT_TEST_SEED_B_47_32_OFF + subfield_offset),
                                                   PCS_REG_BASE_R_SEED_B_3_4_BIT_TEST_SEED_B_47_32_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 emac_pcs_field_range_TEST_SEED_B_47_32_get( emac_pcs_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_TEST_SEED_B_47_32_get( emac_pcs_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_TEST_SEED_B_47_32_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_TEST_SEED_B_47_32_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_TEST_SEED_B_47_32_get", stop_bit, 15 );
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
    /* (0x0000c0a0 bits 15:0) bits 0:15 use field TEST_SEED_B_47_32 of register PMC_PCS_REG_BASE_R_SEED_B_3_4_ */
    reg_value = emac_pcs_reg_BASE_R_SEED_B_3_4__read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & PCS_REG_BASE_R_SEED_B_3_4_BIT_TEST_SEED_B_47_32_MSK)
                  >> PCS_REG_BASE_R_SEED_B_3_4_BIT_TEST_SEED_B_47_32_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_BASE_R_SEED_B_3_4_BIT_TEST_SEED_B_47_32_MSK, PCS_REG_BASE_R_SEED_B_3_4_BIT_TEST_SEED_B_47_32_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_TEST_SEED_B_47_32_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_pcs_field_TEST_SEED_B_57_48_set( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_TEST_SEED_B_57_48_set( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TEST_SEED_B_57_48_set", A, 11);
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_pcs_field_TEST_SEED_B_57_48_set", value, 1023);
    IOLOG( "%s <= A=%d 0x%08x", "emac_pcs_field_TEST_SEED_B_57_48_set", A, value );

    /* (0x0000c0a4 bits 9:0) bits 0:9 use field TEST_SEED_B_57_48 of register PMC_PCS_REG_BASE_R_SEED_B_4_4_ */
    emac_pcs_reg_BASE_R_SEED_B_4_4__field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               PCS_REG_BASE_R_SEED_B_4_4_BIT_TEST_SEED_B_57_48_MSK,
                                               PCS_REG_BASE_R_SEED_B_4_4_BIT_TEST_SEED_B_57_48_OFF,
                                               value);
}

static INLINE UINT32 emac_pcs_field_TEST_SEED_B_57_48_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_TEST_SEED_B_57_48_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TEST_SEED_B_57_48_get", A, 11);
    /* (0x0000c0a4 bits 9:0) bits 0:9 use field TEST_SEED_B_57_48 of register PMC_PCS_REG_BASE_R_SEED_B_4_4_ */
    reg_value = emac_pcs_reg_BASE_R_SEED_B_4_4__read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & PCS_REG_BASE_R_SEED_B_4_4_BIT_TEST_SEED_B_57_48_MSK) >> PCS_REG_BASE_R_SEED_B_4_4_BIT_TEST_SEED_B_57_48_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_TEST_SEED_B_57_48_get", A, value );

    return value;
}
static INLINE void emac_pcs_field_range_TEST_SEED_B_57_48_set( emac_pcs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_range_TEST_SEED_B_57_48_set( emac_pcs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_TEST_SEED_B_57_48_set", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_TEST_SEED_B_57_48_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_TEST_SEED_B_57_48_set", stop_bit, 9 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_TEST_SEED_B_57_48_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000c0a4 bits 9:0) bits 0:9 use field TEST_SEED_B_57_48 of register PMC_PCS_REG_BASE_R_SEED_B_4_4_ */
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
        /* (0x0000c0a4 bits 9:0) bits 0:9 use field TEST_SEED_B_57_48 of register PMC_PCS_REG_BASE_R_SEED_B_4_4_ */
        emac_pcs_reg_BASE_R_SEED_B_4_4__field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (PCS_REG_BASE_R_SEED_B_4_4_BIT_TEST_SEED_B_57_48_OFF + subfield_offset),
                                                   PCS_REG_BASE_R_SEED_B_4_4_BIT_TEST_SEED_B_57_48_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 emac_pcs_field_range_TEST_SEED_B_57_48_get( emac_pcs_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_TEST_SEED_B_57_48_get( emac_pcs_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_TEST_SEED_B_57_48_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_TEST_SEED_B_57_48_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_TEST_SEED_B_57_48_get", stop_bit, 9 );
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
    /* (0x0000c0a4 bits 9:0) bits 0:9 use field TEST_SEED_B_57_48 of register PMC_PCS_REG_BASE_R_SEED_B_4_4_ */
    reg_value = emac_pcs_reg_BASE_R_SEED_B_4_4__read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & PCS_REG_BASE_R_SEED_B_4_4_BIT_TEST_SEED_B_57_48_MSK)
                  >> PCS_REG_BASE_R_SEED_B_4_4_BIT_TEST_SEED_B_57_48_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_BASE_R_SEED_B_4_4_BIT_TEST_SEED_B_57_48_MSK, PCS_REG_BASE_R_SEED_B_4_4_BIT_TEST_SEED_B_57_48_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_TEST_SEED_B_57_48_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void emac_pcs_field_TX_TEST_EN_set( emac_pcs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_TX_TEST_EN_set( emac_pcs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TX_TEST_EN_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_pcs_field_TX_TEST_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_pcs_field_TX_TEST_EN_set", A, value );

    /* (0x0000c0a8 bits 3) field TX_TEST_EN of register PMC_PCS_REG_BASE_R_TEST_CTRL */
    emac_pcs_reg_BASE_R_TEST_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             PCS_REG_BASE_R_TEST_CTRL_BIT_TX_TEST_EN_MSK,
                                             PCS_REG_BASE_R_TEST_CTRL_BIT_TX_TEST_EN_OFF,
                                             value);
}

static INLINE UINT32 emac_pcs_field_TX_TEST_EN_get( emac_pcs_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_TX_TEST_EN_get( emac_pcs_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TX_TEST_EN_get", A, 11);
    /* (0x0000c0a8 bits 3) field TX_TEST_EN of register PMC_PCS_REG_BASE_R_TEST_CTRL */
    reg_value = emac_pcs_reg_BASE_R_TEST_CTRL_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & PCS_REG_BASE_R_TEST_CTRL_BIT_TX_TEST_EN_MSK) >> PCS_REG_BASE_R_TEST_CTRL_BIT_TX_TEST_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_TX_TEST_EN_get", A, value );

    return value;
}
static INLINE void emac_pcs_field_RX_TEST_EN_set( emac_pcs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_RX_TEST_EN_set( emac_pcs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_RX_TEST_EN_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_pcs_field_RX_TEST_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_pcs_field_RX_TEST_EN_set", A, value );

    /* (0x0000c0a8 bits 2) field RX_TEST_EN of register PMC_PCS_REG_BASE_R_TEST_CTRL */
    emac_pcs_reg_BASE_R_TEST_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             PCS_REG_BASE_R_TEST_CTRL_BIT_RX_TEST_EN_MSK,
                                             PCS_REG_BASE_R_TEST_CTRL_BIT_RX_TEST_EN_OFF,
                                             value);
}

static INLINE UINT32 emac_pcs_field_RX_TEST_EN_get( emac_pcs_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_RX_TEST_EN_get( emac_pcs_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_RX_TEST_EN_get", A, 11);
    /* (0x0000c0a8 bits 2) field RX_TEST_EN of register PMC_PCS_REG_BASE_R_TEST_CTRL */
    reg_value = emac_pcs_reg_BASE_R_TEST_CTRL_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & PCS_REG_BASE_R_TEST_CTRL_BIT_RX_TEST_EN_MSK) >> PCS_REG_BASE_R_TEST_CTRL_BIT_RX_TEST_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_RX_TEST_EN_get", A, value );

    return value;
}
static INLINE void emac_pcs_field_TEST_PATTERN_set( emac_pcs_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_TEST_PATTERN_set( emac_pcs_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TEST_PATTERN_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_pcs_field_TEST_PATTERN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_pcs_field_TEST_PATTERN_set", A, value );

    /* (0x0000c0a8 bits 1) field TEST_PATTERN of register PMC_PCS_REG_BASE_R_TEST_CTRL */
    emac_pcs_reg_BASE_R_TEST_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             PCS_REG_BASE_R_TEST_CTRL_BIT_TEST_PATTERN_MSK,
                                             PCS_REG_BASE_R_TEST_CTRL_BIT_TEST_PATTERN_OFF,
                                             value);
}

static INLINE UINT32 emac_pcs_field_TEST_PATTERN_get( emac_pcs_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_TEST_PATTERN_get( emac_pcs_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TEST_PATTERN_get", A, 11);
    /* (0x0000c0a8 bits 1) field TEST_PATTERN of register PMC_PCS_REG_BASE_R_TEST_CTRL */
    reg_value = emac_pcs_reg_BASE_R_TEST_CTRL_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & PCS_REG_BASE_R_TEST_CTRL_BIT_TEST_PATTERN_MSK) >> PCS_REG_BASE_R_TEST_CTRL_BIT_TEST_PATTERN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_TEST_PATTERN_get", A, value );

    return value;
}
static INLINE void emac_pcs_field_DATA_PATTERN_set( emac_pcs_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void emac_pcs_field_DATA_PATTERN_set( emac_pcs_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_DATA_PATTERN_set", A, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "emac_pcs_field_DATA_PATTERN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "emac_pcs_field_DATA_PATTERN_set", A, value );

    /* (0x0000c0a8 bits 0) field DATA_PATTERN of register PMC_PCS_REG_BASE_R_TEST_CTRL */
    emac_pcs_reg_BASE_R_TEST_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             PCS_REG_BASE_R_TEST_CTRL_BIT_DATA_PATTERN_MSK,
                                             PCS_REG_BASE_R_TEST_CTRL_BIT_DATA_PATTERN_OFF,
                                             value);
}

static INLINE UINT32 emac_pcs_field_DATA_PATTERN_get( emac_pcs_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_DATA_PATTERN_get( emac_pcs_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_DATA_PATTERN_get", A, 11);
    /* (0x0000c0a8 bits 0) field DATA_PATTERN of register PMC_PCS_REG_BASE_R_TEST_CTRL */
    reg_value = emac_pcs_reg_BASE_R_TEST_CTRL_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & PCS_REG_BASE_R_TEST_CTRL_BIT_DATA_PATTERN_MSK) >> PCS_REG_BASE_R_TEST_CTRL_BIT_DATA_PATTERN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_DATA_PATTERN_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 emac_pcs_field_SPEED_SEL_get( emac_pcs_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_SPEED_SEL_get( emac_pcs_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_SPEED_SEL_get", A, 11);
    /* (0x0000c000 bits 5:2) bits 0:3 use field SPEED_SEL of register PMC_PCS_REG_CTRL_AND_STAT */
    reg_value = emac_pcs_reg_CTRL_AND_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & PCS_REG_CTRL_AND_STAT_BIT_SPEED_SEL_MSK) >> PCS_REG_CTRL_AND_STAT_BIT_SPEED_SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_SPEED_SEL_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_SPEED_SEL_get( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_SPEED_SEL_get( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_SPEED_SEL_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_SPEED_SEL_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_SPEED_SEL_get", stop_bit, 3 );
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
    /* (0x0000c000 bits 5:2) bits 0:3 use field SPEED_SEL of register PMC_PCS_REG_CTRL_AND_STAT */
    reg_value = emac_pcs_reg_CTRL_AND_STAT_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & PCS_REG_CTRL_AND_STAT_BIT_SPEED_SEL_MSK)
                  >> PCS_REG_CTRL_AND_STAT_BIT_SPEED_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_CTRL_AND_STAT_BIT_SPEED_SEL_MSK, PCS_REG_CTRL_AND_STAT_BIT_SPEED_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_SPEED_SEL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_LINK_STATUS_L_get( emac_pcs_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_LINK_STATUS_L_get( emac_pcs_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_LINK_STATUS_L_get", A, 11);
    /* (0x0000c004 bits 2) field LINK_STATUS_L of register PMC_PCS_REG_STAT_1_2_ */
    reg_value = emac_pcs_reg_STAT_1_2__read(  b_ptr, h_ptr, A);
    value = (reg_value & PCS_REG_STAT_1_2_BIT_LINK_STATUS_L_MSK) >> PCS_REG_STAT_1_2_BIT_LINK_STATUS_L_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_LINK_STATUS_L_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_DEVID_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_DEVID_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_DEVID_LSB_get", A, 11);
    /* (0x0000c008 bits 15:0) bits 0:15 use field DEVID_LSB of register PMC_PCS_REG_DEVICE_ID_1_2_ */
    reg_value = emac_pcs_reg_DEVICE_ID_1_2__read(  b_ptr, h_ptr, A);
    value = (reg_value & PCS_REG_DEVICE_ID_1_2_BIT_DEVID_LSB_MSK) >> PCS_REG_DEVICE_ID_1_2_BIT_DEVID_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_DEVID_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_DEVID_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_DEVID_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_DEVID_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_DEVID_LSB_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_DEVID_LSB_get", stop_bit, 15 );
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
    /* (0x0000c008 bits 15:0) bits 0:15 use field DEVID_LSB of register PMC_PCS_REG_DEVICE_ID_1_2_ */
    reg_value = emac_pcs_reg_DEVICE_ID_1_2__read(  b_ptr, h_ptr, A);
    field_value = (reg_value & PCS_REG_DEVICE_ID_1_2_BIT_DEVID_LSB_MSK)
                  >> PCS_REG_DEVICE_ID_1_2_BIT_DEVID_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_DEVICE_ID_1_2_BIT_DEVID_LSB_MSK, PCS_REG_DEVICE_ID_1_2_BIT_DEVID_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_DEVID_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_DEVID_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_DEVID_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_DEVID_MSB_get", A, 11);
    /* (0x0000c00c bits 15:0) bits 0:15 use field DEVID_MSB of register PMC_PCS_REG_DEVICE_ID_2_2_ */
    reg_value = emac_pcs_reg_DEVICE_ID_2_2__read(  b_ptr, h_ptr, A);
    value = (reg_value & PCS_REG_DEVICE_ID_2_2_BIT_DEVID_MSB_MSK) >> PCS_REG_DEVICE_ID_2_2_BIT_DEVID_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_DEVID_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_DEVID_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_DEVID_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_DEVID_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_DEVID_MSB_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_DEVID_MSB_get", stop_bit, 15 );
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
    /* (0x0000c00c bits 15:0) bits 0:15 use field DEVID_MSB of register PMC_PCS_REG_DEVICE_ID_2_2_ */
    reg_value = emac_pcs_reg_DEVICE_ID_2_2__read(  b_ptr, h_ptr, A);
    field_value = (reg_value & PCS_REG_DEVICE_ID_2_2_BIT_DEVID_MSB_MSK)
                  >> PCS_REG_DEVICE_ID_2_2_BIT_DEVID_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_DEVICE_ID_2_2_BIT_DEVID_MSB_MSK, PCS_REG_DEVICE_ID_2_2_BIT_DEVID_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_DEVID_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_PCS_SEL_get( emac_pcs_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_PCS_SEL_get( emac_pcs_buffer_t *b_ptr,
                                                 emac_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_PCS_SEL_get", A, 11);
    /* (0x0000c01c bits 2:0) bits 0:2 use field PCS_SEL of register PMC_PCS_REG_CTRL_2_2_ */
    reg_value = emac_pcs_reg_CTRL_2_2__read(  b_ptr, h_ptr, A);
    value = (reg_value & PCS_REG_CTRL_2_2_BIT_PCS_SEL_MSK) >> PCS_REG_CTRL_2_2_BIT_PCS_SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_PCS_SEL_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_PCS_SEL_get( emac_pcs_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_PCS_SEL_get( emac_pcs_buffer_t *b_ptr,
                                                       emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_PCS_SEL_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_PCS_SEL_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_PCS_SEL_get", stop_bit, 2 );
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
    /* (0x0000c01c bits 2:0) bits 0:2 use field PCS_SEL of register PMC_PCS_REG_CTRL_2_2_ */
    reg_value = emac_pcs_reg_CTRL_2_2__read(  b_ptr, h_ptr, A);
    field_value = (reg_value & PCS_REG_CTRL_2_2_BIT_PCS_SEL_MSK)
                  >> PCS_REG_CTRL_2_2_BIT_PCS_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_CTRL_2_2_BIT_PCS_SEL_MSK, PCS_REG_CTRL_2_2_BIT_PCS_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_PCS_SEL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_PCS_CFG_MODE_get( emac_pcs_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_PCS_CFG_MODE_get( emac_pcs_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_PCS_CFG_MODE_get", A, 11);
    /* (0x0000c020 bits 5:0) bits 0:5 use field PCS_CFG_MODE of register PMC_PCS_REG_STAT_2_2_ */
    reg_value = emac_pcs_reg_STAT_2_2__read(  b_ptr, h_ptr, A);
    value = (reg_value & PCS_REG_STAT_2_2_BIT_PCS_CFG_MODE_MSK) >> PCS_REG_STAT_2_2_BIT_PCS_CFG_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_PCS_CFG_MODE_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_PCS_CFG_MODE_get( emac_pcs_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_PCS_CFG_MODE_get( emac_pcs_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_PCS_CFG_MODE_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_PCS_CFG_MODE_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_PCS_CFG_MODE_get", stop_bit, 5 );
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
    /* (0x0000c020 bits 5:0) bits 0:5 use field PCS_CFG_MODE of register PMC_PCS_REG_STAT_2_2_ */
    reg_value = emac_pcs_reg_STAT_2_2__read(  b_ptr, h_ptr, A);
    field_value = (reg_value & PCS_REG_STAT_2_2_BIT_PCS_CFG_MODE_MSK)
                  >> PCS_REG_STAT_2_2_BIT_PCS_CFG_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_STAT_2_2_BIT_PCS_CFG_MODE_MSK, PCS_REG_STAT_2_2_BIT_PCS_CFG_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_PCS_CFG_MODE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_PKGID_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_PKGID_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_PKGID_LSB_get", A, 11);
    /* (0x0000c038 bits 15:0) bits 0:15 use field PKGID_LSB of register PMC_PCS_REG_PACKAGE_ID_1_2_ */
    reg_value = emac_pcs_reg_PACKAGE_ID_1_2__read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PCS_REG_PACKAGE_ID_1_2_BIT_PKGID_LSB_MSK) >> PCS_REG_PACKAGE_ID_1_2_BIT_PKGID_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_PKGID_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_PKGID_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_PKGID_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_PKGID_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_PKGID_LSB_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_PKGID_LSB_get", stop_bit, 15 );
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
    /* (0x0000c038 bits 15:0) bits 0:15 use field PKGID_LSB of register PMC_PCS_REG_PACKAGE_ID_1_2_ */
    reg_value = emac_pcs_reg_PACKAGE_ID_1_2__read( b_ptr,
                                                   h_ptr,
                                                   A);
    field_value = (reg_value & PCS_REG_PACKAGE_ID_1_2_BIT_PKGID_LSB_MSK)
                  >> PCS_REG_PACKAGE_ID_1_2_BIT_PKGID_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_PACKAGE_ID_1_2_BIT_PKGID_LSB_MSK, PCS_REG_PACKAGE_ID_1_2_BIT_PKGID_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_PKGID_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_PKGID_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_PKGID_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                   emac_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_PKGID_MSB_get", A, 11);
    /* (0x0000c03c bits 15:0) bits 0:15 use field PKGID_MSB of register PMC_PCS_REG_PACKAGE_ID_2_2_ */
    reg_value = emac_pcs_reg_PACKAGE_ID_2_2__read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & PCS_REG_PACKAGE_ID_2_2_BIT_PKGID_MSB_MSK) >> PCS_REG_PACKAGE_ID_2_2_BIT_PKGID_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_PKGID_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_PKGID_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_PKGID_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                         emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_PKGID_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_PKGID_MSB_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_PKGID_MSB_get", stop_bit, 15 );
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
    /* (0x0000c03c bits 15:0) bits 0:15 use field PKGID_MSB of register PMC_PCS_REG_PACKAGE_ID_2_2_ */
    reg_value = emac_pcs_reg_PACKAGE_ID_2_2__read( b_ptr,
                                                   h_ptr,
                                                   A);
    field_value = (reg_value & PCS_REG_PACKAGE_ID_2_2_BIT_PKGID_MSB_MSK)
                  >> PCS_REG_PACKAGE_ID_2_2_BIT_PKGID_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_PACKAGE_ID_2_2_BIT_PKGID_MSB_MSK, PCS_REG_PACKAGE_ID_2_2_BIT_PKGID_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_PKGID_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_LINK_STATUS_get( emac_pcs_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_LINK_STATUS_get( emac_pcs_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_LINK_STATUS_get", A, 11);
    /* (0x0000c080 bits 12) field LINK_STATUS of register PMC_PCS_REG_BASE_R_STAT_1_2_ */
    reg_value = emac_pcs_reg_BASE_R_STAT_1_2__read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & PCS_REG_BASE_R_STAT_1_2_BIT_LINK_STATUS_MSK) >> PCS_REG_BASE_R_STAT_1_2_BIT_LINK_STATUS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_LINK_STATUS_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_HIGH_BER_get( emac_pcs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_HIGH_BER_get( emac_pcs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_HIGH_BER_get", A, 11);
    /* (0x0000c080 bits 1) field HIGH_BER of register PMC_PCS_REG_BASE_R_STAT_1_2_ */
    reg_value = emac_pcs_reg_BASE_R_STAT_1_2__read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & PCS_REG_BASE_R_STAT_1_2_BIT_HIGH_BER_MSK) >> PCS_REG_BASE_R_STAT_1_2_BIT_HIGH_BER_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_HIGH_BER_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_BLOCK_LOCK_get( emac_pcs_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_BLOCK_LOCK_get( emac_pcs_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_BLOCK_LOCK_get", A, 11);
    /* (0x0000c080 bits 0) field BLOCK_LOCK of register PMC_PCS_REG_BASE_R_STAT_1_2_ */
    reg_value = emac_pcs_reg_BASE_R_STAT_1_2__read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & PCS_REG_BASE_R_STAT_1_2_BIT_BLOCK_LOCK_MSK) >> PCS_REG_BASE_R_STAT_1_2_BIT_BLOCK_LOCK_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_BLOCK_LOCK_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_BLOCK_LOCK_L_get( emac_pcs_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_BLOCK_LOCK_L_get( emac_pcs_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_BLOCK_LOCK_L_get", A, 11);
    /* (0x0000c084 bits 15) field BLOCK_LOCK_L of register PMC_PCS_REG_BASE_R_STAT_2_2_ */
    reg_value = emac_pcs_reg_BASE_R_STAT_2_2__read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & PCS_REG_BASE_R_STAT_2_2_BIT_BLOCK_LOCK_L_MSK) >> PCS_REG_BASE_R_STAT_2_2_BIT_BLOCK_LOCK_L_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_BLOCK_LOCK_L_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_HIGH_BER_L_get( emac_pcs_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_HIGH_BER_L_get( emac_pcs_buffer_t *b_ptr,
                                                    emac_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_HIGH_BER_L_get", A, 11);
    /* (0x0000c084 bits 14) field HIGH_BER_L of register PMC_PCS_REG_BASE_R_STAT_2_2_ */
    reg_value = emac_pcs_reg_BASE_R_STAT_2_2__read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & PCS_REG_BASE_R_STAT_2_2_BIT_HIGH_BER_L_MSK) >> PCS_REG_BASE_R_STAT_2_2_BIT_HIGH_BER_L_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_HIGH_BER_L_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_BER_CNT_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_BER_CNT_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_BER_CNT_LSB_get", A, 11);
    /* (0x0000c084 bits 13:8) bits 0:5 use field BER_CNT_LSB of register PMC_PCS_REG_BASE_R_STAT_2_2_ */
    reg_value = emac_pcs_reg_BASE_R_STAT_2_2__read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & PCS_REG_BASE_R_STAT_2_2_BIT_BER_CNT_LSB_MSK) >> PCS_REG_BASE_R_STAT_2_2_BIT_BER_CNT_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_BER_CNT_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_BER_CNT_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_BER_CNT_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_BER_CNT_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_BER_CNT_LSB_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_BER_CNT_LSB_get", stop_bit, 5 );
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
    /* (0x0000c084 bits 13:8) bits 0:5 use field BER_CNT_LSB of register PMC_PCS_REG_BASE_R_STAT_2_2_ */
    reg_value = emac_pcs_reg_BASE_R_STAT_2_2__read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & PCS_REG_BASE_R_STAT_2_2_BIT_BER_CNT_LSB_MSK)
                  >> PCS_REG_BASE_R_STAT_2_2_BIT_BER_CNT_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_BASE_R_STAT_2_2_BIT_BER_CNT_LSB_MSK, PCS_REG_BASE_R_STAT_2_2_BIT_BER_CNT_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_BER_CNT_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_BLOCK_ERR_CNT_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_BLOCK_ERR_CNT_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_BLOCK_ERR_CNT_LSB_get", A, 11);
    /* (0x0000c084 bits 7:0) bits 0:7 use field BLOCK_ERR_CNT_LSB of register PMC_PCS_REG_BASE_R_STAT_2_2_ */
    reg_value = emac_pcs_reg_BASE_R_STAT_2_2__read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & PCS_REG_BASE_R_STAT_2_2_BIT_BLOCK_ERR_CNT_LSB_MSK) >> PCS_REG_BASE_R_STAT_2_2_BIT_BLOCK_ERR_CNT_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_BLOCK_ERR_CNT_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_BLOCK_ERR_CNT_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_BLOCK_ERR_CNT_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_BLOCK_ERR_CNT_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_BLOCK_ERR_CNT_LSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_BLOCK_ERR_CNT_LSB_get", stop_bit, 7 );
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
    /* (0x0000c084 bits 7:0) bits 0:7 use field BLOCK_ERR_CNT_LSB of register PMC_PCS_REG_BASE_R_STAT_2_2_ */
    reg_value = emac_pcs_reg_BASE_R_STAT_2_2__read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & PCS_REG_BASE_R_STAT_2_2_BIT_BLOCK_ERR_CNT_LSB_MSK)
                  >> PCS_REG_BASE_R_STAT_2_2_BIT_BLOCK_ERR_CNT_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_BASE_R_STAT_2_2_BIT_BLOCK_ERR_CNT_LSB_MSK, PCS_REG_BASE_R_STAT_2_2_BIT_BLOCK_ERR_CNT_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_BLOCK_ERR_CNT_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_TEST_ERR_CNT_get( emac_pcs_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_TEST_ERR_CNT_get( emac_pcs_buffer_t *b_ptr,
                                                      emac_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_TEST_ERR_CNT_get", A, 11);
    /* (0x0000c0ac bits 15:0) bits 0:15 use field TEST_ERR_CNT of register PMC_PCS_REG_BASE_R_TEST_ERR_CNT */
    reg_value = emac_pcs_reg_BASE_R_TEST_ERR_CNT_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & PCS_REG_BASE_R_TEST_ERR_CNT_BIT_TEST_ERR_CNT_MSK) >> PCS_REG_BASE_R_TEST_ERR_CNT_BIT_TEST_ERR_CNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_TEST_ERR_CNT_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_TEST_ERR_CNT_get( emac_pcs_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_TEST_ERR_CNT_get( emac_pcs_buffer_t *b_ptr,
                                                            emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_TEST_ERR_CNT_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_TEST_ERR_CNT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_TEST_ERR_CNT_get", stop_bit, 15 );
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
    /* (0x0000c0ac bits 15:0) bits 0:15 use field TEST_ERR_CNT of register PMC_PCS_REG_BASE_R_TEST_ERR_CNT */
    reg_value = emac_pcs_reg_BASE_R_TEST_ERR_CNT_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & PCS_REG_BASE_R_TEST_ERR_CNT_BIT_TEST_ERR_CNT_MSK)
                  >> PCS_REG_BASE_R_TEST_ERR_CNT_BIT_TEST_ERR_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_BASE_R_TEST_ERR_CNT_BIT_TEST_ERR_CNT_MSK, PCS_REG_BASE_R_TEST_ERR_CNT_BIT_TEST_ERR_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_TEST_ERR_CNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_BER_CNT_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_BER_CNT_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                     emac_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_BER_CNT_MSB_get", A, 11);
    /* (0x0000c0b0 bits 15:0) bits 0:15 use field BER_CNT_MSB of register PMC_PCS_REG_BER_CNT_MSB_ */
    reg_value = emac_pcs_reg_BER_CNT_MSB__read(  b_ptr, h_ptr, A);
    value = (reg_value & PCS_REG_BER_CNT_MSB_BIT_BER_CNT_MSB_MSK) >> PCS_REG_BER_CNT_MSB_BIT_BER_CNT_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_BER_CNT_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_BER_CNT_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_BER_CNT_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_BER_CNT_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_BER_CNT_MSB_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_BER_CNT_MSB_get", stop_bit, 15 );
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
    /* (0x0000c0b0 bits 15:0) bits 0:15 use field BER_CNT_MSB of register PMC_PCS_REG_BER_CNT_MSB_ */
    reg_value = emac_pcs_reg_BER_CNT_MSB__read(  b_ptr, h_ptr, A);
    field_value = (reg_value & PCS_REG_BER_CNT_MSB_BIT_BER_CNT_MSB_MSK)
                  >> PCS_REG_BER_CNT_MSB_BIT_BER_CNT_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_BER_CNT_MSB_BIT_BER_CNT_MSB_MSK, PCS_REG_BER_CNT_MSB_BIT_BER_CNT_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_BER_CNT_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_BLOCK_ERR_CNT_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_BLOCK_ERR_CNT_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                           emac_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_BLOCK_ERR_CNT_MSB_get", A, 11);
    /* (0x0000c0b4 bits 13:0) bits 0:13 use field BLOCK_ERR_CNT_MSB of register PMC_PCS_REG_BLOCK_ERR_CNT_MSB_ */
    reg_value = emac_pcs_reg_BLOCK_ERR_CNT_MSB__read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & PCS_REG_BLOCK_ERR_CNT_MSB_BIT_BLOCK_ERR_CNT_MSB_MSK) >> PCS_REG_BLOCK_ERR_CNT_MSB_BIT_BLOCK_ERR_CNT_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_BLOCK_ERR_CNT_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_BLOCK_ERR_CNT_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_BLOCK_ERR_CNT_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                                 emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_BLOCK_ERR_CNT_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_BLOCK_ERR_CNT_MSB_get", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_BLOCK_ERR_CNT_MSB_get", stop_bit, 13 );
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
    /* (0x0000c0b4 bits 13:0) bits 0:13 use field BLOCK_ERR_CNT_MSB of register PMC_PCS_REG_BLOCK_ERR_CNT_MSB_ */
    reg_value = emac_pcs_reg_BLOCK_ERR_CNT_MSB__read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & PCS_REG_BLOCK_ERR_CNT_MSB_BIT_BLOCK_ERR_CNT_MSB_MSK)
                  >> PCS_REG_BLOCK_ERR_CNT_MSB_BIT_BLOCK_ERR_CNT_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_BLOCK_ERR_CNT_MSB_BIT_BLOCK_ERR_CNT_MSB_MSK, PCS_REG_BLOCK_ERR_CNT_MSB_BIT_BLOCK_ERR_CNT_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_BLOCK_ERR_CNT_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_PMON_BLOCK_ERR_CNT_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_PMON_BLOCK_ERR_CNT_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_PMON_BLOCK_ERR_CNT_LSB_get", A, 11);
    /* (0x0000c0b8 bits 15:0) bits 0:15 use field PMON_BLOCK_ERR_CNT_LSB of register PMC_PCS_REG_PMON_BLOCK_ERR_CNT_1_3_ */
    reg_value = emac_pcs_reg_PMON_BLOCK_ERR_CNT_1_3__read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & PCS_REG_PMON_BLOCK_ERR_CNT_1_3_BIT_PMON_BLOCK_ERR_CNT_LSB_MSK) >> PCS_REG_PMON_BLOCK_ERR_CNT_1_3_BIT_PMON_BLOCK_ERR_CNT_LSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_PMON_BLOCK_ERR_CNT_LSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_PMON_BLOCK_ERR_CNT_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_PMON_BLOCK_ERR_CNT_LSB_get( emac_pcs_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_PMON_BLOCK_ERR_CNT_LSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_PMON_BLOCK_ERR_CNT_LSB_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_PMON_BLOCK_ERR_CNT_LSB_get", stop_bit, 15 );
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
    /* (0x0000c0b8 bits 15:0) bits 0:15 use field PMON_BLOCK_ERR_CNT_LSB of register PMC_PCS_REG_PMON_BLOCK_ERR_CNT_1_3_ */
    reg_value = emac_pcs_reg_PMON_BLOCK_ERR_CNT_1_3__read( b_ptr,
                                                           h_ptr,
                                                           A);
    field_value = (reg_value & PCS_REG_PMON_BLOCK_ERR_CNT_1_3_BIT_PMON_BLOCK_ERR_CNT_LSB_MSK)
                  >> PCS_REG_PMON_BLOCK_ERR_CNT_1_3_BIT_PMON_BLOCK_ERR_CNT_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_PMON_BLOCK_ERR_CNT_1_3_BIT_PMON_BLOCK_ERR_CNT_LSB_MSK, PCS_REG_PMON_BLOCK_ERR_CNT_1_3_BIT_PMON_BLOCK_ERR_CNT_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_PMON_BLOCK_ERR_CNT_LSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_PMON_BLOCK_ERR_CNT_MID_get( emac_pcs_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_PMON_BLOCK_ERR_CNT_MID_get( emac_pcs_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_PMON_BLOCK_ERR_CNT_MID_get", A, 11);
    /* (0x0000c0bc bits 15:0) bits 0:15 use field PMON_BLOCK_ERR_CNT_MID of register PMC_PCS_REG_PMON_BLOCK_ERR_CNT_2_3_ */
    reg_value = emac_pcs_reg_PMON_BLOCK_ERR_CNT_2_3__read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & PCS_REG_PMON_BLOCK_ERR_CNT_2_3_BIT_PMON_BLOCK_ERR_CNT_MID_MSK) >> PCS_REG_PMON_BLOCK_ERR_CNT_2_3_BIT_PMON_BLOCK_ERR_CNT_MID_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_PMON_BLOCK_ERR_CNT_MID_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_PMON_BLOCK_ERR_CNT_MID_get( emac_pcs_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_PMON_BLOCK_ERR_CNT_MID_get( emac_pcs_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_PMON_BLOCK_ERR_CNT_MID_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_PMON_BLOCK_ERR_CNT_MID_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_PMON_BLOCK_ERR_CNT_MID_get", stop_bit, 15 );
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
    /* (0x0000c0bc bits 15:0) bits 0:15 use field PMON_BLOCK_ERR_CNT_MID of register PMC_PCS_REG_PMON_BLOCK_ERR_CNT_2_3_ */
    reg_value = emac_pcs_reg_PMON_BLOCK_ERR_CNT_2_3__read( b_ptr,
                                                           h_ptr,
                                                           A);
    field_value = (reg_value & PCS_REG_PMON_BLOCK_ERR_CNT_2_3_BIT_PMON_BLOCK_ERR_CNT_MID_MSK)
                  >> PCS_REG_PMON_BLOCK_ERR_CNT_2_3_BIT_PMON_BLOCK_ERR_CNT_MID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_PMON_BLOCK_ERR_CNT_2_3_BIT_PMON_BLOCK_ERR_CNT_MID_MSK, PCS_REG_PMON_BLOCK_ERR_CNT_2_3_BIT_PMON_BLOCK_ERR_CNT_MID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_PMON_BLOCK_ERR_CNT_MID_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_PMON_BLOCK_ERR_CNT_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_PMON_BLOCK_ERR_CNT_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_PMON_BLOCK_ERR_CNT_MSB_get", A, 11);
    /* (0x0000c0c0 bits 7:0) bits 0:7 use field PMON_BLOCK_ERR_CNT_MSB of register PMC_PCS_REG_PMON_BLOCK_ERR_CNT_3_3_ */
    reg_value = emac_pcs_reg_PMON_BLOCK_ERR_CNT_3_3__read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & PCS_REG_PMON_BLOCK_ERR_CNT_3_3_BIT_PMON_BLOCK_ERR_CNT_MSB_MSK) >> PCS_REG_PMON_BLOCK_ERR_CNT_3_3_BIT_PMON_BLOCK_ERR_CNT_MSB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_PMON_BLOCK_ERR_CNT_MSB_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_PMON_BLOCK_ERR_CNT_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_PMON_BLOCK_ERR_CNT_MSB_get( emac_pcs_buffer_t *b_ptr,
                                                                      emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_PMON_BLOCK_ERR_CNT_MSB_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_PMON_BLOCK_ERR_CNT_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_PMON_BLOCK_ERR_CNT_MSB_get", stop_bit, 7 );
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
    /* (0x0000c0c0 bits 7:0) bits 0:7 use field PMON_BLOCK_ERR_CNT_MSB of register PMC_PCS_REG_PMON_BLOCK_ERR_CNT_3_3_ */
    reg_value = emac_pcs_reg_PMON_BLOCK_ERR_CNT_3_3__read( b_ptr,
                                                           h_ptr,
                                                           A);
    field_value = (reg_value & PCS_REG_PMON_BLOCK_ERR_CNT_3_3_BIT_PMON_BLOCK_ERR_CNT_MSB_MSK)
                  >> PCS_REG_PMON_BLOCK_ERR_CNT_3_3_BIT_PMON_BLOCK_ERR_CNT_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_PMON_BLOCK_ERR_CNT_3_3_BIT_PMON_BLOCK_ERR_CNT_MSB_MSK, PCS_REG_PMON_BLOCK_ERR_CNT_3_3_BIT_PMON_BLOCK_ERR_CNT_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_PMON_BLOCK_ERR_CNT_MSB_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_ALL_LANES_LOCK_get( emac_pcs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_ALL_LANES_LOCK_get( emac_pcs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_ALL_LANES_LOCK_get", A, 11);
    /* (0x0000c0c8 bits 12) field ALL_LANES_LOCK of register PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_1_4_ */
    reg_value = emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_1_4__read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    value = (reg_value & PCS_REG_MULTI_LANE_ALIGNMENT_STAT_1_4_BIT_ALL_LANES_LOCK_MSK) >> PCS_REG_MULTI_LANE_ALIGNMENT_STAT_1_4_BIT_ALL_LANES_LOCK_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_ALL_LANES_LOCK_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_LANE_7_0_BLOCK_LOCK_get( emac_pcs_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_LANE_7_0_BLOCK_LOCK_get( emac_pcs_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_LANE_7_0_BLOCK_LOCK_get", A, 11);
    /* (0x0000c0c8 bits 7:0) bits 0:7 use field LANE_7_0_BLOCK_LOCK of register PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_1_4_ */
    reg_value = emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_1_4__read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    value = (reg_value & PCS_REG_MULTI_LANE_ALIGNMENT_STAT_1_4_BIT_LANE_7_0_BLOCK_LOCK_MSK) >> PCS_REG_MULTI_LANE_ALIGNMENT_STAT_1_4_BIT_LANE_7_0_BLOCK_LOCK_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_LANE_7_0_BLOCK_LOCK_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_LANE_7_0_BLOCK_LOCK_get( emac_pcs_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_LANE_7_0_BLOCK_LOCK_get( emac_pcs_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_LANE_7_0_BLOCK_LOCK_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_LANE_7_0_BLOCK_LOCK_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_LANE_7_0_BLOCK_LOCK_get", stop_bit, 7 );
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
    /* (0x0000c0c8 bits 7:0) bits 0:7 use field LANE_7_0_BLOCK_LOCK of register PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_1_4_ */
    reg_value = emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_1_4__read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    field_value = (reg_value & PCS_REG_MULTI_LANE_ALIGNMENT_STAT_1_4_BIT_LANE_7_0_BLOCK_LOCK_MSK)
                  >> PCS_REG_MULTI_LANE_ALIGNMENT_STAT_1_4_BIT_LANE_7_0_BLOCK_LOCK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_MULTI_LANE_ALIGNMENT_STAT_1_4_BIT_LANE_7_0_BLOCK_LOCK_MSK, PCS_REG_MULTI_LANE_ALIGNMENT_STAT_1_4_BIT_LANE_7_0_BLOCK_LOCK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_LANE_7_0_BLOCK_LOCK_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_LANE_19_12_BLOCK_LOCK_get( emac_pcs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_LANE_19_12_BLOCK_LOCK_get( emac_pcs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_LANE_19_12_BLOCK_LOCK_get", A, 11);
    /* (0x0000c0cc bits 11:0) bits 0:11 use field LANE_19_12_BLOCK_LOCK of register PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_2_4_ */
    reg_value = emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_2_4__read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    value = (reg_value & PCS_REG_MULTI_LANE_ALIGNMENT_STAT_2_4_BIT_LANE_19_12_BLOCK_LOCK_MSK) >> PCS_REG_MULTI_LANE_ALIGNMENT_STAT_2_4_BIT_LANE_19_12_BLOCK_LOCK_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_LANE_19_12_BLOCK_LOCK_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_LANE_19_12_BLOCK_LOCK_get( emac_pcs_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_LANE_19_12_BLOCK_LOCK_get( emac_pcs_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_LANE_19_12_BLOCK_LOCK_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_LANE_19_12_BLOCK_LOCK_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_LANE_19_12_BLOCK_LOCK_get", stop_bit, 11 );
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
    /* (0x0000c0cc bits 11:0) bits 0:11 use field LANE_19_12_BLOCK_LOCK of register PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_2_4_ */
    reg_value = emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_2_4__read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    field_value = (reg_value & PCS_REG_MULTI_LANE_ALIGNMENT_STAT_2_4_BIT_LANE_19_12_BLOCK_LOCK_MSK)
                  >> PCS_REG_MULTI_LANE_ALIGNMENT_STAT_2_4_BIT_LANE_19_12_BLOCK_LOCK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_MULTI_LANE_ALIGNMENT_STAT_2_4_BIT_LANE_19_12_BLOCK_LOCK_MSK, PCS_REG_MULTI_LANE_ALIGNMENT_STAT_2_4_BIT_LANE_19_12_BLOCK_LOCK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_LANE_19_12_BLOCK_LOCK_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_LANE_ALIGN_LOCK_7_0_get( emac_pcs_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_LANE_ALIGN_LOCK_7_0_get( emac_pcs_buffer_t *b_ptr,
                                                             emac_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_LANE_ALIGN_LOCK_7_0_get", A, 11);
    /* (0x0000c0d0 bits 7:0) bits 0:7 use field LANE_ALIGN_LOCK_7_0 of register PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_3_4_ */
    reg_value = emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_3_4__read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    value = (reg_value & PCS_REG_MULTI_LANE_ALIGNMENT_STAT_3_4_BIT_LANE_ALIGN_LOCK_7_0_MSK) >> PCS_REG_MULTI_LANE_ALIGNMENT_STAT_3_4_BIT_LANE_ALIGN_LOCK_7_0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_LANE_ALIGN_LOCK_7_0_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_LANE_ALIGN_LOCK_7_0_get( emac_pcs_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_LANE_ALIGN_LOCK_7_0_get( emac_pcs_buffer_t *b_ptr,
                                                                   emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_LANE_ALIGN_LOCK_7_0_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_LANE_ALIGN_LOCK_7_0_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_LANE_ALIGN_LOCK_7_0_get", stop_bit, 7 );
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
    /* (0x0000c0d0 bits 7:0) bits 0:7 use field LANE_ALIGN_LOCK_7_0 of register PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_3_4_ */
    reg_value = emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_3_4__read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    field_value = (reg_value & PCS_REG_MULTI_LANE_ALIGNMENT_STAT_3_4_BIT_LANE_ALIGN_LOCK_7_0_MSK)
                  >> PCS_REG_MULTI_LANE_ALIGNMENT_STAT_3_4_BIT_LANE_ALIGN_LOCK_7_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_MULTI_LANE_ALIGNMENT_STAT_3_4_BIT_LANE_ALIGN_LOCK_7_0_MSK, PCS_REG_MULTI_LANE_ALIGNMENT_STAT_3_4_BIT_LANE_ALIGN_LOCK_7_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_LANE_ALIGN_LOCK_7_0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_LANE_ALIGN_LOCK_19_12_get( emac_pcs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_LANE_ALIGN_LOCK_19_12_get( emac_pcs_buffer_t *b_ptr,
                                                               emac_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_LANE_ALIGN_LOCK_19_12_get", A, 11);
    /* (0x0000c0d4 bits 11:0) bits 0:11 use field LANE_ALIGN_LOCK_19_12 of register PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_4_4_ */
    reg_value = emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_4_4__read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    value = (reg_value & PCS_REG_MULTI_LANE_ALIGNMENT_STAT_4_4_BIT_LANE_ALIGN_LOCK_19_12_MSK) >> PCS_REG_MULTI_LANE_ALIGNMENT_STAT_4_4_BIT_LANE_ALIGN_LOCK_19_12_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "emac_pcs_field_LANE_ALIGN_LOCK_19_12_get", A, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_LANE_ALIGN_LOCK_19_12_get( emac_pcs_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_LANE_ALIGN_LOCK_19_12_get( emac_pcs_buffer_t *b_ptr,
                                                                     emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_LANE_ALIGN_LOCK_19_12_get", A, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_LANE_ALIGN_LOCK_19_12_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_LANE_ALIGN_LOCK_19_12_get", stop_bit, 11 );
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
    /* (0x0000c0d4 bits 11:0) bits 0:11 use field LANE_ALIGN_LOCK_19_12 of register PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_4_4_ */
    reg_value = emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_4_4__read( b_ptr,
                                                                  h_ptr,
                                                                  A);
    field_value = (reg_value & PCS_REG_MULTI_LANE_ALIGNMENT_STAT_4_4_BIT_LANE_ALIGN_LOCK_19_12_MSK)
                  >> PCS_REG_MULTI_LANE_ALIGNMENT_STAT_4_4_BIT_LANE_ALIGN_LOCK_19_12_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_MULTI_LANE_ALIGNMENT_STAT_4_4_BIT_LANE_ALIGN_LOCK_19_12_MSK, PCS_REG_MULTI_LANE_ALIGNMENT_STAT_4_4_BIT_LANE_ALIGN_LOCK_19_12_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_LANE_ALIGN_LOCK_19_12_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status_N_size20
 * ==================================================================================
 */
static INLINE UINT32 emac_pcs_field_LANE_BIP_ERR_CNT_get( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_LANE_BIP_ERR_CNT_get( emac_pcs_buffer_t *b_ptr,
                                                          emac_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_LANE_BIP_ERR_CNT_get", A, 11);
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "emac_pcs_field_LANE_BIP_ERR_CNT_get", N, 19);
    /* ((0x0000c168 + (N) * 4) bits 15:0) bits 0:15 use field LANE_BIP_ERR_CNT of register PMC_PCS_REG_LANE_BIP_ERR_CNT index N=0..19 */
    reg_value = emac_pcs_reg_LANE_BIP_ERR_CNT_array_read( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          N);
    value = (reg_value & PCS_REG_LANE_BIP_ERR_CNT_BIT_LANE_BIP_ERR_CNT_MSK) >> PCS_REG_LANE_BIP_ERR_CNT_BIT_LANE_BIP_ERR_CNT_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "emac_pcs_field_LANE_BIP_ERR_CNT_get", A, N, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_LANE_BIP_ERR_CNT_get( emac_pcs_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_LANE_BIP_ERR_CNT_get( emac_pcs_buffer_t *b_ptr,
                                                                emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_LANE_BIP_ERR_CNT_get", A, 11);
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "emac_pcs_field_range_LANE_BIP_ERR_CNT_get", N, 19);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_LANE_BIP_ERR_CNT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_LANE_BIP_ERR_CNT_get", stop_bit, 15 );
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
    /* ((0x0000c168 + (N) * 4) bits 15:0) bits 0:15 use field LANE_BIP_ERR_CNT of register PMC_PCS_REG_LANE_BIP_ERR_CNT index N=0..19 */
    reg_value = emac_pcs_reg_LANE_BIP_ERR_CNT_array_read( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          N);
    field_value = (reg_value & PCS_REG_LANE_BIP_ERR_CNT_BIT_LANE_BIP_ERR_CNT_MSK)
                  >> PCS_REG_LANE_BIP_ERR_CNT_BIT_LANE_BIP_ERR_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_LANE_BIP_ERR_CNT_BIT_LANE_BIP_ERR_CNT_MSK, PCS_REG_LANE_BIP_ERR_CNT_BIT_LANE_BIP_ERR_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_LANE_BIP_ERR_CNT_get", A, N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_LANE_MAP_get( emac_pcs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_LANE_MAP_get( emac_pcs_buffer_t *b_ptr,
                                                  emac_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_LANE_MAP_get", A, 11);
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "emac_pcs_field_LANE_MAP_get", N, 19);
    /* ((0x0000c1b8 + (N) * 4) bits 4:0) bits 0:4 use field LANE_MAP of register PMC_PCS_REG_LANE_MAPPING index N=0..19 */
    reg_value = emac_pcs_reg_LANE_MAPPING_array_read( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N);
    value = (reg_value & PCS_REG_LANE_MAPPING_BIT_LANE_MAP_MSK) >> PCS_REG_LANE_MAPPING_BIT_LANE_MAP_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "emac_pcs_field_LANE_MAP_get", A, N, value );

    return value;
}
static INLINE UINT32 emac_pcs_field_range_LANE_MAP_get( emac_pcs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 emac_pcs_field_range_LANE_MAP_get( emac_pcs_buffer_t *b_ptr,
                                                        emac_handle_t *h_ptr,
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

    if (A > 11)
        IO_RANGE_CHECK("%s A is %d but max is %d", "emac_pcs_field_range_LANE_MAP_get", A, 11);
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "emac_pcs_field_range_LANE_MAP_get", N, 19);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "emac_pcs_field_range_LANE_MAP_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "emac_pcs_field_range_LANE_MAP_get", stop_bit, 4 );
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
    /* ((0x0000c1b8 + (N) * 4) bits 4:0) bits 0:4 use field LANE_MAP of register PMC_PCS_REG_LANE_MAPPING index N=0..19 */
    reg_value = emac_pcs_reg_LANE_MAPPING_array_read( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N);
    field_value = (reg_value & PCS_REG_LANE_MAPPING_BIT_LANE_MAP_MSK)
                  >> PCS_REG_LANE_MAPPING_BIT_LANE_MAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PCS_REG_LANE_MAPPING_BIT_LANE_MAP_MSK, PCS_REG_LANE_MAPPING_BIT_LANE_MAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "emac_pcs_field_range_LANE_MAP_get", A, N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EMAC_PCS_IO_INLINE_H */
