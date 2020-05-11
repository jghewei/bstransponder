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
 *     and register accessor functions for the fc_cbr_patt_ins block
 *****************************************************************************/
#ifndef _FC_CBR_PATT_INS_IO_INLINE_H
#define _FC_CBR_PATT_INS_IO_INLINE_H

#include "cbr_fsgm_api.h"
#include "fc_cbr_patt_ins_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define FC_CBR_PATT_INS_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for fc_cbr_patt_ins
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
    cbr_fsgm_handle_t * h_ptr;
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} fc_cbr_patt_ins_buffer_t;
static INLINE void fc_cbr_patt_ins_buffer_init( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                cbr_fsgm_handle_t *h_ptr,
                                                UINT32 A ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_buffer_init( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                cbr_fsgm_handle_t *h_ptr,
                                                UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x40) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "fc_cbr_patt_ins_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void fc_cbr_patt_ins_buffer_flush( fc_cbr_patt_ins_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_buffer_flush( fc_cbr_patt_ins_buffer_t *b_ptr )
{
    IOLOG( "fc_cbr_patt_ins_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 fc_cbr_patt_ins_reg_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                               cbr_fsgm_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 mem_type,
                                               UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_reg_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                               cbr_fsgm_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 mem_type,
                                               UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->A)*0x40),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg);
    return value;
}
static INLINE void fc_cbr_patt_ins_reg_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                              cbr_fsgm_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                              cbr_fsgm_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->A)*0x40),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                         value);
}

static INLINE void fc_cbr_patt_ins_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                              cbr_fsgm_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 mask,
                                              UINT32 unused_mask,
                                              UINT32 ofs,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                              cbr_fsgm_handle_t *h_ptr,
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
                          (b_ptr->base_address + (b_ptr->A)*0x40),
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
                             (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void fc_cbr_patt_ins_action_on_write_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mem_type,
                                                              UINT32 reg,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_action_on_write_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
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
                                         (b_ptr->base_address + (b_ptr->A)*0x40),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                         value<<ofs);
}

static INLINE void fc_cbr_patt_ins_burst_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                               cbr_fsgm_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 mem_type,
                                               UINT32 reg,
                                               UINT32 len,
                                               UINT32 *value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_burst_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                               cbr_fsgm_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 mem_type,
                                               UINT32 reg,
                                               UINT32 len,
                                               UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_read( b_ptr->coalesce_handle,
                          mem_type,
                          (b_ptr->base_address + (b_ptr->A)*0x40),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                          len,
                          value);
}

static INLINE void fc_cbr_patt_ins_burst_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                cbr_fsgm_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mem_type,
                                                UINT32 reg,
                                                UINT32 len,
                                                UINT32 *value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_burst_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                cbr_fsgm_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mem_type,
                                                UINT32 reg,
                                                UINT32 len,
                                                UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_write( b_ptr->coalesce_handle,
                           mem_type,
                           (b_ptr->base_address + (b_ptr->A)*0x40),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mem_type,
                                                         UINT32 reg,
                                                         UINT32 mask,
                                                         UINT32 value,
                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                         UINT32 max_count,
                                                         UINT32 *num_failed_polls,
                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
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
                           (b_ptr->base_address + (b_ptr->A)*0x40),
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
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x40) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for fc_cbr_patt_ins
 * ==================================================================================
 */

static INLINE void fc_cbr_patt_ins_reg_CFG_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                  cbr_fsgm_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_CFG_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                  cbr_fsgm_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_patt_ins_reg_CFG_write", value );
    fc_cbr_patt_ins_reg_write( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_FC_CBR_PATT_INS_REG_CFG,
                               value);
}

static INLINE void fc_cbr_patt_ins_reg_CFG_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_CFG_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_patt_ins_reg_CFG_field_set", A, mask, ofs, value );
    fc_cbr_patt_ins_field_set( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_FC_CBR_PATT_INS_REG_CFG,
                               mask,
                               PMC_FC_CBR_PATT_INS_REG_CFG_UNUSED_MASK,
                               ofs,
                               value);

}

static INLINE UINT32 fc_cbr_patt_ins_reg_CFG_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                   cbr_fsgm_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_reg_CFG_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                   cbr_fsgm_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_patt_ins_reg_read( b_ptr,
                                          h_ptr,
                                          A,
                                          MEM_TYPE_CONFIG,
                                          PMC_FC_CBR_PATT_INS_REG_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_patt_ins_reg_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATT_31_0_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                 cbr_fsgm_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATT_31_0_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                 cbr_fsgm_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_patt_ins_reg_USER_FLT_PATT_31_0_write", value );
    fc_cbr_patt_ins_reg_write( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0,
                               value);
}

static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATT_31_0_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                     cbr_fsgm_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATT_31_0_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                     cbr_fsgm_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_patt_ins_reg_USER_FLT_PATT_31_0_field_set", A, mask, ofs, value );
    fc_cbr_patt_ins_field_set( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0,
                               mask,
                               PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0_UNUSED_MASK,
                               ofs,
                               value);

}

static INLINE UINT32 fc_cbr_patt_ins_reg_USER_FLT_PATT_31_0_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                  cbr_fsgm_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_reg_USER_FLT_PATT_31_0_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                  cbr_fsgm_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_patt_ins_reg_read( b_ptr,
                                          h_ptr,
                                          A,
                                          MEM_TYPE_CONFIG,
                                          PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_patt_ins_reg_USER_FLT_PATT_31_0_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATT_63_32_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                  cbr_fsgm_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATT_63_32_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                  cbr_fsgm_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_patt_ins_reg_USER_FLT_PATT_63_32_write", value );
    fc_cbr_patt_ins_reg_write( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32,
                               value);
}

static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATT_63_32_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                      cbr_fsgm_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATT_63_32_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                      cbr_fsgm_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_patt_ins_reg_USER_FLT_PATT_63_32_field_set", A, mask, ofs, value );
    fc_cbr_patt_ins_field_set( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32,
                               mask,
                               PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32_UNUSED_MASK,
                               ofs,
                               value);

}

static INLINE UINT32 fc_cbr_patt_ins_reg_USER_FLT_PATT_63_32_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_reg_USER_FLT_PATT_63_32_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_patt_ins_reg_read( b_ptr,
                                          h_ptr,
                                          A,
                                          MEM_TYPE_CONFIG,
                                          PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_patt_ins_reg_USER_FLT_PATT_63_32_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATT_95_64_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                  cbr_fsgm_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATT_95_64_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                  cbr_fsgm_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_patt_ins_reg_USER_FLT_PATT_95_64_write", value );
    fc_cbr_patt_ins_reg_write( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64,
                               value);
}

static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATT_95_64_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                      cbr_fsgm_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATT_95_64_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                      cbr_fsgm_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_patt_ins_reg_USER_FLT_PATT_95_64_field_set", A, mask, ofs, value );
    fc_cbr_patt_ins_field_set( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64,
                               mask,
                               PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64_UNUSED_MASK,
                               ofs,
                               value);

}

static INLINE UINT32 fc_cbr_patt_ins_reg_USER_FLT_PATT_95_64_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_reg_USER_FLT_PATT_95_64_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_patt_ins_reg_read( b_ptr,
                                          h_ptr,
                                          A,
                                          MEM_TYPE_CONFIG,
                                          PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_patt_ins_reg_USER_FLT_PATT_95_64_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATT_127_96_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATT_127_96_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_patt_ins_reg_USER_FLT_PATT_127_96_write", value );
    fc_cbr_patt_ins_reg_write( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96,
                               value);
}

static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATT_127_96_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATT_127_96_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_patt_ins_reg_USER_FLT_PATT_127_96_field_set", A, mask, ofs, value );
    fc_cbr_patt_ins_field_set( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96,
                               mask,
                               PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96_UNUSED_MASK,
                               ofs,
                               value);

}

static INLINE UINT32 fc_cbr_patt_ins_reg_USER_FLT_PATT_127_96_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                    cbr_fsgm_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_reg_USER_FLT_PATT_127_96_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                    cbr_fsgm_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_patt_ins_reg_read( b_ptr,
                                          h_ptr,
                                          A,
                                          MEM_TYPE_CONFIG,
                                          PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_patt_ins_reg_USER_FLT_PATT_127_96_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATTERN_159_128_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATTERN_159_128_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_patt_ins_reg_USER_FLT_PATTERN_159_128_write", value );
    fc_cbr_patt_ins_reg_write( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128,
                               value);
}

static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATTERN_159_128_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                           cbr_fsgm_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_USER_FLT_PATTERN_159_128_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                           cbr_fsgm_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_patt_ins_reg_USER_FLT_PATTERN_159_128_field_set", A, mask, ofs, value );
    fc_cbr_patt_ins_field_set( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128,
                               mask,
                               PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128_UNUSED_MASK,
                               ofs,
                               value);

}

static INLINE UINT32 fc_cbr_patt_ins_reg_USER_FLT_PATTERN_159_128_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                        cbr_fsgm_handle_t *h_ptr,
                                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_reg_USER_FLT_PATTERN_159_128_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                        cbr_fsgm_handle_t *h_ptr,
                                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_patt_ins_reg_read( b_ptr,
                                          h_ptr,
                                          A,
                                          MEM_TYPE_CONFIG,
                                          PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_patt_ins_reg_USER_FLT_PATTERN_159_128_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_cbr_patt_ins_reg_USER_RCVRY_PATT_31_0_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_USER_RCVRY_PATT_31_0_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_patt_ins_reg_USER_RCVRY_PATT_31_0_write", value );
    fc_cbr_patt_ins_reg_write( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0,
                               value);
}

static INLINE void fc_cbr_patt_ins_reg_USER_RCVRY_PATT_31_0_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_USER_RCVRY_PATT_31_0_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_patt_ins_reg_USER_RCVRY_PATT_31_0_field_set", A, mask, ofs, value );
    fc_cbr_patt_ins_field_set( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0,
                               mask,
                               PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0_UNUSED_MASK,
                               ofs,
                               value);

}

static INLINE UINT32 fc_cbr_patt_ins_reg_USER_RCVRY_PATT_31_0_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                    cbr_fsgm_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_reg_USER_RCVRY_PATT_31_0_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                    cbr_fsgm_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_patt_ins_reg_read( b_ptr,
                                          h_ptr,
                                          A,
                                          MEM_TYPE_CONFIG,
                                          PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_patt_ins_reg_USER_RCVRY_PATT_31_0_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_cbr_patt_ins_reg_USER_RCVRY_PATT_39_32_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                    cbr_fsgm_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_USER_RCVRY_PATT_39_32_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                    cbr_fsgm_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_patt_ins_reg_USER_RCVRY_PATT_39_32_write", value );
    fc_cbr_patt_ins_reg_write( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32,
                               value);
}

static INLINE void fc_cbr_patt_ins_reg_USER_RCVRY_PATT_39_32_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                        cbr_fsgm_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_USER_RCVRY_PATT_39_32_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                        cbr_fsgm_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_patt_ins_reg_USER_RCVRY_PATT_39_32_field_set", A, mask, ofs, value );
    fc_cbr_patt_ins_field_set( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_CONFIG,
                               PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32,
                               mask,
                               PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32_UNUSED_MASK,
                               ofs,
                               value);

}

static INLINE UINT32 fc_cbr_patt_ins_reg_USER_RCVRY_PATT_39_32_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                     cbr_fsgm_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_reg_USER_RCVRY_PATT_39_32_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                     cbr_fsgm_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_patt_ins_reg_read( b_ptr,
                                          h_ptr,
                                          A,
                                          MEM_TYPE_CONFIG,
                                          PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_patt_ins_reg_USER_RCVRY_PATT_39_32_read", reg_value, A);
    return reg_value;
}

static INLINE void fc_cbr_patt_ins_reg_UPDATE_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                     cbr_fsgm_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_UPDATE_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                     cbr_fsgm_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_patt_ins_reg_UPDATE_write", value );
    fc_cbr_patt_ins_reg_write( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_FC_CBR_PATT_INS_REG_UPDATE,
                               value);
}

static INLINE void fc_cbr_patt_ins_reg_UPDATE_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_UPDATE_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_patt_ins_reg_UPDATE_field_set", A, mask, ofs, value );
    fc_cbr_patt_ins_field_set( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_FC_CBR_PATT_INS_REG_UPDATE,
                               mask,
                               PMC_FC_CBR_PATT_INS_REG_UPDATE_UNUSED_MASK,
                               ofs,
                               value);

}

static INLINE UINT32 fc_cbr_patt_ins_reg_UPDATE_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_reg_UPDATE_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_patt_ins_reg_read( b_ptr,
                                          h_ptr,
                                          A,
                                          MEM_TYPE_STATUS,
                                          PMC_FC_CBR_PATT_INS_REG_UPDATE);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_patt_ins_reg_UPDATE_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_reg_UPDATE_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                    cbr_fsgm_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_reg_UPDATE_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                    cbr_fsgm_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "fc_cbr_patt_ins_reg_UPDATE_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return fc_cbr_patt_ins_poll( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_FC_CBR_PATT_INS_REG_UPDATE,
                                 mask,
                                 value,
                                 cmp,
                                 max_count,
                                 num_failed_polls,
                                 delay_between_polls_in_microseconds);

}


static INLINE void fc_cbr_patt_ins_reg_INT_INDICATION_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                             cbr_fsgm_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_INT_INDICATION_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                             cbr_fsgm_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_patt_ins_reg_INT_INDICATION_write", value );
    fc_cbr_patt_ins_reg_write( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_FC_CBR_PATT_INS_REG_INT_INDICATION,
                               value);
}

static INLINE void fc_cbr_patt_ins_reg_INT_INDICATION_action_on_write_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                                 cbr_fsgm_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_INT_INDICATION_action_on_write_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                                 cbr_fsgm_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_patt_ins_reg_INT_INDICATION_action_on_write_field_set", A, mask, ofs, value );
    fc_cbr_patt_ins_action_on_write_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               MEM_TYPE_STATUS,
                                               PMC_FC_CBR_PATT_INS_REG_INT_INDICATION,
                                               mask,
                                               ofs,
                                               value);

}

static INLINE UINT32 fc_cbr_patt_ins_reg_INT_INDICATION_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_reg_INT_INDICATION_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_patt_ins_reg_read( b_ptr,
                                          h_ptr,
                                          A,
                                          MEM_TYPE_STATUS,
                                          PMC_FC_CBR_PATT_INS_REG_INT_INDICATION);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_patt_ins_reg_INT_INDICATION_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_reg_INT_INDICATION_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                            cbr_fsgm_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_reg_INT_INDICATION_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                            cbr_fsgm_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "fc_cbr_patt_ins_reg_INT_INDICATION_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return fc_cbr_patt_ins_poll( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_FC_CBR_PATT_INS_REG_INT_INDICATION,
                                 mask,
                                 value,
                                 cmp,
                                 max_count,
                                 num_failed_polls,
                                 delay_between_polls_in_microseconds);

}


static INLINE void fc_cbr_patt_ins_reg_INTERRUPT_ENA_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                            cbr_fsgm_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_INTERRUPT_ENA_write( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                            cbr_fsgm_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "fc_cbr_patt_ins_reg_INTERRUPT_ENA_write", value );
    fc_cbr_patt_ins_reg_write( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA,
                               value);
}

static INLINE void fc_cbr_patt_ins_reg_INTERRUPT_ENA_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                cbr_fsgm_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_reg_INTERRUPT_ENA_field_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                cbr_fsgm_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "fc_cbr_patt_ins_reg_INTERRUPT_ENA_field_set", A, mask, ofs, value );
    fc_cbr_patt_ins_field_set( b_ptr,
                               h_ptr,
                               A,
                               MEM_TYPE_STATUS,
                               PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA,
                               mask,
                               PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA_UNUSED_MASK,
                               ofs,
                               value);

}

static INLINE UINT32 fc_cbr_patt_ins_reg_INTERRUPT_ENA_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                             cbr_fsgm_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_reg_INTERRUPT_ENA_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                             cbr_fsgm_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_patt_ins_reg_read( b_ptr,
                                          h_ptr,
                                          A,
                                          MEM_TYPE_STATUS,
                                          PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_patt_ins_reg_INTERRUPT_ENA_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc_cbr_patt_ins_reg_PN11_SYNC_STAT_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_reg_PN11_SYNC_STAT_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_patt_ins_reg_read( b_ptr,
                                          h_ptr,
                                          A,
                                          MEM_TYPE_STATUS,
                                          PMC_FC_CBR_PATT_INS_REG_PN11_SYNC_STAT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_patt_ins_reg_PN11_SYNC_STAT_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_reg_PN11_SYNC_STAT_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                            cbr_fsgm_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_reg_PN11_SYNC_STAT_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                            cbr_fsgm_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "fc_cbr_patt_ins_reg_PN11_SYNC_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return fc_cbr_patt_ins_poll( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_FC_CBR_PATT_INS_REG_PN11_SYNC_STAT,
                                 mask,
                                 value,
                                 cmp,
                                 max_count,
                                 num_failed_polls,
                                 delay_between_polls_in_microseconds);

}


static INLINE UINT32 fc_cbr_patt_ins_reg_TX_PN11_ERR_CNT_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_reg_TX_PN11_ERR_CNT_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_patt_ins_reg_read( b_ptr,
                                          h_ptr,
                                          A,
                                          MEM_TYPE_STATUS,
                                          PMC_FC_CBR_PATT_INS_REG_TX_PN11_ERR_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_patt_ins_reg_TX_PN11_ERR_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc_cbr_patt_ins_reg_RX_PN11_ERR_CNT_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_reg_RX_PN11_ERR_CNT_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_patt_ins_reg_read( b_ptr,
                                          h_ptr,
                                          A,
                                          MEM_TYPE_STATUS,
                                          PMC_FC_CBR_PATT_INS_REG_RX_PN11_ERR_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_patt_ins_reg_RX_PN11_ERR_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 fc_cbr_patt_ins_reg_RX_PRBS31_ERR_CNT_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                 cbr_fsgm_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_reg_RX_PRBS31_ERR_CNT_read( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                 cbr_fsgm_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = fc_cbr_patt_ins_reg_read( b_ptr,
                                          h_ptr,
                                          A,
                                          MEM_TYPE_STATUS,
                                          PMC_FC_CBR_PATT_INS_REG_RX_PRBS31_ERR_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "fc_cbr_patt_ins_reg_RX_PRBS31_ERR_CNT_read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void fc_cbr_patt_ins_field_FAULT_SIZE_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_FAULT_SIZE_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_FAULT_SIZE_set", A, 3);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_FAULT_SIZE_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_FAULT_SIZE_set", A, value );

    /* (0x00000080 bits 26:24) bits 0:2 use field FAULT_SIZE of register PMC_FC_CBR_PATT_INS_REG_CFG */
    fc_cbr_patt_ins_reg_CFG_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_SIZE_MSK,
                                       FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_SIZE_OFF,
                                       value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_FAULT_SIZE_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                           cbr_fsgm_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_FAULT_SIZE_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                           cbr_fsgm_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_FAULT_SIZE_get", A, 3);
    /* (0x00000080 bits 26:24) bits 0:2 use field FAULT_SIZE of register PMC_FC_CBR_PATT_INS_REG_CFG */
    reg_value = fc_cbr_patt_ins_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_SIZE_MSK) >> FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_SIZE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_FAULT_SIZE_get", A, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_field_range_FAULT_SIZE_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_range_FAULT_SIZE_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_range_FAULT_SIZE_set", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_patt_ins_field_range_FAULT_SIZE_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_patt_ins_field_range_FAULT_SIZE_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_patt_ins_field_range_FAULT_SIZE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000080 bits 26:24) bits 0:2 use field FAULT_SIZE of register PMC_FC_CBR_PATT_INS_REG_CFG */
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
        /* (0x00000080 bits 26:24) bits 0:2 use field FAULT_SIZE of register PMC_FC_CBR_PATT_INS_REG_CFG */
        fc_cbr_patt_ins_reg_CFG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_SIZE_OFF + subfield_offset),
                                           FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_SIZE_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 fc_cbr_patt_ins_field_range_FAULT_SIZE_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                 cbr_fsgm_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_range_FAULT_SIZE_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                 cbr_fsgm_handle_t *h_ptr,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_range_FAULT_SIZE_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_patt_ins_field_range_FAULT_SIZE_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_patt_ins_field_range_FAULT_SIZE_get", stop_bit, 2 );
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
    /* (0x00000080 bits 26:24) bits 0:2 use field FAULT_SIZE of register PMC_FC_CBR_PATT_INS_REG_CFG */
    reg_value = fc_cbr_patt_ins_reg_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_SIZE_MSK)
                  >> FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_SIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_SIZE_MSK, FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_SIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_patt_ins_field_range_FAULT_SIZE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_field_FAULT_MODE_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_FAULT_MODE_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_FAULT_MODE_set", A, 3);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_FAULT_MODE_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_FAULT_MODE_set", A, value );

    /* (0x00000080 bits 21:20) bits 0:1 use field FAULT_MODE of register PMC_FC_CBR_PATT_INS_REG_CFG */
    fc_cbr_patt_ins_reg_CFG_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_MODE_MSK,
                                       FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_MODE_OFF,
                                       value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_FAULT_MODE_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                           cbr_fsgm_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_FAULT_MODE_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                           cbr_fsgm_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_FAULT_MODE_get", A, 3);
    /* (0x00000080 bits 21:20) bits 0:1 use field FAULT_MODE of register PMC_FC_CBR_PATT_INS_REG_CFG */
    reg_value = fc_cbr_patt_ins_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_MODE_MSK) >> FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_FAULT_MODE_get", A, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_field_range_FAULT_MODE_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_range_FAULT_MODE_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_range_FAULT_MODE_set", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_patt_ins_field_range_FAULT_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_patt_ins_field_range_FAULT_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_patt_ins_field_range_FAULT_MODE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000080 bits 21:20) bits 0:1 use field FAULT_MODE of register PMC_FC_CBR_PATT_INS_REG_CFG */
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
        /* (0x00000080 bits 21:20) bits 0:1 use field FAULT_MODE of register PMC_FC_CBR_PATT_INS_REG_CFG */
        fc_cbr_patt_ins_reg_CFG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_MODE_OFF + subfield_offset),
                                           FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_MODE_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 fc_cbr_patt_ins_field_range_FAULT_MODE_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                 cbr_fsgm_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_range_FAULT_MODE_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                 cbr_fsgm_handle_t *h_ptr,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_range_FAULT_MODE_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_patt_ins_field_range_FAULT_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_patt_ins_field_range_FAULT_MODE_get", stop_bit, 1 );
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
    /* (0x00000080 bits 21:20) bits 0:1 use field FAULT_MODE of register PMC_FC_CBR_PATT_INS_REG_CFG */
    reg_value = fc_cbr_patt_ins_reg_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_MODE_MSK)
                  >> FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_MODE_MSK, FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_patt_ins_field_range_FAULT_MODE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_field_BUS_WIDTH_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                        cbr_fsgm_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_BUS_WIDTH_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                        cbr_fsgm_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_BUS_WIDTH_set", A, 3);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_BUS_WIDTH_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_BUS_WIDTH_set", A, value );

    /* (0x00000080 bits 18:16) bits 0:2 use field BUS_WIDTH of register PMC_FC_CBR_PATT_INS_REG_CFG */
    fc_cbr_patt_ins_reg_CFG_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       FC_CBR_PATT_INS_REG_CFG_BIT_BUS_WIDTH_MSK,
                                       FC_CBR_PATT_INS_REG_CFG_BIT_BUS_WIDTH_OFF,
                                       value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_BUS_WIDTH_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                          cbr_fsgm_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_BUS_WIDTH_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                          cbr_fsgm_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_BUS_WIDTH_get", A, 3);
    /* (0x00000080 bits 18:16) bits 0:2 use field BUS_WIDTH of register PMC_FC_CBR_PATT_INS_REG_CFG */
    reg_value = fc_cbr_patt_ins_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_PATT_INS_REG_CFG_BIT_BUS_WIDTH_MSK) >> FC_CBR_PATT_INS_REG_CFG_BIT_BUS_WIDTH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_BUS_WIDTH_get", A, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_field_range_BUS_WIDTH_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_range_BUS_WIDTH_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_range_BUS_WIDTH_set", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_patt_ins_field_range_BUS_WIDTH_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_patt_ins_field_range_BUS_WIDTH_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_patt_ins_field_range_BUS_WIDTH_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000080 bits 18:16) bits 0:2 use field BUS_WIDTH of register PMC_FC_CBR_PATT_INS_REG_CFG */
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
        /* (0x00000080 bits 18:16) bits 0:2 use field BUS_WIDTH of register PMC_FC_CBR_PATT_INS_REG_CFG */
        fc_cbr_patt_ins_reg_CFG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (FC_CBR_PATT_INS_REG_CFG_BIT_BUS_WIDTH_OFF + subfield_offset),
                                           FC_CBR_PATT_INS_REG_CFG_BIT_BUS_WIDTH_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 fc_cbr_patt_ins_field_range_BUS_WIDTH_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                cbr_fsgm_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_range_BUS_WIDTH_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                cbr_fsgm_handle_t *h_ptr,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_range_BUS_WIDTH_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_patt_ins_field_range_BUS_WIDTH_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_patt_ins_field_range_BUS_WIDTH_get", stop_bit, 2 );
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
    /* (0x00000080 bits 18:16) bits 0:2 use field BUS_WIDTH of register PMC_FC_CBR_PATT_INS_REG_CFG */
    reg_value = fc_cbr_patt_ins_reg_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_CBR_PATT_INS_REG_CFG_BIT_BUS_WIDTH_MSK)
                  >> FC_CBR_PATT_INS_REG_CFG_BIT_BUS_WIDTH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_PATT_INS_REG_CFG_BIT_BUS_WIDTH_MSK, FC_CBR_PATT_INS_REG_CFG_BIT_BUS_WIDTH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_patt_ins_field_range_BUS_WIDTH_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_field_PN11_INV_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                       cbr_fsgm_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_PN11_INV_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                       cbr_fsgm_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_PN11_INV_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_PN11_INV_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_PN11_INV_set", A, value );

    /* (0x00000080 bits 12) field PN11_INV of register PMC_FC_CBR_PATT_INS_REG_CFG */
    fc_cbr_patt_ins_reg_CFG_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       FC_CBR_PATT_INS_REG_CFG_BIT_PN11_INV_MSK,
                                       FC_CBR_PATT_INS_REG_CFG_BIT_PN11_INV_OFF,
                                       value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_PN11_INV_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_PN11_INV_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_PN11_INV_get", A, 3);
    /* (0x00000080 bits 12) field PN11_INV of register PMC_FC_CBR_PATT_INS_REG_CFG */
    reg_value = fc_cbr_patt_ins_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_PATT_INS_REG_CFG_BIT_PN11_INV_MSK) >> FC_CBR_PATT_INS_REG_CFG_BIT_PN11_INV_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_PN11_INV_get", A, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_field_SCRAM_EN_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                       cbr_fsgm_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_SCRAM_EN_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                       cbr_fsgm_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_SCRAM_EN_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_SCRAM_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_SCRAM_EN_set", A, value );

    /* (0x00000080 bits 8) field SCRAM_EN of register PMC_FC_CBR_PATT_INS_REG_CFG */
    fc_cbr_patt_ins_reg_CFG_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       FC_CBR_PATT_INS_REG_CFG_BIT_SCRAM_EN_MSK,
                                       FC_CBR_PATT_INS_REG_CFG_BIT_SCRAM_EN_OFF,
                                       value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_SCRAM_EN_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_SCRAM_EN_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_SCRAM_EN_get", A, 3);
    /* (0x00000080 bits 8) field SCRAM_EN of register PMC_FC_CBR_PATT_INS_REG_CFG */
    reg_value = fc_cbr_patt_ins_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_PATT_INS_REG_CFG_BIT_SCRAM_EN_MSK) >> FC_CBR_PATT_INS_REG_CFG_BIT_SCRAM_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_SCRAM_EN_get", A, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_field_ZONE_LEVEL_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_ZONE_LEVEL_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_ZONE_LEVEL_set", A, 3);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_ZONE_LEVEL_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_ZONE_LEVEL_set", A, value );

    /* (0x00000080 bits 6:5) bits 0:1 use field ZONE_LEVEL of register PMC_FC_CBR_PATT_INS_REG_CFG */
    fc_cbr_patt_ins_reg_CFG_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       FC_CBR_PATT_INS_REG_CFG_BIT_ZONE_LEVEL_MSK,
                                       FC_CBR_PATT_INS_REG_CFG_BIT_ZONE_LEVEL_OFF,
                                       value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_ZONE_LEVEL_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                           cbr_fsgm_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_ZONE_LEVEL_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                           cbr_fsgm_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_ZONE_LEVEL_get", A, 3);
    /* (0x00000080 bits 6:5) bits 0:1 use field ZONE_LEVEL of register PMC_FC_CBR_PATT_INS_REG_CFG */
    reg_value = fc_cbr_patt_ins_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_PATT_INS_REG_CFG_BIT_ZONE_LEVEL_MSK) >> FC_CBR_PATT_INS_REG_CFG_BIT_ZONE_LEVEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_ZONE_LEVEL_get", A, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_field_range_ZONE_LEVEL_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_range_ZONE_LEVEL_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_range_ZONE_LEVEL_set", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_patt_ins_field_range_ZONE_LEVEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_patt_ins_field_range_ZONE_LEVEL_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_patt_ins_field_range_ZONE_LEVEL_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000080 bits 6:5) bits 0:1 use field ZONE_LEVEL of register PMC_FC_CBR_PATT_INS_REG_CFG */
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
        /* (0x00000080 bits 6:5) bits 0:1 use field ZONE_LEVEL of register PMC_FC_CBR_PATT_INS_REG_CFG */
        fc_cbr_patt_ins_reg_CFG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (FC_CBR_PATT_INS_REG_CFG_BIT_ZONE_LEVEL_OFF + subfield_offset),
                                           FC_CBR_PATT_INS_REG_CFG_BIT_ZONE_LEVEL_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 fc_cbr_patt_ins_field_range_ZONE_LEVEL_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                 cbr_fsgm_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_range_ZONE_LEVEL_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                 cbr_fsgm_handle_t *h_ptr,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_range_ZONE_LEVEL_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_patt_ins_field_range_ZONE_LEVEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_patt_ins_field_range_ZONE_LEVEL_get", stop_bit, 1 );
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
    /* (0x00000080 bits 6:5) bits 0:1 use field ZONE_LEVEL of register PMC_FC_CBR_PATT_INS_REG_CFG */
    reg_value = fc_cbr_patt_ins_reg_CFG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & FC_CBR_PATT_INS_REG_CFG_BIT_ZONE_LEVEL_MSK)
                  >> FC_CBR_PATT_INS_REG_CFG_BIT_ZONE_LEVEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_PATT_INS_REG_CFG_BIT_ZONE_LEVEL_MSK, FC_CBR_PATT_INS_REG_CFG_BIT_ZONE_LEVEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_patt_ins_field_range_ZONE_LEVEL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_field_FORCE_FAULT_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                          cbr_fsgm_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_FORCE_FAULT_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                          cbr_fsgm_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_FORCE_FAULT_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_FORCE_FAULT_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_FORCE_FAULT_set", A, value );

    /* (0x00000080 bits 4) field FORCE_FAULT of register PMC_FC_CBR_PATT_INS_REG_CFG */
    fc_cbr_patt_ins_reg_CFG_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       FC_CBR_PATT_INS_REG_CFG_BIT_FORCE_FAULT_MSK,
                                       FC_CBR_PATT_INS_REG_CFG_BIT_FORCE_FAULT_OFF,
                                       value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_FORCE_FAULT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                            cbr_fsgm_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_FORCE_FAULT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                            cbr_fsgm_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_FORCE_FAULT_get", A, 3);
    /* (0x00000080 bits 4) field FORCE_FAULT of register PMC_FC_CBR_PATT_INS_REG_CFG */
    reg_value = fc_cbr_patt_ins_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_PATT_INS_REG_CFG_BIT_FORCE_FAULT_MSK) >> FC_CBR_PATT_INS_REG_CFG_BIT_FORCE_FAULT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_FORCE_FAULT_get", A, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_field_FC_CBR_PATT_INS_EN_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                 cbr_fsgm_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_FC_CBR_PATT_INS_EN_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                 cbr_fsgm_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_FC_CBR_PATT_INS_EN_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_FC_CBR_PATT_INS_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_FC_CBR_PATT_INS_EN_set", A, value );

    /* (0x00000080 bits 1) field FC_CBR_PATT_INS_EN of register PMC_FC_CBR_PATT_INS_REG_CFG */
    fc_cbr_patt_ins_reg_CFG_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       FC_CBR_PATT_INS_REG_CFG_BIT_FC_CBR_PATT_INS_EN_MSK,
                                       FC_CBR_PATT_INS_REG_CFG_BIT_FC_CBR_PATT_INS_EN_OFF,
                                       value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_FC_CBR_PATT_INS_EN_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_FC_CBR_PATT_INS_EN_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_FC_CBR_PATT_INS_EN_get", A, 3);
    /* (0x00000080 bits 1) field FC_CBR_PATT_INS_EN of register PMC_FC_CBR_PATT_INS_REG_CFG */
    reg_value = fc_cbr_patt_ins_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_PATT_INS_REG_CFG_BIT_FC_CBR_PATT_INS_EN_MSK) >> FC_CBR_PATT_INS_REG_CFG_BIT_FC_CBR_PATT_INS_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_FC_CBR_PATT_INS_EN_get", A, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_field_SOFT_RST_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                       cbr_fsgm_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_SOFT_RST_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                       cbr_fsgm_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_SOFT_RST_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_SOFT_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_SOFT_RST_set", A, value );

    /* (0x00000080 bits 0) field SOFT_RST of register PMC_FC_CBR_PATT_INS_REG_CFG */
    fc_cbr_patt_ins_reg_CFG_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       FC_CBR_PATT_INS_REG_CFG_BIT_SOFT_RST_MSK,
                                       FC_CBR_PATT_INS_REG_CFG_BIT_SOFT_RST_OFF,
                                       value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_SOFT_RST_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_SOFT_RST_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_SOFT_RST_get", A, 3);
    /* (0x00000080 bits 0) field SOFT_RST of register PMC_FC_CBR_PATT_INS_REG_CFG */
    reg_value = fc_cbr_patt_ins_reg_CFG_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_PATT_INS_REG_CFG_BIT_SOFT_RST_MSK) >> FC_CBR_PATT_INS_REG_CFG_BIT_SOFT_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_SOFT_RST_get", A, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_lfield_USER_FLT_PATT_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                             cbr_fsgm_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value[5] ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_lfield_USER_FLT_PATT_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                             cbr_fsgm_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value[5] )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_lfield_USER_FLT_PATT_set", A, 3);
    IOLOG( "%s <= A=%d 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x", "fc_cbr_patt_ins_lfield_USER_FLT_PATT_set", A, value[4] , value[3] , value[2] , value[1] , value[0] );

    /* (0x00000088 bits 31:0) bits 0:31 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0 */
    fc_cbr_patt_ins_reg_USER_FLT_PATT_31_0_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0_BIT_USER_FLT_PATT_MSK,
                                                      FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0_BIT_USER_FLT_PATT_OFF,
                                                      value[0]);

    /* (0x0000008c bits 31:0) bits 32:63 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32 */
    fc_cbr_patt_ins_reg_USER_FLT_PATT_63_32_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32_BIT_USER_FLT_PATT_MSK,
                                                       FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32_BIT_USER_FLT_PATT_OFF,
                                                       value[1]);

    /* (0x00000090 bits 31:0) bits 64:95 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64 */
    fc_cbr_patt_ins_reg_USER_FLT_PATT_95_64_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64_BIT_USER_FLT_PATT_MSK,
                                                       FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64_BIT_USER_FLT_PATT_OFF,
                                                       value[2]);

    /* (0x00000094 bits 31:0) bits 96:127 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96 */
    fc_cbr_patt_ins_reg_USER_FLT_PATT_127_96_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96_BIT_USER_FLT_PATT_MSK,
                                                        FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96_BIT_USER_FLT_PATT_OFF,
                                                        value[3]);

    /* (0x00000098 bits 31:0) bits 128:159 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128 */
    fc_cbr_patt_ins_reg_USER_FLT_PATTERN_159_128_field_set( b_ptr,
                                                            h_ptr,
                                                            A,
                                                            FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128_BIT_USER_FLT_PATT_MSK,
                                                            FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128_BIT_USER_FLT_PATT_OFF,
                                                            value[4]);
}

static INLINE void fc_cbr_patt_ins_lfield_USER_FLT_PATT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                             cbr_fsgm_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value[5] ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_lfield_USER_FLT_PATT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                             cbr_fsgm_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value[5] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_lfield_USER_FLT_PATT_get", A, 3);
    for (i=0;i<5;i++)
        value[i] = 0;

    /* (0x00000088 bits 31:0) bits 0:31 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0 */
    reg_value = fc_cbr_patt_ins_reg_USER_FLT_PATT_31_0_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0_BIT_USER_FLT_PATT_MSK) >> FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0_BIT_USER_FLT_PATT_OFF;
    value[0] |= field_value;

    /* (0x0000008c bits 31:0) bits 32:63 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32 */
    reg_value = fc_cbr_patt_ins_reg_USER_FLT_PATT_63_32_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32_BIT_USER_FLT_PATT_MSK) >> FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32_BIT_USER_FLT_PATT_OFF;
    value[1] |= field_value;

    /* (0x00000090 bits 31:0) bits 64:95 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64 */
    reg_value = fc_cbr_patt_ins_reg_USER_FLT_PATT_95_64_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64_BIT_USER_FLT_PATT_MSK) >> FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64_BIT_USER_FLT_PATT_OFF;
    value[2] |= field_value;

    /* (0x00000094 bits 31:0) bits 96:127 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96 */
    reg_value = fc_cbr_patt_ins_reg_USER_FLT_PATT_127_96_read( b_ptr,
                                                               h_ptr,
                                                               A);
    field_value = (reg_value & FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96_BIT_USER_FLT_PATT_MSK) >> FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96_BIT_USER_FLT_PATT_OFF;
    value[3] |= field_value;

    /* (0x00000098 bits 31:0) bits 128:159 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128 */
    reg_value = fc_cbr_patt_ins_reg_USER_FLT_PATTERN_159_128_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    field_value = (reg_value & FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128_BIT_USER_FLT_PATT_MSK) >> FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128_BIT_USER_FLT_PATT_OFF;
    value[4] |= field_value;

    IOLOG( "%s -> A=%d 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x", "fc_cbr_patt_ins_lfield_USER_FLT_PATT_get", A, value[4] , value[3] , value[2] , value[1] , value[0] );

}
static INLINE void fc_cbr_patt_ins_lfield_range_USER_FLT_PATT_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_lfield_range_USER_FLT_PATT_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_lfield_range_USER_FLT_PATT_set", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_patt_ins_lfield_range_USER_FLT_PATT_set", stop_bit, start_bit );
    if (stop_bit > 159) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_patt_ins_lfield_range_USER_FLT_PATT_set", stop_bit, 159 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_patt_ins_lfield_range_USER_FLT_PATT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000088 bits 31:0) bits 0:31 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0 */
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
        /* (0x00000088 bits 31:0) bits 0:31 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0 */
        fc_cbr_patt_ins_reg_USER_FLT_PATT_31_0_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          subfield_mask << (FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0_BIT_USER_FLT_PATT_OFF + subfield_offset),
                                                          FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0_BIT_USER_FLT_PATT_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000008c bits 31:0) bits 32:63 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32 */
    if ((start_bit <= 63) && (stop_bit >= 32)) {
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
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000008c bits 31:0) bits 32:63 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32 */
        fc_cbr_patt_ins_reg_USER_FLT_PATT_63_32_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32_BIT_USER_FLT_PATT_OFF + subfield_offset),
                                                           FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32_BIT_USER_FLT_PATT_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000090 bits 31:0) bits 64:95 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64 */
    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 64 - start_bit;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000090 bits 31:0) bits 64:95 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64 */
        fc_cbr_patt_ins_reg_USER_FLT_PATT_95_64_field_set( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           subfield_mask << (FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64_BIT_USER_FLT_PATT_OFF + subfield_offset),
                                                           FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64_BIT_USER_FLT_PATT_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000094 bits 31:0) bits 96:127 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96 */
    if ((start_bit <= 127) && (stop_bit >= 96)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 96) {
            subfield_offset = start_bit - 96;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 96 - start_bit;
            subfield_start_bit = 96;
        }
        if (stop_bit < 127) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 127;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000094 bits 31:0) bits 96:127 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96 */
        fc_cbr_patt_ins_reg_USER_FLT_PATT_127_96_field_set( b_ptr,
                                                            h_ptr,
                                                            A,
                                                            subfield_mask << (FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96_BIT_USER_FLT_PATT_OFF + subfield_offset),
                                                            FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96_BIT_USER_FLT_PATT_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000098 bits 31:0) bits 128:159 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128 */
    if ((start_bit <= 159) && (stop_bit >= 128)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 128) {
            subfield_offset = start_bit - 128;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 128 - start_bit;
            subfield_start_bit = 128;
        }
        if (stop_bit < 159) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 159;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000098 bits 31:0) bits 128:159 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128 */
        fc_cbr_patt_ins_reg_USER_FLT_PATTERN_159_128_field_set( b_ptr,
                                                                h_ptr,
                                                                A,
                                                                subfield_mask << (FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128_BIT_USER_FLT_PATT_OFF + subfield_offset),
                                                                FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128_BIT_USER_FLT_PATT_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 fc_cbr_patt_ins_lfield_range_USER_FLT_PATT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                     cbr_fsgm_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_lfield_range_USER_FLT_PATT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                     cbr_fsgm_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_lfield_range_USER_FLT_PATT_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_patt_ins_lfield_range_USER_FLT_PATT_get", stop_bit, start_bit );
    if (stop_bit > 159) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_patt_ins_lfield_range_USER_FLT_PATT_get", stop_bit, 159 );
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
    /* (0x00000088 bits 31:0) bits 0:31 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0 */
        reg_value = fc_cbr_patt_ins_reg_USER_FLT_PATT_31_0_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
        field_value = (reg_value & FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0_BIT_USER_FLT_PATT_MSK)
                      >> FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0_BIT_USER_FLT_PATT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0_BIT_USER_FLT_PATT_MSK, FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0_BIT_USER_FLT_PATT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 63) && (stop_bit >= 32)) {
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
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000008c bits 31:0) bits 32:63 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32 */
        reg_value = fc_cbr_patt_ins_reg_USER_FLT_PATT_63_32_read( b_ptr,
                                                                  h_ptr,
                                                                  A);
        field_value = (reg_value & FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32_BIT_USER_FLT_PATT_MSK)
                      >> FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32_BIT_USER_FLT_PATT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32_BIT_USER_FLT_PATT_MSK, FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32_BIT_USER_FLT_PATT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 64 - start_bit;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000090 bits 31:0) bits 64:95 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64 */
        reg_value = fc_cbr_patt_ins_reg_USER_FLT_PATT_95_64_read( b_ptr,
                                                                  h_ptr,
                                                                  A);
        field_value = (reg_value & FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64_BIT_USER_FLT_PATT_MSK)
                      >> FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64_BIT_USER_FLT_PATT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64_BIT_USER_FLT_PATT_MSK, FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64_BIT_USER_FLT_PATT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 127) && (stop_bit >= 96)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 96) {
            subfield_offset = start_bit - 96;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 96 - start_bit;
            subfield_start_bit = 96;
        }
        if (stop_bit < 127) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 127;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000094 bits 31:0) bits 96:127 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96 */
        reg_value = fc_cbr_patt_ins_reg_USER_FLT_PATT_127_96_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
        field_value = (reg_value & FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96_BIT_USER_FLT_PATT_MSK)
                      >> FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96_BIT_USER_FLT_PATT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96_BIT_USER_FLT_PATT_MSK, FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96_BIT_USER_FLT_PATT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 159) && (stop_bit >= 128)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 128) {
            subfield_offset = start_bit - 128;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 128 - start_bit;
            subfield_start_bit = 128;
        }
        if (stop_bit < 159) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 159;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000098 bits 31:0) bits 128:159 use field USER_FLT_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128 */
        reg_value = fc_cbr_patt_ins_reg_USER_FLT_PATTERN_159_128_read( b_ptr,
                                                                       h_ptr,
                                                                       A);
        field_value = (reg_value & FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128_BIT_USER_FLT_PATT_MSK)
                      >> FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128_BIT_USER_FLT_PATT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128_BIT_USER_FLT_PATT_MSK, FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128_BIT_USER_FLT_PATT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_patt_ins_lfield_range_USER_FLT_PATT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_lfield_USER_RCVRY_PATT_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_lfield_USER_RCVRY_PATT_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value[2] )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_lfield_USER_RCVRY_PATT_set", A, 3);
    IOLOG( "%s <= A=%d 0x%08x 0x%08x", "fc_cbr_patt_ins_lfield_USER_RCVRY_PATT_set", A, value[1] , value[0] );

    /* (0x0000009c bits 31:0) bits 0:31 use field USER_RCVRY_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0 */
    fc_cbr_patt_ins_reg_USER_RCVRY_PATT_31_0_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0_BIT_USER_RCVRY_PATT_MSK,
                                                        FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0_BIT_USER_RCVRY_PATT_OFF,
                                                        value[0]);

    /* (0x000000a0 bits 7:0) bits 32:39 use field USER_RCVRY_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32 */
    fc_cbr_patt_ins_reg_USER_RCVRY_PATT_39_32_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32_BIT_USER_RCVRY_PATT_MSK,
                                                         FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32_BIT_USER_RCVRY_PATT_OFF,
                                                         value[1]);
}

static INLINE void fc_cbr_patt_ins_lfield_USER_RCVRY_PATT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_lfield_USER_RCVRY_PATT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_lfield_USER_RCVRY_PATT_get", A, 3);
    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x0000009c bits 31:0) bits 0:31 use field USER_RCVRY_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0 */
    reg_value = fc_cbr_patt_ins_reg_USER_RCVRY_PATT_31_0_read( b_ptr,
                                                               h_ptr,
                                                               A);
    field_value = (reg_value & FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0_BIT_USER_RCVRY_PATT_MSK) >> FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0_BIT_USER_RCVRY_PATT_OFF;
    value[0] |= field_value;

    /* (0x000000a0 bits 7:0) bits 32:39 use field USER_RCVRY_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32 */
    reg_value = fc_cbr_patt_ins_reg_USER_RCVRY_PATT_39_32_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32_BIT_USER_RCVRY_PATT_MSK) >> FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32_BIT_USER_RCVRY_PATT_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> A=%d 0x%08x 0x%08x", "fc_cbr_patt_ins_lfield_USER_RCVRY_PATT_get", A, value[1] , value[0] );

}
static INLINE void fc_cbr_patt_ins_lfield_range_USER_RCVRY_PATT_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                     cbr_fsgm_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_lfield_range_USER_RCVRY_PATT_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                     cbr_fsgm_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_lfield_range_USER_RCVRY_PATT_set", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_patt_ins_lfield_range_USER_RCVRY_PATT_set", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_patt_ins_lfield_range_USER_RCVRY_PATT_set", stop_bit, 39 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_patt_ins_lfield_range_USER_RCVRY_PATT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000009c bits 31:0) bits 0:31 use field USER_RCVRY_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0 */
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
        /* (0x0000009c bits 31:0) bits 0:31 use field USER_RCVRY_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0 */
        fc_cbr_patt_ins_reg_USER_RCVRY_PATT_31_0_field_set( b_ptr,
                                                            h_ptr,
                                                            A,
                                                            subfield_mask << (FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0_BIT_USER_RCVRY_PATT_OFF + subfield_offset),
                                                            FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0_BIT_USER_RCVRY_PATT_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000000a0 bits 7:0) bits 32:39 use field USER_RCVRY_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32 */
    if ((start_bit <= 39) && (stop_bit >= 32)) {
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
        if (stop_bit < 39) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 39;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000000a0 bits 7:0) bits 32:39 use field USER_RCVRY_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32 */
        fc_cbr_patt_ins_reg_USER_RCVRY_PATT_39_32_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32_BIT_USER_RCVRY_PATT_OFF + subfield_offset),
                                                             FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32_BIT_USER_RCVRY_PATT_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 fc_cbr_patt_ins_lfield_range_USER_RCVRY_PATT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_lfield_range_USER_RCVRY_PATT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_lfield_range_USER_RCVRY_PATT_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_patt_ins_lfield_range_USER_RCVRY_PATT_get", stop_bit, start_bit );
    if (stop_bit > 39) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_patt_ins_lfield_range_USER_RCVRY_PATT_get", stop_bit, 39 );
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
    /* (0x0000009c bits 31:0) bits 0:31 use field USER_RCVRY_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0 */
        reg_value = fc_cbr_patt_ins_reg_USER_RCVRY_PATT_31_0_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
        field_value = (reg_value & FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0_BIT_USER_RCVRY_PATT_MSK)
                      >> FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0_BIT_USER_RCVRY_PATT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0_BIT_USER_RCVRY_PATT_MSK, FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0_BIT_USER_RCVRY_PATT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 39) && (stop_bit >= 32)) {
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
        if (stop_bit < 39) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 39;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x000000a0 bits 7:0) bits 32:39 use field USER_RCVRY_PATT of register PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32 */
        reg_value = fc_cbr_patt_ins_reg_USER_RCVRY_PATT_39_32_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
        field_value = (reg_value & FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32_BIT_USER_RCVRY_PATT_MSK)
                      >> FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32_BIT_USER_RCVRY_PATT_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32_BIT_USER_RCVRY_PATT_MSK, FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32_BIT_USER_RCVRY_PATT_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_patt_ins_lfield_range_USER_RCVRY_PATT_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void fc_cbr_patt_ins_field_UPDATE_PMON_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                          cbr_fsgm_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_UPDATE_PMON_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                          cbr_fsgm_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_UPDATE_PMON_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_UPDATE_PMON_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_UPDATE_PMON_set", A, value );

    /* (0x00000084 bits 4) field UPDATE_PMON of register PMC_FC_CBR_PATT_INS_REG_UPDATE */
    fc_cbr_patt_ins_reg_UPDATE_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          FC_CBR_PATT_INS_REG_UPDATE_BIT_UPDATE_PMON_MSK,
                                          FC_CBR_PATT_INS_REG_UPDATE_BIT_UPDATE_PMON_OFF,
                                          value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_UPDATE_PMON_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                            cbr_fsgm_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_UPDATE_PMON_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                            cbr_fsgm_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_UPDATE_PMON_get", A, 3);
    /* (0x00000084 bits 4) field UPDATE_PMON of register PMC_FC_CBR_PATT_INS_REG_UPDATE */
    reg_value = fc_cbr_patt_ins_reg_UPDATE_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_PATT_INS_REG_UPDATE_BIT_UPDATE_PMON_MSK) >> FC_CBR_PATT_INS_REG_UPDATE_BIT_UPDATE_PMON_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_UPDATE_PMON_get", A, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_field_UPDATE_USER_PATT_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_UPDATE_USER_PATT_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_UPDATE_USER_PATT_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_UPDATE_USER_PATT_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_UPDATE_USER_PATT_set", A, value );

    /* (0x00000084 bits 0) field UPDATE_USER_PATT of register PMC_FC_CBR_PATT_INS_REG_UPDATE */
    fc_cbr_patt_ins_reg_UPDATE_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          FC_CBR_PATT_INS_REG_UPDATE_BIT_UPDATE_USER_PATT_MSK,
                                          FC_CBR_PATT_INS_REG_UPDATE_BIT_UPDATE_USER_PATT_OFF,
                                          value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_UPDATE_USER_PATT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                 cbr_fsgm_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_UPDATE_USER_PATT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                 cbr_fsgm_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_UPDATE_USER_PATT_get", A, 3);
    /* (0x00000084 bits 0) field UPDATE_USER_PATT of register PMC_FC_CBR_PATT_INS_REG_UPDATE */
    reg_value = fc_cbr_patt_ins_reg_UPDATE_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_PATT_INS_REG_UPDATE_BIT_UPDATE_USER_PATT_MSK) >> FC_CBR_PATT_INS_REG_UPDATE_BIT_UPDATE_USER_PATT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_UPDATE_USER_PATT_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_both
 * ==================================================================================
 */
static INLINE void fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_I_set_to_clear( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                                     cbr_fsgm_handle_t *h_ptr,
                                                                                     UINT32 A,
                                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_I_set_to_clear( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                                     cbr_fsgm_handle_t *h_ptr,
                                                                                     UINT32 A,
                                                                                     UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_I_set_to_clear", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_I_set_to_clear", A, value );

    /* (0x000000a8 bits 24) field RX_PRBS31_ALL_ONES_OR_ZEROS_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    fc_cbr_patt_ins_reg_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  A,
                                                                  FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_I_MSK,
                                                                  FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_I_OFF,
                                                                  value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_I_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                              cbr_fsgm_handle_t *h_ptr,
                                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_I_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                              cbr_fsgm_handle_t *h_ptr,
                                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_I_get", A, 3);
    /* (0x000000a8 bits 24) field RX_PRBS31_ALL_ONES_OR_ZEROS_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    reg_value = fc_cbr_patt_ins_reg_INT_INDICATION_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_I_MSK) >> FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_I_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                                             cbr_fsgm_handle_t *h_ptr,
                                                                                             UINT32 A,
                                                                                             UINT32 value,
                                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                                             UINT32 max_count,
                                                                                             UINT32 *num_failed_polls,
                                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_I_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                                             cbr_fsgm_handle_t *h_ptr,
                                                                                             UINT32 A,
                                                                                             UINT32 value,
                                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                                             UINT32 max_count,
                                                                                             UINT32 *num_failed_polls,
                                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_I_poll", A, value );

    /* (0x000000a8 bits 24) field RX_PRBS31_ALL_ONES_OR_ZEROS_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    return fc_cbr_patt_ins_reg_INT_INDICATION_poll( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_I_MSK,
                                                    (value<<FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_I_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE void fc_cbr_patt_ins_field_RX_PN11_SYNC_I_set_to_clear( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                      cbr_fsgm_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_RX_PN11_SYNC_I_set_to_clear( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                      cbr_fsgm_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PN11_SYNC_I_set_to_clear", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_RX_PN11_SYNC_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PN11_SYNC_I_set_to_clear", A, value );

    /* (0x000000a8 bits 8) field RX_PN11_SYNC_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    fc_cbr_patt_ins_reg_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  A,
                                                                  FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PN11_SYNC_I_MSK,
                                                                  FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PN11_SYNC_I_OFF,
                                                                  value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_RX_PN11_SYNC_I_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_RX_PN11_SYNC_I_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PN11_SYNC_I_get", A, 3);
    /* (0x000000a8 bits 8) field RX_PN11_SYNC_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    reg_value = fc_cbr_patt_ins_reg_INT_INDICATION_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PN11_SYNC_I_MSK) >> FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PN11_SYNC_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PN11_SYNC_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_RX_PN11_SYNC_I_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                              cbr_fsgm_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_RX_PN11_SYNC_I_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                              cbr_fsgm_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PN11_SYNC_I_poll", A, value );

    /* (0x000000a8 bits 8) field RX_PN11_SYNC_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    return fc_cbr_patt_ins_reg_INT_INDICATION_poll( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PN11_SYNC_I_MSK,
                                                    (value<<FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PN11_SYNC_I_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE void fc_cbr_patt_ins_field_TX_PN11_SYNC_I_set_to_clear( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                      cbr_fsgm_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_TX_PN11_SYNC_I_set_to_clear( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                      cbr_fsgm_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_TX_PN11_SYNC_I_set_to_clear", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_TX_PN11_SYNC_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_TX_PN11_SYNC_I_set_to_clear", A, value );

    /* (0x000000a8 bits 4) field TX_PN11_SYNC_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    fc_cbr_patt_ins_reg_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  A,
                                                                  FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_TX_PN11_SYNC_I_MSK,
                                                                  FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_TX_PN11_SYNC_I_OFF,
                                                                  value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_TX_PN11_SYNC_I_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_TX_PN11_SYNC_I_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_TX_PN11_SYNC_I_get", A, 3);
    /* (0x000000a8 bits 4) field TX_PN11_SYNC_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    reg_value = fc_cbr_patt_ins_reg_INT_INDICATION_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_TX_PN11_SYNC_I_MSK) >> FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_TX_PN11_SYNC_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_TX_PN11_SYNC_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_TX_PN11_SYNC_I_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                              cbr_fsgm_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_TX_PN11_SYNC_I_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                              cbr_fsgm_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_patt_ins_field_TX_PN11_SYNC_I_poll", A, value );

    /* (0x000000a8 bits 4) field TX_PN11_SYNC_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    return fc_cbr_patt_ins_reg_INT_INDICATION_poll( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_TX_PN11_SYNC_I_MSK,
                                                    (value<<FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_TX_PN11_SYNC_I_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE void fc_cbr_patt_ins_field_UNDERRUN_I_set_to_clear( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                  cbr_fsgm_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_UNDERRUN_I_set_to_clear( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                  cbr_fsgm_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_UNDERRUN_I_set_to_clear", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_UNDERRUN_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_UNDERRUN_I_set_to_clear", A, value );

    /* (0x000000a8 bits 0) field UNDERRUN_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    fc_cbr_patt_ins_reg_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  A,
                                                                  FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_UNDERRUN_I_MSK,
                                                                  FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_UNDERRUN_I_OFF,
                                                                  value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_UNDERRUN_I_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                           cbr_fsgm_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_UNDERRUN_I_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                           cbr_fsgm_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_UNDERRUN_I_get", A, 3);
    /* (0x000000a8 bits 0) field UNDERRUN_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    reg_value = fc_cbr_patt_ins_reg_INT_INDICATION_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_UNDERRUN_I_MSK) >> FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_UNDERRUN_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_UNDERRUN_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_UNDERRUN_I_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                          cbr_fsgm_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_UNDERRUN_I_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                          cbr_fsgm_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_patt_ins_field_UNDERRUN_I_poll", A, value );

    /* (0x000000a8 bits 0) field UNDERRUN_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    return fc_cbr_patt_ins_reg_INT_INDICATION_poll( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_UNDERRUN_I_MSK,
                                                    (value<<FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_UNDERRUN_I_OFF),
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
static INLINE void fc_cbr_patt_ins_field_RX_PRBS31_ERR_I_set_to_clear( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_RX_PRBS31_ERR_I_set_to_clear( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PRBS31_ERR_I_set_to_clear", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_RX_PRBS31_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PRBS31_ERR_I_set_to_clear", A, value );

    /* (0x000000a8 bits 20) field RX_PRBS31_ERR_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    fc_cbr_patt_ins_reg_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  A,
                                                                  FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PRBS31_ERR_I_MSK,
                                                                  FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PRBS31_ERR_I_OFF,
                                                                  value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_RX_PRBS31_ERR_I_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                cbr_fsgm_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_RX_PRBS31_ERR_I_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                cbr_fsgm_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PRBS31_ERR_I_get", A, 3);
    /* (0x000000a8 bits 20) field RX_PRBS31_ERR_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    reg_value = fc_cbr_patt_ins_reg_INT_INDICATION_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PRBS31_ERR_I_MSK) >> FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PRBS31_ERR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PRBS31_ERR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_RX_PRBS31_ERR_I_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                               cbr_fsgm_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_RX_PRBS31_ERR_I_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                               cbr_fsgm_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PRBS31_ERR_I_poll", A, value );

    /* (0x000000a8 bits 20) field RX_PRBS31_ERR_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    return fc_cbr_patt_ins_reg_INT_INDICATION_poll( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PRBS31_ERR_I_MSK,
                                                    (value<<FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PRBS31_ERR_I_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE void fc_cbr_patt_ins_field_RX_PN11_ERR_I_set_to_clear( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                     cbr_fsgm_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_RX_PN11_ERR_I_set_to_clear( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                     cbr_fsgm_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PN11_ERR_I_set_to_clear", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_RX_PN11_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PN11_ERR_I_set_to_clear", A, value );

    /* (0x000000a8 bits 16) field RX_PN11_ERR_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    fc_cbr_patt_ins_reg_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  A,
                                                                  FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PN11_ERR_I_MSK,
                                                                  FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PN11_ERR_I_OFF,
                                                                  value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_RX_PN11_ERR_I_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_RX_PN11_ERR_I_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PN11_ERR_I_get", A, 3);
    /* (0x000000a8 bits 16) field RX_PN11_ERR_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    reg_value = fc_cbr_patt_ins_reg_INT_INDICATION_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PN11_ERR_I_MSK) >> FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PN11_ERR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PN11_ERR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_RX_PN11_ERR_I_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                             cbr_fsgm_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_RX_PN11_ERR_I_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                             cbr_fsgm_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PN11_ERR_I_poll", A, value );

    /* (0x000000a8 bits 16) field RX_PN11_ERR_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    return fc_cbr_patt_ins_reg_INT_INDICATION_poll( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PN11_ERR_I_MSK,
                                                    (value<<FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PN11_ERR_I_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE void fc_cbr_patt_ins_field_TX_PN11_ERR_I_set_to_clear( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                     cbr_fsgm_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_TX_PN11_ERR_I_set_to_clear( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                     cbr_fsgm_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_TX_PN11_ERR_I_set_to_clear", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_TX_PN11_ERR_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_TX_PN11_ERR_I_set_to_clear", A, value );

    /* (0x000000a8 bits 12) field TX_PN11_ERR_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    fc_cbr_patt_ins_reg_INT_INDICATION_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  A,
                                                                  FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_TX_PN11_ERR_I_MSK,
                                                                  FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_TX_PN11_ERR_I_OFF,
                                                                  value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_TX_PN11_ERR_I_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_TX_PN11_ERR_I_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_TX_PN11_ERR_I_get", A, 3);
    /* (0x000000a8 bits 12) field TX_PN11_ERR_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    reg_value = fc_cbr_patt_ins_reg_INT_INDICATION_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_TX_PN11_ERR_I_MSK) >> FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_TX_PN11_ERR_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_TX_PN11_ERR_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_TX_PN11_ERR_I_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                             cbr_fsgm_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_TX_PN11_ERR_I_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                             cbr_fsgm_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_patt_ins_field_TX_PN11_ERR_I_poll", A, value );

    /* (0x000000a8 bits 12) field TX_PN11_ERR_I of register PMC_FC_CBR_PATT_INS_REG_INT_INDICATION */
    return fc_cbr_patt_ins_reg_INT_INDICATION_poll( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_TX_PN11_ERR_I_MSK,
                                                    (value<<FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_TX_PN11_ERR_I_OFF),
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
static INLINE void fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_E_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                            cbr_fsgm_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_E_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                            cbr_fsgm_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_E_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_E_set", A, value );

    /* (0x000000a4 bits 24) field RX_PRBS31_ALL_ONES_OR_ZEROS_E of register PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA */
    fc_cbr_patt_ins_reg_INTERRUPT_ENA_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_E_MSK,
                                                 FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_E_OFF,
                                                 value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_E_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                              cbr_fsgm_handle_t *h_ptr,
                                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_E_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                              cbr_fsgm_handle_t *h_ptr,
                                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_E_get", A, 3);
    /* (0x000000a4 bits 24) field RX_PRBS31_ALL_ONES_OR_ZEROS_E of register PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA */
    reg_value = fc_cbr_patt_ins_reg_INTERRUPT_ENA_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_E_MSK) >> FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_E_get", A, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_field_RX_PRBS31_ERR_E_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_RX_PRBS31_ERR_E_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PRBS31_ERR_E_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_RX_PRBS31_ERR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PRBS31_ERR_E_set", A, value );

    /* (0x000000a4 bits 20) field RX_PRBS31_ERR_E of register PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA */
    fc_cbr_patt_ins_reg_INTERRUPT_ENA_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PRBS31_ERR_E_MSK,
                                                 FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PRBS31_ERR_E_OFF,
                                                 value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_RX_PRBS31_ERR_E_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                cbr_fsgm_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_RX_PRBS31_ERR_E_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                cbr_fsgm_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PRBS31_ERR_E_get", A, 3);
    /* (0x000000a4 bits 20) field RX_PRBS31_ERR_E of register PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA */
    reg_value = fc_cbr_patt_ins_reg_INTERRUPT_ENA_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PRBS31_ERR_E_MSK) >> FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PRBS31_ERR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PRBS31_ERR_E_get", A, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_field_RX_PN11_ERR_E_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                            cbr_fsgm_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_RX_PN11_ERR_E_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                            cbr_fsgm_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PN11_ERR_E_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_RX_PN11_ERR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PN11_ERR_E_set", A, value );

    /* (0x000000a4 bits 16) field RX_PN11_ERR_E of register PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA */
    fc_cbr_patt_ins_reg_INTERRUPT_ENA_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PN11_ERR_E_MSK,
                                                 FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PN11_ERR_E_OFF,
                                                 value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_RX_PN11_ERR_E_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_RX_PN11_ERR_E_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PN11_ERR_E_get", A, 3);
    /* (0x000000a4 bits 16) field RX_PN11_ERR_E of register PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA */
    reg_value = fc_cbr_patt_ins_reg_INTERRUPT_ENA_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PN11_ERR_E_MSK) >> FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PN11_ERR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PN11_ERR_E_get", A, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_field_TX_PN11_ERR_E_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                            cbr_fsgm_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_TX_PN11_ERR_E_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                            cbr_fsgm_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_TX_PN11_ERR_E_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_TX_PN11_ERR_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_TX_PN11_ERR_E_set", A, value );

    /* (0x000000a4 bits 12) field TX_PN11_ERR_E of register PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA */
    fc_cbr_patt_ins_reg_INTERRUPT_ENA_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_TX_PN11_ERR_E_MSK,
                                                 FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_TX_PN11_ERR_E_OFF,
                                                 value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_TX_PN11_ERR_E_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_TX_PN11_ERR_E_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_TX_PN11_ERR_E_get", A, 3);
    /* (0x000000a4 bits 12) field TX_PN11_ERR_E of register PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA */
    reg_value = fc_cbr_patt_ins_reg_INTERRUPT_ENA_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_TX_PN11_ERR_E_MSK) >> FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_TX_PN11_ERR_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_TX_PN11_ERR_E_get", A, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_field_RX_PN11_SYNC_E_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                             cbr_fsgm_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_RX_PN11_SYNC_E_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                             cbr_fsgm_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PN11_SYNC_E_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_RX_PN11_SYNC_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PN11_SYNC_E_set", A, value );

    /* (0x000000a4 bits 8) field RX_PN11_SYNC_E of register PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA */
    fc_cbr_patt_ins_reg_INTERRUPT_ENA_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PN11_SYNC_E_MSK,
                                                 FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PN11_SYNC_E_OFF,
                                                 value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_RX_PN11_SYNC_E_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_RX_PN11_SYNC_E_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PN11_SYNC_E_get", A, 3);
    /* (0x000000a4 bits 8) field RX_PN11_SYNC_E of register PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA */
    reg_value = fc_cbr_patt_ins_reg_INTERRUPT_ENA_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PN11_SYNC_E_MSK) >> FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PN11_SYNC_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PN11_SYNC_E_get", A, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_field_TX_PN11_SYNC_E_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                             cbr_fsgm_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_TX_PN11_SYNC_E_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                             cbr_fsgm_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_TX_PN11_SYNC_E_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_TX_PN11_SYNC_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_TX_PN11_SYNC_E_set", A, value );

    /* (0x000000a4 bits 4) field TX_PN11_SYNC_E of register PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA */
    fc_cbr_patt_ins_reg_INTERRUPT_ENA_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_TX_PN11_SYNC_E_MSK,
                                                 FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_TX_PN11_SYNC_E_OFF,
                                                 value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_TX_PN11_SYNC_E_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_TX_PN11_SYNC_E_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_TX_PN11_SYNC_E_get", A, 3);
    /* (0x000000a4 bits 4) field TX_PN11_SYNC_E of register PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA */
    reg_value = fc_cbr_patt_ins_reg_INTERRUPT_ENA_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_TX_PN11_SYNC_E_MSK) >> FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_TX_PN11_SYNC_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_TX_PN11_SYNC_E_get", A, value );

    return value;
}
static INLINE void fc_cbr_patt_ins_field_UNDERRUN_E_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void fc_cbr_patt_ins_field_UNDERRUN_E_set( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_UNDERRUN_E_set", A, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "fc_cbr_patt_ins_field_UNDERRUN_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "fc_cbr_patt_ins_field_UNDERRUN_E_set", A, value );

    /* (0x000000a4 bits 0) field UNDERRUN_E of register PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA */
    fc_cbr_patt_ins_reg_INTERRUPT_ENA_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_UNDERRUN_E_MSK,
                                                 FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_UNDERRUN_E_OFF,
                                                 value);
}

static INLINE UINT32 fc_cbr_patt_ins_field_UNDERRUN_E_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                           cbr_fsgm_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_UNDERRUN_E_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                           cbr_fsgm_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_UNDERRUN_E_get", A, 3);
    /* (0x000000a4 bits 0) field UNDERRUN_E of register PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA */
    reg_value = fc_cbr_patt_ins_reg_INTERRUPT_ENA_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_UNDERRUN_E_MSK) >> FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_UNDERRUN_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_UNDERRUN_E_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value
 * ==================================================================================
 */
static INLINE UINT32 fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_V_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                              cbr_fsgm_handle_t *h_ptr,
                                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_V_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                              cbr_fsgm_handle_t *h_ptr,
                                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_V_get", A, 3);
    /* (0x000000ac bits 24) field RX_PRBS31_ALL_ONES_OR_ZEROS_V of register PMC_FC_CBR_PATT_INS_REG_PN11_SYNC_STAT */
    reg_value = fc_cbr_patt_ins_reg_PN11_SYNC_STAT_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_V_MSK) >> FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_V_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                                             cbr_fsgm_handle_t *h_ptr,
                                                                                             UINT32 A,
                                                                                             UINT32 value,
                                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                                             UINT32 max_count,
                                                                                             UINT32 *num_failed_polls,
                                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_V_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                                             cbr_fsgm_handle_t *h_ptr,
                                                                                             UINT32 A,
                                                                                             UINT32 value,
                                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                                             UINT32 max_count,
                                                                                             UINT32 *num_failed_polls,
                                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_V_poll", A, value );

    /* (0x000000ac bits 24) field RX_PRBS31_ALL_ONES_OR_ZEROS_V of register PMC_FC_CBR_PATT_INS_REG_PN11_SYNC_STAT */
    return fc_cbr_patt_ins_reg_PN11_SYNC_STAT_poll( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_V_MSK,
                                                    (value<<FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_V_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 fc_cbr_patt_ins_field_RX_PN11_SYNC_V_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_RX_PN11_SYNC_V_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PN11_SYNC_V_get", A, 3);
    /* (0x000000ac bits 8) field RX_PN11_SYNC_V of register PMC_FC_CBR_PATT_INS_REG_PN11_SYNC_STAT */
    reg_value = fc_cbr_patt_ins_reg_PN11_SYNC_STAT_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_RX_PN11_SYNC_V_MSK) >> FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_RX_PN11_SYNC_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PN11_SYNC_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_RX_PN11_SYNC_V_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                              cbr_fsgm_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_RX_PN11_SYNC_V_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                              cbr_fsgm_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PN11_SYNC_V_poll", A, value );

    /* (0x000000ac bits 8) field RX_PN11_SYNC_V of register PMC_FC_CBR_PATT_INS_REG_PN11_SYNC_STAT */
    return fc_cbr_patt_ins_reg_PN11_SYNC_STAT_poll( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_RX_PN11_SYNC_V_MSK,
                                                    (value<<FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_RX_PN11_SYNC_V_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 fc_cbr_patt_ins_field_TX_PN11_SYNC_V_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_TX_PN11_SYNC_V_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_TX_PN11_SYNC_V_get", A, 3);
    /* (0x000000ac bits 4) field TX_PN11_SYNC_V of register PMC_FC_CBR_PATT_INS_REG_PN11_SYNC_STAT */
    reg_value = fc_cbr_patt_ins_reg_PN11_SYNC_STAT_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_TX_PN11_SYNC_V_MSK) >> FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_TX_PN11_SYNC_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_TX_PN11_SYNC_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_TX_PN11_SYNC_V_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                              cbr_fsgm_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_TX_PN11_SYNC_V_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                              cbr_fsgm_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_patt_ins_field_TX_PN11_SYNC_V_poll", A, value );

    /* (0x000000ac bits 4) field TX_PN11_SYNC_V of register PMC_FC_CBR_PATT_INS_REG_PN11_SYNC_STAT */
    return fc_cbr_patt_ins_reg_PN11_SYNC_STAT_poll( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_TX_PN11_SYNC_V_MSK,
                                                    (value<<FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_TX_PN11_SYNC_V_OFF),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE UINT32 fc_cbr_patt_ins_field_UNDERRUN_V_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                           cbr_fsgm_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_UNDERRUN_V_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                           cbr_fsgm_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_UNDERRUN_V_get", A, 3);
    /* (0x000000ac bits 0) field UNDERRUN_V of register PMC_FC_CBR_PATT_INS_REG_PN11_SYNC_STAT */
    reg_value = fc_cbr_patt_ins_reg_PN11_SYNC_STAT_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_UNDERRUN_V_MSK) >> FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_UNDERRUN_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_UNDERRUN_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_UNDERRUN_V_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                          cbr_fsgm_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_UNDERRUN_V_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                          cbr_fsgm_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_patt_ins_field_UNDERRUN_V_poll", A, value );

    /* (0x000000ac bits 0) field UNDERRUN_V of register PMC_FC_CBR_PATT_INS_REG_PN11_SYNC_STAT */
    return fc_cbr_patt_ins_reg_PN11_SYNC_STAT_poll( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_UNDERRUN_V_MSK,
                                                    (value<<FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_UNDERRUN_V_OFF),
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
static INLINE UINT32 fc_cbr_patt_ins_field_TIP_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                    cbr_fsgm_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_TIP_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                    cbr_fsgm_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_TIP_get", A, 3);
    /* (0x00000084 bits 31) field TIP of register PMC_FC_CBR_PATT_INS_REG_UPDATE */
    reg_value = fc_cbr_patt_ins_reg_UPDATE_read(  b_ptr, h_ptr, A);
    value = (reg_value & FC_CBR_PATT_INS_REG_UPDATE_BIT_TIP_MSK) >> FC_CBR_PATT_INS_REG_UPDATE_BIT_TIP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_TIP_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_TIP_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_TIP_poll( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "fc_cbr_patt_ins_field_TIP_poll", A, value );

    /* (0x00000084 bits 31) field TIP of register PMC_FC_CBR_PATT_INS_REG_UPDATE */
    return fc_cbr_patt_ins_reg_UPDATE_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            FC_CBR_PATT_INS_REG_UPDATE_BIT_TIP_MSK,
                                            (value<<FC_CBR_PATT_INS_REG_UPDATE_BIT_TIP_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE UINT32 fc_cbr_patt_ins_field_TX_PN11_ERR_CNT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                cbr_fsgm_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_TX_PN11_ERR_CNT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                cbr_fsgm_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_TX_PN11_ERR_CNT_get", A, 3);
    /* (0x000000b0 bits 15:0) bits 0:15 use field TX_PN11_ERR_CNT of register PMC_FC_CBR_PATT_INS_REG_TX_PN11_ERR_CNT */
    reg_value = fc_cbr_patt_ins_reg_TX_PN11_ERR_CNT_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & FC_CBR_PATT_INS_REG_TX_PN11_ERR_CNT_BIT_TX_PN11_ERR_CNT_MSK) >> FC_CBR_PATT_INS_REG_TX_PN11_ERR_CNT_BIT_TX_PN11_ERR_CNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_TX_PN11_ERR_CNT_get", A, value );

    return value;
}
static INLINE UINT32 fc_cbr_patt_ins_field_range_TX_PN11_ERR_CNT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                      cbr_fsgm_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_range_TX_PN11_ERR_CNT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                      cbr_fsgm_handle_t *h_ptr,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_range_TX_PN11_ERR_CNT_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_patt_ins_field_range_TX_PN11_ERR_CNT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_patt_ins_field_range_TX_PN11_ERR_CNT_get", stop_bit, 15 );
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
    /* (0x000000b0 bits 15:0) bits 0:15 use field TX_PN11_ERR_CNT of register PMC_FC_CBR_PATT_INS_REG_TX_PN11_ERR_CNT */
    reg_value = fc_cbr_patt_ins_reg_TX_PN11_ERR_CNT_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & FC_CBR_PATT_INS_REG_TX_PN11_ERR_CNT_BIT_TX_PN11_ERR_CNT_MSK)
                  >> FC_CBR_PATT_INS_REG_TX_PN11_ERR_CNT_BIT_TX_PN11_ERR_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_PATT_INS_REG_TX_PN11_ERR_CNT_BIT_TX_PN11_ERR_CNT_MSK, FC_CBR_PATT_INS_REG_TX_PN11_ERR_CNT_BIT_TX_PN11_ERR_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_patt_ins_field_range_TX_PN11_ERR_CNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 fc_cbr_patt_ins_field_RX_PN11_ERR_CNT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                cbr_fsgm_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_RX_PN11_ERR_CNT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                cbr_fsgm_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PN11_ERR_CNT_get", A, 3);
    /* (0x000000b4 bits 15:0) bits 0:15 use field RX_PN11_ERR_CNT of register PMC_FC_CBR_PATT_INS_REG_RX_PN11_ERR_CNT */
    reg_value = fc_cbr_patt_ins_reg_RX_PN11_ERR_CNT_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & FC_CBR_PATT_INS_REG_RX_PN11_ERR_CNT_BIT_RX_PN11_ERR_CNT_MSK) >> FC_CBR_PATT_INS_REG_RX_PN11_ERR_CNT_BIT_RX_PN11_ERR_CNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PN11_ERR_CNT_get", A, value );

    return value;
}
static INLINE UINT32 fc_cbr_patt_ins_field_range_RX_PN11_ERR_CNT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                      cbr_fsgm_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_range_RX_PN11_ERR_CNT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                      cbr_fsgm_handle_t *h_ptr,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_range_RX_PN11_ERR_CNT_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_patt_ins_field_range_RX_PN11_ERR_CNT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_patt_ins_field_range_RX_PN11_ERR_CNT_get", stop_bit, 15 );
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
    /* (0x000000b4 bits 15:0) bits 0:15 use field RX_PN11_ERR_CNT of register PMC_FC_CBR_PATT_INS_REG_RX_PN11_ERR_CNT */
    reg_value = fc_cbr_patt_ins_reg_RX_PN11_ERR_CNT_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & FC_CBR_PATT_INS_REG_RX_PN11_ERR_CNT_BIT_RX_PN11_ERR_CNT_MSK)
                  >> FC_CBR_PATT_INS_REG_RX_PN11_ERR_CNT_BIT_RX_PN11_ERR_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_PATT_INS_REG_RX_PN11_ERR_CNT_BIT_RX_PN11_ERR_CNT_MSK, FC_CBR_PATT_INS_REG_RX_PN11_ERR_CNT_BIT_RX_PN11_ERR_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_patt_ins_field_range_RX_PN11_ERR_CNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 fc_cbr_patt_ins_field_RX_PRBS31_ERR_CNT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                  cbr_fsgm_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_RX_PRBS31_ERR_CNT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                  cbr_fsgm_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_RX_PRBS31_ERR_CNT_get", A, 3);
    /* (0x000000b8 bits 15:0) bits 0:15 use field RX_PRBS31_ERR_CNT of register PMC_FC_CBR_PATT_INS_REG_RX_PRBS31_ERR_CNT */
    reg_value = fc_cbr_patt_ins_reg_RX_PRBS31_ERR_CNT_read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & FC_CBR_PATT_INS_REG_RX_PRBS31_ERR_CNT_BIT_RX_PRBS31_ERR_CNT_MSK) >> FC_CBR_PATT_INS_REG_RX_PRBS31_ERR_CNT_BIT_RX_PRBS31_ERR_CNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "fc_cbr_patt_ins_field_RX_PRBS31_ERR_CNT_get", A, value );

    return value;
}
static INLINE UINT32 fc_cbr_patt_ins_field_range_RX_PRBS31_ERR_CNT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                        cbr_fsgm_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 fc_cbr_patt_ins_field_range_RX_PRBS31_ERR_CNT_get( fc_cbr_patt_ins_buffer_t *b_ptr,
                                                                        cbr_fsgm_handle_t *h_ptr,
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

    if (A > 3)
        IO_RANGE_CHECK("%s A is %d but max is %d", "fc_cbr_patt_ins_field_range_RX_PRBS31_ERR_CNT_get", A, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "fc_cbr_patt_ins_field_range_RX_PRBS31_ERR_CNT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "fc_cbr_patt_ins_field_range_RX_PRBS31_ERR_CNT_get", stop_bit, 15 );
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
    /* (0x000000b8 bits 15:0) bits 0:15 use field RX_PRBS31_ERR_CNT of register PMC_FC_CBR_PATT_INS_REG_RX_PRBS31_ERR_CNT */
    reg_value = fc_cbr_patt_ins_reg_RX_PRBS31_ERR_CNT_read( b_ptr,
                                                            h_ptr,
                                                            A);
    field_value = (reg_value & FC_CBR_PATT_INS_REG_RX_PRBS31_ERR_CNT_BIT_RX_PRBS31_ERR_CNT_MSK)
                  >> FC_CBR_PATT_INS_REG_RX_PRBS31_ERR_CNT_BIT_RX_PRBS31_ERR_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, FC_CBR_PATT_INS_REG_RX_PRBS31_ERR_CNT_BIT_RX_PRBS31_ERR_CNT_MSK, FC_CBR_PATT_INS_REG_RX_PRBS31_ERR_CNT_BIT_RX_PRBS31_ERR_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "fc_cbr_patt_ins_field_range_RX_PRBS31_ERR_CNT_get", A, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FC_CBR_PATT_INS_IO_INLINE_H */
