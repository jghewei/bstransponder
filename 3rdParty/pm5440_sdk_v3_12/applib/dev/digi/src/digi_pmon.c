/*******************************************************************************
*   COPYRIGHT (C) 2012 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*   DESCRIPTION : This module contains PMON related host messages.
*
*   NOTES:
*
*******************************************************************************/
#include "digi_loc.h"
#include "hostmsg_loc.h"


/******************************************************************************* 
 *
 * Constants.
 *
 ******************************************************************************/
/* Length of pmon host messages */
#define HOSTMSG_CMD_CFG_PMON_CONFIG_COLLECTION_MODE_LEN        0x7
#define HOSTMSG_CMD_CFG_PMON_CONFIG_WORK_ORDER_LEN             0x4
#define HOSTMSG_CMD_CFG_PMON_CONFIG_DDEG_COREOTN_SEG_LEN       0x3
#define HOSTMSG_CMD_CFG_PMON_CONFIG_DDEG_PARAM_LEN             0xE
#define HOSTMSG_CMD_CFG_PMON_CONFIG_DDEG_ENABLE_LEN            0xC
#define HOSTMSG_CMD_CFG_PMON_CONFIG_DDEG_INT_ENABLE_LEN        0xC
#define HOSTMSG_CMD_REQ_PMON_DDEG_INT_GET_LEN                  0x3
#define HOSTMSG_CMD_REQ_PMON_DDEG_PARAM_GET_LEN                0x3
#define HOSTMSG_CMD_REQ_PMON_DDEG_ENABLE_GET_LEN               0x3
#define HOSTMSG_CMD_CFG_PMON_FW_FEC_LEN                        0x3
#define HOSTMSG_CMD_CFG_PMON_CONFIG_HW_DDEG_LCLK_LEN           0x3
#define HOSTMSG_CMD_CFG_PMON_FW_FEC_MANUAL_UPDATE_LEN          0x3
#define HOSTMSG_CMD_CFG_PMON_MANUAL_TRIGGER_LEN                0x3
#define HOSTMSG_CMD_CFG_PMON_SET_RCP_LOS_LEN                   0x3
#define HOSTMSG_CMD_REQ_PMON_FW_SM_STATUS_GET_LEN              0x3
#define HOSTMSG_CMD_REQ_PMON_DDEG_STATE_GET_LEN                0x3
#define HOSTMSG_CMD_CFG_PMON_PRBS_COLLECTION_MODE_LEN          0x3

/* PMON_INT(pmon_type, struct_field) */
#define PMON_INT_TABLE_DEFINE()     \
    PMON_INT(MISC, misc_i);         \
    PMON_INT(OTN,  otn_i);          \

#define DIGI_PMON_IPI_BITS_PER_INT_REG              16
#define DIGI_PMON_IPI_REG_OFFSET_GET(ipi_bit_num)   (ipi_bit_num % DIGI_PMON_IPI_BITS_PER_INT_REG)


/* IPI BITS location */
#define DIGI_PMON_INT_IPI_BIT_OTN                           16
#define DIGI_PMON_INT_IPI_BIT_MISC                          17
#define DIGI_PMON_INT_IPI_BIT_DDEG_ENH                      18 
#define DIGI_PMON_INT_IPI_BIT_DDEG_BURSTY                   19


/*
** Local Structures and Unions
*/
/*! Declare function types for IPI interrupt get operation */
typedef PMC_ERROR (*digi_pmon_ipi_int_get_fcn) (LOG_CALLER_DEFN
                                                int_agr_buffer_t *b_ptr,
                                                pmc_handle_t *h_ptr);

/*! Declare function types for IPI interrupt set to clear operation */
typedef void (*digi_pmon_ipi_int_set_to_clear_fcn) (LOG_CALLER_DEFN
                                                    int_agr_buffer_t *b_ptr,
                                                    pmc_handle_t *h_ptr,
                                                    UINT32 value);

/*! Declare function types for IPI interrupt enable operation */
typedef void (*digi_pmon_ipi_int_en_fcn) (LOG_CALLER_DEFN
                                          int_agr_buffer_t *b_ptr,
                                          pmc_handle_t *h_ptr,
                                          UINT32 start_bit,
                                          UINT32 stop_bit,
                                          UINT32 value);

/*
** Private functions
*/

#ifndef DOXYGEN_PUBLIC_ONLY
PRIVATE PMC_ERROR digi_pmon_shared_mem_clear(digi_handle_t *digi_handle);
#endif /*DOXYGEN_PUBLIC_ONLY*/

/*******************************************************************************
* STRUCTURE: digi_pmon_ipi_bit_info_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Stores mapping of IPI bit access information
*
* ELEMENTS:
*   ipi_bit_num     - IPI bit number (0 to 63)
*   reg_offset      - IPI bit register offset. The interrupts corresponding to 
*                     the 64 IPI bits are split up into 4 16-bit interrupt registers.
*                     This is the offset from the beginning of int register
*                     corresponding to this IPI bit.
*   ipi_int_get     - Function pointer to the get function for the int register
*                     corresponding to this IPI bit.
*   ipi_int_clear   - Function pointer to the clear function for the int register
*                     corresponding to this IPI bit.
*   ipi_int_en      - Function pointer to the enable function for the int register
*                     corresponding to this IPI bit.
*   ipi_int_en      - Function pointer to the get enable function for the int register
*                     corresponding to this IPI bit.
*******************************************************************************/
typedef struct digi_pmon_ipi_bit_info_t 
{
    UINT32                              ipi_bit_num;
    UINT32                              reg_offset;
    digi_pmon_ipi_int_get_fcn           ipi_int_get;
    digi_pmon_ipi_int_set_to_clear_fcn  ipi_int_clear;
    digi_pmon_ipi_int_en_fcn            ipi_int_en;

} digi_pmon_ipi_bit_info_t;


#ifndef DOXYGEN_PUBLIC_ONLY

/*******************************************************************************
*  digi_pmon_int_agr_field_IPI2PCIE_1_I1_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This function gets the value of the IPI2PCIE_1_I1 field
* 
*  INPUTS:
*   b_ptr - buffer pointer if colascing operations
*   h_ptr - handle to access the device
*
*  OUTPUTS:
*   None.
*   
*  RETURNS:
*   UINT32 - value read
*
*  NOTES:
*
******************************************************************************/
static UINT32 digi_pmon_int_agr_field_IPI2PCIE_1_I1_get( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
{                         
    return int_agr_field_IPI2PCIE_1_I1_get(b_ptr,h_ptr);
}

/*******************************************************************************
*  digi_pmon_int_agr_field_IPI2PCIE_1_I1_set_to_clear
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This function clears the value of the IPI2PCIE_1_I1 field
* 
*  INPUTS:
*   b_ptr - buffer pointer if colascing operations
*   h_ptr - handle to access the device
*   value - value to write (typically 0)
*
*  OUTPUTS:
*   None
*   
*  RETURNS:
*   None
*
*  NOTES:
*
******************************************************************************/
static void digi_pmon_int_agr_field_IPI2PCIE_1_I1_set_to_clear( int_agr_buffer_t *b_ptr,
                                                             pmc_handle_t *h_ptr,
                                                             UINT32 value )
{
    int_agr_field_IPI2PCIE_1_I1_set_to_clear(b_ptr,h_ptr,value);
}

/*******************************************************************************
*  digi_pmon_int_agr_field_range_IPI2PCIE_1_E1_set
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This function sets the value of the IPI2PCIE_1_E1 field
* 
*  INPUTS:
*   b_ptr - buffer pointer if colascing operations
*   h_ptr - handle to access the device
*   start_bit - bit location where the field starts
*   stop_bit - bit location where the field ends
*   value - value for the field
*
*  OUTPUTS:
*   None
*   
*  RETURNS:
*   None
*
*  NOTES:
*
******************************************************************************/
static INLINE void digi_pmon_int_agr_field_range_IPI2PCIE_1_E1_set( int_agr_buffer_t *b_ptr,
                                                          pmc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    int_agr_field_range_IPI2PCIE_1_E1_set(b_ptr,h_ptr,start_bit,stop_bit,value);
}
#endif /*DOXYGEN_PUBLIC_ONLY*/

/*! Description of the IPI interrupt table (SW/FW defined interrupts */
PRIVATE digi_pmon_ipi_bit_info_t    ipi_bit_tbl[LAST_DIGI_PMON_IPI_TYPE] =
{
    /* DIGI_PMON_IPI_TYPE_PMON_OTN */
    { DIGI_PMON_INT_IPI_BIT_OTN,                              /* ipi_bit_num */
      DIGI_PMON_IPI_REG_OFFSET_GET(DIGI_PMON_INT_IPI_BIT_OTN), /* reg_offset */
      digi_pmon_int_agr_field_IPI2PCIE_1_I1_get,              /* ipi_int_get */
      digi_pmon_int_agr_field_IPI2PCIE_1_I1_set_to_clear,     /* ipi_int_clear */
      digi_pmon_int_agr_field_range_IPI2PCIE_1_E1_set,        /* ipi_E1 */
    },
    
    /* DIGI_PMON_IPI_TYPE_PMON_MISC */
    { DIGI_PMON_INT_IPI_BIT_MISC,                             /* ipi_bit_num */
      DIGI_PMON_IPI_REG_OFFSET_GET(DIGI_PMON_INT_IPI_BIT_MISC), /* reg_offset */
      digi_pmon_int_agr_field_IPI2PCIE_1_I1_get,              /* ipi_int_get */
      digi_pmon_int_agr_field_IPI2PCIE_1_I1_set_to_clear,     /* ipi_int_clear */
      digi_pmon_int_agr_field_range_IPI2PCIE_1_E1_set,        /* ipi_E1 */
    },
    
    /* DIGI_PMON_IPI_TYPE_DDEG_ENH */
    { DIGI_PMON_INT_IPI_BIT_DDEG_ENH,                         /* ipi_bit_num */
      DIGI_PMON_IPI_REG_OFFSET_GET(DIGI_PMON_INT_IPI_BIT_DDEG_ENH), /* reg_offset */
      digi_pmon_int_agr_field_IPI2PCIE_1_I1_get,              /* ipi_int_get */
      digi_pmon_int_agr_field_IPI2PCIE_1_I1_set_to_clear,     /* ipi_int_clear */
      digi_pmon_int_agr_field_range_IPI2PCIE_1_E1_set,        /* ipi_E1 */
    },
    
    /* DIGI_PMON_IPI_TYPE_DDEG_BURSTY */
    { DIGI_PMON_INT_IPI_BIT_DDEG_BURSTY,                      /* ipi_bit_num */
      DIGI_PMON_IPI_REG_OFFSET_GET(DIGI_PMON_INT_IPI_BIT_DDEG_BURSTY), /* reg_offset */
      digi_pmon_int_agr_field_IPI2PCIE_1_I1_get,              /* ipi_int_get */
      digi_pmon_int_agr_field_IPI2PCIE_1_I1_set_to_clear,     /* ipi_int_clear */
      digi_pmon_int_agr_field_range_IPI2PCIE_1_E1_set,        /* ipi_int_en */
    },
};

/*
** Forward References
*/



/*******************************************************************************
 *
 * Private methods.
 *
 ******************************************************************************/
#ifndef DOXYGEN_PUBLIC_ONLY
PRIVATE PMC_ERROR digi_pmon_shared_mem_size_internal_get(digi_handle_t    *digi_handle,
                                                          UINT32           *size_ptr);
PRIVATE PMC_ERROR digi_pmon_ipi_bit_check(digi_handle_t    *digi_handle,
                                          UINT32            ipi_bit_value);
#endif


/*******************************************************************************
 *
 * Public methods.
 *
 ******************************************************************************/

/*******************************************************************************
*  digi_pmon_collection_mode_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION: 
*               This method configures pmon collection mode.
*               Note that automatic triggering only configures the Misc LCLK
*               to automatically latch the non-OTN PMON.
*               OTN PMON must always be manually triggered by calling
*               digi_pmon_manual_trigger_cfg().
*
*               If hardware dDEG is enabled and any of the three OTN PMON work orders are configured,
*               then the PMON collection mode must be auto with period 1000 milliseconds.
*
*               e.g. the following is ok:
*                digi_pmon_ddeg_cfg_hw_ddeg_lclk(digi_handle,TRUE);
*                digi_pmon_collection_mode_cfg(digi_handle,pmon_trig_mode=1,,trig_period=1000,...);
*                digi_pmon_work_order_cfg(digi_handle,work_order={enable_hw_block_msk=
*                                         DIGI_PMON_WORK_ORDER_LINEOTN|
*                                         DIGI_PMON_WORK_ORDER_SYSOTN|
*                                         DIGI_PMON_WORK_ORDER_COREOTN_ERR_BLK_COUNTS});
*
*  INPUTS: 
*               digi_handle - digi device handle. 
*               pmon_trig_mode - PMON Trigger Mode\n
*                                0 -- manual trigger\n
*                                1 -- automatic trigger\n
*               pmon_sync_en - Enable or disable sync of automatic PMON trigger\n
*                              0 -- Disable sync of automatic PMON trigger to\n
*                                   external PMON_SYNC_I pin.\n
*                              1 -- Enable sync of automatic PMON trigger to\n 
*                                   external PMON_SYNC_I pin.\n
*               trig_period - Trigger period in miliseconds. Valid values \n
*                             are 250, 500 and 1000. PMON data will be\n
*                             transferred at the specified interval. 
*               data_store_phys_start_addr - Physical address of the PMON data 
*                                            store. This address cannot be 0x0.
*                                            Also, the lowest 
*                                            PMC_MEM_POOL_ALIGN_PMON 
*                                            bits of  this address must be 
*                                            all 0s.
*               data_store_vir_start_addr -  Virtual address of the PMON data 
*                                            store.
*               data_buf_size - Host buffer size in 32-bit words, e.g. a value 
*                               of 1 indicates 4 bytes.
*               num_data_buf - number of data buffers. Currently only 1 buffer is 
*                              supported.
*
*  OUTPUTS: 
*               digi_handle - digi_handle->digi_pmon_mode_profile is set
*               data_store_phys_start_addr - start address of the DIGI SPRAM data store.
*               data_buf_size - single data store buffer size.
*               num_data_buf - number of data store data buffers.
* 
*  RETURNS: 
                PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES: 
*               CMD_CFG_PMON_Config_Collection_Mode. 
*               Output exists only if mode requested was spram.
**              
**              A special case of pmon_trig_mode = 1 AND trig_period = 0 AND pmon_sync_en=1
**              can be used to configure external PMON triggering.
**
**
*******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_collection_mode_cfg(digi_handle_t *digi_handle,
                                               UINT32 pmon_trig_mode,
                                               UINT32 pmon_sync_en,
                                               UINT32 trig_period,
                                               UINT64 data_store_phys_start_addr,
                                               void  *data_store_vir_start_addr,
                                               UINT32 data_buf_size,
                                               UINT32 num_data_buf)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[5];
    digi_pmon_mode_profile_t *digi_pmon_mode_profile;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    digi_pmon_mode_profile = digi_handle->var.digi_pmon_mode_profile;    

    /* Fill in the structure for creating host message body later */
    digi_pmon_mode_profile->pmon_trig_mode = pmon_trig_mode;
    digi_pmon_mode_profile->pmon_sync_en   = pmon_sync_en;
    digi_pmon_mode_profile->trig_period    = trig_period;
    digi_pmon_mode_profile->data_store_phys_start_addr = data_store_phys_start_addr;
    digi_pmon_mode_profile->data_store_vir_start_addr = data_store_vir_start_addr;
    digi_pmon_mode_profile->data_buf_size = data_buf_size;
    digi_pmon_mode_profile->num_data_buf  = num_data_buf;

    /* CMD_CFG_PMON_Config_Collection_Mode has the following format:
     * Word  bits 24..31      bits 16..23   bits 0..15
     *   0  pmon_trig_mode  pmon_sync_en   trig_period
     *   1  buffer_physical_addr_msb
     *   2  buffer_physical_addr_lsb
     *   3  buffer_size
     *   4  num_buffers
     */
    data[0] = (((UINT8)pmon_trig_mode) << 24) | 
                 ((UINT8)pmon_sync_en << 16) | 
                 ((UINT16)trig_period);

    data[1] = ((UINT32)((data_store_phys_start_addr & 0xffffffff00000000ULL ) >> 32));
    data[2] = ((UINT32)(data_store_phys_start_addr & 0x00000000ffffffffULL));

    /* Add buffer description. */
    data[3] = data_buf_size;
    data[4] = num_data_buf;

    /* Send message*/
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PMON,HOSTMSG_CMD_PMON_CFG_COLLECTION_MODE_ST,data,5,stat);
    }  

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_pmon_collection_mode_cfg*/

/*******************************************************************************
*  digi_pmon_work_order_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This method sends host message to FW to configure PMON data
*               transfered to host memory. This message allows the user to
*               enable/disable PMON collection from individual hardware blocks.
*
*               If hardware dDEG is enabled and any of the three OTN PMON work orders are configured,
*               then the PMON collection mode must be auto with period 1000 milliseconds.
*
*               e.g. the following is ok:
*                digi_pmon_ddeg_cfg_hw_ddeg_lclk(digi_handle,TRUE);
*                digi_pmon_collection_mode_cfg(digi_handle,pmon_trig_mode=1,,trig_period=1000,...);
*                digi_pmon_work_order_cfg(digi_handle,work_order={enable_hw_block_msk=
*                                         DIGI_PMON_WORK_ORDER_LINEOTN|
*                                         DIGI_PMON_WORK_ORDER_SYSOTN|
*                                         DIGI_PMON_WORK_ORDER_COREOTN_ERR_BLK_COUNTS});
*
*  INPUTS:
*               digi_handle - System handle.
*               work_order - PMON work order. 
*
*  OUTPUTS:
*               None.
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_PMON_Config_Work_Order
*
******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_work_order_cfg(digi_handle_t *digi_handle,
                                          digi_pmon_work_order_msg_t *work_order)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[2];

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != work_order), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* CMD_CFG_PMON_Config_Collection_Mode has the following format:
     * Word  bits 0..31
     *   0  enable_hw_block_msk
     *   1  disable_hw_block_msk
     */
    data[0] = work_order->enable_hw_block_msk;
    data[1] = work_order->disable_hw_block_msk;

    /* Send message*/
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PMON,HOSTMSG_CMD_PMON_CFG_WORK_ORDER_ST,data,2,stat);
    }  

    PMC_ATOMIC_RETURN(digi_handle,result);

}/* End of digi_pmon_work_order_cfg*/

/*******************************************************************************
*  digi_pmon_ddeg_coreotn_seg_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               Sends host message to FW to select the COREOTN segment
*               to monitor for dDEG.
*
*  INPUTS:
*               digi_handle - System handle.
*               mon_coreotn_stg_4 - TRUE: Monitor COREOTN Stage 4
*                                   FALSE: Monitor COREOTN Stages 1, 2, N 
*                                          and 3a
*
*  OUTPUTS:
*               None.
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_PMON_Config_dDEG_COREOTN_SEG
******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_ddeg_coreotn_seg_cfg(digi_handle_t *digi_handle,
                                                BOOL mon_coreotn_stg_4)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Build the rest of the message */
    if (mon_coreotn_stg_4) {
        data[0] = 0x1 << 24;
    } else {
        data[0] = 0x0 << 24;
    } /* end if */

    /* Send message*/
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PMON,HOSTMSG_CMD_PMON_CFG_DDEG_COREOTN_SEG_ST,data,1,stat);
    }  

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_pmon_ddeg_coreotn_seg_cfg */

/*******************************************************************************
*
*  digi_pmon_ddeg_param_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               Sends host message to FW to configure dDEG parameters
*
*  INPUTS: 
*   digi_handle            - System handle.
*   ddeg_mode              - dDEG mode to configure parameters for
*   ddeg_hw_block          - Hardware block to configure 
*                            (maximum number of channels):
*                            LINEOTN (12)                   
*                            SYSOTN (1)                     
*                            COREOTN Stage 1 PM (12)        
*                            COREOTN Stage 1 TCM (72)       
*                            COREOTN Stage 2 PM (96)        
*                            COREOTN Stage 2 TCM (256)      
*                            COREOTN Stage N PM (1)         
*                            COREOTN Stage N TCM (6)        
*                            COREOTN Stage 3a / 4 PM (96)   
*                            COREOTN Stage 3a / 4 TCM (256) 
*   ddeg_chnl              - Bitmask of channels to configure. A '1' in a bit 
*                            position indicates that the channel should be 
*                            configured with the monitoring interval and 
*                            thresholds specified in the host message. 
*   mon_intvl              - Monitoring interval. If ddeg_mode = 0 
*                            (Enhanced dDEG), the following values are valid:
*                            0: 10ms  
*                            1: 100ms 
*                            2: 1000ms
*                            If ddeg_mode = 1 (Bursty dDEG), this parameter 
*                            is the value for DEGM (number of 1-second 
*                            interval). Values of 2-10 are valid.
*   set_threshold          - Threshold of Near End Block Errors for each 
*                            1: second monitoring interval used to declare 
*                            a bad interval. An interval is declared bad if 
*                            the number of errors >= set_threshold.
*   clear_threshold        - Threshold of Near End Block Errors for each 
*                            1: second monitoring interval used to declare 
*                            a good interval. An interval is declared good 
*                            if the number of errors < clear_threshold.
*                            clear_threshold must be less than set_threshold.
*                            Only valid if ddeg_mode = 0 (Enhanced dDEG). 
*                            Bursty (G.798) dDEG always uses 
*                            clear_threshold = (set_threshold - 1)
*   mon_intvl_valid        - boolean value to indicate that mon_intvl is being
*                            configured or not. Only when 'mon_intvl_valid' is
*                            set to TRUE, the value provided in 'mon_intvl' 
*                            will be used. When 'mon_intvl_valid' is set to
*                            FALSE, the value provided in 'mon_intvl' is 
*                            ignored by FW.
*   set_threshold_valid    - boolean value to indicate that 'set_threshold' is 
*                            being configured or not. 
*                            Only when 'set_threshold_valid' is set to TRUE,
*                            the value provided in 'set_threshold' will be used.
*                            When 'set_threshold_valid' is set to FALSE, the value
*                            provided in 'set_threshold' is ignored by FW.                           
*   clear_threshold_valid  - boolean value to indicate that clear_threshold is 
*                            being configured or not. 
*                            Only when 'clear_threshold_valid' is set to TRUE,
*                            the value provided in 'clear_threshold' will be 
*                            used. When 'clear_threshold_valid' is set to
*                            FALSE, the value provided in 'clear_threshold' is 
*                            ignored by FW.
*
*  OUTPUTS:
*               None.
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_PMON_Config_dDEG_Param
*
******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_ddeg_param_cfg(digi_handle_t *digi_handle,
                                          digi_pmon_ddeg_mode_t ddeg_mode,
                                          digi_pmon_ddeg_hw_block_t ddeg_hw_block,
                                          digi_pmon_ddeg_chnl_bitmsk_t ddeg_chnl,
                                          UINT8 mon_intvl, UINT32 set_threshold,
                                          UINT32 clear_threshold,
                                          BOOL8 mon_intvl_valid,
                                          BOOL8 set_threshold_valid,
                                          BOOL8 clear_threshold_valid)
{
    PMC_ERROR result = PMC_SUCCESS; 
    UINT32 data[12]; 

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Build the rest of the message */
    data[0] = ((UINT8)ddeg_mode << 24) | ((UINT8)ddeg_hw_block << 16);
    memcpy(&data[1], &ddeg_chnl.chnl_bitmsk[0], sizeof(ddeg_chnl.chnl_bitmsk));
    data[9] = ((UINT8)mon_intvl_valid << 24) | (mon_intvl); /* byte 0 of word 11-13 is for boolean to indicate if this is valid */
    data[10] = ((UINT8)set_threshold_valid << 24) | (set_threshold);
    data[11] = ((UINT8)clear_threshold_valid << 24) | (clear_threshold);

    /* Send message*/
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PMON,HOSTMSG_CMD_PMON_CFG_DDEG_PARAM_ST,data,12,stat);
    }   

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_pmon_ddeg_param_cfg */

/*******************************************************************************
*  digi_pmon_ddeg_en_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               Sends host message to FW to enable/disable dDEG on individual
*               channels. When dDEG is enabled on a channel, firmware will
*               monitor the channel and assert the dDEG interrupt if the 
*               defect occurs.
*               FW supports enabling both enhanced dDEG and bursty dDEG
*               on the same channel at the same time.
*               This host message must be called multiple times to enable
*               dDEG for the different hardware blocks.
*
*  INPUTS:
*          digi_handle   - System handle.
*          ddeg_mode     - dDEG mode to configure channels for
*          ddeg_hw_block - Hardware block to configure 
*                          (maximum number of channels):\n
*                          LINEOTN (12)\n
*                          SYSOTN (1)\n                 
*                          COREOTN Stage 1 PM (12)\n
*                          COREOTN Stage 1 TCM (72)\n
*                          COREOTN Stage 2 PM (96)\n        
*                          COREOTN Stage 2 TCM (256)\n      
*                          COREOTN Stage N PM (1)\n         
*                          COREOTN Stage N TCM (6)\n        
*                          COREOTN Stage 3a / 4 PM (96)\n   
*                          COREOTN Stage 3a / 4 TCM (256) 
*          ddeg_chnl - Bitmask of channels to configure. A '1' in a bit 
*                      position indicates that the channel should be 
*                      configured  
*          enable - FALSE: Disable dDEG on specified channels\n
*                   TRUE:  Enable dDEG on specified channels\n
*                   On device power-up, dDEG is disabled on all channels.
*
*  OUTPUTS:
*           None.
* 
*  RETURNS:
*           PMC_SUCCESS if everything went OK, number of error codes
*           otherwise.
*
*  NOTES:
*           CMD_CFG_PMON_Config_dDEG_Enable
*
******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_ddeg_en_cfg(digi_handle_t *digi_handle,
                                       digi_pmon_ddeg_mode_t ddeg_mode,
                                       digi_pmon_ddeg_hw_block_t ddeg_hw_block,
                                       digi_pmon_ddeg_chnl_bitmsk_t ddeg_chnl,
                                       BOOL enable)
{
    PMC_ERROR result = PMC_SUCCESS;   
    UINT32 data[10];

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Build the rest of the message */
    data[0] = ((UINT8)ddeg_mode << 24) | ((UINT8)ddeg_hw_block << 16);
    memcpy(&data[1], &ddeg_chnl.chnl_bitmsk[0], sizeof(ddeg_chnl.chnl_bitmsk));
    if (enable) {
        data[9] = 0x1 << 24;
    } else {
        data[9] = 0x0 << 24;
    } /* end if */

    /* Send message*/
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PMON,HOSTMSG_CMD_PMON_CFG_DDEG_ENABLE_ST,data,10,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_pmon_ddeg_en_cfg */

/*******************************************************************************
*  digi_pmon_ddeg_int_en_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               Sends host message to FW to enable/disable interrupt propagation
*               for Enhanced or Bursty (G.798) dDEG for each channel. 
*               When interrupt propagation is enabled, if an interrupt is 
*               asserted on a channel, firmware will assert the corresponding 
*               IPI interrupt to notify the host. The following IPI 
*               bits are used:\n
*                Enhanced dDEG - IPI bit 2 in set 2 (INTER_PROCESSOR_INTS, register 0x1801278)\n
*                Bursty (G.798) dDEG - IPI bit 3 in set 2 (INTER_PROCESSOR_INTS, register 0x180127C)
*
*  INPUTS:
*          digi_handle   - System handle.
*          ddeg_mode     - dDEG mode to configure channels for
*          ddeg_hw_block - Hardware block to configure 
*                          (maximum number of channels):
*                          LINEOTN (12)\n
*                          SYSOTN (1)\n                 
*                          COREOTN Stage 1 PM (12)\n
*                          COREOTN Stage 1 TCM (72)\n      
*                          COREOTN Stage 2 PM (96)\n     
*                          COREOTN Stage 2 TCM (256)\n      
*                          COREOTN Stage N PM (1)\n    
*                          COREOTN Stage N TCM (6)\n       
*                          COREOTN Stage 3a / 4 PM (96)\n
*                          COREOTN Stage 3a / 4 TCM (256) 
*          ddeg_chnl - Bitmask of channels to configure. A '1' in a bit 
*                      position indicates that the channel should be 
*                      configured  
*          int_enable - FALSE: Disable interrupt propagation for 
*                               specified channels\n
*                       TRUE: Enable interrupt propagation for 
*                             specified channels.\n
*                       On device power-up, interrupt propagation 
*                       is disabled for all channels.
*
*  OUTPUTS:
*               None.
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_PMON_Config_dDEG_Interrupt_Enable
*
******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_ddeg_int_en_cfg(digi_handle_t *digi_handle,
                                           digi_pmon_ddeg_mode_t ddeg_mode,        
                                           digi_pmon_ddeg_hw_block_t ddeg_hw_block,
                                           digi_pmon_ddeg_chnl_bitmsk_t ddeg_chnl, 
                                           BOOL int_enable)                        
{
    PMC_ERROR result = PMC_SUCCESS; 
    UINT32 data[10];

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Build the rest of the message */
    data[0] = ((UINT8)ddeg_mode << 24) | ((UINT8)ddeg_hw_block << 16);
    memcpy(&data[1], &ddeg_chnl.chnl_bitmsk[0], sizeof(ddeg_chnl.chnl_bitmsk));
    if (int_enable) {
        data[9] = 0x1 << 24;
    } else {
        data[9] = 0x0 << 24;
    } /* end if */

    /* Send message*/
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PMON,HOSTMSG_CMD_PMON_CFG_DDEG_INT_ENABLE_ST,data,10,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_pmon_ddeg_int_en_cfg */

/*******************************************************************************
* digi_pmon_ddeg_int_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves dDEG interrupts from firmware.
*
* INPUTS:
*  digi_handle - DIGI device handle.
*  ddeg_mode   - dDEG mode to read interrupts from
*  int_type    - Interrupt type to read:
*                0: Interrupt value (V-bit). Indicates whether the dDEG 
*                    interrupt is asserted for the current channel.
*                1: Interrupt status (I-bit). Indicates whether there has 
*                    been a change of state in the dDEG interrupt for the 
*                    current channel. The I-bit is cleared after reading.
*
* OUTPUTS:
*  digi_pmon_ddeg_ints_ptr - pointer to storage for returned status.\n
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:      CMD_REQ_PMON_Get_dDEG_Interrupts
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_ddeg_int_get(digi_handle_t *digi_handle,
                                        digi_pmon_ddeg_mode_t ddeg_mode,
                                        UINT8 int_type,
                                        digi_pmon_ddeg_ints_t *digi_pmon_ddeg_ints_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];
    UINT32 response[62];

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Build the rest of the message */
    data[0] = ((UINT8)ddeg_mode << 24) | (int_type << 16);
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_variable_length_query((pmc_handle_t *)digi_handle,MODID_PMON,HOSTMSG_CMD_PMON_REQ_DDEG_INT_ST,data,1,response,62,NULL,stat);
    }

    if (PMC_SUCCESS == result)
    {
        UINT32 *data_ptr = &(response[0]);

        /* Return requested data. */
        digi_pmon_ddeg_ints_ptr->ddeg_mode = (digi_pmon_ddeg_mode_t)((*data_ptr >> 24) & 0xff);
        digi_pmon_ddeg_ints_ptr->int_type = (UINT8)((*data_ptr >> 16) & 0xff);
        data_ptr++;
        digi_pmon_ddeg_ints_ptr->lineotn_int = *data_ptr;
        data_ptr++;
        digi_pmon_ddeg_ints_ptr->sysotn_int = *data_ptr;
        data_ptr++;
        digi_pmon_ddeg_ints_ptr->coreotn_stg_1_pm_int = *data_ptr;
        data_ptr++;
        memcpy(digi_pmon_ddeg_ints_ptr->coreotn_stg_1_tcm_int, data_ptr, sizeof(digi_pmon_ddeg_ints_ptr->coreotn_stg_1_tcm_int));
        data_ptr += sizeof(digi_pmon_ddeg_ints_ptr->coreotn_stg_1_tcm_int) / sizeof(UINT32);
        memcpy(digi_pmon_ddeg_ints_ptr->coreotn_stg_2_pm_int, data_ptr, sizeof(digi_pmon_ddeg_ints_ptr->coreotn_stg_2_pm_int));
        data_ptr += sizeof(digi_pmon_ddeg_ints_ptr->coreotn_stg_2_pm_int) / sizeof(UINT32);
        memcpy(digi_pmon_ddeg_ints_ptr->coreotn_stg_2_tcm_int, data_ptr, sizeof(digi_pmon_ddeg_ints_ptr->coreotn_stg_2_tcm_int));
        data_ptr += sizeof(digi_pmon_ddeg_ints_ptr->coreotn_stg_2_tcm_int) / sizeof(UINT32);
        digi_pmon_ddeg_ints_ptr->coreotn_stg_n_pm_int = *data_ptr;
        data_ptr++;
        digi_pmon_ddeg_ints_ptr->coreotn_stg_n_tcm_int = *data_ptr;
        data_ptr++;
        memcpy(digi_pmon_ddeg_ints_ptr->coreotn_stg_3a_4_pm_int, data_ptr, sizeof(digi_pmon_ddeg_ints_ptr->coreotn_stg_3a_4_pm_int));
        data_ptr += sizeof(digi_pmon_ddeg_ints_ptr->coreotn_stg_3a_4_pm_int) / sizeof(UINT32);
        memcpy(digi_pmon_ddeg_ints_ptr->coreotn_stg_3a_4_tcm_int, data_ptr, sizeof(digi_pmon_ddeg_ints_ptr->coreotn_stg_3a_4_tcm_int));
        data_ptr += sizeof(digi_pmon_ddeg_ints_ptr->coreotn_stg_3a_4_tcm_int) / sizeof(UINT32);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_pmon_ddeg_int_get() */

/*******************************************************************************
* digi_pmon_ddeg_param_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves dDEG parameters from firmware.
*
* INPUTS:
*  digi_handle - DIGI device handle.
*  ddeg_mode   - dDEG mode to read parameters from
*  ddeg_hw_block - Hardware block to read
*                  (maximum number of channels):
*                  LINEOTN (12)                   
*                  SYSOTN (1)                     
*                  COREOTN Stage 1 PM (12)        
*                  COREOTN Stage 1 TCM (72)       
*                  COREOTN Stage 2 PM (96)        
*                  COREOTN Stage 2 TCM (256)      
*                  COREOTN Stage N PM (1)         
*                  COREOTN Stage N TCM (6)        
*                  COREOTN Stage 3a / 4 PM (96)   
*                  COREOTN Stage 3a / 4 TCM (256) 
*  chnl - Channel to read in the specified hardware block. 
*         The first channel starts from channel 0.
*
* OUTPUTS:
*  digi_pmon_ddeg_param_ptr - pointer to storage for returned status.\n
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:      CMD_REQ_PMON_Get_dDEG_Param
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_ddeg_param_get(digi_handle_t *digi_handle,
                                          digi_pmon_ddeg_mode_t ddeg_mode,
                                          digi_pmon_ddeg_hw_block_t ddeg_hw_block,
                                          UINT8 chnl,
                                          digi_pmon_ddeg_param_t *digi_pmon_ddeg_param_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];
    UINT32 response[4];

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Build the rest of the message */
    data[0] = ((UINT8)ddeg_mode << 24) | ((UINT8)ddeg_hw_block << 16) | (chnl << 8);
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_variable_length_query((pmc_handle_t *)digi_handle,MODID_PMON,HOSTMSG_CMD_PMON_REQ_DDEG_PARAM_ST,data,1,response,4,NULL,stat);
    }

    if (PMC_SUCCESS == result)
    {
        /* Return requested data. */
        digi_pmon_ddeg_param_ptr->ddeg_mode = (digi_pmon_ddeg_mode_t)((response[0] >> 24) & 0xff);
        digi_pmon_ddeg_param_ptr->ddeg_hw_block = (digi_pmon_ddeg_hw_block_t)((response[0] >> 16) & 0xff);
        digi_pmon_ddeg_param_ptr->chnl = (UINT8)((response[0] >> 8) & 0xff);

        digi_pmon_ddeg_param_ptr->mon_intvl = (UINT8)((response[1] >> 24) & 0xff);
        digi_pmon_ddeg_param_ptr->mon_coreotn_stg_4 = (BOOL)((response[1] >> 16) & 0xff);

        digi_pmon_ddeg_param_ptr->set_threshold = response[2];
        digi_pmon_ddeg_param_ptr->clear_threshold = response[3];
    } /* end if */

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_pmon_ddeg_param_get() */


/*******************************************************************************
* digi_pmon_ddeg_state_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves dDEG state machine variables from firmware.
*
* INPUTS:
*  digi_handle   - DIGI device handle.
*  ddeg_hw_block - Hardware block to read
*                  (maximum number of channels):
*                  LINEOTN (12)                   
*                  SYSOTN (1)                     
*                  COREOTN Stage 1 PM (12)        
*                  COREOTN Stage 1 TCM (72)       
*                  COREOTN Stage 2 PM (96)        
*                  COREOTN Stage 2 TCM (256)      
*                  COREOTN Stage N PM (1)         
*                  COREOTN Stage N TCM (6)        
*                  COREOTN Stage 3a / 4 PM (96)   
*                  COREOTN Stage 3a / 4 TCM (256) 
*  chnl          - Channel to read in the specified hardware block. 
*                  The first channel starts from channel 0.
*
* OUTPUTS:
*  digi_pmon_ddeg_state_ptr - pointer to storage for returned state.
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_ddeg_state_get(digi_handle_t *digi_handle,
                                          digi_pmon_ddeg_hw_block_t ddeg_hw_block,
                                          UINT8 chnl,
                                          digi_pmon_ddeg_state_t *digi_pmon_ddeg_state_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];
    UINT32 response[17];

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Build the rest of the message */
    data[0] = ((UINT8)ddeg_hw_block << 16) | (chnl << 8);
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_variable_length_query((pmc_handle_t *)digi_handle,MODID_PMON,HOSTMSG_CMD_PMON_REQ_DDEG_STATE_ST,data,1,response,17,NULL,stat);
    }

    if (PMC_SUCCESS == result)
    {

        /* Return requested data. */
        digi_pmon_ddeg_state_ptr->ddeg_hw_block = (digi_pmon_ddeg_hw_block_t)((response[0] >> 16) & 0xff);
        digi_pmon_ddeg_state_ptr->chnl = (UINT8)((response[0] >> 8) & 0xff);
        digi_pmon_ddeg_state_ptr->ddeg_enh_10ms_ticks           = response[1];
        digi_pmon_ddeg_state_ptr->ddeg_enh_berm_state           = response[2];
        digi_pmon_ddeg_state_ptr->ddeg_enh_nebc                 = response[3];
        digi_pmon_ddeg_state_ptr->ddeg_10ms_ticks               = response[4];
        digi_pmon_ddeg_state_ptr->ddeg_berm_state               = response[5];
        digi_pmon_ddeg_state_ptr->ddeg_nebc                     = response[6];
        digi_pmon_ddeg_state_ptr->ddeg_enh_set_thres            = response[7];
        digi_pmon_ddeg_state_ptr->ddeg_enh_clear_thres          = response[8];
        digi_pmon_ddeg_state_ptr->ddeg_set_thres                = response[9];
        digi_pmon_ddeg_state_ptr->ddeg_clear_thres              = response[10];
        digi_pmon_ddeg_state_ptr->ddeg_enh_mon_intvl_10ms_ticks = response[11];
        digi_pmon_ddeg_state_ptr->ddeg_mon_intvl_10ms_ticks     = response[12];
        digi_pmon_ddeg_state_ptr->ddeg_enh_int_en               = response[13];
        digi_pmon_ddeg_state_ptr->ddeg_enh_enable               = response[14];
        digi_pmon_ddeg_state_ptr->ddeg_int_en                   = response[15];
        digi_pmon_ddeg_state_ptr->ddeg_enable                   = response[16];
    } /* end if */

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_pmon_ddeg_state_get() */


/*******************************************************************************
* digi_pmon_ddeg_enable_status_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves dDEG enable/interrupt enable from firmware.
*
* INPUTS:
*  digi_handle - DIGI device handle.
*  ddeg_mode   - dDEG mode to read enables from
*  ddeg_hw_block - Hardware block to read
*                  (maximum number of channels):
*                  LINEOTN (12)                   
*                  SYSOTN (1)                     
*                  COREOTN Stage 1 PM (12)        
*                  COREOTN Stage 1 TCM (72)       
*                  COREOTN Stage 2 PM (96)        
*                  COREOTN Stage 2 TCM (256)      
*                  COREOTN Stage N PM (1)         
*                  COREOTN Stage N TCM (6)        
*                  COREOTN Stage 3a / 4 PM (96)   
*                  COREOTN Stage 3a / 4 TCM (256) 
*
* OUTPUTS:
*  digi_pmon_ddeg_enable_status_ptr - pointer to storage for returned status.\n
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:      CMD_REQ_PMON_Get_dDEG_Enable_Status
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_ddeg_enable_status_get(digi_handle_t *digi_handle,
                                                  digi_pmon_ddeg_mode_t ddeg_mode,
                                                  digi_pmon_ddeg_hw_block_t ddeg_hw_block,
                                                  digi_pmon_ddeg_enable_status_t *digi_pmon_ddeg_enable_status_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];
    UINT32 response[17];

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Build the rest of the message */
    data[0] = ((UINT8)ddeg_mode << 24) | ((UINT8)ddeg_hw_block << 16);
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_variable_length_query((pmc_handle_t *)digi_handle,MODID_PMON,HOSTMSG_CMD_PMON_REQ_DDEG_ENABLE_ST,data,1,response,17,NULL,stat);
    }

    if (PMC_SUCCESS == result)
    {
        /* Return requested data. */
        digi_pmon_ddeg_enable_status_ptr->ddeg_mode     = (digi_pmon_ddeg_mode_t)((response[0] >> 24) & 0xff);
        digi_pmon_ddeg_enable_status_ptr->ddeg_hw_block = (digi_pmon_ddeg_hw_block_t)((response[0] >> 16) & 0xff);
        /* response 1..8 */
        memcpy(digi_pmon_ddeg_enable_status_ptr->chnl_en, &(response[1]), sizeof(digi_pmon_ddeg_enable_status_ptr->chnl_en));
        /* response 9..16 */
        memcpy(digi_pmon_ddeg_enable_status_ptr->chnl_int_en, &(response[9]), sizeof(digi_pmon_ddeg_enable_status_ptr->chnl_int_en));
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_pmon_ddeg_enable_status_get() */

/*******************************************************************************
*  digi_pmon_ddeg_cfg_hw_ddeg_lclk
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               Sends host message to FW to enable/disable the LCLK
*               generator for hardware dDEG.
*               Firmware will configure the LCLK generator for a one 
*               second interval if hardware dDEG is enabled. 
*               When hardware dDEG is enabled, OTN PMON will be transferred 
*               automatically at one second intervals if the work order for 
*               SYSOTN, LINEOTN or COREOTN is enabled. The LCLK used for 
*               hardware dDEG and OTN PMON is shared, so there is 
*               no option to manually trigger LCLK or configure the 
*               LCLK for a different interval.
*
*               If hardware dDEG is enabled and any of the three OTN PMON work orders are configured,
*               then the PMON collection mode must be auto with period 1000 milliseconds.
*
*               e.g. the following is ok:
*                digi_pmon_ddeg_cfg_hw_ddeg_lclk(digi_handle,TRUE);
*                digi_pmon_collection_mode_cfg(digi_handle,pmon_trig_mode=1,,trig_period=1000,...);
*                digi_pmon_work_order_cfg(digi_handle,work_order={enable_hw_block_msk=
*                                         DIGI_PMON_WORK_ORDER_LINEOTN|
*                                         DIGI_PMON_WORK_ORDER_SYSOTN|
*                                         DIGI_PMON_WORK_ORDER_COREOTN_ERR_BLK_COUNTS});
*
*  INPUTS:
*               digi_handle - System handle.
*               enable - TRUE: Enable LCLK for hardware dDEG
*                        FALSE: Disable LCLK for hardware dDEG
*
*  OUTPUTS:
*               None.
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_PMON_Config_Hardware_dDEG_LCLK
*
******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_ddeg_cfg_hw_ddeg_lclk(digi_handle_t *digi_handle,
                                                 BOOL enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Build the rest of the message to the previously created header.*/
    data[0] = enable << 24;

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PMON,HOSTMSG_CMD_PMON_CFG_HW_DDEG_LCLK_ST,data,1,stat);
    }  

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_pmon_ddeg_cfg_hw_ddeg_lclk */


/*******************************************************************************
*  digi_pmon_fw_fec_cfg()
*  ___________________________________________________________________________
*
*  DESCRIPTION: This method sends host message to FW to configure PMON related
*               items for FEC.
*
*  INPUTS:
*               digi_handle - System handle.
*
*  OUTPUTS:
*               None.
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_PMON_Config_FEC
*               This function should be called any time the FEC configuration
*               for LINEOTN is modified so that FW can keep its status in sync
*               with the hardware.
*
******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_fw_fec_cfg(digi_handle_t *digi_handle)
{  
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1] = {0};

    PMC_ATOMIC_ENTRY(digi_handle);

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PMON,HOSTMSG_CMD_PMON_CFG_FW_FEC_ST,data,1,stat);
    } 

    PMC_ATOMIC_RETURN(digi_handle, result);

} /* End of digi_pmon_fw_fec_cfg */

/*******************************************************************************
*  digi_pmon_fw_fec_manual_update
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This method sends host message to FW to manually trigger
*               an update of FEC PMON counts in hardware
*
*  INPUTS:
*               digi_handle - System handle.
*
*  OUTPUTS:
*               None.
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_PMON_Config_Manual_FEC_Update
*               This function is used to ask FW to update the FEC counts in 
*               the device when FW is not used to transfer PMON to the host.
*               This function should be called after digi_pmon_fw_fec_cfg()
*               has been called to update the FEC configuration in FW.
*               Software is responsible for periodically calling this function
*               to ask FW to update the FEC counts in the device. 
*
******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_fw_fec_manual_update(digi_handle_t *digi_handle)
{  
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Build the rest of the message to the previously created header.*/
    data[0] = 0;

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PMON,HOSTMSG_CMD_PMON_CFG_FW_MANUAL_FEC_UPD_ST,data,1,stat);
    }  

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_pmon_fw_fec_manual_update */

/*******************************************************************************
*  digi_pmon_manual_trigger_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This method sends host message to FW to trigger manual update
*               of PMON statistics if automatic triggering is not used.
*
*  INPUTS:
*               digi_handle - System handle.
*               manual_trig_msk - Manual Trigger Mask. Bitmask of PMON to trigger.
*                                 A '1' in a bit position will trigger an update 
*                                 of the respective PMON.\n
*                                 Bit0 -- OTN PMON. Latches the pN_EBC and pF_EBC 
*                                         counts in LINEOTN, SYSOTN and COREOTN,
*                                         and the Ethernet counts in COREOTN OHP 
*                                         and RCP_RI and RCP_DI. Also latches the 
*                                         pN_TSE counts in COREOTN if RCP is not 
*                                         enabled. This trigger can only be triggered 
*                                         if Enhanced or Bursty dDEG is not enabled.\n
*                                 Bit1 -- COREOTN Defects. This trigger can only be 
*                                         triggered if RCP is not enabled.\n
*                                 Bit2 -- Misc PMON. Latches all other non-OTN 
*                                         PMON counts. If dDEG is enabled, 
*                                         this trigger will instruct firmware 
*                                         to transfer the OTN counts to host 
*                                         memory. This trigger also latches the 
*                                         FEC counts for LINEOTN.This trigger can 
*                                         only be used if pmon_trig_mode = 0 
*                                         (manual trigger) in function
*                                         digi_pmon_collection_mode_cfg().\n
*
*  OUTPUTS:
*               None.
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_PMON_Manual_Trigger.
*
******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_manual_trigger_cfg(digi_handle_t *digi_handle, 
                                              UINT32 manual_trig_msk)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];

    PMC_ATOMIC_ENTRY(digi_handle);

    /* CMD_CFG_PMON_Manual_Trigger has the following format:
     * Word  bits 24..31      bits 0..23
     *   0  manual_trig_msk    reserved
     */
    data[0] = ((UINT8)manual_trig_msk) << 24;

    /* Send message*/
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PMON,HOSTMSG_CMD_PMON_CFG_MANUAL_TRIGGER_ST,data,1,stat);
    }   

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_pmon_manual_trigger_cfg */


/*******************************************************************************
*  digi_pmon_set_rcp_los_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This method sends a host message to FW to replace SDH defects reported to
*               DI_RCP with the loss of sync for 10/40/100GBE line side clients.
*
*  INPUTS:
*               digi_handle - System handle.
*               replace_sdh_di_rcp_enet_los_bit_mask - Bitmask to enable or disable feature. \n
*                                 1 ~ enable enet los replacement for that link. \n
*                                 0 ~ disable enet los replacement for that link. \n
*                                 link 0 -> bit 0, link 11 -> bit 11. Note that only the lower
*                                 12 bits are used (max = 0xFFF).
*                                 Sending this message will overwrite the prior enabled links. Set
*                                 all links you would like replacement to take place on all at once.
*                                 
*                                 
*  OUTPUTS:
*               None.
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_PMON_Set_rcp_los. \n
*               Please note that this is to be called after the DI_RCP is configured and only changes
*               what is returned in the location of the SDH defects. \n
*               sbipe[15:0] will now have the 10GBE Gsup43 7.3 Mapper LOS (first 12 bits only: bit0 -> link 0 LOS, bit 11 -> link 11 LOS) \n
*               lbipe[15:0] will now have the 40/100GBE transcoder/mapper (4 bits only: \n
*                                                                          bit 0 -> 40G slice 0 LOS, bit 1 -> 40G slice 1 LOS, bit 2 -> 40G slice 2 LOS, \n
*                                                                          bit 3 -> 100G LOS) \n
*               lreie[15:0] will now have the 10/40/100GE MAC terminated modes and monitoring of 10GE Sup43 7.1/7.2 BMP mapping ODU2e/ODU1e LOS (all 16 bits: \n
*                                                                       bit0 -> 10GBE link 0, bit 11 -> 10GBE link 11 LOS \n
*                                                                       bit 12 -> 40GBE slice 0 LOS, bit 13 -> 40GBE slice 1 LOS, bit 14 -> 40GBE slice 2 LOS \n
*                                                                       bit 15 -> 100GBE LOS) \n
*
******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_set_rcp_los_cfg(digi_handle_t *digi_handle, 
                                           UINT16 replace_sdh_di_rcp_enet_los_bit_mask)
{  
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];

    PMC_ATOMIC_ENTRY(digi_handle);

    /* CMD_CFG_PMON_Set_rcp_los has the following format:
     * Word  bits 24..31   bits 16..23   bits 0..15
     *   2  reserved    reserved         replace_sdh_di_rcp_enet_los_bit_mask
     */
    data[0] = ((UINT16)replace_sdh_di_rcp_enet_los_bit_mask);
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PMON,HOSTMSG_CMD_PMON_CFG_FW_SET_RCP_LOS_ST,data,1,stat);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_pmon_set_rcp_los_cfg */

/*******************************************************************************
*  digi_pmon_prbs_collection_mode_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This method sends host message to FW to update the PRBS collection
*               mode settings. When DI_RCP is configured, it causes the PRBS monitor
*               to accumulate error counts over 5ms. However, if it is required that
*               pN_TSE be accumulated over a 1s period, this API can be used. 
*
*  INPUTS:
*               digi_handle - System handle.
*               prbs_collection_mode - 1 ~ PRBS monitor to accumulate error counts
*                                      over 5ms\n
*                                      0 ~ PRBS monitor to accumulate pN_TSE counts
*                                      over 1s\n
*
*  OUTPUTS:
*               None.
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_PMON_Set_Prbs_Collection_Mode.
*
******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_prbs_collection_mode_cfg(digi_handle_t *digi_handle, 
                                                    UINT32 prbs_collection_mode)
{  
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];

    PMC_ATOMIC_ENTRY(digi_handle);   

    /* CMD_CFG_PMON_Set_Prbs_Collection_Mode has the following format:
     * Word  bits 8..31                       bits 0..7
     *   2  -----------reserved-----------|--prbs_collection_mode----    
     */
    data[0] = ((UINT8)prbs_collection_mode);
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PMON,HOSTMSG_CMD_PMON_CFG_PRBS_COLLECTION_MODE_ST,data,1,stat);
    }  

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_pmon_prbs_collection_mode_cfg() */

/*******************************************************************************
* digi_pmon_fw_sm_dma_status_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves the firmware PMON status related to the DMA and PMON state
*  machines.
*
* INPUTS:
*  digi_handle - DIGI device handle.
*
* OUTPUTS:
*  digi_pmon_status_ptr - pointer to storage for returned status.\n
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:      CMD_REQ_PMON_Get_Status
*              Note that a valid LCLK must have triggered for pmon_xfer_enable
*              to be updated to match the configuration specified in 
*              digi_pmon_work_order_cfg().
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_fw_sm_dma_status_get(digi_handle_t *digi_handle,
                                                digi_pmon_fw_sm_dma_status_t *digi_pmon_status_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;        
    UINT32 data[1] = {0};
    UINT32 response[36];

    PMC_ATOMIC_ENTRY(digi_handle);

    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_variable_length_query((pmc_handle_t *)digi_handle,MODID_PMON,HOSTMSG_CMD_PMON_REQ_FW_SM_GET_STATUS_ST,data,1,response,36,NULL,stat);
    }

    if (PMC_SUCCESS == result)
    {

        /* Return requested data. */
        digi_pmon_status_ptr->pmon_trig_mode           = (response[0] >> 24) & 0xff;
        digi_pmon_status_ptr->pmon_sync_en             = (response[0] >> 16) & 0xff;
        digi_pmon_status_ptr->trig_period              = response[0] & 0xffff;      
        digi_pmon_status_ptr->pmon_xfer_enable         = response[1];               
        digi_pmon_status_ptr->rcp_enable               = (response[2] >> 24) & 0xff;
        digi_pmon_status_ptr->ddeg_enable              = (response[2] >> 16) & 0xff;
        digi_pmon_status_ptr->hw_ddeg_enable           = (response[2] >> 8) & 0xff;
        digi_pmon_status_ptr->lclk_ddeg_cnt            = response[3];               
        digi_pmon_status_ptr->lclk_ddeg_intr_overrun   = response[4];               
        digi_pmon_status_ptr->lclk_rcp_cnt             = response[5];               
        digi_pmon_status_ptr->lclk_dma_xfer_cnt        = response[6];               
        digi_pmon_status_ptr->lineotn_fec_intr_cnt     = response[7];               
        digi_pmon_status_ptr->lineotn_intr_overrun     = response[8];              
        digi_pmon_status_ptr->lclk_otn_intr_overrun    = response[9];              
        digi_pmon_status_ptr->lclk_misc_intr_overrun   = response[10];              
        digi_pmon_status_ptr->lclk_otn_buf_conflict    = response[11];              
        digi_pmon_status_ptr->lclk_misc_buf_conflict   = response[12];              
        digi_pmon_status_ptr->gdma_busy_num            = response[13];              
        digi_pmon_status_ptr->rcp_dma_err_num          = response[14];              
        digi_pmon_status_ptr->lineotn_fec_dma_complete = response[15];
        digi_pmon_status_ptr->otn_dma_complete         = response[16];
        digi_pmon_status_ptr->misc_dma_complete        = response[17];
        digi_pmon_status_ptr->rcp_dma_complete         = response[18];
        digi_pmon_status_ptr->pmon_fec_update_delay    = response[19];
        digi_pmon_status_ptr->ddeg_bursty_cnt          = response[20];
        digi_pmon_status_ptr->ddeg_enh_cnt             = response[21];
        digi_pmon_status_ptr->ddeg_blk_cnt             = response[22];
        memcpy( digi_pmon_status_ptr->nebc_stats,            &(response[23]), sizeof(digi_pmon_status_ptr->nebc_stats)            );
        memcpy( digi_pmon_status_ptr->cumulative_nebc_stats, &(response[28]), sizeof(digi_pmon_status_ptr->cumulative_nebc_stats) );
        digi_pmon_status_ptr->v_bit_set_count          = response[33];
        digi_pmon_status_ptr->v_bit_clear_count        = response[34];
        digi_pmon_status_ptr->cookie                   = response[35];
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_pmon_fw_sm_dma_status_get() */


/*******************************************************************************
* FUNCTION: digi_pmon_shared_mem_size_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves size of shared memory buffer needed for the PMON feature.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*
* OUTPUTS:
*   size_ptr        - Size of shared memory buffer
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_shared_mem_size_get(digi_handle_t    *digi_handle,
                                               UINT32           *size_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_pmon_shared_mem_size_internal_get(digi_handle,size_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_pmon_shared_mem_size_get */

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* FUNCTION: digi_pmon_shared_mem_size_internal_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves size of shared memory buffer needed for the PMON feature.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*
* OUTPUTS:
*   size_ptr        - Size of shared memory buffer
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_pmon_shared_mem_size_internal_get(digi_handle_t    *digi_handle,
                                                         UINT32           *size_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    *size_ptr = sizeof(digi_pmon_t);

    PMC_RETURN(result);
} /* digi_pmon_shared_mem_size_internal_get */


/*******************************************************************************
* FUNCTION: digi_pmon_shared_mem_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves pointer to shared memory where FW collected PMON table resides
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*
* OUTPUTS:
*   pmon_pptr        - Pointer to PMON shared memory
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_shared_mem_get(digi_handle_t    *digi_handle,
                                          digi_pmon_t     **pmon_pptr)
{
    PMC_ERROR            result = PMC_SUCCESS;
    PMC_ENTRY();
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    if (NULL == pmon_pptr)
    {
        result = DIGI_ERR_NULL_HANDLE; 
    } 

    if (PMC_SUCCESS == result)
    {
        *pmon_pptr = (digi_pmon_t *) digi_handle->var.digi_pmon_mode_profile->data_store_vir_start_addr;

        if (NULL == *pmon_pptr) 
        {
            result = DIGI_ERR_PMON_SHARED_MEM_NOT_INIT;
        }
    }

    PMC_RETURN(result);
} /* digi_pmon_shared_mem_get */

/*******************************************************************************
* FUNCTION: digi_pmon_param_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Checks digi PMON input parameters  
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   pmon_table_ptr  - Pointer to PMON table
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_param_check(digi_handle_t    *digi_handle,
                                       void             *pmon_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    if (NULL == pmon_table_ptr)
    {
        PMC_RETURN(DIGI_ERR_NULL_HANDLE); 
    }

    PMC_RETURN(result);
} /* digi_pmon_param_check */



/*******************************************************************************
* FUNCTION: digi_pmon_chnl_param_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Checks digi PMON input parameters  
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   chnl_handle     - Pointer to channel handle
*   pmon_table_ptr  - Pointer to PMON table
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_chnl_param_check(digi_handle_t    *digi_handle,
                                            void             *chnl_handle,
                                            void             *pmon_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    if (NULL == chnl_handle)
    {
        PMC_RETURN(DIGI_ERR_NULL_HANDLE); 
    }
    
    if (NULL == pmon_table_ptr)
    {
        PMC_RETURN(DIGI_ERR_NULL_HANDLE); 
    }

    PMC_RETURN(result);
} /* digi_pmon_chnl_param_check */

#endif /* DOXYGEN_PUBLIC_ONLY */

/*******************************************************************************
* FUNCTION: digi_pmon_int_register_cb()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Registers a callback for aggregation interrupt table processing.
*
*   This callback will be called when an active interrupt is found in any
*   PMON interrupt
*
*   The callback will be called a minimum of zero times if no active interrupts
*   are found, or a maximum of one time if there is an interrupt in at least
*   one interrupt
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   token           - user token to be stored and passed back to callback\n
*                     Optional, can be NULL.
*   token_size      - token size in bytes\n
*                     Optional, can be 0.
*   user_cb         - callback to be registered against this interrupt type
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PMC_ERROR digi_pmon_int_register_cb(digi_handle_t      *digi_handle,
                                    void               *token,
                                    UINT32              token_size,
                                    digi_int_cb_fcn     user_cb)
{
    PMC_ERROR            result = PMC_SUCCESS;
    digi_int_type_t      int_type = DIGI_INT_TYPE_PMON;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);


    if (PMC_SUCCESS == result)
    {
        result = digi_int_callback_set(digi_handle,
                                       int_type,
                                       token,
                                       token_size,
                                       user_cb);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_pmon_int_register_cb */


/*******************************************************************************
* FUNCTION: digi_pmon_int_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the PMON
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_table_ptr   - pointer to pmon interrupt table with bits set to
*                     enable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PMC_ERROR digi_pmon_int_enable(digi_handle_t            *digi_handle,
                               digi_pmon_int_t          *int_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    digi_int_type_t     int_type = DIGI_INT_TYPE_PMON;
    hostmsg_handle_t   *hostmsg_handle = NULL;
    pmc_handle_t       *cpu_handle;

    digi_pmon_int_t    *int_en_table_ptr;
    BOOL                enable = TRUE;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    result = digi_int_param_check(digi_handle, NULL, int_type, int_table_ptr); 
    
    hostmsg_handle = (hostmsg_handle_t *)(digi_handle->base.common_info->hostmsg_handle);
    PMC_ASSERT((NULL != hostmsg_handle),HOSTMSG_ERR_INVALID_PARAMETERS,0,0);
    cpu_handle = (pmc_handle_t *) hostmsg_handle;

    /* Callback for interrupt table must be registered before enable ints */
    if (PMC_SUCCESS == result)
    {
        if(!digi_int_is_cb_registered(digi_handle, int_type))
        {
            result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
        }
    }


    /* Get internal enabled interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }
    
    
    #ifdef PMON_INT 
    #undef PMON_INT 
    #endif
    #define PMON_INT    UTIL_GLOBAL_INT_PMON_ENABLE

    if (PMC_SUCCESS == result)
    {
        PMON_INT_TABLE_DEFINE();
    }
        

    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_pmon_int_enable */




/*******************************************************************************
* FUNCTION: digi_pmon_int_disable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable interrupts based on bits that are set in the PMON
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_table_ptr   - pointer to pmon interrupt table with bits set to
*                     disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PMC_ERROR digi_pmon_int_disable(digi_handle_t            *digi_handle,
                                digi_pmon_int_t          *int_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    digi_int_type_t     int_type = DIGI_INT_TYPE_PMON;
    hostmsg_handle_t   *hostmsg_handle = NULL;
    pmc_handle_t       *cpu_handle;

    digi_pmon_int_t    *int_en_table_ptr;
    BOOL                enable = FALSE;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);
    result = digi_int_param_check(digi_handle, NULL, int_type, int_table_ptr); 

    hostmsg_handle = (hostmsg_handle_t *)(digi_handle->base.common_info->hostmsg_handle);
    PMC_ASSERT((NULL != hostmsg_handle),HOSTMSG_ERR_INVALID_PARAMETERS,0,0);
    cpu_handle = (pmc_handle_t *) hostmsg_handle;


    /* Get internal enabled interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }
    
        
    
    #ifdef PMON_INT 
    #undef PMON_INT 
    #endif
    #define PMON_INT    UTIL_GLOBAL_INT_PMON_ENABLE

    
    if (PMC_SUCCESS == result)
    {
        PMON_INT_TABLE_DEFINE();
    }

    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_pmon_int_disable */

/*******************************************************************************
* FUNCTION: digi_pmon_int_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the PMON
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_table_ptr   - pointer to pmon interrupt table with bits set to
*                     clear interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PMC_ERROR digi_pmon_int_clear(digi_handle_t            *digi_handle,
                              digi_pmon_int_t          *int_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    digi_int_type_t     int_type = DIGI_INT_TYPE_PMON;
    hostmsg_handle_t   *hostmsg_handle = NULL;
    pmc_handle_t       *cpu_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_int_param_check(digi_handle, NULL, int_type, int_table_ptr); 


    hostmsg_handle = (hostmsg_handle_t *)(digi_handle->base.common_info->hostmsg_handle);
    PMC_ASSERT((NULL != hostmsg_handle),HOSTMSG_ERR_INVALID_PARAMETERS,0,0);
    cpu_handle = (pmc_handle_t *) hostmsg_handle;


    /* Callback for interrupt table must be registered before enable ints */
    if (PMC_SUCCESS == result)
    {
        if(!digi_int_is_cb_registered(digi_handle, int_type))
        {
            result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
        }
    }


    #ifdef PMON_INT 
    #undef PMON_INT 
    #endif
    #define PMON_INT    UTIL_GLOBAL_INT_PMON_CLEAR

    if (PMC_SUCCESS == result)
    {
        PMON_INT_TABLE_DEFINE();
    }
        
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_pmon_int_clear */



/*******************************************************************************
* FUNCTION: digi_pmon_int_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves interrupts based on enabled PMON interrupt bits.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* OUTPUTS:
*   int_table_ptr   - pointer to pmon interrupt table to store retrieved interrupts
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PMC_ERROR digi_pmon_int_retrieve(digi_handle_t            *digi_handle,
                                 digi_pmon_int_t          *int_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    digi_int_type_t     int_type = DIGI_INT_TYPE_PMON;
    hostmsg_handle_t   *hostmsg_handle = NULL;
    pmc_handle_t       *cpu_handle;

    digi_pmon_int_t    *int_en_table_ptr;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_int_param_check(digi_handle, NULL, int_type, int_table_ptr); 
   

    hostmsg_handle = (hostmsg_handle_t *)(digi_handle->base.common_info->hostmsg_handle);
    PMC_ASSERT((NULL != hostmsg_handle),HOSTMSG_ERR_INVALID_PARAMETERS,0,0);
    cpu_handle = (pmc_handle_t *) hostmsg_handle;


    PMC_MEMSET(int_table_ptr, 0, sizeof(digi_pmon_int_t));

    /* Get internal enabled interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }
   

    #ifdef PMON_INT 
    #undef PMON_INT 
    #endif
    #define PMON_INT    UTIL_GLOBAL_INT_PMON_RETRIEVE

    if (PMC_SUCCESS == result)
    {
        PMON_INT_TABLE_DEFINE();
    }

#ifdef PMC_SW_SIMULATION
    /* Return fired interrupts for simulation */
    int_table_ptr->misc_i = TRUE;
    int_table_ptr->otn_i  = TRUE;
#endif
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_pmon_int_retrieve */


/*******************************************************************************
* FUNCTION: digi_pmon_int_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in the PMON interrupt table.
*
*   The callback associated with the aggregation interrupt type will be called
*   if an active interrupt is found. The callback will be
*   called a minimum of zero times if no active interrupts are found, or
*   a maximum of one time if there is an interrupt found.
*
*   A callback must be registered for the interrupt type before this API
*   can be called.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_int_enabled_check(digi_handle_t            *digi_handle)
{
    PMC_ERROR           result = PMC_SUCCESS;
    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_pmon_int_enabled_check_internal(digi_handle);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_pmon_int_enabled_check */


/*******************************************************************************
* digi_dcpb_pmon_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Enables or disables a PMON counter on a slave port of the DCPB depending on
*  the pmon_mode. If disable is set, then the inputs count_drop_packets,
*  count_err_packets, count_oversize_packets, and count_good_packets are ignored.
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance.
*   port                    - ENUM defining the DCPB port 
*   counter_num             - The counter number. There are 4 counters per port.
*   pmon_mode               - ENUM defining the PMON mode (byte, packet, or disable)
*   count_drop_packets     - Disable count packets marked as drop in this counter\n
*                            1: disable drop packet count\n
*                            0: enable drop packet count
*   count_err_packets      - Disable count packets marked as error in this counter\n
*                            1: disable drop packet count\n
*                            0: enable drop packet count
*   count_oversize_packets - Disable count oversized packets in this counter\n
*                            1: disable drop packet count\n
*                            0: enable drop packet count
*   count_good_packets     - Disable count normal packets in this counter\n
*                            1: disable drop packet count\n
*                            0: enable drop packet count
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
PUBLIC PMC_ERROR digi_dcpb_pmon_cfg(digi_handle_t *digi_handle,
                                    cpb_dcpb_dpi_port_type_t port,
                                    UINT32 counter_num,
                                    cpb_pmon_mode pmon_mode,
                                    BOOL count_drop_packets,
                                    BOOL count_err_packets,
                                    BOOL count_oversize_packets,
                                    BOOL count_good_packets)
{
    PMC_ERROR result;
    digi_cpb_pmon_counter_t * pmon_counter;
    PMC_ATOMIC_ENTRY(digi_handle);
    
    if(port >= LAST_CPB_DCPB_INGRESS_PORT)
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);


    /* Remember the pmon config in case the cpb goes into low power mode
     * so we can set it again */
    pmon_counter = &(digi_handle->var.cpb_port_pmon[(UINT32)port].pmon_counter[counter_num]);
    pmon_counter->pmon_mode = pmon_mode;
    pmon_counter->count_drop_packets = count_drop_packets; 
    pmon_counter->count_err_packets = count_err_packets;
    pmon_counter->count_oversize_packets = count_oversize_packets;
    pmon_counter->count_good_packets = count_good_packets;

    result = cpb_dcpb_pmon_cfg(digi_handle->dcpb_handle, 
                               (cpb_dcpb_dpi_port_type_t)port, 
                               counter_num,
                               pmon_mode,
                               count_drop_packets,
                               count_err_packets,
                               count_oversize_packets,
                               count_good_packets);
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_dcpb_pmon_cfg */

/*******************************************************************************
* digi_ocpb_pmon_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Enables or disables a PMON counter on a slave port of the OCPB depending on
*  the pmon_mode. If disable is set, then the inputs count_drop_packets,
*  count_err_packets, count_oversize_packets, and count_good_packets are ignored.
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance.
*   port                    - ENUM defining the OCPB port 
*   counter_num             - The counter number. There are 4 counters per port.
*   pmon_mode               - ENUM defining the PMON mode (byte, packet, or disable)
*   count_drop_packets     - Disable count packets marked as drop in this counter\n
*                            1: disable drop packet count\n
*                            0: enable drop packet count
*   count_err_packets      - Disable count packets marked as error in this counter\n
*                            1: disable drop packet count\n
*                            0: enable drop packet count
*   count_oversize_packets - Disable count oversized packets in this counter\n
*                            1: disable drop packet count\n
*                            0: enable drop packet count
*   count_good_packets     - Disable count normal packets in this counter\n
*                            1: disable drop packet count\n
*                            0: enable drop packet count
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
PUBLIC PMC_ERROR digi_ocpb_pmon_cfg(digi_handle_t *digi_handle,
                                    cpb_ocpb_dpi_port_type_t port,
                                    UINT32 counter_num,
                                    cpb_pmon_mode pmon_mode,
                                    BOOL count_drop_packets,
                                    BOOL count_err_packets,
                                    BOOL count_oversize_packets,
                                    BOOL count_good_packets)
{
    PMC_ERROR result;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    result  = digi_ocpb_pmon_internal_cfg(digi_handle,
                                          port,
                                          counter_num,
                                          pmon_mode,
                                          count_drop_packets,
                                          count_err_packets,
                                          count_oversize_packets,
                                          count_good_packets);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_ocpb_pmon_cfg */

/*******************************************************************************
* digi_cpb_pmon_chnl_add_drop_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Adds or drops a channel for a PMON counter. There are four counters per port.
*  Channels can be added and droped from any counter. A counter can count packets
*  or bytes for more than one channel.
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   *input_pc_ptr       - Pointer to the input channel handle on which to operate.
*   counter_num         - The counter number. There are four counters per port.
*   is_dcpb             - TRUE: add PMON on datapath CPB
*                         FALSE: add PMON on ODUk switch
*   add_channel         - TRUE: Add this channel
*                         FALSE: Drop this channel
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
PUBLIC PMC_ERROR digi_cpb_pmon_chnl_add_drop_cfg(digi_handle_t *digi_handle,
                                                 util_global_switch_data_t * input_pc_ptr,
                                                 UINT32 counter_num,
                                                 BOOL is_dcpb,
                                                 BOOL add_channel)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check handle */
    DIGI_SWITCH_DATA_HANDLE_CHECK(digi_handle, input_pc_ptr, result);

    if (PMC_SUCCESS == result)
    {
        result = digi_cpb_pmon_chnl_add_drop_internal_cfg(digi_handle,input_pc_ptr,counter_num,is_dcpb,add_channel);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cpb_pmon_chnl_add_drop_cfg */

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* FUNCTION: digi_pmon_int_enabled_check_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in the PMON interrupt table.
*
*   The callback associated with the aggregation interrupt type will be called
*   if an active interrupt is found. The callback will be
*   called a minimum of zero times if no active interrupts are found, or
*   a maximum of one time if there is an interrupt found.
*
*   A callback must be registered for the interrupt type before this API
*   can be called.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_int_enabled_check_internal(digi_handle_t            *digi_handle)
{
    PMC_ERROR           result = PMC_SUCCESS;
    digi_int_type_t     int_type = DIGI_INT_TYPE_PMON;
    hostmsg_handle_t   *hostmsg_handle = NULL;
    pmc_handle_t       *cpu_handle;

    digi_pmon_int_t    *int_en_table_ptr;
    BOOL                int_found = FALSE;
  
    /* Track if each PMON interrupt has fired */
    #ifdef PMON_INT 
    #undef PMON_INT 
    #endif
    #define PMON_INT(pmon_type, struct_field) \
        BOOL    struct_field##_int_found = FALSE;
    
    PMON_INT_TABLE_DEFINE();



    
    PMC_ENTRY();
    
    PMC_ASSERT((NULL != digi_handle), DIGI_ERR_NULL_HANDLE, 0, 0);


    hostmsg_handle = (hostmsg_handle_t *)(digi_handle->base.common_info->hostmsg_handle);
    PMC_ASSERT((NULL != hostmsg_handle),HOSTMSG_ERR_INVALID_PARAMETERS,0,0);
    cpu_handle = (pmc_handle_t *) hostmsg_handle;


    /* Callback for interrupt table must be registered before checking for active ints */
    if (PMC_SUCCESS == result)
    {
        if(!digi_int_is_cb_registered(digi_handle, int_type))
        {
            result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
        }
    }


    /* Get internal enabled interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }


    if (PMC_SUCCESS == result)
    {
        #ifdef PMON_INT 
        #undef PMON_INT 
        #endif
        #define PMON_INT    UTIL_GLOBAL_INT_PMON_CHECK
        
        PMON_INT_TABLE_DEFINE();
       

#ifdef PMC_SW_SIMULATION
        /* Force found interrupt for simulation so that callback runs */
        int_found = TRUE;
#endif



        if (TRUE == int_found)
        {
            result = digi_int_callback_run(digi_handle, DIGI_INT_ALL_CHNLS, int_type);

            /* Clear Shared Memory Region */
            digi_pmon_shared_mem_clear(digi_handle);
        }
    }
   


    PMC_RETURN(result);
} /* digi_pmon_int_enabled_check_internal */

/*******************************************************************************
* digi_ocpb_pmon_internal_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Enables or disables a PMON counter on a slave port of the OCPB depending on
*  the pmon_mode. If disable is set, then the inputs count_drop_packets,
*  count_err_packets, count_oversize_packets, and count_good_packets are ignored.
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance.
*   port                    - ENUM defining the OCPB port 
*   counter_num             - The counter number. There are 4 counters per port.
*   pmon_mode               - ENUM defining the PMON mode (byte, packet, or disable)
*   count_drop_packets      - Count packets marked as drop in this counter
*   count_err_packets       - Count packets marked as error in this counter
*   count_oversize_packets  - Count oversized packets in this counter
*   count_good_packets      - Count normal packets in this counter
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
PUBLIC PMC_ERROR digi_ocpb_pmon_internal_cfg(digi_handle_t *digi_handle,
                                             cpb_ocpb_dpi_port_type_t port,
                                             UINT32 counter_num,
                                             cpb_pmon_mode pmon_mode,
                                             BOOL count_drop_packets,
                                             BOOL count_err_packets,
                                             BOOL count_oversize_packets,
                                             BOOL count_good_packets)
{
    cpb_handle_t * oduksw_handle;
    digi_cpb_pmon_counter_t * pmon_counter;
    PMC_ERROR result;
    
    PMC_ENTRY();
    
    if(port >= LAST_CPB_OCPB_INGRESS_PORT)
        PMC_RETURN(DIGI_ERR_INVALID_ARG);

    /* Remember the pmon config in case the cpb goes into low power mode
     * so we can set it again */
    pmon_counter = &(digi_handle->var.oduksw_port_pmon[(UINT32)port].pmon_counter[counter_num]);
    pmon_counter->pmon_mode = pmon_mode;
    pmon_counter->count_drop_packets = count_drop_packets; 
    pmon_counter->count_err_packets = count_err_packets;
    pmon_counter->count_oversize_packets = count_oversize_packets;
    pmon_counter->count_good_packets = count_good_packets;


    oduksw_handle = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);
    result = cpb_ocpb_pmon_cfg(oduksw_handle, 
                               (cpb_ocpb_dpi_port_type_t)port, 
                               counter_num,
                               pmon_mode,
                               count_drop_packets,
                               count_err_packets,
                               count_oversize_packets,
                               count_good_packets);
    PMC_RETURN(result);
} /* digi_ocpb_pmon_internal_cfg */

/*******************************************************************************
* digi_cpb_pmon_chnl_add_drop_internal_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Adds or drops a channel for a PMON counter. There are four counters per port.
*  Channels can be added and droped from any counter. A counter can count packets
*  or bytes for more than one channel.
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   *input_pc_ptr       - Pointer to the input channel handle on which to operate.
*   counter_num         - The counter number. There are four counters per port.
*   is_dcpb             - TRUE: add PMON on datapath CPB
*                         FALSE: add PMON on ODUk switch
*   add_channel         - TRUE: Add this channel
*                         FALSE: Drop this channel
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
PUBLIC PMC_ERROR digi_cpb_pmon_chnl_add_drop_internal_cfg(digi_handle_t *digi_handle,
                                                          util_global_switch_data_t * input_pc_ptr,
                                                          UINT32 counter_num,
                                                          BOOL is_dcpb,
                                                          BOOL add_channel)
{
    PMC_ENTRY();
    cpb_handle_t * oduksw_handle;

    util_global_switch_data_def_t *input_pc_data_ptr = NULL;

    PMC_ASSERT(NULL != input_pc_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    input_pc_data_ptr = (util_global_switch_data_def_t *)input_pc_ptr;
    DIGI_CHNL_HANDLE_MAGIC_CHECK(input_pc_data_ptr);
    
    
    
    if (TRUE != util_global_rx_or_tx_chnl_prov_status_get(input_pc_data_ptr->header.prov_state)) 
    {
            PMC_RETURN(DIGI_ERR_CPB_CHNL_NOT_PROV);
    }

    if(is_dcpb)
    {
        cpb_pmon_chnl_add_drop_cfg(digi_handle->dcpb_handle, 
                                   (UINT32)input_pc_data_ptr->cpb_data.port_type, /* the port number */ 
                                   counter_num,
                                   input_pc_data_ptr->cpb_data.channel,
                                   add_channel);
    } else    
    {
        oduksw_handle = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);
        cpb_pmon_chnl_add_drop_cfg(oduksw_handle, 
                                   (UINT32)input_pc_data_ptr->cpb_data.port_type, 
                                   counter_num,
                                   input_pc_data_ptr->cpb_data.channel,
                                   add_channel);
    }
    PMC_RETURN(PMC_SUCCESS);
} /* digi_cpb_pmon_chnl_add_drop_internal_cfg */
#endif /* DOXYGEN_PUBLIC_ONLY */

/*******************************************************************************
* digi_dcpb_pmon_latch_trig_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Transfer the current PMON count to a holding register
*   
* INPUTS:
*   *digi_handle            - pointer to digi handle instance.
*   port                    - ENUM defining DCPB ports
*   counter_mask            - A counter mask where bits 0 to 3 represent counters
*                             0 to 3. Writing a 1 to a bit indicates that the counter
*                             should transfer the count and reset the count. Values
*                             greater than 0xF are considered invalid.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   CPB_ERR_INVALID_ARG
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_dcpb_pmon_latch_trig_set(digi_handle_t *digi_handle, 
                                               cpb_dcpb_dpi_port_type_t port,
                                               UINT32 counter_mask)
{
    
    PMC_ERROR result;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    result = cpb_pmon_latch_trig_set(digi_handle->dcpb_handle, 
                                     (UINT32) port, 
                                     counter_mask);

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_dcpb_pmon_latch_trig_set */

/*******************************************************************************
* digi_ocpb_pmon_latch_trig_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Transfer the current PMON count to a holding register
*   
* INPUTS:
*   *digi_handle            - pointer to digi handle instance.
*   port                    - ENUM defining OCPB ports   
*   counter_mask            - A counter mask where bits 0 to 3 represent counters
*                             0 to 3. Writing a 1 to a bit indicates that the counter
*                             should transfer the count and reset the count. Values
*                             greater than 0xF are considered invalid.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   CPB_ERR_INVALID_ARG
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ocpb_pmon_latch_trig_set(digi_handle_t *digi_handle, 
                                               cpb_ocpb_dpi_port_type_t port,
                                               UINT32 counter_mask)
{
    
    PMC_ERROR result;
    cpb_handle_t * oduksw_handle;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    oduksw_handle = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);
    result = cpb_pmon_latch_trig_set(oduksw_handle, 
                                     (UINT32) port, 
                                     counter_mask);

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_ocpb_pmon_latch_trig_set */

/*******************************************************************************
* digi_dcpb_pmon_counter_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read the count from the holding register.
*   
* INPUTS:
*   *digi_handle            - pointer to CPB handle instance.
*   port                    - ENUM defining DCPB ports  
*   counter_mask           - A counter mask where bits 0 to 3 represent counters
*                            0 to 3. Writing a 1 to a bit indicates a get request
*                            on that counter. Values greater than 0xF are considered 
*                            invalid.
*
* OUTPUTS:
*   count0                  - The PMON count on counter 0.
*                             If the corresponding mask bit was not set it returns a 0.
*   count1                  - The PMON count on counter 1
*                             If the corresponding mask bit was not set it returns a 0.
*   count2                  - The PMON count on counter 2
*                             If the corresponding mask bit was not set it returns a 0.
*   count3                  - The PMON count on counter 3
*                             If the corresponding mask bit was not set it returns a 0.
* RETURNS:
*   CPB_ERR_INVALID_ARG
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_dcpb_pmon_counter_get(digi_handle_t *digi_handle, 
                                            cpb_dcpb_dpi_port_type_t port, 
                                            UINT32 counter_mask,
                                            UINT32 * count0,
                                            UINT32 * count1,
                                            UINT32 * count2,
                                            UINT32 * count3)
{
    PMC_ERROR result;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    result = cpb_pmon_counter_get(digi_handle->dcpb_handle, 
                                 (UINT32) port, 
                                 counter_mask,
                                 count0,
                                 count1,
                                 count2,
                                 count3);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_dcpb_pmon_counter_get*/

/*******************************************************************************
* digi_ocpb_pmon_counter_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read the count from the holding register.
*   
* INPUTS:
*   *digi_handle            - pointer to CPB handle instance.
*   port                    - ENUM defining OCPB ports 
*   counter_mask           - A counter mask where bits 0 to 3 represent counters
*                            0 to 3. Writing a 1 to a bit indicates a get request
*                            on that counter. Values greater than 0xF are considered 
*                            invalid.
*
* OUTPUTS:
*   count0                  - The PMON count on counter 0.
*                             If the corresponding mask bit was not set it returns a 0.
*   count1                  - The PMON count on counter 1
*                             If the corresponding mask bit was not set it returns a 0.
*   count2                  - The PMON count on counter 2
*                             If the corresponding mask bit was not set it returns a 0.
*   count3                  - The PMON count on counter 3
*                             If the corresponding mask bit was not set it returns a 0.
* RETURNS:
*   CPB_ERR_INVALID_ARG
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ocpb_pmon_counter_get(digi_handle_t *digi_handle, 
                                            cpb_ocpb_dpi_port_type_t port, 
                                            UINT32 counter_mask,
                                            UINT32 * count0,
                                            UINT32 * count1,
                                            UINT32 * count2,
                                            UINT32 * count3)
{
    PMC_ERROR result;
    cpb_handle_t * oduksw_handle;   

    PMC_ATOMIC_ENTRY(digi_handle);

    oduksw_handle = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);
    result = cpb_pmon_counter_get(oduksw_handle, 
                                  (UINT32) port, 
                                  counter_mask,
                                  count0,
                                  count1,
                                  count2,
                                  count3);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_ocpb_pmon_counter_get*/



#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* FUNCTION: digi_pmon_ipi_int_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupt corresponding to IPI bit.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   ipi_type        - Type of IPI bit
*   enable          - True to enable the interrupt associated with the IPI bit
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_ipi_int_enable(digi_handle_t         *digi_handle,
                                          digi_pmon_ipi_type_t   ipi_type,
                                          BOOL                   enable)
{
    PMC_ERROR           result = PMC_SUCCESS;
    hostmsg_handle_t   *hostmsg_handle = NULL;
    pmc_handle_t       *cpu_handle;
    UINT32              ipi_bit;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(ipi_type < LAST_DIGI_PMON_IPI_TYPE, DIGI_ERR_INVALID_ARG, 0, 0);
    
    hostmsg_handle = (hostmsg_handle_t *)(digi_handle->base.common_info->hostmsg_handle);
    PMC_ASSERT((NULL != hostmsg_handle),HOSTMSG_ERR_INVALID_PARAMETERS,0,0);
    cpu_handle = (pmc_handle_t *) hostmsg_handle;


    ipi_bit = ipi_bit_tbl[ipi_type].reg_offset;

    ipi_bit_tbl[ipi_type].ipi_int_en(LOG_CALLER
                                     NULL,
                                     cpu_handle,
                                     ipi_bit,
                                     ipi_bit,
                                     enable);

    PMC_RETURN(result);
} /* digi_pmon_ipi_int_enable */


/*******************************************************************************
* FUNCTION: digi_pmon_ipi_int_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears the interrupt corresponding to the IPI bit.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   ipi_type        - Type of IPI bit to clear interrupt for
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_ipi_int_clear(digi_handle_t          *digi_handle,
                                         digi_pmon_ipi_type_t    ipi_type)
{
    PMC_ERROR           result = PMC_SUCCESS;
    UINT32              reg_value = 0;
    hostmsg_handle_t   *hostmsg_handle = NULL;
    pmc_handle_t       *cpu_handle;

    PMC_ENTRY();
    
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(ipi_type < LAST_DIGI_PMON_IPI_TYPE, DIGI_ERR_INVALID_ARG, 0, 0);

    hostmsg_handle = (hostmsg_handle_t *)(digi_handle->base.common_info->hostmsg_handle);
    PMC_ASSERT((NULL != hostmsg_handle),HOSTMSG_ERR_INVALID_PARAMETERS,0,0);
    cpu_handle = (pmc_handle_t *) hostmsg_handle;
   

    reg_value |= (0x1 << ipi_bit_tbl[ipi_type].reg_offset);

    ipi_bit_tbl[ipi_type].ipi_int_clear(LOG_CALLER
                                        NULL,
                                        cpu_handle,
                                        reg_value);


    PMC_RETURN(result);
} /* digi_pmon_ipi_int_clear */


/*******************************************************************************
* FUNCTION: digi_pmon_ipi_int_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the status of the interrupt corresponding to the IPI bit.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   ipi_type        - Type of IPI bit to retrieve interrupt for
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL digi_pmon_ipi_int_retrieve(digi_handle_t          *digi_handle,
                                       digi_pmon_ipi_type_t    ipi_type)
{
    UINT32              reg_value;
    UINT32              mask = 0;
    UINT32              offset = 0;
    hostmsg_handle_t   *hostmsg_handle = NULL;
    pmc_handle_t       *cpu_handle;
    UINT32              ipi_bit_value;

    PMC_ENTRY();
    
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(ipi_type < LAST_DIGI_PMON_IPI_TYPE, DIGI_ERR_INVALID_ARG, 0, 0);
    
    hostmsg_handle = (hostmsg_handle_t *)(digi_handle->base.common_info->hostmsg_handle);
    PMC_ASSERT((NULL != hostmsg_handle),HOSTMSG_ERR_INVALID_PARAMETERS,0,0);
    cpu_handle = (pmc_handle_t *) hostmsg_handle;

    reg_value = ipi_bit_tbl[ipi_type].ipi_int_get(LOG_CALLER
                                                  NULL,
                                                  cpu_handle);

    offset    = ipi_bit_tbl[ipi_type].reg_offset;
    mask      = (0x1 << offset);
    reg_value = (reg_value & mask) >> offset;

    /* If interrupt detected, check that IPI bit is set to FW_DONE */
    if (TRUE == reg_value)
    {
        ipi_bit_value = digi_pmon_ipi_bit_get(digi_handle, ipi_type);
       
        if (PMC_SUCCESS != digi_pmon_ipi_bit_check(digi_handle, ipi_bit_value))
        {
            /* IPI bit is still SW_DONE (FW is not done processing), 
             * manually set the interrupt bit low */
            reg_value = FALSE;
        }
    }
    
    PMC_RETURN(reg_value);
} /* digi_pmon_ipi_int_retrieve */


/*******************************************************************************
* FUNCTION: digi_pmon_ipi_bit_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets the status of the IPI bit.
* 
*   For PMON IPI interrupts, we need to check to make sure that the IPI bit
*   is set to FW_DONE. When SW is done operating on the PMON shared memory,
*   then SW will set the IPI bit back to SW_DONE to signal FW.
*
*   For DDEG IPI interrupts, SW does not need to check the IPI bit so this
*   function will always return FW_DONE.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   ipi_type        - Type of IPI bit to retrieve
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 digi_pmon_ipi_bit_get(digi_handle_t          *digi_handle,
                                    digi_pmon_ipi_type_t    ipi_type)
{
    UINT32              reg_value = 0;
    UINT32              ipi_bit_num;
    hostmsg_handle_t   *hostmsg_handle = NULL;
    pmc_handle_t       *cpu_handle;

    PMC_ENTRY();
    
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(ipi_type < LAST_DIGI_PMON_IPI_TYPE, DIGI_ERR_INVALID_ARG, 0, 0);
    
    hostmsg_handle = (hostmsg_handle_t *)(digi_handle->base.common_info->hostmsg_handle);
    PMC_ASSERT((NULL != hostmsg_handle),HOSTMSG_ERR_INVALID_PARAMETERS,0,0);
    cpu_handle = (pmc_handle_t *) hostmsg_handle;
    

    switch (ipi_type)
    {
      case DIGI_PMON_IPI_TYPE_PMON_OTN:
      case DIGI_PMON_IPI_TYPE_PMON_MISC:
        ipi_bit_num = ipi_bit_tbl[ipi_type].ipi_bit_num;
        reg_value = int_agr_reg_INTER_PROCESSOR_INTS_array_read(NULL, cpu_handle, ipi_bit_num);
        break;

      case DIGI_PMON_IPI_TYPE_DDEG_ENH:
      case DIGI_PMON_IPI_TYPE_DDEG_BURSTY:
        reg_value = DIGI_PMON_IPI_BITS_FW_DONE;
        break;
    
      default:
        break;
    } /* end switch */
    

    PMC_RETURN(reg_value);
} /* digi_pmon_ipi_bit_get */

/*******************************************************************************
* FUNCTION: digi_pmon_ipi_bit_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the status of the IPI bit.
*   
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   ipi_type        - Type of IPI bit
*   enable          - TRUE to set the IPI bit, FALSE to clear the IPI bit
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_pmon_ipi_bit_set(digi_handle_t          *digi_handle,
                                       digi_pmon_ipi_type_t    ipi_type,
                                       BOOL                    enable)
{
    PMC_ERROR           result = PMC_SUCCESS;
    UINT32              ipi_bit_num;
    hostmsg_handle_t   *hostmsg_handle = NULL;
    pmc_handle_t       *cpu_handle;

    PMC_ENTRY();
    
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(ipi_type < LAST_DIGI_PMON_IPI_TYPE, DIGI_ERR_INVALID_ARG, 0, 0);
    
    hostmsg_handle = (hostmsg_handle_t *)(digi_handle->base.common_info->hostmsg_handle);
    PMC_ASSERT((NULL != hostmsg_handle),HOSTMSG_ERR_INVALID_PARAMETERS,0,0);
    cpu_handle = (pmc_handle_t *) hostmsg_handle;
    

    ipi_bit_num = ipi_bit_tbl[ipi_type].ipi_bit_num;
    int_agr_reg_INTER_PROCESSOR_INTS_array_field_set(
                        NULL,
                        cpu_handle,
                        ipi_bit_num,
                        INT_AGR_REG_INTER_PROCESSOR_INTS_BIT_IPI_BITS_MSK,
                        INT_AGR_REG_INTER_PROCESSOR_INTS_BIT_IPI_BITS_OFF,
                        enable);

    PMC_RETURN(result);
} /* digi_pmon_ipi_bit_set */



/*******************************************************************************
* FUNCTION: digi_pmon_ipi_bit_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Checks the IPI BIT before reading shared memory. The IPI BIT must signal
*   to SW that FW has finished writing to shared memory.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   ipi_bit_value   - Value of ipi_bit_register
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_pmon_ipi_bit_check(digi_handle_t    *digi_handle,
                                          UINT32            ipi_bit_value)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

#ifndef PMC_SW_SIMULATION
    if (DIGI_PMON_IPI_BITS_SW_DONE == ipi_bit_value)
    {
        result = DIGI_ERR_IPI_BIT_IS_NOT_FW_DONE;
    }
#endif

    PMC_RETURN(result);
} /* digi_pmon_ipi_bit_check */

/*******************************************************************************
* FUNCTION: digi_pmon_shared_mem_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears the shared memory region.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_pmon_shared_mem_clear(digi_handle_t    *digi_handle)
{
    PMC_ERROR            result = PMC_SUCCESS;
    digi_pmon_t         *fw_pmon_ptr;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    result = digi_pmon_shared_mem_get(digi_handle, &fw_pmon_ptr);

    if (PMC_SUCCESS == result)
    {
        PMC_MEMSET(fw_pmon_ptr, 0, sizeof(digi_pmon_t));   
    }

    PMC_RETURN(result);
} /* digi_pmon_shared_mem_clear */

#endif /*DOXYGEN_PUBLIC_ONLY*/



/*
** End of file
*/
