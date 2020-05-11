/*******************************************************************************
*   COPYRIGHT (C) 2011-2012 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*       This file provides the interface implementations. Note that this module
*       is primarily a set of register interfaces for the DIGI CPU subsystem
*       to inteface to firmware.  There are some additional CPU
*       interrupt aggregation functions.
* 
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "cpuif_loc.h"
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

/*
** Forward References
*/

/*
** Public Functions
*/


/*******************************************************************************
* cpuif_int_agg_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable Aggregator interrupt
*
* INPUTS:
*   cpu_handle      - cpu handle to operate/access the DIGI cpu subsystem
*   int_no          - Interrupt Aggregator Number (< 64)
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
PUBLIC void cpuif_int_agg_enable(pmc_handle_t *cpu_handle,UINT32 int_no)
{
    UINT32 reg_no = int_no >> 4;
    UINT32 bit_no = int_no & 0xf;
    UINT32 bit_set = 1 << bit_no;
    UINT32 x;

    PMC_ENTRY();

    switch(reg_no)
    {
        case 0:  
            x = int_agr_field_IPI2PCIE_0_E0_get(NULL, cpu_handle);
            int_agr_field_IPI2PCIE_0_E0_set(NULL, cpu_handle, x | bit_set);
            break;
        case 1:  
            x = int_agr_field_IPI2PCIE_1_E1_get(NULL, cpu_handle);
            int_agr_field_IPI2PCIE_1_E1_set(NULL, cpu_handle, x | bit_set);
            break;
        case 2:  
            x = int_agr_field_IPI2PCIE_2_E2_get(NULL, cpu_handle);
            int_agr_field_IPI2PCIE_2_E2_set(NULL, cpu_handle, x | bit_set);
            break;
        case 3:  
            x = int_agr_field_IPI2PCIE_3_E3_get(NULL, cpu_handle);
            int_agr_field_IPI2PCIE_3_E3_set(NULL, cpu_handle, x | bit_set);
            break;
        default:
            PMC_ASSERT(FALSE,PMC_ERR_INVALID_PARAMETERS, 0, 0);
    }
    PMC_RETURN();
} /* cpuif_int_agg_enable */

/*******************************************************************************
* cpuif_int_agg_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable Aggregator interrupt
*
* INPUTS:
*   cpu_handle      - cpu handle to operate/access the DIGI cpu subsystem
*   int_no          - Interrupt Aggregator Number (< 64)
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
PUBLIC void cpuif_int_agg_disable(pmc_handle_t *cpu_handle,UINT32 int_no)
{
    UINT32 reg_no = int_no >> 4;
    UINT32 bit_no = int_no & 0xf;
    UINT32 mask = ~bit_no & 0xffff;
    UINT32 x;

    PMC_ENTRY();

    switch(reg_no)
    {
        case 0:  
            x = int_agr_field_IPI2PCIE_0_E0_get(NULL, cpu_handle);
            int_agr_field_IPI2PCIE_0_E0_set(NULL, cpu_handle, x & mask);
            break;
        case 1:  
            x = int_agr_field_IPI2PCIE_1_E1_get(NULL, cpu_handle);
            int_agr_field_IPI2PCIE_1_E1_set(NULL, cpu_handle, x & mask);
            break;
        case 2:  
            x = int_agr_field_IPI2PCIE_2_E2_get(NULL, cpu_handle);
            int_agr_field_IPI2PCIE_2_E2_set(NULL, cpu_handle, x & mask);
            break;
        case 3:  
            x = int_agr_field_IPI2PCIE_3_E3_get(NULL, cpu_handle);
            int_agr_field_IPI2PCIE_3_E3_set(NULL, cpu_handle, x & mask);
            break;
        default:
            PMC_ASSERT(FALSE,PMC_ERR_INVALID_PARAMETERS, 0, 0);
    }
    PMC_RETURN();
} /* cpuif_int_agg_disable() */

/*******************************************************************************
* cpuif_int_agg_clr
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clear Aggregator interrupt
*
* INPUTS:
*   cpu_handle      - cpu handle to operate/access the DIGI cpu subsystem
*   int_no          - Interrupt Aggregator Number (< 64)
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
PUBLIC void cpuif_int_agg_clr(pmc_handle_t *cpu_handle, UINT32 int_no)
{
    UINT32 reg_no = int_no >> 4;
    UINT32 bit_no = int_no & 0xf;
    UINT32 bit_set = 1 << bit_no;
    
    PMC_ENTRY();

    switch(reg_no)
    {
        case 0:  
            int_agr_field_IPI2PCIE_0_I0_set_to_clear(NULL, cpu_handle, bit_set);
            break;
        case 1:  
            int_agr_field_IPI2PCIE_1_I1_set_to_clear(NULL, cpu_handle, bit_set);
            break;
        case 2:  
            int_agr_field_IPI2PCIE_2_I2_set_to_clear(NULL, cpu_handle, bit_set);
            break;
        case 3:  
            int_agr_field_IPI2PCIE_3_I3_set_to_clear(NULL, cpu_handle, bit_set);
            break;
        default:
            PMC_ASSERT(FALSE,PMC_ERR_INVALID_PARAMETERS, 0, 0);
    }
    PMC_RETURN();
} /* cpuif_int_agg_clr() */

/*******************************************************************************
* cpuif_int_agg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get Aggregator interrupt value.
*
* INPUTS:
*   cpu_handle      - cpu handle to operate/access the DIGI cpu subsystem
*   reg_no          - Interrupt Aggregator reg no (0..3)
*
* OUTPUTS:
*   None
*
* RETURNS:
*   UINT32 - Interrupt aggregator value from the IPI2PCIE_x_Ix field.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 cpuif_int_agg_get(pmc_handle_t *cpu_handle, UINT32 reg_no)
{
    UINT32 result;
    PMC_ENTRY();

    switch(reg_no)
    {
        case 0:  
            result= int_agr_field_IPI2PCIE_0_I0_get(NULL, cpu_handle);
            break;
        case 1:  
            result= int_agr_field_IPI2PCIE_1_I1_get(NULL, cpu_handle);
            break;
        case 2:  
            result= int_agr_field_IPI2PCIE_2_I2_get(NULL, cpu_handle);
            break;
        case 3:  
            result= int_agr_field_IPI2PCIE_3_I3_get(NULL, cpu_handle);
            break;
        default:
            PMC_ASSERT(FALSE,PMC_ERR_INVALID_PARAMETERS, 0, 0);
    }
    PMC_RETURN(result);
}/*cpuif_int_agg_get() */

/*******************************************************************************
* cpuif_int_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable/disable PCIe interrupts 
*
* INPUTS:
*   cpu_handle      - cpu handle to operate/access the DIGI cpu subsystem
*   int_type        - Host Interrupt type 
*   int_agg_mask    - Interrupt Aggregator Mask
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
PUBLIC void cpuif_int_cfg(pmc_handle_t *cpu_handle,
                                cpuif_int_t int_type,
                                UINT32 int_agg_mask)
{
    PMC_ENTRY();


    switch(int_type) 
    {
        case CPUIF_INT_DISABLE:
        case CPUIF_INT_MSI_X:
        case CPUIF_INT_MSI:
        case CPUIF_INT_DEVICE_PIN:
            /* Disable legacy Interrupts */
            pcie_al_reg_LEGACY_INT_EN_REG_write(NULL,cpu_handle,0);
            break;
        case CPUIF_INT_LEGACY:
            /* Enable legacy Interrupts */
            pcie_al_reg_LEGACY_INT_EN_REG_write(NULL,cpu_handle,(1 << 8));
            break;
        default:
            break;
    }
 
    if (int_type == CPUIF_INT_DISABLE)
    {
        /* Mask the aggregator outbound interrupt */
        pcie_al_reg_PCIE_OB_IRQ_CLR_REG_write(NULL,cpu_handle,0xffffffff);
        /* Mask the aggregator outbound Doorbell interrupt */
        pcie_al_reg_PCIE_OB_DB_CLR_REG_write(NULL,cpu_handle,0xffffffff);
        /* Mask the Interrupt aggregator DEVICE */
        int_agr_reg_INT_AGGREGATOR_DEVICE_CLEAR_write(NULL,cpu_handle,0xffffffff);
    }
    else if (int_type == CPUIF_INT_DEVICE_PIN) 
    {
        /* Mask the aggregator outbound interrupt */
        pcie_al_reg_PCIE_OB_IRQ_CLR_REG_write(NULL,cpu_handle,0xffffffff);
        /* Mask the aggregator outbound Doorbell interrupt */
        pcie_al_reg_PCIE_OB_DB_CLR_REG_write(NULL,cpu_handle,0xffffffff);
        /* Unmask the Interrupt aggregator DEVICE */
        pcie_al_reg_PCIE_OB_DB_CLR_REG_write(NULL,cpu_handle,0xffffffff);
        int_agr_reg_INT_AGGREGATOR_DEVICE_SET_write(NULL,cpu_handle,int_agg_mask);
    }
    else 
    {
        /*  Unmask the Aggregator outbound interrupts */
        pcie_al_reg_PCIE_OB_IRQ_SET_REG_write(NULL,cpu_handle,int_agg_mask); 
        /* Mask the aggregator outbound Doorbell interrupt */
        pcie_al_reg_PCIE_OB_DB_CLR_REG_write(NULL,cpu_handle,0xffffffff);
        /* Mask the Interrupt aggregator DEVICE */
        int_agr_reg_INT_AGGREGATOR_DEVICE_CLEAR_write(NULL,cpu_handle,0xffffffff);
    }
    /* Clear IPI to PCIE Interrupt status registers */
    int_agr_reg_IPI2PCIE_0_INT_write(NULL,cpu_handle,0xffffffff);
    int_agr_reg_IPI2PCIE_1_INT_write(NULL,cpu_handle,0xffffffff);
    int_agr_reg_IPI2PCIE_2_INT_write(NULL,cpu_handle,0xffffffff);
    int_agr_reg_IPI2PCIE_3_INT_write(NULL,cpu_handle,0xffffffff);

    PMC_RETURN();
} /* cpuif_int_cfg() */

/*******************************************************************************
* cpuif_int_device_aggregator_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable one or more interrupts to propagate to the device pin or PCIE 
*   interrupt.
*
* INPUTS:
*   cpu_handle      - cpu handle to operate/access the DIGI cpu subsystem
*   int_type        - Host Interrupt type 
*   int_enable_mask - Interrupt Aggregator enable Mask
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
PUBLIC void cpuif_int_device_aggregator_set(pmc_handle_t *cpu_handle,
                                            cpuif_int_t int_type,
                                            UINT32 int_enable_mask)
{
    PMC_ENTRY();

    switch(int_type) 
    {
        case CPUIF_INT_DEVICE_PIN:
            /*  Configure the Aggregator outbound interrupts (towards the device pin irq) */
            int_agr_reg_INT_AGGREGATOR_DEVICE_SET_write(NULL,cpu_handle,int_enable_mask);
            break;

        case CPUIF_INT_MSI_X:
        case CPUIF_INT_MSI:
        case CPUIF_INT_LEGACY:
            /*  Configure the Aggregator outbound interrupts (towards the pcie irq) */
            pcie_al_reg_PCIE_OB_IRQ_SET_REG_write(NULL,cpu_handle,int_enable_mask); 
            break;
        default:
            PMC_ASSERT(TRUE, PMC_ERR_INVALID_PARAMETERS, int_type, 0);
    }
    PMC_RETURN();
} /* cpuif_int_device_aggregator_set */

/*******************************************************************************
* cpuif_int_device_aggregator_clr
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disable one or more interrupts from propagating to the device pin or PCIE 
*   interrupt.
*
* INPUTS:
*   cpu_handle      - cpu handle to operate/access the DIGI cpu subsystem
*   int_type        - Host Interrupt type 
*   int_disable_mask - Interrupt Aggregator disable Mask
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
PUBLIC void cpuif_int_device_aggregator_clr(pmc_handle_t *cpu_handle,
                                            cpuif_int_t int_type,
                                            UINT32 int_disable_mask)
{
    PMC_ENTRY();

    switch(int_type) 
    {
        case CPUIF_INT_DEVICE_PIN:
            /*  Configure the Aggregator outbound interrupts (towards the device pin irq) */
            int_agr_reg_INT_AGGREGATOR_DEVICE_CLEAR_write(NULL,cpu_handle,int_disable_mask);
            break;

        case CPUIF_INT_MSI_X:
        case CPUIF_INT_MSI:
        case CPUIF_INT_LEGACY:
            /*  Configure the Aggregator outbound interrupts (towards the pcie irq) */
            pcie_al_reg_PCIE_OB_IRQ_CLR_REG_write(NULL,cpu_handle,int_disable_mask); 
            break;
        default:
            PMC_ASSERT(TRUE, PMC_ERR_INVALID_PARAMETERS, int_type, 0);
    }
    PMC_RETURN();
} /* cpuif_int_device_aggregator_clr */

/*******************************************************************************
* cpuif_int_device_aggregator_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve the device pin or PCIE interrupt aggregator mask.
*
* INPUTS:
*   cpu_handle      - cpu handle to operate/access the DIGI cpu subsystem
*   int_type        - Host Interrupt type 
*
* OUTPUTS:
*   None
*
* RETURNS:
*   UINT32 - Interrupt Aggregator Mask
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 cpuif_int_device_aggregator_get(pmc_handle_t *cpu_handle, 
                                              cpuif_int_t int_type)
{
    UINT32 int_agg_mask = 0;

    PMC_ENTRY();

    switch(int_type) 
    {
        case CPUIF_INT_DEVICE_PIN:
            /* Retrieve the Aggregator outbound irq enables (towards the device pin irq) */
            int_agg_mask = int_agr_reg_INT_AGGREGATOR_DEVICE_EN_read(NULL,cpu_handle);
            break;

        case CPUIF_INT_MSI_X:
        case CPUIF_INT_MSI:
        case CPUIF_INT_LEGACY:
            /* Retrieve the Aggregator outbound irq enables (towards the pcie irq) */
            int_agg_mask = pcie_al_reg_PCIE_OB_IRQ_SET_REG_read(NULL,cpu_handle); 
            break;
        default:
            PMC_ASSERT(TRUE, PMC_ERR_INVALID_PARAMETERS, int_type, 0);
    }
    PMC_RETURN(int_agg_mask);
} /* cpuif_int_device_aggregator_get */


/*
** End of file
*/


