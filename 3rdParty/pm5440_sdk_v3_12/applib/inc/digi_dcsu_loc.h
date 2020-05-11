/*******************************************************************************
*   COPYRIGHT (C) 2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
* --------------------------------------------------------------------------
*   DESCRIPTION :
*
*   NOTES:
*
*******************************************************************************/
#ifndef _DIGI_DCSU_LOC_H
#define _DIGI_DCSU_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: digi_dcsu_num_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This enumeration is used to identify DCSU modules.  These modules have fixed
*   values assigned to each term as they are used to address physical resources.
*
*   These enumerations are reseved for usage with dcsu_num input parameter of
*   field accessors defined in dcsu_io.h
*
* ELEMENTS:
* See typedef.
*
*******************************************************************************/
typedef enum {

    DIGI_DCSU_OTN1  = 0,
    DIGI_DCSU_OTN2  = 1,
    DIGI_DCSU_ENET3  = 2,
    DIGI_DCSU_SYS1  = 4,
    DIGI_DCSU_SYS2  = 5,
    DIGI_DCSU_ENET1 = 6,
    DIGI_DCSU_ENET2 = 7,
    DIGI_DCSU_PTP   = 8,

    LAST_DIGI_DCSU
} digi_dcsu_num_t;

/*
** Constants
*/

#define DCSU_INIT_POLL_ITERATIONS 1000

/*
** Macro Definitions
*/
/* DCSU interrupt and status operations */

#define DIGI_DCSU_INT_TABLE_DEFINE() \
    DIGI_DCSU_INT(digi_handle, dcsu, ERR_FAST        , err_fast_i        ); \
    DIGI_DCSU_INT(digi_handle, dcsu, ERR_SLOW        , err_slow_i        ); \
    DIGI_DCSU_INT(digi_handle, dcsu, LOCK            , lock_i            ); \

#define DIGI_DCSU_STATUS_TABLE_DEFINE() \
    DIGI_DCSU_STATUS(digi_handle, dcsu, ERR_FAST_V        , err_fast_v        ); \
    DIGI_DCSU_STATUS(digi_handle, dcsu, ERR_SLOW_V        , err_slow_v        ); \
    DIGI_DCSU_STATUS(digi_handle, dcsu, LOCK_V            , lock_v            ); \


/*
** Structures and Unions
*/

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR dcsu_init(digi_handle_t *h, digi_dcsu_inst_t dcsu_inst);
PUBLIC PMC_ERROR dcsu_uninit(digi_handle_t *h, digi_dcsu_inst_t dcsu_inst);
PUBLIC void digi_dcsu_conditional_init(digi_handle_t *h, digi_dcsu_inst_t dcsu_inst);
PUBLIC void digi_dcsu_conditional_uninit(digi_handle_t *h, digi_dcsu_inst_t dcsu_inst);

PUBLIC PMC_ERROR digi_dcsu_int_cfg(digi_handle_t             *digi_handle,
                                   digi_dcsu_inst_t           dcsu_inst,
                                   digi_dcsu_int_t           *int_table_ptr,
                                   digi_dcsu_int_t           *int_en_table_ptr,
                                   BOOL                       enable);

PUBLIC PMC_ERROR digi_dcsu_int_enabled_check(digi_handle_t              *digi_handle,
                                             digi_dcsu_inst_t            dcsu_inst,
                                             BOOL                       *int_found_ptr);


PUBLIC void digi_dcsu_act(digi_handle_t *h, 
                          digi_dcsu_inst_t dcsu_inst,
                          BOOL8            act);

PUBLIC void digi_dcsu_adjust_enet1(digi_handle_t *h, BOOL8 rate);
PUBLIC void digi_dcsu_adjust_enet2(digi_handle_t *h, BOOL8 rate);

/* LCOV_EXCL_START */
PUBLIC void digi_dcsu_adjust_otu1_for_sysotn(digi_handle_t *h);

/* LCOV_EXCL_STOP */

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_DCSU_LOC_H */

/* 
** end of file 
*/


