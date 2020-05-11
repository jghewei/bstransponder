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
 *     Contains base address definitions for the tgmp_mtsb block
 * 
 *   NOTES:
 *     This file is generated by the script codegen.pm, version 1.1
 *     Do not modify this file.
 * 
 *     The input file is ../src/ioxml-filtered/pm55_60_21_map.xml
 *     block_uri "file:../docs/rda/pm55_60_21_map.xml"
 *     block_part_number "PM55_60_21"
 *     block_mnemonic "TGMP96"
 * 
 *****************************************************************************/
#ifndef _TGMP_MTSB_MAP_H
#define _TGMP_MTSB_MAP_H


/*
 * ==================================================================================
 * TGMP_MTSB RDA XML Version Info
 * ==================================================================================
 */
#define TGMP_MTSB_MAP_FILE_NAME    "../src/ioxml-filtered/pm55_60_21_map.xml"
#define TGMP_MTSB_MAP_FILE_VERSION "../src/ioxml-filtered/pm55_60_21_map.xml"
/*
 * ==================================================================================
 * TGMP_MTSB Block Base Addresses
 * ==================================================================================
 */
#ifndef TGMP_MTSB_TSB_BASE_ADDR_DEFS_H
#define TGMP_MTSB_TSB_BASE_ADDR_DEFS_H

#define BASE_ADDR_TGMP96_MTSB                             0x00000000
#define BASE_ADDR_TGMP96_TGMP96_CORE                      0x00000000
#define BASE_ADDR_TGMP96_MGEN_MP_MGEN                     0x00001000
#define BASE_ADDR_TGMP96_EXP_MTSB                         0x00002000
#define BASE_ADDR_TGMP96_EXP_EXPAND_IBUF                  0x00002000
#define BASE_ADDR_TGMP96_EXP_EXP_CFC_CFC                  0x00004000
#define BASE_ADDR_TGMP96_TXJC_TXJC_PROC_REG_MTSB          0x00007000
#define BASE_ADDR_TGMP96_TXJC_TXJC_PROC_REG_TOP_TXJC_PROC 0x00007000
#define BASE_ADDR_TGMP96_TXJC_TXJC_PROC_REG_CFC           0x00008000
#define BASE_ADDR_TGMP96_OHFS_MTSB                        0x0000a000
#define BASE_ADDR_TGMP96_OHFS_OHFS_INSERT                 0x0000a000
#define BASE_ADDR_TGMP96_OHFS_CFC                         0x0000b000

#endif /* TGMP_MTSB_TSB_BASE_ADDR_DEFS_H */
#endif /* _TGMP_MTSB_MAP_H */
