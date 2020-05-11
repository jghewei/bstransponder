/*******************************************************************************
*   COPYRIGHT (C) 2011-2013 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*   Lineotn.c implements the Lineotn subsystem API that provides the following
*   functionalities: \n
*   1. Initializing  and instantiating the Lineotn subsystem \n
*   2. LINEOTN provisioning functions \n
*   3. LINEOTN run time configuration functions\n
*   4. LINEOTN performance monitoring functions \n
*
*   NOTES:
*
*******************************************************************************/

/*
** Include Files
*/

#include "lineotn_loc.h"
#include "lineotn_io.h"
#include "lineotn.h"
#include "otu_frm_io.h"
#include "opsm23_io.h"
#include "opsm4_io.h"
#include "otn_mld_io.h"
#include "sfi51_io.h"
#include "sysotn_io.h"
#include "sysotn_opsm4_io.h"
#include "fecstat_io.h"
#include "sfis_rx_io.h"
#include "sfis_tx_io.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/


/* This setting must be used when calling FW to configure the OTU4 rate clock */
#define LINEOTN_DEFAULT_FILL_LEVEL_XOFF_MIN   (18)


/*
** Local Macro Definitions
*/


/*
** Local Structures and Unions
*/

/*
** Local Variables
*/

/* Only include the strings if logging with text*/
#ifdef PMC_LOG_STRINGS

/*
* The following creates the LOG/ERR strings from above by pulling out the text
* portion of the LOG_ERR_TABLE above
*/
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) str,
const char LINEOTN_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    LINEOTN_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else  /* no strings */

const char LINEOTN_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_LOG_STRINGS */


PRIVATE PMC_ERROR lineotn_ctxt_reg_recover(lineotn_handle_t        *lineotn_handle,
                                           BOOL8                    is_lineotn_ss,   
                                           pmc_energy_state_t       top_energy_state_reg,
                                           lineotn_recover_var_t   *lineotn_var_reg,
                                           lineotn_energy_state_t  *lineotn_energy_state_reg);

PRIVATE PMC_ERROR lineotn_ctxt_audit(lineotn_handle_t       *lineotn_handle,
                                     BOOL8                   is_lineotn_ss,   
                                     lineotn_recover_var_t  *lineotn_var_reg,
                                     lineotn_energy_state_t *lineotn_energy_state_reg,
                                     lineotn_var_t          *lineotn_var_clean);

PRIVATE PMC_ERROR lineotn_ctxt_cleanup(lineotn_handle_t         *hndl,
                                       BOOL8                     is_lineotn_ss,   
                                       lineotn_recover_var_t    *lineotn_var_reg,
                                       lineotn_energy_state_t   *lineotn_energy_state_reg,
                                       lineotn_var_t            *lineotn_var_clean);

PRIVATE PMC_ERROR lineotn_oduk_type_to_frame_rate_convert(lineotn_handle_t          *h,
                                                          util_global_odukp_type_t   oduk_type,
                                                          lineotn_frame_rate_t      *frame_rate,
                                                          BOOL8                     *fuzzy);

PRIVATE void lineotn_stream_ctxt_init(lineotn_dsi_cfg_t  *stream_cfg_ptr);

PRIVATE PMC_ERROR lineotn_int_params_get(lineotn_handle_t          *lineotn_handle,
                                         UINT32                     dsi_stream,
                                         UINT32                     *otu_frm_idx,
                                         BOOL8                      *has_opsm4,
                                         BOOL8                      *has_opsm23,
                                         UINT32                     *opsm23_idx,
                                         BOOL8                      *has_sfis,
                                         UINT32                     *sfis_idx,
                                         BOOL8                      *has_sfi51,
                                         UINT32                     *sfi51_idx,
                                         UINT32                     *start_id,
                                         UINT32                     *end_id);

PRIVATE PMC_ERROR lineotn_sfis_int_chnl_enable(lineotn_handle_t        *lineotn_handle,
                                               UINT32                   sfis_link,
                                               lineotn_sfis_int_chnl_t *int_table_ptr,
                                               lineotn_sfis_int_chnl_t *int_en_table_ptr,
                                               BOOL                     enable);

PRIVATE PMC_ERROR lineotn_sfi51_int_chnl_enable(lineotn_handle_t        *lineotn_handle,
                                                UINT32                   sfi51_link,
                                                lineotn_sfi51_int_chnl_t *int_table_ptr,
                                                lineotn_sfi51_int_chnl_t *int_en_table_ptr,
                                                BOOL                     enable);

PRIVATE PMC_ERROR lineotn_sfis_int_chnl_clear(lineotn_handle_t        *lineotn_handle,
                                              UINT32                   sfis_link,
                                              lineotn_sfis_int_chnl_t *int_en_table_ptr,
                                              lineotn_sfis_int_chnl_t *int_table_ptr);

PRIVATE PMC_ERROR lineotn_sfi51_int_chnl_clear(lineotn_handle_t        *lineotn_handle,
                                               UINT32                   sfi51_link,
                                               lineotn_sfi51_int_chnl_t *int_en_table_ptr,
                                               lineotn_sfi51_int_chnl_t *int_table_ptr);

PRIVATE PMC_ERROR lineotn_sfis_int_chnl_retrieve(lineotn_handle_t         *lineotn_handle,
                                                 UINT32                    sfis_link,
                                                 lineotn_sfis_int_chnl_t  *filt_table_ptr,
                                                 lineotn_sfis_int_chnl_t  *int_table_ptr);

PRIVATE PMC_ERROR lineotn_sfi51_int_chnl_retrieve(lineotn_handle_t         *lineotn_handle,
                                                  UINT32                    sfi51_link,
                                                  lineotn_sfi51_int_chnl_t  *filt_table_ptr,
                                                  lineotn_sfi51_int_chnl_t  *int_table_ptr);

PRIVATE PMC_ERROR lineotn_sfis_int_chnl_enabled_check(lineotn_handle_t        *lineotn_handle,
                                                      UINT32                   sfis_link,
                                                      lineotn_sfis_int_chnl_t *int_en_table_ptr,
                                                      BOOL                    *int_found_ptr);

PRIVATE PMC_ERROR lineotn_sfi51_int_chnl_enabled_check(lineotn_handle_t        *lineotn_handle,
                                                       UINT32                   sfi51_link,
                                                       lineotn_sfi51_int_chnl_t *int_en_table_ptr,
                                                       BOOL                    *int_found_ptr);

/*
** Private Functions
*/

/*******************************************************************************
* lineotn_swap_tti_bytes
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is a helper function to be used in cases that host CPU is of
*   big endian type. The function will swap the bytes of the tti message in order
*   to change the endianity to little endian.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
* 
*   tti_buffer_ptr         - pointer to tti buffer to swap endianity for
*
* OUTPUTS:
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void lineotn_swap_tti_bytes(lineotn_handle_t *h,UINT32 *tti_buffer_ptr)
{
    UINT32 *tmp = tti_buffer_ptr;
    UINT8  i = 0;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(tti_buffer_ptr!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    
    /* 64 tti bytes accessed in 16 uint32 */
    for(i=0;i<16;i++)
    {
        /* This macro will swap the bytes on a LE CPU to support the byte order of the TTIs. The TTIs
           put the LSB at the top of the registers (bits 31:24), but in a TTI byte string passed into
           LINEOTN the bytes are passed in byte order (e.g. [0x01, 0x02, 0x03, 0x04]).  Since the /io
           routines write words this number in LE would be interpreted as 0x04030201 and the 0x01 which
           needs to go into the upper bits (31:24) would incorrectly end up in bits (7:0).  By
           swapping here the writes occur correctly.
           For BE, the byte array is interpreted correctly 0x01020304 and there is a swap in the
           customer layer for all register writes.*/
        *tmp = PMC_CPU_TO_BE_32(*tmp);
        tmp++;
    }

    PMC_RETURN();
}/* lineotn_swap_tti_bytes */


/*******************************************************************************
* lineotn_calc_dsi_range
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is a helper function to help calculate DSI streams range in
*   regards to dsi rate and dsi stream number.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - DSI Lane to configure (0..11): Specifies the first DSI
*                            lane in the set of one or more DSI lanes. 
*                             - for 10G dsi lanes (0..11)
*                             - for 40G dsi lane - the lowest lane number in
*                               the 40G group. for example: for 40G lanes
*                               residing at lanes (4-7) --> 4. 
*                             - for 100G lanes --> 0 
*
* OUTPUTS:
*   start_range            - pointer to unsigned user allocated buffer,dsi stream start range
*
*   end_range              - pointer to unsigned user allocated buffer,dsi stream end range +1
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void lineotn_calc_dsi_range(lineotn_handle_t *h,UINT32 dsi_stream,UINT8 *start_range,UINT8 *end_range)
{

    PMC_ENTRY();

    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(start_range!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(end_range!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINE_OTN_CFG_ALL_DSI_STREAMS == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    /* check if this is a single dsi stream configuration */
    if(LINE_OTN_CFG_ALL_DSI_STREAMS == dsi_stream)
    {
        *start_range = 0;
        *end_range = LINE_OTN_MAX_DSI_STREAMS;
    }
    else /*10G rates */
    {
        *start_range = dsi_stream;
        *end_range = dsi_stream+1;  /* + 1 needed for the 'for' loop */
    }

    PMC_RETURN();
}/* lineotn_calc_dsi_range */

/*******************************************************************************
* lineotn_dsi_params_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is a helper function to help calculate dsi streams post mld range,
*   mld hw interface type and ODU rate based on the original dsi stream, frame and
*   line otn configuration.
* 
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   lineotn_cfg            - enum, lineOTN overall configuration mode.
*                            this enum specifies the lineOTN context in which we are
*                            the current dsi stream is being provisioned.
*                            There are 19 possible lineOTN configurations specified within
*                            line_otn_cfg enum.
*
*   dsi_stream             - unsigned, dsi stream number to configure:\n
*                            - for 10G configurations valid values are: (0..11) 
*                             dsi_type must be set to LINE_OTN_DSI_TYPE_OTUK 
*                            - for OTL3.4 40G configurations valid values are: (0,4,8)- 
*                              first lane in each 40G group. 
*                              dsi_type must be: LINE_OTN_DSI_TYPE_OTL for OTL3.4 
*                            - for 40G SFIS configurations valid values are: (0,5) - 
*                              first lane in each SFIS 40G group.
*                              dsi_type must be: LINE_OTN_DSI_TYPE_SFIS 
*                            - for 40G SFI5.1 configurations valid values are: (0,17) - 
*                              first lane in each SFIS 40G group.
*                              dsi_type must be: LINE_OTN_DSI_TYPE_SFI_51_X (X=A,B)
*                            - for 100G configurations valid values are: (0) -
*                              dsi_type can be either:
*                              LINE_OTN_DSI_TYPE_OTL   for OTL4.10 configuration or 
*                              LINE_OTN_DSI_TYPE_SFIS  for SFIS based configurations 
*
*   dsi_type               - enumerated lineotn dsi stream type: \n
*                            - LINE_OTN_DSI_TYPE_SFI_51_A : marks SFI5.1 dsi lanes 0-16 
*                            - LINE_OTN_DSI_TYPE_SFI_51_B : marks SFI5.1 dsi lanes 17-33 
*                            - LINE_OTN_DSI_TYPE_SFIS     : marks SFIS dsi lanes 
*                            - LINE_OTN_DSI_TYPE_OTL      : marks OTL 3.4/4.10 dsi lanes 
*                            - LINE_OTN_DSI_TYPE_OTUK     : marks 10G OTUk lanes 
*
* OUTPUTS:
*   mld_interface_type     - pointer to enum,mld hw interface associated with the dsi stream
*
*   oduk_type              - pointer to enum,oduk rate relative to the otu rate in frame_rate
*
*   dsi_start              - pointer to unsigned,post mld dsi stream start range
*
*   dsi_end                - pointer to unsigned,post mld dsi stream end range
*
* RETURNS:
*   PMC_SUCCESS - on success or otherwise the LINEOTN_ERR_INVALID_PARAMETERS error code is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR lineotn_dsi_params_get(lineotn_handle_t           *h,
                                         lineotn_cfg_type_t         lineotn_cfg,
                                         UINT32                     dsi_stream,
                                         lineotn_dsi_type_t         dsi_type,
                                         mld_ifc_type_t             *mld_interface_type,
                                         util_global_odukp_type_t   *oduk_type,
                                         UINT32                     *dsi_start,
                                         UINT32                     *dsi_end)
{

    PMC_ERROR       rc = PMC_SUCCESS;
    lineotn_frame_rate_t frame_rate = LAST_LINE_OTN_FRAME_RATE;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(mld_interface_type!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(oduk_type!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_start!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_end!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               (LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream && LINE_OTN_DSI_TYPE_SFI_51_B == dsi_type),LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(lineotn_cfg < LAST_LINE_OTN_CFG,LINEOTN_ERR_INVALID_PARAMETERS,lineotn_cfg,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);


    /* if we have sfi 5.1 interface - first we need to get data from the extra config section of the handle  */
    if(LINE_OTN_DSI_TYPE_SFI_51_A == dsi_type )
        frame_rate = h->var.lineotn_sfi51_0_frame_rate;
    else if(LINE_OTN_DSI_TYPE_SFI_51_B == dsi_type )
        frame_rate = h->var.lineotn_sfi51_1_frame_rate;
    else if (dsi_stream<h->cfg.max_dsi_streams)
        frame_rate = h->var.lineotn_dsi_dist[dsi_stream].dsi_rate;

    if( frame_rate != LAST_LINE_OTN_FRAME_RATE )
    {
        /* get mld hw ifc type and oduk type */
        /* the mld HW ifc type is concluded  */
        /* from the configuration number or  */
        /* the dsi stream number or both.    */
        switch(frame_rate)
        {

        case LINE_OTN_RATE_OTU4:
            if(LINE_OTN_CFG_0==lineotn_cfg)
                (*mld_interface_type) = MLD_OTL;
            else /* other 2 configurations are SFIS */
                (*mld_interface_type) = MLD_SFIS;

            (*oduk_type) = UTIL_GLOBAL_ODU4;

            /* although some 100G configurations require less than 12 DSI streams */
            /* all 12 dsi streams are updated for code simplicity sake and also */
            /* because there cannot be another configuration present             */
            (*dsi_start) = 0;
            (*dsi_end)   = LINE_OTN_MAX_DSI_STREAMS-1;
            break;

        case LINE_OTN_RATE_OTU3E2:
        case LINE_OTN_RATE_OTU3E1:
        case LINE_OTN_RATE_OTU3:

            if(LINE_OTN_DSI_TYPE_OTL == dsi_type)
            {
                (*mld_interface_type) = MLD_OTL;
                (*dsi_start) = (dsi_stream/4)*4; /* dsi streams are 0-3,4-7,8-11*/
                (*dsi_end)   = (dsi_stream/4)*4 + 3;
            }
            else if(LINE_OTN_DSI_TYPE_SFIS == dsi_type)
            {
                (*mld_interface_type) = MLD_SFIS;       /* dsi streams are 0-4,5-9*/
                (*dsi_start) = (dsi_stream/4)*4;        /* map to post mld streams*/
                (*dsi_end)   = (dsi_stream/4)*4 + 3;    /* 0-3,4-7                */
            }
            else
            {
                (*mld_interface_type) = MLD_SFI_51;
                /* we are looking for the OPSM idx that is taken by   */
                /* the sfi 5.1 lanes. this will tell us which post    */
                /* mld dsi stream are occupied by the sfi 5.1 streams.*/
                rc = mld_sfi_51_dsi_idx_get(lineotn_cfg,dsi_type,dsi_start,dsi_end);
            }

            if( LINE_OTN_RATE_OTU3E2 == frame_rate )
                (*oduk_type) = UTIL_GLOBAL_ODU3E2;
            else if(LINE_OTN_RATE_OTU3E1 == frame_rate)
                (*oduk_type) = UTIL_GLOBAL_ODU3E1;
            else
                (*oduk_type) = UTIL_GLOBAL_ODU3;

            break;

        case LINE_OTN_RATE_OTU2F:
        case LINE_OTN_RATE_OTU2E:
        case LINE_OTN_RATE_OTU2:
        case LINE_OTN_RATE_OTU1F:
        case LINE_OTN_RATE_OTU1E:
            (*mld_interface_type) = MLD_OTU2;

            if(LINE_OTN_RATE_OTU2F == frame_rate)
                (*oduk_type) = UTIL_GLOBAL_ODU2F;
            else if(LINE_OTN_RATE_OTU2E == frame_rate)
                (*oduk_type) = UTIL_GLOBAL_ODU2E;
            else if (LINE_OTN_RATE_OTU2 == frame_rate)
                (*oduk_type) = UTIL_GLOBAL_ODU2;
            else if (LINE_OTN_RATE_OTU1F == frame_rate)
                (*oduk_type) = UTIL_GLOBAL_ODU1F;
            else
                (*oduk_type) = UTIL_GLOBAL_ODU1E;

            if (dsi_stream < LINE_OTN_MAX_DSI_STREAMS)
            {           
                (*dsi_start) = dsi_stream;
                (*dsi_end) = dsi_stream;
            }
            else {
                rc = LINEOTN_ERR_INVALID_PARAMETERS;
            }
            break;

        default:
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;
        }

    }
    else {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* lineotn_dsi_params_get */

/*******************************************************************************
* lineotn_oduk_type_to_frame_rate_convert
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Find frame rate associated to given packet oduk type. Fuzzy flag is also returned
*   when multiple frame rate might correspond to oduk type.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*   oduk_type              - packet ODUK type
*   
*
* OUTPUTS:
*   *frame_rate            - frame rate associated to packet ODUK type
*   *fuzzy                 - TRUE when multiple frame rates can be associated to 
*                            packet ODUK type.
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR lineotn_oduk_type_to_frame_rate_convert(lineotn_handle_t          *h,
                                                          util_global_odukp_type_t   oduk_type,
                                                          lineotn_frame_rate_t      *frame_rate,
                                                          BOOL8                     *fuzzy)
{

    PMC_ERROR       rc = PMC_SUCCESS;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h != NULL, LINEOTN_ERR_INVALID_PTR, 0, 0);
    PMC_ASSERT(frame_rate != NULL, LINEOTN_ERR_INVALID_PTR, 0, 0);

    switch(oduk_type)
    {        
    case UTIL_GLOBAL_ODU4:
        *frame_rate = LINE_OTN_RATE_OTU4;
        *fuzzy = TRUE; /* LINE_OTN_RATE_OTU4P: */
        break;
    case UTIL_GLOBAL_ODU3E2:
        *frame_rate = LINE_OTN_RATE_OTU3E2;
        *fuzzy = TRUE; /* LINE_OTN_RATE_OTU3E2P */
        break;
    case UTIL_GLOBAL_ODU3E1:
        *frame_rate = LINE_OTN_RATE_OTU3E1;
        *fuzzy = FALSE; 
        break;
    case UTIL_GLOBAL_ODU3:
        *frame_rate = LINE_OTN_RATE_OTU3;
        *fuzzy = TRUE; /*LINE_OTN_RATE_OTU3EP/LINE_OTN_RATE_OTU3: */
        break;
    case UTIL_GLOBAL_ODU2:
        *frame_rate = LINE_OTN_RATE_OTU2;
        *fuzzy = FALSE; 
        break;
    case UTIL_GLOBAL_ODU2F:
        *frame_rate = LINE_OTN_RATE_OTU2F;
        *fuzzy = FALSE; 
        break;
    case UTIL_GLOBAL_ODU2E:
        *frame_rate = LINE_OTN_RATE_OTU2E;
        *fuzzy = FALSE; 
        break;
    case UTIL_GLOBAL_ODU1F:
        *frame_rate = LINE_OTN_RATE_OTU1F;
        *fuzzy = FALSE; 
        break;
    case UTIL_GLOBAL_ODU1E:
        *frame_rate = LINE_OTN_RATE_OTU1E;
        *fuzzy = FALSE; 
        break;
    default:
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
        break;
    }

    PMC_RETURN(rc);
}/* lineotn_oduk_type_to_frame_rate_convert */


/*******************************************************************************
* lineotn_validate_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is a helper function to validate a provision request.
*   The request is validated based on 5 parameters: \n
*   1. the current lineotn configuration \n
*   2. the dsi stream to provision \n
*   3. the dsi type to provision \n
*   4. frame rate for the dsi stream \n
*   5. the requested route within the mld block \n\n
*
*   If provision is possible this function will return success indication to the calling
*   function.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   lineotn_cfg            - enum,lineOTN overall configuration mode.
*                            This enum specifies the lineOTN context in which we are
*                            the current dsi stream is being provisioned.
*                            There are 19 possible lineOTN configurations specified within
*                            line_otn_cfg enum. 
*
*   lineotn_deskew_type    - enum,lineotn required deskew type 
*
*   dsi_type               - enumerated lineotn dsi stream type
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   frame_rate             - enum, OTU frame rate provisioned for this DSI stream:
*                            (LINE_OTN_RATE_OTU1E ... LINE_OTN_RATE_OTU4).
*
* RETURNS:
*   PMC_SUCCESS - on success or the LINEOTN_ERR_PROVISION_ERR error code is returned
*
* NOTES:
*   1. This function assumes that the dsi lane allocation function has been invoked
*      successfully and all dsi streams are valid.
*
*******************************************************************************/
PRIVATE PMC_ERROR lineotn_validate_prov  ( lineotn_handle_t      *h,
                                           lineotn_cfg_type_t    lineotn_cfg,
                                           lineotn_deskew_type_t lineotn_deskew_type,
                                           lineotn_dsi_type_t    dsi_type,
                                           UINT32                dsi_stream,
                                           lineotn_frame_rate_t  frame_rate )
{

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    /* check for incorrect ranges */
    if( lineotn_cfg >= LAST_LINE_OTN_CFG)
        PMC_RETURN(LINEOTN_ERR_PROVISION_ERR);

    if (dsi_stream > 11 && dsi_stream != LINEOTN_SFI51_1_DSI_STREAM_IDX)
        PMC_RETURN(LINEOTN_ERR_PROVISION_ERR);

    if (lineotn_deskew_type >= LAST_LINE_OTN_DESKEW_TYPE)
        PMC_RETURN(LINEOTN_ERR_PROVISION_ERR);

    if (dsi_type >= LAST_LINE_OTN_DSI_TYPE)
        PMC_RETURN(LINEOTN_ERR_PROVISION_ERR);

    if (frame_rate >= LAST_LINE_OTN_FRAME_RATE)
        PMC_RETURN(LINEOTN_ERR_PROVISION_ERR);

    /* for OTU2/OTL mld route must be OTL */
    if ( (dsi_type == LINE_OTN_DSI_TYPE_OTL || dsi_type == LINE_OTN_DSI_TYPE_OTUK) && 
         lineotn_deskew_type !=  LINE_OTN_NO_DESKEW )
        PMC_RETURN(LINEOTN_ERR_PROVISION_ERR);

    /* for SFIS/SFI5.1 mld route cannot be OTL */
    if ( (dsi_type == LINE_OTN_DSI_TYPE_SFIS || dsi_type == LINE_OTN_DSI_TYPE_SFI_51_A || dsi_type == LINE_OTN_DSI_TYPE_SFI_51_B ) && 
         lineotn_deskew_type ==  LINE_OTN_NO_DESKEW )
        PMC_RETURN(LINEOTN_ERR_PROVISION_ERR);

    /* check for correct frame rates based for dsi type */
    if ( (dsi_type == LINE_OTN_DSI_TYPE_OTL || dsi_type == LINE_OTN_DSI_TYPE_SFIS) && 
         frame_rate  > LINE_OTN_RATE_OTU3 )
        PMC_RETURN(LINEOTN_ERR_PROVISION_ERR);

    /* sfi 5.1  can be only 40G */
    if ( (dsi_type == LINE_OTN_DSI_TYPE_SFI_51_A || dsi_type == LINE_OTN_DSI_TYPE_SFI_51_B ) && 
         (frame_rate >  LINE_OTN_RATE_OTU3 || frame_rate <  LINE_OTN_RATE_OTU3E2) )
        PMC_RETURN(LINEOTN_ERR_PROVISION_ERR);

    if ( dsi_type == LINE_OTN_DSI_TYPE_OTUK && 
         frame_rate <  LINE_OTN_RATE_OTU2F )
        PMC_RETURN(LINEOTN_ERR_PROVISION_ERR);

    /* SYSOTN does not support LINE_OTN_ENHANCED_SFIS_DESKEW */
    if( dsi_type == LINE_OTN_DSI_TYPE_SFIS &&
        lineotn_deskew_type == LINE_OTN_ENHANCED_SFIS_DESKEW &&
        h->var.lineotn_mode == FALSE)
        PMC_RETURN(LINEOTN_ERR_PROVISION_ERR);


    PMC_RETURN(PMC_SUCCESS);
}/* lineotn_validate_prov */


/*******************************************************************************
* lineotn_var_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes lineotn logical variables.
*
*
* INPUTS:
*   *var_ptr            - pointer to LINEOTN variable configuration
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void lineotn_var_init(lineotn_var_t *var_ptr)
{
    UINT8 i;
    
    PMC_ENTRY();

    /* Init Lineotn variables as not configured */
    for(i=0;i<LINE_OTN_MAX_DSI_STREAMS;i++)
    {
        var_ptr->lineotn_dsi_dist[i].dci_channel = FREE_CHANNEL;
        var_ptr->lineotn_dsi_dist[i].dsi_hw_ifc = LAST_MLD_IFC_TYPE;
        var_ptr->lineotn_dsi_dist[i].dsi_rate = LAST_LINE_OTN_FRAME_RATE;
        var_ptr->lineotn_dsi_dist[i].dsi_provisioned = FALSE;
        var_ptr->lineotn_dsi_dist[i].dsi_activated_rx= FALSE;
        var_ptr->lineotn_dsi_dist[i].dsi_activated_tx= FALSE;
        var_ptr->lineotn_dsi_dist[i].deskew_type = LAST_LINE_OTN_DESKEW_TYPE;
        var_ptr->lineotn_dsi_dist[i].bergen_enabled = FALSE;        
        var_ptr->lineotn_dsi_dist[i].scramble_in_opsm = FALSE;
    }

    var_ptr->current_lineotn_cfg = LAST_LINE_OTN_CFG; /* no configuration set */
    var_ptr->lineotn_sfi51_0_frame_rate = LAST_LINE_OTN_FRAME_RATE;
    var_ptr->lineotn_sfi51_1_frame_rate = LAST_LINE_OTN_FRAME_RATE;
    var_ptr->fw_ddeg = FALSE;
    fec_var_init(var_ptr);
    mld_var_init(var_ptr);

    PMC_RETURN();
}/* lineotn_var_init */

/*******************************************************************************
* lineotn_stream_ctxt_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes dsi stream configuration.
*
* INPUTS:
*   *stream_cfg_ptr            - Pointer to the stream configuration
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void lineotn_stream_ctxt_init(lineotn_dsi_cfg_t  *stream_cfg_ptr)
{
    
    PMC_ENTRY();

    /* Init Lineotn variables as not configured */
    stream_cfg_ptr->dci_channel = FREE_CHANNEL;
    stream_cfg_ptr->dsi_hw_ifc = LAST_MLD_IFC_TYPE;
    stream_cfg_ptr->dsi_rate = LAST_LINE_OTN_FRAME_RATE;
    stream_cfg_ptr->dsi_provisioned = FALSE;
    stream_cfg_ptr->dsi_activated_rx= FALSE;
    stream_cfg_ptr->dsi_activated_tx= FALSE;
    stream_cfg_ptr->deskew_type = LAST_LINE_OTN_DESKEW_TYPE;
    stream_cfg_ptr->dsi_fec_type = LAST_LINE_OTN_FEC;
    stream_cfg_ptr->dsi_fec_pwr = LINE_OTN_PWR_LOWPWR;
    stream_cfg_ptr->fec_bypassed = TRUE;
    stream_cfg_ptr->fec_dec_passthru = TRUE;
    stream_cfg_ptr->bergen_enabled = FALSE;        
    stream_cfg_ptr->scramble_in_opsm = FALSE;
    
    PMC_RETURN();
}/* lineotn_stream_ctxt_init */



/*******************************************************************************
* lineotn_otu4_clk_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to configure the clock for OTU4 rate.  This
*  function should only be called if LINEOTN is configured for OTU4 rate.
*  This function must also be called if LINEOTN is being deprovisioned to disable
*  the configuration that is specific for OTU4 rate.
*
* INPUTS:
*  h                - pointer to LINEOTN handle instance
*  enable_clk       - TRUE: Configure OTU4 clock based on input lineotn_dsi_type and then
*                           enable OTU4 Clock Configuration.
*                     FALSE: Disable OTU4 Clock.  Should be used when LINEOTN is being 
*  dsi_type         - DSI Interface type to use for configuration
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*   This function should only be called if 'dsi_type' is LINE_OTN_DSI_TYPE_OTL or 
*   LINE_OTN_DSI_TYPE_SFIS because FW only configures the clock applicable for
*   OTU4 rate.  Also it should only be called once per activation and it is
*   recommended to call it in Tx direction.
*
*******************************************************************************/
PRIVATE PMC_ERROR lineotn_otu4_clk_cfg(lineotn_handle_t *h, 
                                       BOOL enable_clk,
                                       lineotn_dsi_type_t dsi_type)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[3];
    UINT32 i;
    UINT32 hostmsg_lineotn_dsi_type = 0;
    UINT32 fill_level_xoff_min = LINEOTN_DEFAULT_FILL_LEVEL_XOFF_MIN;

    PMC_ENTRY();

    PMC_ASSERT(NULL != h, HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    if (FALSE == pmc_enable_otu4_clock_cfg_in_fw(&h->base))
    {
        PMC_ASSERT(FALSE, LINEOTN_INVALID_REVISION, 0, 0);
    }
   
    /*
      FW definition of dsi_type
      0 - OTL (includes OTL over SFI-S)
      1 - SFI-S
    */
       
    if (LINE_OTN_DSI_TYPE_OTL == dsi_type ||
    LINE_OTN_ENHANCED_SFIS_DESKEW == h->var.lineotn_dsi_dist[0].deskew_type
        )
    {
        hostmsg_lineotn_dsi_type = 0;
    }
    else if (LINE_OTN_DSI_TYPE_SFIS == dsi_type)
    {        
        hostmsg_lineotn_dsi_type = 1;
    }
    else
    {
        result = HOSTMSG_ERR_INVALID_PARAMETERS;
    }



    if (PMC_SUCCESS == result)
    {    
        /* Add message body to the previously created header. */
        /* Words 0,1,2 of CMD_CFG_SERDES_T8 have the following format:
         * Word    bits 0..31
         *   0  <-----------enable_clk---------->
         *   1  <-------lineotn_dsi_type-------->
         *   2  <----fill_level_xoff_min-------->
         */
        data[0] = (UINT32)enable_clk;
        data[1] = hostmsg_lineotn_dsi_type;
        data[2] = (UINT32) fill_level_xoff_min;
    
        PMC_LOG_TRACE("DSI type: %d\n", hostmsg_lineotn_dsi_type);
        PMC_LOG_TRACE("CMD_CFG_LineOTN_Config_OTU4_Clock host message contents:\n");
        for (i = 0; i < 3; i++) {
            PMC_LOG_TRACE("Word %d: 0x%x\n", i, data[i]);
        }

        {
            HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
            result = hostmsg_send_cfg_message((pmc_handle_t *)h,MODID_LINEOTN,HOSTMSG_CMD_CFG_LINEOTN_CONFIG_OTU4_CLOCK_ST,data,3,stat);
        }
    }
    PMC_RETURN(result);

} /* lineotn_otu4_clk_cfg() */

/*******************************************************************************
*  lineotn_swizzle_fec_frame_slip_monitor_cfg
*  _____________________________________________________________________________
*
*  DESCRIPTION:
*   Sends host message to firmware to enable the monitoring of Tx frame slips
*   when using Swizzle FEC.
*
*  INPUTS:
*   h              - pointer to LINEOTN handle instance
*   enable         - TRUE: Enable monitoring for Tx frame slips\n
*                    FALSE: Disable monitoring for Tx frame slips
*
*  OUTPUTS:
*   None.
*
*  RETURNS:
*   PMC_SUCCESS - if everything went OK, error code is returned otherwise.
*
*  NOTES:
*   This function should be called when 100G Swizzle FEC is used and is only
*   applicable for the TX direction
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_swizzle_fec_frame_slip_monitor_cfg(lineotn_handle_t *h, BOOL enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[2];

    PMC_ENTRY();

    PMC_ASSERT(NULL != h, HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Add message body to the previously created header. */
    data[0] = (UINT32)enable;
    data[1] = (UINT32)LINEOTN_SWIZZLE_FEC_FRAME_SLIP_MAX_NUM_RETRY;

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)h,MODID_LINEOTN,HOSTMSG_CMD_CFG_LINEOTN_CONFIG_SWIZZLE_FEC_FRAME_SLIP_MON_ST,data,2,stat);
    }

    PMC_RETURN(result);

} /* lineotn_swizzle_fec_frame_slip_monitor_cfg() */


/*******************************************************************************
* lineotn_lof_defect_monitor_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Sends host message to firmware to enable the monitoring of LOF defect.
*
* INPUTS:
*  h              - pointer to LINEOTN handle instance
*  lineotn_sel    - TRUE: Configure LINEOTN. FALSE: Configure SYSOTN
*  interface_sel  - 0: OTU4; 
*                   1: OTU3[0];
*                   2: OTU3[1];
*                   3: OTU3[2];
*                   OTU3 is only valid if sysotn_sel = FALSE (LINEOTN).
*  enable         - TRUE: Enable monitoring for LOF;
*                   FALSE: Disable monitoring for LOF
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - if everything went OK, error code is returned otherwise.
*
* NOTES:
*   This function should be called when the SFI-S or SFI5.1 interfaces
*   are used.  It is only applicable for the Rev A part.
*
*******************************************************************************/
PRIVATE PMC_ERROR lineotn_lof_defect_monitor_cfg(lineotn_handle_t *h, 
                                                 BOOL lineotn_sel, UINT8 interface_sel,
                                                 BOOL enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[3];

    PMC_ENTRY();

    PMC_ASSERT(NULL != h, HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* Add message body to the previously created header. */
    data[0] = (UINT32)lineotn_sel;
    data[1] = (UINT32)interface_sel;
    data[2] = (UINT32)enable;

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)h,MODID_LINEOTN,HOSTMSG_CMD_CFG_LINEOTN_CONFIG_LOF_MON_ST,data,3,stat);
    }

    PMC_RETURN(result);

} /* lineotn_lof_defect_monitor_cfg() */



/*******************************************************************************
* FUNCTION: lineotn_int_init()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes/deinitializes interrupt and status support.
*   
* INPUTS:
*   lineotn_handle  - Pointer to lineotn handle
*   enable          - Enables/disables interrupt and status support.
*
* OUTPUTS:
*   None
* 
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void lineotn_int_init(lineotn_handle_t    *lineotn_handle,
                              BOOL                 enable)
{    
    PMC_ENTRY();

    /* Disable interrupts by default */
    opsm4_field_RX_OTU_OOF_INT_E_set(NULL, lineotn_handle, FALSE);

    /*
     * If this is a sysotn handle then reset the sysotn OPMS4 Enable 
     * register which by default is enabled in SYSOTN.
     */
    if (FALSE == lineotn_handle->var.lineotn_mode)
    {
        sysotn_opsm4_reg_RX_OTU4_INT_EN_REG_write(NULL, 
                                                  lineotn_handle,
                                                  0x0);
    }
    /* Enable top level aggregation interrupts */    
    lineotn_field_INT1_E_set(NULL, lineotn_handle, 0x1FFFF * enable);
    lineotn_field_INT2_E_set(NULL, lineotn_handle, 0x1FFFF * enable);

    PMC_RETURN();
} /* lineotn_int_init */


/*
** Public Functions
*/

/*******************************************************************************
* FUNCTION: lineotn_frame_rate_otuk_num_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   A helper function to return the number k of OTUk from the lineotn_frame_rate_t enum 
*   
* INPUTS:
*  frame_rate  - The frame rate (OTU1E..OTU4)
*
* OUTPUTS:
*   None
* RETURNS:
*   UINT32 - returns 2,3 or 4 which corresponds to the OTUk number,
*   0 is returned if an illegal input is provided
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 lineotn_frame_rate_otuk_num_get(lineotn_frame_rate_t frame_rate)
{
    UINT32 otuk_num = 0;

    PMC_ENTRY();

    switch (frame_rate)
    {
    case LINE_OTN_RATE_OTU4:
        otuk_num = 4;        
        break;

    case LINE_OTN_RATE_OTU3E2:
    case LINE_OTN_RATE_OTU3E1:
    case LINE_OTN_RATE_OTU3:
        otuk_num = 3;
        break;

    case LINE_OTN_RATE_OTU2F:
    case LINE_OTN_RATE_OTU2E:
    case LINE_OTN_RATE_OTU2:
    case LINE_OTN_RATE_OTU1F:
    case LINE_OTN_RATE_OTU1E:
        otuk_num = 2;
        break;

    default:
        otuk_num = 0;
        break;
    }

    PMC_RETURN(otuk_num);
} /* lineotn_frame_rate_otuk_num_get */


/*******************************************************************************
* lineotn_find_dsi_by_dci
* ______________________________________________________________________________
*
* DESCRIPTION:
*   helper function to find the dsi stream and dsi type based on dci channel.
*
*
* INPUTS:
*   h                         - pointer to LINEOTN handle instance
*   dci_channel               - dci channel number  
*
* OUTPUTS:
*   *dsi_stream               - pointer to user allocated dsi stream buffer.
*   *dsi_type                 - pointer to user allocated dsi type buffer.
*
* RETURNS:
*   PMC_SUCCESS - on success, or a meaningful error code is returned.
*
* NOTES:
*   
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_find_dsi_by_dci(lineotn_handle_t           *h,
                                         UINT32                     dci_channel,
                                         UINT32                     *dsi_stream,
                                         lineotn_dsi_type_t         *dsi_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8 i = 0;

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_type!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dci_channel<LINE_OTN_MAX_DSI_STREAMS,LINEOTN_ERR_INVALID_PARAMETERS,dci_channel,0);
    
    PMC_ENTRY();

    for(i=0;i<h->cfg.max_dsi_streams;i++)
    {
        if(h->var.lineotn_dsi_dist[i].dci_channel == dci_channel)
            break;
    }

    if(i<h->cfg.max_dsi_streams && 
       h->var.lineotn_dsi_dist[i].dsi_provisioned )
    {
        *dsi_stream = i;
        
        switch(h->var.lineotn_dsi_dist[i].dsi_hw_ifc)
        {
        case MLD_OTU2:
            *dsi_type = LINE_OTN_DSI_TYPE_OTUK;
            break;

        case MLD_OTL:
            *dsi_type = LINE_OTN_DSI_TYPE_OTL;
            break;

        case MLD_SFIS:
            *dsi_type = LINE_OTN_DSI_TYPE_SFIS;
            break;

        case MLD_SFI_51:
            if( i<=7 && i >=4) /* sfi 5.1 interface B only routes to post mld lanes 4-7 */
                *dsi_type = LINE_OTN_DSI_TYPE_SFI_51_B;
            else /* sfi 5.1 interface A only routes to post mld lanes 0-3 or 8-11 */
                *dsi_type = LINE_OTN_DSI_TYPE_SFI_51_A;
            break;

        default:
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
            *dsi_type = LAST_LINE_OTN_DSI_TYPE;
            *dsi_stream = h->cfg.max_dsi_streams;
            break;

        }
        
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
        *dsi_type = LAST_LINE_OTN_DSI_TYPE;
        *dsi_stream = h->cfg.max_dsi_streams;
    }

    PMC_RETURN(rc);
}/* lineotn_find_dsi_by_dci */


/*******************************************************************************
* lineotn_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an LINEOTN block instance.
*
*
* INPUTS:
*   *parent             - pointer to parent handle.  If no parent then pass in
*                         NULL
*   base_address        - base address of the LINEOTN subsystem relative to
*                         the device memory space
*   *sys_handle         - pointer to user system handle
*
*   *tsb_name           - pointer to char array that contains the tsb name
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   lineotn_handle_t    - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC lineotn_handle_t *lineotn_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    lineotn_handle_t *lineotn_handle;

    PMC_ENTRY();

    lineotn_handle = (lineotn_handle_t*)PMC_CTXT_CALLOC(sizeof(lineotn_handle_t), parent);
    pmc_handle_init(parent, lineotn_handle, sys_handle, PMC_MID_DIGI_LINEOTN, tsb_name, base_address);

    /* Register log strings. */
    pmc_log_block_strings_register(LINEOTN_LOG_ERR_STRINGS[0], LINEOTN_LOG_ERR_TABLE_BASE, LINEOTN_LOG_ERR_COUNT);

    /* Init children's handles - we have only OTU_FRM */
    lineotn_handle->otu_frm_handle = otu_frm_ctxt_create((pmc_handle_t *)lineotn_handle, base_address+BASE_ADDR_LINEOTN_SS_OTU_FRM_PM55_82_20_OTU_FRM_MTSB,sys_handle,"OTU_FRM");
    PMC_CTXT_REGISTER_PTR(&lineotn_handle->otu_frm_handle, parent);

    PMC_RETURN(lineotn_handle);
} /* lineotn_ctxt_create */


/*******************************************************************************
* lineotn_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for an LINEOTN block instance.
*
*
* INPUTS:
*   h         - pointer to LINEOTN handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void lineotn_ctxt_destroy(lineotn_handle_t *h)
{
    pmc_handle_mid_verify(h, PMC_MID_DIGI_LINEOTN);

    PMC_ENTRY();

    /* Free Children's handles */
    otu_frm_ctxt_destroy(h->otu_frm_handle);

    h->base.tsb_mid = PMC_MID_INVALID;
    PMC_CTXT_FREE(&h, h);
    PMC_RETURN();

} /* lineotn_ctxt_destroy */


/*******************************************************************************
* lineotn_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a LINEOTN block handle instance.
*
*
* INPUTS:
*   h            - pointer to LINEOTN handle instance
*   lineotn_mode - TRUE : lineotn code is operating on LINEOTN;
*                  FALSE : lineotn code is operating on SYSOTN
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void lineotn_handle_init(lineotn_handle_t *h,
                                BOOL8 lineotn_mode)
{
    PMC_ENTRY();

    /* init lineotn variables */
    lineotn_var_init(&h->var);

    h->var.lineotn_mode = lineotn_mode;
    
    /* init otu_frm handle and var for all sub blocks */
    otu_frm_handle_init(h->otu_frm_handle);

    if (TRUE == lineotn_mode)
    {
        h->cfg.max_dsi_streams = LINE_OTN_MAX_DSI_STREAMS;
    }
    else
    {        
        h->cfg.max_dsi_streams = 1;
    }

    PMC_RETURN();
} /* lineotn_handle_init */


/*******************************************************************************
* lineotn_set_ho_db_populate_ctxt
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Determines whether the COREOTN HO datapath will be populated
*
*
* INPUTS:
*   h            - pointer to LINEOTN handle instance
*   ho_db_pop    - TRUE : provision calls will populate COREOTN's HO database; 
*                  FALSE : provision calls will not populate COREOTN's HO database.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void lineotn_set_ho_db_populate_ctxt(lineotn_handle_t *h,
                                            BOOL8 ho_db_pop)
{
    PMC_ENTRY();

    h->otu_frm_handle->pop_ho_db = ho_db_pop;

    PMC_RETURN();
} /* lineotn_set_ho_db_populate_ctxt */



/*******************************************************************************
* lineotn_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   lineotn instance.
*
*
* INPUTS:
*   h                 - pointer to LINEOTN handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL - Returns TRUE if resources are all in start state (not provisioned), or
*          FALSE if least one resource is provisioned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL lineotn_start_state_test(lineotn_handle_t *h)
{
    UINT32 dsi_stream = 0;
    BOOL   rc = TRUE;  /* no resources provisioned */

    PMC_ENTRY();
    PMC_ASSERT(h!=NULL, OTU_FRM_ERR_INVALID_PTR, 0, 0);

    /* all rc statuses should be TRUE in order to allow configuration */
    /* check SS state */
    for (dsi_stream = 0; dsi_stream < h->cfg.max_dsi_streams ; dsi_stream++) 
    {
        if(h->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned || 
           h->var.current_lineotn_cfg != LAST_LINE_OTN_CFG )
        {
            rc = FALSE; /* found at least one resource that is allocated ! */    
            break;
        }
    }
       
    /* check different block states */
    if(TRUE == rc)
        rc = mld_start_state_test(h);

    if(TRUE == rc)
        rc = fec_start_state_test(h);

    if(TRUE == rc)
        rc = otu_frm_start_state_test(h->otu_frm_handle);

    PMC_RETURN( rc );
    
} /* lineotn_start_state_test */


/*******************************************************************************
* lineotn_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   For PMC_ENERGY_STATE_RUNNING, the function sets the register to a 
*   configurable state and puts the sub-blocks into the same state.
*   For PMC_ENERGY_STATE_RESET, the function configures the LINEOTN context
*   to the starting state for all dynamic context. All dynamic context will be
*   returned to the values that were generated during the call to
*   lineotn_handle_init().
*
* INPUTS:
*   h                 - pointer to LINEOTN handle instance.
*
*   energy_state      - Energy state to put LINEOTN into
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void lineotn_init(lineotn_handle_t     *h,
                         pmc_energy_state_t   energy_state)
{

    PMC_ENTRY();
    
    switch (energy_state)
    {
    case PMC_ENERGY_STATE_RUNNING:
        /* mld block */
        /* takes mld out of reset/ high power to the block level */
        /* all to be provisioned components are in low power    */
        mld_init(h,PMC_ENERGY_STATE_RUNNING); 

        /* fec block */
        /* takes fec out of reset/ high power to the block level */
        /* all to be provisioned components are in low power    */
        fec_init(h,PMC_ENERGY_STATE_RUNNING);
       
        if(h->var.lineotn_mode == FALSE)
        {
            sysotn_field_OTN_OTU_LOWPWR_set(NULL, h, 0);
        }

        /* otu frm block */
        /* takes fec out of reset/ high power to the block level */
        /* all to be provisioned components are in low power    */
        otu_frm_init(h->otu_frm_handle,PMC_ENERGY_STATE_RUNNING);

        /* Enable interrupts to go to external pin */
        lineotn_int_init(h, TRUE);
        break;

    case PMC_ENERGY_STATE_RESET:
        mld_init(h,PMC_ENERGY_STATE_RESET); 
        fec_init(h,PMC_ENERGY_STATE_RESET);

        if(h->var.lineotn_mode == FALSE)
        {
            sysotn_field_OTN_OTU_LOWPWR_set(NULL, h, 1);
        }

        otu_frm_init(h->otu_frm_handle,PMC_ENERGY_STATE_RESET);

        lineotn_var_init(&h->var); /* reset context */
            
        /* Disable interrupts to go to external pin */
        lineotn_int_init(h, FALSE);
        break;
        
    default:
        PMC_ASSERT(FALSE, LINEOTN_ERR_INVALID_PARAMETERS, energy_state, 0);
        break;
    } /* end switch */
    
    PMC_RETURN();
} /* lineotn_init */



/*******************************************************************************
* lineotn_stream_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions a full duplex (Rx/Tx) dsi stream across the lineOTN
*   sub system (SS) within a lineOTN pre-defined configuration.
*   The lineOTN has 19 possible SS configurations and the user must specify
*   the relevant configuration for the requested dsi stream. \n
*   All allocated HW resources are left in a disabled mode and at low power mode (where
*   applicable) until the lineotn_stream_act is called. \n
*   If successful: stream state changes from STREAM_STATE_START --> STREAM_STATE_EQUIPPED
*
*   Following actions are taken: \n
*       1. Validation of the provisioning request from state/resources point of view. \n
*
*       2. Allocation of HW resources at the MLD block: 
*           a) put all relevant mld resources per configuration into operational mode.
*              this includes: muxes,SFIS modules,SFI5.1 modules and OPSM modules. \n
*
*       3. Allocation of HW resources at the OTU_FRM block :
*           a) dci channel allocation
*           b) set the dci channel to pass through mode \n
*   
*       4. DSI streams configuration:
*           a) FEC ins/removal enabled, 
*           b) dLom enabled, 
*           c) sf squelch enabled,
*           d) scrambling/de-scrambling enabled \n
*
*       5. OTU_FRM_RX (SK) configuration:
*           a) maintenance signal disabled,
*           b) No maintenance signal is forced,
*           c) Expected TTI data set to 0,
*           d) Degradation threshold are left in default values,
*           e) Data smoothing configuration enabled \n
*
*       6. OTU_FRM_TX (SO) configuration:
*           a) set OTUk_So SM OH fields to 0
*           b) set RES8/RES7/RES13/RES14/GCCO/SM TTI to 0 \n
*
*       7. FEC is set to BYPASS mode and should be assigned by the user by calling the
*          lineotn_fec_cfg function prior to the activation of the channel. \n
*
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   lineotn_cfg            - enum,lineOTN overall configuration mode. 
*                            This enum specifies the lineOTN context in which we are
*                            the current dsi stream is being provisioned.
*                            There are 19 possible lineOTN configurations specified within
*                            line_otn_cfg enum. 
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            - for 10G configurations valid values are: (0..11) 
*                             dsi_type must be set to LINE_OTN_DSI_TYPE_OTUK 
*                            - for OTL3.4 40G configurations valid values are: (0,4,8)- 
*                              first lane in each 40G group. 
*                              dsi_type must be: LINE_OTN_DSI_TYPE_OTL for OTL3.4 
*                            - for 40G SFIS configurations valid values are: (0,5) - 
*                              first lane in each SFIS 40G group.
*                              dsi_type must be: LINE_OTN_DSI_TYPE_SFIS 
*                            - for 40G SFI5.1 configurations valid values are: (0,17) - 
*                              first lane in each SFIS 40G group.
*                              dsi_type must be: LINE_OTN_DSI_TYPE_SFI_51_X (X=A,B)
*                            - for 100G configurations valid values are: (0) -
*                              dsi_type can be either:
*                              LINE_OTN_DSI_TYPE_OTL   for OTL4.10 configuration or 
*                              LINE_OTN_DSI_TYPE_SFIS  for SFIS based configurations 
*
*   dsi_type               - enumerated lineotn dsi stream type:
*                            - LINE_OTN_DSI_TYPE_SFI_51_A : marks SFI5.1 dsi lanes 0-16;
*                            - LINE_OTN_DSI_TYPE_SFI_51_B : marks SFI5.1 dsi lanes 17-33;
*                            - LINE_OTN_DSI_TYPE_SFIS     : marks SFIS dsi lanes;.
*                            - LINE_OTN_DSI_TYPE_OTL      : marks OTL 3.4/4.10 dsi lanes;
*                            - LINE_OTN_DSI_TYPE_OTUK     : marks 10G OTUk lanes;
*
*   lineotn_deskew_type    - enum,lineotn required deskew type: 
*                            - LINE_OTN_NO_DESKEW              : no deskew required, OTL3.4/OTU2 configurations;
*                            - LINE_OTN_OIF_SFIS_DESKEW        : OIF SFI-S deskew;  
*                            - LINE_OTN_ENHANCED_SFIS_DESKEW   : OTL over SFI deskew mode;
*                            - etc.
*
*   frame_rate             - enum, OTU frame rate provisioned for this DSI stream:
*                            (LINE_OTN_RATE_OTU1E ... LINE_OTN_RATE_OTU4).
*
* OUTPUTS:
*   *dci_channel_ptr       - pointer to dci_channel associated with LINE OTN framer
*                            processing OTN signal, set to NULL upon failure.
*
* RETURNS:
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
*   1. Once provisioning started using a line_otn_cfg mode user cannot change
*      line_otn_cfg mode on the next dsi stream configuration without going through
*      lineotn_stream_deprov function and deprovisioning ALL the dsi streams
*      that are already provisioned. lineotn_stream_deprov function interface
*      allows full deprovisioning in one function call.\n
*   2. FEC is set to BYPASS mode and should be assigned by the user by calling the
*      lineotn_fec_cfg function prior to the activation of the channel.\n
*   3. dsi stream state must be STREAM_STATE_START when calling this function.\n
*   4. This is a full duplex configuration function which will provision both
*      Tx and Rx sides. user must make sure that both Rx and Tx are set to the
*      proper stream state before calling this function.\n
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_stream_prov           ( lineotn_handle_t      *h,
                                                 lineotn_cfg_type_t    lineotn_cfg,
                                                 UINT32                dsi_stream,
                                                 lineotn_dsi_type_t    dsi_type,
                                                 lineotn_deskew_type_t lineotn_deskew_type,
                                                 lineotn_frame_rate_t  frame_rate,
                                                 UINT32                *dci_channel_ptr )
{
    PMC_ERROR       rc = PMC_SUCCESS;
    mld_ifc_type_t  mld_interface_type = MLD_NA;
    util_global_odukp_type_t oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32          dsi_start = 0;
    UINT32          dsi_end   = 0;
    UINT8           i = 0;
    UINT32          dci_channel = 0;

    PMC_ENTRY();

    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dci_channel_ptr!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(lineotn_cfg<=LAST_LINE_OTN_CFG,LINEOTN_ERR_INVALID_PARAMETERS,lineotn_cfg,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);
    PMC_ASSERT(lineotn_deskew_type<LAST_LINE_OTN_DESKEW_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,lineotn_deskew_type,0);
    PMC_ASSERT(frame_rate<LAST_LINE_OTN_FRAME_RATE,LINEOTN_ERR_INVALID_PARAMETERS,frame_rate,0);
    
    /* validate configuration parameters */
    rc = lineotn_validate_prov(h,lineotn_cfg,lineotn_deskew_type,dsi_type,dsi_stream,frame_rate);

    if(PMC_SUCCESS == rc)
    {
        /* if we have sfi 5.1 interface - first we need to save frame rate data to the extra config section of the handle  */
        if(LINE_OTN_DSI_TYPE_SFI_51_A == dsi_type ) {
            h->var.lineotn_sfi51_0_frame_rate = frame_rate ;
        } 
        else if(LINE_OTN_DSI_TYPE_SFI_51_B == dsi_type )
        {
            h->var.lineotn_sfi51_1_frame_rate = frame_rate ;
        }
        else {
            h->var.lineotn_dsi_dist[dsi_stream].dsi_rate = frame_rate ;
        }
        
        rc = lineotn_dsi_params_get(h,lineotn_cfg, dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

        if(PMC_SUCCESS == rc)
        {
            /* set new lineotn configuration */
            if(LAST_LINE_OTN_CFG==h->var.current_lineotn_cfg)
                h->var.current_lineotn_cfg = lineotn_cfg;

            /* start provisioning with otu_frm because it has a validation layer     */
            /* if otu_frm (either rx or tx) return PMC_SUCCESS - this means that     */
            /* this is valid configuration and we can go ahead and provision mld/fec */

            /* provision otu_frm rx (sk) */
            /* mark the SFI5.1 post mld lanes for otu_frm otudg needs) */
            if(MLD_SFI_51 == mld_interface_type)
                h->otu_frm_handle->var.otudg_sfi51_lanes_bm = h->otu_frm_handle->var.otudg_sfi51_lanes_bm | (0x00000001<<(dsi_start/4));

            /* provision otu_frm rx (sk) */


            /* TRUE, TRUE --> scrambling in the OTU_FRM and data smoothing are enabled by default */
            if(frame_rate >= LINE_OTN_RATE_OTU3E2 && frame_rate <= LINE_OTN_RATE_OTU3) /* 40G */
                /* division of dsi_start by 4 will map 40G dsi streams (0,4,8,0,5) to (0,1,2) range needed by otu_frm */
                rc = otu_frm_blk_prov_rx_stream(h->otu_frm_handle,(dsi_start/4),oduk_type,TRUE, TRUE,&dci_channel);
            else
                rc = otu_frm_blk_prov_rx_stream(h->otu_frm_handle,dsi_start,oduk_type,TRUE, TRUE,&dci_channel);

            /* provision otu_frm tx (so) */
            if(PMC_SUCCESS == rc)
            {   /* TRUE, --> descrambling in the OTU_FRM is enabled by default */
                if(frame_rate >= LINE_OTN_RATE_OTU3E2 && frame_rate <= LINE_OTN_RATE_OTU3) /* 40G */
                    /* division of dsi_start by 4 will map 40G dsi streams (0,4,8,0,5) to (0,1,2) range needed by otu_frm */
                    rc = otu_frm_blk_prov_tx_stream(h->otu_frm_handle,(dsi_start/4),oduk_type,TRUE,&dci_channel);
                else
                    rc = otu_frm_blk_prov_tx_stream(h->otu_frm_handle,dsi_start,oduk_type,TRUE,&dci_channel);
            }

            /* Store stream to channel mapping */
            if (PMC_SUCCESS == rc)
            {
                for(i=dsi_start;i<dsi_end+1;i++)
                {
                    h->var.lineotn_dsi_dist[i].dci_channel = dci_channel;
                }
            }

            if (PMC_SUCCESS == rc)
            {
                rc = lineotn_consq_action_cfg(h, dsi_stream, dsi_type, LINE_OTN_A_AIS, LINE_OTN_D_LOM, TRUE);
            }

            /* setup D_DEG propagation based on FW/HW dDeg handling */
            if (PMC_SUCCESS == rc)
            {
                rc = lineotn_consq_action_cfg(h, dsi_stream, dsi_type, LINE_OTN_A_TSD, LINE_OTN_D_DEG, h->var.fw_ddeg);
            }
            
            /* provision mld block */
            if(PMC_SUCCESS == rc)
            {
                if (mld_interface_type == MLD_OTL || mld_interface_type == MLD_OTU2) 
                {
                    rc = mld_blk_prov_stream(h,lineotn_cfg,lineotn_deskew_type ,mld_interface_type,dsi_start);
                } 
                else if (mld_interface_type == MLD_SFIS) 
                { /* for SFIS -mld range of dsi stream should be 0 or 5 */

                    rc = mld_blk_prov_stream(h,lineotn_cfg,lineotn_deskew_type ,mld_interface_type,dsi_stream);
                    if (h->var.lineotn_mode == FALSE) {
                        sysotn_field_OTL_MODE_set(NULL, h, FALSE);
                    }

                } 
                else 
                { /* for SFI51 -mld range of dsi stream should be 0 or 17 */
                
                    if (LINE_OTN_DSI_TYPE_SFI_51_A == dsi_type) {
                        rc = mld_blk_prov_stream(h,lineotn_cfg,lineotn_deskew_type ,mld_interface_type,0);
                    } else {
                        rc = mld_blk_prov_stream(h,lineotn_cfg,lineotn_deskew_type ,mld_interface_type,LINEOTN_SFI51_1_DSI_STREAM_IDX);
                    }
                }
            }

            /* if everything was successful - update lineotn global state/tracking variables */
            if(PMC_SUCCESS == rc)
            {
                for(i=dsi_start;i<dsi_end+1;i++)
                {
                    h->var.lineotn_dsi_dist[i].dsi_hw_ifc  = mld_interface_type;
                    h->var.lineotn_dsi_dist[i].dsi_provisioned = TRUE;
                    h->var.lineotn_dsi_dist[i].dsi_rate = frame_rate;
                    h->var.lineotn_dsi_dist[i].deskew_type = lineotn_deskew_type;
                }

                /* if this is an sfi 5.1 streams - extra configuration required */
                if(LINE_OTN_DSI_TYPE_SFI_51_A == dsi_type )
                    h->var.lineotn_sfi51_0_frame_rate = frame_rate;
                else if(LINE_OTN_DSI_TYPE_SFI_51_B == dsi_type )
                    h->var.lineotn_sfi51_1_frame_rate = frame_rate;
                                
            }
            else
            {
                for(i=dsi_start;i<dsi_end+1;i++)
                {
                    h->var.lineotn_dsi_dist[i].dci_channel = FREE_CHANNEL;
                }
            }

            /* last, put fec to bypass mode */
            if(PMC_SUCCESS == rc) {
                rc = lineotn_fec_bypass_cfg(h,dsi_start,dsi_type,LINE_OTN_FEC_BYPASS);
            }

        }
        else /* failure - remove the frame rate data saved at the beginning*/
        {
            /* if we have sfi 5.1 interface - first we need to save frame rate data to the extra config section of the handle  */
            if(LINE_OTN_DSI_TYPE_SFI_51_A == dsi_type )
                h->var.lineotn_sfi51_0_frame_rate = LAST_LINE_OTN_FRAME_RATE;
            else if(LINE_OTN_DSI_TYPE_SFI_51_B == dsi_type )
                h->var.lineotn_sfi51_1_frame_rate = LAST_LINE_OTN_FRAME_RATE ;
            else
                h->var.lineotn_dsi_dist[dsi_stream].dsi_rate = LAST_LINE_OTN_FRAME_RATE ;

            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }

    }

    /* all is good? - update the dci channel output pointer and current configuration */
    if(PMC_SUCCESS == rc)
        (*dci_channel_ptr) = dci_channel;
    else /* reset lineotn configuration on provisioning error */
        h->var.current_lineotn_cfg = LAST_LINE_OTN_CFG;

    PMC_RETURN(rc);
}/* lineotn_stream_prov */


/*******************************************************************************
* lineotn_stream_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function de-provisions an already provisioned (Rx/Tx) dsi stream
*   across the lineOTN subsystem.
*   All HW resources are set to a disabled mode and at low power mode.
*   If successful: stream state changes from STREAM_STATE_EQUIPPED --> STREAM_STATE_START \n
* 
*   The following actions are taken: \n
*       1. Validation of the de-provisioning request from state point of view \n
*
*       2. deallocation of HW resources at the MLD block:
*           a) put all relevant mld resources per configuration into idle mode.
*              this includes: muxes,SFIS modules,SFI5.1 modules and OPSM modules. \n
*
*       3. deallocation of HW resources at the OTU_FRM block :
*           a) set the dci channel to pass through mode
*           b) dci channel deallocation \n
*
*       4. DSI streams configuration:
*           a) FEC ins/removal disabled
*           b) dLom disabled
*           c) sf squelch disabled
*           d) scrambling/de-scrambling disabled \n
*
*       5. OTU_FRM_RX (SK) configuration:
*           a) maintenance signal disabled
*           b) No maintenance signal is forced
*           c) Expected TTI data set to 0
*           d) Degradation threshold are not changed
*           e) Data smoothing disabled \n
*
*       6. OTU_FRM_TX (SO) configuration:
*           a) set OTUk_So SM OH fields to 0 \n
*
*       7. FEC deprovisioned and is set to BYPASS mode.
*
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for the detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for the detailed description
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   0 - PMC_SUCCESS or internally generated error code is returned
*
* NOTES:
*   1. dsi stream state must be STREAM_STATE_EQUIPPED or STREAM_STATE_OPERATIONAL
*      when calling this function.\n
*   2. This is a full duplex configuration function which will de-provision both
*      Tx and Rx sides. user must make sure that both Rx and Tx are set to the
*      proper stream state before calling this function.\n
*   3. Invoking this function will deprovision any configured fec for the dsi stream
*      and put the fec into bypass mode.\n
* 
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_stream_deprov         ( lineotn_handle_t *h,
                                                 UINT32 dsi_stream,
                                                 lineotn_dsi_type_t dsi_type )
{
    PMC_ERROR                   rc = PMC_SUCCESS;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    UINT8                       i = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;
    lineotn_frame_rate_t        frame_rate = LAST_LINE_OTN_FRAME_RATE;

    PMC_ENTRY();

    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        /* if we have sfi 5.1 interface - first we need to get data from the extra config section of the handle  */
        if(LINE_OTN_DSI_TYPE_SFI_51_A == dsi_type ) 
        {
            frame_rate = h->var.lineotn_sfi51_0_frame_rate;
        } 
        else if(LINE_OTN_DSI_TYPE_SFI_51_B == dsi_type )
        {
            frame_rate = h->var.lineotn_sfi51_1_frame_rate;
        }
        else 
        {
            frame_rate = h->var.lineotn_dsi_dist[dsi_start].dsi_rate;
        }

        /* start deprovisioning with otu_frm because it has a validation layer   */
        /* if otu_frm (either rx or tx) return PMC_SUCCESS - this means that     */
        /* this is valid configuration and we can go ahead and provision mld/fec */

        /* de-provision otu_frm rx (sk) */
        if(PMC_SUCCESS == rc)
        {
            /* remove the SFI5.1 post mld lanes for otu_frm otudg needs) */
            if(MLD_SFI_51 == mld_interface_type)
                h->otu_frm_handle->var.otudg_sfi51_lanes_bm &= ~(0x00000001<<dsi_start/4);

            /* de-provision otu_frm rx (sk) */

            if(frame_rate >= LINE_OTN_RATE_OTU3E2 && frame_rate <= LINE_OTN_RATE_OTU3) /* 40G */
                /* division of dsi_start by 4 will map 40G dsi streams (0,4,8,0,5) to (0,1,2) range needed by otu_frm */
                rc = otu_frm_blk_deprov_rx_stream(h->otu_frm_handle,(dsi_start/4),oduk_type);
            else
                rc = otu_frm_blk_deprov_rx_stream(h->otu_frm_handle,dsi_start,oduk_type);
        }

        /* de-provision otu_frm tx (so) */
        if(PMC_SUCCESS == rc)
        {
            if(frame_rate >= LINE_OTN_RATE_OTU3E2 && frame_rate <= LINE_OTN_RATE_OTU3) /* 40G */
                /* division of dsi_start by 4 will map 40G dsi streams (0,4,8,0,5) to (0,1,2) range needed by otu_frm */
                rc = otu_frm_blk_deprov_tx_stream(h->otu_frm_handle,(dsi_start/4),oduk_type);
            else
                rc = otu_frm_blk_deprov_tx_stream(h->otu_frm_handle,dsi_start,oduk_type);
        }

        /* de-provision mld block */
        if(PMC_SUCCESS == rc)
        {
            if( mld_interface_type == MLD_OTL || mld_interface_type == MLD_OTU2 )
            {
                rc = mld_blk_deprov_stream(h,lineotn_cfg,h->var.lineotn_dsi_dist[dsi_start].deskew_type ,mld_interface_type,dsi_start);
            }
            else if( mld_interface_type == MLD_SFIS ) /* for SFIS -mld range of dsi stream should be 0 or 5 */
            {
                rc = mld_blk_deprov_stream(h,lineotn_cfg,h->var.lineotn_dsi_dist[dsi_start].deskew_type ,mld_interface_type,dsi_stream);
            }
            else /* for SFI51 -mld range of dsi stream should be 0 or 17 */
            {
                if( LINE_OTN_DSI_TYPE_SFI_51_A == dsi_type ) /* sfi 5.1   0 */
                    rc = mld_blk_deprov_stream(h,lineotn_cfg,h->var.lineotn_dsi_dist[dsi_start].deskew_type ,mld_interface_type,0);
                else    /* sfi 5.1   1 */
                    rc = mld_blk_deprov_stream(h,lineotn_cfg,h->var.lineotn_dsi_dist[dsi_start].deskew_type ,mld_interface_type,LINEOTN_SFI51_1_DSI_STREAM_IDX);
            }
        }

    /* Disable Frame Slip Monitor if FEC type is Swizzle 100G */
        if (PMC_SUCCESS == rc)
        {
            if (LINE_OTN_FEC_SWIZZLE_100G == h->var.lineotn_dsi_dist[dsi_start].dsi_fec_type) 
            {
                rc = lineotn_swizzle_fec_frame_slip_monitor_cfg(h, FALSE); 
            }
        }

        if (PMC_SUCCESS == rc)
        {
            if(h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned)
                rc = fec_blk_deprov_stream(h,dsi_start);
        }

        /* if everything was successful - update lineotn global state/tracking variables */
        if(PMC_SUCCESS == rc)
        {
            for(i=dsi_start;i<dsi_end+1;i++)
            {
                h->var.lineotn_dsi_dist[i].dci_channel = FREE_CHANNEL;
                h->var.lineotn_dsi_dist[i].dsi_hw_ifc = LAST_MLD_IFC_TYPE;
                h->var.lineotn_dsi_dist[i].dsi_rate = LAST_LINE_OTN_FRAME_RATE;
                h->var.lineotn_dsi_dist[i].dsi_provisioned = FALSE;
                h->var.lineotn_dsi_dist[i].deskew_type = LAST_LINE_OTN_DESKEW_TYPE;
                h->var.lineotn_dsi_dist[i].dsi_fec_type = LAST_LINE_OTN_FEC;
            }

            /* if this is an sfi 5.1 streams - extra configuration required */
            if(LINE_OTN_DSI_TYPE_SFI_51_A == dsi_type )
                h->var.lineotn_sfi51_0_frame_rate = LAST_LINE_OTN_FRAME_RATE;
            else if(LINE_OTN_DSI_TYPE_SFI_51_B == dsi_type )
                h->var.lineotn_sfi51_1_frame_rate = LAST_LINE_OTN_FRAME_RATE;                        
        }

    }

    /* all is good? - update the dci channel output pointer and current configuration */
    if(PMC_SUCCESS == rc)
    {
        for (i = 0;i < h->cfg.max_dsi_streams ; i++) /* check if all streams deprovisioned */
        {
            if(h->var.lineotn_dsi_dist[i].dsi_provisioned == TRUE)
                break;
        }

        if(h->cfg.max_dsi_streams == i) /* no provisioned streams found */
        {
            h->var.current_lineotn_cfg = LAST_LINE_OTN_CFG;
        }    
    }

    PMC_RETURN(rc);
}/* lineotn_stream_deprov */


/*******************************************************************************
* lineotn_stream_act
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function activates an already provisioned (Rx/Tx) dsi stream across
*   the lineOTN subsystem. \n
*
*   The following actions are taken: \n
*       1. Validation of the provisioning request from state point of view. \n
*       2. Setting all MLD/FEC/OTU_FRM HW resources to enabled/operational mode. \n
*
*   If successful: stream state changes from STREAM_STATE_EQUIPPED --> STREAM_STATE_OPERATIONAL
*
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*   
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
*   act_dir                - enum, requested activation direction:
*                            LINE_OTN_RX_DIR,
*                            LINE_OTN_TX_DIR
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, or internally generated error code is returned
*
* NOTES:
*   1. dsi stream state must be STREAM_STATE_EQUIPPED when calling this function.
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_stream_act            ( lineotn_handle_t *h,
                                                 UINT32 dsi_stream,
                                                 lineotn_dsi_type_t dsi_type,
                                                 lineotn_dir_t act_dir )
{
    PMC_ERROR                   rc = PMC_SUCCESS;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;
    lineotn_frame_rate_t        frame_rate = LAST_LINE_OTN_FRAME_RATE;
    UINT8 i = 0;

    PMC_ENTRY();

    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        /* if we have sfi 5.1 interface - first we need to get data from the extra config section of the handle  */
        if(LINE_OTN_DSI_TYPE_SFI_51_A == dsi_type )
            frame_rate = h->var.lineotn_sfi51_0_frame_rate;
        else if(LINE_OTN_DSI_TYPE_SFI_51_B == dsi_type )
            frame_rate = h->var.lineotn_sfi51_1_frame_rate;
        else
            frame_rate = h->var.lineotn_dsi_dist[dsi_stream].dsi_rate;

        /* only otu_frm has activation/deactivation functions */
        /* activate otu_frm rx (sk) */
        if(LINE_OTN_RX_DIR== act_dir)
        {
            if(frame_rate >= LINE_OTN_RATE_OTU3E2 && frame_rate <= LINE_OTN_RATE_OTU3) /* 40G */
            {
                /* division of dsi_start by 4 will map 40G dsi streams (0,4,8,0,5) to (0,1,2) range needed by otu_frm */
                rc = otu_frm_blk_act_rx_stream(h->otu_frm_handle,(dsi_start/4),oduk_type);
            }
            else /* 10G or 100G */
            {
                rc = otu_frm_blk_act_rx_stream(h->otu_frm_handle,dsi_start,oduk_type);
            }

            /* mark all relevant dsi streams as activated */
            if(PMC_SUCCESS == rc)
            {
                for( i = dsi_start; i<=dsi_end ; i++ )
                    h->var.lineotn_dsi_dist[i].dsi_activated_rx = TRUE;
            }

        }
        else /* TX side */
        {

            if(frame_rate >= LINE_OTN_RATE_OTU3E2 && frame_rate <= LINE_OTN_RATE_OTU3) /* 40G */
            {
                /* division of dsi_start by 4 will map 40G dsi streams (0,4,8,0,5) to (0,1,2) range needed by otu_frm */
                rc = otu_frm_blk_act_tx_stream(h->otu_frm_handle,(dsi_start/4),oduk_type);
            }
            else  /* 10G or 100G */
            {
                rc = otu_frm_blk_act_tx_stream(h->otu_frm_handle,dsi_start,oduk_type);
            }


            /* mark all relevant dsi streams as activated */
            if(PMC_SUCCESS == rc)
            {
                for( i = dsi_start; i<=dsi_end ; i++ )
                    h->var.lineotn_dsi_dist[i].dsi_activated_tx = TRUE;
            }


            /*  Note that we only need to configure the clock for OTU4 in Tx direction 
                and it should only be called once per activation.  */
            if ( PMC_SUCCESS == rc && 
                 TRUE == pmc_enable_otu4_clock_cfg_in_fw(&h->base))
            {
                    if ( LINE_OTN_RATE_OTU4 == frame_rate )      
                    {
                        /* Make sure the rate and the dsi_type match up before calling the function
                           to configure the clock for LINEOTN OTU4 rate.
                           Note that for 100G OTU4 rate configuration, only the following
                           values for 'dsi_type' are valid:
                           -- LINE_OTN_DSI_TYPE_OTL   for OTL4.10 configuration 
                           -- LINE_OTN_DSI_TYPE_SFIS  for SFIS based configurations
                        */
                        if ((LINE_OTN_DSI_TYPE_OTL == dsi_type) || (LINE_OTN_DSI_TYPE_SFIS == dsi_type))
                        {
                            /* Enable the configuration of the clock for LINEOTN OTU4 rate 
                               based on the input dsi_type interface. */
                            rc = lineotn_otu4_clk_cfg(h, TRUE, dsi_type);
                        }
                        else
                        {
                            PMC_ASSERT(FALSE,LINEOTN_ERR_UNEXPECTED_PARAM_FOR_OTU4_CLK_CFG,dsi_type,0);
                        }
                    } /* end if( LINE_OTN_RATE_OTU4 == frame_rate) */
            }
        } /* end if(LINE_OTN_RX_DIR== act_dir) */
    }

    PMC_RETURN(rc);
}/* lineotn_stream_act */


/*******************************************************************************
* lineotn_stream_deact
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function deactivates an already operational (Rx/Tx) dsi stream across
*   the lineOTN sub system. \n
*
*   The following actions are taken: \n
*       1. Validation of the provisioning request from state point of view. \n
*       2. Setting all MLD/FEC/OTU_FRM HW resources to disabled mode. \n\n
*
*   If successful: stream state changes from STREAM_STATE_OPERATIONAL --> STREAM_STATE_EQUIPPED
*
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type:
*                            see lineotn_stream_prov for a detailed description*
* 
*
*   deact_dir              - enum, requested deactivation direction:
*                            LINE_OTN_RX_DIR,
*                            LINE_OTN_TX_DIR
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success or an internally generated error code is returned
*
* NOTES:
*   1. dsi stream state must be STREAM_STATE_OPERATIONAL when calling this function.\n
*   2. De-activation does not free any HW resources or take HW into low power
*      mode.\n
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_stream_deact          ( lineotn_handle_t *h,
                                                 UINT32 dsi_stream,
                                                 lineotn_dsi_type_t dsi_type,
                                                 lineotn_dir_t deact_dir )
{
    PMC_ERROR                   rc = PMC_SUCCESS;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;
    lineotn_frame_rate_t        frame_rate = LAST_LINE_OTN_FRAME_RATE;
    UINT8 i = 0;

    PMC_ENTRY();

    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);
     
    /* check that FEC STAT is not enabled on this stream, otherwise we will lost the synchronization of the FEC STAT table  */   
    /* Note that SFI5.1 or SFIS interfaces are not supported, so we can safely restrict the dsi_stream to be less than MAX_DSI_STREAM */ 
    if (dsi_stream < LINE_OTN_MAX_DSI_STREAMS)
    {
        if (h->var.lineotn_dsi_dist[dsi_stream].fec_stat_enabled == TRUE)
        {
            PMC_RETURN(LINEOTN_ERR_DEPROVISION_FEC_STAT_ERR);
        }                
    }

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        /* check if there is a BERGEN enabled */
        for( i = dsi_start; i<=dsi_end && i < LINE_OTN_MAX_DSI_STREAMS; i++ )
        {
            if (h->var.lineotn_dsi_dist[i].bergen_enabled == TRUE)
            {
                PMC_RETURN(LINEOTN_ERR_DEPROVISION_BERGEN_ERR);
            }
        }

        /* check if lineotn was already activated for the rx side  */
        /* if it was not activated, then no de-activation of HW is */
        /* required. just exit with success status.                */
        if(LINE_OTN_RX_DIR==deact_dir && 
           FALSE==h->var.lineotn_dsi_dist[dsi_start].dsi_activated_rx)
            PMC_RETURN(PMC_SUCCESS); 

        /* check if lineotn was already activated for the tx side  */
        /* if it was not activated, then no de-activation of HW is */
        /* required. just exit with success status.                */
        if(LINE_OTN_TX_DIR==deact_dir && 
           FALSE==h->var.lineotn_dsi_dist[dsi_start].dsi_activated_tx)
            PMC_RETURN(PMC_SUCCESS); 
    }

    if(PMC_SUCCESS == rc)
    {
        /* if we have sfi 5.1 interface - first we need to get data from the extra config section of the handle  */
        if(LINE_OTN_DSI_TYPE_SFI_51_A == dsi_type )
            frame_rate = h->var.lineotn_sfi51_0_frame_rate;
        else if(LINE_OTN_DSI_TYPE_SFI_51_B == dsi_type )
            frame_rate = h->var.lineotn_sfi51_1_frame_rate;
        else
            frame_rate = h->var.lineotn_dsi_dist[dsi_stream].dsi_rate;

        /* only otu_frm has activation/deactivation functions */
        /* activate otu_frm rx (sk) */
        if(LINE_OTN_RX_DIR== deact_dir)
        {

            if(frame_rate >= LINE_OTN_RATE_OTU3E2 && frame_rate <= LINE_OTN_RATE_OTU3) /* 40G */
            {
                /* division of dsi_start by 4 will map 40G dsi streams (0,4,8,0,5) to (0,1,2) range needed by otu_frm */
                rc = otu_frm_blk_deact_rx_stream(h->otu_frm_handle,(dsi_start/4),oduk_type);

            }
            else /* 10G  or 100G */
            {
                rc = otu_frm_blk_deact_rx_stream(h->otu_frm_handle,dsi_start,oduk_type);
            }

            /* mark all relevant dsi streams as de-activated */
            if(PMC_SUCCESS == rc)
            {
                for( i = dsi_start; i<=dsi_end ; i++ ) 
                {
                    h->var.lineotn_dsi_dist[i].dsi_activated_rx = FALSE;
                }
            }

        }

        else /* TX side */
        {
            if(frame_rate >= LINE_OTN_RATE_OTU3E2 && frame_rate <= LINE_OTN_RATE_OTU3) /* 40G */
            {
                /* division of dsi_start by 4 will map 40G dsi streams (0,4,8,0,5) to (0,1,2) range needed by otu_frm */
                rc = otu_frm_blk_deact_tx_stream(h->otu_frm_handle,(dsi_start/4),oduk_type);
            }
            else /* 10G or 100G */
            {
                rc = otu_frm_blk_deact_tx_stream(h->otu_frm_handle,dsi_start,oduk_type);
            }

            /* mark all relevant dsi streams as de-activated */
            if(PMC_SUCCESS == rc)
            {
                for( i = dsi_start; i<=dsi_end ; i++ )
                {
                    h->var.lineotn_dsi_dist[i].dsi_activated_tx = FALSE;
                }
            }

            /* Disable OTU4 Clock configuration*/
            if(PMC_SUCCESS == rc &&
               TRUE == pmc_enable_otu4_clock_cfg_in_fw(&h->base))
            {
                if ( 4 == lineotn_frame_rate_otuk_num_get(frame_rate) )
                {
                    /* Only need to check the rate to make sure it is OTU4 rate before 
                       calling FW to disable the OTU4 clock configuration.
                       Note that 'dsi_type' is a don't care from FW perspective when disabling
                       the OTU4 clock configuration.*/
                    rc = lineotn_otu4_clk_cfg(h, FALSE, dsi_type);
                } 
            }

        } /* end if(LINE_OTN_RX_DIR== deact_dir) */

    }

    PMC_RETURN(rc);
}/* lineotn_stream_deact */


/*******************************************************************************
* lineotn_fec_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the fec type for an already provisioned dsi stream.
*   It is the user responsibility to allocate the correct fec type to the correct dsi
*   stream as not all fec type are available for every data rate.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance \n
*
*   dsi_stream             - unsigned, dsi stream number to configure:\n
*                            - for 10G configurations valid values are: (0..11) 
*                             dsi_type must be set to LINE_OTN_DSI_TYPE_OTUK 
*                            - for OTL3.4 40G configurations valid values are: (0,4,8)- 
*                              first lane in each 40G group. 
*                              dsi_type must be: LINE_OTN_DSI_TYPE_OTL for OTL3.4 
*                            - for 40G SFIS configurations valid values are: (0,5) - 
*                              first lane in each SFIS 40G group.
*                              dsi_type must be: LINE_OTN_DSI_TYPE_SFIS 
*                            - for 40G SFI5.1 configurations valid values are: (0,17) - 
*                              first lane in each SFIS 40G group.
*                              dsi_type must be: LINE_OTN_DSI_TYPE_SFI_51_X (X=A,B)
*                            - for 100G configurations valid values are: (0) -
*                              dsi_type can be either:
*                              LINE_OTN_DSI_TYPE_OTL   for OTL4.10 configuration or 
*                              LINE_OTN_DSI_TYPE_SFIS  for SFIS based configurations 
*
*   dsi_type               - enumerated lineotn dsi stream type:
*                            - LINE_OTN_DSI_TYPE_SFI_51_A : marks SFI5.1 dsi lanes 0-16;
*                            - LINE_OTN_DSI_TYPE_SFI_51_B : marks SFI5.1 dsi lanes 17-33;
*                            - LINE_OTN_DSI_TYPE_SFIS     : marks SFIS dsi lanes;.
*                            - LINE_OTN_DSI_TYPE_OTL      : marks OTL 3.4/4.10 dsi lanes;
*                            - LINE_OTN_DSI_TYPE_OTUK     : marks 10G OTUk lanes;
* 
*   fec_type                - enum,requested fec type for the dsi stream: 
*                             - LINE_OTN_FEC_NONE 
*                             - LINE_OTN_FEC_I4 
*                             - LINE_OTN_FEC_I7 
*                             - LINE_OTN_FEC_G709 
*                             - LINE_OTN_FEC_SWIZZLE 
*                             - etc.
* OUTPUTS:
*   None.
*
* RETURNS:
*   0 - PMC_SUCCESS
*   LINEOTN_ERR_FEC_SET_FAILED  - Internally generated error code
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*   LINEOTN_ERR_OTHER_FEC_ALREADY_PROV - Internally generated error code
*   LINEOTN_ERR_STRONG_FEC_MIX_NOT_SUPPORTED - Internally generated error code
*
* NOTES:
*   1. invoking this function with a dsi stream that is not provisioned
*      will result in an error status returned.\n
*
*   2. invoking this function with a dsi stream that has a FEC type already provisioned
*      without going through deprovision (LINE_OTN_FEC_NONE) will result in an error
*      status returned.\n
*      Please note the following:
*           - in 100G all lanes are configured with the requested FEC type -
*             no other FEC type is allowed.
*           - in 40G all slice lanes (0-3,4-7,8-11) are configured with the requested FEC type -
*             no other FEC type is allowed.
*           - in 10G mode slice lanes (0-3,4-7,8-11) can hold up to 2 FEC types on the same slice,
*             however, one of them MUST be LINE_OTN_FEC_G709.
*           - Any deviation from this scheme will result with LINEOTN_ERR_OTHER_FEC_ALREADY_PROV failure status. \n
*
*   3. Invoking this function puts the fec into operational mode and removes
*      any bypass set earlier. 
*
*******************************************************************************/

PUBLIC PMC_ERROR lineotn_fec_cfg ( lineotn_handle_t *h,
                                   UINT32 dsi_stream,
                                   lineotn_dsi_type_t dsi_type,
                                   lineotn_fec_t fec_type )
{
    PMC_ERROR   rc = PMC_SUCCESS;
    UINT8       loop_counter = 0;
    UINT8       loop_start = 0;
    UINT8       i = 0;
    UINT8       dsi_streams_count = 0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    PMC_ASSERT(fec_type< LAST_LINE_OTN_FEC,LINEOTN_ERR_INVALID_PARAMETERS,fec_type,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);


    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        /* get the range of dsi stream configuration */
        lineotn_calc_dsi_range(h,dsi_start,&loop_start,&loop_counter);

        for(i=loop_start;i<loop_counter;i++)
        {
            if(h->var.lineotn_dsi_dist[i].dsi_provisioned) /* configure only provisioned streams */
            {
                dsi_streams_count++; /* count the configured dsi streams */

                /* De-provision the current FEC selection */
                if (LINE_OTN_FEC_SWIZZLE_100G == h->var.lineotn_dsi_dist[i].dsi_fec_type)
                {
                    /*  Disable Frame Slip Monitor if the previous FEC type is Swizzle 100G */
                    rc = lineotn_swizzle_fec_frame_slip_monitor_cfg(h, FALSE); 
                }        
                if (h->var.lineotn_dsi_dist[i].dsi_fec_type != fec_type && 
                    h->var.lineotn_dsi_dist[i].dsi_fec_type != LAST_LINE_OTN_FEC)
                {
                    rc = fec_blk_deprov_stream(h,i);
                    h->var.lineotn_dsi_dist[i].dsi_fec_type = LAST_LINE_OTN_FEC;
            
                }

                /* Now, provision the new FEC selection */
                if (PMC_SUCCESS == rc) 
                {
                    rc = fec_blk_prov_stream(h,i,fec_type);
                }       
            }
            if(rc != PMC_SUCCESS) 
            {
                break;
            }
        }
    }
    
    if(PMC_SUCCESS==rc && 0 == dsi_streams_count )
    {
        rc = LINEOTN_ERR_FEC_SET_FAILED;
    }

    PMC_RETURN(rc);
}/* lineotn_fec_cfg */


/*******************************************************************************
* lineotn_fec_act
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This function activates an already provisioned FEC.  Specifically, only 
*   the 100G Swizzle FEC needs to be activated following provisioning and it
*   relies upon having traffic flowing through the FEC.
*
*   It is the user responsibility to allocate the correct fec type to the correct dsi
*   stream as not all fec type are available for every data rate.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance 
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_fec_cfg for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type:
*                            see lineotn_fec_cfg for a detailed description
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_fec_act ( lineotn_handle_t *h,
                                   UINT32 dsi_stream,
                                   lineotn_dsi_type_t dsi_type)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    lineotn_fec_t fec_type;

    PMC_ENTRY();

    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    if (PMC_SUCCESS == rc) 
    {
        rc = lineotn_fec_type_get(h, dsi_stream, dsi_type, &fec_type);
    }

    /* Enable Frame Slip Monitor if FEC type is Swizzle 100G */
    if (PMC_SUCCESS == rc && LINE_OTN_FEC_SWIZZLE_100G == fec_type) 
    {
        rc = lineotn_swizzle_fec_frame_slip_monitor_cfg(h, TRUE);
    }

    PMC_RETURN(rc);

} /* lineotn_fec_act */


/*******************************************************************************
* lineotn_fec_bypass_cfg
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This function configures the fec type for an already provisioned dsi stream.
*
*   It is the user responsibility to allocate the correct fec type to the correct dsi
*   stream as not all fec type are available for every data rate.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance 
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_fec_cfg for a detailed description
*                            
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_fec_cfg for a detailed description
*
*   fec_bypass             - enum,requested fec type for the dsi stream: 
*                            LINE_OTN_FEC_BYPASS, 
*                            LINE_OTN_FEC_NO_BYPASS_SET 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   LINEOTN_ERR_FEC_SET_FAILED  - Internally generated error code
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
*   1. invoking this function with a dsi stream that is not provisioned
*      will result in an error status returned.\n
*   2. invoking this function with a dsi stream that has a FEC type already provisioned
*      without going through deprovision (LINE_OTN_FEC_NONE) will result in an error
*      status returned.\n
*   3. Invoking this function puts the fec into operational mode and removes
*      any bypass set earlier. \n
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_fec_bypass_cfg          ( lineotn_handle_t   *h,
                                                   UINT32             dsi_stream,
                                                   lineotn_dsi_type_t dsi_type,
                                                   lineotn_fec_bypass_t fec_bypass )
{
    PMC_ERROR   rc = PMC_SUCCESS;
    UINT8       loop_counter = 0;
    UINT8       loop_start = 0;
    UINT8       i = 0;
    UINT8       dsi_streams_count = 0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    PMC_ASSERT((UINT32)fec_bypass< (UINT32)LAST_LINE_OTN_FEC,LINEOTN_ERR_INVALID_PARAMETERS,fec_bypass,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);


    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        /* get the range of dsi stream configuration */
        lineotn_calc_dsi_range(h,dsi_start,&loop_start,&loop_counter);

        for(i=loop_start;i<loop_counter;i++)
        {
            if(h->var.lineotn_dsi_dist[i].dsi_provisioned) /* configure only provisioned streams */
            {
                dsi_streams_count++; /* count the configured dsi streams */

                switch(fec_bypass)
                {
                case LINE_OTN_FEC_BYPASS:
                    if ( FALSE == h->var.lineotn_dsi_dist[i].fec_bypassed )
                    {
                        rc = fec_mfec_cfg_set(h,i,LINE_OTN_FEC_NONE);
                        if (PMC_SUCCESS == rc)
                        {
                            rc = fec_mfec_bypass_set(h,i,TRUE);
                        }
                    }
                    break;

                case LINE_OTN_FEC_NO_BYPASS_SET:
                    if ( TRUE == h->var.lineotn_dsi_dist[i].fec_bypassed )
                        rc = fec_mfec_bypass_set(h,i,FALSE);
                    break;

                default:
                    rc = LINEOTN_ERR_INVALID_PARAMETERS;
                    break;
                }
            }

            if(rc!=PMC_SUCCESS) /* incase of error break and report */
                break;
        }

    }

    /* check how many dsi streams where configured --> at least one stream should have been configured */
    if (PMC_SUCCESS==rc && 0 == dsi_streams_count ) {
        rc = LINEOTN_ERR_FEC_SET_FAILED;
    }

    PMC_RETURN(rc);
}/* lineotn_fec_bypass_cfg */


/*******************************************************************************
* lineotn_consq_action_cfg
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Enables/Disables the requested consequential action per defect setting.
*   In order to enable/disable a consequential action at least one relevant defect
*   bit/s should appear in the defect bit mask variable.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            - for 10G configurations valid values are: (0..11) 
*                             dsi_type must be set to LINE_OTN_DSI_TYPE_OTUK 
*                            - for OTL3.4 40G configurations valid values are: (0,4,8)- 
*                              first lane in each 40G group. 
*                              dsi_type must be: LINE_OTN_DSI_TYPE_OTL for OTL3.4 
*                            - for 40G SFIS configurations valid values are: (0,5) - 
*                              first lane in each SFIS 40G group.
*                              dsi_type must be: LINE_OTN_DSI_TYPE_SFIS 
*                            - for 40G SFI5.1 configurations valid values are: (0,17) - 
*                              first lane in each SFIS 40G group.
*                              dsi_type must be: LINE_OTN_DSI_TYPE_SFI_51_X (X=A,B)
*                            - for 100G configurations valid values are: (0) -
*                              dsi_type can be either:
*                              LINE_OTN_DSI_TYPE_OTL   for OTL4.10 configuration or 
*                              LINE_OTN_DSI_TYPE_SFIS  for SFIS based configurations 
*
*   dsi_type               - enumerated lineotn dsi stream type:
*                            - LINE_OTN_DSI_TYPE_SFI_51_A : marks SFI5.1 dsi lanes 0-16 
*                            - LINE_OTN_DSI_TYPE_SFI_51_B : marks SFI5.1 dsi lanes 17-33 
*                            - LINE_OTN_DSI_TYPE_SFIS     : marks SFIS dsi lanes 
*                            - LINE_OTN_DSI_TYPE_OTL      : marks OTL 3.4/4.10 dsi lanes 
*                            - LINE_OTN_DSI_TYPE_OTUK     : marks 10G OTUk lanes 
*
*   cons_action_type       - enum, requested consequential action: 
*                            LINE_OTN_A_TSF,
*                            LINE_OTN_A_SSF,
*                            LINE_OTN_A_TSD,
*                            LINE_OTN_A_IAE,
*                            LINE_OTN_A_SSD,
*                            LINE_OTN_A_AIS,
*                            LINE_OTN_A_BEI,
*                            LINE_OTN_A_BIAE,
*                            LINE_OTN_A_BDI,
*                            LINE_OTN_A_FFAIL 
*
*   defect_bm              - unsigned
*                            - relevant defect bit mask based on the following bits: 
*                            LINE_OTN_D_LOS_P,
*                            LINE_OTN_D_LOL_P,
*                            LINE_OTN_D_LOFLANE,
*                            LINE_OTN_D_AIS,
*                            LINE_OTN_D_LOF,
*                            LINE_OTN_D_LOM,
*                            LINE_OTN_D_TIM,
*                            LINE_OTN_D_IAE,
*                            LINE_OTN_D_DEG,
*                            LINE_OTN_N_BIPV,
*                            LINE_OTN_IAE,
*                            LINE_OTN_D_BDI_FROM_SSF,
*                            LINE_OTN_D_TSF_FROM_SSF,
*                            LINE_OTN_D_SSF_FROM_TSF,
*                            LINE_OTN_D_AIS_FROM_TSF,
*                            LINE_OTN_D_SSD_FROM_TSD
*                            - or a pre-defined activation bit masks which set all relevant bits: 
*                            LINE_OTN_A_TSF_ACT,
*                            LINE_OTN_A_SSF_ACT,
*                            LINE_OTN_A_BDI_ACT,
*                            LINE_OTN_N_BEI_ACT,
*                            LINE_OTN_N_BIAE_ACT,
*                            LINE_OTN_A_TSD_ACT,
*                            LINE_OTN_A_SSD_ACT,
*                            LINE_OTN_A_IAE_ACT,
*                            LINE_OTN_A_AIS_ACT,
*                            LINE_OTN_A_FFAIL_ACT,
*
*   set_defects           - boolean, TRUE : set the defect to active mode; 
*                                    FALSE: reset defect to non-active mode
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
*   1) Note that the pre-defined activation bit masks include both OPSM and OTUk \n
*      defects; but this function will not enable OPSM related defects if the OTL \n
*      is not used.
* 
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_consq_action_cfg  ( lineotn_handle_t          *h,
                                             UINT32                    dsi_stream,
                                             lineotn_dsi_type_t        dsi_type,
                                             lineotn_action_type_t     cons_action_type,
                                             UINT32                    defect_bm,
                                             BOOL                      set_defects)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    UINT8       dci_channel=0;
    UINT32      temp_bm = 0;
    UINT32      defect_bit = 0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_deskew_type_t       deskew_type = LAST_LINE_OTN_DESKEW_TYPE;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;
    UINT32                      shift_bit=1;
    BOOL                        opsm_layer_used = FALSE;

    PMC_ENTRY();

    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);
    PMC_ASSERT(defect_bm>0,LINEOTN_ERR_INVALID_PARAMETERS,defect_bm,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);
    deskew_type = h->var.lineotn_dsi_dist[dsi_start].deskew_type;

    if ((deskew_type == LINE_OTN_ENHANCED_SFIS_DESKEW) ||
        (dsi_type == LINE_OTN_DSI_TYPE_OTL))
    {
        opsm_layer_used = TRUE;
    }

    if (((defect_bm & LINE_OTN_A_SSF_OPSM_A) == LINE_OTN_A_SSF_OPSM_A) && (opsm_layer_used == FALSE))
    {
        /* It appears that the pre-defined activation function was used; but the
           OPSM layer is not active; thus mask any OPSM-layer related defects*/
        defect_bm = defect_bm & (~ (LINE_OTN_D_LOL_P | LINE_OTN_D_LOFLANE));
    }
    temp_bm = defect_bm;

    if(PMC_SUCCESS == rc)
    {
        /* configure only provisioned streams, 
         * or for streams that are about to be provisioned */
        if ((h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned) ||
            ((cons_action_type == LINE_OTN_A_AIS) && 
             (defect_bm == LINE_OTN_D_LOM))) 
        {
            dci_channel = h->var.lineotn_dsi_dist[dsi_start].dci_channel;

            switch(cons_action_type)
            {
            case LINE_OTN_A_TSF:
                /* go over bits and configure accordingly */
                do
                {
                    defect_bit =  (temp_bm & shift_bit);
                    temp_bm &= ~shift_bit; /* reset the bit on the temp_bm */

                    if(defect_bit > 0)
                    {
                        switch(defect_bit)
                        {
                        case LINE_OTN_D_LOL_P:
                        case LINE_OTN_D_LOFLANE:
                            if (opsm_layer_used == TRUE)
                            {
                                rc = mld_opsm_ssf_defect_set(h,dsi_start,defect_bit,set_defects);
                            }                            
                            else 
                            { 
                                rc = LINEOTN_ERR_INVALID_PARAMETERS; 
                                temp_bm = 0; 
                            } 
                            break;

                        case LINE_OTN_D_LOS_P:
                        case LINE_OTN_D_AIS:
                        case LINE_OTN_D_LOF:
                            rc = mld_opsm_ssf_defect_set(h,dsi_start,defect_bit,set_defects);
                            break;

                        case LINE_OTN_D_LOM:
                            if(set_defects)
                            {
                                if(oduk_type<UTIL_GLOBAL_ODU3) /*10G stream */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_10G,dsi_start,(otu_frm_bit_value_t)1); /* enable */
                                else if(oduk_type<UTIL_GLOBAL_ODU4) /*40G stream */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_40G,(dsi_start/4),(otu_frm_bit_value_t)1); /* enable */
                                else /* 100G */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_100G,0,(otu_frm_bit_value_t)1); /* enable */
                            }
                            else
                            {
                                if(oduk_type<UTIL_GLOBAL_ODU3) /*10G stream */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_10G,dsi_start,(otu_frm_bit_value_t)0); /* disable */
                                else if(oduk_type<UTIL_GLOBAL_ODU4) /*40G stream */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_40G,(dsi_start/4),(otu_frm_bit_value_t)0); /* disable */
                                else /* 100G */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_100G,0,(otu_frm_bit_value_t)0); /* disable */
                            }
                            break;

                        case LINE_OTN_D_TIM:
                            if(set_defects)
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_TIM_ACT_DIS,0 ); /* enable */
                            else
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_TIM_ACT_DIS,1 ); /* disable */
                            break;

                        case LINE_OTN_D_TSF_FROM_SSF:
                            if(set_defects)
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_TSF_DIS_FROM_SSF,0 ); /* enable */
                            else
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_TSF_DIS_FROM_SSF,1 ); /* disable */
                            break;

                        default:
                            rc = LINEOTN_ERR_INVALID_PARAMETERS;
                            temp_bm = 0; /* force 0 to break loop and report error */
                            break;
                        }
                    }

                    if (PMC_SUCCESS == rc)
                        shift_bit=(shift_bit<<1);
                    else
                        break; /* stop and report error */

                } while(temp_bm > 0);

                break;

            case LINE_OTN_A_BDI:
                /* go over bits and configure accordingly */
                do
                {
                    defect_bit =  (temp_bm & shift_bit);
                    temp_bm &= ~shift_bit; /* reset the bit on the temp_bm */

                    if(defect_bit > 0)
                    {
                        switch(defect_bit)
                        {
                        case LINE_OTN_D_LOL_P:
                        case LINE_OTN_D_LOFLANE:
                            if (opsm_layer_used == TRUE)
                            {
                                rc = mld_opsm_ssf_defect_set(h,dsi_start,defect_bit,set_defects);
                            }              
                            else 
                            { 
                                rc = LINEOTN_ERR_INVALID_PARAMETERS; 
                                temp_bm = 0; 
                            } 
                            break;

                        case LINE_OTN_D_LOS_P:
                        case LINE_OTN_D_AIS:
                        case LINE_OTN_D_LOF:
                            rc = mld_opsm_ssf_defect_set(h,dsi_start,defect_bit,set_defects);
                            break;

                        case LINE_OTN_D_LOM:
                            if(set_defects)
                            {
                                if(oduk_type<UTIL_GLOBAL_ODU3) /*10G stream */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_10G,dsi_start,(otu_frm_bit_value_t)1); /* enable */
                                else if(oduk_type<UTIL_GLOBAL_ODU4) /*40G stream */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_40G,(dsi_start/4),(otu_frm_bit_value_t)1); /* enable */
                                else /* 100G */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_100G,0,(otu_frm_bit_value_t)1); /* enable */
                            }
                            else
                            {
                                if(oduk_type<UTIL_GLOBAL_ODU3) /*10G stream */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_10G,dsi_start,(otu_frm_bit_value_t)0); /* disable */
                                else if(oduk_type<UTIL_GLOBAL_ODU4) /*40G stream */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_40G,(dsi_start/4),(otu_frm_bit_value_t)0); /* disable */
                                else /* 100G */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_100G,0,(otu_frm_bit_value_t)0); /* disable */
                            }
                            break;

                        case LINE_OTN_D_TIM:
                            if(set_defects)
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_BDI_DIS_FROM_TTI,0 ); /* enable */
                            else
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_BDI_DIS_FROM_TTI,1 ); /* disable */
                            break;

                        case LINE_OTN_D_BDI_FROM_SSF:
                            if(set_defects)
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_BDI_DIS_FROM_SSF,0 ); /* enable */
                            else
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_BDI_DIS_FROM_SSF,1 ); /* disable */
                            break;

                        default:
                            rc = LINEOTN_ERR_INVALID_PARAMETERS;
                            temp_bm = 0; /* force 0 to break loop and report error */
                            break;
                        }
                    }

                    if (PMC_SUCCESS == rc)
                        shift_bit=(shift_bit<<1);
                    else
                        break; /* stop and report error */

                }while(temp_bm > 0);

                break;

            case LINE_OTN_A_SSF:
                /* go over bits and configure accordingly */
                do
                {
                    defect_bit =  (temp_bm & shift_bit);
                    temp_bm &= ~shift_bit; /* reset the bit on the temp_bm */

                    if(defect_bit > 0)
                    {
                        switch(defect_bit)
                        {
                        case LINE_OTN_D_LOL_P:
                        case LINE_OTN_D_LOFLANE:
                            if (opsm_layer_used == TRUE)
                            {
                                rc = mld_opsm_ssf_defect_set(h,dsi_start,defect_bit,set_defects);
                            }
                            else 
                            { 
                                rc = LINEOTN_ERR_INVALID_PARAMETERS; 
                                temp_bm = 0; 
                            }              
                            break;

                        case LINE_OTN_D_LOS_P:
                        case LINE_OTN_D_AIS:
                        case LINE_OTN_D_LOF:
                            rc = mld_opsm_ssf_defect_set(h,dsi_start,defect_bit,set_defects);
                            break;

                        case LINE_OTN_D_LOM:
                            if(set_defects)
                            {
                                if(oduk_type<UTIL_GLOBAL_ODU3) /*10G stream */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_10G,dsi_start,(otu_frm_bit_value_t)1); /* enable */
                                else if(oduk_type<UTIL_GLOBAL_ODU4) /*40G stream */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_40G,(dsi_start/4),(otu_frm_bit_value_t)1); /* enable */
                                else /* 100G */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_100G,0,(otu_frm_bit_value_t)1); /* enable */
                            }
                            else
                            {
                                if(oduk_type<UTIL_GLOBAL_ODU3) /*10G stream */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_10G,dsi_start,(otu_frm_bit_value_t)0); /* disable */
                                else if(oduk_type<UTIL_GLOBAL_ODU4) /*40G stream */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_40G,(dsi_start/4),(otu_frm_bit_value_t)0); /* disable */
                                else /* 100G */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_100G,0,(otu_frm_bit_value_t)0); /* disable */
                            }
                            break;

                        case LINE_OTN_D_TIM:
                            if(set_defects)
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_TIM_ACT_DIS,0 ); /* enable */
                            else
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_TIM_ACT_DIS,1 ); /* disable */
                            break;

                        case LINE_OTN_D_TSF_FROM_SSF:
                            if(set_defects)
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_TSF_DIS_FROM_SSF,0 ); /* enable */
                            else
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_TSF_DIS_FROM_SSF,1 ); /* disable */
                            break;

                        case LINE_OTN_D_SSF_FROM_TSF:
                            if(set_defects)
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_ADAPT_SSF_DIS,0 ); /* enable */
                            else
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_ADAPT_SSF_DIS,1 ); /* disable */
                            break;

                        default:
                            rc = LINEOTN_ERR_INVALID_PARAMETERS;
                            temp_bm = 0; /* force 0 to break loop and report error */
                            break;
                        }
                    }

                    if (PMC_SUCCESS == rc)
                        shift_bit=(shift_bit<<1);
                    else
                        break; /* stop and report error */

                }while(temp_bm > 0);

                break;

            case LINE_OTN_A_TSD:
                if ( LINE_OTN_D_DEG == defect_bm )
                {
                    if(set_defects)
                        rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_TSD_DIS,0 ); /* enable */
                    else
                        rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_TSD_DIS,1 ); /* disable */
                }
                else
                {
                    rc = LINEOTN_ERR_INVALID_PARAMETERS;
                }
                break;

            case LINE_OTN_A_IAE:
                if ( LINE_OTN_IAE == defect_bm )
                {
                    if(set_defects) {
                        rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SO_SM_IAE_INS_CTRL,1); /* enable */
                    } 
                    else
                    {
                        rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SO_SM_IAE_INS_CTRL,0); /* disable */
                    }
                }
                else 
                {
                    rc = LINEOTN_ERR_INVALID_PARAMETERS;
                }

                break;

            case LINE_OTN_A_SSD:
                /* go over bits and configure accordingly */
                do
                {
                    defect_bit =  (temp_bm & shift_bit);
                    temp_bm &= ~shift_bit; /* reset the bit on the temp_bm */

                    if(defect_bit > 0)
                    {
                        switch(defect_bit)
                        {

                        case LINE_OTN_D_DEG:
                            if(set_defects)
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_TSD_DIS,0 ); /* enable */
                            else
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_TSD_DIS,1 ); /* disable */
                            break;

                        case LINE_OTN_D_SSD_FROM_TSD:
                            if(set_defects)
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_ADAPT_SSD_DIS,0 ); /* enable */
                            else
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_ADAPT_SSD_DIS,1 ); /* disable */
                            break;

                        case LINE_OTN_D_SSF_FROM_TSF:
                            if(set_defects)
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_ADAPT_SSF_DIS,0 ); /* enable */
                            else
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_ADAPT_SSF_DIS,1 ); /* disable */
                            break;

                        default:
                            rc = LINEOTN_ERR_INVALID_PARAMETERS;
                            temp_bm = 0; /* force 0 to break loop and report error */
                            break;
                        }
                    }

                    if (PMC_SUCCESS == rc)
                        shift_bit=(shift_bit<<1);
                    else
                        break; /* stop and report error */

                }while(temp_bm > 0);

                break;

            case LINE_OTN_A_AIS:
                /* go over bits and configure accordingly */
                do
                {
                    defect_bit =  (temp_bm & shift_bit);
                    temp_bm &= ~shift_bit; /* reset the bit on the temp_bm */

                    if(defect_bit > 0)
                    {
                        switch(defect_bit)
                        {
                        case LINE_OTN_D_LOL_P:
                        case LINE_OTN_D_LOFLANE:
                            if (opsm_layer_used == TRUE)
                            {
                                rc = mld_opsm_ssf_defect_set(h,dsi_start,defect_bit,set_defects);
                            }
                            else 
                            { 
                                rc = LINEOTN_ERR_INVALID_PARAMETERS; 
                                temp_bm = 0; 
                            } 
                            break;

                        case LINE_OTN_D_LOS_P:
                        case LINE_OTN_D_AIS:
                        case LINE_OTN_D_LOF:
                            rc = mld_opsm_ssf_defect_set(h,dsi_start,defect_bit,set_defects);
                            break;

                        case LINE_OTN_D_LOM:
                            if(set_defects)
                            {
                                if(oduk_type<UTIL_GLOBAL_ODU3) /*10G stream */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_10G,dsi_start,(otu_frm_bit_value_t)1); /* enable */
                                else if(oduk_type<UTIL_GLOBAL_ODU4) /*40G stream */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_40G,(dsi_start/4),(otu_frm_bit_value_t)1); /* enable */
                                else /* 100G */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_100G,0,(otu_frm_bit_value_t)1); /* enable */
                            }
                            else
                            {
                                if(oduk_type<UTIL_GLOBAL_ODU3) /*10G stream */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_10G,dsi_start,(otu_frm_bit_value_t)0); /* disable */
                                else if(oduk_type<UTIL_GLOBAL_ODU4) /*40G stream */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_40G,(dsi_start/4),(otu_frm_bit_value_t)0); /* disable */
                                else /* 100G */
                                    rc = otu_frm_dlom_to_sf_set( h->otu_frm_handle,RATE_100G,0,(otu_frm_bit_value_t)0); /* disable */
                            }
                            break;

                        case LINE_OTN_D_TIM:
                            if(set_defects)
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_TIM_ACT_DIS,0 ); /* enable */
                            else
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_TIM_ACT_DIS,1 ); /* disable */
                            break;

                        case LINE_OTN_D_TSF_FROM_SSF:
                            if(set_defects)
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_TSF_DIS_FROM_SSF,0 ); /* enable */
                            else
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_TSF_DIS_FROM_SSF,1 ); /* disable */
                            break;

                        case LINE_OTN_D_AIS_FROM_TSF:
                            if(set_defects)
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_ADAPT_AIS_DIS,0 ); /* enable */
                            else
                                rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_ADAPT_AIS_DIS,1 ); /* disable */
                            break;

                        default:
                            rc = LINEOTN_ERR_INVALID_PARAMETERS;
                            temp_bm = 0; /* force 0 to break loop and report error */
                            break;
                        }
                    }

                    if (PMC_SUCCESS == rc)
                        shift_bit=(shift_bit<<1);
                    else
                        break; /* stop and report error */

                }while(temp_bm > 0);

                break;

            case LINE_OTN_A_BEI:
                if ( LINE_OTN_N_BIPV == defect_bm )
                {
                    if(set_defects)
                        rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_BEI_DIS,0 ); /* enable */
                    else
                        rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_BEI_DIS,1 ); /* disable */
                }
                else
                {
                    rc = LINEOTN_ERR_INVALID_PARAMETERS;
                }

                break;

            case LINE_OTN_A_BIAE:
                if ( LINE_OTN_D_IAE == defect_bm )
                {
                    if(set_defects)
                        rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_BIAE_DIS,0 ); /* enable */
                    else
                        rc = otu_frm_sm_ctl_set( h->otu_frm_handle,dci_channel,OTU_SK_BIAE_DIS,1 ); /* disable */
                }
                else
                {
                    rc = LINEOTN_ERR_INVALID_PARAMETERS;
                }

                break;

            case LINE_OTN_A_FFAIL:
                /* go over bits and configure accordingly */
                do
                {
                    defect_bit =  (temp_bm & shift_bit);
                    temp_bm &= ~shift_bit; /* reset the bit on the temp_bm */

                    if(defect_bit > 0)
                    {
                        switch(defect_bit)
                        {
                        case LINE_OTN_D_LOL_P:
                        case LINE_OTN_D_LOFLANE:
                            if (opsm_layer_used == TRUE)
                            {
                                rc = mld_opsm_ffail_defect_set(h,dsi_start,defect_bit,set_defects);
                            }   
                            else 
                            { 
                                rc = LINEOTN_ERR_INVALID_PARAMETERS; 
                                temp_bm = 0; 
                            }          
                            break;

                        case LINE_OTN_D_LOS_P:
                        case LINE_OTN_D_AIS:
                        case LINE_OTN_D_LOF:
                        case LINE_OTN_D_LOM:
                            rc = mld_opsm_ffail_defect_set(h,dsi_start,defect_bit,set_defects);
                            break;
  
                        default:
                            rc = LINEOTN_ERR_INVALID_PARAMETERS;
                            temp_bm = 0; /* force 0 to break loop and report error */
                            break;
                        }
                    }

                    if (PMC_SUCCESS == rc)
                        shift_bit=(shift_bit<<1);
                    else
                        break; /* stop and report error */

                }while(temp_bm > 0);

                break;

            default:
                rc = LINEOTN_ERR_INVALID_PARAMETERS;
                break;
            }

        }
    }

    PMC_RETURN(rc);
} /* lineotn_consq_action_cfg */


/*******************************************************************************
* lineotn_consq_action_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This function checks to see if a certain defect bit mask is configured in HW 
*   for a user specified consequential action.
*   User should provide a defect bit mask containing at least one defect bit set
*   and the requested consequential action to check this defect against.
*   Results are returned in a user pre allocated bit mask where every set bit 
*   stands for an activated defect bit.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_consq_action_cfg for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type
*
*   cons_action_type       - enum, requested consequential action:
*                            LINE_OTN_A_TSF, 
*                            LINE_OTN_A_SSF,
*                            LINE_OTN_A_TSD,
*                            LINE_OTN_A_IAE,
*                            LINE_OTN_A_SSD,
*                            LINE_OTN_A_AIS,
*                            LINE_OTN_A_BEI,
*                            LINE_OTN_A_BIAE,
*                            LINE_OTN_A_BDI,
*                            LINE_OTN_A_FFAIL
*
*   defect_bm              - unsigned,requested defect bit mask to check for based on the following bits: 
*                            LINE_OTN_D_LOS_P,
*                            LINE_OTN_D_LOL_P,
*                            LINE_OTN_D_LOFLANE,
*                            LINE_OTN_D_AIS,
*                            LINE_OTN_D_LOF,
*                            LINE_OTN_D_LOM,
*                            LINE_OTN_D_TIM,
*                            LINE_OTN_D_IAE,
*                            LINE_OTN_D_DEG,
*                            LINE_OTN_N_BIPV,
*                            LINE_OTN_IAE,
*                            LINE_OTN_D_BDI_FROM_SSF,
*                            LINE_OTN_D_TSF_FROM_SSF,
*                            LINE_OTN_D_SSF_FROM_TSF,
*                            LINE_OTN_D_AIS_FROM_TSF,
*                            LINE_OTN_D_SSD_FROM_TSD
*
*
* OUTPUTS:
*   *defects_set_bm        - pointer to unsigned,contains the defect bit mask for defects
*                            that are set for the consequential action. Bit mask will
*                            made out of the following bits:
*                            LINE_OTN_D_LOS_P,
*                            LINE_OTN_D_LOL_P,
*                            LINE_OTN_D_LOFLANE,
*                            LINE_OTN_D_AIS,
*                            LINE_OTN_D_LOF,
*                            LINE_OTN_D_LOM,
*                            LINE_OTN_D_TIM,
*                            LINE_OTN_D_IAE,
*                            LINE_OTN_D_DEG,
*                            LINE_OTN_N_BIPV,
*                            LINE_OTN_IAE,
*                            LINE_OTN_D_BDI_FROM_SSF,
*                            LINE_OTN_D_TSF_FROM_SSF,
*                            LINE_OTN_D_SSF_FROM_TSF,
*                            LINE_OTN_D_AIS_FROM_TSF,
*                            LINE_OTN_D_SSD_FROM_TSD
*
* RETURNS:
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
*     1) Note that the pre-defined activation bit masks used in the set function 
*        include both OPSM and OTUk defects; but the set function will not enable 
*        OPSM related defects if the OTL is not used. Thus for datapaths that do not 
*        use the OPSM layer (OTL interface), this get function may return a slightly 
*        different bit mask if the pre-defined activation bit masks were used.
* 
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_consq_action_cfg_get   ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_action_type_t     cons_action_type,
                                                  UINT32                    defect_bm,
                                                  UINT32                    *defects_set_bm)
{
    PMC_ERROR   rc = PMC_SUCCESS;
    UINT8       dci_channel=0;
    UINT32      temp_bm = defect_bm;
    UINT32      defect_bit = 0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;
    UINT32                      shift_bit=1;
    UINT32                      defect_field_value = 0;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);
    PMC_ASSERT(defect_bm>0,LINEOTN_ERR_INVALID_PARAMETERS,defect_bm,0);
    PMC_ASSERT(defects_set_bm!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);


    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        if(h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned) /* configure only provisioned streams */
        {
            dci_channel = h->var.lineotn_dsi_dist[dsi_start].dci_channel;
            *defects_set_bm = 0; /* reset output defect bit mask */
            switch(cons_action_type)
            {
            case LINE_OTN_A_TSF:
                /* go over bits and configure accordingly */
                do
                {
                    defect_bit =  (temp_bm & shift_bit);
                    temp_bm &= ~shift_bit; /* reset the bit on the temp_bm */

                    if(defect_bit > 0)
                    {
                        switch(defect_bit)
                        {
                        case LINE_OTN_D_LOS_P:
                        case LINE_OTN_D_LOL_P:
                        case LINE_OTN_D_LOFLANE:
                        case LINE_OTN_D_AIS:
                        case LINE_OTN_D_LOF:
                            defect_field_value = mld_opsm_ssf_defect_get(h,dsi_start,defect_bit);
                            if(defect_field_value>0) 
                            { /* if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            }
                            break;

                        case LINE_OTN_D_LOM:
                            if(oduk_type<UTIL_GLOBAL_ODU3) /*10G stream */
                                defect_field_value = otu_frm_dlom_to_sf_get( h->otu_frm_handle,RATE_10G,dsi_start); 
                            else if(oduk_type<UTIL_GLOBAL_ODU4) /*40G stream */
                                defect_field_value = otu_frm_dlom_to_sf_get( h->otu_frm_handle,RATE_40G,(dsi_start/4));
                            else /* 100G */
                                defect_field_value = otu_frm_dlom_to_sf_get( h->otu_frm_handle,RATE_100G,0); 

                            if(defect_field_value>0)
                            { /* if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            }
                            break;

                        case LINE_OTN_D_TIM:
                            defect_field_value = otu_frm_sm_ctl_get( h->otu_frm_handle,dci_channel,OTU_SK_TIM_ACT_DIS);

                            if(defect_field_value==0)
                            { /*reversed logic for this bit-->0 = enabled, if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            }
                            break;

                        case LINE_OTN_D_TSF_FROM_SSF:
                            defect_field_value = otu_frm_sm_ctl_get( h->otu_frm_handle,dci_channel,OTU_SK_TSF_DIS_FROM_SSF );

                            if(defect_field_value==0)
                            { /*reversed logic for this bit-->0 = enabled, if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            }
                            break;

                        default:
                            rc = LINEOTN_ERR_INVALID_PARAMETERS;
                            temp_bm = 0; /* force 0 to break loop and report error */
                            break;
                        }
                    }

                    if (PMC_SUCCESS == rc)
                        shift_bit=(shift_bit<<1);
                    else
                        break; /* stop and report error */

                }while(temp_bm > 0);

                break;

            case LINE_OTN_A_BDI:
                /* go over bits and configure accordingly */
                do
                {
                    defect_bit =  (temp_bm & shift_bit);
                    temp_bm &= ~shift_bit; /* reset the bit on the temp_bm */

                    if(defect_bit > 0)
                    {
                        switch(defect_bit)
                        {
                        case LINE_OTN_D_LOS_P:
                        case LINE_OTN_D_LOL_P:
                        case LINE_OTN_D_LOFLANE:
                        case LINE_OTN_D_AIS:
                        case LINE_OTN_D_LOF:
                            defect_field_value = mld_opsm_ssf_defect_get(h,dsi_start,defect_bit);
                            if(defect_field_value>0) /* if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            break;

                        case LINE_OTN_D_LOM:
                            if(oduk_type<UTIL_GLOBAL_ODU3) /*10G stream */
                                defect_field_value = otu_frm_dlom_to_sf_get( h->otu_frm_handle,RATE_10G,dsi_start); 
                            else if(oduk_type<UTIL_GLOBAL_ODU4) /*40G stream */
                                defect_field_value = otu_frm_dlom_to_sf_get( h->otu_frm_handle,RATE_40G,(dsi_start/4));
                            else /* 100G */
                                defect_field_value = otu_frm_dlom_to_sf_get( h->otu_frm_handle,RATE_100G,0); 

                            if(defect_field_value>0) /* if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            break;

                        case LINE_OTN_D_TIM:
                            defect_field_value = otu_frm_sm_ctl_get( h->otu_frm_handle,dci_channel,OTU_SK_BDI_DIS_FROM_TTI); 

                            if(defect_field_value==0) /*reversed logic for this bit-->0 = enabled, if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            break;

                        case LINE_OTN_D_BDI_FROM_SSF:
                            defect_field_value = otu_frm_sm_ctl_get( h->otu_frm_handle,dci_channel,OTU_SK_BDI_DIS_FROM_SSF);
                                    
                            if(defect_field_value==0) /* reversed logic for this bit-->0 = enabled, if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            break;

                        default:
                            rc = LINEOTN_ERR_INVALID_PARAMETERS;
                            temp_bm = 0; /* force 0 to break loop and report error */
                            break;
                        }
                    }

                    if (PMC_SUCCESS == rc)
                        shift_bit=(shift_bit<<1);
                    else
                        break; /* stop and report error */

                }while(temp_bm > 0);

                break;

            case LINE_OTN_A_SSF:
                /* go over bits and configure accordingly */
                do
                {
                    defect_bit =  (temp_bm & shift_bit);
                    temp_bm &= ~shift_bit; /* reset the bit on the temp_bm */

                    if(defect_bit > 0)
                    {
                        switch(defect_bit)
                        {
                        case LINE_OTN_D_LOS_P:
                        case LINE_OTN_D_LOL_P:
                        case LINE_OTN_D_LOFLANE:
                        case LINE_OTN_D_AIS:
                        case LINE_OTN_D_LOF:
                            defect_field_value = mld_opsm_ssf_defect_get(h,dsi_start,defect_bit);
                            if(defect_field_value>0) /* if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            break;

                        case LINE_OTN_D_LOM:
                            if(oduk_type<UTIL_GLOBAL_ODU3) /*10G stream */
                                defect_field_value = otu_frm_dlom_to_sf_get( h->otu_frm_handle,RATE_10G,dsi_start); 
                            else if(oduk_type<UTIL_GLOBAL_ODU4) /*40G stream */
                                defect_field_value = otu_frm_dlom_to_sf_get( h->otu_frm_handle,RATE_40G,(dsi_start/4));
                            else /* 100G */
                                defect_field_value = otu_frm_dlom_to_sf_get( h->otu_frm_handle,RATE_100G,0); 

                            if(defect_field_value>0) /* if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            break;

                        case LINE_OTN_D_TIM:
                            defect_field_value = otu_frm_sm_ctl_get( h->otu_frm_handle,dci_channel,OTU_SK_TIM_ACT_DIS);

                            if(defect_field_value==0) /*reversed logic for this bit-->0 = enabled, if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            break;

                        case LINE_OTN_D_TSF_FROM_SSF:
                            defect_field_value = otu_frm_sm_ctl_get( h->otu_frm_handle,dci_channel,OTU_SK_TSF_DIS_FROM_SSF );

                            if(defect_field_value==0) /*reversed logic for this bit-->0 = enabled, if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            break;

                        case LINE_OTN_D_SSF_FROM_TSF:
                            defect_field_value = otu_frm_sm_ctl_get( h->otu_frm_handle,dci_channel,OTU_SK_ADAPT_SSF_DIS); 

                            if(defect_field_value==0) /*reversed logic for this bit-->0 = enabled, if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            break;

                        default:
                            rc = LINEOTN_ERR_INVALID_PARAMETERS;
                            temp_bm = 0; /* force 0 to break loop and report error */
                            break;
                        }
                    }

                    if (PMC_SUCCESS == rc)
                        shift_bit=(shift_bit<<1);
                    else
                        break; /* stop and report error */

                }while(temp_bm > 0);

                break;

            case LINE_OTN_A_TSD:
                if ( LINE_OTN_D_DEG == defect_bm )
                {
                    defect_field_value = otu_frm_sm_ctl_get( h->otu_frm_handle,dci_channel,OTU_SK_TSD_DIS );

                    if(defect_field_value==0) /*reversed logic for this bit-->0 = enabled, if we found that the defect bit is set - mark it in the bm */
                        *defects_set_bm|=defect_bit;
                }
                else
                {
                    rc = LINEOTN_ERR_INVALID_PARAMETERS;
                }
                break;

            case LINE_OTN_A_IAE:
                if ( LINE_OTN_IAE == defect_bm )
                {
                    rc = otu_frm_sm_ctl_get(h->otu_frm_handle,dci_channel,OTU_SO_SM_IAE_INS_CTRL);

                    if(defect_field_value>0) /* if we found that the defect bit is set - mark it in the bm */
                        *defects_set_bm|=defect_bit;
                }
                else
                {
                    rc = LINEOTN_ERR_INVALID_PARAMETERS;
                }

                break;

            case LINE_OTN_A_SSD:
                /* go over bits and configure accordingly */
                do
                {
                    defect_bit =  (temp_bm & shift_bit);
                    temp_bm &= ~shift_bit; /* reset the bit on the temp_bm */

                    if(defect_bit > 0)
                    {
                        switch(defect_bit)
                        {

                        case LINE_OTN_D_DEG:
                            defect_field_value = otu_frm_sm_ctl_get( h->otu_frm_handle,dci_channel,OTU_SK_TSD_DIS); 
    
                            if(defect_field_value==0) /*reversed logic for this bit-->0 = enabled, if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            break;

                        case LINE_OTN_D_SSD_FROM_TSD:
                            defect_field_value = otu_frm_sm_ctl_get( h->otu_frm_handle,dci_channel,OTU_SK_ADAPT_SSD_DIS ); 

                            if(defect_field_value==0) /*reversed logic for this bit-->0 = enabled, if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            break;

                        case LINE_OTN_D_SSF_FROM_TSF:
                            defect_field_value = otu_frm_sm_ctl_get( h->otu_frm_handle,dci_channel,OTU_SK_ADAPT_SSF_DIS);

                            if(defect_field_value==0) /*reversed logic for this bit-->0 = enabled, if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            break;

                        default:
                            rc = LINEOTN_ERR_INVALID_PARAMETERS;
                            temp_bm = 0; /* force 0 to break loop and report error */
                            break;
                        }
                    }

                    if (PMC_SUCCESS == rc)
                        shift_bit=(shift_bit<<1);
                    else
                        break; /* stop and report error */

                }while(temp_bm > 0);

                break;

            case LINE_OTN_A_AIS:
                /* go over bits and configure accordingly */
                do
                {
                    defect_bit =  (temp_bm & shift_bit);
                    temp_bm &= ~shift_bit; /* reset the bit on the temp_bm */

                    if(defect_bit > 0)
                    {
                        switch(defect_bit)
                        {
                        case LINE_OTN_D_LOS_P:
                        case LINE_OTN_D_LOL_P:
                        case LINE_OTN_D_LOFLANE:
                        case LINE_OTN_D_AIS:
                        case LINE_OTN_D_LOF:
                            defect_field_value = mld_opsm_ssf_defect_get(h,dsi_start,defect_bit);
                            if(defect_field_value>0) /* if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            break;

                        case LINE_OTN_D_LOM:
                            if(oduk_type<UTIL_GLOBAL_ODU3) /*10G stream */
                                defect_field_value = otu_frm_dlom_to_sf_get( h->otu_frm_handle,RATE_10G,dsi_start); 
                            else if(oduk_type<UTIL_GLOBAL_ODU4) /*40G stream */
                                defect_field_value = otu_frm_dlom_to_sf_get( h->otu_frm_handle,RATE_40G,(dsi_start/4));
                            else /* 100G */
                                defect_field_value = otu_frm_dlom_to_sf_get( h->otu_frm_handle,RATE_100G,0); 

                            if(defect_field_value>0) /* if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            break;

                        case LINE_OTN_D_TIM:
                            defect_field_value = otu_frm_sm_ctl_get( h->otu_frm_handle,dci_channel,OTU_SK_TIM_ACT_DIS);

                            if(defect_field_value==0) /*reversed logic for this bit-->0 = enabled, if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            break;

                        case LINE_OTN_D_TSF_FROM_SSF:
                            defect_field_value = otu_frm_sm_ctl_get( h->otu_frm_handle,dci_channel,OTU_SK_TSF_DIS_FROM_SSF );

                            if(defect_field_value==0) /*reversed logic for this bit-->0 = enabled, if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            break;

                        case LINE_OTN_D_AIS_FROM_TSF:
                            defect_field_value = otu_frm_sm_ctl_get( h->otu_frm_handle,dci_channel,OTU_SK_ADAPT_AIS_DIS);

                            if(defect_field_value==0) /*reversed logic for this bit-->0 = enabled, if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            break;

                        default:
                            rc = LINEOTN_ERR_INVALID_PARAMETERS;
                            temp_bm = 0; /* force 0 to break loop and report error */
                            break;
                        }
                    }

                    if (PMC_SUCCESS == rc)
                        shift_bit=(shift_bit<<1);
                    else
                        break; /* stop and report error */

                }while(temp_bm > 0);

                break;

            case LINE_OTN_A_BEI:
                if ( LINE_OTN_N_BIPV == defect_bm )
                {
                    defect_field_value = otu_frm_sm_ctl_get( h->otu_frm_handle,dci_channel,OTU_SK_BEI_DIS);
                    if(defect_field_value==0) /*reversed logic for this bit-->0 = enabled, if we found that the defect bit is set - mark it in the bm */
                        *defects_set_bm|=defect_bit;
                }
                else
                {
                    rc = LINEOTN_ERR_INVALID_PARAMETERS;
                }

                break;

            case LINE_OTN_A_BIAE:
                if ( LINE_OTN_D_IAE == defect_bm )
                {
                    defect_field_value = otu_frm_sm_ctl_get( h->otu_frm_handle,dci_channel,OTU_SK_BIAE_DIS ); 
                    if(defect_field_value==0) /*reversed logic for this bit-->0 = enabled, if we found that the defect bit is set - mark it in the bm */
                        *defects_set_bm|=defect_bit;

                }
                else
                {
                    rc = LINEOTN_ERR_INVALID_PARAMETERS;
                }

                break;

            case LINE_OTN_A_FFAIL:
                /* go over bits and configure accordingly */
                do
                {
                    defect_bit =  (temp_bm & shift_bit);
                    temp_bm &= ~shift_bit; /* reset the bit on the temp_bm */

                    if(defect_bit > 0)
                    {
                        switch(defect_bit)
                        {
                        case LINE_OTN_D_LOS_P:
                        case LINE_OTN_D_LOL_P:
                        case LINE_OTN_D_LOFLANE:
                        case LINE_OTN_D_AIS:
                        case LINE_OTN_D_LOF:
                        case LINE_OTN_D_LOM:
                            defect_field_value = mld_opsm_ffail_defect_get(h,dsi_start,defect_bit);
                            if(defect_field_value>0) /* if we found that the defect bit is set - mark it in the bm */
                                *defects_set_bm|=defect_bit;
                            break;
  
                        default:
                            rc = LINEOTN_ERR_INVALID_PARAMETERS;
                            temp_bm = 0; /* force 0 to break loop and report error */
                            break;
                        }
                    }

                    if (PMC_SUCCESS == rc)
                        shift_bit=(shift_bit<<1);
                    else
                        break; /* stop and report error */

                }while(temp_bm > 0);

                break;

            default:
                rc = LINEOTN_ERR_INVALID_PARAMETERS;
                break;

            }

        }
    }

    if( PMC_SUCCESS != rc)
    {
        *defects_set_bm = 0; /* cleanup any defect residues that might have been set */
    } 
 
    PMC_RETURN(rc);
}/* lineotn_consq_action_cfg_get */


/*******************************************************************************
* lineotn_lof_mon_fw_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the LOF monitoring by FW. The monitoring is enabled when
*   the SFI-S or SFI5.1 interface is used.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type:
*                            see lineotn_stream_prov for a detailed description
* 
*   prov                   - TRUE: Perform configuration for provisioning.;
*                            FALSE: Perform configuration for deprovisioning.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
*   1. This routine is only called for the Rev A part.
* 
*******************************************************************************/


PUBLIC PMC_ERROR lineotn_lof_mon_fw_cfg ( lineotn_handle_t          *h,
                                          UINT32                    dsi_stream,
                                          lineotn_dsi_type_t        dsi_type,
                                          BOOL                      prov)
{
    PMC_ERROR                   rc = PMC_SUCCESS;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;
    BOOL                        config_mon = FALSE;
    UINT8                       intf_sel;

    PMC_ENTRY();

    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);

    if (FALSE == pmc_lof_monitoring_in_fw(&h->base))
    {
        PMC_ASSERT(FALSE, LINEOTN_INVALID_REVISION, 0, 0);
    }
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    /* Get post mld dsi stream range */
    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        if(h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned) /* configure only provisioned streams */
        {
            /* Configure LOF monitor if using SFI-S (OTU4, OTU3) or SFI5.1 (OTU3) 
           but not if the datapath involves OTL (i.e. OTL over SFI-S / SFI5.1) */
            if (4 == lineotn_frame_rate_otuk_num_get(h->var.lineotn_dsi_dist[dsi_start].dsi_rate))
            { 
                if(MLD_SFIS == h->var.lineotn_dsi_dist[dsi_start].dsi_hw_ifc &&
                   ! (LINE_OTN_ENHANCED_SFIS_DESKEW == h->var.lineotn_dsi_dist[dsi_start].deskew_type
                      ))
                {
                    config_mon = TRUE;
                    intf_sel = LINE_OTN_LOF_MON_INTF_OTU4;
                }
            } 
            else if (h->var.lineotn_dsi_dist[dsi_start].dsi_rate >= LINE_OTN_RATE_OTU3E2 &&
                     h->var.lineotn_dsi_dist[dsi_start].dsi_rate <= LINE_OTN_RATE_OTU3) 
            {
                if ((MLD_SFIS == h->var.lineotn_dsi_dist[dsi_start].dsi_hw_ifc || 
                     MLD_SFI_51== h->var.lineotn_dsi_dist[dsi_start].dsi_hw_ifc) &&
                        ! (LINE_OTN_ENHANCED_SFIS_DESKEW == h->var.lineotn_dsi_dist[dsi_start].deskew_type
                   ))
                {
                    config_mon = TRUE;
                    intf_sel = LINE_OTN_LOF_MON_INTF_OTU3_0 + dsi_start/4;
                }
            }

            if (config_mon) 
            {
                if (prov) {
                    /* Disable contribution of LOF to SSF and FEC_FAIL defects */
                    if(PMC_SUCCESS == rc)
                    {
                        rc = mld_opsm_ssf_defect_set(h, dsi_start, LINE_OTN_D_LOF, FALSE);
                    }
                    if(PMC_SUCCESS == rc)
                    {
                        rc = mld_opsm_ffail_defect_set(h, dsi_start, LINE_OTN_D_LOF, FALSE);
                    }
                } 
                else 
                {
                    /* Enable contribution of LOF to SSF and FEC_FAIL defects */
                    if(PMC_SUCCESS == rc)
                    {
                        rc = mld_opsm_ssf_defect_set(h, dsi_start, LINE_OTN_D_LOF, TRUE);
                    }
                    if(PMC_SUCCESS == rc)
                    {
                        rc = mld_opsm_ffail_defect_set(h, dsi_start, LINE_OTN_D_LOF, TRUE);
                    }
                } /* end if (prov) */
                /* Send host message to configure FW */
                if(PMC_SUCCESS == rc)
                {
                    rc = lineotn_lof_defect_monitor_cfg(h, h->var.lineotn_mode, intf_sel, prov);
                }
            } /* end if (config_mon) */
        } /* end if(h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned) */
    }

    PMC_RETURN(rc);
}/* lineotn_lof_mon_fw_cfg */


/*******************************************************************************
* lineotn_mfas_oh_ins_cfg
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Enables/Disables the requested mfas field OH insertion.
*   This function allows the user to set an mfas value to be inserted as an OH
*   field. By setting the mfas insertion type the user can choose an insertion
*   method out of 3 insertion methods specified bellow.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*                            
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
*   mfas_ins_type          - enum, requested insertion type: 
*                            - LINE_OTN_MFAS_INS_TYPE_UNMODIFIED: mfas will not be inserted; 
*                            - LINE_OTN_MFAS_INS_TYPE_USER_VALUE: user value mfas will be
*                            inserted 
*                            - LINE_OTN_MFAS_INS_TYPE_UPSTREAM_VALUE: mfas field is inserted
*                            via a freewheeling counter based on the upstream MFAS. 
*
*   mfas_value             - unsigned,user defined mfas value 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   LINEOTN_ERR_SM_OH_INS_FAILED  - Internally generated error code
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
*   1. For gracefully updating the OH field this function should be invoked
*      after a dsi stream is provisioned and before it is activated.\n
*   2. Invoking this function before a dsi stream is provisioned will result
*      in failure and an error status will be returned.\n
*   3. Invoking this function after a dsi stream is active (operational) might
*      cause a burst of alarms.\n
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_mfas_oh_ins_cfg ( lineotn_handle_t          *h,
                                           UINT32                    dsi_stream,
                                           lineotn_dsi_type_t        dsi_type,
                                           lineotn_mfas_ins_type_t   mfas_ins_type,
                                           UINT32                    mfas_value )
{
    PMC_ERROR   rc = PMC_SUCCESS;
    UINT8       dci_channel=0;
    UINT8       loop_counter = 0;
    UINT8       loop_start = 0;
    UINT8       i = 0;
    UINT8       dsi_streams_count = 0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);


    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        /* get the range of dsi stream configuration */
        lineotn_calc_dsi_range(h,dsi_start,&loop_start,&loop_counter);

        for(i=loop_start;i<loop_counter;i++)
        {
            dci_channel = h->var.lineotn_dsi_dist[i].dci_channel;

            if(h->var.lineotn_dsi_dist[i].dsi_provisioned) /* configure only provisioned streams */
            {
                dsi_streams_count++; /* count the configured dsi streams */

                switch(mfas_ins_type)
                {
                case LINE_OTN_MFAS_INS_TYPE_UNMODIFIED:
                    /* disable register based mfas insertion*/
                    rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SO_MFAS_INS_CTRL,0);
                    break;

                case LINE_OTN_MFAS_INS_TYPE_USER_VALUE:
                    rc = otu_frm_sm_oh_set(h->otu_frm_handle,dci_channel,OTU_SO_MFAS,mfas_value); /* set user value */
                    if ( PMC_SUCCESS == rc )
                        rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SO_MFAS_INS_CTRL,2);
                    break;

                case LINE_OTN_MFAS_INS_TYPE_UPSTREAM_VALUE:
                    rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SO_MFAS_INS_CTRL,1);
                    break;

                default:
                    rc = LINEOTN_ERR_INVALID_PARAMETERS;
                    break;
                }
            }

            if(rc!=PMC_SUCCESS) /* incase of error break and report */
                break;
        }
    }

    /* check how many dsi streams where configured --> at least one stream should have been configured */
    if(PMC_SUCCESS==rc && 0 == dsi_streams_count )
        rc = LINEOTN_ERR_OH_INS_FAILED;

    PMC_RETURN(rc);

}/* lineotn_mfas_oh_ins_cfg */


/*******************************************************************************
* lineotn_mfas_oh_ins_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Returns the MFAS OH insertion control field 
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*                            
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
* OUTPUTS:
*   mfas_ins_type          - enum, requested insertion type: 
*                            - LINE_OTN_MFAS_INS_TYPE_UNMODIFIED: mfas will not be inserted 
*                            - LINE_OTN_MFAS_INS_TYPE_USER_VALUE: user value mfas will be
*                            inserted 
*                            - LINE_OTN_MFAS_INS_TYPE_UPSTREAM_VALUE: mfas field is inserted
*                            via a freewheeling counter based on the upstream MFAS. 
*
*   mfas_value             - unsigned,user defined mfas value 
*
* RETURNS:
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_mfas_oh_ins_get ( lineotn_handle_t          *h,
                                           UINT32                    dsi_stream,
                                           lineotn_dsi_type_t        dsi_type,
                                           lineotn_mfas_ins_type_t   *mfas_ins_type,
                                           UINT32                    *mfas_value )
{
    PMC_ERROR   rc = PMC_SUCCESS;
    UINT8       dci_channel=0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);


    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {              
        if(h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned) 
        {   
            dci_channel = h->var.lineotn_dsi_dist[dsi_start].dci_channel;
            *mfas_ins_type = (lineotn_mfas_ins_type_t) otu_frm_sm_ctl_get(h->otu_frm_handle, dci_channel, OTU_SO_MFAS_INS_CTRL);
            *mfas_value = otu_frm_sm_oh_get(h->otu_frm_handle,dci_channel,OTU_SO_MFAS);
        }
        else 
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }                    
    }

    PMC_RETURN(rc);
}/* lineotn_mfas_oh_ins_get */


/*******************************************************************************
* lineotn_bip8_oh_ins_cfg
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Enables/Disables the requested bip8 field OH insertion.
*   This function allows the user to set a bip8 value to be inserted as an OH
*   field. By setting the bip8 insertion type the user can choose an insertion
*   method out of 5 insertion methods specified bellow.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
* 
*   dsi_type               - enumerated lineotn dsi stream type: \n
*                            see lineotn_stream_prov for a detailed description
*
*   bip8_ins_type          - enum, requested insertion type: 
*                            - LINE_OTN_BIP8_INS_DISABLED: bip8 will not be inserted 
*                            - LINE_OTN_BIP8_INS_TYPE_ENABLED:  The calculated
*                              bip8 value is inserted 
*                            - LINE_OTN_BIP8_INS_TYPE_UPSTREAM_XOR_USER_VALUE: calculated
*                              value insertion is disabled and the upstream SM bip8 value
*                              in the frame is XOR'd with the bip8 user value *
*                            - LINE_OTN_BIP8_INS_TYPE_CALCULATED_XOR_USER_VALUE:
*                              calculated SM bip8 value is XOR'd with the bip8
*                              user value
*                            - LINE_OTN_BIP8_INS_TYPE_USER_VALUE:
*                              bip8 constant user value insertion 
*   bip8_value             - unsigned,user defined bip8 value 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   LINEOTN_ERR_SM_OH_INS_FAILED  - Internally generated error code
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
*   1. For gracefully updating the OH field this function should be invoked
*      after a dsi stream is provisioned and before it is activated.\n
*   2. Invoking this function before a dsi stream is provisioned will result
*      in failure and an error status will be returned.\n
*   3. Invoking this function after a dsi stream is active (operational) might
*      cause a burst of alarms.\n
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_bip8_oh_ins_cfg ( lineotn_handle_t          *h,
                                           UINT32                    dsi_stream,
                                           lineotn_dsi_type_t        dsi_type,
                                           lineotn_bip8_ins_type_t   bip8_ins_type,
                                           UINT32                    bip8_value )
{
    PMC_ERROR   rc = PMC_SUCCESS;
    UINT8       dci_channel=0;
    UINT8       loop_counter = 0;
    UINT8       loop_start = 0;
    UINT8       i = 0;
    UINT8       dsi_streams_count = 0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);


    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        /* get the range of dsi stream configuration */
        lineotn_calc_dsi_range(h,dsi_start,&loop_start,&loop_counter);

        for(i=loop_start;i<loop_counter;i++)
        {
            dci_channel = h->var.lineotn_dsi_dist[i].dci_channel;

            if(h->var.lineotn_dsi_dist[i].dsi_provisioned) /* configure only provisioned streams */
            {
                dsi_streams_count++; /* count the configured dsi streams */

                switch(bip8_ins_type)
                {
                case LINE_OTN_BIP8_INS_DISABLED:
                    /* disable register based bip8 insertion*/
                    rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SO_SM_BIP8_INS_CTRL,0);
                    break;

                case LINE_OTN_BIP8_INS_TYPE_ENABLED:
                    rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SO_SM_BIP8_INS_CTRL,1);
                    break;

                case LINE_OTN_BIP8_INS_TYPE_UPSTREAM_XOR_USER_VALUE:
                    rc = otu_frm_sm_oh_set(h->otu_frm_handle,dci_channel,OTU_SO_SM_BIP8,bip8_value); /* set user value */
                    if ( PMC_SUCCESS == rc )
                        rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SO_SM_BIP8_INS_CTRL,2);
                    break;

                case LINE_OTN_BIP8_INS_TYPE_CALCULATED_XOR_USER_VALUE:
                    rc = otu_frm_sm_oh_set(h->otu_frm_handle,dci_channel,OTU_SO_SM_BIP8,bip8_value); /* set user value */
                    if ( PMC_SUCCESS == rc )
                        rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SO_SM_BIP8_INS_CTRL,3);
                    break;

                case LINE_OTN_BIP8_INS_TYPE_USER_VALUE:
                    rc = otu_frm_sm_oh_set(h->otu_frm_handle,dci_channel,OTU_SO_SM_BIP8,bip8_value); /* set user value */
                    if ( PMC_SUCCESS == rc )
                        rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SO_SM_BIP8_INS_CTRL,4);
                    break;

                default:
                    rc = LINEOTN_ERR_INVALID_PARAMETERS;
                    break;
                }
            }

            if(rc!=PMC_SUCCESS) /* incase of error break and report */
                break;
        }

    }

    /* check how many dsi streams where configured --> at least one stream should have been configured */
    if (PMC_SUCCESS==rc && 0 == dsi_streams_count )
        rc = LINEOTN_ERR_OH_INS_FAILED;

    PMC_RETURN(rc);
}/* lineotn_bip8_oh_ins_cfg */

/*******************************************************************************
* lineotn_bip8_oh_ins_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Returns the requested bip8 OH insertion field.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
* OUTPUTS:
*   bip8_ins_type          - enum, requested insertion type: 
*                            - LINE_OTN_BIP8_INS_DISABLED: bip8 will not be inserted 
*                            - LINE_OTN_BIP8_INS_TYPE_ENABLED:  The calculated
*                            bip8 value is inserted 
*                            - LINE_OTN_BIP8_INS_TYPE_UPSTREAM_XOR_USER_VALUE: calculated
*                            value insertion is disabled and the upstream SM bip8 value
*                            in the frame is XOR'd with the bip8 user value
*                            - LINE_OTN_BIP8_INS_TYPE_CALCULATED_XOR_USER_VALUE:
*                            calculated SM bip8 value is XOR'd with the bip8
*                            user value 
*                            - LINE_OTN_BIP8_INS_TYPE_USER_VALUE:
*                            bip8 constant user value insertion 
*
*   bip8_value             - unsigned,user defined bip8 value 
*
* RETURNS:
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_bip8_oh_ins_get ( lineotn_handle_t          *h,
                                           UINT32                    dsi_stream,
                                           lineotn_dsi_type_t        dsi_type,
                                           lineotn_bip8_ins_type_t   *bip8_ins_type,
                                           UINT32                    *bip8_value )
{
    PMC_ERROR   rc = PMC_SUCCESS;
    UINT8       dci_channel=0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);


    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        if(h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned) /* configure only provisioned streams */
        {
            dci_channel = h->var.lineotn_dsi_dist[dsi_start].dci_channel;
            *bip8_ins_type = (lineotn_bip8_ins_type_t) otu_frm_sm_ctl_get(h->otu_frm_handle,dci_channel,OTU_SO_SM_BIP8_INS_CTRL);
            *bip8_value = otu_frm_sm_oh_get(h->otu_frm_handle,dci_channel,OTU_SO_SM_BIP8);
        }
        else
        {            
            rc = LINEOTN_ERR_INVALID_PARAMETERS;                        
        }
    }
    
    PMC_RETURN(rc);
}/* lineotn_bip8_oh_ins_get */


/*******************************************************************************
* lineotn_iae_oh_ins_cfg
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Enables/Disables the requested iae field OH insertion.
*   This function allows the user to set an iae  value to be inserted as an OH
*   field. By setting the iae insertion type the user can choose an insertion
*   method out of 3 insertion methods specified bellow.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
* 
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
*   iae_ins_type           - enum, requested insertion type:
*                            - LINE_OTN_IAE_INS_TYPE_UNMODIFIED:  value insertion is
*                              disabled and the iae field passes through unmodified from
*                              the upstream 
*                            - LINE_OTN_IAE_INS_TYPE_USER_VALUE: user value iae will be
*                              inserted 
*                            - LINE_OTN_IAE_INS_TYPE_DETECTED_VALUE: iae field contains
*                              the value detected by the OTUk/ODUk_A So function 
*
*   iae_value              - unsigned,user defined iae value
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   LINEOTN_ERR_SM_OH_INS_FAILED  - Internally generated error code
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
*   1. For gracefully updating the OH field this function should be invoked
*      after a dsi stream is provisioned and before it is activated.\n
*   2. Invoking this function before a dsi stream is provisioned will result
*      in failure and an error status will be returned.\n
*   3. Invoking this function after a dsi stream is active (operational) might
*      cause a burst of alarms.\n
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_iae_oh_ins_cfg ( lineotn_handle_t          *h,
                                          UINT32                    dsi_stream,
                                          lineotn_dsi_type_t        dsi_type,
                                          lineotn_iae_ins_type_t    iae_ins_type,
                                          UINT32                    iae_value )
{
    PMC_ERROR   rc = PMC_SUCCESS;
    UINT8       dci_channel=0;
    UINT8       loop_counter = 0;
    UINT8       loop_start = 0;
    UINT8       i = 0;
    UINT8       dsi_streams_count = 0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        /* get the range of dsi stream configuration */
        lineotn_calc_dsi_range(h,dsi_start,&loop_start,&loop_counter);

        for(i=loop_start;i<loop_counter;i++)
        {
            dci_channel = h->var.lineotn_dsi_dist[i].dci_channel;

            if(h->var.lineotn_dsi_dist[i].dsi_provisioned) /* configure only provisioned streams */
            {
                dsi_streams_count++; /* count the configured dsi streams */

                switch(iae_ins_type)
                {
                case LINE_OTN_IAE_INS_TYPE_UNMODIFIED:
                    /* disable register based iae insertion*/
                    rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SO_SM_IAE_INS_CTRL,0);
                    break;

                case LINE_OTN_IAE_INS_TYPE_USER_VALUE:
                    rc = otu_frm_sm_oh_set(h->otu_frm_handle,dci_channel,OTU_SO_SM_IAE,iae_value); /* set user value */
                    if ( PMC_SUCCESS == rc )
                        rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SO_SM_IAE_INS_CTRL,2);
                    break;

                case LINE_OTN_IAE_INS_TYPE_DETECTED_VALUE:
                    rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SO_SM_IAE_INS_CTRL,1);
                    break;

                default:
                    rc = LINEOTN_ERR_INVALID_PARAMETERS;
                    break;
                }
            }

            if(rc!=PMC_SUCCESS) /* incase of error break and report */
                break;
        }
    }

    /* check how many dsi streams where configured --> at least one stream should have been configured */
    if (PMC_SUCCESS==rc && 0 == dsi_streams_count )
        rc = LINEOTN_ERR_OH_INS_FAILED;

    PMC_RETURN(rc);
}/* lineotn_iae_oh_ins_cfg */


/*******************************************************************************
* lineotn_iae_oh_ins_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Returns the IAE OH insertion field
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
* OUTPUTS:
*   iae_ins_type           - enum, requested insertion type:
*                            - LINE_OTN_IAE_INS_TYPE_UNMODIFIED:  value insertion is
*                            disabled and the iae field passes through unmodified from
*                            the upstream 
*                            - LINE_OTN_IAE_INS_TYPE_USER_VALUE: user value iae will be
*                            inserted 
*                            - LINE_OTN_IAE_INS_TYPE_DETECTED_VALUE: iae field contains
*                            the value detected by the OTUk/ODUk_A So function 
*
*   iae_value              - unsigned,user defined iae value
*
* RETURNS:
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_iae_oh_ins_get ( lineotn_handle_t          *h,
                                          UINT32                    dsi_stream,
                                          lineotn_dsi_type_t        dsi_type,
                                          lineotn_iae_ins_type_t    *iae_ins_type,
                                          UINT32                    *iae_value )
{
    PMC_ERROR   rc = PMC_SUCCESS;
    UINT8       dci_channel=0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        if(h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned) 
        {
            dci_channel = h->var.lineotn_dsi_dist[dsi_start].dci_channel;
            *iae_ins_type = (lineotn_iae_ins_type_t) otu_frm_sm_ctl_get(h->otu_frm_handle,dci_channel,OTU_SO_SM_IAE_INS_CTRL);
            *iae_value = otu_frm_sm_oh_get(h->otu_frm_handle,dci_channel,OTU_SO_SM_IAE); 
        }
        else
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
    }

    PMC_RETURN(rc);
}/* lineotn_iae_oh_ins_get */


/*******************************************************************************
* lineotn_bkwd_oh_ins_cfg
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Enables/Disables the requested backward error indication (BEI BIAE,BDI)field OH
*   insertion.
*   This function allows the user to set a backward indication value to be inserted
*   as an OH field for either BEI_BIAE or BDI. By setting the bkwd_ins_type type the user
*   can choose an insertion method out of 3 insertion methods specified bellow.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:\n
*                            see lineotn_stream_prov for a detailed description
* 
*   dsi_type               - enumerated lineotn dsi stream type: \n
*                            see lineotn_stream_prov for a detailed description
*
*   bkwd_ctrl_ins_field    - enum,requested backward error field: 
*                            LINE_OTN_SO_BEI_BIAE, 
*                            LINE_OTN_SO_BDI 
*
*   bkwd_ins_type          - enum, requested insertion type: 
*                            - LINE_OTN_BKWD_INS_TYPE_UNMODIFIED:
*                              BDI/BEI/BIAE field passes through unmodified from
*                              the upstream 
*                            - LINE_OTN_BKWD_INS_TYPE_USER_VALUE: user value BDI/BEI/BIAE
*                              is inserted 
*                            - LINE_OTN_BKWD_INS_TYPE_CONSQ_ACT_VALUE:  BDI/BEI/BIAE field
*                              contains the backwards consequential action value from the
*                              OTUk_Sk 
*
*   bkwd_value             - unsigned,user defined backward indication value\n
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   LINEOTN_ERR_SM_OH_INS_FAILED  - Internally generated error code
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
*   1. For gracefully updating the OH field this function should be invoked
*      after a dsi stream is provisioned and before it is activated.\n
*   2. Invoking this function before a dsi stream is provisioned will result
*      in failure and an error status will be returned.\n
*   3. Invoking this function after a dsi stream is active (operational) might
*      cause a burst of alarms.\n
* 
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_bkwd_oh_ins_cfg        ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_bkwd_oh_field_t   bkwd_ctrl_ins_field,
                                                  lineotn_bkwd_ins_type_t   bkwd_ins_type,
                                                  UINT32                    bkwd_value )
{
    PMC_ERROR           rc = PMC_SUCCESS;
    UINT8               dci_channel=0;
    UINT8               loop_counter = 0;
    UINT8               loop_start = 0;
    UINT8               i = 0;
    UINT8               dsi_streams_count = 0;
    otu_frm_ctl_field_t otu_frm_ins_field;
    otu_frm_sm_oh_t     otu_frm_oh_field;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(bkwd_ctrl_ins_field<LAST_LINE_OTN_BKWD_OH_FIELD,LINEOTN_ERR_INVALID_PARAMETERS,bkwd_ctrl_ins_field,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        /* get the range of dsi stream configuration */
        lineotn_calc_dsi_range(h,dsi_start,&loop_start,&loop_counter);

        /* set insertion field to BDI /BEI_BIAE */
        if(LINE_OTN_SO_BDI == bkwd_ctrl_ins_field)
        {
            otu_frm_ins_field = OTU_SO_SM_BDI_INS_CTRL;
            otu_frm_oh_field = OTU_SO_SM_BDI;
        }
        else
        {
            otu_frm_ins_field = OTU_SO_SM_BEI_BIAE_INS_CTRL;
            otu_frm_oh_field = OTU_SO_SM_BEI_BIAE;
        }

        for(i=loop_start;i<loop_counter;i++)
        {
            dci_channel = h->var.lineotn_dsi_dist[i].dci_channel;

            if(h->var.lineotn_dsi_dist[i].dsi_provisioned) /* configure only provisioned streams */
            {
                dsi_streams_count++; /* count the configured dsi streams */

                switch(bkwd_ins_type)
                {
                case LINE_OTN_BKWD_INS_TYPE_UNMODIFIED:
                    /* disable register based bkwd insertion*/
                    rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,otu_frm_ins_field,0);
                    break;

                case LINE_OTN_BKWD_INS_TYPE_USER_VALUE:
                    rc = otu_frm_sm_oh_set(h->otu_frm_handle,dci_channel,otu_frm_oh_field,bkwd_value); /* set user value */
                    if ( PMC_SUCCESS == rc )
                        rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,otu_frm_ins_field,2);
                    break;

                case LINE_OTN_BKWD_INS_TYPE_CONSQ_ACT_VALUE:
                    rc = otu_frm_sm_oh_set(h->otu_frm_handle,dci_channel,otu_frm_oh_field,bkwd_value); /* set user value */
                    if ( PMC_SUCCESS == rc )
                        rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,otu_frm_ins_field,1);
                    break;

                default:
                    rc = LINEOTN_ERR_INVALID_PARAMETERS;
                    break;
                }
            }

            if(rc!=PMC_SUCCESS) /* incase of error break and report */
                break;
        }
    }
    /* check how many dsi streams where configured --> at least one stream should have been configured */
    if(PMC_SUCCESS==rc && 0 == dsi_streams_count )
        rc = LINEOTN_ERR_OH_INS_FAILED;

    PMC_RETURN(rc);
}/* lineotn_bkwd_oh_ins_cfg */


/*******************************************************************************
* lineotn_bkwd_oh_ins_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Returns the backward error indication (BEI BIAE,BDI)field OH insertion field.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
* 
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
* 
*   bkwd_ctrl_ins_field    - enum,requested backward error field: 
*                            LINE_OTN_SO_BEI_BIAE,
*                            LINE_OTN_SO_BDI 
*
* OUTPUTS:
*   bkwd_ins_type          - enum, requested insertion type: 
*                            - LINE_OTN_BKWD_INS_TYPE_UNMODIFIED:
*                              BDI/BEI/BIAE field passes through unmodified from
*                              the upstream 
*                            - LINE_OTN_BKWD_INS_TYPE_USER_VALUE: user value BDI/BEI/BIAE
*                              is inserted 
*                            - LINE_OTN_BKWD_INS_TYPE_CONSQ_ACT_VALUE:  BDI/BEI/BIAE field
*                              contains the backwards consequential action value from the
*                              OTUk_Sk
*
*   bkwd_value             - unsigned,user defined backward indication value\n
*
* RETURNS:
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
* 
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_bkwd_oh_ins_get        ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_bkwd_oh_field_t   bkwd_ctrl_ins_field,
                                                  lineotn_bkwd_ins_type_t   *bkwd_ins_type,
                                                  UINT32                    *bkwd_value )
{
    PMC_ERROR           rc = PMC_SUCCESS;
    UINT8               dci_channel=0;
    otu_frm_ctl_field_t otu_frm_ins_field;
    otu_frm_sm_oh_t     otu_frm_oh_field;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(bkwd_ctrl_ins_field<LAST_LINE_OTN_BKWD_OH_FIELD,LINEOTN_ERR_INVALID_PARAMETERS,bkwd_ctrl_ins_field,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {        
        if(LINE_OTN_SO_BDI == bkwd_ctrl_ins_field)
        {
            otu_frm_ins_field = OTU_SO_SM_BDI_INS_CTRL;
            otu_frm_oh_field = OTU_SO_SM_BDI;
        }
        else
        {
            otu_frm_ins_field = OTU_SO_SM_BEI_BIAE_INS_CTRL;
            otu_frm_oh_field = OTU_SO_SM_BEI_BIAE;
        }

        if(h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned) 
        {
            dci_channel = h->var.lineotn_dsi_dist[dsi_start].dci_channel;
            *bkwd_ins_type = (lineotn_bkwd_ins_type_t) otu_frm_sm_ctl_get(h->otu_frm_handle,dci_channel,otu_frm_ins_field);
            *bkwd_value = otu_frm_sm_oh_get(h->otu_frm_handle,dci_channel,otu_frm_oh_field);
        }
        else
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
    }

    PMC_RETURN(rc);
}/* lineotn_bkwd_oh_ins_get */


/*******************************************************************************
* lineotn_oh_ins_cfg
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Enables/Disables the a requested field OH insertion.
*   This function handles a set of OH fields (listed bellow)
*   that have only 2 insertion options (either they are inserted or not).
*   Also, it allows to the user to specify an insertion value for the insertion
*   fields.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
*   ctrl_ins_field         - enum,requested control insertion field: 
*                            LINE_OTN_SO_FAS_F6,
*                            LINE_OTN_SO_FAS_28,
*                            LINE_OTN_SO_RES7,
*                            LINE_OTN_SO_RES8,
*                            LINE_OTN_SO_GCC0,
*                            LINE_OTN_SO_RES13,
*                            LINE_OTN_SO_RES14,
*
*   ctrl_ins_type          - enum, requested insertion type: 
*                            - LINE_OTN_INS_TYPE_UNMODIFIED:
*                              field passes through unmodified from the upstream 
*                            - LINE_OTN_INS_TYPE_USER_VALUE: user value
*                              is inserted 
*
*   ctrl_field_value       - unsigned,user defined insertion field value
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   LINEOTN_ERR_SM_OH_INS_FAILED  - Internally generated error code
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
*   1. For gracefully updating the OH field this function should be invoked
*      after a dsi stream is provisioned and before it is activated.\n
*   2. Invoking this function before a dsi stream is provisioned will result
*      in failure and an error status will be returned.\n
*   3. Invoking this function after a dsi stream is active (operational) might
*      cause a burst of alarms.\n
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_oh_ins_cfg             ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_oh_field_t        ctrl_ins_field,
                                                  lineotn_oh_ins_type_t     ctrl_ins_type,
                                                  UINT32                    ctrl_field_value )
{
    PMC_ERROR           rc = PMC_SUCCESS;
    UINT8               dci_channel=0;
    UINT8               loop_counter = 0;
    UINT8               loop_start = 0;
    UINT8               i = 0;
    UINT8               dsi_streams_count = 0;
    otu_frm_ctl_field_t otu_frm_ins_field = (otu_frm_ctl_field_t)0;
    otu_frm_sm_oh_t     otu_frm_oh_field = (otu_frm_sm_oh_t)0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(ctrl_ins_field<LAST_LINE_OTN_OH_FIELD,LINEOTN_ERR_INVALID_PARAMETERS,ctrl_ins_field,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        /* get the range of dsi stream configuration */
        lineotn_calc_dsi_range(h,dsi_start,&loop_start,&loop_counter);

        /* set insertion field to otu type insertion fields */
        switch(ctrl_ins_field)
        {
        case LINE_OTN_SO_RES7:
            otu_frm_ins_field = OTU_SO_SM_RES7_INS_CTRL;
            otu_frm_oh_field = OTU_SO_SM_RES7;
            break;

        case LINE_OTN_SO_RES8:
            otu_frm_ins_field = OTU_SO_SM_RES8_INS_CTRL;
            otu_frm_oh_field = OTU_SO_SM_RES8;
            break;

        case LINE_OTN_SO_GCC0:
            otu_frm_ins_field = OTU_SO_GCC0_INS_CTRL;
            otu_frm_oh_field = OTU_SO_GCC0;
            break;

        case LINE_OTN_SO_RES13:
            otu_frm_ins_field = OTU_SO_RES13_INS_CTRL;
            otu_frm_oh_field = OTU_SO_RES13;
            break;

        case LINE_OTN_SO_RES14:
            otu_frm_ins_field = OTU_SO_RES14_INS_CTRL;
            otu_frm_oh_field = OTU_SO_RES14;
            break;

        case LINE_OTN_SO_FAS_F6:
            otu_frm_ins_field = OTU_SO_FAS_INS_CTRL;
            otu_frm_oh_field = OTU_SO_FAS_F6;
            break;

        case LINE_OTN_SO_FAS_28:
            otu_frm_ins_field = OTU_SO_FAS_INS_CTRL;
            otu_frm_oh_field = OTU_SO_FAS_28;
            break;

        default:
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
            break;
        }

        if(PMC_SUCCESS == rc )
        {
            for(i=loop_start;i<loop_counter;i++)
            {
                dci_channel = h->var.lineotn_dsi_dist[i].dci_channel;

                if(h->var.lineotn_dsi_dist[i].dsi_provisioned) /* configure only provisioned streams */
                {
                    dsi_streams_count++; /* count the configured dsi streams */

                    switch(ctrl_ins_type)
                    {
                    case LINE_OTN_INS_TYPE_UNMODIFIED:
                        /* disable register based oh insertion*/
                        rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,otu_frm_ins_field,0);
                        break;

                    case LINE_OTN_INS_TYPE_USER_VALUE:
                        rc = otu_frm_sm_oh_set(h->otu_frm_handle,dci_channel,otu_frm_oh_field,ctrl_field_value); /* set user value */

                        if ( PMC_SUCCESS == rc )
                            rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,otu_frm_ins_field,1);
                        break;

                    default:
                        rc = LINEOTN_ERR_INVALID_PARAMETERS;
                        break;
                    }
                }

                if(rc!=PMC_SUCCESS) /* incase of error break and report */
                    break;
            }
        }
    }

    /* check how many dsi streams where configured --> at least one stream should have been configured */
    if(PMC_SUCCESS==rc && 0 == dsi_streams_count )
            rc = LINEOTN_ERR_OH_INS_FAILED;

    PMC_RETURN(rc);
}/* lineotn_oh_ins_cfg */


/*******************************************************************************
* lineotn_oh_ins_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Returns the configuration for the requested OH field: the inserted value and
*   the type of insertion (unmodified or user value)
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
*   ctrl_ins_field         - enum,requested control insertion field: 
*                            LINE_OTN_SO_FAS_F6,
*                            LINE_OTN_SO_FAS_28,
*                            LINE_OTN_SO_RES7,
*                            LINE_OTN_SO_RES8,
*                            LINE_OTN_SO_GCC0,
*                            LINE_OTN_SO_RES13,
*                            LINE_OTN_SO_RES14
*                            
* OUTPUTS:
*   ctrl_ins_type          - enum, requested insertion type: 
*                            - LINE_OTN_INS_TYPE_UNMODIFIED:
*                              field passes through unmodified from the upstream 
*                            - LINE_OTN_INS_TYPE_USER_VALUE: user value
*                              is inserted 
*
*   ctrl_field_value       - unsigned,user defined insertion field value
*
*   None.
*
* RETURNS:
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*   0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_oh_ins_get             ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_oh_field_t        ctrl_ins_field,
                                                  lineotn_oh_ins_type_t     *ctrl_ins_type,
                                                  UINT32                    *ctrl_field_value )
{
    PMC_ERROR           rc = PMC_SUCCESS;
    UINT8               dci_channel=0;
    otu_frm_ctl_field_t otu_frm_ins_field = (otu_frm_ctl_field_t)0;
    otu_frm_sm_oh_t     otu_frm_oh_field = (otu_frm_sm_oh_t)0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(ctrl_ins_field<LAST_LINE_OTN_OH_FIELD,LINEOTN_ERR_INVALID_PARAMETERS,ctrl_ins_field,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);
    dci_channel = h->var.lineotn_dsi_dist[dsi_start].dci_channel;

    /* set insertion field to otu type insertion fields */
    switch(ctrl_ins_field)
    {
    case LINE_OTN_SO_RES7:
        otu_frm_ins_field = OTU_SO_SM_RES7_INS_CTRL;
        otu_frm_oh_field = OTU_SO_SM_RES7;
        break;
    
    case LINE_OTN_SO_RES8:
        otu_frm_ins_field = OTU_SO_SM_RES8_INS_CTRL;
        otu_frm_oh_field = OTU_SO_SM_RES8;
        break;
    
    case LINE_OTN_SO_GCC0:
        otu_frm_ins_field = OTU_SO_GCC0_INS_CTRL;
        otu_frm_oh_field = OTU_SO_GCC0;
        break;
    
    case LINE_OTN_SO_RES13:
        otu_frm_ins_field = OTU_SO_RES13_INS_CTRL;
        otu_frm_oh_field = OTU_SO_RES13;
        break;
    
    case LINE_OTN_SO_RES14:
        otu_frm_ins_field = OTU_SO_RES14_INS_CTRL;
        otu_frm_oh_field = OTU_SO_RES14;
        break;
    
    case LINE_OTN_SO_FAS_F6:
        otu_frm_ins_field = OTU_SO_FAS_INS_CTRL;
        otu_frm_oh_field = OTU_SO_FAS_F6;
        break;
    
    case LINE_OTN_SO_FAS_28:
        otu_frm_ins_field = OTU_SO_FAS_INS_CTRL;
        otu_frm_oh_field = OTU_SO_FAS_28;
        break;
    
    default:
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
        break;
    }

    if(PMC_SUCCESS == rc )
    {    
        if(h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned)
        {
            *ctrl_ins_type = (lineotn_oh_ins_type_t) otu_frm_sm_ctl_get(h->otu_frm_handle,dci_channel,otu_frm_ins_field);
            *ctrl_field_value = otu_frm_sm_oh_get(h->otu_frm_handle,dci_channel,otu_frm_oh_field); 
        }
        else 
        {
            rc = LINEOTN_ERR_OH_INS_FAILED;
        }
    }

    PMC_RETURN(rc);
}/* lineotn_oh_ins_get */


/*******************************************************************************
* lineotn_oh_extract_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Returns the a requested OH extracted field from rx side per requested dsi
*   stream. The value returned contains the extracted SM overhead field for a
*   single OTUk frame.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
*   extract_field          - enum,requested sm extracted field: 
*                            LINE_OTN_EXT_SK_SM_TTI,
*                            LINE_OTN_EXT_SK_SM_BIP8,
*                            LINE_OTN_EXT_SK_SM_BEI_BIAE,
*                            LINE_OTN_EXT_SK_SM_BDI,
*                            LINE_OTN_EXT_SK_SM_IAE,
*                            LINE_OTN_EXT_SK_SM_RES,
*                            LINE_OTN_EXT_SK_RES,
*                            LINE_OTN_EXT_SK_GCC0
*
* OUTPUTS:
*   extracted_val_ptr      - pointer to user allocated unsigned in which
*                            the extracted value will be returned. 
*
* RETURNS:
*   LINEOTN_ERR_OH_EXT_FAILED  - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
*   1. The value returned contains the extracted SM overhead field for a
*      SINGLE OTUk frame.\n
*   2. While accessing OH register values, this register is will not be updated
*      by the device HW.\n
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_oh_extract_get         ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_oh_extract_t      extract_field,
                                                  UINT32                    *extracted_val_ptr )
{
    PMC_ERROR  rc = PMC_SUCCESS;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(extracted_val_ptr!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(extract_field<LAST_LINE_OTN_OH_EXT_FIELD,LINEOTN_ERR_INVALID_PARAMETERS,extract_field,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc && h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned)
    {
        rc = otu_frm_sm_oh_ext_get(h->otu_frm_handle,
                                   h->var.lineotn_dsi_dist[dsi_start].dci_channel,
                                   (otu_frm_sm_oh_t)extract_field,
                                   extracted_val_ptr);
    }
    else
    {
        rc = LINEOTN_ERR_OH_EXT_FAILED;
    }

    PMC_RETURN(rc);
}
/* lineotn_oh_extract_get */


/*******************************************************************************
* lineotn_force_maintenance_sig_cfg
* ______________________________________________________________________________
*
* DESCRIPTION: 
*  Forces the a requested maintenance signal insertion.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
*   maint_sig_type         - enum,requested maintenance signal:
*                            LINE_OTN_NO_MAINT_SIG,
*                            LINE_OTN_ODUK_AIS,
*                            LINE_OTN_ODUK_LCK,
*                            LINE_OTN_ODUK_OCI
*
*   maint_sig_dir          - enum,requested maintenance signal direction:
*                            LINE_OTN_RX_DIR,
*                            LINE_OTN_TX_DIR
* OUTPUTS:
*   None.
*
* RETURNS:
*   LINEOTN_ERR_SM_OH_INS_FAILED  - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_force_maintenance_sig_cfg( lineotn_handle_t          *h,
                                                    UINT32                    dsi_stream,
                                                    lineotn_dsi_type_t        dsi_type,
                                                    lineotn_maint_sig_type_t  maint_sig_type,
                                                    lineotn_dir_t             maint_sig_dir )
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8     loop_counter = 0;
    UINT8     loop_start = 0;
    UINT8     dci_channel = 0;
    UINT8     i = 0;
    UINT8     dsi_streams_count = 0;
    otu_frm_ctl_field_t ctl_field;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(maint_sig_type<LAST_LINE_OTN_MAINT_SIG,LINEOTN_ERR_INVALID_PARAMETERS,maint_sig_type,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        if(LINE_OTN_RX_DIR==maint_sig_dir)
            ctl_field = OTU_SK_MAINT_SIG ;
        else
            ctl_field = OTU_SO_MAINT_SIG;

        /* get the range of dsi stream configuration */
        lineotn_calc_dsi_range(h,dsi_start,&loop_start,&loop_counter);

        for(i=loop_start;i<loop_counter;i++)
        {
            dci_channel = h->var.lineotn_dsi_dist[i].dci_channel;

            if(h->var.lineotn_dsi_dist[i].dsi_provisioned) /* configure only provisioned streams */
            {
                dsi_streams_count++; /* count the configured dsi streams */

                rc = otu_frm_sm_ctl_set(h->otu_frm_handle,
                                        dci_channel,
                                        ctl_field,
                                        maint_sig_type );
            }

            if(rc!=PMC_SUCCESS) /* incase of error break and report */
                break;

        }
    }

    /* check how many dsi streams where configured --> at least one stream should have been configured */

    if(PMC_SUCCESS==rc && 0 == dsi_streams_count )
        rc = LINEOTN_ERR_OH_INS_FAILED;

    PMC_RETURN(rc);
}/* lineotn_force_maintenance_sig_cfg */


/*******************************************************************************
* lineotn_maintenance_sig_state_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*  inquires about the maintenance signal state for the requested dsi stream in the
*  requested direction.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
*   maint_sig_dir          - enum,requested maintenance signal direction: 
*                            LINE_OTN_RX_DIR,
*                            LINE_OTN_TX_DIR 
* OUTPUTS:
*   maint_sig_type_ptr     - pre- allocated user enum,maintenance signal set: 
*                            LINE_OTN_NO_MAINT_SIG,
*                            LINE_OTN_ODUK_AIS,
*                            LINE_OTN_ODUK_LCK,
*                            LINE_OTN_ODUK_OCI,
*                            LAST_LINE_OTN_MAINT_SIG : on error 
*
* RETURNS:
*   LINEOTN_ERR_INVALID_PARAMETERS  - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_maintenance_sig_state_get( lineotn_handle_t          *h,
                                                    UINT32                    dsi_stream,
                                                    lineotn_dsi_type_t        dsi_type,
                                                    lineotn_dir_t             maint_sig_dir,
                                                    lineotn_maint_sig_type_t  *maint_sig_type_ptr )
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8     dci_channel = 0;
    otu_frm_ctl_field_t ctl_field;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;
    UINT32                      maint_sig_val = 0;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(maint_sig_type_ptr != NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        if(LINE_OTN_RX_DIR==maint_sig_dir)
            ctl_field = OTU_SK_MAINT_SIG ;
        else
            ctl_field = OTU_SO_MAINT_SIG;

        if(h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned) /* only provisioned streams */
        {
            dci_channel = h->var.lineotn_dsi_dist[dsi_start].dci_channel;
            maint_sig_val = otu_frm_sm_ctl_get(h->otu_frm_handle,
                                               dci_channel,
                                               ctl_field );
            /*
              00: No maintenance signal is forced.
              01: Force ODUk-AIS insertion.
              10: Force ODUk-LCK insertion.
              11: Force ODUk-OCI insertion.
            */
            switch(maint_sig_val)
            {
            case 0:
                *maint_sig_type_ptr = LINE_OTN_NO_MAINT_SIG;
                break;

            case 1:
                *maint_sig_type_ptr = LINE_OTN_ODUK_AIS;
                break;

            case 2:
                *maint_sig_type_ptr = LINE_OTN_ODUK_LCK;
                break;

            case 3:
                *maint_sig_type_ptr = LINE_OTN_ODUK_OCI;
                break;

            default:
                *maint_sig_type_ptr = LAST_LINE_OTN_MAINT_SIG;
                rc = LINEOTN_ERR_INVALID_PARAMETERS;
                break;
            }
        }
    }

    PMC_RETURN(rc);
}/* lineotn_maintenance_sig_state_get */


/*******************************************************************************
* lineotn_mi_admin_state_cfg
* ______________________________________________________________________________
*
* DESCRIPTION: 
*  Forces the a requested mi admin signal LCK insertion.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
* 
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
*   admin_state_dir        - enum,requested maintenance signal direction: 
*                            LINE_OTN_RX_DIR,
*                            LINE_OTN_TX_DIR 
*
*   admin_state            - boolean,TRUE: enable the mi admin state signal;
*                                   FALSE: Disable the mi admin state signal
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   LINEOTN_ERR_SM_OH_INS_FAILED  - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
*   1. MI admin signal is masked if the maintenance signal function.
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_mi_admin_state_cfg    ( lineotn_handle_t          *h,
                                                 UINT32                    dsi_stream,
                                                 lineotn_dsi_type_t        dsi_type,
                                                 lineotn_dir_t             admin_state_dir,
                                                 lineotn_mi_admin_state_t  admin_state )
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8     loop_counter = 0;
    UINT8     loop_start = 0;
    UINT8     dci_channel = 0;
    UINT8     i = 0;
    UINT8     dsi_streams_count = 0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(admin_state_dir<LAST_LINE_OTN_DIR,LINEOTN_ERR_INVALID_PARAMETERS,admin_state_dir,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);
    PMC_ASSERT(admin_state<LAST_LINE_OTN_MI_ADMIN_STATE,LINEOTN_ERR_INVALID_PARAMETERS,admin_state,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        /* get the range of dsi stream configuration */
        lineotn_calc_dsi_range(h,dsi_start,&loop_start,&loop_counter);

        for(i=loop_start;i<loop_counter;i++)
        {
            dci_channel = h->var.lineotn_dsi_dist[i].dci_channel;

            if(h->var.lineotn_dsi_dist[i].dsi_provisioned) /* configure only provisioned streams */
            {
                dsi_streams_count++; /* count the configured dsi streams */

                if(LINE_OTN_RX_DIR==admin_state_dir)
                {
                    if(LINE_OTN_MI_ADMIN_STATE_ENABLED==admin_state)
                    {
                        /* stop SSF/AIS/SSD conseq. actions */
                        rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SK_SSF_MI_DIS,1 );
                        rc += otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SK_AIS_MI_DIS,1 );
                        rc += otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SK_SSD_MI_DIS,1 );

                        /* insert LCK signal */
                        rc += otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SK_MI_LCK,1 );
                    }
                    else /* Disable signal */
                    {
                        /* start SSF/AIS/SSD conseq. actions */
                        rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SK_SSF_MI_DIS,0 );
                        rc += otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SK_AIS_MI_DIS,0 );
                        rc += otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SK_SSD_MI_DIS,0);

                        /* remove LCK signal */
                        rc += otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SK_MI_LCK,0 );
                    }
                }
                else /* TX Dir */
                {
                    if(LINE_OTN_MI_ADMIN_STATE_ENABLED==admin_state)
                        rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SO_MI_ADMINSTATE,1 );
                    else /* Disable signal */
                        rc = otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SO_MI_ADMINSTATE,0 );
                }

            }

            if(rc!=PMC_SUCCESS) /* incase of error break and report */
                break;

        }
    }

    /* check how many dsi streams where configured --> at least one stream should have been configured */
    if(PMC_SUCCESS==rc && 0 == dsi_streams_count )
            rc = LINEOTN_ERR_OH_INS_FAILED;

    PMC_RETURN(rc);
}/* lineotn_mi_admin_state_cfg */


/*******************************************************************************
* lineotn_force_mi_active_cfg
* ______________________________________________________________________________
*
* DESCRIPTION: 
*  Forces the a requested mi active configuration.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:\n
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: \n
*                            see lineotn_stream_prov for a detailed description
*
*   mi_active_type         - enum,requested mi active configuration: 
*                            - LINE_OTN_MI_ACTIVE_DISABLED: no mi active 
*                            - LINE_OTN_MI_ACTIVE_ENABLED : mi active on OTUk TT (OTU_FRM) 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   LINEOTN_ERR_SM_OH_INS_FAILED  - Internally generated error code
*   0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_force_mi_active_cfg( lineotn_handle_t          *h,
                                              UINT32                    dsi_stream,
                                              lineotn_dsi_type_t        dsi_type,
                                              lineotn_mi_active_type_t  mi_active_type)
                                             
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8     loop_counter = 0;
    UINT8     loop_start = 0;
    UINT8     i = 0;
    UINT8     dsi_streams_count = 0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(mi_active_type<LAST_LINE_OTN_MI_ACTIVE_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,mi_active_type,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        /* get the range of dsi stream configuration */
        lineotn_calc_dsi_range(h,dsi_start,&loop_start,&loop_counter);

        for(i=loop_start;i<loop_counter;i++)
        {
            if(h->var.lineotn_dsi_dist[i].dsi_provisioned) /* configure only provisioned streams */
            {
                dsi_streams_count++; /* count the configured dsi streams */
                
                if( LINE_OTN_MI_ACTIVE_DISABLED == mi_active_type )
                {
                    if(oduk_type<UTIL_GLOBAL_ODU3) /*10G stream */
                        rc = otu_frm_mi_active_set( h->otu_frm_handle,RATE_10G,dsi_start,(otu_frm_bit_value_t)0); /* disable */
                    else if(oduk_type<UTIL_GLOBAL_ODU4) /*40G stream */
                        rc = otu_frm_mi_active_set( h->otu_frm_handle,RATE_40G,(dsi_start/4),(otu_frm_bit_value_t)0); /* disable */
                    else /* 100G */
                        rc = otu_frm_mi_active_set( h->otu_frm_handle,RATE_100G,0,(otu_frm_bit_value_t)0); /* disable */

                }
                else  /* trigger mi active on otu_frm */
                {
                    if(oduk_type<UTIL_GLOBAL_ODU3) /*10G stream */
                        rc = otu_frm_mi_active_set( h->otu_frm_handle,RATE_10G,dsi_start,(otu_frm_bit_value_t)1); /* enable */
                    else if(oduk_type<UTIL_GLOBAL_ODU4) /*40G stream */
                        rc = otu_frm_mi_active_set( h->otu_frm_handle,RATE_40G,(dsi_start/4),(otu_frm_bit_value_t)1); /* enable */
                    else /* 100G */
                        rc = otu_frm_mi_active_set( h->otu_frm_handle,RATE_100G,0,(otu_frm_bit_value_t)1); /* enable */
                }
                
            }

            if(rc!=PMC_SUCCESS) /* incase of error break and report */
                break;

        }
    }

    /* check how many dsi streams where configured --> at least one stream should have been configured */
    if(PMC_SUCCESS==rc && 0 == dsi_streams_count )
        rc = LINEOTN_ERR_OH_INS_FAILED;

    PMC_RETURN(rc);
}/* lineotn_force_mi_active_cfg */


/*******************************************************************************
* lineotn_sm_deg_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This function sets SK SM DEG fields per requested dsi stream.
*   This is a SK (Rx) only function.
*
* INPUTS:
*   h                   - pointer to lineotn handle instance.
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
*   deg_field           - enum,SK DEG fields:
*                         - LINE_OTN_OTU_SK_SM_DEG_M: the degraded defect
*                           consecutive one second monitoring
*                           intervals for the SM channel. 
*                         - LINE_OTN_OTU_SK_SM_DEG_THR: the degraded defect
*                           one second errored block count for the SM channel. 
*
*   deg_value           - UINT32, 0..32 bit values per field bits.
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_sm_deg_set ( lineotn_handle_t    *h,
                                      UINT32              dsi_stream,
                                      lineotn_dsi_type_t  dsi_type,
                                      lineotn_sk_sm_deg_t deg_field,
                                      UINT32              deg_value )
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8     loop_counter = 0;
    UINT8     loop_start = 0;
    UINT8     dci_channel = 0;
    UINT8     i = 0;
    UINT8     dsi_streams_count = 0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deg_field<LAST_LINE_OTN_SM_DEG,LINEOTN_ERR_INVALID_PARAMETERS,deg_field,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        /* get the range of dsi stream configuration */
        lineotn_calc_dsi_range(h,dsi_start,&loop_start,&loop_counter);

        for(i=loop_start;i<loop_counter;i++)
        {
            dci_channel = h->var.lineotn_dsi_dist[i].dci_channel;

            if(h->var.lineotn_dsi_dist[i].dsi_provisioned) /* configure only provisioned streams */
            {
                dsi_streams_count++; /* count the configured dsi streams */

                rc = otu_frm_sm_deg_set(h->otu_frm_handle,
                                        dci_channel,
                                        (otu_frm_sk_sm_deg_t)deg_field,
                                        deg_value );
            }

            if(rc!=PMC_SUCCESS) /* incase of error break and report */
                break;

        }
    }

    /* check how many dsi streams where configured --> at least one stream should have been configured */
    if(PMC_SUCCESS==rc && 0 == dsi_streams_count )
            rc = LINEOTN_ERR_INVALID_PARAMETERS;

    PMC_RETURN(rc);
}/* lineotn_sm_deg_set */


/*******************************************************************************
* lineotn_sm_deg_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This function gets SK SM DEG fields per requested dsi stream.
*   This is a SK (Rx) only function.
*
* INPUTS:
*   h                   - pointer to lineotn handle instance.
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
*   deg_field           - enum,SK DEG fields:
*                         - LINE_OTN_OTU_SK_SM_DEG_M: the degraded defect
*                           consecutive one second monitoring
*                           intervals for the SM channel. 
*                         - LINE_OTN_OTU_SK_SM_DEG_THR: the degraded defect
*                           one second errored block count for the SM channel. 
*
* OUTPUTS:
*   *deg_value          - pointer to pre allocated UINT32,0..32 bit values per field bits.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*   1. Invoking this function will cause a read into device registers.
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_sm_deg_get             ( lineotn_handle_t    *h,
                                                  UINT32              dsi_stream,
                                                  lineotn_dsi_type_t  dsi_type,
                                                  lineotn_sk_sm_deg_t deg_field,
                                                  UINT32              *deg_value )
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT8     dci_channel = 0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(deg_value!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(deg_field<LAST_LINE_OTN_SM_DEG,LINEOTN_ERR_INVALID_PARAMETERS,deg_field,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        if(h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned) /* read only provisioned streams */
        {
            dci_channel = h->var.lineotn_dsi_dist[dsi_start].dci_channel;
            rc = otu_frm_sm_deg_get(h->otu_frm_handle,
                                    dci_channel,
                                    (otu_frm_sk_sm_deg_t)deg_field,
                                    deg_value );
        }
        else
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }        
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* lineotn_sm_deg_get */


/*******************************************************************************
* lineotn_sm_so_tti_set
* ______________________________________________________________________________
*
* DESCRIPTION: PRELIMINARY
*   This function sets the all fields (SAPI/DAPI/OP) in the OTU_SO_SM_TTI
*   registers per the requested dsi stream (0..11). The function works in a
*   read/modify/write of all the TTI values.
*   User is expected to provide ALL of the values to be written to the registers.
*
*
* INPUTS:
*   h                      - pointer to lineotn handle instance.
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
*   ctrl_ins_type          - enum, requested insertion type: 
*                            - LINE_OTN_INS_TYPE_UNMODIFIED:
*                              field passes through unmodified from the upstream 
*                            - LINE_OTN_INS_TYPE_USER_VALUE: user value
*                              is inserted
*
*   tti_data               - 16xUINT32 unsigned array containing all
*                            tti all values (SAPI/DAPI/OP):
*                            - sapi_data is the first 16 bytes. 
*                            - dapi_data is the second 16 bytes 
*                            - op_data is the last 32 bytes. 
* OUTPUTS:
*   None.
*
* RETURNS:
*   0 - PMC_SUCCESS
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*
* NOTES:
*   1. For gracefully updating the OH field this function should be invoked
*      after a dsi stream is provisioned and before it is activated.\n
*   2. Invoking this function before a dsi stream is provisioned will result
*      in failure and an error status will be returned.\n
*   3. Invoking this function after a dsi stream is active (operational) might
*      cause a burst of alarms.\n
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_sm_so_tti_set ( lineotn_handle_t          *h,
                                         UINT32                    dsi_stream,
                                         lineotn_dsi_type_t        dsi_type,
                                         lineotn_oh_ins_type_t     ctrl_ins_type,
                                         UINT32                    tti_data[16] )
{
    PMC_ERROR           rc = PMC_SUCCESS;
    UINT8               dci_channel=0;
    UINT8               loop_counter = 0;
    UINT8               loop_start = 0;
    UINT8               i = 0;
    UINT8               dsi_streams_count = 0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;
    UINT32                      endian_fixed_tti_data[16];

    PMC_ENTRY();

    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(tti_data!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    /* Make copy of user array to modify for endian issues*/
    PMC_MEMCPY(endian_fixed_tti_data, tti_data, 16*sizeof(UINT32));

    if(PMC_SUCCESS == rc)
    {
        /* get the range of dsi stream configuration */
        lineotn_calc_dsi_range(h,dsi_start,&loop_start,&loop_counter);
        
        /* take care of endianity issues */    
        lineotn_swap_tti_bytes(h,endian_fixed_tti_data);

        for(i=loop_start;i<loop_counter;i++)
        {
            dci_channel = h->var.lineotn_dsi_dist[i].dci_channel;

            if(h->var.lineotn_dsi_dist[i].dsi_provisioned) /* configure only provisioned streams */
            {
                dsi_streams_count++; /* count the configured dsi streams */

                /* set insertion field to otu type insertion fields */
                if(LINE_OTN_INS_TYPE_USER_VALUE==ctrl_ins_type)
                {
                    rc = otu_frm_sm_tti_set(h->otu_frm_handle,(otu_frm_direction_t)LINE_OTN_TX_DIR,dci_channel,endian_fixed_tti_data); /* set user value */

                    if ( PMC_SUCCESS == rc )
                    {
                        rc =  otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SO_SM_TTI_INS_CTRL,1);
                    }

                }
                else  /* unmodified */
                {
                    rc =  otu_frm_sm_ctl_set(h->otu_frm_handle,dci_channel,OTU_SO_SM_TTI_INS_CTRL,0);
                }

            }

            if(rc!=PMC_SUCCESS) /* incase of error break and report */
                break;
        }
    }

    /* check how many dsi streams where configured --> at least one stream should have been configured */
    if(PMC_SUCCESS==rc && 0 == dsi_streams_count )
        rc = LINEOTN_ERR_INVALID_PARAMETERS;

    PMC_RETURN(rc);
}/* lineotn_sm_so_tti_set */


/*******************************************************************************
* lineotn_sm_so_tti_get
* ______________________________________________________________________________
*
* DESCRIPTION: PRELIMINARY
*   This function gets the all fields (SAPI/DAPI/OP) in the OTU_SO_SM_TTI
*   registers per the requested dsi stream (0..11). 
*
*
* INPUTS:
*   h                      - pointer to lineotn handle instance.
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
* OUTPUTS:
*   tti_data               - 16xUINT32 unsigned array containing all
*                            tti all values (SAPI/DAPI/OP):
*                            - sapi_data is the first 16 bytes. 
*                            - dapi_data is the second 16 bytes 
*                            - op_data is the last 32 bytes. 
*
* RETURNS:
*   0 - PMC_SUCCESS
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*
* NOTES:
*   1. Invoking this function before a dsi stream is provisioned will result
*      in failure and an error status will be returned.\n
*   2. Invoking this function will cause a read into device registers.\n
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_sm_so_tti_get          ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  UINT32                    tti_data[16] )
{
    PMC_ERROR           rc = PMC_SUCCESS;
    UINT8               dci_channel=0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(tti_data!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        if(h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned) /* query only provisioned streams */
        {    
            dci_channel = h->var.lineotn_dsi_dist[dsi_start].dci_channel;
            rc = otu_frm_sm_tti_get(h->otu_frm_handle,(otu_frm_direction_t)LINE_OTN_TX_DIR,dci_channel,tti_data); /* get user value */

            if(PMC_SUCCESS == rc)
                lineotn_swap_tti_bytes(h,tti_data);         /* take care of endianity issues */    

        }
        else
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* lineotn_sm_so_tti_get */


/*******************************************************************************
* lineotn_sm_exp_tti_set
* ______________________________________________________________________________
*
* DESCRIPTION: PRELIMINARY
*   This function sets the all fields (SAPI/DAPI/OP) in the OTU_SK_SM_EXP_TTI
*   registers per the requested dsi stream (0..11). The function works in a
*   read/modify/write of all the TTI values.
*   User is expected to provide ALL of the values to be written to the registers.
*
*
* INPUTS:
*   h                      - pointer to lineotn handle instance.
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
*   tti_data               - 16xUINT32 unsigned array containing all
*                            tti all values (SAPI/DAPI/OP). 
*                            - sapi_data is the first 16 bytes. 
*                            - dapi_data is the second 16 bytes 
*                            - op_data is the last 32 bytes. 
* OUTPUTS:
*   None.
*
* RETURNS:
*   0 - PMC_SUCCESS
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_sm_exp_tti_set         ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  UINT32                    tti_data[16] )
{
    PMC_ERROR           rc = PMC_SUCCESS;
    UINT8               dci_channel=0;
    UINT8               loop_counter = 0;
    UINT8               loop_start = 0;
    UINT8               i = 0;
    UINT8               dsi_streams_count = 0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    UINT32                      endian_fixed_tti_data[16];

    PMC_ENTRY();

    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(tti_data!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);
    
    /* Make copy of user array to modify for endian issues*/
    PMC_MEMCPY(endian_fixed_tti_data, tti_data, 16*sizeof(UINT32));

    if(PMC_SUCCESS == rc)
    {
        /* get the range of dsi stream configuration */
        lineotn_calc_dsi_range(h,dsi_start,&loop_start,&loop_counter);

        /* take care of endianity issues */    
        lineotn_swap_tti_bytes(h,endian_fixed_tti_data);

        for(i=loop_start;i<loop_counter;i++)
        {
            dci_channel = h->var.lineotn_dsi_dist[i].dci_channel;

            if(h->var.lineotn_dsi_dist[i].dsi_provisioned) /* configure only provisioned streams */
            {
                dsi_streams_count++; /* count the configured dsi streams */

                /* set insertion field to otu type insertion fields */
                rc = otu_frm_sm_tti_set(h->otu_frm_handle,(otu_frm_direction_t)LINE_OTN_RX_DIR,dci_channel,endian_fixed_tti_data); /* set user value */

            }

            if(rc!=PMC_SUCCESS) /* incase of error break and report */
                break;
        }
    }

    /* check how many dsi streams where configured --> at least one stream should have been configured */
    if(PMC_SUCCESS==rc && 0 == dsi_streams_count )
        rc = LINEOTN_ERR_INVALID_PARAMETERS;

    PMC_RETURN(rc);
}/* lineotn_sm_exp_tti_set */


/*******************************************************************************
* lineotn_sm_exp_tti_get
* ______________________________________________________________________________
*
* DESCRIPTION: PRELIMINARY
*   This function gets the all fields (SAPI/DAPI/OP) in the OTU_SK_SM_EXP_TTI
*   registers per the requested dsi stream (0..11). 
*
*
* INPUTS:
*   h                      - pointer to lineotn handle instance.
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
* OUTPUTS:
*   tti_data               - 16xUINT32 unsigned array containing all
*                            tti all values (SAPI/DAPI/OP). 
*                            - sapi_data is the first 16 bytes. 
*                            - dapi_data is the second 16 bytes 
*                            - op_data is the last 32 bytes. 
*
* RETURNS:
*   0 - PMC_SUCCESS
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*
* NOTES:
*   1. Invoking this function before a dsi stream is provisioned will result
*      in failure and an error status will be returned.\n
*   2. Invoking this function will cause a read into device registers.\n
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_sm_exp_tti_get         ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  UINT32                    tti_data[16] )
{
    PMC_ERROR           rc = PMC_SUCCESS;
    UINT8               dci_channel=0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(tti_data!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        if(h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned) /* query only provisioned streams */
        {    
            dci_channel = h->var.lineotn_dsi_dist[dsi_start].dci_channel;

            if(h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned) /* query only provisioned streams */
                rc = otu_frm_sm_tti_get(h->otu_frm_handle,(otu_frm_direction_t)LINE_OTN_RX_DIR,dci_channel,tti_data); /* get user value */

            if(PMC_SUCCESS == rc)
                lineotn_swap_tti_bytes(h,tti_data);        /* take care of endianity issues */    
        }
        else
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* lineotn_sm_exp_tti_get */


/*******************************************************************************
* lineotn_exp_tti_tim_det_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION: PRELIMINARY
*   This function sets OTU_SK_SM_EXP_TTI dTIM detection mode registers per the 
*   requested dsi stream  (0..11). 
*
* INPUTS:
*   h                      - pointer to lineotn handle instance.
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
*   exp_tti_tim_det_mode   - enum, dTIM detection mode:
*    - OTU_SK_SM_DTIM_DET_DISABLED    : dTIM defect detection is disabled 
*    - OTU_SK_SM_DTIM_DET_SAPI        : The expected SAPI field is used when determining the dTIM defect 
*    - OTU_SK_SM_DTIM_DET_DAPI        : The expected DAPI field is used when determining the dTIM defect 
*    - OTU_SK_SM_DTIM_DET_BOTH        : Both the expected SAPI field and the expected DAPI field are 
*                                     used when determining the dTIM defect 
* OUTPUTS:
*   None
*
* RETURNS:
*       0 - PMC_SUCCESS or meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_exp_tti_tim_det_mode_set( lineotn_handle_t          *h,
                                                   UINT32                    dsi_stream,
                                                   lineotn_dsi_type_t        dsi_type,
                                                   lineotn_sk_tim_det_mode_t exp_tti_tim_det_mode )
{
    PMC_ERROR           rc = PMC_SUCCESS;
    UINT8               dci_channel=0;
    UINT8               loop_counter = 0;
    UINT8               loop_start = 0;
    UINT8               i = 0;
    UINT8               dsi_streams_count = 0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);
    PMC_ASSERT(exp_tti_tim_det_mode < LAST_LINE_OTN_SK_SM_DTIM_DET  ,LINEOTN_ERR_INVALID_PARAMETERS,exp_tti_tim_det_mode,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        /* get the range of dsi stream configuration */
        lineotn_calc_dsi_range(h,dsi_start,&loop_start,&loop_counter);

        for(i=loop_start;i<loop_counter;i++)
        {
            dci_channel = h->var.lineotn_dsi_dist[i].dci_channel;

            if(h->var.lineotn_dsi_dist[i].dsi_provisioned) /* configure only provisioned streams */
            {
                dsi_streams_count++; /* count the configured dsi streams */
                /* set insertion field to otu type insertion fields */
                otu_frm_exp_tti_tim_det_mode_set(h->otu_frm_handle,dci_channel,(otu_frm_sk_tim_det_mode_t)exp_tti_tim_det_mode); /* set user value */

            }
        }
    }

    /* check how many dsi streams where configured --> at least one stream should have been configured */
    if(PMC_SUCCESS==rc && 0 == dsi_streams_count )
        rc = LINEOTN_ERR_INVALID_PARAMETERS;

    PMC_RETURN(rc);
} /* lineotn_exp_tti_tim_det_mode_set */


/*******************************************************************************
* lineotn_exp_tti_tim_det_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION: PRELIMINARY
*   This function gets OTU_SK_SM_EXP_TTI dTIM detection mode 
*   registers per the requested dsi stream  (0..11). 
*
* INPUTS:
*   h                      - pointer to lineotn handle instance.
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
* OUTPUTS:
*   exp_tti_tim_det_mode   - pointer to pre allocated enum, dTIM detection mode:
*    - OTU_SK_SM_DTIM_DET_DISABLED    : dTIM defect detection is disabled 
*    - OTU_SK_SM_DTIM_DET_SAPI        : The expected SAPI field is used when determining the dTIM defect 
*    - OTU_SK_SM_DTIM_DET_DAPI        : The expected DAPI field is used when determining the dTIM defect 
*    - OTU_SK_SM_DTIM_DET_BOTH        : Both the expected SAPI field and the expected DAPI field are 
*                                      used when determining the dTIM defect 
* RETURNS:
*       0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_exp_tti_tim_det_mode_get( lineotn_handle_t          *h,
                                                   UINT32                    dsi_stream,
                                                   lineotn_dsi_type_t        dsi_type,
                                                   lineotn_sk_tim_det_mode_t *exp_tti_tim_det_mode )
{
    PMC_ERROR           rc = PMC_SUCCESS;
    UINT8               dci_channel=0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);
    PMC_ASSERT(exp_tti_tim_det_mode!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
  
    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        if(h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned) /* query only provisioned streams */
        {    
            dci_channel = h->var.lineotn_dsi_dist[dsi_start].dci_channel;

            if(h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned) /* query only provisioned streams */
                otu_frm_exp_tti_tim_det_mode_get(h->otu_frm_handle,dci_channel,(otu_frm_sk_tim_det_mode_t*)exp_tti_tim_det_mode); /* get user value */
        }
        else
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);

} /* lineotn_exp_tti_tim_det_mode_get */


/*******************************************************************************
* lineotn_ebc_get
* ______________________________________________________________________________
*
* DESCRIPTION: PRELIMINARY
*   retrieve LINEOTN otu framer far/near end counters. 
*
* INPUTS:
*   h                      - pointer to lineotn handle instance.
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
* OUTPUTS:
*   *ebc   - pointer to LINEOTN otu framer far/near end counters\n
*
* RETURNS:
*       0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_ebc_get( lineotn_handle_t          *h,
                                  UINT32                    dsi_stream,
                                  lineotn_dsi_type_t        dsi_type,
                                  lineotn_otu_frm_counters_t *ebc )
{
    PMC_ERROR           rc = PMC_SUCCESS;
    UINT8               dci_channel=0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);
    PMC_ASSERT(ebc!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
  
    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        if(h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned) /* query only provisioned streams */
        {    
            dci_channel = h->var.lineotn_dsi_dist[dsi_start].dci_channel;

            if(h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned) /* query only provisioned streams */
                otu_frm_ebc_get(h->otu_frm_handle,dci_channel,(otu_frm_counters_t *)ebc); /* get user value */
        }
        else
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS;
        }
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);

} /* lineotn_ebc_get */


/*******************************************************************************
* lineotn_sm_accepted_tti_get
* ______________________________________________________________________________
*
* DESCRIPTION: PRELIMINARY
*   This function reads the accepted TTI register per requested dsi stream (0..11)
*   into a user buffer. This is a SK (Rx) only function.
*
*
* INPUTS:
*   h                      - pointer to lineotn handle instance.
*
*   dsi_stream             - unsigned, dsi stream number to configure:\n
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: \n
*                            see lineotn_stream_prov for a detailed description
*
* OUTPUTS:
*   tti_data_buf           - pointer to 16xUINT32 array containing user
*                            buffer to which the tti values will be copied to.
*
* RETURNS:
*       0 - PMC_SUCCESS
*       LINEOTN_ERR_OH_EXT_FAILED  - Internally generated error code
* NOTES:
*   1. While accessing accepted tti register values, this register is will not be updated
*      by the device HW.
* 
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_sm_accepted_tti_get    ( lineotn_handle_t   *h,
                                                  UINT32             dsi_stream,
                                                  lineotn_dsi_type_t dsi_type,
                                                  UINT32             tti_data_buf[16] )
{
    PMC_ERROR           rc = PMC_SUCCESS;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(tti_data_buf!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc && h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned)
    {
        otu_frm_sm_accepted_tti_get(h->otu_frm_handle,h->var.lineotn_dsi_dist[dsi_start].dci_channel, tti_data_buf);       
        lineotn_swap_tti_bytes(h,tti_data_buf);        /* take care of endianity issues */    
    }
    else
    {
        rc = LINEOTN_ERR_OH_EXT_FAILED;
    }

    PMC_RETURN(rc);
}/* lineotn_sm_accepted_tti_get */


/*******************************************************************************
* lineotn_loopback_cfg
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This function configures the opsm loopback for an already provisioned dsi stream.
*
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance \n
*
*   dsi_stream             - unsigned, dsi stream number to configure:\n
*                            see lineotn_stream_prov for a detailed description
*
*   loopback_dir           - enum, loopback direction: 
*                            LINE_OTN_RX_DIR;
*                            LINE_OTN_TX_DIR
*
*   loopback_state         - enum, requested loopback state:
*                            LINE_OTN_LOOPBACK_DISABLED  : loopback disabled;
*                            LINE_OTN_LOOPBACK_ENABLED   : loopback enabled
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_loopback_cfg          ( lineotn_handle_t           *h,
                                                 UINT32                     dsi_stream,
                                                 lineotn_dir_t              loopback_dir,
                                                 lineotn_loopback_state_t   loopback_state )
{
    PMC_ERROR   rc = PMC_SUCCESS;
    BOOL loopback_enable = FALSE;
    lineotn_frame_rate_t frame_rate;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(loopback_dir< LAST_LINE_OTN_DIR,LINEOTN_ERR_INVALID_PARAMETERS,loopback_dir,0);
    PMC_ASSERT(loopback_state< LAST_LINE_OTN_LOOPBACK_STATE,LINEOTN_ERR_INVALID_PARAMETERS,loopback_state,0);
    PMC_ASSERT(h->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned!=FALSE,LINEOTN_ERR_INVALID_PARAMETERS,h->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned,0);

    frame_rate = h->var.lineotn_dsi_dist[dsi_stream].dsi_rate;
    PMC_ASSERT(frame_rate<LAST_LINE_OTN_FRAME_RATE,LINEOTN_ERR_INVALID_PARAMETERS,frame_rate,0);

    loopback_enable = (loopback_state == LINE_OTN_LOOPBACK_ENABLED?TRUE:FALSE);

    if (4 == lineotn_frame_rate_otuk_num_get(frame_rate) )
    {
        rc = mld_opsm_loopback_set(h, MLD_OPSM4,dsi_stream,frame_rate,loopback_dir,loopback_enable);
        if (loopback_dir == LINE_OTN_TX_DIR) 
        {
            if (loopback_state == LINE_OTN_LOOPBACK_ENABLED && rc == PMC_SUCCESS)
            {
                otu_frm_ssf_assertion_on_intlv_fifo_empty_set(h->otu_frm_handle, FALSE);
            } 
            else 
            {
                otu_frm_ssf_assertion_on_intlv_fifo_empty_set(h->otu_frm_handle, TRUE);
            }
        }
    }
    else if (3 == lineotn_frame_rate_otuk_num_get(frame_rate) ||
             2 == lineotn_frame_rate_otuk_num_get(frame_rate))
    {
    
        if( dsi_stream < 4 )
            rc = mld_opsm_loopback_set(h, MLD_OPSM23_0,dsi_stream,frame_rate,loopback_dir,loopback_enable);
        else if( dsi_stream >= 4 &&  dsi_stream < 8 )
            rc = mld_opsm_loopback_set(h, MLD_OPSM23_1,dsi_stream,frame_rate,loopback_dir,loopback_enable);
        else
            rc = mld_opsm_loopback_set(h, MLD_OPSM23_2,dsi_stream,frame_rate,loopback_dir,loopback_enable);

        if (loopback_dir == LINE_OTN_TX_DIR && rc == PMC_SUCCESS) 
        {
            if (loopback_state == LINE_OTN_LOOPBACK_ENABLED )
            {
                otu_frm_ssf_assertion_on_intlv_fifo_empty_set(h->otu_frm_handle, FALSE);
            } 
            else 
            {
                BOOL temp_en;
                UINT8 i;                
                /* Only re-enable the SSF assertion if all channels do not use the MLD Diag loopback */                    
                for (i = 0; i < h->cfg.max_dsi_streams; i += 1)
                {  
                    if( i < 4 )
                        rc = mld_opsm_loopback_get(h, MLD_OPSM23_0,dsi_stream,frame_rate,loopback_dir,&temp_en);
                    else if( i >= 4 && i < 8 )
                        rc = mld_opsm_loopback_get(h, MLD_OPSM23_1,dsi_stream,frame_rate,loopback_dir,&temp_en);
                    else
                        rc = mld_opsm_loopback_get(h, MLD_OPSM23_2,dsi_stream,frame_rate,loopback_dir,&temp_en);
                    
                    if (TRUE == temp_en || rc != PMC_SUCCESS)
                    {
                        break;
                    }
                }
                if (i == h->cfg.max_dsi_streams && rc == PMC_SUCCESS)
                {
                    /* No channels have a loopback enabled*/
                    otu_frm_ssf_assertion_on_intlv_fifo_empty_set(h->otu_frm_handle, TRUE);
                }                
            } /* if (loopback_state == LINE_OTN_LOOPBACK_ENABLED )*/
        } /* if (loopback_dir == LINE_OTN_TX_DIR && rc == PMC_SUCCESS) */
    }
    else /* Invalid frame_rate */
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;     
    }
    
    PMC_RETURN(rc);
}/* lineotn_loopback_cfg */


/*******************************************************************************
* lineotn_sfi51_lane_swap_cfg
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This function configures the sfi 5.1 lane swap property.
*
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance 
*
*   sfi51_ifc              - enum, specify sfi 5.1 interface A or B
*                            - LINE_OTN_DSI_TYPE_SFI_51_A 
*                            - LINE_OTN_DSI_TYPE_SFI_51_B 
*
*   swap_dir               - enum, swap direction: 
*                            LINE_OTN_RX_DIR;
*                            LINE_OTN_TX_DIR
*
*   lanes_swap_state       - enum, requested loopback state:
*                            LINE_OTN_SFI51_LANES_SWAP_DISABLED;
*                            LINE_OTN_SFI51_LANES_SWAP_ENABLED  
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_sfi51_lane_swap_cfg          ( lineotn_handle_t           *h,
                                                        lineotn_dsi_type_t         sfi51_ifc, 
                                                        lineotn_dir_t              swap_dir,
                                                        lineotn_sfi51_swap_state_t lanes_swap_state )
{
    PMC_ERROR   rc = PMC_SUCCESS;
    
    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(sfi51_ifc==LINE_OTN_DSI_TYPE_SFI_51_A || sfi51_ifc==LINE_OTN_DSI_TYPE_SFI_51_B  ,LINEOTN_ERR_INVALID_PARAMETERS,sfi51_ifc,0);
    PMC_ASSERT(swap_dir< LAST_LINE_OTN_DIR,LINEOTN_ERR_INVALID_PARAMETERS,swap_dir,0);
    PMC_ASSERT(lanes_swap_state< LAST_LINE_OTN_SFI51_LANES_SWAP_STATE,LINEOTN_ERR_INVALID_PARAMETERS,lanes_swap_state,0);


    switch (swap_dir)
    {
    case LINE_OTN_RX_DIR:
        sfi51_field_RX_LANE_SWAP_set (NULL,h,sfi51_ifc,lanes_swap_state);
        break;

    case LINE_OTN_TX_DIR:
        sfi51_field_TX_LANE_SWAP_set (NULL,h,sfi51_ifc,lanes_swap_state);
        break;
    default:
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
        break;
    }
    
    PMC_RETURN(rc);
}/* lineotn_sfi51_lane_swap_cfg */


/*******************************************************************************
* lineotn_fec_type_get
* ______________________________________________________________________________
*
* DESCRIPTION: PRELIMINARY
*   This function reads fec type per requested dsi stream (0..11)
*   into a user buffer. 
*
*
* INPUTS:
*   h                      - pointer to lineotn handle instance.
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_stream_prov for a detailed description
*
* OUTPUTS:
*   *fec_type              - pointer to pre allocated enum, fec type:
*                            - LINE_OTN_FEC_NONE          : No FEC configured
*                            - LINE_OTN_FEC_I4            : FEC type I4
*                            - LINE_OTN_FEC_I7            : FEC type I7
*                            - LINE_OTN_FEC_G709          : FEC type RSFEC (G7.09)
*                            - LINE_OTN_FEC_SWIZZLE_100G  : FEC type Swizzle 100G 
*                            - LINE_OTN_FEC_SWIZZLE_40G   : FEC type Swizzle 40G 
*                            - etc.
*
* RETURNS:
*       0 - PMC_SUCCESS
*       LINEOTN_ERR_INVALID_PARAMETERS  - Internally generated error code
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_fec_type_get    ( lineotn_handle_t   *h,
                                           UINT32             dsi_stream,
                                           lineotn_dsi_type_t dsi_type,
                                           lineotn_fec_t      *fec_type )
{
    PMC_ERROR                   rc = PMC_SUCCESS;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(fec_type!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc && h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned)
    {
        *fec_type = h->var.lineotn_dsi_dist[dsi_start].dsi_fec_type;
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* lineotn_fec_type_get */


/*******************************************************************************
* lineotn_fec_dec_passthru_set
* ______________________________________________________________________________
*
* DESCRIPTION: PRELIMINARY
*   This function will set the decoder state for a selected fec per dsi stream.
*   The function will change the decoder state WITHIN a specified fec block.
*   Setting the decoder state to disabled will cause traffic to go through the 
*   decoder keeping the fec latency, however, no error corrections or statistics
*   collection will be performed.
*   This function is required for implementation of MI_FECEn of G.798 .
*
* INPUTS:
*   h                      - pointer to lineotn handle instance.
*
*   dsi_stream             - unsigned, dsi stream number to configure:\n
*                            see lineotn_fec_cfg for a detailed description
* 
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_fec_cfg for a detailed description
* 
*   fec_type               - enum, fec type (only fec on this list are supported):\n
*                            - LINE_OTN_FEC_I4            : FEC type I4
*                            - LINE_OTN_FEC_I7            : FEC type I7
*                            - LINE_OTN_FEC_G709          : FEC type RSFEC (G7.09)
*                            - LINE_OTN_FEC_SWIZZLE_40G   : FEC type Swizzle 40G 
*                            - etc.
*
*   dec_passthru           - boolean, TRUE  :fec decoder is passed through;
*                                     FALSE :fec decoder is correcting errors
* OUTPUTS:
*   None
* 
* RETURNS:
*       0 - PMC_SUCCESS
*       LINEOTN_ERR_INVALID_PARAMETERS  - Internally generated error code
* 
* NOTES:
*   
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_fec_dec_passthru_set    ( lineotn_handle_t   *h,
                                                   UINT32             dsi_stream,
                                                   lineotn_dsi_type_t dsi_type,
                                                   lineotn_fec_t      fec_type,
                                                   BOOL               dec_passthru )
{
    PMC_ERROR                   rc = PMC_SUCCESS;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(fec_type<LAST_LINE_OTN_FEC,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc && h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned &&
       h->var.lineotn_dsi_dist[dsi_start].dsi_fec_type == fec_type)
    {
        rc = fec_dec_passthru_set(h,dsi_start,fec_type,dec_passthru);
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* lineotn_fec_dec_passthru_set */


/*******************************************************************************
* lineotn_fec_dec_passthru_get
* ______________________________________________________________________________
*
* DESCRIPTION: PRELIMINARY
*   This function returns a TRUE boolean if fec decoder mode is in pass through mode 
*   per requested dsi stream (0..11).
*
*
* INPUTS:
*   h                      - pointer to lineotn handle instance.
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_fec_cfg for a detailed description
* 
*   dsi_type               - enumerated lineotn dsi stream type: 
*                            see lineotn_fec_cfg for a detailed description
* 
* OUTPUTS:
*   *dec_in_passthru_mode  - pointer to pre allocated boolean, fec dec pass thru mode:
*                            TRUE           : fec decoder is in pass thru mode;
*                            FALSE          : fec decoder is not in pass thru mode
* RETURNS:
*       0 - PMC_SUCCESS
*       LINEOTN_ERR_INVALID_PARAMETERS  - Internally generated error code
* 
* NOTES:
* 
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_fec_dec_passthru_get    ( lineotn_handle_t   *h,
                                                   UINT32             dsi_stream,
                                                   lineotn_dsi_type_t dsi_type,
                                                   BOOL               *dec_in_passthru_mode )
{
    PMC_ERROR           rc = PMC_SUCCESS;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg = h->var.current_lineotn_cfg;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dec_in_passthru_mode!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);


    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc && h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned)
    {
        *dec_in_passthru_mode = h->var.lineotn_dsi_dist[dsi_start].fec_dec_passthru;
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* lineotn_fec_dec_passthru_get */


/*******************************************************************************
* lineotn_bergen_cfg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables/disables a bergen block and its bit error insertion 
*   per user requested error rate and error mode for a provisioned dsi stream.
*
* INPUTS:
*   h                       - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             - for 10G configurations (0..11) 
*                             - for 40G configurations (0,4,8)
*                             - for 100G (0) 
*
*   logical_lanes_msk  - it is the logical lane mask associated to 
*                        the aggregated port. Use 0xF and 0x3FF to respectively 
*                        enable all lanes for OTU3 and OTU4 stream. This field
*                        is ignored for an OTU2 streams.

*   error_rate - The bit error density. You can specify a value between 0 and
*                1.0. This field has a precision of 24 bits.
*
*
*   ringosc_enable   - Enables or disables the bergen Ring Oscillator. The 32
*                      bit Bit Error Rate Generator (BERGEN32) blocks have the
*                      ability to use clocks created by free running ring 
*                      oscillators to create asynchronous events that better 
*                      randomize the distribution of bit errors inserted in 
*                      the transmit line data.
*
*   bergen_mode             - enum, error insertion mode:
*                             - LINE_OTN_BERGEN_NORMAL : Inverts the input bit in order to 
*                                                      insert an erroneous bit.
*                             - LINE_OTN_BERGEN_0_INSERTION : Inverts the input bit only 
*                                                           if the input bit is 1 
*                                                           in order to insert an erroneous 0.
*                             - LINE_OTN_BERGEN_1_INSERTION : Inverts the input bit only 
*                                                           if the input bit is 0 
*                                                           in order to insert an erroneous 1.
*
*   bergen_enable           - boolean, TRUE: enable the bergen;
*                                      FALSE: disable bergen 
* OUTPUTS:
*   None.
*
* RETURNS:
*       LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*       0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_bergen_cfg_set(lineotn_handle_t            *h,
                                        UINT8                       dsi_stream,
                                        UINT32                      logical_lanes_msk,
                                        FLOAT                       error_rate,
                                        BOOL8                       ringosc_enable,
                                        lineotn_bergen_err_mode     bergen_mode,
                                        BOOL8                       bergen_enable)
{

    UINT8 start_dsi = 0;
    UINT8 stop_dsi  = 0;
    UINT8 i         = 0;
    UINT32 validity_msk = 0;
    PMC_ERROR rc = PMC_SUCCESS;
    lineotn_frame_rate_t frame_rate;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);    
    PMC_ASSERT(bergen_mode < LAST_LINE_OTN_BERGEN_ERROR_MODE,LINEOTN_ERR_INVALID_PARAMETERS,bergen_mode,0);

    if (error_rate < 0.0 || error_rate > 1.0)
    {
        PMC_RETURN(LINEOTN_ERR_INVALID_PARAMETERS);
    }

    frame_rate = h->var.lineotn_dsi_dist[dsi_stream].dsi_rate;

    if(h->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned)
    {
        if (4 == lineotn_frame_rate_otuk_num_get(frame_rate) )
        {
            start_dsi= 0; 
            /* check logical mask*/
            if (MLD_SFIS == h->var.lineotn_dsi_dist[dsi_stream].dsi_hw_ifc)
            {
                validity_msk = 0xFFFFF800;
                stop_dsi = 11; 
            } 
            else 
            {
                validity_msk = 0xFFFFFC00;
                stop_dsi = 10; 
            }            
        }
        else if (3 == lineotn_frame_rate_otuk_num_get(frame_rate) )
        {
            /* check logical mask*/
            if (MLD_SFIS == h->var.lineotn_dsi_dist[dsi_stream].dsi_hw_ifc)
            {
                validity_msk = 0xFFFFFFE0;
                start_dsi=(dsi_stream/5)*5; /* normalize to 0,5*/
                stop_dsi = start_dsi + 5; 
            
            } 
            else {       
                validity_msk = 0xFFFFFFF0;
                start_dsi=(dsi_stream/4)*4; /* normalize to 0,4,8*/
                stop_dsi = start_dsi + 4; 
            }
            /* The lane mask is 0xf (or 0x1f for MLD_SFIS)- shift to match the dsi */
            logical_lanes_msk <<= start_dsi;
            validity_msk <<= start_dsi;
        }
        else if (2 == lineotn_frame_rate_otuk_num_get(frame_rate) )
        {   
            validity_msk = 0;
            start_dsi=dsi_stream;
            stop_dsi =dsi_stream + 1; 
            /* The logical_lanes_msk parameter is a don't care for 10G, 
               but the code below won't work unless it's set. */
            logical_lanes_msk = (1 << dsi_stream);                
        }
        else
        {   
            rc = LINEOTN_ERR_INVALID_PARAMETERS;                
        }

        /* check logical mask */
        validity_msk = logical_lanes_msk & validity_msk;
        if (0 != validity_msk)
        {
            rc = LINEOTN_ERR_INVALID_PARAMETERS; 
        }

        if(PMC_SUCCESS == rc)
        {
            for( i=start_dsi; i < stop_dsi && rc == PMC_SUCCESS; i++)
            {
                if (((logical_lanes_msk >> i) & 0x1) == 1) 
                {
                    rc = mld_bergen_cfg_set(h,i,error_rate,ringosc_enable,bergen_mode,bergen_enable);
                }
            }
        }
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* lineotn_bergen_cfg_set */


/*******************************************************************************
* lineotn_bergen_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get Configuration on Bit Error Generator on 32-bit interfaces (BERGEN32)
*
*   This function should be used a specified lane. 
*
* INPUTS:
*   h                       - pointer to lineotn handle instance.
*
*   dsi_stream              - unsigned, dsi stream to configure:
*                             - for 10G configurations (0..11) 
*                             - for 40G configurations (0,4,8) 
*                             - for 100G (0) 
*
*   logical_lane_idx       - it is the logical lane index associated to 
*                            an aggregated port.This field is ignored for an
*                            OTU2 streams.
*
*
* OUTPUTS:
*   *error_rate - The bit error density. You can specify a value between 0 and
*                1.0. This field has a precision of 24 bits. 
*                Only valid when enable is TRUE
*
*   *ringosc_enable   - Enables or disables the bergen Ring Oscillator. The 32
*                      bit Bit Error Rate Generator (BERGEN32) blocks have the
*                      ability to use clocks created by free running ring 
*                      oscillators to create asynchronous events that better 
*                      randomize the distribution of bit errors inserted in 
*                      the transmit line data. *Only valid when enable is TRUE
*
*   *bergen_mode   - enum, error insertion mode:
*                    - LINE_OTN_BERGEN_NORMAL : Inverts the input bit in order to 
*                                               insert an erroneous bit.
*                   - LINE_OTN_BERGEN_0_INSERTION : Inverts the input bit only 
*                                                   if the input bit is 1 
*                                                   in order to insert an erroneous 0.
*                   - LINE_OTN_BERGEN_1_INSERTION : Inverts the input bit only 
*                                                   if the input bit is 0 
*                                                   in order to insert an erroneous 1.
*                    *Only valid when enable is TRUE
* 
*   *bergen_enable         - boolean, TRUE  --BERGEN is enabled;
*                                     FALSE -- BERGEN is disabled
*
* RETURNS:
*   0 - PMC_SUCCESS
*   LINEOTN_ERR_INVALID_PARAMETERS - Internally generated error code
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_bergen_cfg_get(  lineotn_handle_t          *h,
                                          UINT8                     dsi_stream,
                                          UINT32                    logical_lane_idx,
                                          FLOAT                     *error_rate,
                                          BOOL8                     *ringosc_enable,
                                          lineotn_bergen_err_mode   *bergen_mode,
                                          BOOL8                     *bergen_enable)
{
    UINT8 start_dsi = 0;
    UINT8 stop_dsi  = 0;
    lineotn_frame_rate_t frame_rate;
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream < 12,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);    
        
    frame_rate = h->var.lineotn_dsi_dist[dsi_stream].dsi_rate;
            
    if(h->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned)
    {
        /* set number of lanes by rate */
        if (4 == lineotn_frame_rate_otuk_num_get(frame_rate) )
        {
            start_dsi= 0;        
            /* check logical mask*/
            if (MLD_SFIS == h->var.lineotn_dsi_dist[dsi_stream].dsi_hw_ifc)
            {
                stop_dsi = 11; 
            } else {             
                stop_dsi = 10; 
            }            
        }
        else if (3 == lineotn_frame_rate_otuk_num_get(frame_rate) )
        {
            /* check logical mask*/
            if (MLD_SFIS == h->var.lineotn_dsi_dist[dsi_stream].dsi_hw_ifc)
            {
                start_dsi=(dsi_stream/5)*5; /* normalize to 0,5*/
                stop_dsi = start_dsi + 5; 

            } else {
                start_dsi=(dsi_stream/4)*4; /* normalize to 0,4,8*/
                stop_dsi = start_dsi + 4; 
            }
        }
        else if (2 == lineotn_frame_rate_otuk_num_get(frame_rate) )
        {
            start_dsi=dsi_stream;
            stop_dsi =dsi_stream + 1; 
        }
        else
        {   
            rc = LINEOTN_ERR_INVALID_PARAMETERS;                
        }

        if (PMC_SUCCESS == rc ) 
        {
            /* check logical lane idx */
            if (start_dsi + logical_lane_idx >= stop_dsi)
            {
                rc = LINEOTN_ERR_INVALID_PARAMETERS; 
            }
        }
        if(PMC_SUCCESS == rc)
        {
            mld_bergen_cfg_get(h,
                               start_dsi + logical_lane_idx,
                               error_rate,ringosc_enable,bergen_mode,bergen_enable);
        }
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
   
} /* lineotn_bergen_cfg_get */


/*******************************************************************************
* lineotn_fec_stat_intf_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*  
*  This function initializes 4-wire OTN FEC statistics interface. This function 
*  shall be called before enabling collection of FEC statistics on a specific 
*  stream.  lineotn_fec_stat_set can be used to enable statistic 
*  collection and lineotn_fec_stat_intf_uninit is provided to disable the
*  interface.
*
* INPUTS:
*   *h                     - pointer to LINEOTN handle instance
* 
*   *fec_stat_intf_cfg_ptr - pointer to FEC stat interface configuration
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_fec_stat_intf_init(lineotn_handle_t      *h,
                                            digi_fec_stat_intf_cfg_t *fec_stat_intf_cfg_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();
    
    /* pass it the fec layer */
    rc = fec_stat_intf_init(h, fec_stat_intf_cfg_ptr);

    PMC_RETURN(rc);
} /*lineotn_fec_stat_intf_init*/


/*******************************************************************************
* lineotn_fec_stat_intf_uninit
* ______________________________________________________________________________
*
* DESCRIPTION: 
*
*   The function disables the 4-wire OTN FEC statistics interface.
*
* INPUTS:
*   *h          - pointer to LINEOTN handle instance
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS           - when API execution is successful otherwise a 
*                           descriptive error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_fec_stat_intf_uninit(lineotn_handle_t      *h)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* pass it the fec layer */
    rc = fec_stat_intf_uninit(h);

    PMC_RETURN(rc);
} /*lineotn_fec_stat_intf_uninit*/


/*******************************************************************************
* lineotn_fec_stat_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables/disables a statistic collection on the 4-wire OTN FEC
*   statistics interface for a specified stream. This information sent on 
*   this interface is intended to be used an off-chip CPLD/FPGA for calibration
*   purpose.\n
*   This function validates that the specified stream has the proper 
*   characteristic. In order to setup FEC statistic, it is required to resolve
*   the mapping  between logical lanes and physical lanes. To achieve it, 
*   status values are read  in proper lane aggregators. Therefore, to successfully
*   enable  statistic collection, all RX lanes shall not be in LANE Loss of 
*   Frame (LOF)\n
*   This functionality is not supported for stream provisioned:
*      - With RSFEC (G.709) or when FEC is bypassed
*      - On SFIS and SFI5.1 MLD interface 
*
* INPUTS:
*   *h              - pointer to LINEOTN handle instance
* 
*   dsi_stream      - unsigned, dsi stream number to configure:
*                            - for 10G configurations valid values are: (0..11) 
*                            - for OTL3.4 40G configurations valid values are: (0,4,8)
*                            - for 40G SFIS configurations valid values are: (0,5) 
*                            - for 40G SFI5.1 configurations valid values are: (0,17) 
*                            - for 100G configurations valid values are: (0)
* 
*   enable           - boolean, TRUE: enable statistic collection;
*                               FALSE: disable enable statistic collection 
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - when API execution is successful otherwise a 
*                 descriptive error is returned.
*
* NOTES:
*   If not all RX lanes are defect free (absence of LOF), then a fixed 1:1 logical-lane
*   to physical lane relationship is established.  If the user desires to re-program
*   the logical-lane to physical lane relationship, they must first disable FEC statistic
*   collection and then re-enable it.
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_fec_stat_set(lineotn_handle_t *h,
                                      UINT32 dsi_stream,
                                      BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 mld_opsms_defects = 0;
    UINT32 logical_lane_id[LINEOTN_MLD_OTL410_NUM_LOGICAL_LANE];
    UINT32 i = 0;

    PMC_ENTRY();
    
    if (FALSE == fec_stat_enabled(h))
    {
        rc = LINEOTN_ERR_FEC_STAT_NOT_INIT;
    }
    
    if (PMC_SUCCESS == rc)
    {
        rc = fec_stat_validate_stream(h,dsi_stream);
    }
    
    if (PMC_SUCCESS == rc)
    {
        if (TRUE == enable)
        {
            /* get MLD LANE dLOF status */
            mld_opsms_defects = mld_opsm_defect_value_get(h,dsi_stream,LINE_OTN_D_LOFLANE);
#ifdef PMC_SW_SIMULATION
            /* we assume that lanes are all in sync */
            mld_opsms_defects = 0 ;
#endif
            if (0 != mld_opsms_defects)
            {
                for (i=0; i<LINEOTN_MLD_OTL410_NUM_LOGICAL_LANE; i++)
                {
                     logical_lane_id[i] = i;
                }
            }
            
            /* get logical identifiers */            
            mld_otl_logical_lane_id_get(h,dsi_stream,logical_lane_id,TRUE);            
        }        
        /* try to configure logical lane entries for this dsi_stream */
        rc = fec_stat_set(h,dsi_stream, logical_lane_id,enable);
        
    }

    PMC_RETURN(rc);
} /* lineotn_fec_stat_set */


/*******************************************************************************
* lineotn_alloc_int
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Allocate any local memory
*
* INPUTS:
*   *lineotn_handle          - pointer to lineotn handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void lineotn_alloc_init(lineotn_handle_t     *lineotn_handle)
{
    PMC_ENTRY();

    otu_frm_alloc_init(lineotn_handle->otu_frm_handle);

    PMC_RETURN();
}/*lineotn_alloc_init*/


/*******************************************************************************
* lineotn_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on lineotn ss handle instance.
*
* INPUTS:
*   *lineotn_handle          - pointer to lineotn handle instance
* 
*    is_lineotn_ss           - when TRUE, LINEOTN SS is to be cleaned up, otherwise
* 
*   *restart_init_cfg_ptr    - pointer to restart init config
* 
*   top_energy_state_reg     - The expected energy state 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - WARM : register/context are coherent; or
*                 CRASH: everything is coherent (a cleanup may has been executed)
*   LINEOTN_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT - incoherences have been detected
*                 on communal energy configuration
*                 that is incoherent with top energy state
*                 - WARM : ERROR
*                 - CRASH: TOP shall put LINEOTN in RESET mode
*                   subsystem shall be put in reset mode 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_handle_restart_init(lineotn_handle_t     *lineotn_handle,  
                                             BOOL8                 is_lineotn_ss,   
                                             util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                             pmc_energy_state_t    top_energy_state_reg)
{
    lineotn_recover_var_t   *lineotn_var_reg   = NULL;
    lineotn_energy_state_t   lineotn_energy_state_reg;
    lineotn_var_t           *lineotn_var_clean = NULL;
    PMC_ERROR rc = PMC_SUCCESS;
    
    PMC_ENTRY();
    lineotn_var_reg = (lineotn_recover_var_t*) PMC_CALLOC(sizeof(lineotn_recover_var_t));
    
    if (NULL == lineotn_var_reg) 
    {
        rc = LINEOTN_ERR_INVALID_PTR;
    }

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(LINEOTN_LOG_ERR_STRINGS[0], LINEOTN_LOG_ERR_TABLE_BASE, LINEOTN_LOG_ERR_COUNT);
    }

    /* restart sub-clock */
    rc = otu_frm_handle_restart_init(lineotn_handle->otu_frm_handle, restart_init_cfg_ptr);        

    if (PMC_SUCCESS == rc)
    {
        /* 
           Recover context from register space        
           - var context is partially recovered from register space (good enough to have a valid audit)
           - only OPERATIONAL channel configuration are recovered
           - anyway a partial channel config is an error in warm restart and will be cleanup in crash-restarted
           - detailed energy states are recovered
           - incoherency in register context means that the subsystem shall be shutdown
        */
        rc = lineotn_ctxt_reg_recover(lineotn_handle,
                                      is_lineotn_ss,
                                      top_energy_state_reg,
                                      lineotn_var_reg,
                                      &lineotn_energy_state_reg);


        if (PMC_SUCCESS == rc)
        {
            lineotn_var_clean = (lineotn_var_t*) PMC_CALLOC(sizeof(lineotn_var_t));
        
            if(NULL == lineotn_var_clean) 
            {
                rc = LINEOTN_ERR_INVALID_PTR;
            }
            
            if (PMC_SUCCESS == rc)
            {
                /* 
                 * Audit register context and memory context 
                 *   - Audit power and decide if we should just shut down the subsystem 
                 *   - Create a new context that represents the intersection between memory and register space (lineotn_var_clean)
                 */
                rc = lineotn_ctxt_audit(lineotn_handle, 
                                        is_lineotn_ss,
                                        lineotn_var_reg,
                                        &lineotn_energy_state_reg,
                                        lineotn_var_clean); 
                
                PMC_LOG_TRACE("lineotn_ctxt_audit rc = %s\n", pmc_log_string_get(rc));
                if (LINEOTN_ERR_CONTEXT_CLEANUP == rc && restart_init_cfg_ptr->is_warm_restart == FALSE)
                {
                    /* 
                     * Cleanup 
                     *   - At the end, lineotn_var_clean is the new context
                     *    - We keep OPERATIONAL channel defined in lineotn_var_clean
                     *    - We clean (deactivate/deprov) all channels that are not OPERATIONAL
                     *       - they may have be partially provisioned
                     *    - We adjust power setting based on lineotn_var_clean
                     */                                                
                    rc = lineotn_ctxt_cleanup(lineotn_handle, 
                                              is_lineotn_ss,
                                              lineotn_var_reg,
                                              &lineotn_energy_state_reg,
                                              lineotn_var_clean);                 
                }
                
                if ( NULL != lineotn_var_clean)
                {
                    PMC_FREE(&lineotn_var_clean);
                }
            }
        }

    }
    if (NULL != lineotn_var_reg)
    {
        PMC_FREE(&lineotn_var_reg);
    }

    PMC_RETURN(rc);
} /* lineotn_handle_restart_init */


/*******************************************************************************
* FUNCTION: lineotn_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   lineotn_handle     - Pointer to subsystem handle
* 
*   dsi_stream         - DSI stream
* 
*   int_table_ptr      - pointer to aggregation or channel interrupt table with
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - is returned when inputs are valid; otherwise the
*                     LINEOTN_ERR_INVALID_PARAMETERS error code is returned
*                     
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR lineotn_int_validate(lineotn_handle_t      *lineotn_handle,
                                       UINT32                 dsi_stream,
                                       void                  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != lineotn_handle, LINEOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (dsi_stream >= lineotn_handle->cfg.max_dsi_streams &&
        dsi_stream != LINEOTN_SFI51_1_DSI_STREAM_IDX)
    {
        result = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    if (NULL == int_table_ptr)
    {
        result = LINEOTN_ERR_INVALID_PARAMETERS;
    }


    PMC_RETURN(result);
} /* lineotn_int_validate */


/*******************************************************************************
* FUNCTION: lineotn_int_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the interrupt table.
*   
*
* INPUTS:
*   lineotn_handle   - Pointer to subsystem handle
* 
*   int_table_ptr     - pointer to interrupt table with bits set to enable
*                       interrupts. If NULL, enables/disables all interrupts
* 
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
* 
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - when successful, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_int_enable(lineotn_handle_t   *lineotn_handle,
                                    lineotn_int_t      *int_table_ptr,
                                    lineotn_int_t      *int_en_table_ptr,
                                    BOOL                enable)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != lineotn_handle, LINEOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(NULL != int_en_table_ptr, LINEOTN_ERR_INVALID_PARAMETERS, 0, 0);

    if (PMC_SUCCESS == result)
    {
        result = otu_frm_int_enable(lineotn_handle->otu_frm_handle, 
                                    &int_table_ptr->otu_frm,
                                    &int_en_table_ptr->otu_frm,
                                    enable);
    }

#ifdef LINEOTN_INT
#undef LINEOTN_INT
#endif
#define LINEOTN_INT UTIL_GLOBAL_INT_ENABLE_SINGLE
            
    if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
    {
        LINEOTN_INT_TABLE_DEFINE();
    }
    else
    {
        LINEOTN_INT_TABLE_DEFINE(sysotn_);
    }
    
    PMC_RETURN(result);
} /* lineotn_int_enable */


/*******************************************************************************
* FUNCTION: lineotn_int_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the interrupt table.
*   
*
* INPUTS:
*   lineotn_handle    - Pointer to subsystem handle
* 
*   int_table_ptr     - pointer to interrupt table with bits set to clear
*                       interrupts.
* 
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*                       If NULL, clears all interrupts.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - when successful, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_int_clear(lineotn_handle_t  *lineotn_handle,
                                   lineotn_int_t     *int_table_ptr,
                                   lineotn_int_t     *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != lineotn_handle, LINEOTN_ERR_INVALID_PARAMETERS, 0, 0);

    if (PMC_SUCCESS == result)
    {
        result = otu_frm_int_clear(lineotn_handle->otu_frm_handle, 
                                    &int_table_ptr->otu_frm,
                                    &int_en_table_ptr->otu_frm);
    }

#ifdef LINEOTN_INT
#undef LINEOTN_INT
#endif
#define LINEOTN_INT UTIL_GLOBAL_INT_CLEAR_SINGLE

    if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
    {
        LINEOTN_INT_TABLE_DEFINE();
    }
    else
    {
        LINEOTN_INT_TABLE_DEFINE(sysotn_);
    }

    PMC_RETURN(result);
} /* lineotn_int_clear */


/*******************************************************************************
* FUNCTION: lineotn_int_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the interrupt table.
*   
*
* INPUTS:
*   lineotn_handle    - Pointer to subsystem handle
* 
*   filt_table_ptr    - pointer to filter table. Mark fields as TRUE within this
*                       table to indicate that the interrupt/status will be
*                       retrieved to the output table. NULL to retrieve all
*                       interrupt/status.
*
* OUTPUTS:
*   int_table_ptr     - pointer to channel interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS     -  when successful, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_int_retrieve(lineotn_handle_t    *lineotn_handle,
                                      lineotn_int_t       *filt_table_ptr,
                                      lineotn_int_t       *int_table_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;
    otu_frm_int_t *otu_frm_ptr = NULL;
    PMC_ENTRY();

    PMC_ASSERT(NULL != lineotn_handle, LINEOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(NULL != int_table_ptr, LINEOTN_ERR_INVALID_PARAMETERS, 0, 0);

    if (NULL != filt_table_ptr)
    {
        otu_frm_ptr = &filt_table_ptr->otu_frm;
    }

    result = otu_frm_int_retrieve(lineotn_handle->otu_frm_handle,
                                  otu_frm_ptr,
                                  &int_table_ptr->otu_frm);

    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(lineotn_handle, fecstat, UTIL_GLOBAL_INT_OPTIMIZE);
    
#ifdef LINEOTN_INT
#undef LINEOTN_INT
#endif
#define LINEOTN_INT UTIL_GLOBAL_INT_RETRIEVE_SINGLE

    if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
    {
        LINEOTN_INT_TABLE_DEFINE();
    }
    else
    {
        LINEOTN_INT_TABLE_DEFINE(sysotn_);
    }

    UTIL_GLOBAL_INT_BUFFER_FLUSH(lineotn_handle, fecstat);
    
    PMC_RETURN(result);
} /* lineotn_int_retrieve */


/*******************************************************************************
* FUNCTION: lineotn_int_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   lineotn_handle    - Pointer to subsystem handle
* 
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS     - when successful or a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_int_enabled_check(lineotn_handle_t   *lineotn_handle,
                                           lineotn_int_t      *int_en_table_ptr,
                                           BOOL               *int_found_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != lineotn_handle, LINEOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(NULL != int_found_ptr, LINEOTN_ERR_INVALID_PARAMETERS, 0, 0);

    if (PMC_SUCCESS == result)
    {
        result = otu_frm_int_enabled_check(lineotn_handle->otu_frm_handle,
                                           &int_en_table_ptr->otu_frm, 
                                           int_found_ptr);
    }

#ifdef LINEOTN_INT
#undef LINEOTN_INT
#endif
#define LINEOTN_INT UTIL_GLOBAL_INT_CHECK_SINGLE
        
    if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
    {
        LINEOTN_INT_TABLE_DEFINE();
    }
    else
    {
        LINEOTN_INT_TABLE_DEFINE(sysotn_);
    }

    PMC_RETURN(result);
} /* lineotn_int_enabled_check */


/*******************************************************************************
* FUNCTION: lineotn_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the channel interrupt table.
*   
*
* INPUTS:
*   lineotn_handle   - Pointer to subsystem handle
* 
*   dsi_stream        - DSI stream
* 
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
* 
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
* 
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - when successful, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_int_chnl_enable(lineotn_handle_t   *lineotn_handle,
                                         UINT32              dsi_stream,
                                         lineotn_int_chnl_t *int_table_ptr,
                                         lineotn_int_chnl_t *int_en_table_ptr,
                                         BOOL                enable)
{
    PMC_ERROR       result = PMC_SUCCESS;

    UINT32          i = 0;
    UINT32          j = 0;
    UINT32          start_id;
    UINT32          end_id;
    UINT32          link = 0;
    UINT32          otu_frm_idx = 0;
    BOOL8           has_opsm4 = FALSE;
    BOOL8           has_opsm23 = FALSE;
    UINT32          opsm23_idx = 0;
    BOOL8           has_sfis = FALSE;
    UINT32          sfis_idx = 0;
    BOOL8           has_sfi51 = FALSE;
    UINT32          sfi51_idx = 0;

    PMC_ENTRY();

    result = lineotn_int_validate(lineotn_handle, dsi_stream, int_en_table_ptr);

    if (PMC_SUCCESS == result)
    {
        /* Get the right index for OTU_FRM, SFIS, SFI5.1 for dsi_stream */
        result = lineotn_int_params_get(lineotn_handle,
                                        dsi_stream,
                                        &otu_frm_idx,
                                        &has_opsm4,
                                        &has_opsm23,
                                        &opsm23_idx,
                                        &has_sfis,
                                        &sfis_idx,
                                        &has_sfi51,
                                        &sfi51_idx,
                                        &start_id,
                                        &end_id);
        if (opsm23_idx >= 3)
        {
            PMC_RETURN(LINEOTN_ERR_INVALID_PARAMETERS);
        }        
    }
    PMC_ATOMIC_YIELD(lineotn_handle->base.parent_handle,0);
    if (PMC_SUCCESS == result)
    {
        result = otu_frm_int_chnl_enable(lineotn_handle->otu_frm_handle, 
                                         lineotn_handle->var.lineotn_dsi_dist[otu_frm_idx].dci_channel,
                                         &int_table_ptr->otu_frm,
                                         &int_en_table_ptr->otu_frm,
                                         enable);
    }
    PMC_ATOMIC_YIELD(lineotn_handle->base.parent_handle,0);
    
    if (PMC_SUCCESS == result && TRUE == has_sfis)
    {
        result = lineotn_sfis_int_chnl_enable(lineotn_handle, 
                                              sfis_idx,
                                              &int_table_ptr->sfis,
                                              &int_en_table_ptr->sfis,
                                              enable);
    }

    if (PMC_SUCCESS == result && TRUE == has_sfi51)
    {
        result = lineotn_sfi51_int_chnl_enable(lineotn_handle, 
                                               sfi51_idx,
                                               &int_table_ptr->sfi51,
                                               &int_en_table_ptr->sfi51,
                                               enable);
    }

    PMC_ATOMIC_YIELD(lineotn_handle->base.parent_handle,0);
    
    if (PMC_SUCCESS == result)
    {
        
#ifdef LINEOTN_INT
#undef LINEOTN_INT
#endif
#define LINEOTN_INT UTIL_GLOBAL_INT_ENABLE

#ifdef LINEOTN_INT_LANE
#undef LINEOTN_INT_LANE
#endif
#define LINEOTN_INT_LANE LINEOTN_INT_ENABLE_ARRAY

#ifdef LINEOTN_INT_ARRAY
#undef LINEOTN_INT_ARRAY
#endif
#define LINEOTN_INT_ARRAY LINEOTN_INT_ENABLE_ARRAY_RANGE

#ifdef LINEOTN_INT_100G
#undef LINEOTN_INT_100G
#endif
#define LINEOTN_INT_100G UTIL_GLOBAL_INT_ENABLE_SINGLE

#ifdef LINEOTN_INT_100G_LANE
#undef LINEOTN_INT_100G_LANE
#endif
#define LINEOTN_INT_100G_LANE UTIL_GLOBAL_INT_ENABLE_ARRAY_SINGLE

        if (has_opsm4)
        {
            link = 0;
            if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
            {
                LINEOTN_OPSM4_INT_CHNL_TABLE_DEFINE();
            }
            else
            {
                LINEOTN_OPSM4_INT_CHNL_TABLE_DEFINE(sysotn_);
            }
        }
        else if (has_opsm23)
        {
            link = opsm23_idx;
            if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
            {
                LINEOTN_OPSM23_INT_CHNL_TABLE_DEFINE();
            }
            else
            {
                LINEOTN_OPSM23_INT_CHNL_TABLE_DEFINE(sysotn_);
            }
        }

        /* Save valid flags */
        int_en_table_ptr->opsm4_valid = has_opsm4;
        int_en_table_ptr->opsm23_valid = has_opsm23;
        int_en_table_ptr->sfis_valid = has_sfis;

    }
    PMC_RETURN(result);
} /* lineotn_int_chnl_enable */


/*******************************************************************************
* FUNCTION: lineotn_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the channel interrupt table.
*   
*
* INPUTS:
*   lineotn_handle    - Pointer to subsystem handle
* 
*   dsi_stream        - DSI stream
* 
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts.
*                       If NULL, clears all interrupts.
* 
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - when successful otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_int_chnl_clear(lineotn_handle_t  *lineotn_handle,
                                        UINT32              dsi_stream,
                                        lineotn_int_chnl_t *int_en_table_ptr,
                                        lineotn_int_chnl_t *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          i;
    UINT32          link;
    UINT32          otu_frm_idx;
    BOOL8           has_opsm4;
    BOOL8           has_opsm23;
    UINT32          opsm23_idx;
    BOOL8           has_sfis;
    UINT32          sfis_idx;
    BOOL8           has_sfi51;
    UINT32          sfi51_idx;
    UINT32          start_id;
    UINT32          end_id;
    UINT32          j = 0;

    PMC_ENTRY();

    result = lineotn_int_validate(lineotn_handle, dsi_stream, lineotn_handle);

    if (PMC_SUCCESS == result)
    {
        /* Get the right index for OTU_FRM, SFIS, SFI5.1 for dsi_stream */
        result = lineotn_int_params_get(lineotn_handle,
                                        dsi_stream,
                                        &otu_frm_idx,
                                        &has_opsm4,
                                        &has_opsm23,
                                        &opsm23_idx,
                                        &has_sfis,
                                        &sfis_idx,
                                        &has_sfi51,
                                        &sfi51_idx,
                                        &start_id,
                                        &end_id);
    }

    if (PMC_SUCCESS == result)
    {
        result = otu_frm_int_chnl_clear(lineotn_handle->otu_frm_handle, 
                                        lineotn_handle->var.lineotn_dsi_dist[otu_frm_idx].dci_channel,
                                        UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, otu_frm),
                                        UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, otu_frm));
    }

    if (PMC_SUCCESS == result && TRUE == has_sfis)
    {
        result = lineotn_sfis_int_chnl_clear(lineotn_handle, 
                                             sfis_idx,
                                             UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, sfis),
                                             UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, sfis));
    }

    if (PMC_SUCCESS == result && TRUE == has_sfi51)
    {
        result = lineotn_sfi51_int_chnl_clear(lineotn_handle, 
                                              sfi51_idx,
                                              UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, sfi51),
                                              UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, sfi51));
    }
    if (PMC_SUCCESS == result)
    {
        if (opsm23_idx >= 3)
        {
            PMC_RETURN(LINEOTN_ERR_INVALID_PARAMETERS);
        }

#ifdef LINEOTN_INT
#undef LINEOTN_INT
#endif
#define LINEOTN_INT UTIL_GLOBAL_INT_CLEAR

#ifdef LINEOTN_INT_LANE
#undef LINEOTN_INT_LANE
#endif
#define LINEOTN_INT_LANE LINEOTN_INT_CLEAR_ARRAY

#ifdef LINEOTN_INT_ARRAY
#undef LINEOTN_INT_ARRAY
#endif
#define LINEOTN_INT_ARRAY LINEOTN_INT_CLEAR_ARRAY_RANGE

#ifdef LINEOTN_INT_100G
#undef LINEOTN_INT_100G
#endif
#define LINEOTN_INT_100G UTIL_GLOBAL_INT_CLEAR_SINGLE

#ifdef LINEOTN_INT_100G_LANE
#undef LINEOTN_INT_100G_LANE
#endif
#define LINEOTN_INT_100G_LANE UTIL_GLOBAL_INT_CLEAR_ARRAY_SINGLE
        
        if (has_opsm4)
        {
            link = 0;
            if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
            {
                LINEOTN_OPSM4_INT_CHNL_TABLE_DEFINE();
            }
            else
            {
                LINEOTN_OPSM4_INT_CHNL_TABLE_DEFINE(sysotn_);
            }
        }
        else if (has_opsm23)
        {
            link = opsm23_idx;
            if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
            {
                LINEOTN_OPSM23_INT_CHNL_TABLE_DEFINE();
            }
            else
            {
                LINEOTN_OPSM23_INT_CHNL_TABLE_DEFINE(sysotn_);
            }
        }
    }

    PMC_RETURN(result);
} /* lineotn_int_chnl_clear */


/*******************************************************************************
* FUNCTION: lineotn_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   lineotn_handle    - Pointer to subsystem handle
*   dsi_stream        - DSI stream (see lineotn_stream_prov function for description)
* 
*   filt_table_ptr    - pointer to filter table. Mark fields as TRUE within this
*                       table to indicate that the interrupt/status will be retrieved
*                       to the output table. NULL to retrieve all 
*                       interrupt/status.
*
* OUTPUTS:
*   int_table_ptr     - pointer to channel interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS     - when successful, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_int_chnl_retrieve(lineotn_handle_t    *lineotn_handle,
                                           UINT32               dsi_stream,
                                           lineotn_int_chnl_t  *filt_table_ptr,
                                           lineotn_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          i;
    UINT32          link;
    UINT32          otu_frm_idx;
    BOOL8           has_opsm4;
    BOOL8           has_opsm23;
    UINT32          opsm23_idx;
    BOOL8           has_sfis;
    UINT32          sfis_idx;
    BOOL8           has_sfi51;
    UINT32          sfi51_idx;
    UINT32          start_id;
    UINT32          end_id;
    UINT32          j = 0;

    PMC_ENTRY();

    result = lineotn_int_validate(lineotn_handle, dsi_stream, int_table_ptr);

    if (PMC_SUCCESS == result)
    {
        /* Get the right index for OTU_FRM, SFIS, SFI5.1 for dsi_stream */
        result = lineotn_int_params_get(lineotn_handle,
                                        dsi_stream,
                                        &otu_frm_idx,
                                        &has_opsm4,
                                        &has_opsm23,
                                        &opsm23_idx,
                                        &has_sfis,
                                        &sfis_idx,
                                        &has_sfi51,
                                        &sfi51_idx,
                                        &start_id,
                                        &end_id);
    }

    if (PMC_SUCCESS == result)
    {
        result = otu_frm_int_chnl_retrieve(lineotn_handle->otu_frm_handle, 
                                           lineotn_handle->var.lineotn_dsi_dist[otu_frm_idx].dci_channel,
                                           UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, otu_frm),
                                           &int_table_ptr->otu_frm);
    }

    if (PMC_SUCCESS == result && TRUE == has_sfis)
    {
        result = lineotn_sfis_int_chnl_retrieve(lineotn_handle, 
                                                sfis_idx,
                                                UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, sfis),
                                                &int_table_ptr->sfis);
    }

    if (PMC_SUCCESS == result && TRUE == has_sfi51)
    {
        result = lineotn_sfi51_int_chnl_retrieve(lineotn_handle, 
                                                 sfi51_idx,
                                                 UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, sfi51),
                                                 &int_table_ptr->sfi51);
    }

    if (PMC_SUCCESS == result)
    {
        if (opsm23_idx >= 3)
        {
            PMC_RETURN(LINEOTN_ERR_INVALID_PARAMETERS);
        }


        link = opsm23_idx;
        UTIL_GLOBAL_INT_BUFFER_INIT(lineotn_handle, opsm23, UTIL_GLOBAL_INT_OPTIMIZE);
        UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(lineotn_handle, opsm4,  UTIL_GLOBAL_INT_OPTIMIZE);
        UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(lineotn_handle, sysotn_opsm4,  UTIL_GLOBAL_INT_OPTIMIZE);
    
#ifdef LINEOTN_INT
#undef LINEOTN_INT
#endif
#define LINEOTN_INT UTIL_GLOBAL_INT_RETRIEVE
        
#ifdef LINEOTN_INT_LANE
#undef LINEOTN_INT_LANE
#endif
#define LINEOTN_INT_LANE LINEOTN_INT_RETRIEVE_ARRAY
        
#ifdef LINEOTN_INT_ARRAY
#undef LINEOTN_INT_ARRAY
#endif
#define LINEOTN_INT_ARRAY LINEOTN_INT_RETRIEVE_ARRAY_RANGE

#ifdef LINEOTN_INT_100G
#undef LINEOTN_INT_100G
#endif
#define LINEOTN_INT_100G UTIL_GLOBAL_INT_RETRIEVE_SINGLE
        
#ifdef LINEOTN_INT_100G_LANE
#undef LINEOTN_INT_100G_LANE
#endif
#define LINEOTN_INT_100G_LANE UTIL_GLOBAL_INT_RETRIEVE_ARRAY_SINGLE

        if (has_opsm4)
        {
            link = 0;
            if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
            {
                LINEOTN_OPSM4_INT_CHNL_TABLE_DEFINE();
            }
            else
            {
                LINEOTN_OPSM4_INT_CHNL_TABLE_DEFINE(sysotn_);
            }
        }
        else if (has_opsm23)
        {
            link = opsm23_idx;
            if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
            {
                LINEOTN_OPSM23_INT_CHNL_TABLE_DEFINE();
            }
            else
            {
                LINEOTN_OPSM23_INT_CHNL_TABLE_DEFINE(sysotn_);
            }
        }


        /* Status */
    
#ifdef LINEOTN_STATUS
#undef LINEOTN_STATUS
#endif
#define LINEOTN_STATUS UTIL_GLOBAL_STATUS_RETRIEVE
        
#ifdef LINEOTN_STATUS_LANE
#undef LINEOTN_STATUS_LANE
#endif
#define LINEOTN_STATUS_LANE LINEOTN_STATUS_RETRIEVE_ARRAY
        
#ifdef LINEOTN_STATUS_ARRAY
#undef LINEOTN_STATUS_ARRAY
#endif
#define LINEOTN_STATUS_ARRAY LINEOTN_STATUS_RETRIEVE_ARRAY_RANGE

#ifdef LINEOTN_STATUS_100G
#undef LINEOTN_STATUS_100G
#endif
#define LINEOTN_STATUS_100G UTIL_GLOBAL_STATUS_RETRIEVE_SINGLE
        
#ifdef LINEOTN_STATUS_100G_LANE
#undef LINEOTN_STATUS_100G_LANE
#endif
#define LINEOTN_STATUS_100G_LANE UTIL_GLOBAL_STATUS_RETRIEVE_ARRAY_SINGLE

        if (has_opsm4)
        {
            link = 0;
            if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
            {
                LINEOTN_OPSM4_STATUS_CHNL_TABLE_DEFINE();
            }
            else
            {
                LINEOTN_OPSM4_STATUS_CHNL_TABLE_DEFINE(sysotn_);
            }
        }
        else if (has_opsm23)
        {
            link = opsm23_idx;
            if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
            {
                LINEOTN_OPSM23_STATUS_CHNL_TABLE_DEFINE();
            }
            else
            {
                LINEOTN_OPSM23_STATUS_CHNL_TABLE_DEFINE(sysotn_);
            }
        }
   
    
        UTIL_GLOBAL_INT_BUFFER_FLUSH(lineotn_handle, opsm23);
        UTIL_GLOBAL_INT_BUFFER_FLUSH(lineotn_handle, opsm4);
        UTIL_GLOBAL_INT_BUFFER_FLUSH(lineotn_handle, sysotn_opsm4);
    
        /* Save valid flags */
        int_table_ptr->opsm4_valid = has_opsm4;
        int_table_ptr->opsm23_valid = has_opsm23;
        int_table_ptr->sfis_valid = has_sfis;
    }
    
    PMC_RETURN(result);
} /* lineotn_int_chnl_retrieve */


/*******************************************************************************
* FUNCTION: lineotn_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   lineotn_handle    - Pointer to subsystem handle
* 
*   dsi_stream        - DSI stream (see lineotn_stream_prov function for description)
* 
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS     - when successful, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_int_chnl_enabled_check(lineotn_handle_t   *lineotn_handle,
                                                UINT32              dsi_stream,
                                                lineotn_int_chnl_t *int_en_table_ptr,
                                                BOOL               *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          i;
    UINT32          link;
    UINT32          otu_frm_idx;
    BOOL8           has_opsm4;
    BOOL8           has_opsm23;
    UINT32          opsm23_idx;
    BOOL8           has_sfis;
    UINT32          sfis_idx;
    BOOL8           has_sfi51;
    UINT32          sfi51_idx;
    UINT32          start_id;
    UINT32          end_id;
    UINT32          j = 0;

    PMC_ENTRY();

    result = lineotn_int_validate(lineotn_handle, dsi_stream, int_found_ptr);

    if (PMC_SUCCESS == result)
    {
        /* Get the right index for OTU_FRM, SFIS, SFI5.1 for dsi_stream */
        result = lineotn_int_params_get(lineotn_handle,
                                        dsi_stream,
                                        &otu_frm_idx,
                                        &has_opsm4,
                                        &has_opsm23,
                                        &opsm23_idx,
                                        &has_sfis,
                                        &sfis_idx,
                                        &has_sfi51,
                                        &sfi51_idx,
                                        &start_id,
                                        &end_id);
    }

    if ((PMC_SUCCESS == result) &&
        (FALSE == *int_found_ptr))

    {
        result = otu_frm_int_chnl_enabled_check(lineotn_handle->otu_frm_handle, 
                                                lineotn_handle->var.lineotn_dsi_dist[otu_frm_idx].dci_channel,
                                                &int_en_table_ptr->otu_frm,
                                                int_found_ptr);
    }

    if ((PMC_SUCCESS == result && TRUE == has_sfis) &&
        (FALSE == *int_found_ptr))

    {
        result = lineotn_sfis_int_chnl_enabled_check(lineotn_handle, 
                                                     sfis_idx,
                                                     &int_en_table_ptr->sfis,
                                                     int_found_ptr);
    }
    
    if ((PMC_SUCCESS == result && TRUE == has_sfi51) &&
        (FALSE == *int_found_ptr))

    {
        result = lineotn_sfi51_int_chnl_enabled_check(lineotn_handle, 
                                                      sfi51_idx,
                                                      &int_en_table_ptr->sfi51,
                                                      int_found_ptr);
    }
    if (PMC_SUCCESS == result)
    {
        if (opsm23_idx >= 3)
        {
            PMC_RETURN(LINEOTN_ERR_INVALID_PARAMETERS);
        }

    
#ifdef LINEOTN_INT
#undef LINEOTN_INT
#endif
#define LINEOTN_INT UTIL_GLOBAL_INT_CHECK
        
#ifdef LINEOTN_INT_LANE
#undef LINEOTN_INT_LANE
#endif
#define LINEOTN_INT_LANE LINEOTN_INT_CHECK_ARRAY
        
#ifdef LINEOTN_INT_ARRAY
#undef LINEOTN_INT_ARRAY
#endif
#define LINEOTN_INT_ARRAY LINEOTN_INT_CHECK_ARRAY_RANGE

#ifdef LINEOTN_INT_100G
#undef LINEOTN_INT_100G
#endif
#define LINEOTN_INT_100G UTIL_GLOBAL_INT_CHECK_SINGLE
        
#ifdef LINEOTN_INT_100G_LANE
#undef LINEOTN_INT_100G_LANE
#endif
#define LINEOTN_INT_100G_LANE UTIL_GLOBAL_INT_CHECK_ARRAY_SINGLE

        if (has_opsm4)
        {
            link = 0;
            if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
            {
                LINEOTN_OPSM4_INT_CHNL_TABLE_DEFINE();
            }
            else
            {
                LINEOTN_OPSM4_INT_CHNL_TABLE_DEFINE(sysotn_);
            }
        }
        else if (has_opsm23)
        {
            link = opsm23_idx;
            if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
            {
                LINEOTN_OPSM23_INT_CHNL_TABLE_DEFINE();
            }
            else
            {
                LINEOTN_OPSM23_INT_CHNL_TABLE_DEFINE(sysotn_);
            }
        }
    }

    PMC_RETURN(result);
} /* lineotn_int_chnl_enabled_check */


/*******************************************************************************
* lineotn_int_params_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is a helper function to retrieve which interrupt structures
*   are associated with the current stream ID.
* INPUTS:
*   lineotn_handle         - pointer to lineOTN handle instance
*
*   dsi_stream             - unsigned, dsi stream number to configure:
*                            see lineotn_stream_prov for a detailed description
*   
* OUTPUTS:
*   otu_frm_idx            - Index for the OTU_FRM interrupts [0-11]
*
*   has_opsm4              - TRUE: OPSM4 interrupts are valid and should be retrieved
*
*   has_opsm23             - TRUE: OPSM32 interrupts are valid at index opsm23_idx
*                              and should be retrieved
*
*   opsm23_idx             - Index for the OPSM32 interrupts if has_opsm23=TRUE [0-11]
*
*   has_sfis               - TRUE: SFIS interrupts are valid for index sfis_idx
*                            and should be retrieved
*
*   sfis_idx               - Index for the SFIS interrupts if has_sfis=TRUE [0-3]
*
*   has_sfi51              - TRUE: SFI51 interrupts are valid for index sfi51_idx
*                            and should be retrieved
*
*   sfi51_idx              - Index for the SFI51 interrupts if has_sfi51=TRUE [0-1]
* 
*   opsm23_start_id        - OPSM23 start lane index
* 
*   opsm23_end_id          - OPSM23 end lane index + 1
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise LINEOTN_ERR_INVALID_PARAMETERS is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR lineotn_int_params_get(lineotn_handle_t          *lineotn_handle,
                                         UINT32                     dsi_stream,
                                         UINT32                     *otu_frm_idx,
                                         BOOL8                      *has_opsm4,
                                         BOOL8                      *has_opsm23,
                                         UINT32                     *opsm23_idx,
                                         BOOL8                      *has_sfis,
                                         UINT32                     *sfis_idx,
                                         BOOL8                      *has_sfi51,
                                         UINT32                     *sfi51_idx,
                                         UINT32                     *opsm23_start_id,
                                         UINT32                     *opsm23_end_id)
{
    PMC_ERROR       result = PMC_SUCCESS;

    mld_ifc_type_t  mld_interface_type = MLD_NA;
    util_global_odukp_type_t oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    lineotn_dsi_type_t dsi_type = LAST_LINE_OTN_DSI_TYPE;
    UINT32                     dsi_start = 0;
    UINT32                     dsi_end = 0;

    PMC_ENTRY();

    /* Initialized returned values */
    *has_opsm4 = FALSE;
    *has_opsm23 = FALSE;
    *opsm23_idx = 0;
    *has_sfis = FALSE;
    *sfis_idx = 0;
    *has_sfi51 = FALSE;
    *sfi51_idx = 0;

    /* Find dsi_type */
    if (dsi_stream < lineotn_handle->cfg.max_dsi_streams)
    {
        switch (lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_hw_ifc)
        {
            case MLD_SFI_51:
                dsi_type = LINE_OTN_DSI_TYPE_SFI_51_A;
                break;
            case MLD_SFIS:
                dsi_type = LINE_OTN_DSI_TYPE_SFIS;
                break;
            case MLD_OTL:
                dsi_type = LINE_OTN_DSI_TYPE_OTL;
                break;
            case MLD_OTU2:
                dsi_type = LINE_OTN_DSI_TYPE_OTUK;
                break;
            case MLD_NA:
            default:
                dsi_type = LAST_LINE_OTN_DSI_TYPE;
                break;
        }
    }
    else
    {
        if (lineotn_handle->var.lineotn_sfi51_1_frame_rate != LAST_LINE_OTN_FRAME_RATE)
        {
            dsi_type = LINE_OTN_DSI_TYPE_SFI_51_B;
        }
        else
        {
            result = LINEOTN_ERR_INVALID_PARAMETERS;
        }
    }

    if (PMC_SUCCESS == result)
    {
        /* get mld hw ifc type and oduk type */    
        result = lineotn_dsi_params_get(lineotn_handle,
                                        lineotn_handle->var.current_lineotn_cfg,
                                        dsi_stream,
                                        dsi_type,
                                        &mld_interface_type,
                                        &oduk_type,
                                        &dsi_start,
                                        &dsi_end);
    }
    
    *otu_frm_idx = dsi_start;

    switch ( lineotn_handle->var.current_lineotn_cfg )
    {
            /* 100GB, 1x100GB , 1xOTL4.10  */
        case LINE_OTN_CFG_0:
            *has_opsm4 = TRUE;
            break;

            /* 100GB, 1x100GB , 1xSFI-S(n=10+1) */
        case LINE_OTN_CFG_1:
            *has_opsm4 = TRUE;
            *has_sfis = TRUE;
            *sfis_idx = 0;
            break;


            /* all OTL configurations that are not 100G have the same config */
        case LINE_OTN_CFG_3:/* 120GB , 12x10GB , 12xOTU2  */
        case LINE_OTN_CFG_4:/* 120GB , 1x40GB + 8x10GB , 1xOTL3.4 + 8xOTU2 */
        case LINE_OTN_CFG_11:/* 120GB , 2x40GB + 4x10GB , 2xOTL3.4 + 4xOTU2 */
        case LINE_OTN_CFG_18:/* 120GB , 3x40GB , 3xOTL3.4  */
            if (dsi_stream < lineotn_handle->cfg.max_dsi_streams)
            {
                *has_opsm23 = TRUE;
                *opsm23_idx = dsi_stream / 4;
            }
            else
            {
                result = LINEOTN_ERR_INVALID_PARAMETERS;
            }
            break;

            /* 110GB , 1x40GB + 7x10GB , 1xSFI-S(n=4+1) + 7xOTU2 */
        case LINE_OTN_CFG_5:
            if( (0 == dsi_stream ) &&
                (MLD_SFIS == mld_interface_type) ) /* dsi streams 0-4 SFIS#3 */
            {
                *has_opsm23 = TRUE;
                *opsm23_idx = 0;
                *has_sfis = TRUE;
                *sfis_idx = 2;
            }
            else if (dsi_stream >=5 && dsi_stream < 12) /* dsi streams 5-11 */
            {
                *has_opsm23 = TRUE;
                *opsm23_idx = dsi_stream / 4;
            }
            else
            {
                result = LINEOTN_ERR_INVALID_PARAMETERS;
            }
            break;

            /* 110GB , 1x40GB + 1x40GB + 3x10GB , 1xOTL3.4 + 1xSFI-S(n=4+1) + 3xOTU2  */
        case LINE_OTN_CFG_6:
            if( (0 == dsi_stream ) &&
                (MLD_SFIS == mld_interface_type) ) /* dsi streams 0-4 SFIS#3 */
            {
                *has_opsm23 = TRUE;
                *opsm23_idx = 0;
                *has_sfis = TRUE;
                *sfis_idx = 2;
            }
            else if (dsi_stream >=5 && dsi_stream < 9) /* dsi streams 5-8 */
            {
                *has_opsm23 = TRUE;
                *opsm23_idx = dsi_stream / 4;
            }
            else
            {
                result = LINEOTN_ERR_INVALID_PARAMETERS;
            }
            break;

        case LINE_OTN_CFG_7:/* 120GB , 1x40GB + 8x10GB , 1xSFI5.1 + 8xOTU2 */
        case LINE_OTN_CFG_8:/* 120GB , 1x40GB + 1x40GB + 4x10GB , 1xOTL3.4 + 1xSFI5.1 + 4xOTU2  */
        case LINE_OTN_CFG_12:/* 120GB , 2x40GB + 1x40GB , 2xOTL3.4 + 1xSFI5.1  */
            if ( 0 == dsi_stream && MLD_SFI_51 == mld_interface_type ) /* dsi streams 0-16 SFI5.1#0 */
            {
                *has_opsm23 = TRUE;
                *opsm23_idx = 0;
                *has_sfi51 = TRUE;
                *sfi51_idx = 0;
            }
            else if (dsi_stream >=4 && dsi_stream < 12) /* dsi streams 4-11 */
            {
                *has_opsm23 = TRUE;
                *opsm23_idx = dsi_stream / 4;
            }
            else
            {
                result = LINEOTN_ERR_INVALID_PARAMETERS;
            }
            break;

        case LINE_OTN_CFG_9: /* 120GB , 1x40GB + 1x40GB + 4x10GB , 1xSFI-S(n=4+1) + 1xSFI5.1 + 4xOTU2  */
        case LINE_OTN_CFG_10:/* 120GB , 1x40GB + 1x40GB + 1x40GB , 1xOTL3.4 + 1xSFI-S(n=4+1) + 1xSFI5.1*/
            if ( LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream && MLD_SFI_51 == mld_interface_type ) /* dsi streams 17-33 SFI5.1#1 */
            {
                *has_opsm23 = TRUE;
                *opsm23_idx = 1;
                *has_sfi51 = TRUE;
                *sfi51_idx = 1;
            }
            else if( (0 == dsi_stream ) && (MLD_SFIS == mld_interface_type) ) /* dsi streams 0-4 SFIS#3 */
            {
                *has_opsm23 = TRUE;
                *opsm23_idx = 0;
                *has_sfis = TRUE;
                *sfis_idx = 2;
            }
            else if (dsi_stream >= 8 && dsi_stream < 12)/* dsi streams 8-11 */
            {
                *has_opsm23 = TRUE;
                *opsm23_idx = dsi_stream / 4;
            }
            else
            {
                result = LINEOTN_ERR_INVALID_PARAMETERS;
            }
            break;

            /* 100GB , 2x40GB + 2x10GB , 2xSFI-S(n=4+1) + 2xOTU2 */
        case LINE_OTN_CFG_13:
            if( (0 == dsi_stream || 5 == dsi_stream ) && /* dsi streams 5-9 SFIS#4 */
                (MLD_SFIS == mld_interface_type) ) /* dsi streams 0-4 SFIS#3 */
            {
                if(0 == dsi_stream)
                {
                    *has_opsm23 = TRUE;
                    *opsm23_idx = 0;
                    *has_sfis = TRUE;
                    *sfis_idx = 2;
                }
                else
                {
                    *has_opsm23 = TRUE;
                    *opsm23_idx = 1;
                    *has_sfis = TRUE;
                    *sfis_idx = 3;
                }
            }
            else if (dsi_stream >=10 && dsi_stream < 12)/* dsi streams 10,11 */
            {
                *has_opsm23 = TRUE;
                *opsm23_idx = dsi_stream / 4;
            }
            else
            {
                result = LINEOTN_ERR_INVALID_PARAMETERS;
            }
            break;

            /* 120GB , 2x40GB + 1x40GB , 2xSFI-S(n=4+1) + 1xSFI5.1  */
        case LINE_OTN_CFG_14:
            if( (0 == dsi_stream || 5 == dsi_stream ) && /* dsi streams 5-9 SFIS#4 */
                (MLD_SFIS == mld_interface_type) ) /* dsi streams 0-4 SFIS#3 */
            {
                if(0 == dsi_stream)
                {
                    *has_opsm23 = TRUE;
                    *opsm23_idx = 0;
                    *has_sfis = TRUE;
                    *sfis_idx = 2;
                }
                else
                {
                    *has_opsm23 = TRUE;
                    *opsm23_idx = 1;
                    *has_sfis = TRUE;
                    *sfis_idx = 3;
                }
            }
            else if ( 0 == dsi_stream && MLD_SFI_51 == mld_interface_type ) /* dsi streams 0-16 SFI5.1#0 */
            {
                *has_opsm23 = TRUE;
                *opsm23_idx = 2;
                *has_sfi51 = TRUE;
                *sfi51_idx = 0;
            }
            else
            {
                result = LINEOTN_ERR_INVALID_PARAMETERS;
            }
            break;
            /* 120GB , 2x40GB + 4x10GB , 2xSFI5.1 + 4xOTU2  */
        case LINE_OTN_CFG_15:
            if( dsi_stream >= 8 && MLD_OTU2 == mld_interface_type ) /* dsi streams 8-11 OTU2 */
            {
                *has_opsm23 = TRUE;
                *opsm23_idx = dsi_stream / 4;
            }
            else if ( (0 == dsi_stream || LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream) && /* dsi streams 0-16 SFI5.1#0 */
                      (MLD_SFI_51 == mld_interface_type) ) /* dsi streams 17-33 SFI5.1#1 */
            {
                if(0 == dsi_stream) /* on this configuration sfi5.1 0 uses opsm23 #0 */
                {
                    *has_opsm23 = TRUE;
                    *opsm23_idx = 0;
                    *has_sfi51 = TRUE;
                    *sfi51_idx = 0;
                }
                else
                {
                    *has_opsm23 = TRUE;
                    *opsm23_idx = 1;
                    *has_sfi51 = TRUE;
                    *sfi51_idx = 1;
                }
            }
            else
            {
                result = LINEOTN_ERR_INVALID_PARAMETERS;
            }
            break;
            /* 120GB , 2x40GB + 1x40GB , 2xSFI5.1 + 1xOTL3.4 */
        case LINE_OTN_CFG_16:
            if( 8 == dsi_stream && MLD_OTL == mld_interface_type ) /* dsi streams 8-11 OTL3.4 */
            {
                *has_opsm23 = TRUE;
                *opsm23_idx = dsi_stream / 4;
            }
            else if ( (0 == dsi_stream || LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream) && /* dsi streams 0-16 SFI5.1#0 */
                      (MLD_SFI_51 == mld_interface_type) ) /* dsi streams 17-33 SFI5.1#1 */
            {
                if(0 == dsi_stream) /* on this configuration sfi5.1 0 uses opsm23 #0 */
                {
                    *has_opsm23 = TRUE;
                    *opsm23_idx = 0;
                    *has_sfi51 = TRUE;
                    *sfi51_idx = 0;
                }
                else
                {
                    *has_opsm23 = TRUE;
                    *opsm23_idx = 1;
                    *has_sfi51 = TRUE;
                    *sfi51_idx = 1;
                }
            }
            else
            {
                result = LINEOTN_ERR_INVALID_PARAMETERS;
            }
            break;

            /* 120GB , 2x40GB + 1x40GB , 2xSFI5.1 + 1xSFI-S(n=4+1)   */
        case LINE_OTN_CFG_17:
            if( (0 == dsi_stream ) &&
                (MLD_SFIS == mld_interface_type) ) /* dsi streams 0-4 SFIS#3 */
            {
                *has_opsm23 = TRUE;
                *opsm23_idx = 0;
                *has_sfis = TRUE;
                *sfis_idx = 2;
            }
            else if ( (0 == dsi_stream || LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream) && /* dsi streams 0-16 SFI5.1#0 */
                      (MLD_SFI_51 == mld_interface_type) ) /* dsi streams 17-33 SFI5.1#1 */
            {
                if(0 == dsi_stream) /* on this configuration sfi5.1 0 uses opsm23 #0 */
                {
                    *has_opsm23 = TRUE;
                    *opsm23_idx = 2;
                    *has_sfi51 = TRUE;
                    *sfi51_idx = 0;
                }
                else
                {
                    *has_opsm23 = TRUE;
                    *opsm23_idx = 1;
                    *has_sfi51 = TRUE;
                    *sfi51_idx = 1;
                }
            }
            else
            {
                result = LINEOTN_ERR_INVALID_PARAMETERS;
            }
            break;

        default:
            result = LINEOTN_ERR_INVALID_PARAMETERS;
            break;

    }

    if (PMC_SUCCESS == result && TRUE == *has_opsm23)
    {
        *opsm23_start_id = dsi_start % 4;
        *opsm23_end_id = ((dsi_end) % 4) + 1;
    }

    PMC_RETURN(result);

}/* lineotn_int_params_get */


/*******************************************************************************
* FUNCTION: lineotn_sfis_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   lineotn_handle    - Pointer to subsystem handle
* 
*   sfis_link         - SFI-S link [0-3]
* 
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
* 
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
* 
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR lineotn_sfis_int_chnl_enable(lineotn_handle_t        *lineotn_handle,
                                               UINT32                   sfis_link,
                                               lineotn_sfis_int_chnl_t *int_table_ptr,
                                               lineotn_sfis_int_chnl_t *int_en_table_ptr,
                                               BOOL                     enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link;
    UINT32          i;
    UINT32          j;
    UINT32          start_id;
    UINT32          end_id;

    PMC_ENTRY();

    if (sfis_link >= 4)
    {
        PMC_RETURN(LINEOTN_ERR_INVALID_PARAMETERS);
    }

    link = sfis_link;
    start_id = 0;
    end_id = LINEOTN_SFIS_NUM_LANE;

#ifdef LINEOTN_INT
#undef LINEOTN_INT
#endif
#define LINEOTN_INT UTIL_GLOBAL_INT_ENABLE
        
#ifdef LINEOTN_INT_ARRAY
#undef LINEOTN_INT_ARRAY
#endif
#define LINEOTN_INT_ARRAY LINEOTN_INT_ENABLE_ARRAY_RANGE

#ifdef LINEOTN_INT_FIELD
#undef LINEOTN_INT_FIELD
#endif
#define LINEOTN_INT_FIELD LINEOTN_SFIS_INT_FIELD10_ARRAY_ENABLE

    if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
    {
        LINEOTN_SFIS_INT_CHNL_TABLE_DEFINE();
    }
    else
    {
        LINEOTN_SFIS_INT_CHNL_TABLE_DEFINE(sysotn_);
    }

    PMC_RETURN(result);

} /* lineotn_sfis_int_chnl_enable */


/*******************************************************************************
* FUNCTION: lineotn_sfi51_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   lineotn_handle    - Pointer to subsystem handle
* 
*   sfi51_link        - SFI-5.1 link [0-1]
* 
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
* 
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
* 
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR lineotn_sfi51_int_chnl_enable(lineotn_handle_t        *lineotn_handle,
                                                UINT32                   sfi51_link,
                                                lineotn_sfi51_int_chnl_t *int_table_ptr,
                                                lineotn_sfi51_int_chnl_t *int_en_table_ptr,
                                                BOOL                     enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link;
    UINT32          i;
    UINT32          j;
    UINT32          start_id;
    UINT32          end_id;

    PMC_ENTRY();

    if (sfi51_link >= 2)
    {
        PMC_RETURN(LINEOTN_ERR_INVALID_PARAMETERS);
    }

    link = sfi51_link;
    start_id = 0;
    end_id = LINEOTN_SFI51_NUM_LANE;
#ifdef LINEOTN_INT
#undef LINEOTN_INT
#endif
#define LINEOTN_INT UTIL_GLOBAL_INT_ENABLE
        
#ifdef LINEOTN_INT_ARRAY
#undef LINEOTN_INT_ARRAY
#endif
#define LINEOTN_INT_ARRAY LINEOTN_INT_ENABLE_ARRAY_RANGE

    if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
    {
        LINEOTN_SFI51_INT_CHNL_TABLE_DEFINE();
    }
    else
    {
        result = LINEOTN_ERR_INVALID_PARAMETERS; /* SYSOTN doesn't use SFI 5.1 */
    }

    PMC_RETURN(result);

 } /* lineotn_sfi51_int_chnl_enable */


/*******************************************************************************
* FUNCTION: lineotn_sfis_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   lineotn_handle    - Pointer to subsystem handle
* 
*   sfis_link         - SFI-S link [0-3]
* 
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts.
*                       If NULL, clears all interrupts.
* 
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR lineotn_sfis_int_chnl_clear(lineotn_handle_t        *lineotn_handle,
                                              UINT32                   sfis_link,
                                              lineotn_sfis_int_chnl_t *int_en_table_ptr,
                                              lineotn_sfis_int_chnl_t *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link;
    UINT32          i;
    UINT32          j;
    UINT32          start_id;
    UINT32          end_id;

    PMC_ENTRY();

    if (sfis_link >= 4)
    {
        PMC_RETURN(LINEOTN_ERR_INVALID_PARAMETERS);
    }

    link = sfis_link;
    start_id = 0;
    end_id = LINEOTN_SFIS_NUM_LANE;

#ifdef LINEOTN_INT
#undef LINEOTN_INT
#endif
#define LINEOTN_INT UTIL_GLOBAL_INT_CLEAR
        
#ifdef LINEOTN_INT_ARRAY
#undef LINEOTN_INT_ARRAY
#endif
#define LINEOTN_INT_ARRAY LINEOTN_INT_CLEAR_ARRAY_RANGE
        
#ifdef LINEOTN_INT_FIELD
#undef LINEOTN_INT_FIELD
#endif
#define LINEOTN_INT_FIELD LINEOTN_SFIS_INT_FIELD10_ARRAY_CLEAR

    if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
    {
        LINEOTN_SFIS_INT_CHNL_TABLE_DEFINE();
    }
    else
    {
        LINEOTN_SFIS_INT_CHNL_TABLE_DEFINE(sysotn_);
    }

    PMC_RETURN(result);
} /* lineotn_sfis_int_chnl_clear */


/*******************************************************************************
* FUNCTION: lineotn_sfi51_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the channel interrupt table.
*   
*
* INPUTS:
*   lineotn_handle    - Pointer to subsystem handle
* 
*   sfi51_link        - SFI-5.1 link [0-1]
* 
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts.
*                       If NULL, clears all interrupts.
* 
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR lineotn_sfi51_int_chnl_clear(lineotn_handle_t        *lineotn_handle,
                                               UINT32                   sfi51_link,
                                               lineotn_sfi51_int_chnl_t *int_en_table_ptr,
                                               lineotn_sfi51_int_chnl_t *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link;
    UINT32          i;
    UINT32          j;
    UINT32          start_id;
    UINT32          end_id;

    PMC_ENTRY();

    if (sfi51_link >= 2)
    {
        PMC_RETURN(LINEOTN_ERR_INVALID_PARAMETERS);
    }

    link = sfi51_link;
    start_id = 0;
    end_id = LINEOTN_SFI51_NUM_LANE;

#ifdef LINEOTN_INT
#undef LINEOTN_INT
#endif
#define LINEOTN_INT UTIL_GLOBAL_INT_CLEAR
        
#ifdef LINEOTN_INT_ARRAY
#undef LINEOTN_INT_ARRAY
#endif
#define LINEOTN_INT_ARRAY LINEOTN_INT_CLEAR_ARRAY_RANGE
        
    if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
    {
        LINEOTN_SFI51_INT_CHNL_TABLE_DEFINE();
    }
    else
    {
        result = LINEOTN_ERR_INVALID_PARAMETERS; /* SYSOTN doesn't use SFI 5.1 */
    }

    PMC_RETURN(result);
} /* lineotn_sfi51_int_chnl_clear */


/*******************************************************************************
* FUNCTION: lineotn_sfis_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
* INPUTS:
*   lineotn_handle    - Pointer to subsystem handle
* 
*   sfis_link         - SFI-S link [0-3]
* 
*   filt_table_ptr    - pointer to filter table. Mark fields as TRUE within this
*                       table to indicate that the interrupt/status will be retrieved
*                       to the output table. NULL to retrieve all 
*                       interrupt/status.
*
* OUTPUTS:
*   int_table_ptr     - pointer to channel interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS     - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR lineotn_sfis_int_chnl_retrieve(lineotn_handle_t         *lineotn_handle,
                                                 UINT32                    sfis_link,
                                                 lineotn_sfis_int_chnl_t  *filt_table_ptr,
                                                 lineotn_sfis_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR            result = PMC_SUCCESS;
    UINT32               link;
    UINT32               i;
    UINT32          j;
    UINT32          start_id;
    UINT32          end_id;
    PMC_ENTRY();

    if (sfis_link >= 4)
    {
        PMC_RETURN(LINEOTN_ERR_INVALID_PARAMETERS);
    }

    link = sfis_link;
    start_id = 0;
    end_id = LINEOTN_SFIS_NUM_LANE;

    UTIL_GLOBAL_INT_BUFFER_INIT(lineotn_handle, sfis_rx,  UTIL_GLOBAL_INT_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT(lineotn_handle, sfis_tx,  UTIL_GLOBAL_INT_OPTIMIZE);
    
#ifdef LINEOTN_INT
#undef LINEOTN_INT
#endif
#define LINEOTN_INT UTIL_GLOBAL_INT_RETRIEVE
        
#ifdef LINEOTN_INT_ARRAY
#undef LINEOTN_INT_ARRAY
#endif
#define LINEOTN_INT_ARRAY LINEOTN_INT_RETRIEVE_ARRAY_RANGE
        
#ifdef LINEOTN_INT_FIELD
#undef LINEOTN_INT_FIELD
#endif
#define LINEOTN_INT_FIELD LINEOTN_SFIS_INT_FIELD10_ARRAY_RETRIEVE

    if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
    {
        LINEOTN_SFIS_INT_CHNL_TABLE_DEFINE();
    }
    else
    {
        LINEOTN_SFIS_INT_CHNL_TABLE_DEFINE(sysotn_);
    }

    /* Status */
    
#ifdef LINEOTN_STATUS
#undef LINEOTN_STATUS
#endif
#define LINEOTN_STATUS UTIL_GLOBAL_STATUS_RETRIEVE
        
#ifdef LINEOTN_STATUS_ARRAY
#undef LINEOTN_STATUS_ARRAY
#endif
#define LINEOTN_STATUS_ARRAY LINEOTN_STATUS_RETRIEVE_ARRAY_RANGE
        
    if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
    {
        LINEOTN_SFIS_STATUS_CHNL_TABLE_DEFINE();
    }
    else
    {
        LINEOTN_SFIS_STATUS_CHNL_TABLE_DEFINE(sysotn_);
    }
   
    
    UTIL_GLOBAL_INT_BUFFER_FLUSH(lineotn_handle, sfis_rx);
    UTIL_GLOBAL_INT_BUFFER_FLUSH(lineotn_handle, sfis_tx);
    
    PMC_RETURN(result);

} /* lineotn_sfis_int_chnl_retrieve */


/*******************************************************************************
* FUNCTION: lineotn_sfi51_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   lineotn_handle    - Pointer to subsystem handle
* 
*   sfi51_link        - SFI-5.1 link [0-1]
* 
*   filt_table_ptr    - pointer to filter table. Mark fields as TRUE within this
*                       table to indicate that the interrupt/status will be retrieved
*                       to the output table. NULL to retrieve all 
*                       interrupt/status.
*
* OUTPUTS:
*   int_table_ptr     - pointer to channel interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS     - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR lineotn_sfi51_int_chnl_retrieve(lineotn_handle_t         *lineotn_handle,
                                                  UINT32                    sfi51_link,
                                                  lineotn_sfi51_int_chnl_t *filt_table_ptr,
                                                  lineotn_sfi51_int_chnl_t *int_table_ptr)
{
    PMC_ERROR            result = PMC_SUCCESS;
    UINT32               link;
    UINT32               i;
    UINT32          j;
    UINT32          start_id;
    UINT32          end_id;

    PMC_ENTRY();

    if (sfi51_link >= 2)
    {
        PMC_RETURN(LINEOTN_ERR_INVALID_PARAMETERS);
    }

    link = sfi51_link;
    start_id = 0;
    end_id = LINEOTN_SFI51_NUM_LANE;

    UTIL_GLOBAL_INT_BUFFER_INIT(lineotn_handle, sfi51,  UTIL_GLOBAL_INT_OPTIMIZE);
    
#ifdef LINEOTN_INT
#undef LINEOTN_INT
#endif
#define LINEOTN_INT UTIL_GLOBAL_INT_RETRIEVE
        
#ifdef LINEOTN_INT_ARRAY
#undef LINEOTN_INT_ARRAY
#endif
#define LINEOTN_INT_ARRAY LINEOTN_INT_RETRIEVE_ARRAY_RANGE
        
    if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
    {
        LINEOTN_SFI51_INT_CHNL_TABLE_DEFINE();
    }
    else
    {
        result = LINEOTN_ERR_INVALID_PARAMETERS; /* SYSOTN doesn't use SFI 5.1 */
    }

    /* Status */
    
#ifdef LINEOTN_STATUS
#undef LINEOTN_STATUS
#endif
#define LINEOTN_STATUS UTIL_GLOBAL_STATUS_RETRIEVE
        
#ifdef LINEOTN_STATUS_ARRAY
#undef LINEOTN_STATUS_ARRAY
#endif
#define LINEOTN_STATUS_ARRAY LINEOTN_STATUS_RETRIEVE_ARRAY_RANGE
        
    if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
    {
        LINEOTN_SFI51_STATUS_CHNL_TABLE_DEFINE();
    }
    else
    {
        result = LINEOTN_ERR_INVALID_PARAMETERS; /* SYSOTN doesn't use SFI 5.1 */
    }
       
    UTIL_GLOBAL_INT_BUFFER_FLUSH(lineotn_handle, sfi51);
    
    PMC_RETURN(result);
} /* lineotn_sfi51_int_chnl_retrieve */


/*******************************************************************************
* FUNCTION: lineotn_sfis_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   lineotn_handle    - Pointer to subsystem handle
* 
*   sfis_link         - SFI-S link [0-3]
* 
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS     - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR lineotn_sfis_int_chnl_enabled_check(lineotn_handle_t        *lineotn_handle,
                                                      UINT32                   sfis_link,
                                                      lineotn_sfis_int_chnl_t *int_en_table_ptr,
                                                      BOOL                    *int_found_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    UINT32              link;
    UINT32              i;
    UINT32          j;
    UINT32          start_id;
    UINT32          end_id;

    PMC_ENTRY();

    if (sfis_link >= 4)
    {
        PMC_RETURN(LINEOTN_ERR_INVALID_PARAMETERS);
    }

    link = sfis_link;
    start_id = 0;
    end_id = LINEOTN_SFIS_NUM_LANE;

#ifdef LINEOTN_INT
#undef LINEOTN_INT
#endif
#define LINEOTN_INT UTIL_GLOBAL_INT_CHECK
        
#ifdef LINEOTN_INT_ARRAY
#undef LINEOTN_INT_ARRAY
#endif
#define LINEOTN_INT_ARRAY LINEOTN_INT_CHECK_ARRAY_RANGE
        
#ifdef LINEOTN_INT_FIELD
#undef LINEOTN_INT_FIELD
#endif
#define LINEOTN_INT_FIELD LINEOTN_SFIS_INT_FIELD10_ARRAY_CHECK

    if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
    {
        LINEOTN_SFIS_INT_CHNL_TABLE_DEFINE();
    }
    else
    {
        LINEOTN_SFIS_INT_CHNL_TABLE_DEFINE(sysotn_);
    }

    PMC_RETURN(result);
} /* lineotn_sfis_int_chnl_enabled_check */


/*******************************************************************************
* FUNCTION: lineotn_sfi51_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   lineotn_handle    - Pointer to subsystem handle
* 
*   sfi51_link        - SFI-5.1 link [0-1]
* 
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS     - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR lineotn_sfi51_int_chnl_enabled_check(lineotn_handle_t        *lineotn_handle,
                                                       UINT32                   sfi51_link,
                                                       lineotn_sfi51_int_chnl_t *int_en_table_ptr,
                                                       BOOL                    *int_found_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    UINT32              link;
    UINT32              i;
    UINT32              j;
    UINT32              start_id;
    UINT32              end_id;

    PMC_ENTRY();

    if (sfi51_link >= 2)
    {
        PMC_RETURN(LINEOTN_ERR_INVALID_PARAMETERS);
    }

    link = sfi51_link;
    start_id = 0;
    end_id = LINEOTN_SFI51_NUM_LANE;

#ifdef LINEOTN_INT
#undef LINEOTN_INT
#endif
#define LINEOTN_INT UTIL_GLOBAL_INT_CHECK
        
#ifdef LINEOTN_INT_ARRAY
#undef LINEOTN_INT_ARRAY
#endif
#define LINEOTN_INT_ARRAY LINEOTN_INT_CHECK_ARRAY_RANGE
        
    if (LINEOTN_IS_LINE_MODE(lineotn_handle)) 
    {
        LINEOTN_SFI51_INT_CHNL_TABLE_DEFINE();
    }
    else
    {
        result = LINEOTN_ERR_INVALID_PARAMETERS; /* SYSOTN doesn't use SFI 5.1 */
    }

    PMC_RETURN(result);
} /* lineotn_sfi51_int_chnl_enabled_check */




/*******************************************************************************
* lineotn_sysotn_mode_db_entry_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   In SYSOTN mode, The MPMA/MPMO/ODUkSW require access to the LINEOTN calendar.
*   As SYSOTN mode will populate COREOTN's HO database, this method is
*   intended to provide similar functionality to mapotn_db_entry_get(), but
*   accesses the SCBS3 context to do so.
*
*
* INPUTS:
*   *lineotn_handle     - pointer to LINEOTN handle instance to be operated on
* 
*   schd_addr           - calendar offset to get the channel data;
*                         Valid values 0 to 95
*
* OUTPUTS:
*   *chnl               - pointer to the channel ID; 
*                         Valid range: 0 - 95 
*
* RETURNS:
*   PMC_ERROR - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_sysotn_mode_db_entry_get(lineotn_handle_t *lineotn_handle,
                                                  UINT32 schd_addr, 
                                                  UINT32 *chnl)
{

    UINT32 num_of_recs = 0;
    mapotn_db_key   key;
    mapotn_db_query_res *q_result;

    PMC_ENTRY();
    
    PMC_ASSERT(lineotn_handle != NULL, LINEOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(schd_addr < 96, LINEOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    /* query chnl ID data for the schd_addr */
    key.calendar_entry = schd_addr;
    key.channel_num = DO_NOT_USE_KEY;
    num_of_recs = gen_db_query(&(lineotn_handle->base),
                               UTIL_GEN_DB_LINEOTN_SYSOTN_MODE,
                               lineotn_handle->otu_frm_handle->var.lineotn_db_handle_sysotn_mode,
                               (void *)&key,
                               (void **)&q_result);

    if(num_of_recs == 0)
    {
        PMC_RETURN(LINEOTN_DB_ENTRY_NOT_FOUND);
    }

    PMC_LOG_TRACE("MAPOTN DB Element: schd_addr=0x%x,cal channel_id=0x%x,\n",
                  q_result->p_recs[0]->calendar_entry, q_result->p_recs[0]->channel_num); 
        
    *chnl = q_result->p_recs[0]->channel_num;

    PMC_RETURN(PMC_SUCCESS);

} /*lineotn_sysotn_mode_db_entry_get  */


/*******************************************************************************
* lineotn_dsi_rate_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns the dsi_rate associated with the specified dsi_stream
*
*
* INPUTS:
*   *lineotn_handle      - pointer to LINEOTN handle instance to be operated on
* 
*   dsi_stream           - DSI Lane to be configured.
*
* OUTPUTS:
*   *dsi_rate            - Enum for OTU frame rate. See lineotn_frame_rate_t. 
*
* RETURNS:
*   PMC_ERROR - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_dsi_rate_get (lineotn_handle_t     *lineotn_handle,
                                       UINT32                dsi_stream,
                                       lineotn_frame_rate_t *dsi_rate)
{
    
    PMC_ENTRY();

    *dsi_rate = lineotn_handle->var.lineotn_dsi_dist[dsi_stream].dsi_rate;

    PMC_RETURN(PMC_SUCCESS);

} /* lineotn_dsi_rate_get */


/*******************************************************************************
* lineotn_ctxt_reg_recover
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Recover a lineotn_recover_var_t context and energy state from register space.
*
*  This function is used by lineotn_handle_restart_init. It also recovers 
*  volatile information from lineotn_var_t context.
*  
*  This function return an error when something is incoherent in the register 
*  space. In such case, the only solution for crash restart is to reset the 
*  subsystem
* 
* INPUTS:
*  *lineotn_handle            - pointer to LINEOTN handle instance to be operated on
* 
*   is_lineotn_ss             - when TRUE, LINEOTN SS is to be cleaned up, otherwise
* 
*   top_energy_state_reg      - energy state from top-level
*   
*
* OUTPUTS:
*   *lineotn_var_reg          - A partial lineotn_var_t context + additional
*                              information to facilitate audit phase
* 
*   *lineotn_energy_state_reg - LINEOTN energy states 
*
* RETURNS:
*  PMC_SUCCESS - recovery is a success 
*  LINEOTN_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT - Communal resource energy states
*                are incoherent. DIGI layer will have to shutdown the subsystem
*  LINEOTN_ERR_CONTEXT_REG_INCOHERENT - Register context is incoherent. DIGI layer will have
*                to shutdown the subsystem
* 
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR lineotn_ctxt_reg_recover(lineotn_handle_t        *lineotn_handle,
                                           BOOL8                    is_lineotn_ss,   
                                           pmc_energy_state_t       top_energy_state_reg,         
                                           lineotn_recover_var_t   *lineotn_var_reg,              
                                           lineotn_energy_state_t  *lineotn_energy_state_reg)
{
    PMC_ERROR                    rc = PMC_SUCCESS;
    BOOL8                        rx_act[LINE_OTN_MAX_DSI_STREAMS];
    BOOL8                        tx_act[LINE_OTN_MAX_DSI_STREAMS];
    otu_frm_stream_rate_t        lane_rate[LINE_OTN_MAX_DSI_STREAMS];
    UINT8                        dci_channel[LINE_OTN_MAX_DSI_STREAMS];
    UINT32                       itr;
    UINT32                       jtr;
    UINT32                       incr;
    util_global_odukp_type_t     oduk_type;
    lineotn_frame_rate_t         dsi_rate = LAST_LINE_OTN_FRAME_RATE;
    lineotn_fec_t                fec_type[LINE_OTN_MAX_DSI_STREAMS];
    lineotn_power_mode_t         fec_pwr[LINE_OTN_MAX_DSI_STREAMS];
    BOOL8                        fec_bypassed[LINE_OTN_MAX_DSI_STREAMS];
    BOOL8                        fec_stat_enabled[LINE_OTN_MAX_DSI_STREAMS];
    BOOL8                        fec_dec_passthru[LINE_OTN_MAX_DSI_STREAMS];
    UINT32                       itr_start;
    UINT32                       itr_end;
    UINT32                       disabled_stream;
    lineotn_power_mode_t         stream_fec_pwr;
    PMC_ENTRY();

    /* 
       create a register context 
    */
    /* start from a clean context */
    lineotn_var_init(&lineotn_var_reg->var); 

    /* this information is coming from top-level register */
    lineotn_var_reg->var.lineotn_start_state = (top_energy_state_reg == PMC_ENERGY_STATE_RESET);

    /* 
       IF start_state is TRUE, the subsystem is completely in power down
       enet_register_var is completed (anyway, digi-layer should not call use with PMC_ENERGY_STATE_RESET)
    */
    if (FALSE == lineotn_var_reg->var.lineotn_start_state)
    {

        /* depending on enabled sub blocks, determine which datapaths are operational :*/
        /* fill up 
           lineotn_dsi_cfg_t       lineotn_dsi_dist[lineotn_handle->cfg.max_dsi_streams];

           lineotn_frame_rate_t    dsi_rate;          -- almost done
           BOOL                    dsi_provisioned;   -- done
           BOOL                    dsi_activated_rx;  -- almost done
           BOOL                    dsi_activated_tx;  -- almost done
           UINT8                   dci_channel;       -- done
           lineotn_fec_t           dsi_fec_type;     --- FEC var
           lineotn_power_mode_t    dsi_fec_pwr;      --- FEC var
           BOOL                    fec_bypassed;     --- FEC var
           BOOL8                   fec_stat_enabled; --- FEC var
           BOOL                    fec_dec_passthru; --- FEC var
           BOOL                    scramble_in_opsm; --- MLD var -- done
           mld_ifc_type_t          dsi_hw_ifc;        -- done
           lineotn_deskew_type_t   deskew_type;       -- done
           BOOL8                   bergen_enabled    --- MLD var   -- done
           lineotn_frame_rate_t    lineotn_sfi51_0_frame_rate;
           lineotn_frame_rate_t    lineotn_sfi51_1_frame_rate;    
           lineotn_cfg_type_t      current_lineotn_cfg;           -- don't know if i will have it 
           lineotn_fec_stat_cfg_t  fec_stat_cfg;      --- FEC var -- done
        */

        lineotn_var_reg->var.lineotn_mode = is_lineotn_ss;

        /* recover MLD lanes status */
        lineotn_mld_ctxt_reg_recover(lineotn_handle,
                                     top_energy_state_reg,         
                                     lineotn_var_reg,              
                                     lineotn_energy_state_reg);

        rc = otu_frm_channel_dsc_get(lineotn_handle->otu_frm_handle, rx_act, tx_act, lane_rate, dci_channel);

        if (PMC_SUCCESS == rc)
        {
            rc = fec_config_get(lineotn_handle,
                                is_lineotn_ss,
                                fec_type,
                                fec_pwr,
                                fec_bypassed,
                                fec_stat_enabled,
                                fec_dec_passthru,
                                lineotn_var_reg->fec_type_fuzzy);
        }

        /* retrieve FEC stat FSL table */
        if (PMC_SUCCESS == rc)
        {
            fec_stat_cfg_get(lineotn_handle, &lineotn_var_reg->var.fec_stat_cfg);
        }

        /* filled up lineotn variable from registers space read values */
        for (itr = 0; itr < lineotn_handle->cfg.max_dsi_streams && PMC_SUCCESS == rc; itr += incr)
        {
            incr = 1;
            lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_activated_rx = rx_act[itr];
            lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_activated_tx = tx_act[itr];
            lineotn_var_reg->var.lineotn_dsi_dist[itr].dci_channel = dci_channel[itr];
            lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_type = fec_type[itr];
            lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_pwr = fec_pwr[itr];
            lineotn_var_reg->var.lineotn_dsi_dist[itr].fec_bypassed = fec_bypassed[itr];
            lineotn_var_reg->var.lineotn_dsi_dist[itr].fec_stat_enabled = fec_stat_enabled[itr];
            lineotn_var_reg->var.lineotn_dsi_dist[itr].fec_dec_passthru = fec_dec_passthru[itr];

            /* interpolate DSI rate according to packet ODUK type */
            if (MAX_DCI_CHANNEL_NUM > dci_channel[itr])
            {
                /* for SYSOTN: client rate should be ODU4 */
                if (TRUE == lineotn_var_reg->var.lineotn_mode)
                {
                    otu_frm_client_rate_get(lineotn_handle->otu_frm_handle,
                                            dci_channel[itr],
                                            &oduk_type);
                }
                else
                {
                    oduk_type = UTIL_GLOBAL_ODU4;
                }
                
                rc = lineotn_oduk_type_to_frame_rate_convert(lineotn_handle,
                                                             oduk_type,
                                                             &dsi_rate,
                                                             &lineotn_var_reg->oduk_type_fuzzy[itr]);
            }
            else
            {
                dsi_rate = LAST_LINE_OTN_FRAME_RATE;
            }

            if (PMC_SUCCESS == rc && TRUE == lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_provisioned)
            {
             
                lineotn_var_reg->var.mld_curr_num_otu_configured = lineotn_var_reg->var.mld_curr_num_otu_configured | (1 << itr);
   
                lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_rate = dsi_rate;
                /* special case for SFI51*/
                if (MLD_SFI_51 == lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_hw_ifc)
                {
                    if (4 != itr)
                    {
                        lineotn_var_reg->var.lineotn_sfi51_0_frame_rate = dsi_rate;
                    }
                    else
                    {
                        lineotn_var_reg->var.lineotn_sfi51_1_frame_rate = dsi_rate;
                    }
                }                

                itr_start = itr;
                itr_end = itr;
                disabled_stream = LINE_OTN_MAX_DSI_STREAMS;
                /* if 40G or 100G: interpolate some parameters according to master stream status: 0,4,8 for 40G and 0 for 100G */
                switch (lineotn_stream_rate_get(dsi_rate))
                {
                case LINE_OTN_RATE_100G:
                    PMC_LOG_TRACE("100G datapath\n");
                    if (0 == itr)
                    {
                        itr_start = itr + 1;
                        itr_end = itr + LINE_OTN_MAX_DSI_STREAMS;
                        disabled_stream = 10;
                        if (LINE_OTN_FEC_NONE != lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_type &&
                            LAST_LINE_OTN_FEC != lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_type)
                        {
                            stream_fec_pwr = lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_pwr;
                        }
                        else
                        {
                            stream_fec_pwr =  LINE_OTN_PWR_LOWPWR;
                        }
                        incr = LINE_OTN_MAX_DSI_STREAMS;
                    }
                    break;
                case LINE_OTN_RATE_40G:
                    jtr = itr;
                    itr_start = itr + 1;
                    itr_end = itr + 4;
                    PMC_LOG_TRACE("40G stream %d %d\n", itr, lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_type);
                    if (LINE_OTN_FEC_NONE != lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_type &&
                        LAST_LINE_OTN_FEC != lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_type)
                    {
                        stream_fec_pwr = lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_pwr;
                    }
                    else
                    {
                        stream_fec_pwr =  LINE_OTN_PWR_LOWPWR;
                    }
                    incr = 4;
                    lineotn_var_reg->var.lineotn_dsi_dist[itr].scramble_in_opsm = lineotn_var_reg->var.lineotn_dsi_dist[itr + 3].scramble_in_opsm;
                    break;
                case LINE_OTN_RATE_10G:
                    break;
                default:
                    break;
                }

                /* assign 40G and 100G stream block of links status here */
                for (jtr = itr_start; jtr < itr_end; jtr++)
                {
                    if ((0 != otn_mld_field_LANE_10G_TX_SRC_get(NULL, lineotn_handle, jtr) ||
                         (MLD_SFI_51 == lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_hw_ifc))
                        || jtr >= disabled_stream ||
                        (FALSE == is_lineotn_ss && 
                         TRUE == lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_provisioned))
                    {
                        PMC_LOG_TRACE("jitr = %d itr = %d %d\n", jtr, itr,lineotn_var_reg->var.lineotn_dsi_dist[itr].dci_channel);                        
                        lineotn_var_reg->var.lineotn_dsi_dist[jtr].dsi_rate = lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_rate;
                        lineotn_var_reg->var.lineotn_dsi_dist[jtr].dsi_provisioned = lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_provisioned;
                        lineotn_var_reg->var.lineotn_dsi_dist[jtr].dsi_activated_rx = lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_activated_rx;
                        lineotn_var_reg->var.lineotn_dsi_dist[jtr].dsi_activated_tx = lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_activated_tx;
                        lineotn_var_reg->var.lineotn_dsi_dist[jtr].dsi_hw_ifc = lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_hw_ifc;
                        lineotn_var_reg->var.lineotn_dsi_dist[jtr].deskew_type = lineotn_var_reg->var.lineotn_dsi_dist[itr].deskew_type;
                        lineotn_var_reg->var.lineotn_dsi_dist[jtr].dci_channel = lineotn_var_reg->var.lineotn_dsi_dist[itr].dci_channel;

                        if (LINE_OTN_FEC_NONE != lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_type &&
                            LAST_LINE_OTN_FEC != lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_type)
                        {                            
                            lineotn_var_reg->var.lineotn_dsi_dist[jtr].dsi_fec_type = fec_type[itr];
                            lineotn_var_reg->var.lineotn_dsi_dist[jtr].dsi_fec_pwr = fec_pwr[jtr];
                            lineotn_var_reg->var.lineotn_dsi_dist[jtr].fec_bypassed = fec_bypassed[jtr];
                            lineotn_var_reg->var.lineotn_dsi_dist[jtr].fec_dec_passthru = fec_dec_passthru[jtr];
                        }
                        else
                        {                            
                            lineotn_var_reg->var.lineotn_dsi_dist[jtr].dsi_fec_type = fec_type[itr];
                            lineotn_var_reg->var.lineotn_dsi_dist[jtr].dsi_fec_pwr = stream_fec_pwr;
                            lineotn_var_reg->var.lineotn_dsi_dist[jtr].fec_bypassed = fec_bypassed[itr];
                            lineotn_var_reg->var.lineotn_dsi_dist[jtr].fec_dec_passthru = fec_dec_passthru[itr];
                        }
                        
                        lineotn_var_reg->var.lineotn_dsi_dist[jtr].scramble_in_opsm = lineotn_var_reg->var.lineotn_dsi_dist[itr].scramble_in_opsm;
                        lineotn_var_reg->var.lineotn_dsi_dist[jtr].fec_stat_enabled = fec_stat_enabled[jtr];
                        lineotn_var_reg->var.mld_curr_num_otu_configured = lineotn_var_reg->var.mld_curr_num_otu_configured | (1 << jtr);

                        if (disabled_stream <= jtr && 0 != otn_mld_field_LANE_10G_TX_SRC_get(NULL, lineotn_handle, jtr))
                        {
                            if (PMC_SUCCESS != rc)
                            {
                                rc = LINEOTN_ERR_CONTEXT_REG_INCOHERENT;
                            }
                        }
                    }
                }                
            }
            else
            {
                /* erase set parameters if stream status retrieving failed */
                lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_provisioned  = FALSE;
                lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_activated_rx = FALSE;
                lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_activated_tx = FALSE;
                lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_rate         = LAST_LINE_OTN_FRAME_RATE;
                rc = PMC_SUCCESS;
            }  
        }

        if (PMC_SUCCESS == rc && 
            TRUE == lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_provisioned)
        {
            if (MLD_SFI_51 == lineotn_var_reg->var.lineotn_dsi_dist[0].dsi_hw_ifc)
            {
                lineotn_var_reg->var.lineotn_sfi51_0_frame_rate = lineotn_var_reg->var.lineotn_dsi_dist[0].dsi_rate;
            }
            if (MLD_SFI_51 == lineotn_var_reg->var.lineotn_dsi_dist[8].dsi_hw_ifc)
            {
                lineotn_var_reg->var.lineotn_sfi51_0_frame_rate = lineotn_var_reg->var.lineotn_dsi_dist[8].dsi_rate;
            }
            if (MLD_SFI_51 == lineotn_var_reg->var.lineotn_dsi_dist[4].dsi_hw_ifc)
            {
                lineotn_var_reg->var.lineotn_sfi51_1_frame_rate = lineotn_var_reg->var.lineotn_dsi_dist[4].dsi_rate;
            }
        }

        /* retrieve oduksc scbs3 calendar entries */
        scbs3_page_get(lineotn_handle->otu_frm_handle->oduksc_handle->scbs3_handle, 
                       SCBS3_ACTIVE_MEM, 
                       &lineotn_var_reg->oduksc_scbs3_page);
        
    }
    
    PMC_RETURN(rc);
} /* lineotn_ctxt_reg_recover */


/*******************************************************************************
* lineotn_ctxt_audit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Audit memory context with recovered register context. This function 
*   determines if a cleanup is required
*   
*   A clean context which represents the intersection between both context is also 
*   generated.
*
* INPUTS:
*   *lineotn_handle        - pointer to LINEOTN handle instance to be operated on
* 
*    is_lineotn_ss         - when TRUE, LINEOTN SS is to be cleaned up, otherwise 
*                            it is a SYSOTN SS.
* 
*   *lineotn_var_reg       - A partial lineotn_var_t context + additional
*                           information to facilitate audit phase
* 
*   *lineotn_energy_state_reg - LINEOTN energy states 
*
* OUTPUTS:
*   *lineotn_var_clean     - A clean context which represents the intersection
*                           between both context is also generated.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - Memory & register context matches
*   LINEOTN_ERR_CONTEXT_CLEANUP - A cleanup is required.
* 
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR lineotn_ctxt_audit(lineotn_handle_t        *lineotn_handle,
                                     BOOL8                    is_lineotn_ss,   
                                     lineotn_recover_var_t   *lineotn_var_reg,
                                     lineotn_energy_state_t  *lineotn_energy_state_reg,
                                     lineotn_var_t           *lineotn_var_clean)
{
    PMC_ERROR rc = PMC_SUCCESS;  
    PMC_ERROR loop_rc = PMC_SUCCESS;
    UINT32    itr;
    UINT32    jtr;
    UINT32    incr;
    UINT32    entry_itr;
    UINT32    num_of_recs;
    BOOL8     is_fsl_tbl_empty = TRUE;
    BOOL8     chnl_act;

    PMC_ENTRY();
    
    /* init the new context  */
    lineotn_var_init(lineotn_var_clean);

    lineotn_var_clean->lineotn_mode = lineotn_handle->var.lineotn_mode;
    lineotn_var_clean->mld_curr_num_otu_configured = lineotn_var_reg->var.mld_curr_num_otu_configured;

    /* audit energy state */
    if (lineotn_var_reg->var.lineotn_start_state != lineotn_start_state_test(lineotn_handle))
    {       
        lineotn_var_clean->lineotn_start_state = TRUE;
        rc = LINEOTN_ERR_CONTEXT_CLEANUP;        
        PMC_LOG_TRACE("step 0 rc = %x %d %d\n", rc, lineotn_var_reg->var.lineotn_start_state, lineotn_start_state_test(lineotn_handle));
    } 
    else if (lineotn_var_reg->var.lineotn_start_state == FALSE) 
    {      
        if (PMC_SUCCESS == rc)
        {
            /* audit each DSI stream parameters */
            for (itr = 0; itr < lineotn_handle->cfg.max_dsi_streams; itr += incr)
            {
                loop_rc = PMC_SUCCESS;   
                incr = 1;
                chnl_act = FALSE;

                /* check DSI stream RX path status */
                if (PMC_SUCCESS == loop_rc)
                {
                    if (lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_activated_rx == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_activated_rx)
                    {
                        lineotn_var_clean->lineotn_dsi_dist[itr].dsi_activated_rx = lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_activated_rx;
                    }
                    else
                    {
                        loop_rc = LINEOTN_ERR_CONTEXT_CLEANUP;   
                        PMC_LOG_TRACE("step 1 rc = %x %d %d\n", loop_rc, lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_activated_rx, lineotn_handle->var.lineotn_dsi_dist[itr].dsi_activated_rx);                      
                    }
                }
            
                /* check DSI stream TX path status */
                if (PMC_SUCCESS == loop_rc)
                {
                    if (lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_activated_tx == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_activated_tx)
                    {
                        lineotn_var_clean->lineotn_dsi_dist[itr].dsi_activated_tx = lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_activated_tx;
                    }
                    else
                    {
                        loop_rc = LINEOTN_ERR_CONTEXT_CLEANUP;     
                        PMC_LOG_TRACE("step 2 rc = %x %d %d\n", loop_rc, lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_activated_tx, lineotn_handle->var.lineotn_dsi_dist[itr].dsi_activated_tx);                           
                    }
                }
                /* DCI channel and DSI rate could mismatch if TX and TX channels are not activated */
                if (PMC_SUCCESS == rc)
                {
                    chnl_act = (TRUE == lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_activated_rx ||
                                TRUE == lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_activated_tx);
                }

                /* audit DCI channel */
                if (lineotn_var_reg->var.lineotn_dsi_dist[itr].dci_channel == lineotn_handle->var.lineotn_dsi_dist[itr].dci_channel)
                {
                    lineotn_var_clean->lineotn_dsi_dist[itr].dci_channel = lineotn_var_reg->var.lineotn_dsi_dist[itr].dci_channel;
                }
                else
                {
                    if (TRUE == chnl_act)
                    {
                        loop_rc = LINEOTN_ERR_CONTEXT_CLEANUP;     
                        PMC_LOG_TRACE("step 3 dsi_stream %d rc = %x %d %d\n", itr, loop_rc, lineotn_var_reg->var.lineotn_dsi_dist[itr].dci_channel, lineotn_handle->var.lineotn_dsi_dist[itr].dci_channel);                    
                    }
                    else
                    {
                        lineotn_var_reg->var.lineotn_dsi_dist[itr].dci_channel = lineotn_handle->var.lineotn_dsi_dist[itr].dci_channel;
                    }
                }

                /* audit hardware interface type */
                if (PMC_SUCCESS == loop_rc)
                {
                    if (lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_hw_ifc == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_hw_ifc)
                    {
                        lineotn_var_clean->lineotn_dsi_dist[itr].dsi_hw_ifc = lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_hw_ifc;
                    }
                    else
                    {
                        loop_rc = LINEOTN_ERR_CONTEXT_CLEANUP;     
                        PMC_LOG_TRACE("step 4 dsi_stream %d rc = %x %d %d\n", itr, loop_rc,
                                      lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_hw_ifc, lineotn_handle->var.lineotn_dsi_dist[itr].dsi_hw_ifc);                    
                    }
                }
            
                /* audit stream dsi rate */
                if (PMC_SUCCESS == loop_rc)
                {
                    if (lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_rate == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_rate)
                    {
                        lineotn_var_clean->lineotn_dsi_dist[itr].dsi_rate = lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_rate;
                    }
                    else
                    {
                        /* check fuzzy */
                        if ((TRUE == lineotn_handle->var.lineotn_mode)
                            && (TRUE == lineotn_var_reg->oduk_type_fuzzy[itr]))
                        {                                                                            
                        }
                        else
                        {
                            if (TRUE == chnl_act)
                            {
                                loop_rc = LINEOTN_ERR_CONTEXT_CLEANUP;  
                                PMC_LOG_TRACE("step 6 dsi_stream = %d  rc = %x %d %d\n", itr, loop_rc,lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_rate, lineotn_handle->var.lineotn_dsi_dist[itr].dsi_rate);                       
                            }
                            else
                            {
                                lineotn_var_clean->lineotn_dsi_dist[itr].dsi_rate = lineotn_handle->var.lineotn_dsi_dist[itr].dsi_rate; 
                            }
                        }
                    }
                }

                /* check DSI stream provisioning state */
                if (PMC_SUCCESS == loop_rc)
                {
                    if (lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_provisioned == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_provisioned)
                    {
                        lineotn_var_clean->lineotn_dsi_dist[itr].dsi_provisioned = lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_provisioned;
                    }
                    else
                    {
                        loop_rc = LINEOTN_ERR_CONTEXT_CLEANUP;    
                        PMC_LOG_TRACE("step 7 stream = %d rc = %x %d %d\n", itr, loop_rc, lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_provisioned, lineotn_handle->var.lineotn_dsi_dist[itr].dsi_provisioned);                     
                    }
                }

                /* check DSI stream deskew type */
                if (PMC_SUCCESS == loop_rc)
                {
                    if (lineotn_var_reg->var.lineotn_dsi_dist[itr].deskew_type == lineotn_handle->var.lineotn_dsi_dist[itr].deskew_type ||
                        (LAST_LINE_OTN_DESKEW_TYPE == lineotn_var_reg->var.lineotn_dsi_dist[itr].deskew_type &&
                         LINE_OTN_NO_DESKEW == lineotn_handle->var.lineotn_dsi_dist[itr].deskew_type))
                    {
                        lineotn_var_clean->lineotn_dsi_dist[itr].deskew_type = lineotn_handle->var.lineotn_dsi_dist[itr].deskew_type;
                    }
                    else
                    {
                        loop_rc = LINEOTN_ERR_CONTEXT_CLEANUP;  
                        PMC_LOG_TRACE("step 8 stream = %d rc = %x %d %d\n", itr, loop_rc, lineotn_var_reg->var.lineotn_dsi_dist[itr].deskew_type, lineotn_handle->var.lineotn_dsi_dist[itr].deskew_type);                              
                    }
                }

                /* check fec type */
                if (PMC_SUCCESS == loop_rc)
                {
                    if (lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_type == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_type)
                    {
                        lineotn_var_clean->lineotn_dsi_dist[itr].dsi_fec_type = lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_type;
                    }
                    else
                    {
                        if (TRUE == lineotn_handle->var.lineotn_mode && 
                            TRUE == lineotn_var_reg->fec_type_fuzzy[itr])
                        {
                            if ((LINE_OTN_FEC_I4 == lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_type || 
                                 LINE_OTN_FEC_I7 == lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_type ||
                                 LINE_OTN_FEC_G709 == lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_type) &&
                                (LINE_OTN_FEC_NONE == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_type ||
                                 LAST_LINE_OTN_FEC == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_type))
                            {
                                lineotn_var_clean->lineotn_dsi_dist[itr].dsi_fec_type = LINE_OTN_FEC_NONE;
                            }
                            else
                            {


                                loop_rc = LINEOTN_ERR_CONTEXT_CLEANUP; 
                                PMC_LOG_TRACE("step 11 stream = %d  rc = %x %d %d\n", itr, loop_rc, lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_type, lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_type);
                            }
                        }                    
                        else
                        {
                            if (FALSE == (((LINE_OTN_FEC_NONE == lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_type) 
                                           || (LAST_LINE_OTN_FEC == lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_type)) &&
                                          ((LINE_OTN_FEC_NONE == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_type) 
                                           || (LAST_LINE_OTN_FEC == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_type))))                
                            {
                                /* also check fec decoder status */
                                if (FALSE == fec_dec_status_get(lineotn_handle, itr, lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_type) &&
                                    ((LINE_OTN_FEC_NONE == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_type) 
                                     || (LAST_LINE_OTN_FEC == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_type)))
                                {
                                    lineotn_var_clean->lineotn_dsi_dist[itr].dsi_fec_type = lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_type;
                                }
                                else
                                {
                                    loop_rc = LINEOTN_ERR_CONTEXT_CLEANUP;
                                    PMC_LOG_TRACE("step 12 dsi_stream = %d rc = %x %d %d\n", itr, loop_rc, lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_type, lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_type);        
                                }
                            }
                        }
                    }
                }

                /* audit fec power mode */
                if (PMC_SUCCESS == loop_rc)
                {
                    if (lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_pwr == lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_pwr)
                    {
                        lineotn_var_clean->lineotn_dsi_dist[itr].dsi_fec_pwr = lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_pwr;
                    }
                    else
                    {
                        if (LINE_OTN_FEC_NONE != lineotn_var_clean->lineotn_dsi_dist[itr].dsi_fec_type &&
                            LAST_LINE_OTN_FEC != lineotn_var_clean->lineotn_dsi_dist[itr].dsi_fec_type )
                        {
                            loop_rc = LINEOTN_ERR_CONTEXT_CLEANUP;
                            PMC_LOG_TRACE("step 13 itr = %d rc = %x %d %d\n", itr, loop_rc, lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_fec_pwr, lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_pwr);
                        }
                        else
                        {
                            lineotn_var_clean->lineotn_dsi_dist[itr].dsi_fec_pwr = lineotn_handle->var.lineotn_dsi_dist[itr].dsi_fec_pwr;
                        }
                    }
                }

                /* audit fec bypass state */
                if (PMC_SUCCESS == loop_rc)
                {
                    if (lineotn_var_reg->var.lineotn_dsi_dist[itr].fec_bypassed == lineotn_handle->var.lineotn_dsi_dist[itr].fec_bypassed)
                    {
                        lineotn_var_clean->lineotn_dsi_dist[itr].fec_bypassed = lineotn_var_reg->var.lineotn_dsi_dist[itr].fec_bypassed;
                    }
                    else
                    {
                        loop_rc = LINEOTN_ERR_CONTEXT_CLEANUP; 
                        PMC_LOG_TRACE("step 14 stream = %d rc = %x %d %d\n", itr,  loop_rc, lineotn_var_reg->var.lineotn_dsi_dist[itr].fec_bypassed, lineotn_handle->var.lineotn_dsi_dist[itr].fec_bypassed);                            
                    }
                }

                /* audit fec decoder status */
                if (PMC_SUCCESS == loop_rc)
                {
                    if (lineotn_var_reg->var.lineotn_dsi_dist[itr].fec_dec_passthru == lineotn_handle->var.lineotn_dsi_dist[itr].fec_dec_passthru)
                    {
                        lineotn_var_clean->lineotn_dsi_dist[itr].fec_dec_passthru = lineotn_var_reg->var.lineotn_dsi_dist[itr].fec_dec_passthru;
                    }
                    else
                    {
                        if ( LINE_OTN_FEC_NONE != lineotn_var_clean->lineotn_dsi_dist[itr].dsi_fec_type &&
                             LAST_LINE_OTN_FEC != lineotn_var_clean->lineotn_dsi_dist[itr].dsi_fec_type )
                        {
                            loop_rc = LINEOTN_ERR_CONTEXT_CLEANUP;
                            PMC_LOG_TRACE("step 15 dsi_stream = %d rc = %x dsi stream = %d %d %d\n", itr, loop_rc, itr, lineotn_var_reg->var.lineotn_dsi_dist[itr].fec_dec_passthru, lineotn_handle->var.lineotn_dsi_dist[itr].fec_dec_passthru);
                        }
                        else
                        { 
                            lineotn_var_clean->lineotn_dsi_dist[itr].fec_dec_passthru = lineotn_handle->var.lineotn_dsi_dist[itr].fec_dec_passthru;                            
                        }
                    }
                }

                /* audit BERGEN state */
                if (PMC_SUCCESS == loop_rc)
                {
                    if (lineotn_var_reg->var.lineotn_dsi_dist[itr].bergen_enabled == lineotn_handle->var.lineotn_dsi_dist[itr].bergen_enabled)
                    {
                        lineotn_var_clean->lineotn_dsi_dist[itr].bergen_enabled = lineotn_var_reg->var.lineotn_dsi_dist[itr].bergen_enabled;
                    }
                    else
                    {
                        loop_rc = LINEOTN_ERR_CONTEXT_CLEANUP; 
                        PMC_LOG_TRACE("step 16 stream = %d rc = %x %d %d\n", itr, loop_rc, lineotn_var_reg->var.lineotn_dsi_dist[itr].bergen_enabled, lineotn_handle->var.lineotn_dsi_dist[itr].bergen_enabled);  
                    }
                }

                /* audit scrambler state */
                if (PMC_SUCCESS == loop_rc)
                {
                    if (lineotn_var_reg->var.lineotn_dsi_dist[itr].scramble_in_opsm == lineotn_handle->var.lineotn_dsi_dist[itr].scramble_in_opsm)
                    {
                        lineotn_var_clean->lineotn_dsi_dist[itr].scramble_in_opsm = lineotn_var_reg->var.lineotn_dsi_dist[itr].scramble_in_opsm;
                    }
                    else
                    {
                        loop_rc = LINEOTN_ERR_CONTEXT_CLEANUP; 
                        PMC_LOG_TRACE("step 17 dsi_stream = %d rc = %x %d %d\n", itr, loop_rc, lineotn_var_reg->var.lineotn_dsi_dist[itr].scramble_in_opsm, lineotn_handle->var.lineotn_dsi_dist[itr].scramble_in_opsm);                            
                    }
                }

                /* audit OTU framer database */
                if (PMC_SUCCESS == loop_rc)
                {
                    /* compute number of entries */
                    if (FREE_CHANNEL != lineotn_var_reg->var.lineotn_dsi_dist[itr].dci_channel &&
                        TRUE == lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_activated_rx)
                    {
                        loop_rc =  otu_frm_db_dci_chnl_num_entries_get(lineotn_handle->otu_frm_handle, 
                                                                       lineotn_var_reg->var.lineotn_dsi_dist[itr].dci_channel,
                                                                       TRUE,
                                                                       &num_of_recs);        
                        /* if (PMC_SUCCESS == loop_rc) */
                        {   
                            loop_rc |=  otu_frm_db_dci_chnl_num_entries_get(lineotn_handle->otu_frm_handle, 
                                                                           lineotn_var_reg->var.lineotn_dsi_dist[itr].dci_channel,
                                                                           FALSE,
                                                                           &num_of_recs);
                        }

                        if (PMC_SUCCESS != loop_rc)
                        {
                            PMC_LOG_TRACE("DB issue stream = %d\n", itr);
                            loop_rc = LINEOTN_ERR_CONTEXT_CLEANUP; 
                        }
                    }
                }

                /* audit lineotn ODUK switch scbs3 entries */
                if (PMC_SUCCESS == loop_rc  && 
                    TRUE == lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_provisioned && 
                    FREE_CHANNEL != lineotn_var_reg->var.lineotn_dsi_dist[itr].dci_channel)
                {
                    loop_rc = otu_frm_db_chnl_entry_audit(lineotn_handle->otu_frm_handle, 
                                                          &lineotn_var_reg->oduksc_scbs3_page, 
                                                          lineotn_var_reg->var.lineotn_dsi_dist[itr].dci_channel);
                    if (PMC_SUCCESS != loop_rc)
                    {
                        if (loop_rc == OTU_FRM_ERR_CONTEXT_CLEANUP)
                        {
                            loop_rc = LINEOTN_ERR_CONTEXT_CLEANUP;
                        }
                        
                        PMC_LOG_TRACE("step 22 otu_frm_db_chnl_entry_audit failed -> rc = %x\n", loop_rc);  
                    }
                } 
                 
                if(PMC_SUCCESS != loop_rc)
                {
                    /* cleanup link and */
                    /* discard 40G group of dsi_lanes */
                    switch (lineotn_stream_rate_get(lineotn_var_reg->var.lineotn_dsi_dist[itr].dsi_rate))
                    {
                    case LINE_OTN_RATE_40G:
                        lineotn_var_clean->mld_curr_num_otu_configured &= (~(0xF << itr));
                        incr = 4;
                        break;
                    case LINE_OTN_RATE_100G:
                        /* lineotn_var_clean->mld_curr_num_otu_configured bit 
                           31-30 was re-used for sysotn interrupt type, mask 
                           0xC0000000 to ignore bit 31-30 */
                        lineotn_var_clean->mld_curr_num_otu_configured &= SYSOTN_INT_TYPE_STORAGE_MASK;
                        incr = LINE_OTN_MAX_DSI_STREAMS;
                        break;
                    default:
                        lineotn_var_clean->mld_curr_num_otu_configured &= (~(1 << itr));
                        break;
                    }
                  
                    
                    for (jtr = itr; jtr < (itr + incr); jtr++)
                    {                                
                        /* cleanup link */
                        lineotn_stream_ctxt_init(&lineotn_var_clean->lineotn_dsi_dist[jtr]);
                    } 
                }
            
                if (PMC_SUCCESS == rc)
                {
                    rc = loop_rc;
                }
            }    

            if (TRUE == lineotn_handle->var.lineotn_mode)
            {
                /* audit SFI5 slice 0 interface frame rate */
                if (lineotn_var_reg->var.lineotn_sfi51_0_frame_rate == lineotn_handle->var.lineotn_sfi51_0_frame_rate)
                {
                    lineotn_var_clean->lineotn_sfi51_0_frame_rate = lineotn_var_reg->var.lineotn_sfi51_0_frame_rate;
                }
                else
                {    
                    lineotn_var_clean->lineotn_sfi51_0_frame_rate = LAST_LINE_OTN_FRAME_RATE;
                    for (jtr = 0; jtr < 4; jtr++)
                    {                                
                        /* cleanup link */
                        lineotn_stream_ctxt_init(&lineotn_var_clean->lineotn_dsi_dist[jtr]);
                    } 
                    if (PMC_SUCCESS == rc)
                    {
                        
                        rc = LINEOTN_ERR_CONTEXT_CLEANUP;
                        PMC_LOG_TRACE("step 18 rc = %x\n", loop_rc);
                    }
                }
                
                /* audit SFI5 slice 1 interface frame rate */
                
                if (lineotn_var_reg->var.lineotn_sfi51_1_frame_rate == lineotn_handle->var.lineotn_sfi51_1_frame_rate)
                {
                    lineotn_var_clean->lineotn_sfi51_1_frame_rate = lineotn_var_reg->var.lineotn_sfi51_1_frame_rate;
                }
                else
                {
                    lineotn_var_clean->lineotn_sfi51_1_frame_rate = LAST_LINE_OTN_FRAME_RATE;
                    for (jtr = 4; jtr < 8; jtr++)
                    {                                
                        /* cleanup link */
                        lineotn_stream_ctxt_init(&lineotn_var_clean->lineotn_dsi_dist[jtr]);
                    } 
                    if (PMC_SUCCESS == rc)
                    {
                        rc = LINEOTN_ERR_CONTEXT_CLEANUP;
                    PMC_LOG_TRACE("step 19 rc = %x\n", loop_rc);                          
                    }    
                }
            }

            loop_rc = rc;
            /* audit FEC stat table */
            for (entry_itr = 0; 
                 entry_itr < LINEOTN_FEC_STAT_MAX_FSL_TBL_ENTRIES && 
                     PMC_SUCCESS == loop_rc &&
                     ((TRUE == lineotn_var_reg->var.fec_stat_cfg.enable) ||
                      (TRUE == lineotn_handle->var.fec_stat_cfg.enable)); 
                 entry_itr++)
            {
                if (TRUE == lineotn_handle->var.fec_stat_cfg.enable)
                {
                    if (1 == lineotn_var_reg->var.fec_stat_cfg.fsl_table[entry_itr] &&
                        LINEOTN_FEC_STAT_NULL_ENTRY != lineotn_handle->var.fec_stat_cfg.fsl_table[entry_itr] &&
                        lineotn_handle->cfg.max_dsi_streams > lineotn_handle->var.fec_stat_cfg.fsl_table[entry_itr] &&
                        TRUE == lineotn_handle->var.lineotn_dsi_dist[lineotn_handle->var.fec_stat_cfg.fsl_table[entry_itr]].dsi_provisioned)
                    {
                        PMC_LOG_TRACE("about to audit fec_stat entry = %d\n", entry_itr);
                        lineotn_var_clean->fec_stat_cfg.fsl_table[entry_itr] = lineotn_handle->var.fec_stat_cfg.fsl_table[entry_itr];
                        is_fsl_tbl_empty = FALSE;
                    }
                    else
                    {    
                        lineotn_var_clean->fec_stat_cfg.fsl_table[entry_itr] = LINEOTN_FEC_STAT_NULL_ENTRY;

                        if ((LINEOTN_FEC_STAT_NULL_ENTRY != lineotn_var_reg->var.fec_stat_cfg.fsl_table[entry_itr]))
                        {   
                            loop_rc = LINEOTN_ERR_CONTEXT_CLEANUP;
                            PMC_LOG_TRACE("step 21 rc = %x entry_itr = %d\n", loop_rc, entry_itr); 
                        }
                    }
                }
                else
                {               
                    if (LINEOTN_FEC_STAT_NULL_ENTRY != lineotn_var_reg->var.fec_stat_cfg.fsl_table[entry_itr] ||
                        LINEOTN_FEC_STAT_NULL_ENTRY != lineotn_handle->var.fec_stat_cfg.fsl_table[entry_itr])
                    {                   
                        loop_rc = LINEOTN_ERR_CONTEXT_CLEANUP;
                        PMC_LOG_TRACE("step 21-1 rc = %x\n", loop_rc);                     
                    } 
                    lineotn_var_clean->fec_stat_cfg.fsl_table[entry_itr] = LINEOTN_FEC_STAT_NULL_ENTRY;
                }

                if (PMC_SUCCESS == rc)
                {
                    rc = loop_rc;
                }
            }
            if (PMC_SUCCESS == rc)
            {
                if (TRUE != is_fsl_tbl_empty)
                {
                    
                    /* check fec stat status: enables or disabled */
                    if (lineotn_var_reg->var.fec_stat_cfg.enable != lineotn_handle->var.fec_stat_cfg.enable)
                    {
                        rc = LINEOTN_ERR_CONTEXT_CLEANUP;
                        PMC_LOG_TRACE("FEC STAT error %d %d\n", lineotn_var_reg->var.fec_stat_cfg.enable, lineotn_handle->var.fec_stat_cfg.enable);
                    }
                    else
                    {
                        lineotn_var_clean->fec_stat_cfg.enable = lineotn_var_reg->var.fec_stat_cfg.enable;
                    }
                }
                else
                {
                    lineotn_var_clean->fec_stat_cfg.enable = lineotn_handle->var.fec_stat_cfg.enable;
                }
            }
        }        
    }
    
    PMC_RETURN(rc);
} /* lineotn_ctxt_audit */


/*******************************************************************************
*  lineotn_ctxt_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reset and unconfigure LINEOTN and its sub-block.
*
* INPUTS:
*   *lineotn_handle           - handle to the Lineotn instance to be operated on containing 
*                               variable configuration retrieved from saved context
* 
*    is_lineotn_ss            - when TRUE, LINEOTN SS is to be cleaned up, otherwise 
*                               it is a SYSOTN SS.
* 
*   *lineotn_var_reg          - LINEOTN variable configuration recovered from device registers
* 
*   *lineotn_energy_state_reg - LINEOTN energy state
*
* OUTPUTS:
*   *lineotn_var_clean        - new LINEOTN variable configuration that matches device state.
*
* RETURNS:
*   PMC_SUCCESS - if operation is successfully executed, 
*                 otherwise a descriptive error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR lineotn_ctxt_cleanup(lineotn_handle_t         *lineotn_handle,
                                       BOOL8                     is_lineotn_ss,
                                       lineotn_recover_var_t    *lineotn_var_reg,
                                       lineotn_energy_state_t   *lineotn_energy_state_reg,
                                       lineotn_var_t            *lineotn_var_clean)
{
    PMC_ERROR          rc = PMC_SUCCESS;
    UINT32             num_chnl_to_clean = 0;
    UINT32             chnl_to_clean[LINE_OTN_MAX_DSI_STREAMS];
    UINT32             chnl_itr;
    UINT32 jtr;

    PMC_ENTRY();

    PMC_LOG_TRACE("about to clean up lineotn\n");

    PMC_MEMSET(chnl_to_clean, 0, sizeof(UINT32) * LINE_OTN_MAX_DSI_STREAMS);

    /* Perform register cleanup and sub-blocks memory context cleanup */
    if (lineotn_start_state_test(lineotn_handle) == FALSE)
    {
        /* build list of dsi streams that should be cleanup */
        num_chnl_to_clean = 0;
        for (chnl_itr = 0; chnl_itr  < lineotn_handle->cfg.max_dsi_streams;  chnl_itr += 1)
        {
            if (FALSE == lineotn_var_clean->lineotn_dsi_dist[chnl_itr].dsi_activated_rx ||
                FALSE == lineotn_var_clean->lineotn_dsi_dist[chnl_itr].dsi_activated_tx) 
            {   
                PMC_LOG_TRACE("LINEOTN dsi_stream %d needs to be cleanup\n", chnl_itr);
                chnl_to_clean[num_chnl_to_clean] = chnl_itr;
                num_chnl_to_clean +=1;
                /* if OTU 2-3*/
                if ((0 != (chnl_itr % 4)) && LINE_OTN_RATE_40G == lineotn_stream_rate_get(lineotn_var_clean->lineotn_dsi_dist[(chnl_itr / 4) * 4].dsi_rate))
                {                    
                    for (jtr = 0; jtr < num_chnl_to_clean; jtr++)
                    {
                        if (chnl_to_clean[jtr] == ((chnl_itr / 4) * 4))
                        {                            
                            chnl_to_clean[num_chnl_to_clean] = chnl_itr;
                            num_chnl_to_clean += 1;
                            PMC_LOG_TRACE("cleanup chnl = %d\n", chnl_itr);
                            break;
                        }
                    }
                }

                if ((0 != (chnl_itr % 10)) && LINE_OTN_RATE_100G == lineotn_stream_rate_get(lineotn_var_clean->lineotn_dsi_dist[(chnl_itr / 10) * 10].dsi_rate))
                {
                    for (jtr = 0; jtr < num_chnl_to_clean; jtr++)
                    {
                        if (chnl_to_clean[jtr] == ((chnl_itr / 10) * 10))
                        {                            
                            chnl_to_clean[num_chnl_to_clean] = chnl_itr;
                            num_chnl_to_clean += 1;
                            PMC_LOG_TRACE("cleanup chnl = %d\n", chnl_itr);
                            break;
                        }
                    }
                }
            }  
        }

        if (PMC_SUCCESS == rc)
        {
            lineotn_var_clean->current_lineotn_cfg = lineotn_handle->var.current_lineotn_cfg; 
            if(12 == num_chnl_to_clean)
            {
                lineotn_var_clean->current_lineotn_cfg = LAST_LINE_OTN_CFG;
            }    
        }
   
        /* the old lineotn var context is incoherent, we use the new one */
        PMC_MEMCPY(&lineotn_handle->var, lineotn_var_clean, sizeof(lineotn_var_t));
        
        lineotn_top_level_interrupt_recover(lineotn_handle);
        
        /* Cleanup MLD, FEC and OTU_FRM registers space */
        lineotn_mld_reg_cleanup(lineotn_handle, num_chnl_to_clean, chnl_to_clean);
        lineotn_fec_reg_cleanup(lineotn_handle, num_chnl_to_clean, chnl_to_clean);
        otu_frm_reg_cleanup(lineotn_handle->otu_frm_handle, num_chnl_to_clean, chnl_to_clean);
    }

    PMC_LOG_TRACE("number of cleaned streams: %d\n", num_chnl_to_clean);

    PMC_RETURN(rc);
    
} /* lineotn_ctxt_cleanup */


/*******************************************************************************
*  lineotn_stream_rate_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Convert type lineotn_frame_rate_t to lineotn_chnl_rate_t.
*
* INPUTS:
*   dsi_rate                  - frame rate in lineotn_frame_rate_t
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   lineotn_chnl_rate_t - frame rate in lineotn_chnl_rate_t format.
*
* NOTES:
*
*******************************************************************************/
PUBLIC lineotn_chnl_rate_t lineotn_stream_rate_get(lineotn_frame_rate_t dsi_rate)
{
    lineotn_chnl_rate_t chnl_rate = LAST_LINE_OTN_RATE;

    PMC_ENTRY();

    if (4 == lineotn_frame_rate_otuk_num_get(dsi_rate) )
    {
        chnl_rate = LINE_OTN_RATE_100G;
    }
    else if (3 == lineotn_frame_rate_otuk_num_get(dsi_rate) )
    {  
        chnl_rate = LINE_OTN_RATE_40G;
    }
    else if (2 == lineotn_frame_rate_otuk_num_get(dsi_rate) )
    {  
        chnl_rate = LINE_OTN_RATE_10G;
    }

    PMC_RETURN(chnl_rate);
}/* lineotn_stream_rate_get */


/*******************************************************************************
* lineotn_prov_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Compare given state against RX or TX stream state. Only operational state is 
*    processed.
*
* INPUTS:
*    h                      - pointer to LINEOTN handle instance
* 
*    dsi_stream             - channel ID to be configured Valid range: 0 to 11
* 
*    rx_chnl                - when TRUE query concerns RX channel state, 
*                             otherwise TX state is returned
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - when operation is successful otherwise a descriptive error 
*   code is returned.
*
* NOTES: 
*
*******************************************************************************/
PUBLIC BOOL8 lineotn_prov_get(lineotn_handle_t         *h,
                              UINT32                    dsi_stream,
                              BOOL8                     rx_chnl)
{
    BOOL8 rc = FALSE;

    PMC_ENTRY();

    PMC_ASSERT(h != NULL, LINEOTN_ERR_INVALID_PTR, 0, 0);

    if (LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream && MLD_SFI_51 == h->var.lineotn_dsi_dist[4].dsi_hw_ifc)
    {
        dsi_stream = 4;
    }

    /* Check that link number is less than 12*/
    if (dsi_stream < h->cfg.max_dsi_streams && 
        dsi_stream < LINE_OTN_MAX_DSI_STREAMS) 
    {

        if (TRUE == rx_chnl)
        {            
            rc = (TRUE == h->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned &&
                  TRUE == h->var.lineotn_dsi_dist[dsi_stream].dsi_activated_rx);
        }
        else
        {  
            rc = (TRUE == h->var.lineotn_dsi_dist[dsi_stream].dsi_provisioned &&
                  TRUE == h->var.lineotn_dsi_dist[dsi_stream].dsi_activated_tx);
        }
    }

    PMC_RETURN(rc);
}/* lineotn_prov_get */


/*******************************************************************************
* lineotn_adjust_dci_clock
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Performs MPMO/ODUKSC adjustments given an integer numerator and
*   denominator multiplier on the DCI clock frequency.
*                                                                              
* INPUTS:                                                                       
*   *lineotn_handle     - pointer to LINEOTN handle instance to be operated on
* 
*   num_mult            - numerator to be applied to the DCI clock frequency
* 
*   denom_mult          - denominator to be applied to the DCI clock frequency   
*                                                                               
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.                                   
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/


PUBLIC PMC_ERROR lineotn_adjust_dci_clock(lineotn_handle_t *lineotn_handle,
                                          UINT32 num_mult,
                                          UINT32 denom_mult)
{
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();
    
    if(result == PMC_SUCCESS)
    {

        result = oduksc_adjust_dci_clk(lineotn_handle->otu_frm_handle->oduksc_handle,
                                       denom_mult,
                                       num_mult);
    }
   
    PMC_RETURN(result);
} /* lineotn_adjust_dci_clock  */




/*******************************************************************************
* lineotn_deskew_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Returns the deskew_type value set when the channel was provisioned.
*
* INPUTS:
*   handle       - pointer to LINEOTN handle instance
* 
*   dsi_stream   - LineOTN DSI stream being accessed.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   lineotn_deskew_type_t - Deskew type
*
* NOTES:
*
*******************************************************************************/
PUBLIC lineotn_deskew_type_t lineotn_deskew_get(lineotn_handle_t *handle,
                                                UINT32 dsi_stream)
{
    lineotn_deskew_type_t deskew_type = LAST_LINE_OTN_DESKEW_TYPE;

    PMC_ENTRY();

    PMC_ASSERT(dsi_stream < handle->cfg.max_dsi_streams, 
               LINEOTN_ERR_INVALID_PARAMETERS, dsi_stream, 0);

    deskew_type = handle->var.lineotn_dsi_dist[dsi_stream].deskew_type;

    PMC_RETURN(deskew_type);
} /* lineotn_deskew_get */


/*******************************************************************************
* lineotn_ddeg_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Used to configure which dDEG monitoring will be used.
*
*
* INPUTS:
*   h            - pointer to LINEOTN handle instance
*   fw_ddeg      - TRUE  : dDeg is handled by firmware;
*                  FALSE : dDeg is handled by hardware
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_ddeg_mode_set(lineotn_handle_t *h,
                                       BOOL8 fw_ddeg)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 dsi_itr;
    PMC_ENTRY();
    
    if (h->var.fw_ddeg != fw_ddeg)
    {
        /* TSD propagation shall be disabled when dDeg is handled by firmware */
        h->var.fw_ddeg = fw_ddeg;
        
        for (dsi_itr = 0;
             dsi_itr < h->cfg.max_dsi_streams && rc == PMC_SUCCESS;
             dsi_itr++)
        {
            if (TRUE == h->var.lineotn_dsi_dist[dsi_itr].dsi_provisioned)
            {
                if (TRUE == h->var.fw_ddeg)
                    rc = otu_frm_sm_ctl_set( h->otu_frm_handle,h->var.lineotn_dsi_dist[dsi_itr].dci_channel,OTU_SK_TSD_DIS,1 ); /* disable */
                else
                    rc = otu_frm_sm_ctl_set( h->otu_frm_handle,h->var.lineotn_dsi_dist[dsi_itr].dci_channel,OTU_SK_TSD_DIS,0 ); /* enable */
            }
        }

    }
    
    PMC_RETURN(rc);
} /* lineotn_ddeg_mode_set */




/*******************************************************************************
* lineotn_otu_frm_dintlv_xoff_thres_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the XOFF Threshold in OTU_FRM De-interleaver
*
* INPUTS:
*   h            - pointer to LINEOTN handle instance
* 
*   dci_channel         - UINT8, requested channel (0..11)
* 
*   dintlv_xoff         - Desired deinterleaver fifo xoff threshold
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success; otherwise a meaningful error code is returned
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_otu_frm_dintlv_xoff_thres_set(lineotn_handle_t *h,
                                                       UINT32           dci_channel,
                                                       UINT32           dintlv_xoff)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    rc = otu_frm_dintlv_xoff_thres_set(h->otu_frm_handle,
                                       dci_channel,
                                       dintlv_xoff);
     
    PMC_RETURN(rc);
} /* lineotn_otu_frm_dintlv_xoff_thres_set */



/*******************************************************************************
* lineotn_fec_dec_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This function configures the operation mode (enabled, passthru, bypassed) of a 
*   provisioned FEC DECODER for a given DSI stream.
*
* INPUTS:
*   h                      - pointer to lineotn handle instance.
*
*   dsi_stream             - unsigned, dsi stream number to configure
* 
*   dsi_type               - enumerated lineotn dsi stream type: \n
*                            see lineotn_dsi_type_t for a detailed description
* 
*   fec_dec_mode           - decoder FEC processing mode:\n
*                            see lineotn_fec_dec_mode_t for a detailed description
*
* OUTPUTS:
*   None
* 
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
* 
* NOTES:
*   
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_fec_dec_mode_set        ( lineotn_handle_t       *h,
                                                   UINT32                 dsi_stream,
                                                   lineotn_dsi_type_t     dsi_type,
                                                   lineotn_fec_dec_mode_t fec_dec_mode )
{
    PMC_ERROR                   rc = PMC_SUCCESS;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg;
    lineotn_fec_t               fec_type = LAST_LINE_OTN_FEC;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);
    PMC_ASSERT(fec_dec_mode<LAST_LINE_OTN_FEC_RX,LINEOTN_ERR_INVALID_PARAMETERS,fec_dec_mode,0);
    
    lineotn_cfg = h->var.current_lineotn_cfg;

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);
    
    if(PMC_SUCCESS == rc)
    {
        fec_type = h->var.lineotn_dsi_dist[dsi_start].dsi_fec_type;
        if(LINE_OTN_FEC_NONE == fec_type)
        {
            rc = LINEOTN_ERR_FEC_NOT_CONFIGURED;
        }
    }
    
    if(PMC_SUCCESS == rc && h->var.lineotn_dsi_dist[dsi_start].dsi_provisioned)
    {
        switch (fec_dec_mode)
        {
            case LINE_OTN_FEC_RX_ENABLED:
            
                rc = lineotn_fec_dec_bypass_set(h,dsi_start,dsi_type,LINE_OTN_FEC_NO_BYPASS_SET);
                
                if(PMC_SUCCESS == rc && TRUE == h->var.lineotn_dsi_dist[dsi_start].fec_dec_passthru)
                {
                    rc = fec_dec_passthru_set(h,dsi_start,fec_type,FALSE);
                }

                break;           
            case LINE_OTN_FEC_RX_CORRECTION_DISABLED:
                if( LINE_OTN_FEC_I4 == fec_type ||
                    LINE_OTN_FEC_G709 == fec_type ||
                    LINE_OTN_FEC_SWIZZLE_100G == fec_type ||
                    LINE_OTN_FEC_SWIZZLE_40G == fec_type )
                {
                    if(FALSE == h->var.lineotn_dsi_dist[dsi_start].fec_dec_passthru)
                    {
                        rc = fec_dec_passthru_set(h,dsi_start,fec_type,TRUE);
                    }
                } 
                else
                {
                    rc = LINEOTN_ERR_FEC_DEC_CORR_DISABLE_NOT_SUPPORTED;
                }
                break;
            case LINE_OTN_FEC_RX_BYPASSED:
                if(LINE_OTN_FEC_I7 == fec_type || LINE_OTN_FEC_G709 == fec_type)
                {
                    rc = lineotn_fec_dec_bypass_set(h,dsi_start,dsi_type,LINE_OTN_FEC_BYPASS);
                }
                else
                {
                    rc = LINEOTN_ERR_FEC_DEC_BYPASS_NOT_SUPPORTED;
                }
                break;
            default:
                rc = LINEOTN_ERR_INVALID_PARAMETERS;
                break;
        }
    }
    

    PMC_RETURN(rc);
}/* lineotn_fec_dec_mode_set */


/*******************************************************************************
* lineotn_fec_dec_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This function returns the operation mode (enabled, passthru, bypassed) of a 
*   provisioned FEC DECODER for a given DSI stream.
*
*
* INPUTS:
*   h                      - pointer to lineotn handle instance.
*
*   dsi_stream             - unsigned, dsi stream number to configure
* 
*   dsi_type               - enumerated lineotn dsi stream type: \n
*                            see lineotn_dsi_type_t for a detailed description
* 
* OUTPUTS:
*   *fec_dec_mode          - pointer to Decoder FEC processing mode  \n
*                            see lineotn_fec_dec_mode_t for a detailed description
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
* 
* NOTES:
* 
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_fec_dec_mode_get        ( lineotn_handle_t       *h,
                                                   UINT32                 dsi_stream,
                                                   lineotn_dsi_type_t     dsi_type,
                                                   lineotn_fec_dec_mode_t *fec_dec_mode )
{
    PMC_ERROR           rc = PMC_SUCCESS;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg;
    BOOL                        dec_in_passthru_mode = FALSE;
    lineotn_fec_bypass_t        fec_dec_bypass_mode = LAST_LINE_OTN_FEC_BYPASS;

    PMC_ENTRY();

    /* check inputs */
    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(fec_dec_mode!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    lineotn_cfg = h->var.current_lineotn_cfg;

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc )
    {
        rc = lineotn_fec_dec_bypass_get(h,dsi_stream,dsi_type,&fec_dec_bypass_mode);
        
        if(PMC_SUCCESS == rc && LINE_OTN_FEC_BYPASS == fec_dec_bypass_mode)
        {
            *fec_dec_mode = LINE_OTN_FEC_RX_BYPASSED;
        }
        else
        {
            rc = lineotn_fec_dec_passthru_get(h,dsi_stream,dsi_type,&dec_in_passthru_mode);
            
            if(PMC_SUCCESS == rc && TRUE == dec_in_passthru_mode )
            {
                *fec_dec_mode = LINE_OTN_FEC_RX_CORRECTION_DISABLED;
            }
            else
            {
                *fec_dec_mode = LINE_OTN_FEC_RX_ENABLED;
            }
        }
    }
    else
    {
        rc = LINEOTN_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(rc);
}/* lineotn_fec_dec_mode_get */


/*******************************************************************************
* lineotn_fec_dec_bypass_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This function configures the bypass setting of a provisioned FEC DECODER for a 
*   given DSI stream.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance 
*
*   dsi_stream             - unsigned, dsi stream number to configure
*                            
*   dsi_type               - enumerated lineotn dsi stream type: \n
*                            see lineotn_dsi_type_t for a detailed description
*
*   fec_dec_bypass_mode    - enum,requested fec decoder type for the dsi stream: \n
*                            see lineotn_fec_bypass_t for a detailed description \n
*                            LINE_OTN_FEC_BYPASS, \n
*                            LINE_OTN_FEC_NO_BYPASS_SET 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_fec_dec_bypass_set      ( lineotn_handle_t   *h,
                                                   UINT32             dsi_stream,
                                                   lineotn_dsi_type_t dsi_type,
                                                   lineotn_fec_bypass_t fec_dec_bypass_mode )
{
    PMC_ERROR   rc = PMC_SUCCESS;
    UINT8       loop_counter = 0;
    UINT8       loop_start = 0;
    UINT8       i = 0;
    UINT8       dsi_streams_count = 0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg;

    PMC_ENTRY();

    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);

    PMC_ASSERT((UINT32)fec_dec_bypass_mode< (UINT32)LAST_LINE_OTN_FEC,LINEOTN_ERR_INVALID_PARAMETERS,fec_dec_bypass_mode,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    lineotn_cfg = h->var.current_lineotn_cfg;

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        /* get the range of dsi stream configuration */
        lineotn_calc_dsi_range(h,dsi_start,&loop_start,&loop_counter);

        for(i=loop_start;i<loop_counter;i++)
        {
            if(h->var.lineotn_dsi_dist[i].dsi_provisioned) /* configure only provisioned streams */
            {
                dsi_streams_count++; /* count the configured dsi streams */

                switch(fec_dec_bypass_mode)
                {
                    case LINE_OTN_FEC_BYPASS:
                        rc = fec_mfec_dec_bypass_set(h,i,TRUE);
                        break;

                    case LINE_OTN_FEC_NO_BYPASS_SET:
                        rc = fec_mfec_dec_bypass_set(h,i,FALSE);
                        break;

                    default:
                        rc = LINEOTN_ERR_INVALID_PARAMETERS;
                    break;
                }
            }
            else
            {
                rc = LINEOTN_ERR_FEC_DEC_CFG_STREAM_NOT_PROV;
                
            }

            if(rc!=PMC_SUCCESS) /* incase of error break and report */
                break;
        }

    }

    /* check how many dsi streams where configured --> at least one stream should have been configured */
    if (PMC_SUCCESS==rc && 0 == dsi_streams_count ) {
        rc = LINEOTN_ERR_FEC_SET_FAILED;
    }

    PMC_RETURN(rc);
}/* lineotn_fec_dec_bypass_set */


/*******************************************************************************
* lineotn_fec_dec_bypass_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This function returns the bypass setting of a provisioned FEC DECODER for a 
*   given DSI stream.
*
* INPUTS:
*   h                      - pointer to lineOTN handle instance 
*
*   dsi_stream             - unsigned, dsi stream number to configure
*                            
*   dsi_type               - enumerated lineotn dsi stream type: \n
*                            see lineotn_dsi_type_t for a detailed description
*
*
* OUTPUTS:
*   *fec_dec_bypass_mode   - pointer to fec decoder bypass mode: \n
*                            see lineotn_fec_bypass_t for a detailed description
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR lineotn_fec_dec_bypass_get      ( lineotn_handle_t   *h,
                                                   UINT32             dsi_stream,
                                                   lineotn_dsi_type_t dsi_type,
                                                   lineotn_fec_bypass_t *fec_dec_bypass_mode )
{
    PMC_ERROR   rc = PMC_SUCCESS;  
    UINT8       loop_counter = 0;
    UINT8       loop_start = 0;
    UINT8       i = 0;
    mld_ifc_type_t              mld_interface_type = MLD_NA;
    util_global_odukp_type_t    oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
    UINT32                      dsi_start = 0;
    UINT32                      dsi_end   = 0;
    lineotn_cfg_type_t          lineotn_cfg;
    BOOL                        bypass_dec = FALSE;

    PMC_ENTRY();

    PMC_ASSERT(h!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(fec_dec_bypass_mode!=NULL,LINEOTN_ERR_INVALID_PTR,0,0);
    PMC_ASSERT(dsi_stream<h->cfg.max_dsi_streams ||
               LINEOTN_SFI51_1_DSI_STREAM_IDX == dsi_stream,LINEOTN_ERR_INVALID_PARAMETERS,dsi_stream,0);
    PMC_ASSERT(dsi_type<LAST_LINE_OTN_DSI_TYPE,LINEOTN_ERR_INVALID_PARAMETERS,dsi_type,0);

    lineotn_cfg = h->var.current_lineotn_cfg;

    rc = lineotn_dsi_params_get(h,lineotn_cfg,dsi_stream,dsi_type,&mld_interface_type,&oduk_type,&dsi_start,&dsi_end);

    if(PMC_SUCCESS == rc)
    {
        /* get the range of dsi stream configuration */
        lineotn_calc_dsi_range(h,dsi_start,&loop_start,&loop_counter);

        for(i=loop_start;i<loop_counter;i++)
        {
            if(h->var.lineotn_dsi_dist[i].dsi_provisioned) /* check only provisioned streams */
            {
                rc = fec_mfec_dec_bypass_get(h,i,&bypass_dec);
                
            }
            else
            {
                rc = LINEOTN_ERR_FEC_DEC_CFG_STREAM_NOT_PROV;
            }

            if(rc!=PMC_SUCCESS) /* incase of error break and report */
                break;
        }

    }
    
    if(PMC_SUCCESS == rc)
    {
        if(TRUE == bypass_dec)
        {
            *fec_dec_bypass_mode = LINE_OTN_FEC_BYPASS;
        }
        else
        {
            *fec_dec_bypass_mode = LINE_OTN_FEC_NO_BYPASS_SET;
        }
    }
    
    PMC_RETURN(rc);
}/* lineotn_fec_dec_bypass_get */

/*******************************************************************************
* lineotn_top_level_interrupt_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Function sets the top level interrupt used to enable SYSOTN interrupts
*
* INPUTS:
*  handle                 - pointer to lineOTN handle instance 
*  enable                 - Whether to enable the setting or not.
*
*
* OUTPUTS:
*  None.
*
* RETURNS:
*  None
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC void lineotn_top_level_interrupt_set(lineotn_handle_t *handle,
                                            BOOL8 enable)
{
    UINT32             top_level_mask = 0xFF00;
    sysotn_intr_type_t type;
    
    PMC_ENTRY();

    /*
     * Each 8 bits of the register represent what interrupts will trigger
     * through this top level interrupt. There in all 5 IRQ bits even
     * though all 8 bits are set.
     * BITS 0:1 - Handle SFI Tx/Rx interrpts
     * BIT 2 - Handles GFEC interrupts
     * BIT 4 - OTN OTU related interrupts
     * BIT 6 - OPSM related interrupts.
     * This is the same for each of the 4 quadrants in the register. That 
     * is for OTN OTU interrupts Bit 4, 12, 20 and 28 apply.
     * Which quadrant is used depends on where the interrupt is to be 
     * routed, namely its output. From table 97 of digi120g_ddd.pdf:
     * Quadrant 1: Internal MIPS
     * Quadrant 2: PCIe
     * Quadrant 3: Device Pin
     * Quadrant 4; External MIPs
     * The value of g_top_level_mask is a function of the cpuif_int_t value
     * passed to digi_int_init and whether or not the application is SYSOTN
     * The folowing values are used:
     * CPUIF_INT_DISABLE or application is not SYSOTN: 0x0
     * CPU_INT_MSI, MSI-X or Legacy: 0xFF00
     * CPU_INT_DEVICE_PIN: 0xFF0000;
     */

    type =(sysotn_intr_type_t)((handle->var.mld_curr_num_otu_configured >>30) & 0x3);
    if((SYSOTN_INTR_TYPE_CPUIF_INT_DISABLE == type) ||
       (SYSOTN_INTR_TYPE_SYSOTN_DISABLED == type))
    {
        top_level_mask = 0;
    }
    else if(SYSOTN_INTR_TYPE_CPUIF_INT_OTHER == type)
    {
        top_level_mask = 0xFF00;
    }
    else if(SYSOTN_INTR_TYPE_CPUIF_INT_DEVICE_PIN == type)
    {
        top_level_mask = 0xFF0000;
    }
     
    sysotn_field_TOP_INT_E_set(NULL, handle, top_level_mask);
    
    PMC_RETURN();
} /* lineotn_top_level_interrupt_set */


/*******************************************************************************
* lineotn_top_level_interrupt_recover
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Function recover the top level interrupt used to enable SYSOTN interrupts
*
* INPUTS:
*  handle                 - pointer to lineOTN handle instance 
*
*
* OUTPUTS:
*  None.
*
* RETURNS:
*  None
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC void lineotn_top_level_interrupt_recover(lineotn_handle_t *handle)
{
    UINT32             top_level_mask;
    sysotn_intr_type_t type = SYSOTN_INTR_TYPE_SYSOTN_DISABLED;
    
    PMC_ENTRY();

    top_level_mask = sysotn_field_TOP_INT_E_get(NULL, handle);

    if(0 == top_level_mask)
    {
        type = SYSOTN_INTR_TYPE_SYSOTN_DISABLED;
    }
    else if(0xFF00 == top_level_mask)
    {
        type = SYSOTN_INTR_TYPE_CPUIF_INT_OTHER;
    }
    else if(0xFF0000 == top_level_mask)
    {
        type = SYSOTN_INTR_TYPE_CPUIF_INT_DEVICE_PIN;
    }

    lineotn_interrupt_type_save(handle,type);
    
    PMC_RETURN();
} /* lineotn_top_level_interrupt_recover */


/*******************************************************************************
* lineotn_interrupt_type_save
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Function to save the sysotn top level interrupt type in the context
*
* INPUTS:
*  handle               - pointer to lineOTN handle instance 
*  type                 - interrupt type.
*
*
* OUTPUTS:
*  None.
*
* RETURNS:
*  None
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC void lineotn_interrupt_type_save(lineotn_handle_t    *handle, 
                                        sysotn_intr_type_t  type)
{
    PMC_ENTRY();
    
    /*clear bit 31-30*/
    handle->var.mld_curr_num_otu_configured &= SYSOTN_INT_TYPE_STORAGE_MASK_REVERSE;
    
    /*save type to bit 31-30*/
    handle->var.mld_curr_num_otu_configured |= (((UINT32)type<<30) & SYSOTN_INT_TYPE_STORAGE_MASK);
    
    PMC_RETURN();
}




/*
** end of file
*/
        
