/*******************************************************************************
*   COPYRIGHT (C) 2008-2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.                  
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
*   DESCRIPTION:
*    This file contains PMC system access function definitions. This file is
*    setup to support customer systems as well as the PMC EVBD, emulation,
*    simulation, and verification environment. There is no customer porting
*    required in this file that occurs in pmc_customer_os.h and
*    pmc_customer_os.c.
* 
*   NOTES: 
*
******************************************************************************/

#ifndef L6_SYS_H
#define L6_SYS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "pmc_types.h"
#include "pmc_err.h"

#ifdef PMC_SW_SIMULATION
#include "pmc_sw_simulation.h"
#else
#define simulation_get_description( sys, addr ) ""
#define simulation_get_description_and_value( sys, addr, mask, value ) ""
#endif /* PMC_SW_SIMULATION */


#ifdef PMC_CUSTOMER_OS
#include "pmc_customer_os.h"
#endif /*  PMC_CUSTOMER_OS */

#ifdef PMC_COMEXPRESS
#include "pmc_emulation.h"
#endif /*  PMC_COMEXPRESS */

/******************************************************************************
*  pmc_sys_reg_read
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*                 This function performs a direct register read of a 32 bit
*                 wide register.
*
*  INPUTS:        
*                 *hndl - opaque customer's handle.
*                 addr        - address of the register location to be read
*                               as it appears in the hardware data sheet
*
*  RETURNS:       
*                 UINT32 - 32 bit value read.
*
******************************************************************************/
static INLINE UINT32 pmc_sys_reg_read ( pmc_sys_handle_t *hndl,
                                        UINT32 addr ) ATTRIBUTE_WARN_UNUSED_RESULT;

static INLINE UINT32 pmc_sys_reg_read ( pmc_sys_handle_t *hndl,
                                        UINT32 addr )
{
#ifdef PMC_SW_SIMULATION
    return simulation_sys_reg_read( hndl, addr );
#else
#ifdef PMC_CUSTOMER_OS
    return customer_sys_reg_read( hndl, addr );
#else
#ifdef PMC_COMEXPRESS
    return emulation_sys_reg_read( hndl, addr);
#else
    /*--- CUSTOMER INTEGRATION POINT ---*/
    /* example code direct memory access */
    return *(volatile UINT32 *) (size_t)((UINT32)(addr));
#endif
#endif
#endif
} /* pmc_sys_reg_read */


/*******************************************************************************
*  pmc_sys_reg_write
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*                 This function performs a direct register write of a 32 bit
*                 wide register.
*
*  INPUTS:        
*                 *hndl       - opaque customer's handle.
*                 addr        - address of the register location to be read
*                               as it appears in the hardware data sheet
*                 value       - 32 bit value to write to the register.
*
*  OUTPUS:
*   None
* 
*  RETURNS:       
*   None
*
******************************************************************************/
static INLINE void pmc_sys_reg_write( pmc_sys_handle_t *hndl,
                                      UINT32 addr,
                                      UINT32 value )
{
#ifdef PMC_SW_SIMULATION
    simulation_sys_reg_write( hndl, addr, value );
#else
#ifdef PMC_CUSTOMER_OS
    customer_sys_reg_write( hndl, addr, value );
#else
#ifdef PMC_COMEXPRESS
    emulation_sys_reg_write( hndl, addr, value );
#endif
#endif
#endif
} /* pmc_sys_reg_write */

/*******************************************************************************
*  pmc_sys_burst_read
*  ___________________________________________________________________________
*
*  DESCRIPTION:   
*                 This function performs a burst read of an array of 32 bit
*                 wide registers.
*
*  INPUTS:        
*                 *hndl   - customer's opaque handle.
*                 addr    - start address of the register location to be read
*                           as it appears in the hardware data sheet
*                 len     - number of words to read.
*                 data    - array of 32 bit values read from the registers
*
*  RETURNS:       
*                 none.
*
******************************************************************************/
static INLINE void pmc_sys_burst_read( pmc_sys_handle_t *hndl,
                                       UINT32 addr,
                                       UINT32 len,
                                       UINT32 *data )
{
#ifdef PMC_SW_SIMULATION
    simulation_sys_burst_read( hndl, addr, len, data );
#else
#ifdef PMC_CUSTOMER_OS
    customer_sys_burst_read( hndl, addr, len, data );
#else
#ifdef PMC_COMEXPRESS
    emulation_sys_burst_read( hndl, addr, len, data );
#endif
#endif
#endif
} /* pmc_sys_burst_read */

/*******************************************************************************
*  pmc_sys_burst_write
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                 This function performs a burst write of an array of 32 bit
*                 wide registers.
*
*  INPUTS:
*                 *hndl   - customer's opaque handle.
*                 addr    - start address of the register location to be written
*                           as it appears in the hardware data sheet
*                 len     - number of words to write.
*                 data    - array of 32 bit values to write to the registers.
*
*  RETURNS:
*                 none.
*
******************************************************************************/
static INLINE void pmc_sys_burst_write( pmc_sys_handle_t *hndl, 
                                        UINT32 addr,
                                        UINT32 len,
                                        UINT32 *data )
{
#ifdef PMC_SW_SIMULATION
    simulation_sys_burst_write( hndl, addr, len, data );
#else
#ifdef PMC_CUSTOMER_OS
    customer_sys_burst_write( hndl, addr, len, data );
#else
#ifdef PMC_COMEXPRESS
    emulation_sys_burst_write( hndl, addr, len, data );
#endif
#endif
#endif
} /* pmc_sys_burst_write */

/*******************************************************************************
*  pmc_sys_mem_write
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                 This function performs a memory write of 8 bit data of len
*                 bytes. Since it is a byte array there is no endian swapping
*                 performed.
*
*  INPUTS:
*                 *hndl   - customer's opaque handle.
*                 addr    - offset address of the register location to be written
*                           from the base address in hndl
*                 len     - number of bytes to write.
*                 data_ptr - array of 8 bit values to write to addr.
*
*  RETURNS:
*                 none.
*
******************************************************************************/
static INLINE void pmc_sys_mem_write(pmc_sys_handle_t *hndl, 
                                     UINT32 addr,
                                     UINT32 len,
                                     UINT8 *data_ptr)
{
#ifdef PMC_SW_SIMULATION
    simulation_sys_mem_write( hndl, addr, len, data_ptr );
#else
#ifdef PMC_CUSTOMER_OS
    customer_sys_mem_write( hndl, addr, len, data_ptr );
#else
#ifdef PMC_COMEXPRESS
    emulation_sys_mem_write( hndl, addr, len, data_ptr );
#endif
#endif
#endif
} /* pmc_sys_mem_write */

/*******************************************************************************
*  pmc_sys_reg_read_modify_write
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                 This function performs a burst write of an array of 32 bit
*                 wide registers.
*
*  INPUTS:
*                 *hndl   - customer's opaque handle.
*                 addr    - start address of the register location to be written
*                           as it appears in the hardware data sheet
*                 mask    - bits to update.
*                 value   - new value for the updated field (note: this value
*                           must be pre-shifted)
*
*  OUTPUTS:
*                 *oldval  - previous value of the register.
*                            note: NULL may be passed in for oldval.
*
*  RETURNS:
*                 UINT32 - value written to the register.
*
*  NOTES:
*  suppose the previous value of the register was 0x12345678 and
*  we want to update the middle bits to 0xcafe.
*
*  UINT32 mask  = 0x00ffff00;
*  UINT32 value = 0x00cafe00;
*  UINT32 newval;
*  UINT32 oldval;
*
*  newval = pmc_sys_reg_read_modify_write( hndl, addr, mask, value, &oldval );
*
*  // at this point oldval and new val would be:
*  // oldval = 0x12345678
*  // newval = 0x12cafe78
*
******************************************************************************/
static INLINE UINT32 pmc_sys_reg_read_modify_write( pmc_sys_handle_t *hndl, 
                                                    UINT32 addr,
                                                    UINT32 mask,
                                                    UINT32 value,
                                                    UINT32 *oldval )
{
#ifdef PMC_CUSTOMER_OS
   return customer_sys_reg_read_modify_write( hndl, addr, mask, value, oldval );
#else
#ifdef PMC_SW_SIMULATION
   return simulation_sys_reg_read_modify_write( hndl, addr, mask, value, oldval );
#else
#ifdef PMC_COMEXPRESS
   return emulation_sys_reg_read_modify_write( hndl, addr, mask, value, oldval );
#else
   return 0;    // add by xxu1 to avoid compile warning
#endif
#endif
#endif
} /* pmc_sys_reg_read_modify_write */

/*******************************************************************************
*  pmc_sys_poll
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                 This function polls a register until the comparison is met
*
*  INPUTS:
*                 *sys      - customer's opaque handle.
*                 addr      - address of the register location to be read
*                             as it appears in the hardware data sheet
*                 mask      - field being examined.
*                 value     - desired value of the field (note: this value
*                             must be pre-shifted)
*                 cmp       - type of comparison
*                 max_count - maximum number of times to read the register.
*                 delay_between_polls_in_microseconds
*                           - how long to wait between register reads.
*
*  OUTPUTS:
*                 *num_failed_polls - number of register reads that were
*                                     not the desired value.
*
*  RETURNS:
*                 PMC_SUCCESS
*                 PMC_ERR_POLL_TIMEOUT
*
*  NOTES:
*  To wait for the value of the middle bits of a field to be 0xcafe
*  (which admittedly is strange thing to wait for) the following code could
*  be used:
*
*  UINT32 mask  = 0x00ffff00;
*  UINT32 value = 0x00cafe00;
*  UINT32 max_count = 1000;  // wait for up to 1000 register reads.
*  UINT32 num_failed_polls;
*  UINT32 delay_between_polls_in_microseconds = 10; // wait 10 microseconds between reads.
*  UINT32 result;
*
*  result = pmc_sys_poll( hndl, addr, mask, value, PMC_POLL_EQUAL, max_count, &num_failed_polls, delay_between_polls_in_microseconds );
*  if (result == PMC_ERR_POLL_TIMEOUT)
*  {
*    ... // poll timeout
*  }
*
******************************************************************************/
PMC_POLL_RETURN_TYPE pmc_sys_poll( pmc_sys_handle_t *sys, 
                                   UINT32 addr,
                                   UINT32 mask,
                                   UINT32 value,
                                   PMC_POLL_COMPARISON_TYPE cmp,
                                   UINT32 max_count,
                                   UINT32 *num_failed_polls,
                                   UINT32 delay_between_polls_in_microseconds );

/*******************************************************************************
*  pmc_sys_poll_bit
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                 This function polls a register field until the desired value is read.
*
*  INPUTS:
*                 *sys      - customer's opaque handle.
*                 addr      - address of the register location to be read
*                             as it appears in the hardware data sheet
*                 mask      - field being examined.
*                 value     - desired value of the field (note: this value
*                             must be pre-shifted)
*                 max_count - maximum number of times to read the register.
*                 delay_between_polls_in_microseconds
*                           - how long to wait between register reads.
*
*  OUTPUTS:
*                 *num_failed_polls - number of register reads that were
*                                     not the desired value.
*
*  RETURNS:
*                 PMC_SUCCESS
*                 PMC_ERR_POLL_TIMEOUT
*
*  NOTES:
*  To wait for the value of the middle bits of a field to be 0xcafe
*  (which admittedly is strange thing to wait for) the following code could
*  be used:
*
*  UINT32 mask  = 0x00ffff00;
*  UINT32 value = 0x00cafe00;
*  UINT32 max_count = 1000;  // wait for up to 1000 register reads.
*  UINT32 num_failed_polls;
*  UINT32 delay_between_polls_in_microseconds = 10; // wait 10 microseconds between reads.
*  UINT32 result;
*
*  result = pmc_sys_poll_bit( hndl, addr, mask, value, max_count, &num_failed_polls, delay_between_polls_in_microseconds );
*  if (result == PMC_ERR_POLL_TIMEOUT)
*  {
*    ... // poll timeout
*  }
*
******************************************************************************/
PMC_ERROR pmc_sys_poll_bit( pmc_sys_handle_t *sys, 
                            UINT32 addr,
                            UINT32 mask,
                            UINT32 value,
                            UINT32 max_count,
                            UINT32 *num_failed_polls,
                            UINT32 delay_between_polls_in_microseconds );


/*******************************************************************************
*  pmc_sys_wb_invalidate_cache
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*                 This function writes back and invalidates the cache lines for
*                 a region of memory.
*
*  INPUTS:
*                 virt_addr  - virtual address of the memory region to flush.
*                 size       - size in bytes of the region to be flushed.
*
*  OUTPUTS:
*    None
* 
*  RETURNS:
*    none
*
*  NOTES:
*    This is used when communicating with the firmware queue and
*    will be used when reading the PMON data uploaded by firmware.
*
******************************************************************************/
static INLINE void pmc_sys_wb_invalidate_cache( void *virt_addr, UINT32 size )
{
#ifdef PMC_CUSTOMER_OS
   customer_sys_wb_invalidate_cache( virt_addr, size );
#endif
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PMC_SYS_H */
