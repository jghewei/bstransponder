/******************************************************************************/
/**  COPYRIGHT (C) 2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                **/
/**--------------------------------------------------------------------------**/
/** This software embodies materials and concepts which are proprietary and  **/
/** confidential to PMC-Sierra, Inc. PMC-Sierra distributes this software    **/
/** to its customers pursuant to the terms and conditions of the Software    **/
/** License Agreement contained in the text file software.lic that is        **/
/** distributed along with the software. This software can only be utilized  **/
/** if all terms and conditions of the Software License Agreement are        **/
/** accepted. If there are any questions, concerns, or if the                **/
/** Software License Agreement text file, software.lic, is missing please    **/
/** contact PMC-Sierra for assistance.                                       **/
/**--------------------------------------------------------------------------**/
/**                                                                          **/
/******************************************************************************/
/*******************************************************************************
**  MODULE      : SWIG interface to DIGI Adapter library
**
**  FILE        : digi_adpt.i
**
**  $Date$
**
**  $Revision$
**
**  DESCRIPTION :  This file contains SWIG stubs for interfacing DIGI Adapter
**                 library to a Tcl shell.
**
**  NOTES       :
**
*******************************************************************************/
%module adpt
%include digi.i
%{
#include "pmc_log.h"
#include "digi_adpt_com.h"
#include "digi_adpt.h"
#include "digi_adpt_basic_cfg.h"
#include "digi_adpt_customer.h"
#include "digi_adpt_datapath.h"
#include "digi_adpt_otn.h"
#include "digi_adpt_enet.h"
#include "digi_adpt_gfp.h"
#include "digi_adpt_debug.h"
#include "digi_adpt_interlaken.h"
#include "digi_adpt_interrupt.h"
#include "digi_adpt_alm.h"
#include "digi_adpt_pmon.h"
#include "digi_adpt_api.h"
#include "digi_adpt_sdh.h"
#include "digi_adpt_cbr.h"
#include "digi_adpt_pv_cvt.h"
#include "digi_adpt_loopback.h"
#include "digi_adpt_serdes.h"
#include "digi_adpt_port.h"
#include "digi_adpt_misc.h"
#include "digi_adpt_ptp.h"
#include "digi_adpt_ghao.h"
#include "cpuif_api.h"
%}

%include "pmc_log.h"
%include "digi_adpt_com.h"
%include "digi_adpt.h"
%include "digi_adpt_basic_cfg.h"
%include "digi_adpt_customer.h"
%include "digi_adpt_datapath.h"
%include "digi_adpt_otn.h"
%include "digi_adpt_enet.h"
%include "digi_adpt_gfp.h"
%include "digi_adpt_debug.h"
%include "digi_adpt_interlaken.h"
%include "digi_adpt_interrupt.h"
%include "digi_adpt_alm.h"
%include "digi_adpt_pmon.h"
%include "digi_adpt_api.h"
%include "digi_adpt_sdh.h"
%include "digi_adpt_cbr.h"
%include "digi_adpt_pv_cvt.h"
%include "digi_adpt_loopback.h"
%include "digi_adpt_serdes.h"
%include "digi_adpt_port.h"
%include "digi_adpt_misc.h"
%include "digi_adpt_ptp.h"
%include "digi_adpt_ghao.h"
%include "cpuif_api.h"

/* SWIG External Initialization Routine */
%init %{ 
    digi_interp = interp;
    PMC_PRINT("Loaded libadpt_swig.so: %s\n", swig_get_version());
%}
