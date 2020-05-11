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
*     This file contains the function prototypes for SDH_PMG functionality
*     within the CBRC:
*     - SONET/SDH performance monitoring
*     - SONET/SDH indirect access
*
*   NOTES:
*
*******************************************************************************/

#ifndef _SDH_PMG_API_H
#define _SDH_PMG_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_types.h"
#include "pmc_mid.h"

/*
** Enumerated Types
*/


#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */


/*******************************************************************************
* ENUM: sdh_pmg_rrmp_byte_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration for RRMP byte extractions.
*
* ELEMENTS:
*   SDH_PMG_RRMP_J0         - Received J0 byte, 8 bits
*   SDH_PMG_RRMP_SSM        - Sync Status Message, 8 bits
*   SDH_PMG_RRMP_K2         - APS K2, 8 bits
*   SDH_PMG_RRMP_K1         - APS K1,  8 bits
*
*******************************************************************************/
typedef enum {
    SDH_PMG_RRMP_BYTE_TYPE_J0 = 0,
    SDH_PMG_RRMP_BYTE_TYPE_SSM,
    SDH_PMG_RRMP_BYTE_TYPE_K2,
    SDH_PMG_RRMP_BYTE_TYPE_K1,
    
    LAST_SDH_PMG_RRMP_BYTE_TYPE,
    
} sdh_pmg_rrmp_byte_type_t;



/*******************************************************************************
* ENUM: sdh_pmg_rrmp_aps_algo_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration for RRMP APS behaviour
*
* ELEMENTS:
*   SDH_PMG_RRMP_APS_ALGO_1 - K1 byte is monitored to detect an APS byte failure
*   SDH_PMG_RRMP_APS_ALGO_2 - K1 and K2 bytes are monitored together to detect
*                             an APS byte failure. However, detection of RDI-L
*                             as the only mismatch is not considered as an APS
*                             byte failure.
*   SDH_PMG_RRMP_APS_ALGO_3 - K1 and K2 bytes are monitored together to detect
*                             an APS byte failure.
*
*
*******************************************************************************/
typedef enum {
    SDH_PMG_RRMP_APS_ALGO_1 = 0,
    SDH_PMG_RRMP_APS_ALGO_2 = 1,
    SDH_PMG_RRMP_APS_ALGO_3 = 2,
    
    LAST_SDH_PMG_RRMP_APS_ALGO,
    
} sdh_pmg_rrmp_aps_algo_t;



/*******************************************************************************
* ENUM:sdh_pmg_rrmp_alarm_cfg_mask_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bitmask to indicate which fields to config in sdh_pmg_rrmp_alarm_cfg_t
*
* ELEMENTS:
*    See enumeration.
*
*******************************************************************************/
typedef enum { 
    SDH_PMG_RRMP_ALARM_CFG_MASK_EXTENDED_APS = 0x00000001,
    SDH_PMG_RRMP_ALARM_CFG_MASK_APS_ALGO     = 0x00000002,
    SDH_PMG_RRMP_ALARM_CFG_MASK_LRDI_ALGO    = 0x00000004,
    SDH_PMG_RRMP_ALARM_CFG_MASK_LAIS_ALGO    = 0x00000008,
    SDH_PMG_RRMP_ALARM_CFG_MASK_LOS_SHORT    = 0x00000010,
    SDH_PMG_RRMP_ALARM_CFG_MASK_LREI_ALGO    = 0x00000020,
    SDH_PMG_RRMP_ALARM_CFG_MASK_SSM_FRAMES   = 0x00000040,
    SDH_PMG_RRMP_ALARM_CFG_MASK_LBIP_ACCUM   = 0x00000080,
    SDH_PMG_RRMP_ALARM_CFG_MASK_SBIP_ACCUM   = 0x00000100,
    SDH_PMG_RRMP_ALARM_CFG_MASK_M0EN         = 0x00000200,
} sdh_pmg_rrmp_alarm_cfg_mask_t;


/*******************************************************************************
* STRUCTURE: sdh_pmg_rrmp_alarm_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for configuring RRMP alarms.
*   Default configuration is 0 for all fields.
*
* ELEMENTS:
*   mask            - sdh_pmg_rrmp_alarm_cfg_mask_t bitmask to indicate config to set
*
*   extended_APS_en - Selects the K1K2 byte filtering algorithm.\n
*                     1: K1[1:8]K2[1:5] bytes are filtered together to detect
*                        a change of APS (COAPS) and K2[6:8] bits are filtered
*                        together to detect a change of K2[6:8] (COK2).\n
*                     0: K1K2 bytes are filtered together to detect a change of
*                        APS (COAPS) and change of K2[2:0] (COK2) are never reported.
*
*   APS_algo        - Selects APS byte failure algorithm.
*
*   LRDI_algo       - Selects the Line RDI detection algorithm.\n
*                     1: Line RDI is declared when a 110 pattern is detected
*                        in bits 6,7,8 of the K2 byte for three consecutive
*                        frames.\n
*                     0: Line RDI is declared when a 110 pattern is detected
*                        in bits 6,7,8 of the K2 byte for five consecutive frames.
*
*   LAIS_algo       - Selects the Line AIS detection algorithm.\n
*                     1: Line AIS is declared when a 111 pattern is detected in
*                        bits 6,7,8 of the K2 byte for three consecutive frames.
*                     0: Line AIS is declared when a 111 pattern is detected
*                        in bits 6,7,8 of the K2 byte for five consecutive frames.
*
*   LOS_short_en    - Modifies the period before declaring LOS.
*                     1: The accumulation period is 2.3 us of consecutive all
*                        zeros pattern.
*                     0: The accumulation period is 20 us of consecutive all
*                        zeros pattern.
*
*   LREI_algo       - Selects the Line REI detection algorithm.\n
*                     1: The M0 byte is taken into consideration when extracting
*                        the LREI from the incoming stream.
*                     0: The M0 byte is not considered for the LREI extraction.
*
*   SSM_frames      - Selects the number of frames needed to accept a new SSM
*                     1: The new SSM value is accepted and updated after receiving
*                        three consecutive frames with a stable SSM value.
*                     0: The new SSM value is accepted and updated after receiving
*                        eight consecutive frames with a stable SSM value.
*
*   LBIP_accum      - Controls the accumulation of line BIP (B2) errors.\n
*                     1: The line BIP accumulation represents BIP-24 block
*                        errors (a maximum of 1 error per STS 3/STM-1 per frame).
*                     0: The line BIP accumulation represents BIP-8 errors
*                        (a maximum of 8 errors per STS 1/STM-0 per frame).
*
*   SBIP_accum      - Controls the accumulation of section BIP (B1) errors.\n
*                     1: The section BIP accumulation represents BIP-8 block
*                        errors (a maximum of 1 error per frame).
*                     0: The section BIP accumulation represents BIP-8 errors
*                        (a maximum of 8 errors per frame).
*   M0_en           - Indicate whether the receive M0 byte is valid and should be considered
*                     1: the M0 byte is taken into consideration when extracting
*                        the LREI from the incoming stream
*                     0: the M0 byte is not considered for the REI extraction.
*
*******************************************************************************/
typedef struct {
    UINT32                      mask;
    BOOL                        extended_APS_en;
    sdh_pmg_rrmp_aps_algo_t     APS_algo;
    BOOL                        LRDI_algo;
    BOOL                        LAIS_algo;
    BOOL                        LOS_short_en;
    BOOL                        LREI_algo;
    BOOL                        SSM_frames;
    BOOL                        LBIP_accum;
    BOOL                        SBIP_accum;
    BOOL                        M0_en;
} sdh_pmg_rrmp_alarm_cfg_t;





/*******************************************************************************
* ENUM:sdh_pmg_rrmp_scramble_cfg_mask_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bitmask to indicate which fields to config in sdh_pmg_rrmp_scramble_cfg_t
*
* ELEMENTS:
*    See enumeration.
*
*******************************************************************************/
typedef enum { 
    SDH_PMG_RRMP_SCRAMBLE_CFG_MASK_J0       = 0x00000001,
    SDH_PMG_RRMP_SCRAMBLE_CFG_MASK_A1A2     = 0x00000002,
} sdh_pmg_rrmp_scramble_cfg_mask_t;


/*******************************************************************************
* STRUCTURE: sdh_pmg_rrmp_scramble_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for configuring RRMP OH byte descrambling.
*   Default configuration is 0 for all fields.
*
* ELEMENTS:
*  mask            - sdh_pmg_rrmp_scramble_cfg_mask_t bitmask to indicate config to set
*  J0_descramble   - 1: The J0/Z0 section of the data stream is descrambled.
*                    0: The J0/Z0 section of the data stream is not descrambled.
*  A1A2_descramble - 1: All but the last 64 A1 bytes and the first 64 A2 bytes
*                       are considered invalid and are descrambled.
*                    0: All A1 and A2 bytes of column 1 and 2 are not descrambled.
*
******************************************************************************/
typedef struct {
    UINT32      mask;
    BOOL        J0_descramble;
    BOOL        A1A2_descramble;
} sdh_pmg_rrmp_scramble_cfg_t;




/*******************************************************************************
* ENUM:sdh_pmg_rrmp_framing_cfg_mask_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bitmask to indicate which fields to config in sdh_pmg_rrmp_framing_cfg_t
*
* ELEMENTS:
*    See enumeration.
*
*******************************************************************************/
typedef enum { 
    SDH_PMG_RRMP_FRAMING_CFG_MASK_ALGO       = 0x00000001,
} sdh_pmg_rrmp_framing_cfg_mask_t;


/*******************************************************************************
* STRUCTURE: sdh_pmg_rrmp_framing_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for configuring RRMP framing.
*   Default configuration is 0 for all fields.
*
* ELEMENTS:
*   mask         - sdh_pmg_rrmp_framing_cfg_mask_t bitmask to indicate config to set
*
*   algo         - 1: The A1 and A2 bytes used for framing are the 12 A1 bytes
*                     located in STS-1 timeslots 129 to 140 and the 12 A2 bytes
*                     located in STS-1 timeslots 53 to 64.
*
*                  0: The A1 and A2 bytes used for framing are the 12 A1 bytes
*                     located in STS-1 timeslots 1 to 12 and the 12 A2 bytes
*                     located in STS-1 timeslots 181 to 192.
*
*******************************************************************************/
typedef struct {
    UINT32      mask;
    BOOL        algo;
} sdh_pmg_rrmp_framing_cfg_t;


/*******************************************************************************
* ENUM:sdh_pmg_trmp_alarm_cfg_mask_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bitmask to indicate which fields to config in sdh_pmg_trmp_alarm_cfg_t
*
* ELEMENTS:
*    See enumeration.
*
*******************************************************************************/
typedef enum { 
    SDH_PMG_TRMP_ALARM_CFG_MASK_M0EN         = 0x00000001,
} sdh_pmg_trmp_alarm_cfg_mask_t;

/*******************************************************************************
* STRUCTURE: sdh_pmg_trmp_alarm_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for configuring TRMP alarms.
*   Default configuration is 0 for all fields.
*
* ELEMENTS:
*   mask            - Bitmask to indicate TRMP config to set, refer to sdh_pmg_trmp_alarm_cfg_mask_t
*
*   M0_en           - Indicate whether the receive M0 byte is valid and should be considered
*                     1: the M0 byte is taken into consideration when extracting 
*                        the LREI from the incoming stream
*                     0: the M0 byte is not considered for the LREI extraction.
*
*
*
*
*******************************************************************************/
typedef struct {
   UINT32                      mask;
   BOOL                        M0_en;
} sdh_pmg_trmp_alarm_cfg_t;

/*******************************************************************************
* ENUM: sdh_pmg_pn11_insert_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration for enable PN11 insertion on LOF or LOS.
*
* ELEMENTS:
*    See enumeration.
*
*******************************************************************************/
typedef enum {
    SDH_PMG_PN11_INSERT_LOF = 0,
    SDH_PMG_PN11_INSERT_LOS,
    
    LAST_SDH_PMG_PN11_INSERT,
    
} sdh_pmg_pn11_insert_t;



/*******************************************************************************
* ENUM: sdh_pmg_sber_byte_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration for SBER (B1 and B2) config
*
* ELEMENTS:
*   SDH_PMG_SBER_BYTE_B1     - Configure SBER to use B1 byte
*   SDH_PMG_SBER_BYTE_B2     - Configure SBER to use B2 byte
*
*******************************************************************************/
typedef enum {
    SDH_PMG_SBER_BYTE_B1 = 0,
    SDH_PMG_SBER_BYTE_B2,
    
    LAST_SDH_PMG_SBER_BYTE,
    
} sdh_pmg_sber_byte_t;



/*******************************************************************************
* ENUM: sdh_pmg_sber_alarm_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration for SBER alarm type
*
* ELEMENTS:
*   SDH_PMG_SBER_ALARM_SD - Signal degrade
*   SDH_PMG_SBER_ALARM_SF - Signal fail
*
*******************************************************************************/
typedef enum {
    SDH_PMG_SBER_ALARM_SD = 0,
    SDH_PMG_SBER_ALARM_SF,
    
    LAST_SDH_PMG_SBER_ALARM,
    
} sdh_pmg_sber_alarm_t;

/*******************************************************************************
* ENUM: sdh_pmg_sber_ber_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration for bit error rates for signal degrade and fail thresholds.
*
* ELEMENTS:
*   See enumeration.
*
*******************************************************************************/
typedef enum {
    SDH_PMG_SBER_BER_DISABLED = 0,
    SDH_PMG_SBER_BER_1E_03,
    SDH_PMG_SBER_BER_1E_04,
    SDH_PMG_SBER_BER_1E_05,
    SDH_PMG_SBER_BER_1E_06,
    SDH_PMG_SBER_BER_1E_07,
    SDH_PMG_SBER_BER_1E_08,
    SDH_PMG_SBER_BER_1E_09,
    SDH_PMG_SBER_BER_1E_10,
    SDH_PMG_SBER_BER_1E_11,
    SDH_PMG_SBER_BER_1E_12,
   
    LAST_SDH_PMG_SBER_BER,
    
} sdh_pmg_sber_ber_t;



/*******************************************************************************
* ENUM: sdh_pmg_rttp_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration for trace message processing (TIU and TIM) status and interrupts.
*   See sdh_pmg_rttp_algo_t for TIM and TIU trigger definitions.
*
* ELEMENTS:
*   SDH_PMG_RTTP_TIM    - Trace Identifier Unstable
*   SDH_PMG_RTTP_TIU    - Trace Identifier Mismatch
*
*******************************************************************************/
typedef enum {
    SDH_PMG_RTTP_TIM = 0,
    SDH_PMG_RTTP_TIU,
    
    LAST_SDH_PMG_RTTP,
    
} sdh_pmg_rttp_status_t;



/*******************************************************************************
* ENUM: sdh_pmg_rttp_length_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration to set the length of the trace message to 16 or 64 bytes.
*   The 64 and 16 byte settings only apply to SDH_PMG_RTTP_ALGO_1 and
*   SDH_PMG_RTTP_ALGO_2. The 1 byte setting only applies to SDH_PMG_RTTP_ALGO_3.
*
*
* ELEMENTS:
*  See enumeration.
*
*******************************************************************************/
typedef enum {
    SDH_PMG_RTTP_LENGTH_1_BYTE   = 1,
    SDH_PMG_RTTP_LENGTH_16_BYTES = 16,
    SDH_PMG_RTTP_LENGTH_64_BYTES = 64, 
    LAST_SDH_PMG_RTTP_LENGTH,
    
} sdh_pmg_rttp_length_t;



/*
** Constants
*/
/* Maximum size of SDH PRBS synchornization state array */
#define SDH_PMG_PRBS_SYN_ARRAY_SIZE  6


/*
** Structures and Unions
*/


/*******************************************************************************
* ENUM: sdh_pmg_rttp_algo_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration for specifying trace message matching algorithm.\n
*
*
*   SDH_PMG_RTTP_ALGO_1:
*   Algorithm 1 is TELCORDIA compliant. The algorithm detects TIM on a 16 or
*   64 bytes tail trace message.\n
*
*   This algorithm captures, synchronizes and validates the tail trace message,
*   using a sliding window of 20 messages. The current tail trace message is
*   stored in the captured page. \n
*
*   A match tail trace message is declared when 16 of the last 20 messages
*   match the expected message. A mismatch tail trace message is declared when
*   none of the last 20 messages matches the expected message. The expected
*   tail trace message is a static message written in the expected page.\n
*
*
*   SDH_PMG_RTTP_ALGO_2:
*   Algorithm 2 is ITU compliant. The algorithm detects TIU and TIM on a 16 or
*   64 bytes tail trace message.\n
*
*   This algorithm captures, synchronizes, monitors persistency, monitors
*   stability and validates the tail trace message. The current tail trace
*   message is stored in the captured page. \n
*
*   A persistent tail trace message is declared when an identical message is
*   received for 3 or 5 consecutive messages (16 or 64 frames). A persistent
*   message becomes the accepted message. The accepted message is stored in the
*   accepted page.\n
*
*   A stable tail trace message is declared when a persistent message is
*   received. An unstable tail trace message is declared when one or more
*   erroneous bytes are detected between the current message and the previous
*   message in a total of 8 tail trace messages without any persistent
*   message in between.\n
*
*
*   SDH_PMG_RTTP_ALGO_3:
*   Algorithm 3 is not TELCORDIA/ITU compliant. The algorithm detects TIU on a
*   single continuous tail trace byte.\n
*
*   A stable tail trace byte is declared when an identical byte is received
*   for 48 consecutive frames. An unstable tail trace byte is declared when
*   one or more erroneous bytes are detected in three consecutive 16 byte
*   windows. The first window starts on the first erroneous byte.
*
*
* ELEMENTS:
*    SDH_PMG_RTTP_ALGO_DISABLED - Disable trace message processing.
*
*    SDH_PMG_RTTP_ALGO_1        - Use Algorithm 1 for trace message processing.
*
*    SDH_PMG_RTTP_ALGO_2        - Use Algorithm 2 for trace message processing.
*
*    SDH_PMG_RTTP_ALGO_3        - Use Algorithm 3 for trace message processing.
*
*******************************************************************************/
typedef enum {
    SDH_PMG_RTTP_ALGO_DISABLED = 0,
    SDH_PMG_RTTP_ALGO_1,
    SDH_PMG_RTTP_ALGO_2,
    SDH_PMG_RTTP_ALGO_3,
    
    LAST_SDH_PMG_RTTP_ALGO,
    
} sdh_pmg_rttp_algo_t;

/*******************************************************************************
* STRUCTURE: sdh_pmg_rttp_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configuration structure for the RTTP.
*
* ELEMENTS:
*   length      - Configures the RTTP for 1, 16, or 64 byte trace messages.
*   algo        - Trace message matching algorithm
*
*******************************************************************************/
typedef struct {
   sdh_pmg_rttp_length_t    length; 
   sdh_pmg_rttp_algo_t      algo;
} sdh_pmg_rttp_cfg_t;


/*******************************************************************************
* STRUCTURE: sdh_pmg_rttp_trace_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for getting a trace message.
*
* ELEMENTS:
*   trace_msg[64]   - trace message, max of 64 bytes
*   size            - size of trace message in bytes
*   e164_crc_ptr    - pointer to CRC byte (if exists)
*   crlf_ptr        - pointer to CRLF byte (if exists)
*
*******************************************************************************/
typedef struct {
    UINT8   trace_msg[SDH_PMG_RTTP_LENGTH_64_BYTES];
    UINT8   size;
    UINT8  *e164_crc_ptr;
    UINT8  *crlf_ptr;
} sdh_pmg_rttp_trace_t;


/*******************************************************************************
* ENUM: sdh_pmg_rttp_trace_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration to indicate which type of tail trace message to retrieve
*   in sdh_pmg_rttp_trace_get()
*
* ELEMENTS:
*    SDH_PMG_RTTP_TRACE_TYPE_EXPECTED - expected tail trace message
*    SDH_PMG_RTTP_TRACE_TYPE_ACCEPTED - accepted tail trace message
*    SDH_PMG_RTTP_TRACE_TYPE_CAPTURED - captured tail trace message
*
*******************************************************************************/
typedef enum {
    SDH_PMG_RTTP_TRACE_TYPE_EXPECTED = 0,
    SDH_PMG_RTTP_TRACE_TYPE_ACCEPTED,
    SDH_PMG_RTTP_TRACE_TYPE_CAPTURED,
    LAST_SDH_PMG_RTTP_TRACE_TYPE,
    
} sdh_pmg_rttp_trace_type_t;


/*******************************************************************************
* STRUCTURE: sdh_pmg_rttp_trace_1byte_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for setting a 1 byte trace message.
*
* ELEMENTS:
*   trace_msg[1]   - 1 byte trace message
*
*******************************************************************************/
typedef struct {
    char    trace_msg[1];
} sdh_pmg_rttp_trace_1byte_t;



/*******************************************************************************
* STRUCTURE: sdh_pmg_rttp_trace_16byte_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for setting a 16 byte trace message.
*
* ELEMENTS:
*   trace_msg[15]   - First 15 bytes of the 16 byte trace message.
*                     Byte 16 will have the E.164 CRC automatically inserted.
*
*******************************************************************************/
typedef struct {
    char    trace_msg[15];
} sdh_pmg_rttp_trace_16byte_t;



/*******************************************************************************
* STRUCTURE: sdh_pmg_rttp_trace_64byte_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for setting a 64 byte trace message.
*
* ELEMENTS:
*   trace_msg[62]   - First 62 bytes of the 64 byte trace message.
*                     Bytes 63 and 64 will have CR/LF automatically inserted.
*
*******************************************************************************/
typedef struct {
    char    trace_msg[62];
} sdh_pmg_rttp_trace_64byte_t;



/*******************************************************************************
* ENUM: sdh_pmg_prbs_oh_ins_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration for overhead bytes insertion for PRBS
*
* ELEMENTS:
*   SDH_PMG_PRBS_OH_INS_NATIONAL - Insert national byte into overhead
*   SDH_PMG_PRBS_OH_INS_E2       - Insert E2 byte into overhead
*   SDH_PMG_PRBS_OH_INS_Z2       - Insert Z2 byte into overhead
*   SDH_PMG_PRBS_OH_INS_Z1       - Insert Z1 byte into overhead
*   SDH_PMG_PRBS_OH_INS_S1       - Insert S1 byte into overhead
*   SDH_PMG_PRBS_OH_INS_D4D12    - Insert D4D12 bytes into overhead
*   SDH_PMG_PRBS_OH_INS_D1D3     - Insert D1D3 bytes into overhead
*   SDH_PMG_PRBS_OH_INS_K1K2     - Insert K1K2 bytes into overhead
*   SDH_PMG_PRBS_OH_INS_F1       - Insert F1 byte into overhead
*   SDH_PMG_PRBS_OH_INS_E1       - Insert E1 byte into overhead
*   SDH_PMG_PRBS_OH_INS_Z0       - Insert Z0 byte into overhead
*   SDH_PMG_PRBS_OH_INS_J0       - Insert J0 byte into overhead
*   
*
*******************************************************************************/
typedef enum {
    SDH_PMG_PRBS_OH_INS_NATIONAL = 0,
    SDH_PMG_PRBS_OH_INS_E2,
    SDH_PMG_PRBS_OH_INS_Z2,
    SDH_PMG_PRBS_OH_INS_Z1,
    SDH_PMG_PRBS_OH_INS_S1,
    SDH_PMG_PRBS_OH_INS_D4D12,
    SDH_PMG_PRBS_OH_INS_K1K2,
    SDH_PMG_PRBS_OH_INS_D1D3,
    SDH_PMG_PRBS_OH_INS_F1,
    SDH_PMG_PRBS_OH_INS_E1,
    SDH_PMG_PRBS_OH_INS_Z0,
    SDH_PMG_PRBS_OH_INS_J0,
    
    LAST_SDH_PMG_PRBS_OH_INS,
    
} sdh_pmg_prbs_oh_ins_t;


/*******************************************************************************
* ENUM: sdh_pmg_prbs_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration for PRBS type: monitor or generator.
*
* ELEMENTS:
*   SDH_PMG_PRBS_TYPE_RX_MONITOR      - RX monitor PRBS,
*   SDH_PMG_PRBS_TYPE_TX_GENERATOR    - TX generator PRBS
*   
*
*******************************************************************************/
typedef enum { 
    SDH_PMG_PRBS_TYPE_RX_MONITOR = 0,
    SDH_PMG_PRBS_TYPE_TX_GENERATOR,
    LAST_SDH_PMG_PRBS_TYPE,
} sdh_pmg_prbs_type_t;


/*******************************************************************************
* STRUCTURE: sdh_pmg_prbs_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for configuring SDH PRBS.
*
* ELEMENTS:
*   prbs_type   - PRBS type: RX monitor or TX generator.
*   inv_cfg     - Invert the PRBS generator and monitor.
*   enable_prbs - Enable/disable the PRBS generator and monitor
*
*******************************************************************************/
typedef struct {
    sdh_pmg_prbs_type_t     prbs_type;
    BOOL8                   inv_cfg;
    BOOL8                   enable_prbs;
} sdh_pmg_prbs_cfg_t;



/*******************************************************************************
* STRUCTURE: sdh_pmg_prbs_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for storing prbs sync status.
*
* ELEMENTS:
*   sync[3] - 192-bit array containing the SYNC status bit for the PRBS.
*             When sync is 1, the monitor's state machine is in sync for the
*             corresponding STS-1 Path.\n
*
*             The LSB (0) contains STS-1 Path #1. 
*             The MSB (192) contains STS-1 Path #192.
*
*******************************************************************************/
typedef struct {
    UINT32  sync[SDH_PMG_PRBS_SYN_ARRAY_SIZE];
} sdh_pmg_prbs_status_t;



/*******************************************************************************
* STRUCTURE: sdh_pmg_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   SDH_PMG level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8   cok2_i;
    BOOL8   lreie_i;
    BOOL8   lbipe_i;
    BOOL8   sbipe_i;
    BOOL8   cossm_i;
    BOOL8   coaps_i;
    BOOL8   apsbf_i;
    BOOL8   lrdi_i;
    BOOL8   lais_i;
    BOOL8   los_i;
    BOOL8   lof_i;
    BOOL8   oof_i;
    BOOL8   tim_i;
    BOOL8   tiu_i;
    BOOL8   sf_i;
    BOOL8   sd_i;
    
    BOOL8   apsbf_v;
    BOOL8   lrdi_v;
    BOOL8   lais_v;
    BOOL8   los_v;
    BOOL8   lof_v;
    BOOL8   oof_v;
    BOOL8   tim_v;
    BOOL8   tiu_v;
    BOOL8   sf_v;
    BOOL8   sd_v;
} sdh_pmg_int_chnl_t;




/*
** Macro Definitions
*/

#ifndef DOXYGEN_PUBLIC_ONLY 
typedef struct sdh_pmg_handle_struct_t sdh_pmg_handle_t; /*!< Opaque reference to module handle */ 
#endif

/* 
 * Add any new log codes in the following paired definitions
 * below. These will be defined as a (enum, string) pair if printing out
 * errors or if embedded (limited memory) then will just define the enum.
 * This method means that one table of log/error values and string pairs
 * can be used to generate just a list of enums or strings and enums 
 */
#define SDH_PMG_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SDH_PMG_LOG_CODE_DEBUG,    "SDH_PMG: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SDH_PMG_ERR_CODE_ASSERT,   "SDH_PMG: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SDH_PMG_LOG_CODE_LOG,      "SDH_PMG: Log logging code" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SDH_PMG_ERR_NO_SW_CONTENT, "SDH_PMG: No SW content" )   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SDH_PMG_ERR_NULL_HANDLE,   "SDH_PMG: NULL handle" )   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SDH_PMG_ERR_INVALID_ARG,   "SDH_PMG: Invalid argument" )   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SDH_PMG_ERR_INVALID_INDEX, "SDH_PMG: Index out of range" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SDH_PMG_ERR_NOT_PROVISIONED, "SDH_PMG: Index not provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SDH_PMG_ERR_RTTP_ALGO3_LEN_MISMATCH,  "SDH_PMG: RTTP Algo 3 must only be used with 1 byte message" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SDH_PMG_ERR_RTTP_LEN_MISMATCH,        "SDH_PMG: RTTP trace message length does not match config" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SDH_PMG_ERR_RTTP_LEN_NOT_CFG,         "SDH_PMG: RTTP length needs to be configured first" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SDH_PMG_ERR_RTTP_ALGO_DISABLED,       "SDH_PMG: RTTP monitoring is disabled" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SDH_PMG_ERR_RTTP_ACCEPTED_MISMATCH,   "SDH_PMG: RTTP accept message is only availble for Algo 2" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SDH_PMG_ERR_RTTP_CRLF_NOT_ADJ,        "SDH_PMG: RTTP CR/LF in extracted message is not adjacent" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SDH_PMG_ERR_RTTP_CRLF_NOT_FOUND,      "SDH_PMG: RTTP CR/LF in extracted message was not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SDH_PMG_ERR_RTTP_CRC_NOT_FOUND,       "SDH_PMG: RTTP CRC in extracted message was not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SDH_PMG_ERR_RTTP_INVALID_MSG_CHAR,    "SDH_PMG: RTTP trace msg char must be less than 0x80" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SDH_PMG_ERR_SBER_INVALID_BER,         "SDH_PMG: SBER Invalid BER for alarm/sts combination") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SDH_PMG_ERR_INVALID_STS_768_INDEX,    "SDH_PMG: Invalid index for STS768. Must be 0." ) \

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum SDH_PMG_LOG_ERR_INDEX
{
    SDH_PMG_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_SDH_PMG << 16),
    SDH_PMG_LOG_ERR_TABLE
    SDH_PMG_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char SDH_PMG_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char SDH_PMG_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */


#endif /* DOXYGEN_PUBLIC_ONLY */
/*
** Global variables
*/

/*
** Function Prototypes
*/

#ifdef __cplusplus
}
#endif
#endif /* _SDH_PMG_API_H */

/*
** end of file
*/
