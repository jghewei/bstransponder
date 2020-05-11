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
 *     and register accessor functions for the rttp_indirect block
 *****************************************************************************/
#ifndef _RTTP_INDIRECT_IO_INLINE_H
#define _RTTP_INDIRECT_IO_INLINE_H

#include "sdh_pmg_api.h"
#include "rttp_indirect_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define RTTP_INDIRECT_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for rttp_indirect
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

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ACCEPTED_TRACE
 * ==================================================================================
 */
/*-----------------------------.
 | parameterset ACCEPTED_TRACE |
 |                             |
 | parameters are:             |
 |     ATRACE                  |
 +----------------------------*/
typedef struct {
    rttp_indirect_buffer_t base;
} rttp_indirect_ACCEPTED_TRACE_buffer_t;
static INLINE void rttp_indirect_ACCEPTED_TRACE_buffer_init( rttp_indirect_ACCEPTED_TRACE_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void rttp_indirect_ACCEPTED_TRACE_buffer_init( rttp_indirect_ACCEPTED_TRACE_buffer_t *b_ptr )
{
    IOLOG( "%s", "rttp_indirect_buffer_init");
    rttp_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void rttp_indirect_ACCEPTED_TRACE_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  rttp_indirect_ACCEPTED_TRACE_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void rttp_indirect_ACCEPTED_TRACE_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  rttp_indirect_ACCEPTED_TRACE_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "rttp_indirect_buffer_retrieve", A);
 *     rttp_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void rttp_indirect_ACCEPTED_TRACE_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                               UINT32 A,
 *                                                               UINT4 some_indexing_parameter,
 *                                                               UINT4 some_other_indexing_parameter,
 *                                                               rttp_indirect_ACCEPTED_TRACE_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void rttp_indirect_ACCEPTED_TRACE_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                               UINT32 A,
 *                                                               UINT4 some_indexing_parameter,
 *                                                               UINT4 some_other_indexing_parameter,
 *                                                               rttp_indirect_ACCEPTED_TRACE_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "rttp_indirect_buffer_apply", A);
 *     rttp_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE UINT32 rttp_indirect_ifield_ATRACE_get( rttp_indirect_ACCEPTED_TRACE_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_indirect_ifield_ATRACE_get( rttp_indirect_ACCEPTED_TRACE_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* ((0x00000080 + (N) * 0x1) bits 7:0) bits 0:7 use field ATRACE of register PMC_RTTP_INDIRECT_REG_ACCEPTED_TRACE index N=0..63 */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & RTTP_INDIRECT_REG_ACCEPTED_TRACE_BIT_ATRACE_MSK) >> RTTP_INDIRECT_REG_ACCEPTED_TRACE_BIT_ATRACE_OFF;
    IOLOG( "%s -> 0x%08x", "rttp_indirect_ifield_ATRACE_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset CAPTURED_TRACE
 * ==================================================================================
 */
/*-----------------------------.
 | parameterset CAPTURED_TRACE |
 |                             |
 | parameters are:             |
 |     CTRACE                  |
 +----------------------------*/
typedef struct {
    rttp_indirect_buffer_t base;
} rttp_indirect_CAPTURED_TRACE_buffer_t;
static INLINE void rttp_indirect_CAPTURED_TRACE_buffer_init( rttp_indirect_CAPTURED_TRACE_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void rttp_indirect_CAPTURED_TRACE_buffer_init( rttp_indirect_CAPTURED_TRACE_buffer_t *b_ptr )
{
    IOLOG( "%s", "rttp_indirect_buffer_init");
    rttp_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void rttp_indirect_CAPTURED_TRACE_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  rttp_indirect_CAPTURED_TRACE_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void rttp_indirect_CAPTURED_TRACE_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  rttp_indirect_CAPTURED_TRACE_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "rttp_indirect_buffer_retrieve", A);
 *     rttp_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void rttp_indirect_CAPTURED_TRACE_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                               UINT32 A,
 *                                                               UINT4 some_indexing_parameter,
 *                                                               UINT4 some_other_indexing_parameter,
 *                                                               rttp_indirect_CAPTURED_TRACE_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void rttp_indirect_CAPTURED_TRACE_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                               UINT32 A,
 *                                                               UINT4 some_indexing_parameter,
 *                                                               UINT4 some_other_indexing_parameter,
 *                                                               rttp_indirect_CAPTURED_TRACE_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "rttp_indirect_buffer_apply", A);
 *     rttp_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE UINT32 rttp_indirect_ifield_CTRACE_get( rttp_indirect_CAPTURED_TRACE_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_indirect_ifield_CTRACE_get( rttp_indirect_CAPTURED_TRACE_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* ((0x00000040 + (N) * 0x1) bits 7:0) bits 0:7 use field CTRACE of register PMC_RTTP_INDIRECT_REG_CAPTURED_TRACE index N=0..63 */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & RTTP_INDIRECT_REG_CAPTURED_TRACE_BIT_CTRACE_MSK) >> RTTP_INDIRECT_REG_CAPTURED_TRACE_BIT_CTRACE_OFF;
    IOLOG( "%s -> 0x%08x", "rttp_indirect_ifield_CTRACE_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset EXPECTED_TRACE
 * ==================================================================================
 */
/*-----------------------------.
 | parameterset EXPECTED_TRACE |
 |                             |
 | parameters are:             |
 |     ETRACE                  |
 +----------------------------*/
typedef struct {
    rttp_indirect_buffer_t base;
} rttp_indirect_EXPECTED_TRACE_buffer_t;
static INLINE void rttp_indirect_EXPECTED_TRACE_buffer_init( rttp_indirect_EXPECTED_TRACE_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void rttp_indirect_EXPECTED_TRACE_buffer_init( rttp_indirect_EXPECTED_TRACE_buffer_t *b_ptr )
{
    IOLOG( "%s", "rttp_indirect_buffer_init");
    rttp_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void rttp_indirect_EXPECTED_TRACE_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  rttp_indirect_EXPECTED_TRACE_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void rttp_indirect_EXPECTED_TRACE_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                  UINT32 A,
 *                                                                  UINT4 some_indexing_parameter,
 *                                                                  UINT4 some_other_indexing_parameter,
 *                                                                  rttp_indirect_EXPECTED_TRACE_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "rttp_indirect_buffer_retrieve", A);
 *     rttp_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void rttp_indirect_EXPECTED_TRACE_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                               UINT32 A,
 *                                                               UINT4 some_indexing_parameter,
 *                                                               UINT4 some_other_indexing_parameter,
 *                                                               rttp_indirect_EXPECTED_TRACE_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void rttp_indirect_EXPECTED_TRACE_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                               UINT32 A,
 *                                                               UINT4 some_indexing_parameter,
 *                                                               UINT4 some_other_indexing_parameter,
 *                                                               rttp_indirect_EXPECTED_TRACE_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "rttp_indirect_buffer_apply", A);
 *     rttp_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void rttp_indirect_ifield_ETRACE_set( rttp_indirect_EXPECTED_TRACE_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_indirect_ifield_ETRACE_set( rttp_indirect_EXPECTED_TRACE_buffer_t *b_ptr, UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rttp_indirect_ifield_ETRACE_set", value, 255);
    IOLOG( "%s <= 0x%08x", "rttp_indirect_ifield_ETRACE_set", value );

    /* ((0x000000c0 + (N) * 0x1) bits 7:0) bits 0:7 use field ETRACE of register PMC_RTTP_INDIRECT_REG_EXPECTED_TRACE index N=0..63 */
    b_ptr->base.mask[0] |= RTTP_INDIRECT_REG_EXPECTED_TRACE_BIT_ETRACE_MSK;
    b_ptr->base.data[0] &= ~RTTP_INDIRECT_REG_EXPECTED_TRACE_BIT_ETRACE_MSK;
    b_ptr->base.data[0] |= RTTP_INDIRECT_REG_EXPECTED_TRACE_BIT_ETRACE_MSK & (value << RTTP_INDIRECT_REG_EXPECTED_TRACE_BIT_ETRACE_OFF);
}

static INLINE UINT32 rttp_indirect_ifield_ETRACE_get( rttp_indirect_EXPECTED_TRACE_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_indirect_ifield_ETRACE_get( rttp_indirect_EXPECTED_TRACE_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* ((0x000000c0 + (N) * 0x1) bits 7:0) bits 0:7 use field ETRACE of register PMC_RTTP_INDIRECT_REG_EXPECTED_TRACE index N=0..63 */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & RTTP_INDIRECT_REG_EXPECTED_TRACE_BIT_ETRACE_MSK) >> RTTP_INDIRECT_REG_EXPECTED_TRACE_BIT_ETRACE_OFF;
    IOLOG( "%s -> 0x%08x", "rttp_indirect_ifield_ETRACE_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset TRACE_CFG
 * ==================================================================================
 */
/*------------------------.
 | parameterset TRACE_CFG |
 |                        |
 | parameters are:        |
 |     TTMMONEN           |
 |     SYNC_CRLF          |
 |     ZEROEN             |
 |     PER5               |
 |     LENGTH16           |
 |     ALGO               |
 +-----------------------*/
typedef struct {
    rttp_indirect_buffer_t base;
} rttp_indirect_TRACE_CFG_buffer_t;
static INLINE void rttp_indirect_TRACE_CFG_buffer_init( rttp_indirect_TRACE_CFG_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void rttp_indirect_TRACE_CFG_buffer_init( rttp_indirect_TRACE_CFG_buffer_t *b_ptr )
{
    IOLOG( "%s", "rttp_indirect_buffer_init");
    rttp_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void rttp_indirect_TRACE_CFG_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                             UINT32 A,
 *                                                             UINT4 some_indexing_parameter,
 *                                                             UINT4 some_other_indexing_parameter,
 *                                                             rttp_indirect_TRACE_CFG_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void rttp_indirect_TRACE_CFG_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                             UINT32 A,
 *                                                             UINT4 some_indexing_parameter,
 *                                                             UINT4 some_other_indexing_parameter,
 *                                                             rttp_indirect_TRACE_CFG_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "rttp_indirect_buffer_retrieve", A);
 *     rttp_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void rttp_indirect_TRACE_CFG_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                          UINT32 A,
 *                                                          UINT4 some_indexing_parameter,
 *                                                          UINT4 some_other_indexing_parameter,
 *                                                          rttp_indirect_TRACE_CFG_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void rttp_indirect_TRACE_CFG_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                          UINT32 A,
 *                                                          UINT4 some_indexing_parameter,
 *                                                          UINT4 some_other_indexing_parameter,
 *                                                          rttp_indirect_TRACE_CFG_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "rttp_indirect_buffer_apply", A);
 *     rttp_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void rttp_indirect_ifield_TTMMONEN_set( rttp_indirect_TRACE_CFG_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_indirect_ifield_TTMMONEN_set( rttp_indirect_TRACE_CFG_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rttp_indirect_ifield_TTMMONEN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rttp_indirect_ifield_TTMMONEN_set", value );

    /* (0x00000003 bits 6) field TTMMONEN of register PMC_RTTP_INDIRECT_REG_TRACE_CFG */
    b_ptr->base.mask[0] |= RTTP_INDIRECT_REG_TRACE_CFG_BIT_TTMMONEN_MSK;
    b_ptr->base.data[0] &= ~RTTP_INDIRECT_REG_TRACE_CFG_BIT_TTMMONEN_MSK;
    b_ptr->base.data[0] |= RTTP_INDIRECT_REG_TRACE_CFG_BIT_TTMMONEN_MSK & (value << RTTP_INDIRECT_REG_TRACE_CFG_BIT_TTMMONEN_OFF);
}

static INLINE UINT32 rttp_indirect_ifield_TTMMONEN_get( rttp_indirect_TRACE_CFG_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_indirect_ifield_TTMMONEN_get( rttp_indirect_TRACE_CFG_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000003 bits 6) field TTMMONEN of register PMC_RTTP_INDIRECT_REG_TRACE_CFG */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & RTTP_INDIRECT_REG_TRACE_CFG_BIT_TTMMONEN_MSK) >> RTTP_INDIRECT_REG_TRACE_CFG_BIT_TTMMONEN_OFF;
    IOLOG( "%s -> 0x%08x", "rttp_indirect_ifield_TTMMONEN_get", value );

    return value;
}
static INLINE void rttp_indirect_ifield_SYNC_CRLF_set( rttp_indirect_TRACE_CFG_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_indirect_ifield_SYNC_CRLF_set( rttp_indirect_TRACE_CFG_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rttp_indirect_ifield_SYNC_CRLF_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rttp_indirect_ifield_SYNC_CRLF_set", value );

    /* (0x00000003 bits 5) field SYNC_CRLF of register PMC_RTTP_INDIRECT_REG_TRACE_CFG */
    b_ptr->base.mask[0] |= RTTP_INDIRECT_REG_TRACE_CFG_BIT_SYNC_CRLF_MSK;
    b_ptr->base.data[0] &= ~RTTP_INDIRECT_REG_TRACE_CFG_BIT_SYNC_CRLF_MSK;
    b_ptr->base.data[0] |= RTTP_INDIRECT_REG_TRACE_CFG_BIT_SYNC_CRLF_MSK & (value << RTTP_INDIRECT_REG_TRACE_CFG_BIT_SYNC_CRLF_OFF);
}

static INLINE UINT32 rttp_indirect_ifield_SYNC_CRLF_get( rttp_indirect_TRACE_CFG_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_indirect_ifield_SYNC_CRLF_get( rttp_indirect_TRACE_CFG_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000003 bits 5) field SYNC_CRLF of register PMC_RTTP_INDIRECT_REG_TRACE_CFG */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & RTTP_INDIRECT_REG_TRACE_CFG_BIT_SYNC_CRLF_MSK) >> RTTP_INDIRECT_REG_TRACE_CFG_BIT_SYNC_CRLF_OFF;
    IOLOG( "%s -> 0x%08x", "rttp_indirect_ifield_SYNC_CRLF_get", value );

    return value;
}
static INLINE void rttp_indirect_ifield_ZEROEN_set( rttp_indirect_TRACE_CFG_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_indirect_ifield_ZEROEN_set( rttp_indirect_TRACE_CFG_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rttp_indirect_ifield_ZEROEN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rttp_indirect_ifield_ZEROEN_set", value );

    /* (0x00000003 bits 4) field ZEROEN of register PMC_RTTP_INDIRECT_REG_TRACE_CFG */
    b_ptr->base.mask[0] |= RTTP_INDIRECT_REG_TRACE_CFG_BIT_ZEROEN_MSK;
    b_ptr->base.data[0] &= ~RTTP_INDIRECT_REG_TRACE_CFG_BIT_ZEROEN_MSK;
    b_ptr->base.data[0] |= RTTP_INDIRECT_REG_TRACE_CFG_BIT_ZEROEN_MSK & (value << RTTP_INDIRECT_REG_TRACE_CFG_BIT_ZEROEN_OFF);
}

static INLINE UINT32 rttp_indirect_ifield_ZEROEN_get( rttp_indirect_TRACE_CFG_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_indirect_ifield_ZEROEN_get( rttp_indirect_TRACE_CFG_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000003 bits 4) field ZEROEN of register PMC_RTTP_INDIRECT_REG_TRACE_CFG */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & RTTP_INDIRECT_REG_TRACE_CFG_BIT_ZEROEN_MSK) >> RTTP_INDIRECT_REG_TRACE_CFG_BIT_ZEROEN_OFF;
    IOLOG( "%s -> 0x%08x", "rttp_indirect_ifield_ZEROEN_get", value );

    return value;
}
static INLINE void rttp_indirect_ifield_PER5_set( rttp_indirect_TRACE_CFG_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_indirect_ifield_PER5_set( rttp_indirect_TRACE_CFG_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rttp_indirect_ifield_PER5_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rttp_indirect_ifield_PER5_set", value );

    /* (0x00000003 bits 3) field PER5 of register PMC_RTTP_INDIRECT_REG_TRACE_CFG */
    b_ptr->base.mask[0] |= RTTP_INDIRECT_REG_TRACE_CFG_BIT_PER5_MSK;
    b_ptr->base.data[0] &= ~RTTP_INDIRECT_REG_TRACE_CFG_BIT_PER5_MSK;
    b_ptr->base.data[0] |= RTTP_INDIRECT_REG_TRACE_CFG_BIT_PER5_MSK & (value << RTTP_INDIRECT_REG_TRACE_CFG_BIT_PER5_OFF);
}

static INLINE UINT32 rttp_indirect_ifield_PER5_get( rttp_indirect_TRACE_CFG_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_indirect_ifield_PER5_get( rttp_indirect_TRACE_CFG_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000003 bits 3) field PER5 of register PMC_RTTP_INDIRECT_REG_TRACE_CFG */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & RTTP_INDIRECT_REG_TRACE_CFG_BIT_PER5_MSK) >> RTTP_INDIRECT_REG_TRACE_CFG_BIT_PER5_OFF;
    IOLOG( "%s -> 0x%08x", "rttp_indirect_ifield_PER5_get", value );

    return value;
}
static INLINE void rttp_indirect_ifield_LENGTH16_set( rttp_indirect_TRACE_CFG_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_indirect_ifield_LENGTH16_set( rttp_indirect_TRACE_CFG_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rttp_indirect_ifield_LENGTH16_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rttp_indirect_ifield_LENGTH16_set", value );

    /* (0x00000003 bits 2) field LENGTH16 of register PMC_RTTP_INDIRECT_REG_TRACE_CFG */
    b_ptr->base.mask[0] |= RTTP_INDIRECT_REG_TRACE_CFG_BIT_LENGTH16_MSK;
    b_ptr->base.data[0] &= ~RTTP_INDIRECT_REG_TRACE_CFG_BIT_LENGTH16_MSK;
    b_ptr->base.data[0] |= RTTP_INDIRECT_REG_TRACE_CFG_BIT_LENGTH16_MSK & (value << RTTP_INDIRECT_REG_TRACE_CFG_BIT_LENGTH16_OFF);
}

static INLINE UINT32 rttp_indirect_ifield_LENGTH16_get( rttp_indirect_TRACE_CFG_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_indirect_ifield_LENGTH16_get( rttp_indirect_TRACE_CFG_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000003 bits 2) field LENGTH16 of register PMC_RTTP_INDIRECT_REG_TRACE_CFG */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & RTTP_INDIRECT_REG_TRACE_CFG_BIT_LENGTH16_MSK) >> RTTP_INDIRECT_REG_TRACE_CFG_BIT_LENGTH16_OFF;
    IOLOG( "%s -> 0x%08x", "rttp_indirect_ifield_LENGTH16_get", value );

    return value;
}
static INLINE void rttp_indirect_ifield_ALGO_set( rttp_indirect_TRACE_CFG_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_indirect_ifield_ALGO_set( rttp_indirect_TRACE_CFG_buffer_t *b_ptr, UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rttp_indirect_ifield_ALGO_set", value, 3);
    IOLOG( "%s <= 0x%08x", "rttp_indirect_ifield_ALGO_set", value );

    /* (0x00000003 bits 1:0) bits 0:1 use field ALGO of register PMC_RTTP_INDIRECT_REG_TRACE_CFG */
    b_ptr->base.mask[0] |= RTTP_INDIRECT_REG_TRACE_CFG_BIT_ALGO_MSK;
    b_ptr->base.data[0] &= ~RTTP_INDIRECT_REG_TRACE_CFG_BIT_ALGO_MSK;
    b_ptr->base.data[0] |= RTTP_INDIRECT_REG_TRACE_CFG_BIT_ALGO_MSK & (value << RTTP_INDIRECT_REG_TRACE_CFG_BIT_ALGO_OFF);
}

static INLINE UINT32 rttp_indirect_ifield_ALGO_get( rttp_indirect_TRACE_CFG_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_indirect_ifield_ALGO_get( rttp_indirect_TRACE_CFG_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000003 bits 1:0) bits 0:1 use field ALGO of register PMC_RTTP_INDIRECT_REG_TRACE_CFG */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & RTTP_INDIRECT_REG_TRACE_CFG_BIT_ALGO_MSK) >> RTTP_INDIRECT_REG_TRACE_CFG_BIT_ALGO_OFF;
    IOLOG( "%s -> 0x%08x", "rttp_indirect_ifield_ALGO_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset TRACE_MISMATCH_UNSTBL_INT_EN
 * ==================================================================================
 */
/*-------------------------------------------.
 | parameterset TRACE_MISMATCH_UNSTBL_INT_EN |
 |                                           |
 | parameters are:                           |
 |     TIM_E                                 |
 |     TIU_E                                 |
 +------------------------------------------*/
typedef struct {
    rttp_indirect_buffer_t base;
} rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t;
static INLINE void rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_init( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_init( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t *b_ptr )
{
    IOLOG( "%s", "rttp_indirect_buffer_init");
    rttp_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                                UINT32 A,
 *                                                                                UINT4 some_indexing_parameter,
 *                                                                                UINT4 some_other_indexing_parameter,
 *                                                                                rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                                UINT32 A,
 *                                                                                UINT4 some_indexing_parameter,
 *                                                                                UINT4 some_other_indexing_parameter,
 *                                                                                rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "rttp_indirect_buffer_retrieve", A);
 *     rttp_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                             UINT32 A,
 *                                                                             UINT4 some_indexing_parameter,
 *                                                                             UINT4 some_other_indexing_parameter,
 *                                                                             rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                             UINT32 A,
 *                                                                             UINT4 some_indexing_parameter,
 *                                                                             UINT4 some_other_indexing_parameter,
 *                                                                             rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "rttp_indirect_buffer_apply", A);
 *     rttp_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void rttp_indirect_ifield_TIM_E_set( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_indirect_ifield_TIM_E_set( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rttp_indirect_ifield_TIM_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rttp_indirect_ifield_TIM_E_set", value );

    /* (0x00000001 bits 1) field TIM_E of register PMC_RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN */
    b_ptr->base.mask[0] |= RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN_BIT_TIM_E_MSK;
    b_ptr->base.data[0] &= ~RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN_BIT_TIM_E_MSK;
    b_ptr->base.data[0] |= RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN_BIT_TIM_E_MSK & (value << RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN_BIT_TIM_E_OFF);
}

static INLINE UINT32 rttp_indirect_ifield_TIM_E_get( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_indirect_ifield_TIM_E_get( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000001 bits 1) field TIM_E of register PMC_RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN_BIT_TIM_E_MSK) >> RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN_BIT_TIM_E_OFF;
    IOLOG( "%s -> 0x%08x", "rttp_indirect_ifield_TIM_E_get", value );

    return value;
}
static INLINE void rttp_indirect_ifield_TIU_E_set( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_indirect_ifield_TIU_E_set( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rttp_indirect_ifield_TIU_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rttp_indirect_ifield_TIU_E_set", value );

    /* (0x00000001 bits 0) field TIU_E of register PMC_RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN */
    b_ptr->base.mask[0] |= RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN_BIT_TIU_E_MSK;
    b_ptr->base.data[0] &= ~RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN_BIT_TIU_E_MSK;
    b_ptr->base.data[0] |= RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN_BIT_TIU_E_MSK & (value << RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN_BIT_TIU_E_OFF);
}

static INLINE UINT32 rttp_indirect_ifield_TIU_E_get( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_indirect_ifield_TIU_E_get( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000001 bits 0) field TIU_E of register PMC_RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN_BIT_TIU_E_MSK) >> RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN_BIT_TIU_E_OFF;
    IOLOG( "%s -> 0x%08x", "rttp_indirect_ifield_TIU_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset TRACE_MISMATCH_UNSTBL_INT_STAT
 * ==================================================================================
 */
/*---------------------------------------------.
 | parameterset TRACE_MISMATCH_UNSTBL_INT_STAT |
 |                                             |
 | parameters are:                             |
 |     TIM_I                                   |
 |     TIU_I                                   |
 +--------------------------------------------*/
typedef struct {
    rttp_indirect_buffer_t base;
} rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t;
static INLINE void rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_init( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_init( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t *b_ptr )
{
    IOLOG( "%s", "rttp_indirect_buffer_init");
    rttp_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                                  UINT32 A,
 *                                                                                  UINT4 some_indexing_parameter,
 *                                                                                  UINT4 some_other_indexing_parameter,
 *                                                                                  rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                                  UINT32 A,
 *                                                                                  UINT4 some_indexing_parameter,
 *                                                                                  UINT4 some_other_indexing_parameter,
 *                                                                                  rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "rttp_indirect_buffer_retrieve", A);
 *     rttp_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                               UINT32 A,
 *                                                                               UINT4 some_indexing_parameter,
 *                                                                               UINT4 some_other_indexing_parameter,
 *                                                                               rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                               UINT32 A,
 *                                                                               UINT4 some_indexing_parameter,
 *                                                                               UINT4 some_other_indexing_parameter,
 *                                                                               rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "rttp_indirect_buffer_apply", A);
 *     rttp_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE void rttp_indirect_ifield_TIM_I_set( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_indirect_ifield_TIM_I_set( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rttp_indirect_ifield_TIM_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rttp_indirect_ifield_TIM_I_set", value );

    /* (0x00000002 bits 1) field TIM_I of register PMC_RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT */
    b_ptr->base.mask[0] |= RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIM_I_MSK;
    b_ptr->base.data[0] &= ~RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIM_I_MSK;
    b_ptr->base.data[0] |= RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIM_I_MSK & (value << RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIM_I_OFF);
}

static INLINE UINT32 rttp_indirect_ifield_TIM_I_get( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_indirect_ifield_TIM_I_get( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000002 bits 1) field TIM_I of register PMC_RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIM_I_MSK) >> RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIM_I_OFF;
    IOLOG( "%s -> 0x%08x", "rttp_indirect_ifield_TIM_I_get", value );

    return value;
}
static INLINE void rttp_indirect_ifield_TIU_I_set( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t *b_ptr, UINT32 value ) ALWAYS_INLINE;
static INLINE void rttp_indirect_ifield_TIU_I_set( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t *b_ptr, UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rttp_indirect_ifield_TIU_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rttp_indirect_ifield_TIU_I_set", value );

    /* (0x00000002 bits 0) field TIU_I of register PMC_RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT */
    b_ptr->base.mask[0] |= RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIU_I_MSK;
    b_ptr->base.data[0] &= ~RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIU_I_MSK;
    b_ptr->base.data[0] |= RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIU_I_MSK & (value << RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIU_I_OFF);
}

static INLINE UINT32 rttp_indirect_ifield_TIU_I_get( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_indirect_ifield_TIU_I_get( rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000002 bits 0) field TIU_I of register PMC_RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIU_I_MSK) >> RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIU_I_OFF;
    IOLOG( "%s -> 0x%08x", "rttp_indirect_ifield_TIU_I_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset TRACE_MISMATCH_UNSTBL_STAT
 * ==================================================================================
 */
/*-----------------------------------------.
 | parameterset TRACE_MISMATCH_UNSTBL_STAT |
 |                                         |
 | parameters are:                         |
 |     TIM_V                               |
 |     TIU_V                               |
 +----------------------------------------*/
typedef struct {
    rttp_indirect_buffer_t base;
} rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_t;
static INLINE void rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_init( rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_init( rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_t *b_ptr )
{
    IOLOG( "%s", "rttp_indirect_buffer_init");
    rttp_indirect_buffer_init(&(b_ptr->base));
}
/*
 * // user must define the retieve function since the indexing is complicated
 * static INLINE void rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                              UINT32 A,
 *                                                                              UINT4 some_indexing_parameter,
 *                                                                              UINT4 some_other_indexing_parameter,
 *                                                                              rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_retrieve( sdh_pmg_handle_t *h_ptr,
 *                                                                              UINT32 A,
 *                                                                              UINT4 some_indexing_parameter,
 *                                                                              UINT4 some_other_indexing_parameter,
 *                                                                              rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "rttp_indirect_buffer_retrieve", A);
 *     rttp_indirect_buffer_retrieve( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
/*
 * // user must define the apply function since the indexing is complicated
 * static INLINE void rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                           UINT32 A,
 *                                                                           UINT4 some_indexing_parameter,
 *                                                                           UINT4 some_other_indexing_parameter,
 *                                                                           rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_t *b_ptr ) ALWAYS_INLINE;
 * static INLINE void rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_apply( sdh_pmg_handle_t *h_ptr,
 *                                                                           UINT32 A,
 *                                                                           UINT4 some_indexing_parameter,
 *                                                                           UINT4 some_other_indexing_parameter,
 *                                                                           rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_t *b_ptr )
 * {
 *     LOG( "%s A=%d", "rttp_indirect_buffer_apply", A);
 *     rttp_indirect_buffer_apply( h_ptr, some_indexing_parameter, some_other_indexing_parameter, &(b_ptr->base));
 * }
 */
static INLINE UINT32 rttp_indirect_ifield_TIM_V_get( rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_indirect_ifield_TIM_V_get( rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 1) field TIM_V of register PMC_RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_STAT */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_STAT_BIT_TIM_V_MSK) >> RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_STAT_BIT_TIM_V_OFF;
    IOLOG( "%s -> 0x%08x", "rttp_indirect_ifield_TIM_V_get", value );

    return value;
}
static INLINE UINT32 rttp_indirect_ifield_TIU_V_get( rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_t *b_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rttp_indirect_ifield_TIU_V_get( rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_t *b_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field TIU_V of register PMC_RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_STAT */
    reg_value = b_ptr->base.data[0];
    value = (reg_value & RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_STAT_BIT_TIU_V_MSK) >> RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_STAT_BIT_TIU_V_OFF;
    IOLOG( "%s -> 0x%08x", "rttp_indirect_ifield_TIU_V_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RTTP_INDIRECT_IO_INLINE_H */
