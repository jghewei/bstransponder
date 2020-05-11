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
/*
** Include Files
*/
#include "util_opsa_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Local Macro Definitions
*/

/*
** Local Structures and Unions
*/

/*
** Local Variables
*/

/*
** Log Messaging
*/
/* Only include the strings if logging with text*/
#ifdef PMC_LOG_STRINGS

/* 
 * The following creates the LOG/ERR strings from above by pulling out the text
 * portion of the LOG_ERR_TABLE above 
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) str,
const char UTIL_OPSA_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    UTIL_OPSA_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char UTIL_OPSA_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References
*/

/*
** Public Functions
*/
/*******************************************************************************
* util_opsa_pkt_period_calc_double
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Generates a packet period, packet size combo according to the 
*    general equation
*      pkt_size = pkt_period * f_client
*
* INPUTS:
*   f_client_bit    - client bit rate, in Hz.
*   t_frame         - Cn frame period.
*                     Set to 0 if not applicable.
*   min_pkt_size    - minimum packet size to choose 
*   max_pkt_size    - maximum packet size to choose 
*   pkt_period_calc - packet period and size calculation method.
*
* OUTPUTS:
*   pkt_per_int - Packet Period Integer 
*                 (in UTIL_OPSA_REF_FREQ clock cycles)
*                 (Packet Period = pkt_per_int + pkt_per_n/pkt_per_d)
*   pkt_per_n   - Packet Period Numerator
*   pkt_per_d   - Packet Period Denominator
*   pkt_size    - Packet Size (in bytes)
*   epsilon     - Epsilon
*   n_ratio     - ratio of t_frame / packet period
*                 Used for calculating eps_adder.
*
* RETURNS:
*   PMC_SUCCESS - if calculations completed successfully.
*   PMC_ERROR   - Error codes if error encountered.
*        
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_opsa_pkt_period_calc_double(DOUBLE f_client_bit,
                                                  DOUBLE t_frame, 
                                                  UINT32 min_pkt_size, 
                                                  UINT32 max_pkt_size, 
                                                  util_opsa_pkt_period_calc_t pkt_period_calc,
                                                  UINT32 *pkt_per_int, 
                                                  UINT32 *pkt_per_n, 
                                                  UINT32 *pkt_per_d, 
                                                  UINT32 *pkt_size, 
                                                  DOUBLE *epsilon, 
                                                  DOUBLE *n_ratio)
{
    DOUBLE f_client_byte = f_client_bit / UTIL_OPSA_BIT_PER_BYTE;
    UINT32 pp_n_temp;
    UINT32 pp_d_temp;
    UINT32 pkt_size_temp;
    DOUBLE eps_temp;
    UINT32 t_frame_div;
    DOUBLE pkt_per_temp;
    DOUBLE pkt_per_temp_i;

    /* Temp arrays to hold the results */
    UINT32 result_pp_n     = 0;
    UINT32 result_pp_d     = 0;
    UINT32 result_pkt_size = 0;
    DOUBLE result_eps      = 0;
    BOOL   result_valid    = FALSE;

    UINT32 i = 0;

    PMC_ENTRY();

    /* Calculate packet period parameters based on the packet size range */
    if (t_frame == 0) {
        /* Local timing (C mode) */

        /* Use 1 for denominator if only using integer packet period. */
        if (UTIL_OPSA_INT_PKT_PER == pkt_period_calc) {
            pp_d_temp = 1;
        } else {
            pp_d_temp = UTIL_OPSA_PKT_PERIOD_STEP_DEFAULT;
        }

        /* Calculate the packet period numerator */
        pp_n_temp = (UINT32)PMC_FLOOR((DOUBLE)min_pkt_size/f_client_byte * UTIL_OPSA_REF_FREQ) * pp_d_temp;

        /* Calculate the epsilon for this numerator */
        pkt_size_temp = round_pos((DOUBLE)pp_n_temp/pp_d_temp * f_client_byte/UTIL_OPSA_REF_FREQ); 
        eps_temp      = (DOUBLE)pp_n_temp/pp_d_temp * f_client_byte/UTIL_OPSA_REF_FREQ - pkt_size_temp;

        /* Iterate for the desired packet range */
        while ((pkt_size_temp <= max_pkt_size) && (i < UTIL_OPSA_MAX_RESULT_CALC)) {
            /* Store result if packet size is within desired range */
            if (pkt_size_temp >= min_pkt_size) {
                PMC_LOG_TRACE("Index %u, pp_int %u, pp_n %u, pp_d %u, pkt_sz %u, eps %f\n",
                           i,
                           pp_n_temp/pp_d_temp,
                           pp_n_temp - (pp_n_temp/pp_d_temp)*pp_d_temp,
                           pp_d_temp,
                           pkt_size_temp,
                           eps_temp);
                if (!result_valid || (PMC_FABS(result_eps) > PMC_FABS(eps_temp))) {
                    result_pp_n     = pp_n_temp;
                    result_pp_d     = pp_d_temp;
                    result_pkt_size = pkt_size_temp;
                    result_eps      = eps_temp;
                    result_valid    = TRUE;
                }
                i++;
            }

            /* Increment the packet period numerator. */
            pp_n_temp++;

            /* Calculate the epsilon for this numerator */
            pkt_size_temp = round_pos((DOUBLE)pp_n_temp/pp_d_temp * f_client_byte/UTIL_OPSA_REF_FREQ); 
            eps_temp      = (DOUBLE)pp_n_temp/pp_d_temp * f_client_byte/UTIL_OPSA_REF_FREQ - pkt_size_temp;    
        }

        /* Check if the calculations reached the maximum packet size */
        if (i >= UTIL_OPSA_MAX_RESULT_CALC) {
            PMC_LOG_TRACE("%s:%d %s maximun packet size exceeded\n",
                       __FILE__,__LINE__,__func__);
            PMC_RETURN(UTIL_OPSA_LOG_ERR_PKT_SIZE_RANGE_TOO_LARGE);
        }

    } else {
        /* 
         * Source node reference clock timing (B + C mode).
         * In this mode, we want Cn frame period (Tf) / packet period (Tb) = N
         * where N is an integer.
         */

        /* 
         * Find starting integer divider to use. 
         * N,start = floor( Tf / (packet period for max packet size) )
         * packet period for max packet size = max_pkt_size / f_client_byte
         */
        t_frame_div = (UINT32)PMC_FLOOR(t_frame/max_pkt_size * f_client_byte);
        PMC_LOG_TRACE("t_frame != 0...\n");
        /* Calculate the packet period for this divider. */
        pkt_per_temp = t_frame/t_frame_div * UTIL_OPSA_REF_FREQ;

        /* Use 1 for denominator if only using integer packet period. */
        if (UTIL_OPSA_INT_PKT_PER == pkt_period_calc) {
            pp_n_temp = round_pos(pkt_per_temp);
            pp_d_temp = 1;
        } else {
            /* Find fractional representation of the packet period */
            util_opsa_find_rational(pkt_per_temp, UTIL_OPSA_PKT_PER_D_MAX, &pp_n_temp, &pp_d_temp);
            pkt_per_temp_i = (DOUBLE) pp_n_temp / (DOUBLE) pp_d_temp;
            PMC_LOG_TRACE("pkt_per_temp:%f, pkt_per_temp_i:%f\n", pkt_per_temp, pkt_per_temp_i);
            PMC_LOG_TRACE("pp_n_temp:%d, pp_d_temp:%d\n", pp_n_temp, pp_d_temp);


        }

        /* Calculate the epsilon for this packet period */
        pkt_size_temp = round_pos((DOUBLE)pp_n_temp/pp_d_temp * f_client_byte/UTIL_OPSA_REF_FREQ); 
        eps_temp      = (DOUBLE)pp_n_temp/pp_d_temp * f_client_byte/UTIL_OPSA_REF_FREQ - pkt_size_temp;
        PMC_LOG_TRACE("pkt_size_temp = %u... min_pkt_size = %u... max_pkt_size = %u\n",
                   pkt_size_temp, min_pkt_size, max_pkt_size);
        /* Iterate for the desired packet range */
        while ((pkt_size_temp >= min_pkt_size) && (i < UTIL_OPSA_MAX_RESULT_CALC)) {
            PMC_LOG_TRACE("min pkt_size info passed...\n");
            /* Store result if packet size is within desired range */
            if (pkt_size_temp <= max_pkt_size) {
                PMC_LOG_TRACE("Index %u, pp_int %u, pp_n %u, pp_d %u, pkt_sz %u, eps %f\n",
                           i,
                           pp_n_temp/pp_d_temp,
                           pp_n_temp - (pp_n_temp/pp_d_temp)*pp_d_temp,
                           pp_d_temp,
                           pkt_size_temp,
                           eps_temp);
                if (!result_valid || (fabs(result_eps) > fabs(eps_temp))) {
                    PMC_LOG_TRACE("max pkt_size info passed...\n");
                    result_pp_n     = pp_n_temp;
                    result_pp_d     = pp_d_temp;
                    result_pkt_size = pkt_size_temp;
                    result_eps      = eps_temp;
                    result_valid    = TRUE;
                }
                i++;
            }
            PMC_LOG_TRACE("result_valid = %u\n", result_valid);
            /* Increment divider and calculate the packet period for this divider. */
            t_frame_div++;
            pkt_per_temp = t_frame/t_frame_div * UTIL_OPSA_REF_FREQ;

            /* Use 1 for denominator if only using integer packet period. */
            if (UTIL_OPSA_INT_PKT_PER == pkt_period_calc) {
                pp_n_temp = round_pos(pkt_per_temp);
                pp_d_temp = 1;
            } else {
                /* Find fractional representation of the packet period */
                util_opsa_find_rational(pkt_per_temp, UTIL_OPSA_PKT_PER_D_MAX, &pp_n_temp, &pp_d_temp);
                pkt_per_temp_i = (DOUBLE) pp_n_temp / (DOUBLE) pp_d_temp;
                PMC_LOG_TRACE("pkt_per_temp:%f, pkt_per_temp_i:%f\n", pkt_per_temp, pkt_per_temp_i);
                PMC_LOG_TRACE("pp_n_temp:%d, pp_d_temp:%d\n", pp_n_temp, pp_d_temp);
            }

            /* Calculate the epsilon for this packet period */
            pkt_size_temp = round_pos((DOUBLE)pp_n_temp/pp_d_temp * f_client_byte/UTIL_OPSA_REF_FREQ); 
            eps_temp      = (DOUBLE)pp_n_temp/pp_d_temp * f_client_byte/UTIL_OPSA_REF_FREQ - pkt_size_temp;
        }

        /* Check if the calculations reached the minimum packet size */
        if (i >= UTIL_OPSA_MAX_RESULT_CALC) {
            PMC_LOG_TRACE("%s:%d %s minimum packet size exceeded\n",
                       __FILE__,__LINE__,__func__);
            PMC_RETURN(UTIL_OPSA_LOG_ERR_PKT_SIZE_RANGE_TOO_LARGE);
        }
    } /* if (t_frame == 0) { */

    /* Check if any valid results were calculated. */
    if (!result_valid) {
        PMC_LOG_TRACE("%s:%d %s no valid results\n",
                   __FILE__,__LINE__,__func__);
        PMC_RETURN(UTIL_OPSA_LOG_ERR_NO_VALID_RESULTS);
    }

    /* Write return values */
    *pkt_per_n = result_pp_n;
    *pkt_per_d = result_pp_d;
    *pkt_size  = result_pkt_size;
    *epsilon   = result_eps;
    *n_ratio   = (DOUBLE)t_frame/((DOUBLE)*pkt_per_n/(DOUBLE)*pkt_per_d/UTIL_OPSA_REF_FREQ);

    /* Calculate integer portion of the packet period fraction and subtract from the numerator */
    *pkt_per_int = (*pkt_per_n)/(*pkt_per_d);
    *pkt_per_n   = *pkt_per_n - (*pkt_per_int) * (*pkt_per_d);
    
     /* Ensure that -0.5 < pkt_per_n/ pkt_per_d < 0.5 */
     if (*pkt_per_n > (*pkt_per_d/2)) {
        *pkt_per_int = *pkt_per_int + 1;
        *pkt_per_n  = *pkt_per_n - *pkt_per_d;
     }

    PMC_RETURN(PMC_SUCCESS);

} /* util_opsa_pkt_period_calc_double */

/*******************************************************************************
* util_opsa_pkt_period_calc_float
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Generates a packet period, packet size combo according to the 
*    general equation
*      pkt_size = pkt_period * f_client
*
* INPUTS:
*   f_client_bit    - client bit rate, in Hz.
*   t_frame         - Cn frame period.
*                     Set to 0 if not applicable.
*   min_pkt_size    - minimum packet size to choose 
*   max_pkt_size    - maximum packet size to choose 
*   pkt_period_calc - packet period and size calculation method.
*
* OUTPUTS:
*   pkt_per_int - Packet Period Integer 
*                 (in UTIL_OPSA_REF_FREQ clock cycles)
*                 (Packet Period = pkt_per_int + pkt_per_n/pkt_per_d)
*   pkt_per_n   - Packet Period Numerator
*   pkt_per_d   - Packet Period Denominator
*   pkt_size    - Packet Size (in bytes)
*   epsilon     - Epsilon
*   n_ratio     - ratio of t_frame / packet period
*                 Used for calculating eps_adder.
*
* RETURNS:
*   PMC_SUCCESS - if calculations completed successfully.
*   PMC_ERROR   - Error codes if error encountered.
*        
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_opsa_pkt_period_calc_float(DOUBLE f_client_bit, 
                                                 DOUBLE t_frame, 
                                                 UINT32 min_pkt_size, 
                                                 UINT32 max_pkt_size, 
                                                 util_opsa_pkt_period_calc_t pkt_period_calc,
                                                 UINT32 *pkt_per_int, 
                                                 UINT32 *pkt_per_n, 
                                                 UINT32 *pkt_per_d, 
                                                 UINT32 *pkt_size, 
                                                 DOUBLE *epsilon, 
                                                 DOUBLE *n_ratio)
{
    FLOAT f_client_byte = (FLOAT)f_client_bit / UTIL_OPSA_BIT_PER_BYTE;
    UINT32 pp_n_temp;
    UINT32 pp_d_temp;
    UINT32 pkt_size_temp;
    FLOAT eps_temp;
    UINT32 t_frame_div;
    FLOAT pkt_per_temp;
    FLOAT pkt_per_temp_i;

    /* Temp arrays to hold the results */
    UINT32 result_pp_n     = 0;
    UINT32 result_pp_d     = 0;
    UINT32 result_pkt_size = 0;
    FLOAT result_eps      = 0;
    BOOL   result_valid    = FALSE;

    UINT32 i = 0;

    PMC_ENTRY();

    /* Calculate packet period parameters based on the packet size range */
    if (t_frame == 0) {
        /* Local timing (C mode) */

        /* Use 1 for denominator if only using integer packet period. */
        if (UTIL_OPSA_INT_PKT_PER == pkt_period_calc) {
            pp_d_temp = 1;
        } else {
            pp_d_temp = UTIL_OPSA_PKT_PERIOD_STEP_DEFAULT;
        }

        /* Calculate the packet period numerator */
        pp_n_temp = (UINT32)PMC_FLOORF((FLOAT)min_pkt_size/(FLOAT)f_client_byte * UTIL_OPSA_REF_FREQ) * pp_d_temp;

        /* Calculate the epsilon for this numerator */
        pkt_size_temp = round_pos((FLOAT)pp_n_temp/pp_d_temp * (FLOAT)f_client_byte/UTIL_OPSA_REF_FREQ); 
        eps_temp      = (FLOAT)pp_n_temp/pp_d_temp * (FLOAT)f_client_byte/UTIL_OPSA_REF_FREQ - pkt_size_temp;

        /* Iterate for the desired packet range */
        while ((pkt_size_temp <= max_pkt_size) && (i < UTIL_OPSA_MAX_RESULT_CALC)) {
            /* Store result if packet size is within desired range */
            if (pkt_size_temp >= min_pkt_size) {
                PMC_LOG_TRACE("Index %u, pp_int %u, pp_n %u, pp_d %u, pkt_sz %u, eps %f\n",
                           i,
                           pp_n_temp/pp_d_temp,
                           pp_n_temp - (pp_n_temp/pp_d_temp)*pp_d_temp,
                           pp_d_temp,
                           pkt_size_temp,
                           eps_temp);
                if (!result_valid || (PMC_FABSF(result_eps) > PMC_FABSF(eps_temp))) {
                    result_pp_n     = pp_n_temp;
                    result_pp_d     = pp_d_temp;
                    result_pkt_size = pkt_size_temp;
                    result_eps      = eps_temp;
                    result_valid    = TRUE;
                }
                i++;
            }

            /* Increment the packet period numerator. */
            pp_n_temp++;

            /* Calculate the epsilon for this numerator */
            pkt_size_temp = round_pos((FLOAT)pp_n_temp/pp_d_temp * f_client_byte/UTIL_OPSA_REF_FREQ); 
            eps_temp      = (FLOAT)pp_n_temp/pp_d_temp * f_client_byte/UTIL_OPSA_REF_FREQ - pkt_size_temp;    
        }

        /* Check if the calculations reached the maximum packet size */
        if (i >= UTIL_OPSA_MAX_RESULT_CALC) {
            PMC_LOG_TRACE("%s:%d %s maximun packet size exceeded\n",
                       __FILE__,__LINE__,__func__);
            PMC_RETURN(UTIL_OPSA_LOG_ERR_PKT_SIZE_RANGE_TOO_LARGE);
        }

    } else {
        /* 
         * Source node reference clock timing (B + C mode).
         * In this mode, we want Cn frame period (Tf) / packet period (Tb) = N
         * where N is an integer.
         */

        /* 
         * Find starting integer divider to use. 
         * N,start = floor( Tf / (packet period for max packet size) )
         * packet period for max packet size = max_pkt_size / f_client_byte
         */
        t_frame_div = (UINT32)PMC_FLOORF((FLOAT)t_frame/max_pkt_size * f_client_byte);
        PMC_LOG_TRACE("t_frame != 0...\n");
        /* Calculate the packet period for this divider. */
        pkt_per_temp = (FLOAT)t_frame/t_frame_div * UTIL_OPSA_REF_FREQ_F;

        /* Use 1 for denominator if only using integer packet period. */
        if (UTIL_OPSA_INT_PKT_PER == pkt_period_calc) {
            pp_n_temp = round_pos(pkt_per_temp);
            pp_d_temp = 1;
        } else {
            /* Find fractional representation of the packet period */
            util_opsa_find_rational((DOUBLE)pkt_per_temp, UTIL_OPSA_PKT_PER_D_MAX, &pp_n_temp, &pp_d_temp);
            pkt_per_temp_i = (FLOAT) pp_n_temp / (FLOAT) pp_d_temp;
            PMC_LOG_TRACE("pkt_per_temp:%f, pkt_per_temp_i:%f\n", pkt_per_temp, pkt_per_temp_i);
            PMC_LOG_TRACE("pp_n_temp:%d, pp_d_temp:%d\n", pp_n_temp, pp_d_temp);


        }

        /* Calculate the epsilon for this packet period */
        pkt_size_temp = round_pos((FLOAT)pp_n_temp/pp_d_temp * f_client_byte/UTIL_OPSA_REF_FREQ_F); 
        eps_temp      = (FLOAT)pp_n_temp/pp_d_temp * f_client_byte/UTIL_OPSA_REF_FREQ_F - pkt_size_temp;
        PMC_LOG_TRACE("pkt_size_temp = %u... min_pkt_size = %u... max_pkt_size = %u\n",
                   pkt_size_temp, min_pkt_size, max_pkt_size);
        /* Iterate for the desired packet range */
        while ((pkt_size_temp >= min_pkt_size) && (i < UTIL_OPSA_MAX_RESULT_CALC)) {
            PMC_LOG_TRACE("min pkt_size info passed...\n");
            /* Store result if packet size is within desired range */
            if (pkt_size_temp <= max_pkt_size) {
                PMC_LOG_TRACE("Index %u, pp_int %u, pp_n %u, pp_d %u, pkt_sz %u, eps %f\n",
                           i,
                           pp_n_temp/pp_d_temp,
                           pp_n_temp - (pp_n_temp/pp_d_temp)*pp_d_temp,
                           pp_d_temp,
                           pkt_size_temp,
                           eps_temp);
                if (!result_valid || (PMC_FABSF(result_eps) > PMC_FABSF(eps_temp))) {
                    PMC_LOG_TRACE("max pkt_size info passed...\n");
                    result_pp_n     = pp_n_temp;
                    result_pp_d     = pp_d_temp;
                    result_pkt_size = pkt_size_temp;
                    result_eps      = eps_temp;
                    result_valid    = TRUE;
                }
                i++;
            }
            PMC_LOG_TRACE("result_valid = %u\n", result_valid);
            /* Increment divider and calculate the packet period for this divider. */
            t_frame_div++;
            pkt_per_temp = (FLOAT)t_frame/t_frame_div * UTIL_OPSA_REF_FREQ_F;

            /* Use 1 for denominator if only using integer packet period. */
            if (UTIL_OPSA_INT_PKT_PER == pkt_period_calc) {
                pp_n_temp = round_pos(pkt_per_temp);
                pp_d_temp = 1;
            } else {
                /* Find fractional representation of the packet period */
                util_opsa_find_rational((DOUBLE)pkt_per_temp, UTIL_OPSA_PKT_PER_D_MAX, &pp_n_temp, &pp_d_temp);
                pkt_per_temp_i = (FLOAT) pp_n_temp / (FLOAT) pp_d_temp;
                PMC_LOG_TRACE("pkt_per_temp:%f, pkt_per_temp_i:%f\n", pkt_per_temp, pkt_per_temp_i);
                PMC_LOG_TRACE("pp_n_temp:%d, pp_d_temp:%d\n", pp_n_temp, pp_d_temp);
            }

            /* Calculate the epsilon for this packet period */
            pkt_size_temp = round_pos((FLOAT)pp_n_temp/pp_d_temp * f_client_byte/UTIL_OPSA_REF_FREQ_F); 
            eps_temp      = (FLOAT)pp_n_temp/pp_d_temp * f_client_byte/UTIL_OPSA_REF_FREQ_F - pkt_size_temp;
        }

        /* Check if the calculations reached the minimum packet size */
        if (i >= UTIL_OPSA_MAX_RESULT_CALC) {
            PMC_LOG_TRACE("%s:%d %s minimum packet size exceeded\n",
                       __FILE__,__LINE__,__func__);
            PMC_RETURN(UTIL_OPSA_LOG_ERR_PKT_SIZE_RANGE_TOO_LARGE);
        }
    } /* if (t_frame == 0) { */

    /* Check if any valid results were calculated. */
    if (!result_valid) {
        PMC_LOG_TRACE("%s:%d %s no valid results\n",
                   __FILE__,__LINE__,__func__);
        PMC_RETURN(UTIL_OPSA_LOG_ERR_NO_VALID_RESULTS);
    }

    /* Write return values */
    *pkt_per_n = result_pp_n;
    *pkt_per_d = result_pp_d;
    *pkt_size  = result_pkt_size;
    *epsilon   = (DOUBLE)result_eps;
    *n_ratio   = (DOUBLE)((FLOAT)t_frame/((FLOAT)*pkt_per_n/(FLOAT)*pkt_per_d/UTIL_OPSA_REF_FREQ_F));

    /* Calculate integer portion of the packet period fraction and subtract from the numerator */
    *pkt_per_int = (*pkt_per_n)/(*pkt_per_d);
    *pkt_per_n   = *pkt_per_n - (*pkt_per_int) * (*pkt_per_d);
    
     /* Ensure that -0.5 < pkt_per_n/ pkt_per_d < 0.5 */
     if (*pkt_per_n > (*pkt_per_d/2)) {
        *pkt_per_int = *pkt_per_int + 1;
        *pkt_per_n  = *pkt_per_n - *pkt_per_d;
     }

    PMC_RETURN(PMC_SUCCESS);

} /* util_opsa_pkt_period_calc_float */

/*******************************************************************************
* util_opsa_pkt_period_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Generates a packet period, packet size combo according to the 
*    general equation
*      pkt_size = pkt_period * f_client
*
* INPUTS:
*   f_client_bit    - client bit rate, in Hz.
*   t_frame         - Cn frame period.
*                     Set to 0 if not applicable.
*   min_pkt_size    - minimum packet size to choose 
*   max_pkt_size    - maximum packet size to choose 
*   pkt_period_calc - packet period and size calculation method.
*
* OUTPUTS:
*   pkt_per_int - Packet Period Integer 
*                 (in UTIL_OPSA_REF_FREQ clock cycles)
*                 (Packet Period = pkt_per_int + pkt_per_n/pkt_per_d)
*   pkt_per_n   - Packet Period Numerator
*   pkt_per_d   - Packet Period Denominator
*   pkt_size    - Packet Size (in bytes)
*   epsilon     - Epsilon
*   n_ratio     - ratio of t_frame / packet period
*                 Used for calculating eps_adder.
*
* RETURNS:
*   PMC_SUCCESS - if calculations completed successfully.
*   PMC_ERROR   - Error codes if error encountered.
*        
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_opsa_pkt_period_calc(DOUBLE f_client_bit, DOUBLE t_frame, 
                                           UINT32 min_pkt_size, UINT32 max_pkt_size, 
                                           util_opsa_pkt_period_calc_t pkt_period_calc,
                                           UINT32 *pkt_per_int, UINT32 *pkt_per_n, UINT32 *pkt_per_d, 
                                           UINT32 *pkt_size, DOUBLE *epsilon, DOUBLE *n_ratio)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    if (!digi_use_float32_get())
    {
        rc = util_opsa_pkt_period_calc_double(f_client_bit, t_frame, 
                                            min_pkt_size, max_pkt_size, 
                                            pkt_period_calc,
                                            pkt_per_int, pkt_per_n, pkt_per_d, 
                                            pkt_size, epsilon, n_ratio);
    }
    else
    {
        rc = util_opsa_pkt_period_calc_float(f_client_bit, t_frame, 
                                            min_pkt_size, max_pkt_size, 
                                            pkt_period_calc,
                                            pkt_per_int, pkt_per_n, pkt_per_d, 
                                            pkt_size, epsilon, n_ratio);
    }

    PMC_RETURN(rc);
} /* util_opsa_pkt_period_calc */

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* util_opsa_pkt_period_calc_double
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Generates a packet period, packet size combo according to the 
*    general equation
*      pkt_size = pkt_period * f_client
*
* INPUTS:
*   f_client_bit    - client bit rate, in Hz.
*   max_fabric_pkt_size    - maximum packet size to choose 
*   pkt_period_calc - packet period and size calculation method.
*
* OUTPUTS:
*   pkt_per_int - Packet Period Integer 
*                 (in UTIL_OPSA_REF_FREQ clock cycles)
*                 (Packet Period = pkt_per_int + pkt_per_n/pkt_per_d)
*   pkt_per_n   - Packet Period Numerator
*   pkt_per_d   - Packet Period Denominator
*   pkt_size    - Packet Size (in bytes)
*   epsilon     - Epsilon
*
* RETURNS:
*   PMC_SUCCESS if calculations completed successfully.
*   Error codes if error encountered.
*        
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR util_opsa_oif_pkt_period_calc_double(DOUBLE f_client_bit, 
                                               UINT32 max_fabric_pkt_size, 
                                               util_opsa_pkt_period_calc_t pkt_period_calc,
                                               UINT32 *pkt_per_int, 
                                               UINT32 *pkt_per_n, 
                                               UINT32 *pkt_per_d, 
                                               UINT32 *pkt_size, 
                                               DOUBLE *epsilon)
{
    UINT32 pkt_size_min;
    UINT32 pkt_size_max;
    DOUBLE f_client_bit_gbps = f_client_bit/1000000000;
    DOUBLE f_client_bit_loc = f_client_bit;
    UINT32 seg_ratio_n;

    UINT32 d_max;
    UINT32 d_delta;
    UINT32 d_nom_tmax;
    DOUBLE d_avg;
    UINT32 d_nom;

    DOUBLE eps_ppm;
    DOUBLE eps_nom;
    DOUBLE bytes_per_ref_clk;

    UINT32 t_max;
    UINT32 t_adj;
    UINT32 t;

    PMC_ENTRY();
    /* 
     * Determine valid packet size range and pick the segmentation
     * ration (N) based on the fabric size.
     */
    switch (pkt_period_calc) {
    case UTIL_OPSA_USE_OIF_128_BYTE:
        pkt_size_min = UTIL_OPSA_OIF_128_MIN_BMAX;
        pkt_size_max = UTIL_OPSA_OIF_128_MAX_BMAX;

        if (f_client_bit_gbps <= UTIL_OPSA_OIF_SEG_RATE_1) {
            seg_ratio_n = UTIL_OPSA_OIF_128_SEG_N_1;
        } else if ((f_client_bit_gbps > UTIL_OPSA_OIF_SEG_RATE_1) &&
                   (f_client_bit_gbps <= UTIL_OPSA_OIF_SEG_RATE_2)) {
            seg_ratio_n = UTIL_OPSA_OIF_128_SEG_N_2;
        } else if ((f_client_bit_gbps > UTIL_OPSA_OIF_SEG_RATE_2) &&
                   (f_client_bit_gbps <= UTIL_OPSA_OIF_SEG_RATE_3)) {
            seg_ratio_n = UTIL_OPSA_OIF_128_SEG_N_3;
        } else {
            PMC_RETURN(UTIL_OPSA_LOG_ERR_INVALID_PARAMETERS);
        }
        break;
    case UTIL_OPSA_USE_OIF_256_BYTE:
        pkt_size_min = UTIL_OPSA_OIF_256_MIN_BMAX;
        pkt_size_max = UTIL_OPSA_OIF_256_MAX_BMAX;

        if (f_client_bit_gbps <= UTIL_OPSA_OIF_SEG_RATE_1) {
            seg_ratio_n = UTIL_OPSA_OIF_256_SEG_N_1;
        } else if ((f_client_bit_gbps > UTIL_OPSA_OIF_SEG_RATE_1) &&
                   (f_client_bit_gbps <= UTIL_OPSA_OIF_SEG_RATE_2)) {
            seg_ratio_n = UTIL_OPSA_OIF_256_SEG_N_2;
        } else if ((f_client_bit_gbps > UTIL_OPSA_OIF_SEG_RATE_2) &&
                   (f_client_bit_gbps <= UTIL_OPSA_OIF_SEG_RATE_3)) {
            seg_ratio_n = UTIL_OPSA_OIF_256_SEG_N_3;
        } else {
            PMC_RETURN(UTIL_OPSA_LOG_ERR_INVALID_PARAMETERS);
        }
        break;
    case UTIL_OPSA_USE_OIF_512_BYTE:
        pkt_size_min = UTIL_OPSA_OIF_512_MIN_BMAX;
        pkt_size_max = UTIL_OPSA_OIF_512_MAX_BMAX;

        if (f_client_bit_gbps <= UTIL_OPSA_OIF_SEG_RATE_1) {
            seg_ratio_n = UTIL_OPSA_OIF_512_SEG_N_1;
        } else if ((f_client_bit_gbps > UTIL_OPSA_OIF_SEG_RATE_1) &&
                   (f_client_bit_gbps <= UTIL_OPSA_OIF_SEG_RATE_2)) {
            seg_ratio_n = UTIL_OPSA_OIF_512_SEG_N_2;
        } else if ((f_client_bit_gbps > UTIL_OPSA_OIF_SEG_RATE_2) &&
                   (f_client_bit_gbps <= UTIL_OPSA_OIF_SEG_RATE_3)) {
            seg_ratio_n = UTIL_OPSA_OIF_512_SEG_N_3;
        } else {
            PMC_RETURN(UTIL_OPSA_LOG_ERR_INVALID_PARAMETERS);
        }
        break;
    default:
        PMC_RETURN(UTIL_OPSA_LOG_ERR_INVALID_PARAMETERS);
        break;
    }

    /* Check for valid packet size. */
    if ((max_fabric_pkt_size < pkt_size_min) ||
        (max_fabric_pkt_size > pkt_size_max)) {
        PMC_RETURN(UTIL_OPSA_LOG_ERR_INVALID_PARAMETERS);
    }

    /* Calculate required parameters */
    /* Dmax (maximum decision size) */
    d_max = seg_ratio_n * max_fabric_pkt_size;

    /* 
     * eps_ppm (offset from Dnom at maximum client rate ppm and reference clock ppm)
     * Use 100ppm for UTIL_OPSA_ODU_PPM_MAX since the end result is the same 
     * for the ODU rates with 20ppm tolerance. 
     */
    eps_ppm = d_max * (DOUBLE)(UTIL_OPSA_REF_FREQ_PPM_MAX + UTIL_OPSA_ODU_PPM_MAX)/1000000;

    /* 
     * D_delta (variance of decision size)
     * If N >= 4 then D_delta = ROUNDUP(2 * eps_max) else D_delta = 1
     * eps_max = eps_nom,max (0.5) + eps_ppm
     */
    if (seg_ratio_n >= 4) {
        d_delta = (UINT32)PMC_CEIL(2 * (0.5 + eps_ppm));
    } else {
        d_delta = 1;
    }
#ifdef DIGI_OIF_SPREADSHEET_INPUTS
    /* 
     * BpRC - number of ODU bytes received at the client rate in 
     * one reference clock tick.
     */
    if (f_client_bit_loc == UTIL_GLOBAL_ODU1_RATE) {
        f_client_bit_loc =   2498775E3;
    } else if (f_client_bit_loc == UTIL_GLOBAL_ODU2_RATE) {
        f_client_bit_loc =  10037274E3;
    } else if (f_client_bit_loc == UTIL_GLOBAL_ODU2E_RATE) {
        f_client_bit_loc =  10399525E3;
    } else if (f_client_bit_loc == UTIL_GLOBAL_ODU2F_RATE) {
        f_client_bit_loc = 106075155E2;
    } else if (f_client_bit_loc == UTIL_GLOBAL_ODU3_RATE) {
        f_client_bit_loc =  40319219E3;
    } else if (f_client_bit_loc == UTIL_GLOBAL_ODU3E1_RATE) {
        f_client_bit_loc =  41774364E3;
    } else if (f_client_bit_loc == UTIL_GLOBAL_ODU3E2_RATE) {
        f_client_bit_loc =  41785969E3;
    } else if (f_client_bit_loc == UTIL_GLOBAL_ODU4_RATE) {
        f_client_bit_loc = 104794446E3;
    }
#endif /* DIGI_OIF_SPREADSHEET_INPUTS */


    bytes_per_ref_clk = f_client_bit_loc / (UTIL_OPSA_REF_FREQ * 8);

    /* 
     * Tmax - integer portion of the number of reference clock ticks required 
     * to receive Dmax bytes at the nominal ODU bit rate. 
     * This is the maximum time between decisions.
     */
    t_max = (UINT32)((DOUBLE)d_max / bytes_per_ref_clk);

    /* Dnom,Tmax - nearest integer to the average number of bytes received in Tmax */
    d_nom_tmax = round_pos(t_max * bytes_per_ref_clk);

    /* 
     * For certain client rates, Tmax must be reduced by Tadj > 0 to avoid 
     * Dnom + D_delta > Dmax 
     * Tadj = ROUNDUP((Dnom,Tmax + D_delta  Dmax) / BpRC) or 0, whichever is greater
     */
    if (d_nom_tmax + d_delta > d_max) {
        t_adj = (UINT32)PMC_CEIL((d_nom_tmax + d_delta - d_max)/bytes_per_ref_clk);
    } else {
        t_adj = 0;
    }

    /* 
     * T is the actual time between decisions, the largest integer number of 
     * reference clocks that results in an acceptable Dnom + D_delta.
     */
    t =  t_max - t_adj;

    /* 
     * Davg - average number of decision bytes in T 
     * Dnom - nearest integer to Davg
     */
    d_avg = t * bytes_per_ref_clk;
    d_nom = round_pos(d_avg);

    /* eps_nom - fractional offset from Dnom to Davg */
    eps_nom = d_avg - d_nom;

    /* 
     * For each decision D, the ODU data stream is segmented into exactly N packets, 
     * where each packet is of size B bytes equal to one of [Bnom1, Bnom, Bnom+1]
     */
    *pkt_size = round_pos(d_avg/seg_ratio_n);
    if (*pkt_size > (max_fabric_pkt_size - 1)) {
        *pkt_size = max_fabric_pkt_size - 1;
    }

    /* Calculate packet period. */
    *pkt_per_int = t/seg_ratio_n;
    *pkt_per_n   = t - (*pkt_per_int * seg_ratio_n);
    *pkt_per_d   = seg_ratio_n;

     /* Ensure that -0.5 < pkt_per_n/ pkt_per_d < 0.5 */
     if (*pkt_per_n > (*pkt_per_d/2)) {
        *pkt_per_int = *pkt_per_int + 1;
        *pkt_per_n  = *pkt_per_n - *pkt_per_d;
     }

    *epsilon = eps_nom;

    PMC_RETURN(PMC_SUCCESS);

} /* util_opsa_oif_pkt_period_calc_double */

/*******************************************************************************
* util_opsa_pkt_period_calc_float
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Generates a packet period, packet size combo according to the 
*    general equation
*      pkt_size = pkt_period * f_client
*
* INPUTS:
*   _f_client_bit    - client bit rate, in Hz.
*   max_fabric_pkt_size    - maximum packet size to choose 
*   pkt_period_calc - packet period and size calculation method.
*
* OUTPUTS:
*   pkt_per_int - Packet Period Integer 
*                 (in UTIL_OPSA_REF_FREQ clock cycles)
*                 (Packet Period = pkt_per_int + pkt_per_n/pkt_per_d)
*   pkt_per_n   - Packet Period Numerator
*   pkt_per_d   - Packet Period Denominator
*   pkt_size    - Packet Size (in bytes)
*   epsilon     - Epsilon
*
* RETURNS:
*   PMC_SUCCESS if calculations completed successfully.
*   Error codes if error encountered.
*        
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR util_opsa_oif_pkt_period_calc_float(DOUBLE _f_client_bit, 
                                               UINT32 max_fabric_pkt_size, 
                                               util_opsa_pkt_period_calc_t pkt_period_calc,
                                               UINT32 *pkt_per_int, 
                                               UINT32 *pkt_per_n,
                                               UINT32 *pkt_per_d, 
                                               UINT32 *pkt_size,
                                               DOUBLE *epsilon)
{
    FLOAT f_client_bit = (FLOAT)_f_client_bit;
    UINT32 pkt_size_min;
    UINT32 pkt_size_max;
    FLOAT f_client_bit_gbps = f_client_bit/1000000000;
    FLOAT f_client_bit_loc = f_client_bit;
    UINT32 seg_ratio_n;

    UINT32 d_max;
    UINT32 d_delta;
    UINT32 d_nom_tmax;
    FLOAT d_avg;
    UINT32 d_nom;

    FLOAT eps_ppm;
    FLOAT eps_nom;
    FLOAT bytes_per_ref_clk;

    UINT32 t_max;
    UINT32 t_adj;
    UINT32 t;

    PMC_ENTRY();
    /* 
     * Determine valid packet size range and pick the segmentation
     * ration (N) based on the fabric size.
     */
    switch (pkt_period_calc) {
    case UTIL_OPSA_USE_OIF_128_BYTE:
        pkt_size_min = UTIL_OPSA_OIF_128_MIN_BMAX;
        pkt_size_max = UTIL_OPSA_OIF_128_MAX_BMAX;

        if (f_client_bit_gbps <= UTIL_OPSA_OIF_SEG_RATE_1) {
            seg_ratio_n = UTIL_OPSA_OIF_128_SEG_N_1;
        } else if ((f_client_bit_gbps > UTIL_OPSA_OIF_SEG_RATE_1) &&
                   (f_client_bit_gbps <= UTIL_OPSA_OIF_SEG_RATE_2)) {
            seg_ratio_n = UTIL_OPSA_OIF_128_SEG_N_2;
        } else if ((f_client_bit_gbps > UTIL_OPSA_OIF_SEG_RATE_2) &&
                   (f_client_bit_gbps <= UTIL_OPSA_OIF_SEG_RATE_3)) {
            seg_ratio_n = UTIL_OPSA_OIF_128_SEG_N_3;
        } else {
            PMC_RETURN(UTIL_OPSA_LOG_ERR_INVALID_PARAMETERS);
        }
        break;
    case UTIL_OPSA_USE_OIF_256_BYTE:
        pkt_size_min = UTIL_OPSA_OIF_256_MIN_BMAX;
        pkt_size_max = UTIL_OPSA_OIF_256_MAX_BMAX;

        if (f_client_bit_gbps <= UTIL_OPSA_OIF_SEG_RATE_1) {
            seg_ratio_n = UTIL_OPSA_OIF_256_SEG_N_1;
        } else if ((f_client_bit_gbps > UTIL_OPSA_OIF_SEG_RATE_1) &&
                   (f_client_bit_gbps <= UTIL_OPSA_OIF_SEG_RATE_2)) {
            seg_ratio_n = UTIL_OPSA_OIF_256_SEG_N_2;
        } else if ((f_client_bit_gbps > UTIL_OPSA_OIF_SEG_RATE_2) &&
                   (f_client_bit_gbps <= UTIL_OPSA_OIF_SEG_RATE_3)) {
            seg_ratio_n = UTIL_OPSA_OIF_256_SEG_N_3;
        } else {
            PMC_RETURN(UTIL_OPSA_LOG_ERR_INVALID_PARAMETERS);
        }
        break;
    case UTIL_OPSA_USE_OIF_512_BYTE:
        pkt_size_min = UTIL_OPSA_OIF_512_MIN_BMAX;
        pkt_size_max = UTIL_OPSA_OIF_512_MAX_BMAX;

        if (f_client_bit_gbps <= UTIL_OPSA_OIF_SEG_RATE_1) {
            seg_ratio_n = UTIL_OPSA_OIF_512_SEG_N_1;
        } else if ((f_client_bit_gbps > UTIL_OPSA_OIF_SEG_RATE_1) &&
                   (f_client_bit_gbps <= UTIL_OPSA_OIF_SEG_RATE_2)) {
            seg_ratio_n = UTIL_OPSA_OIF_512_SEG_N_2;
        } else if ((f_client_bit_gbps > UTIL_OPSA_OIF_SEG_RATE_2) &&
                   (f_client_bit_gbps <= UTIL_OPSA_OIF_SEG_RATE_3)) {
            seg_ratio_n = UTIL_OPSA_OIF_512_SEG_N_3;
        } else {
            PMC_RETURN(UTIL_OPSA_LOG_ERR_INVALID_PARAMETERS);
        }
        break;
    default:
        PMC_RETURN(UTIL_OPSA_LOG_ERR_INVALID_PARAMETERS);
        break;
    }

    /* Check for valid packet size. */
    if ((max_fabric_pkt_size < pkt_size_min) ||
        (max_fabric_pkt_size > pkt_size_max)) {
        PMC_RETURN(UTIL_OPSA_LOG_ERR_INVALID_PARAMETERS);
    }

    /* Calculate required parameters */
    /* Dmax (maximum decision size) */
    d_max = seg_ratio_n * max_fabric_pkt_size;

    /* 
     * eps_ppm (offset from Dnom at maximum client rate ppm and reference clock ppm)
     * Use 100ppm for UTIL_OPSA_ODU_PPM_MAX since the end result is the same 
     * for the ODU rates with 20ppm tolerance. 
     */
    eps_ppm = d_max * (FLOAT)(UTIL_OPSA_REF_FREQ_PPM_MAX + UTIL_OPSA_ODU_PPM_MAX)/1000000;

    /* 
     * D_delta (variance of decision size)
     * If N >= 4 then D_delta = ROUNDUP(2 * eps_max) else D_delta = 1
     * eps_max = eps_nom,max (0.5) + eps_ppm
     */
    if (seg_ratio_n >= 4) {
        d_delta = (UINT32)PMC_CEIL(2 * (0.5 + eps_ppm));
    } else {
        d_delta = 1;
    }
#ifdef DIGI_OIF_SPREADSHEET_INPUTS
    /* 
     * BpRC - number of ODU bytes received at the client rate in 
     * one reference clock tick.
     */
    if (f_client_bit_loc == UTIL_GLOBAL_ODU1_RATE) {
        f_client_bit_loc =   2498775E3F;
    } else if (f_client_bit_loc == UTIL_GLOBAL_ODU2_RATE) {
        f_client_bit_loc =  10037274E3F;
    } else if (f_client_bit_loc == UTIL_GLOBAL_ODU2E_RATE) {
        f_client_bit_loc =  10399525E3F;
    } else if (f_client_bit_loc == UTIL_GLOBAL_ODU2F_RATE) {
        f_client_bit_loc = 106075155E2F;
    } else if (f_client_bit_loc == UTIL_GLOBAL_ODU3_RATE) {
        f_client_bit_loc =  40319219E3F;
    } else if (f_client_bit_loc == UTIL_GLOBAL_ODU3E1_RATE) {
        f_client_bit_loc =  41774364E3F;
    } else if (f_client_bit_loc == UTIL_GLOBAL_ODU3E2_RATE) {
        f_client_bit_loc =  41785969E3F;
    } else if (f_client_bit_loc == UTIL_GLOBAL_ODU4_RATE) {
        f_client_bit_loc = 104794446E3F;
    }
#endif /* DIGI_OIF_SPREADSHEET_INPUTS */


    bytes_per_ref_clk = f_client_bit_loc / ((FLOAT)UTIL_OPSA_REF_FREQ * 8);

    /* 
     * Tmax - integer portion of the number of reference clock ticks required 
     * to receive Dmax bytes at the nominal ODU bit rate. 
     * This is the maximum time between decisions.
     */
    t_max = (UINT32)((FLOAT)d_max / bytes_per_ref_clk);

    /* Dnom,Tmax - nearest integer to the average number of bytes received in Tmax */
    d_nom_tmax = round_pos(t_max * bytes_per_ref_clk);

    /* 
     * For certain client rates, Tmax must be reduced by Tadj > 0 to avoid 
     * Dnom + D_delta > Dmax 
     * Tadj = ROUNDUP((Dnom,Tmax + D_delta  Dmax) / BpRC) or 0, whichever is greater
     */
    if (d_nom_tmax + d_delta > d_max) {
        t_adj = (UINT32)PMC_CEIL((d_nom_tmax + d_delta - d_max)/bytes_per_ref_clk);
    } else {
        t_adj = 0;
    }

    /* 
     * T is the actual time between decisions, the largest integer number of 
     * reference clocks that results in an acceptable Dnom + D_delta.
     */
    t =  t_max - t_adj;

    /* 
     * Davg - average number of decision bytes in T 
     * Dnom - nearest integer to Davg
     */
    d_avg = t * bytes_per_ref_clk;
    d_nom = round_pos(d_avg);

    /* eps_nom - fractional offset from Dnom to Davg */
    eps_nom = d_avg - d_nom;

    /* 
     * For each decision D, the ODU data stream is segmented into exactly N packets, 
     * where each packet is of size B bytes equal to one of [Bnom1, Bnom, Bnom+1]
     */
    *pkt_size = round_pos(d_avg/seg_ratio_n);
    if (*pkt_size > (max_fabric_pkt_size - 1)) {
        *pkt_size = max_fabric_pkt_size - 1;
    }

    /* Calculate packet period. */
    *pkt_per_int = t/seg_ratio_n;
    *pkt_per_n   = t - (*pkt_per_int * seg_ratio_n);
    *pkt_per_d   = seg_ratio_n;

     /* Ensure that -0.5 < pkt_per_n/ pkt_per_d < 0.5 */
     if (*pkt_per_n > (*pkt_per_d/2)) {
        *pkt_per_int = *pkt_per_int + 1;
        *pkt_per_n  = *pkt_per_n - *pkt_per_d;
     }

    *epsilon = (DOUBLE)(eps_nom);

    PMC_RETURN(PMC_SUCCESS);

} /* util_opsa_oif_pkt_period_calc_float */
#endif /* DOXYGEN_PUBLIC_ONLY */

/*******************************************************************************
* util_opsa_oif_pkt_period_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Generates a packet period, packet size combo according to the 
*   OIF recommendations.
*
* INPUTS:
*   f_client_bit        - client bit rate, in Hz.
*   max_fabric_pkt_size - maximum packet size supported by the fabric
*   pkt_period_calc     - packet period and size calculation method.
*
* OUTPUTS:
*   pkt_per_int - Packet Period Integer 
*                 (in UTIL_OPSA_REF_FREQ clock cycles)
*                 (Packet Period = pkt_per_int + pkt_per_n/pkt_per_d)
*   pkt_per_n   - Packet Period Numerator
*   pkt_per_d   - Packet Period Denominator
*   pkt_size    - Packet Size (in bytes)
*   epsilon     - Epsilon
*
* RETURNS:
*   PMC_SUCCESS if calculations completed successfully.
*   Error codes if error encountered.
*        
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_opsa_oif_pkt_period_calc(DOUBLE f_client_bit, UINT32 max_fabric_pkt_size, 
                                               util_opsa_pkt_period_calc_t pkt_period_calc,
                                               UINT32 *pkt_per_int, UINT32 *pkt_per_n, UINT32 *pkt_per_d, 
                                               UINT32 *pkt_size, DOUBLE *epsilon)
{
    BOOL8 use_float32_mode = digi_use_float32_get();
    PMC_ERROR result;

    PMC_ENTRY();

    if (!use_float32_mode) {
        result = util_opsa_oif_pkt_period_calc_double(f_client_bit, max_fabric_pkt_size,
                                                   pkt_period_calc,
                                                   pkt_per_int, pkt_per_n, pkt_per_d, 
                                                   pkt_size, epsilon);
    } else {
        result = util_opsa_oif_pkt_period_calc_float(f_client_bit, max_fabric_pkt_size,
                                                   pkt_period_calc,
                                                   pkt_per_int, pkt_per_n, pkt_per_d, 
                                                   pkt_size, epsilon);
    }

    PMC_RETURN(result);
} /* util_opsa_oif_pkt_period_calc */

/*******************************************************************************
* util_opsa_mpma_gain_calc_double
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Calculates the gain of the MPMA control loop based on the packet period.
*
* INPUTS:
*   pkt_period      - Packet Period (in seconds).
*   freq_corner     - Corner frequency in units of Hz.
*   formula_type    - Divisor used in formula
*                    FALSE: 16 bit divisor
*                    TRUE:  20 bit divisor
*
* OUTPUTS:
*   mpma_gain   - MPMA_GAIN value [3:0].
*
* RETURNS:
*   None.
*        
* NOTES:
*
*******************************************************************************/
PRIVATE void util_opsa_mpma_gain_calc_double(DOUBLE pkt_period, DOUBLE freq_corner, 
                                     BOOL8 formula_type, UINT32 *mpma_gain)
{
    DOUBLE mpma_gain_temp;
    DOUBLE multiplier;

    PMC_ENTRY();
    /*
     * Calculate the gain.
     * MPMA_GAIN = floor( log2( (2*pi*300*(PP_INT+PP_N/PP_D)/311.04e6*2^20) ) ) 
     * MPMA_GAIN = floor( log2( (2*pi*300*(PP_INT+PP_N/PP_D)/311.04e6*2^16) ) ) 
     *  where 300 is a frequency corner of 300 Hz.
     */ 
    multiplier = formula_type == TRUE ? 1048576:65536;

    mpma_gain_temp = 2 * UTIL_OPSA_PI * freq_corner * pkt_period * multiplier;
    /* Find the log2 and PMC_FLOOR. */
    mpma_gain_temp = PMC_MATH_LOG(mpma_gain_temp)/PMC_MATH_LOG(2);

    
    if (mpma_gain_temp > 8 && formula_type == FALSE) {
        mpma_gain_temp = 8;
    } else if (mpma_gain_temp > 12 && formula_type == TRUE) {
        mpma_gain_temp = 12;
    } else if (mpma_gain_temp < 0) {
        mpma_gain_temp = 0;
    }

    *mpma_gain = (UINT32)PMC_FLOOR(mpma_gain_temp);

    PMC_RETURN();
} /* util_opsa_mpma_gain_calc_double */

/*******************************************************************************
* util_opsa_mpma_gain_calc_float
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Calculates the gain of the MPMA control loop based on the packet period.
*
* INPUTS:
*   pkt_period      - Packet Period (in seconds).
*   freq_corner     - Corner frequency in units of Hz.
*   formula_type    - Divisor used in formula
*                    FALSE: 16 bit divisor
*                    TRUE:  20 bit divisor
*
* OUTPUTS:
*   mpma_gain   - MPMA_GAIN value [3:0].
*
* RETURNS:
*   None.
*        
* NOTES:
*
*******************************************************************************/
PRIVATE void util_opsa_mpma_gain_calc_float(DOUBLE pkt_period, DOUBLE freq_corner, 
                                     BOOL8 formula_type, UINT32 *mpma_gain)
{
    FLOAT mpma_gain_temp;
    FLOAT multiplier;

    PMC_ENTRY();
    /*
     * Calculate the gain.
     * MPMA_GAIN = floor( log2( (2*pi*300*(PP_INT+PP_N/PP_D)/311.04e6*2^20) ) ) 
     * MPMA_GAIN = floor( log2( (2*pi*300*(PP_INT+PP_N/PP_D)/311.04e6*2^16) ) ) 
     *  where 300 is a frequency corner of 300 Hz.
     */ 
    multiplier = formula_type == TRUE ? 1048576:65536;

    mpma_gain_temp = 2 * (FLOAT)UTIL_OPSA_PI * (FLOAT)freq_corner * (FLOAT)pkt_period * multiplier;
    /* Find the log2 and PMC_FLOOR. */
    mpma_gain_temp = PMC_MATH_LOGF(mpma_gain_temp)/PMC_MATH_LOGF(2);

    
    if (mpma_gain_temp > 8 && formula_type == FALSE) {
        mpma_gain_temp = 8;
    } else if (mpma_gain_temp > 12 && formula_type == TRUE) {
        mpma_gain_temp = 12;
    } else if (mpma_gain_temp < 0) {
        mpma_gain_temp = 0;
    }

    *mpma_gain = (UINT32)PMC_FLOORF(mpma_gain_temp);

    PMC_RETURN();
} /* util_opsa_mpma_gain_calc_float */

/*******************************************************************************
* util_opsa_mpma_gain_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Calculates the gain of the MPMA control loop based on the packet period.
*
* INPUTS:
*   pkt_period      - Packet Period (in seconds).
*   freq_corner     - Corner frequency in units of Hz.
*   formula_type    - Divisor used in formula
*                    FALSE: 16 bit divisor
*                    TRUE:  20 bit divisor
*
* OUTPUTS:
*   mpma_gain   - MPMA_GAIN value [3:0].
*
* RETURNS:
*   None.
*        
* NOTES:
*
*******************************************************************************/
PUBLIC void util_opsa_mpma_gain_calc(DOUBLE pkt_period, DOUBLE freq_corner, 
                                     BOOL8 formula_type, UINT32 *mpma_gain)
{
    PMC_ENTRY();

    if (!digi_use_float32_get()) {
        util_opsa_mpma_gain_calc_double(pkt_period, freq_corner, 
                                     formula_type, mpma_gain);
    } else {
        util_opsa_mpma_gain_calc_float(pkt_period, freq_corner, 
                                     formula_type, mpma_gain);
    }

    PMC_RETURN();
} /* util_opsa_mpma_gain_calc */

/*******************************************************************************
* util_opsa_mpmo_lpf_gain_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Calculates the gain of the MPMO low pass filter.
*    The larger the value of LPF_GAIN, the higher the cutoff frequency 
*    of the filter (less filtering).
*
* INPUTS:
*   frm_period_n     - Cn frame period numerator [31:0]
*                      (in UTIL_OPSA_REF_FREQ clock cycles)
*   frm_period_d     - Cn frame period denominator [23:0]
*
* OUTPUTS:
*   lpf_gain   - LPF_GAIN value [3:0].
*
* RETURNS:
*   None.
*        
* NOTES:
*
*******************************************************************************/
PUBLIC void util_opsa_mpmo_lpf_gain_calc(UINT32 frm_period_n, UINT32 frm_period_d,
                                         UINT32 *lpf_gain)
{
    DOUBLE t_frame;
    DOUBLE lpf_gain_temp;
    FLOAT f_t_frame;
    FLOAT f_lpf_gain_temp;
    UINT32 lpf_fc;

    PMC_ENTRY();
    lpf_fc = 300;

    /* 
     * Calculate the LPF gain. 
     * lpf_gain = PMC_FLOOR( log2( 2*pi*300*FrmPeriod*2^16))
     */
    if (!digi_use_float32_get()) {
        t_frame = (DOUBLE)frm_period_n / frm_period_d / UTIL_OPSA_REF_FREQ;
        lpf_gain_temp = 2 * UTIL_OPSA_PI * lpf_fc * t_frame * 65536;

        /* Find the log2 and PMC_FLOOR. */
        lpf_gain_temp = PMC_MATH_LOG(lpf_gain_temp)/PMC_MATH_LOG(2);
        *lpf_gain = (UINT32)PMC_FLOOR(lpf_gain_temp);
    } else {
        f_t_frame = (FLOAT)frm_period_n / frm_period_d / (FLOAT)UTIL_OPSA_REF_FREQ;
        f_lpf_gain_temp = 2 * (FLOAT)UTIL_OPSA_PI * lpf_fc * f_t_frame * 65536;

        /* Find the log2 and PMC_FLOOR. */
        f_lpf_gain_temp = PMC_MATH_LOGF(f_lpf_gain_temp)/PMC_MATH_LOGF(2);
        *lpf_gain = (UINT32)PMC_FLOORF(f_lpf_gain_temp);
    }

    PMC_RETURN();
} /* util_opsa_mpmo_lpf_gain_calc */

/*******************************************************************************
* util_opsa_mpmo_cn_base_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Calculates the MPMO Cn,base value from the Cn frame period, 
*    packet period and packet size according to the general equation
*      Cn,base = frame_period/packet_period * pkt_size
*
* INPUTS:
*   frm_period_n     - Cn frame period numerator [31:0]
*                      (in UTIL_OPSA_REF_FREQ clock cycles)
*   frm_period_d     - Cn frame period denominator [23:0]
*   pkt_per_int      - Packet Period Integer [15:0]
*                      (in UTIL_OPSA_REF_FREQ clock cycles)
*                     (Packet Period = pkt_per_int + pkt_per_n/pkt_per_d)
*   pkt_per_n        - Packet Period Numerator [7:0]
*   pkt_per_d        - Packet Period Denominator [7:0]
*   pkt_size         - packet size per packet period (in bytes). [8:0]
*
* OUTPUTS:
*   *cn_base_int_ptr - Cn,base Integer [23:0]
*                    (Cn,base = cn_base_int + cn_base_n/cn_base_d)
*   *cn_base_n_ptr   - Cn,base Numerator [31:0]
*   *cn_base_d_ptr   - Cn,base Denominator [31:0]
*
* RETURNS:
*   None.
*        
* NOTES:
*
*******************************************************************************/
PUBLIC void util_opsa_mpmo_cn_base_calc(UINT32 frm_period_n, UINT32 frm_period_d,
                                        UINT32 pkt_per_int, UINT32 pkt_per_n, UINT32 pkt_per_d, UINT32 pkt_size,
                                        UINT32 *cn_base_int_ptr, UINT32 *cn_base_n_ptr, UINT32 *cn_base_d_ptr)
{
    UINT32 pkt_period_n;
    DOUBLE cn_base_n;
    DOUBLE cn_base_d;
    DOUBLE cn_base_frac;
    DOUBLE remainder;
    FLOAT f_cn_base_n;
    FLOAT f_cn_base_d;
    FLOAT f_cn_base_frac;
    FLOAT f_remainder;
    PMC_ENTRY();

    if (!digi_use_float32_get())
    {
        /* 
         * Cn,base = frame_period (Tf) /packet_period (Tb) * pkt_size (B)
         * Cn,base N = Tf_n * Tb_d * B
         * Cn,base D = Tf_d * Tb_n
         */
        cn_base_n = (DOUBLE)frm_period_n * pkt_per_d * pkt_size;
        pkt_period_n = (pkt_per_int * pkt_per_d) + (INT32)pkt_per_n;
        cn_base_d = (DOUBLE)frm_period_d * pkt_period_n;

        /* Find fractional representation of Cn,base */
        *cn_base_int_ptr = (UINT32)(cn_base_n/cn_base_d);
        cn_base_frac = cn_base_n/cn_base_d - *cn_base_int_ptr;
        /*util_opsa_find_rational(cn_base_frac, UTIL_OPSA_CN_BASE_D_MAX, cn_base_n_ptr, cn_base_d_ptr);*/
        remainder = cn_base_frac - (DOUBLE)PMC_FLOOR(cn_base_frac);
        util_opsa_find_rational_less_than_one(remainder, UTIL_OPSA_CN_BASE_D_MAX, cn_base_n_ptr, cn_base_d_ptr);
        
        
        PMC_LOG_TRACE("\n");
        PMC_LOG_TRACE("frm_period_n:%u frm_period_d:%u  pkt_per_int:%u  pkt_per_n:%u  pkt_per_d:%u  pkt_size:%u  cn_base_int_ptr:%u  cn_base_n_ptr:%u  cn_base_d_ptr:%u\n", 
                   frm_period_n, frm_period_d, pkt_per_int, (INT32)pkt_per_n, pkt_per_d, pkt_size, *cn_base_int_ptr, *cn_base_n_ptr, *cn_base_d_ptr);
        PMC_LOG_TRACE("cn_base_n:%f pkt_period_n:%u  cn_base_d:%f  cn_base_frac:%f  \n", 
                   cn_base_n, pkt_period_n, cn_base_d, cn_base_frac);          
        PMC_LOG_TRACE("\n");
    }
    else
    {
        /* 
         * Cn,base = frame_period (Tf) /packet_period (Tb) * pkt_size (B)
         * Cn,base N = Tf_n * Tb_d * B
         * Cn,base D = Tf_d * Tb_n
         */
        f_cn_base_n = (FLOAT)frm_period_n * pkt_per_d * pkt_size;
        pkt_period_n = (pkt_per_int * pkt_per_d) + (INT32)pkt_per_n;
        f_cn_base_d = (FLOAT)frm_period_d * pkt_period_n;

        /* Find fractional representation of Cn,base */
        *cn_base_int_ptr = (UINT32)(f_cn_base_n/f_cn_base_d);
        f_cn_base_frac = f_cn_base_n/f_cn_base_d - *cn_base_int_ptr;
        /*util_opsa_find_rational(cn_base_frac, UTIL_OPSA_CN_BASE_D_MAX, cn_base_n_ptr, cn_base_d_ptr);*/
        f_remainder = f_cn_base_frac - (FLOAT)PMC_FLOORF(f_cn_base_frac);
        util_opsa_find_rational_less_than_one((DOUBLE)f_remainder, UTIL_OPSA_CN_BASE_D_MAX, cn_base_n_ptr, cn_base_d_ptr);
        
        
        PMC_LOG_TRACE("\n");
        PMC_LOG_TRACE("frm_period_n:%u frm_period_d:%u  pkt_per_int:%u  pkt_per_n:%u  pkt_per_d:%u  pkt_size:%u  cn_base_int_ptr:%u  cn_base_n_ptr:%u  cn_base_d_ptr:%u\n", 
                   frm_period_n, frm_period_d, pkt_per_int, (INT32)pkt_per_n, pkt_per_d, pkt_size, *cn_base_int_ptr, *cn_base_n_ptr, *cn_base_d_ptr);
        PMC_LOG_TRACE("cn_base_n:%f pkt_period_n:%u  cn_base_d:%f  cn_base_frac:%f  \n", 
                   f_cn_base_n, pkt_period_n, f_cn_base_d, f_cn_base_frac);          
        PMC_LOG_TRACE("\n");
    }

    PMC_RETURN();
} /* util_opsa_mpmo_cn_base_calc */

/*******************************************************************************
* util_opsa_cn_frame_period_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Returns the fractions for the frame period for the specified 
*    Cn frame period.
*
* INPUTS:
*   frame_period  - supported Cn frame period.
*
* OUTPUTS:
*   *frm_per_n     - Numerator for the frame period
*   *frm_per_d     - Denominator for the frame period
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void util_opsa_cn_frame_period_get(util_opsa_cn_frame_period_t frame_period, 
                                          UINT32 *frm_per_n, UINT32 *frm_per_d)
{
    PMC_ENTRY();

    switch (frame_period) {
    case UTIL_OPSA_MAPOTN_ODU0_T_FRAME:
        *frm_per_n = MAPOTN_ODU0_T_FRAME_N;
        *frm_per_d = MAPOTN_ODU0_T_FRAME_D;
        break;
    case UTIL_OPSA_MAPOTN_ODU1_T_FRAME:
        *frm_per_n = MAPOTN_ODU1_T_FRAME_N;
        *frm_per_d = MAPOTN_ODU1_T_FRAME_D;
        break;
    case UTIL_OPSA_MAPOTN_ODU1E_T_FRAME:
        *frm_per_n = MAPOTN_ODU1E_T_FRAME_N;
        *frm_per_d = MAPOTN_ODU1E_T_FRAME_D;
        break; 
    case UTIL_OPSA_MAPOTN_ODU1F_T_FRAME:
        *frm_per_n = MAPOTN_ODU1F_T_FRAME_N;
        *frm_per_d = MAPOTN_ODU1F_T_FRAME_D;
        break;       
    case UTIL_OPSA_MAPOTN_ODU2_T_FRAME:
        *frm_per_n = MAPOTN_ODU2_T_FRAME_N;
        *frm_per_d = MAPOTN_ODU2_T_FRAME_D;
        break;
    case UTIL_OPSA_MAPOTN_ODU2E_T_FRAME:
        *frm_per_n = MAPOTN_ODU2E_T_FRAME_N;
        *frm_per_d = MAPOTN_ODU2E_T_FRAME_D;
        break;
    case UTIL_OPSA_MAPOTN_ODU2F_T_FRAME:
        *frm_per_n = MAPOTN_ODU2F_T_FRAME_N;
        *frm_per_d = MAPOTN_ODU2F_T_FRAME_D;
        break;
    case UTIL_OPSA_MAPOTN_ODU3_T_FRAME:
        *frm_per_n = MAPOTN_ODU3_T_FRAME_N;
        *frm_per_d = MAPOTN_ODU3_T_FRAME_D;
        break;
    case UTIL_OPSA_MAPOTN_ODU3E1_T_FRAME:
        *frm_per_n = MAPOTN_ODU3E1_T_FRAME_N;
        *frm_per_d = MAPOTN_ODU3E1_T_FRAME_D;
        break;
    case UTIL_OPSA_MAPOTN_ODU3E2_T_FRAME:
        *frm_per_n = MAPOTN_ODU3E2_T_FRAME_N;
        *frm_per_d = MAPOTN_ODU3E2_T_FRAME_D;
        break;
    case UTIL_OPSA_MAPOTN_ODU4_T_FRAME:
        *frm_per_n = MAPOTN_ODU4_T_FRAME_N;
        *frm_per_d = MAPOTN_ODU4_T_FRAME_D;
        break; 
    case UTIL_OPSA_COREOTN_ODU1_1G25_T_FRAME:
        *frm_per_n = COREOTN_ODU1_1G25_T_FRAME_N;
        *frm_per_d = COREOTN_ODU1_1G25_T_FRAME_D;
        break;
    case UTIL_OPSA_COREOTN_ODU2_1G25_T_FRAME:
        *frm_per_n = COREOTN_ODU2_1G25_T_FRAME_N;
        *frm_per_d = COREOTN_ODU2_1G25_T_FRAME_D;
        break; 
    case UTIL_OPSA_COREOTN_ODU2_2G5_T_FRAME:
        *frm_per_n = COREOTN_ODU2_2G5_T_FRAME_N;
        *frm_per_d = COREOTN_ODU2_2G5_T_FRAME_D;
        break;       
    case UTIL_OPSA_COREOTN_ODU3_1G25_T_FRAME:
        *frm_per_n = COREOTN_ODU3_1G25_T_FRAME_N;
        *frm_per_d = COREOTN_ODU3_1G25_T_FRAME_D;
        break;
    case UTIL_OPSA_COREOTN_ODU3_2G5_T_FRAME:
        *frm_per_n = COREOTN_ODU3_2G5_T_FRAME_N;
        *frm_per_d = COREOTN_ODU3_2G5_T_FRAME_D;
        break;
    case UTIL_OPSA_COREOTN_ODU3E1_2G5_T_FRAME:
        *frm_per_n = COREOTN_ODU3E1_2G5_T_FRAME_N;
        *frm_per_d = COREOTN_ODU3E1_2G5_T_FRAME_D;
        break;
    case UTIL_OPSA_COREOTN_ODU3E2_1G25_T_FRAME:
        *frm_per_n = COREOTN_ODU3E2_1G25_T_FRAME_N;
        *frm_per_d = COREOTN_ODU3E2_1G25_T_FRAME_D;
        break;
    case UTIL_OPSA_COREOTN_ODU4_1G25_T_FRAME:
        *frm_per_n = COREOTN_ODU4_1G25_T_FRAME_N;
        *frm_per_d = COREOTN_ODU4_1G25_T_FRAME_D;
        break;
    case UTIL_OPSA_COREOTN_RESERVED_1_T_FRAME:
        *frm_per_n = COREOTN_RESERVED_1_T_FRAME_N;
        *frm_per_d = COREOTN_RESERVED_1_T_FRAME_D;
        break;
    case UTIL_OPSA_COREOTN_RESERVED_2_T_FRAME:
        *frm_per_n = COREOTN_RESERVED_2_T_FRAME_N;
        *frm_per_d = COREOTN_RESERVED_2_T_FRAME_D;
        break; 
    default:
        /* UTIL_OPSA_T_FRAME_UNUSED */
        *frm_per_n = 0;
        *frm_per_d = 1;
        break;
    }

    PMC_RETURN();
} /* util_opsa_cn_frame_period_get */

/*******************************************************************************
* util_opsa_find_rational
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Finds rational approximation for a floating point number.
*
* INPUTS:
*   decimal - decimal number
*   maxden  - maximum value for denominator
*
* OUTPUTS:
*   num - numerator of the fraction representing the decimal
*   den - denominator of the fraction representing the decimal
*
* RETURNS:
*
* NOTES: 
*   Source code take from http://www.math.niu.edu/~rusin/known-math/95/rationalize
*   Find rational approximation to given real number
*   David Eppstein / UC Irvine / 8 Aug 1993
*
*   With corrections from Arno Formella, May 2008
*
*   usage: a.out r d
*     r is real number to approx
*     d is the maximum denominator allowed
*
*   based on the theory of continued fractions
*   if x = a1 + 1/(a2 + 1/(a3 + 1/(a4 + ...)))
*   then best approximation is found by truncating this series
*   (with some adjustments in the last term).
*
*   Note the fraction can be recovered as the first column of the matrix
*    ( a1 1 ) ( a2 1 ) ( a3 1 ) ...
*    ( 1  0 ) ( 1  0 ) ( 1  0 )
*   Instead of keeping the sequence of continued fraction terms,
*   we just keep the last partial product of these matrices.
*
*******************************************************************************/
PUBLIC void util_opsa_find_rational(DOUBLE decimal, UINT32 maxden, UINT32* num, UINT32* den)
{
    UINT32 m[2][2];
    DOUBLE x;
    UINT32 ai;
    UINT32 t;

    PMC_ENTRY();
    
    x = decimal;    
    /* initialize matrix */
    m[0][0] = m[1][1] = 1;
    m[0][1] = m[1][0] = 0;

    /* loop finding terms until denom gets too big */
    ai = (UINT32)x;
    while (m[1][0] * ai + m[1][1] <= maxden) {
        t = m[0][0] * ai + m[0][1];
        m[0][1] = m[0][0];
        m[0][0] = t;

        t = m[1][0] * ai + m[1][1];
        m[1][1] = m[1][0];
        m[1][0] = t;

        if (x == (DOUBLE)ai) 
            break;     /* AF: division by zero */

        x = 1/(x - (DOUBLE)ai);
        if (x > (DOUBLE)0x7FFFFFFF) 
            break;   /* AF: representation failure */
        ai = (UINT32)x;
    } 

    *num = m[0][0];
    *den = m[1][0];

    PMC_RETURN();
} /* util_opsa_find_rational */

/*******************************************************************************
* util_opsa_mpmo_latency_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Generates latency in reference clock cycles to be used in MPMO.
*
* INPUTS:
*   f_client_bit    - client bit rate, in Hz.
*   pkt_size        - packet size supported by the fabric
*   num_words       - latency in units of number of words(blocks). Set to 0 of  
*                     latency is to be applied in seconds
*
* OUTPUTS:
*   latency_refclk  - latency in reference clock cycles
*
* RETURNS:
*   PMC_SUCCESS if calculations completed successfully.
*   Error codes if error encountered.
*        
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_opsa_mpmo_latency_calc(DOUBLE f_client_bit, 
                                             UINT32 pkt_size, 
                                             UINT32 num_words,
                                             UINT32 *latency_refclk) 
{
    /* variable declaration */
    DOUBLE words_per_pkt;
    DOUBLE num_bytes_in_client;
    DOUBLE latency;
    DOUBLE remainder;
    UINT32 quotient;
    UINT32 latency_blk = 0;
    FLOAT f_words_per_pkt;
    FLOAT f_num_bytes_in_client;
    FLOAT f_latency;
    FLOAT f_remainder;

    PMC_ENTRY();   
    PMC_ASSERT(num_words != 0, UTIL_OPSA_ERR_CODE_ASSERT, 0, 0);

    if (!digi_use_float32_get())
    {
        /* calculate words per packet
           words_per_pkt = PMC_CEIL[((pkt_size + UTIL_OPSA_HEADER_SIZE) / UTIL_OPSA_DPI_WORD_SIZE), 1] */
        words_per_pkt = (DOUBLE)((pkt_size + UTIL_OPSA_HEADER_SIZE) / (DOUBLE)UTIL_OPSA_DPI_WORD_SIZE/1.0);
        quotient = (UINT32)words_per_pkt;
        
        remainder = words_per_pkt - quotient;
        if (remainder != 0)
        {
            words_per_pkt = (UINT32)words_per_pkt + 1;
        }

        /* calculate number of bytes of client data given the required depth and
           packet size */
        PMC_ASSERT(words_per_pkt != 0, UTIL_OPSA_LOG_ERR_DIVIDED_BY_ZERO, 0, 0);
        num_bytes_in_client = (DOUBLE)(num_words / words_per_pkt) * (DOUBLE)(pkt_size/1.0);
        num_bytes_in_client = PMC_FLOOR(num_bytes_in_client);
        
        PMC_ASSERT(f_client_bit != 0, UTIL_OPSA_LOG_ERR_DIVIDED_BY_ZERO, 0, 0);
        latency = (DOUBLE)num_bytes_in_client * 8/ f_client_bit;
        
        PMC_LOG_TRACE("latency = %f\n", latency);
        PMC_LOG_TRACE("latency * UTIL_OPSA_REF_FREQ = %f\n", latency * UTIL_OPSA_REF_FREQ);
        latency_blk = round_pos(latency * UTIL_OPSA_REF_FREQ);
        
        *latency_refclk = latency_blk;
    }
    else
    {
        /* calculate words per packet
           words_per_pkt = PMC_CEIL[((pkt_size + UTIL_OPSA_HEADER_SIZE) / UTIL_OPSA_DPI_WORD_SIZE), 1] */
        f_words_per_pkt = (FLOAT)((pkt_size + UTIL_OPSA_HEADER_SIZE) / (FLOAT)UTIL_OPSA_DPI_WORD_SIZE/1.0);
        quotient = (UINT32)f_words_per_pkt;
        
        f_remainder = f_words_per_pkt - quotient;
        if (f_remainder != 0)
        {
            f_words_per_pkt = (UINT32)f_words_per_pkt + 1;
        }

        /* calculate number of bytes of client data given the required depth and
           packet size */
        PMC_ASSERT(f_words_per_pkt != 0, UTIL_OPSA_LOG_ERR_DIVIDED_BY_ZERO, 0, 0);
        f_num_bytes_in_client = (FLOAT)(num_words / f_words_per_pkt) * (FLOAT)(pkt_size/1.0);
        f_num_bytes_in_client = PMC_FLOORF(f_num_bytes_in_client);
        
        PMC_ASSERT(f_client_bit != 0, UTIL_OPSA_LOG_ERR_DIVIDED_BY_ZERO, 0, 0);
        f_latency = f_num_bytes_in_client * 8/ (FLOAT)f_client_bit;
        
        PMC_LOG_TRACE("latency = %f\n", f_latency);
        PMC_LOG_TRACE("latency * UTIL_OPSA_REF_FREQ = %f\n", f_latency * UTIL_OPSA_REF_FREQ_F);
        latency_blk = round_pos(f_latency * UTIL_OPSA_REF_FREQ_F);
        
        *latency_refclk = latency_blk;
    }

    PMC_RETURN(PMC_SUCCESS);
} /* util_opsa_mpmo_latency_calc */ 

/*******************************************************************************
* util_opsa_find_rational_less_than_one
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Finds rational approximation for a floating point number.
*   can only be used if (0 <= decimal <= 1.0)
*
*   note: this function works for large values of maxden, whereas
*   util_opsa_find_rational will fail if (maxden * decimal > 0xffffffff)
*
* INPUTS:
*   decimal - decimal number
*   maxden  - maximum value for denominator
*
* OUTPUTS:
*   num - numerator of the fraction representing the decimal
*   den - denominator of the fraction representing the decimal
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES: 
*   Source code take from http://www.math.niu.edu/~rusin/known-math/95/rationalize
*   Find rational approximation to given real number
*   David Eppstein / UC Irvine / 8 Aug 1993
*
*   With corrections from Arno Formella, May 2008
*
*   based on the theory of continued fractions
*   if x = a1 + 1/(a2 + 1/(a3 + 1/(a4 + ...)))
*   then best approximation is found by truncating this series
*   (with some adjustments in the last term).
*
*   Note the fraction can be recovered as the first column of the matrix
*    ( a1 1 ) ( a2 1 ) ( a3 1 ) ...
*    ( 1  0 ) ( 1  0 ) ( 1  0 )
*   Instead of keeping the sequence of continued fraction terms,
*   we just keep the last partial product of these matrices.
*
*******************************************************************************/
PUBLIC PMC_ERROR util_opsa_find_rational_less_than_one(DOUBLE decimal, UINT32 maxden, UINT32* num, UINT32* den)
{
    UINT32 m[2][2];
    UINT32 ai;
    UINT32 t;
    UINT32 iterations = 0;
    BOOL8  use_float32_mode = FALSE;

    PMC_ENTRY();

    if ((decimal > 1.0) || (decimal < 0)) {
         PMC_LOG_TRACE("util_opsa_find_rational_less_than_one - decimal %g invalid, must be [0..1]\n",decimal);
         PMC_ASSERT(NULL, UTIL_OPSA_LOG_ERR_INVALID_PARAMETERS, decimal, 0);
    }

    use_float32_mode = digi_use_float32_get();

    /* initialize matrix */
    m[0][0] = m[1][1] = 1;
    m[0][1] = m[1][0] = 0;

    /* loop finding terms until denom gets too big */
    ai = (UINT32)decimal;
    while (1.0 * m[1][0] * ai + m[1][1] <= 1.0 * maxden) {
        iterations++;
        if (iterations > 50)
            break;
        t = m[0][0] * ai + m[0][1];
        m[0][1] = m[0][0];
        m[0][0] = t;

        t = m[1][0] * ai + m[1][1];
        m[1][1] = m[1][0];
        m[1][0] = t;

        /* The above series x = 1/(x-ai) suffers from successive rounding errors.
        This is an alternative method of calculating ai that uses the integer values
        of the previous two fractions and does not incur as much rounding error. */
        if (!use_float32_mode) {
            DOUBLE t1divt0;
            DOUBLE t1;
            DOUBLE t0 = (decimal -  1.0*m[0][0]/m[1][0]) * m[1][0];
            if (m[1][1] == 0) {
              t1 = (decimal *m[1][1] -  m[0][1]);
            } else {
              t1 = (decimal -  1.0*m[0][1]/m[1][1]) * m[1][1];
            }
            if ((t0 == 0.0) || (t1 == 0))
                break;
            t1divt0 = -t1 / t0;
            if (t1divt0 > 4294967295.0)
                break;   /* AF: representation failure */
            ai = (UINT32)t1divt0;
            if (ai == 0) {
                ai=1;
                /* this is also a bug, but seems to occur due to rounding problems. */
                /* this could also mean that there are problems where a continued
                 * fraction should be n but is accidentally rounded down to n-1. 
                 * for (n>1), if this is happening it seems to be only after a
                 * many iterations of the algorithm.
                 */
            }
        }
        else
        {
            FLOAT f_decimal = (FLOAT)decimal;
            FLOAT t1divt0;
            FLOAT t1;
            FLOAT t0 = (f_decimal -  1.0*m[0][0]/m[1][0]) * m[1][0];
            if (m[1][1] == 0) {
              t1 = (f_decimal *m[1][1] -  m[0][1]);
            } else {
              t1 = (f_decimal -  1.0*m[0][1]/m[1][1]) * m[1][1];
            }
            if ((t0 == 0.0) || (t1 == 0))
                break;
            t1divt0 = -t1 / t0;
            if (t1divt0 > 4294967295.0F)
                break;   /* AF: representation failure */
            ai = (UINT32)t1divt0;
            if (ai == 0) {
                ai=1;
                /* this is also a bug, but seems to occur due to rounding problems. */
                /* this could also mean that there are problems where a continued
                 * fraction should be n but is accidentally rounded down to n-1. 
                 * for (n>1), if this is happening it seems to be only after a
                 * many iterations of the algorithm.
                 */
            }
        }
                /*
                * This code has been tested by calling:
                *   x = rand();
                *   find_rational( x, 0xffffffff, &num, &den );
                *   and verifying (x - num/den == 0)
                * so it seems to be working (at least for 2,000,000 random floating point numbers)
                */
    } 

    *num = m[0][0];
    *den = m[1][0];
     PMC_RETURN(PMC_SUCCESS);
} /* util_opsa_find_rational_less_than_one */
    

/*******************************************************************************
* util_opsa_find_best_rate
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This function find the best rate in a list of rate 

* INPUTS:
*   rate - the rate
*   *candidate_rates - A list of candidate rates
*   num_candidate_rates - The number of candidate rates
*
* OUTPUTS:
*   best_rate_idx   - the best match index
*   error       - the error on the best match
*
* RETURNS:
*   None
*        
* NOTES:
*
*******************************************************************************/
PUBLIC void util_opsa_find_best_rate( DOUBLE rate ,
                                      DOUBLE *candidate_rates,                                      
                                      UINT32 num_candidate_rates,
                                      UINT32 *best_rate_idx,
                                      DOUBLE *error)
{
    UINT32 rate_itr;
    DOUBLE candidate_error;
    PMC_ENTRY();
    
    PMC_ASSERT(num_candidate_rates != 0, UTIL_OPSA_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    /* find the best */
    *error = 1;
    for(rate_itr = 0; rate_itr < num_candidate_rates; rate_itr+=1)
    {
        candidate_error = fabs((candidate_rates[rate_itr] - rate)/candidate_rates[rate_itr]);
        if (candidate_error < *error)
        {
            *error = candidate_error;
            *best_rate_idx = rate_itr;
        }            
    }
    
    PMC_RETURN();
} /* util_opsa_find_best_rate */


/*******************************************************************************
* util_opsa_find_best_rate_per_pkt_size_period
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This function find the best rate based on packet period/size combo and 
*    a list of rates. The function is also return the error of selected rate. 
*    User shall decide if the error make sens.

*    general equation
*      best_rate = pkt_size/pkt_period
*      error     = (rate-best_rate)/rate
*
* INPUTS:
*
*   pkt_size    - packet size
*   pkts_per_cn - packet per cn
*   cn_base - cn base
*   *candidate_rates - A list of candidate rates
*   *candidates_cn_frame - A list of candidate cn frame
*   num_candidate_rates - The number of candidate rates
*
* OUTPUTS:
*   best_rate_idx - the best match
*   error       - the error on the best match
*
* RETURNS:
*   None
*        
* NOTES:
*
*******************************************************************************/
PUBLIC void util_opsa_find_best_rate_per_pkt_size_period( UINT32 pkt_size,
                                                          UINT32 pkts_per_cn,
                                                          UINT32 cn_base,
                                                          DOUBLE *candidate_rates,
                                                          util_opsa_cn_frame_period_t *candidates_cn_frame,
                                                          UINT32 num_candidate_rates,
                                                          UINT32 *best_rate_idx,
                                                          DOUBLE *error)
{
    UINT32 rate_itr;
    DOUBLE pkt_per;
    DOUBLE candidate_error;
    DOUBLE calc_rate;
    FLOAT f_pkt_per;
    FLOAT f_candidate_error;
    FLOAT f_calc_rate;
    UINT32 t_frame_n;
    UINT32 t_frame_d;
    PMC_ENTRY();
    
    PMC_ASSERT(pkt_size != 0, UTIL_OPSA_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(num_candidate_rates != 0, UTIL_OPSA_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    if (!digi_use_float32_get())
    {
        /* find the best */
        *error = 1;
        for(rate_itr = 0; rate_itr < num_candidate_rates; rate_itr+=1)
        {
            if (UTIL_OPSA_MAPOTN_ODUFLEX_T_FRAME  == candidates_cn_frame[rate_itr])
            {
                pkt_per = ((DOUBLE)UTIL_OPSA_ODUFLEX_PAYLOAD_BYTES/candidate_rates[rate_itr])*UTIL_OPSA_REF_FREQ / (DOUBLE) pkts_per_cn ;         
            } else 
            {        
                util_opsa_cn_frame_period_get(candidates_cn_frame[rate_itr], &t_frame_n, &t_frame_d);
                pkt_per = ((DOUBLE)t_frame_n / (DOUBLE)t_frame_d)/ (DOUBLE)pkts_per_cn ;       
            }         
            calc_rate = ((pkt_size * UTIL_OPSA_REF_FREQ) / (pkt_per) ) * UTIL_OPSA_BIT_PER_BYTE;

            
            candidate_error = fabs((candidate_rates[rate_itr] - calc_rate)/candidate_rates[rate_itr]);
            if (candidate_error < *error)
            {
                *error = candidate_error;
                *best_rate_idx = rate_itr;
            }            
        }
    }
    else
    {
        /* find the best */
        *error = 1;
        for(rate_itr = 0; rate_itr < num_candidate_rates; rate_itr+=1)
        {
            if (UTIL_OPSA_MAPOTN_ODUFLEX_T_FRAME  == candidates_cn_frame[rate_itr])
            {
                f_pkt_per = ((FLOAT)UTIL_OPSA_ODUFLEX_PAYLOAD_BYTES/(FLOAT)candidate_rates[rate_itr])*UTIL_OPSA_REF_FREQ_F / (FLOAT) pkts_per_cn ;         
            } else 
            {        
                util_opsa_cn_frame_period_get(candidates_cn_frame[rate_itr], &t_frame_n, &t_frame_d);
                f_pkt_per = ((FLOAT)t_frame_n / (FLOAT)t_frame_d)/ (FLOAT)pkts_per_cn ;       
            }         
            f_calc_rate = ((pkt_size * UTIL_OPSA_REF_FREQ_F) / (f_pkt_per) ) * UTIL_OPSA_BIT_PER_BYTE;

            
            f_candidate_error = PMC_FABSF(((FLOAT)candidate_rates[rate_itr] - f_calc_rate)/(FLOAT)candidate_rates[rate_itr]);
            if (f_candidate_error < (FLOAT)(*error))
            {
                *error = (DOUBLE)f_candidate_error;
                *best_rate_idx = rate_itr;
            }            
        }
    }

    PMC_RETURN();
} /* util_opsa_find_best_rate_per_pkt_size_period */

/*******************************************************************************
* util_opsa_find_best_rate_per_cn_base
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This function find the best rate based on packet size/frame per/cn base and 
*    a list of rates. The function is also return the error of selected rate. 
*    User shall decide if the error make sens.

*    general equation
*    pkt_period = frm_period/cn_base * pkt_size
*    best_rate  = pkt_size/pkt_period
*    error      = (rate-best_rate)/rate
*
* INPUTS:
*
*   pkt_size         - packet size
*   cn_base_int      - Cn,base Integer [23:0]
*                      (Cn,base = cn_base_int + cn_base_n/cn_base_d)
*   cn_base_n        - Cn,base Numerator [31:0]
*   cn_base_d        - Cn,base Denominator [31:0]
*   cn_frm_per_n     - Numerator for the frame period
*   cn_frm_per_d     - Denominator for the frame period
*   *candidate_rates - A list of candidate rates
*   num_candidate_rates - The number of candidate rates
*
* OUTPUTS:
*   best_rate_idx - the best match index
*   error         - the error on the best match
*
* RETURNS:
*   None
*        
* NOTES:
*
*******************************************************************************/
PUBLIC void util_opsa_find_best_rate_per_cn_base( UINT32 pkt_size,
                                                  UINT32 cn_base_int, 
                                                  UINT32 cn_base_n, 
                                                  UINT32 cn_base_d,
                                                  UINT32 cn_frm_per_n, 
                                                  UINT32 cn_frm_per_d,
                                                  DOUBLE *candidate_rates,
                                                  UINT32 num_candidate_rates,
                                                  UINT32 *best_rate_idx,
                                                  DOUBLE *error)
{
    DOUBLE pkt_period;
    DOUBLE cn_base;
    DOUBLE frm_period;
    DOUBLE calc_rate;
    FLOAT f_pkt_period;
    FLOAT f_cn_base;
    FLOAT f_frm_period;
    FLOAT f_calc_rate;
    PMC_ENTRY();

    PMC_ASSERT(pkt_size != 0, UTIL_OPSA_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(num_candidate_rates != 0, UTIL_OPSA_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(cn_frm_per_d != 0, UTIL_OPSA_LOG_ERR_DIVIDED_BY_ZERO, 0, 0);

    /* doublify ...*/
    if (!digi_use_float32_get())
    {
        cn_base    = (cn_base_d==0)?cn_base_int:(DOUBLE)cn_base_int + (DOUBLE)cn_base_n/(DOUBLE)cn_base_d;
        frm_period = (DOUBLE) cn_frm_per_n/(DOUBLE) cn_frm_per_d;
        pkt_period = (frm_period/cn_base) * (DOUBLE) pkt_size;   
        PMC_ASSERT(pkt_period != 0, UTIL_OPSA_LOG_ERR_DIVIDED_BY_ZERO, 0, 0);
        calc_rate = (((DOUBLE)pkt_size * UTIL_OPSA_REF_FREQ) / (pkt_period) ) * UTIL_OPSA_BIT_PER_BYTE;

        /* find the best */
        util_opsa_find_best_rate(calc_rate,candidate_rates,num_candidate_rates,best_rate_idx,error);
    }
    else
    {
        f_cn_base    = (cn_base_d==0)?cn_base_int:(FLOAT)cn_base_int + (FLOAT)cn_base_n/(FLOAT)cn_base_d;
        f_frm_period = (FLOAT) cn_frm_per_n/(FLOAT) cn_frm_per_d;
        f_pkt_period = (f_frm_period/f_cn_base) * (FLOAT) pkt_size;   
        PMC_ASSERT(f_pkt_period != 0, UTIL_OPSA_LOG_ERR_DIVIDED_BY_ZERO, 0, 0);
        f_calc_rate = (((FLOAT)pkt_size * UTIL_OPSA_REF_FREQ_F) / (f_pkt_period) ) * UTIL_OPSA_BIT_PER_BYTE;

        /* find the best */
        util_opsa_find_best_rate((DOUBLE)f_calc_rate,candidate_rates,num_candidate_rates,best_rate_idx,error);
    }
    PMC_RETURN();
} /* util_opsa_find_best_rate_per_cn_base */
                                    
/*
** Private Functions
*/

/*
** End of file
*/

