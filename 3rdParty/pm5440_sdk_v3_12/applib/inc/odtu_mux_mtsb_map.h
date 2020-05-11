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
 *     Contains base address definitions for the odtu_mux_mtsb block
 * 
 *   NOTES:
 *     This file is generated by the script codegen.pm, version 1.1
 *     Do not modify this file.
 * 
 *     The input file is ../src/ioxml/pm55_60_17_map.xml
 *     block_uri "file:../docs/rda/pm55_60_17_map.xml"
 *     block_part_number "PM55_60_17"
 *     block_mnemonic "MUX"
 * 
 *****************************************************************************/
#ifndef _ODTU_MUX_MTSB_MAP_H
#define _ODTU_MUX_MTSB_MAP_H


/*
 * ==================================================================================
 *                        ODTU_MUX_MTSB RDA XML Version Info
 * ==================================================================================
 */
#define ODTU_MUX_MTSB_MAP_FILE_NAME    "../src/ioxml/pm55_60_17_map.xml"
#define ODTU_MUX_MTSB_MAP_FILE_VERSION "../src/ioxml/pm55_60_17_map.xml"
/*
 * ==================================================================================
 *                        ODTU_MUX_MTSB Block Base Addresses
 * ==================================================================================
 */
#ifndef ODTU_MUX_MTSB_TSB_BASE_ADDR_DEFS_H
#define ODTU_MUX_MTSB_TSB_BASE_ADDR_DEFS_H

#define BASE_ADDR_MUX_MTSB                      0x00000000
#define BASE_ADDR_MUX_ODTU_MUX                  0x00000000
#define BASE_ADDR_MUX_EXP_CFC_CFC_CFC_INDIRECT  0x0
#define BASE_ADDR_MUX_TXJC_CFC_CFC_CFC_INDIRECT 0x0
#define BASE_ADDR_MUX_OH_CFC_CFC_CFC_INDIRECT   0x0
#define BASE_ADDR_MUX_MGEN_MP_MGEN              0x00001000
#define BASE_ADDR_MUX_EXP_EXPAND_IBUF           0x00002000
#define BASE_ADDR_MUX_EXP_CFC_CFC               0x00004000
#define BASE_ADDR_MUX_TXJC_TXJC_PROC            0x00007000
#define BASE_ADDR_MUX_TXJC_CFC_CFC              0x00008000
#define BASE_ADDR_MUX_OHFS_OHFS_INSERT          0x0000a000
#define BASE_ADDR_MUX_OH_CFC_CFC                0x0000b000
#define BASE_ADDR_MUX_STS_STS_SW                0x00010000

#endif /* ODTU_MUX_MTSB_TSB_BASE_ADDR_DEFS_H */
#endif /* _ODTU_MUX_MTSB_MAP_H */