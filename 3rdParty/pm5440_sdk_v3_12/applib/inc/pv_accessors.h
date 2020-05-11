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
*     This file contains functions for the creation and destruction of
*     emulation accessor functions.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _EMUL_ACCESSORS_H
#define _EMUL_ACCESSORS_H

#ifdef __cplusplus
extern "C" {
#endif

PUBLIC UINT32 access_util_global_switch_data_def(void *util_global_ptr,UINT8 req_field);
PUBLIC UINT32 access_otn_server_chnl_def        (void *otn_srvr_ptr,UINT8 req_field);

#ifdef __cplusplus
}
#endif

#endif /* _EMUL_ACCESSORS_H */


