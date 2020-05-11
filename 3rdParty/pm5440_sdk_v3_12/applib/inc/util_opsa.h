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

#ifndef _UTIL_OPSA_H
#define _UTIL_OPSA_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "util_global.h"
#include "util_opsa_api.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: util_opsa_cn_frame_period_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Frame periods
*
* ELEMENTS:
*   See typdef.
*
*******************************************************************************/
typedef enum {
    UTIL_OPSA_MAPOTN_ODU0_T_FRAME,
    UTIL_OPSA_MAPOTN_ODU1_T_FRAME,
    UTIL_OPSA_MAPOTN_ODU1E_T_FRAME,
    UTIL_OPSA_MAPOTN_ODU1F_T_FRAME,
    UTIL_OPSA_MAPOTN_ODU2_T_FRAME,
    UTIL_OPSA_MAPOTN_ODU2E_T_FRAME,
    UTIL_OPSA_MAPOTN_ODU2F_T_FRAME,
    UTIL_OPSA_MAPOTN_ODU3_T_FRAME,
    UTIL_OPSA_MAPOTN_ODU3E1_T_FRAME,
    UTIL_OPSA_MAPOTN_ODU3E2_T_FRAME,
    UTIL_OPSA_MAPOTN_ODU4_T_FRAME,
    UTIL_OPSA_MAPOTN_ODUFLEX_T_FRAME,
    UTIL_OPSA_COREOTN_ODU1_1G25_T_FRAME,
    UTIL_OPSA_COREOTN_ODU2_1G25_T_FRAME,
    UTIL_OPSA_COREOTN_ODU2_2G5_T_FRAME,
    UTIL_OPSA_COREOTN_ODU3_1G25_T_FRAME,
    UTIL_OPSA_COREOTN_ODU3_2G5_T_FRAME,
    UTIL_OPSA_COREOTN_ODU3E1_2G5_T_FRAME,
    UTIL_OPSA_COREOTN_ODU3E2_1G25_T_FRAME,
    UTIL_OPSA_COREOTN_ODU4_1G25_T_FRAME,
    UTIL_OPSA_COREOTN_RESERVED_1_T_FRAME,
    UTIL_OPSA_COREOTN_RESERVED_2_T_FRAME, 
    UTIL_OPSA_T_FRAME_UNUSED,
} util_opsa_cn_frame_period_t;

/*
** Constants
*/
#define UTIL_OPSA_DPI_WORD_SIZE 48
#define UTIL_OPSA_HEADER_SIZE   4

#define UTIL_OPSA_REF_FREQ                   (DOUBLE)311040000.0
#define UTIL_OPSA_REF_FREQ_F                 (FLOAT)311040000.0F
#define UTIL_OPSA_PKT_PERIOD_STEP_DEFAULT    128
#define UTIL_OPSA_BIT_PER_BYTE               8
#define UTIL_OPSA_MAX_RESULT_CALC            50000
/* Resolution for denominator values in hardware */
#define UTIL_OPSA_PKT_PER_D_MAX              0xFFFFFF
/* Actual resolution is 0xffffffff (32-bits), but this increases the calculation time in util_opsa_find_rational() too much. */
#define UTIL_OPSA_CN_BASE_D_MAX              0xFFFFFF
#define UTIL_OPSA_ODUFLEX_PAYLOAD_BYTES      121856

/* Cn frame periods */
/* ODU0 => ~98.354 us */
#define ODU0_T_FRAME_N   0x00007780
#define ODU0_T_FRAME_D   0x000001
#define ODU0_T_FRAME     (DOUBLE)ODU0_T_FRAME_N/ODU0_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* ODU1 => ~48.971 us */
#define ODU1_T_FRAME_N   0x00003B80
#define ODU1_T_FRAME_D   0x000001
#define ODU1_T_FRAME     (DOUBLE)ODU1_T_FRAME_N/ODU1_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* ODU1E => ~11.816 us */
#define ODU1E_T_FRAME_N   0x25A70000
#define ODU1E_T_FRAME_D   0x00029F63
#define ODU1E_T_FRAME     (DOUBLE)ODU1E_T_FRAME_N/ODU1E_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* ODU1F => ~11.585 us */
#define ODU1F_T_FRAME_N   0x017A0000
#define ODU1F_T_FRAME_D   0x00001ADB
#define ODU1F_T_FRAME     (DOUBLE)ODU1F_T_FRAME_N/ODU1F_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* ODU2 => ~12.191 us */
#define ODU2_T_FRAME_N   0x00000ED0
#define ODU2_T_FRAME_D   0x000001
#define ODU2_T_FRAME     (DOUBLE)ODU2_T_FRAME_N/ODU2_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* ODU2E => ~11.767 us */
#define ODU2E_T_FRAME_N  0x257E8000
#define ODU2E_T_FRAME_D  0x00029F63
#define ODU2E_T_FRAME     (DOUBLE)ODU2E_T_FRAME_N/ODU2E_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* ODU2F => ~11.536 us */
#define ODU2F_T_FRAME_N  0x18FF0000
#define ODU2F_T_FRAME_D  0x0001C88B
#define ODU2F_T_FRAME     (DOUBLE)ODU2F_T_FRAME_N/ODU2F_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* ODU3 => ~3.035 us */
#define ODU3_T_FRAME_N   0x000003B0
#define ODU3_T_FRAME_D   0x000001
#define ODU3_T_FRAME     (DOUBLE)ODU3_T_FRAME_N/ODU3_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* ODU3E1 => ~2.929 us */
#define ODU3E1_T_FRAME_N 0x09558000
#define ODU3E1_T_FRAME_D 0x00029F63
#define ODU3E1_T_FRAME   (DOUBLE)ODU3E1_T_FRAME_N/ODU3E1_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* ODU3E2 => ~2.928 us */
#define ODU3E2_T_FRAME_N 0x00012034
#define ODU3E2_T_FRAME_D 0x000051
#define ODU3E2_T_FRAME   (DOUBLE)ODU3E2_T_FRAME_N/ODU3E2_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* ODU4 => ~1.168 us */
#define ODU4_T_FRAME_N   0x00000718
#define ODU4_T_FRAME_D   0x000005
#define ODU4_T_FRAME     (DOUBLE)ODU4_T_FRAME_N/ODU4_T_FRAME_D/UTIL_OPSA_REF_FREQ


/*
** Cn frame periods when dynamic array is implemented 
*/
/* MAPOTN_ODU0 => ~98.354 us */
#define MAPOTN_ODU0_T_FRAME_N   0x00007780
#define MAPOTN_ODU0_T_FRAME_D   0x000001
#define MAPOTN_ODU0_T_FRAME     (DOUBLE)MAPOTN_ODU0_T_FRAME_N/MAPOTN_ODU0_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* MAPOTN_ODU1 => ~48.971 us */
#define MAPOTN_ODU1_T_FRAME_N   0x00003B80
#define MAPOTN_ODU1_T_FRAME_D   0x000001
#define MAPOTN_ODU1_T_FRAME     (DOUBLE)MAPOTN_ODU1_T_FRAME_N/MAPOTN_ODU1_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* MAPOTN_ODU1E => ~11.816 us */
#define MAPOTN_ODU1E_T_FRAME_N   0x25A70000
#define MAPOTN_ODU1E_T_FRAME_D   0x00029F63
#define MAPOTN_ODU1E_T_FRAME     (DOUBLE)MAPOTN_ODU1E_T_FRAME_N/MAPOTN_ODU1E_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* MAPOTN_ODU1F => ~11.585 us */
#define MAPOTN_ODU1F_T_FRAME_N   0x017A0000
#define MAPOTN_ODU1F_T_FRAME_D   0x00001ADB
#define MAPOTN_ODU1F_T_FRAME     (DOUBLE)MAPOTN_ODU1F_T_FRAME_N/MAPOTN_ODU1F_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* MAPOTN_ODU2 => ~12.191 us */
#define MAPOTN_ODU2_T_FRAME_N   0x00000ED0
#define MAPOTN_ODU2_T_FRAME_D   0x000001
#define MAPOTN_ODU2_T_FRAME     (DOUBLE)MAPOTN_ODU2_T_FRAME_N/MAPOTN_ODU2_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* MAPOTN_ODU2E => ~11.767 us */
#define MAPOTN_ODU2E_T_FRAME_N  0x257E8000
#define MAPOTN_ODU2E_T_FRAME_D  0x00029F63
#define MAPOTN_ODU2E_T_FRAME     (DOUBLE)MAPOTN_ODU2E_T_FRAME_N/MAPOTN_ODU2E_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* MAPOTN_ODU2F => ~11.536 us */
#define MAPOTN_ODU2F_T_FRAME_N  0x18FF0000
#define MAPOTN_ODU2F_T_FRAME_D  0x0001C88B
#define MAPOTN_ODU2F_T_FRAME     (DOUBLE)MAPOTN_ODU2F_T_FRAME_N/MAPOTN_ODU2F_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* MAPOTN_ODU3 => ~3.035 us */
#define MAPOTN_ODU3_T_FRAME_N   0x000003B0
#define MAPOTN_ODU3_T_FRAME_D   0x000001
#define MAPOTN_ODU3_T_FRAME     (DOUBLE)MAPOTN_ODU3_T_FRAME_N/MAPOTN_ODU3_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* MAPOTN_ODU3E1 => ~2.929 us */
#define MAPOTN_ODU3E1_T_FRAME_N 0x09558000
#define MAPOTN_ODU3E1_T_FRAME_D 0x00029F63
#define MAPOTN_ODU3E1_T_FRAME   (DOUBLE)MAPOTN_ODU3E1_T_FRAME_N/MAPOTN_ODU3E1_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* MAPOTN_ODU3E2 => ~2.928 us */
#define MAPOTN_ODU3E2_T_FRAME_N 0x00012034
#define MAPOTN_ODU3E2_T_FRAME_D 0x000051
#define MAPOTN_ODU3E2_T_FRAME   (DOUBLE)MAPOTN_ODU3E2_T_FRAME_N/MAPOTN_ODU3E2_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* MAPOTN_ODU4 => ~1.168 us */
#define MAPOTN_ODU4_T_FRAME_N   0x00000718
#define MAPOTN_ODU4_T_FRAME_D   0x000005
#define MAPOTN_ODU4_T_FRAME     (DOUBLE)MAPOTN_ODU4_T_FRAME_N/MAPOTN_ODU4_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* UTIL_OPSA_COREOTN_ODU1_1G25_T_FRAME => ~97.942 us */
#define COREOTN_ODU1_1G25_T_FRAME_N   0x00007700
#define COREOTN_ODU1_1G25_T_FRAME_D   0x000001
#define COREOTN_ODU1_1G25_T_FRAME     (DOUBLE)COREOTN_ODU1_1G25_T_FRAME_N/COREOTN_ODU1_1G25_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* UTIL_OPSA_COREOTN_ODU2_1G25_T_FRAME => ~97.531 us */
#define COREOTN_ODU2_1G25_T_FRAME_N   0x00007680
#define COREOTN_ODU2_1G25_T_FRAME_D   0x000001
#define COREOTN_ODU2_1G25_T_FRAME     (DOUBLE)COREOTN_ODU2_1G25_T_FRAME_N/COREOTN_ODU2_1G25_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* UTIL_OPSA_COREOTN_ODU2_2G5_T_FRAME => ~48.765 us */
#define COREOTN_ODU2_2G5_T_FRAME_N   0x00003B40
#define COREOTN_ODU2_2G5_T_FRAME_D   0x000001
#define COREOTN_ODU2_2G5_T_FRAME     (DOUBLE)COREOTN_ODU2_2G5_T_FRAME_N/COREOTN_ODU2_2G5_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* UTIL_OPSA_COREOTN_ODU3_1G25_T_FRAME => ~97.119 us */
#define COREOTN_ODU3_1G25_T_FRAME_N   0x00007600
#define COREOTN_ODU3_1G25_T_FRAME_D   0x000001
#define COREOTN_ODU3_1G25_T_FRAME     (DOUBLE)COREOTN_ODU3_1G25_T_FRAME_N/COREOTN_ODU3_1G25_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* UTIL_OPSA_COREOTN_ODU3_2G5_T_FRAME => ~48.560 us */
#define COREOTN_ODU3_2G5_T_FRAME_N   0x00003B00
#define COREOTN_ODU3_2G5_T_FRAME_D   0x000001
#define COREOTN_ODU3_2G5_T_FRAME     (DOUBLE)COREOTN_ODU3_2G5_T_FRAME_N/COREOTN_ODU3_2G5_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* UTIL_OPSA_COREOTN_ODU3E1_2G5_T_FRAME => ~46.868 us */
#define COREOTN_ODU3E1_2G5_T_FRAME_N   0x95580000
#define COREOTN_ODU3E1_2G5_T_FRAME_D   0x029F63
#define COREOTN_ODU3E1_2G5_T_FRAME     (DOUBLE)COREOTN_ODU3E1_2G5_T_FRAME_N/COREOTN_ODU3E1_2G5_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* UTIL_OPSA_COREOTN_ODU3E2_1G25_T_FRAME => ~93.710 us */
#define COREOTN_ODU3E2_1G25_T_FRAME_N  0x00240680
#define COREOTN_ODU3E2_1G25_T_FRAME_D  0x00000051
#define COREOTN_ODU3E2_1G25_T_FRAME     (DOUBLE)COREOTN_ODU3E2_1G25_T_FRAME_N/COREOTN_ODU3E2_1G25_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* UTIL_OPSA_COREOTN_ODU4_1G25_T_FRAME => ~93.416 us */
#define COREOTN_ODU4_1G25_T_FRAME_N  0x00007180
#define COREOTN_ODU4_1G25_T_FRAME_D  0x000001
#define COREOTN_ODU4_1G25_T_FRAME     (DOUBLE)COREOTN_ODU4_1G25_T_FRAME_N/COREOTN_ODU4_1G25_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* UTIL_OPSA_COREOTN_RESERVED_1_T_FRAME => ~46.855 us */
#define COREOTN_RESERVED_1_T_FRAME_N   0x00120340
#define COREOTN_RESERVED_1_T_FRAME_D   0x000051
#define COREOTN_RESERVED_1_T_FRAME     (DOUBLE)COREOTN_RESERVED_1_T_FRAME_N/COREOTN_RESERVED_1_T_FRAME_D/UTIL_OPSA_REF_FREQ

/* UTIL_OPSA_COREOTN_RESERVED_2_T_FRAME => ~11.714 us */
#define COREOTN_RESERVED_2_T_FRAME_N    0x000480D0
#define COREOTN_RESERVED_2_T_FRAME_D    0x000051
#define COREOTN_RESERVED_2_T_FRAME      (DOUBLE)COREOTN_RESERVED_2_T_FRAME_N/COREOTN_RESERVED_2_T_FRAME_D/UTIL_OPSA_REF_FREQ



/* Pi constant */
#define UTIL_OPSA_PI  (DOUBLE)3.1415926535897932


/* OIF Recommendations for packet sizes */
#define UTIL_OPSA_OIF_128_MIN_BMAX    112
#define UTIL_OPSA_OIF_128_MAX_BMAX    124
#define UTIL_OPSA_OIF_256_MIN_BMAX    240
#define UTIL_OPSA_OIF_256_MAX_BMAX    252
#define UTIL_OPSA_OIF_512_MIN_BMAX    496
#define UTIL_OPSA_OIF_512_MAX_BMAX    508

/* OIF Recommendations for segmentation ratios based on rate (Gbps) */
#define UTIL_OPSA_OIF_SEG_RATE_1       11
#define UTIL_OPSA_OIF_128_SEG_N_1       1
#define UTIL_OPSA_OIF_256_SEG_N_1       1
#define UTIL_OPSA_OIF_512_SEG_N_1       1
#define UTIL_OPSA_OIF_SEG_RATE_2       42
#define UTIL_OPSA_OIF_128_SEG_N_2       8
#define UTIL_OPSA_OIF_256_SEG_N_2       4
#define UTIL_OPSA_OIF_512_SEG_N_2       2
#define UTIL_OPSA_OIF_SEG_RATE_3      105
#define UTIL_OPSA_OIF_128_SEG_N_3      16
#define UTIL_OPSA_OIF_256_SEG_N_3       8
#define UTIL_OPSA_OIF_512_SEG_N_3       4

/* ODU ppm offsets */
#define UTIL_OPSA_ODU_PPM_MAX         100  
#define UTIL_OPSA_REF_FREQ_PPM_MAX     20


/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR util_opsa_pkt_period_calc(DOUBLE f_client_bit, DOUBLE t_frame, 
                                           UINT32 min_pkt_size,UINT32 max_pkt_size, util_opsa_pkt_period_calc_t pkt_period_calc,
                                           UINT32 *pkt_per_int, UINT32 *pkt_per_n, UINT32 *pkt_per_d, 
                                           UINT32 *pkt_size, DOUBLE *epsilon, DOUBLE *n_ratio);
PUBLIC PMC_ERROR util_opsa_oif_pkt_period_calc(DOUBLE f_client_bit, UINT32 max_fabric_pkt_size, 
                                               util_opsa_pkt_period_calc_t pkt_period_calc,
                                               UINT32 *pkt_per_int, UINT32 *pkt_per_n, UINT32 *pkt_per_d, 
                                               UINT32 *pkt_size, DOUBLE *epsilon);
PUBLIC void util_opsa_mpma_gain_calc(DOUBLE pkt_period, DOUBLE freq_corner, 
                                     BOOL8 formula_type, UINT32 *mpma_gain);
PUBLIC void util_opsa_mpmo_lpf_gain_calc(UINT32 frm_period_n, UINT32 frm_period_d,
                                         UINT32 *lpf_gain);
PUBLIC void util_opsa_mpmo_cn_base_calc(UINT32 frm_period_n, UINT32 frm_period_d,
                                        UINT32 pkt_per_int, UINT32 pkt_per_n, UINT32 pkt_per_d, UINT32 pkt_size,
                                        UINT32 *cn_base_int_ptr, UINT32 *cn_base_n_ptr, UINT32 *cn_base_d_ptr);
PUBLIC void util_opsa_cn_frame_period_get(util_opsa_cn_frame_period_t cn_frame_period, 
                                          UINT32 *cn_frm_per_n, UINT32 *cn_frm_per_d);
PUBLIC void util_opsa_find_rational(DOUBLE decimal, UINT32 maxden, UINT32* num, UINT32* den);
PUBLIC PMC_ERROR util_opsa_mpmo_latency_calc(DOUBLE f_client_bit, 
                                             UINT32 pkt_size, 
                                             UINT32 num_words,
                                             UINT32 *latency_refclk);
PUBLIC PMC_ERROR util_opsa_find_rational_less_than_one(DOUBLE decimal, UINT32 maxden, UINT32* num, UINT32* den); 
PUBLIC void util_opsa_find_best_rate_per_pkt_size_period( UINT32 pkt_size,
                                                          UINT32 pkts_per_cn,
                                                          UINT32 cn_base,
                                                          DOUBLE *candidate_rates,
                                                          util_opsa_cn_frame_period_t *candidates_cn_frame,
                                                          UINT32 num_candidate_rates,
                                                          UINT32 *best_rate_idx,
                                                          DOUBLE *error);  
PUBLIC void util_opsa_find_best_rate_per_cn_base( UINT32 pkt_size,
                                                  UINT32 cn_base_int, 
                                                  UINT32 cn_base_n, 
                                                  UINT32 cn_base_d,
                                                  UINT32 cn_frm_per_n, 
                                                  UINT32 cn_frm_per_d,
                                                  DOUBLE *candidate_rates,
                                                  UINT32 num_candidate_rates,
                                                  UINT32 *best_rate_idx,
                                                  DOUBLE *error);
PUBLIC void util_opsa_find_best_rate(DOUBLE rate,
                                     DOUBLE *candidate_rates,
                                     UINT32 num_candidate_rates,
                                     UINT32 *best_rate_idx,
                                     DOUBLE *error);

#ifdef __cplusplus
}
#endif

#endif /* _UTIL_OPSA_H */

/*
** end of file
*/
