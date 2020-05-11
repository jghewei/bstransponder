/******************************************************************************
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
 * -------------------------------------------------------------------------
 *   DESCRIPTION:
 *     Contains base address definitions for the fc1200_ptd_mtsb block
 * 
 *   NOTES:
 *     This file is generated by the script codegen.pm, version 1.1
 *     Do not modify this file.
 * 
 *     The input file is ../src/ioxml-filtered/pm70_82_17_map.xml
 *     block_uri "file:../docs/rda/pm33_82_06_map.xml"
 *     block_part_number "PM33_82_06"
 *     block_mnemonic "FC1200_PTD"
 * 
 *****************************************************************************/
#ifndef _FC1200_PTD_MTSB_MAP_H
#define _FC1200_PTD_MTSB_MAP_H


/*
 * ==================================================================================
 * FC1200_PTD_MTSB RDA XML Version Info
 * ==================================================================================
 */
#define FC1200_PTD_MTSB_MAP_FILE_NAME    "../src/ioxml-filtered/pm70_82_17_map.xml"
#define FC1200_PTD_MTSB_MAP_FILE_VERSION "../src/ioxml-filtered/pm70_82_17_map.xml"
/*
 * ==================================================================================
 * FC1200_PTD_MTSB Block Base Addresses
 * ==================================================================================
 */
#ifndef FC1200_PTD_MTSB_TSB_BASE_ADDR_DEFS_H
#define FC1200_PTD_MTSB_TSB_BASE_ADDR_DEFS_H

#define BASE_ADDR_CBRC_CBRC_FC_DLVR_FC1200_PTD_MTSB(A)           (0x00050000 + (A) * 0x200)
#define MAX_CBRC_CBRC_FC_DLVR_FC1200_PTD_MTSB                    11
#define BASE_ADDR_CBRC_CBRC_FC_DLVR_FC1200_PTD_FC1200_PCS_TX(A)  (0x00050000 + (A) * 0x200)
#define MAX_CBRC_CBRC_FC_DLVR_FC1200_PTD_FC1200_PCS_TX           11
#define BASE_ADDR_CBRC_CBRC_FC_DLVR_FC1200_PTD_FC1200_PMON(A)    (0x00050080 + (A) * 0x200)
#define MAX_CBRC_CBRC_FC_DLVR_FC1200_PTD_FC1200_PMON             11
#define BASE_ADDR_CBRC_CBRC_FC_DLVR_FC1200_PTD_FC_TTTD(A)        (0x00050100 + (A) * 0x200)
#define MAX_CBRC_CBRC_FC_DLVR_FC1200_PTD_FC_TTTD                 11

#endif /* FC1200_PTD_MTSB_TSB_BASE_ADDR_DEFS_H */
#endif /* _FC1200_PTD_MTSB_MAP_H */