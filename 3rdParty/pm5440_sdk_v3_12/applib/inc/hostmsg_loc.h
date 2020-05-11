/*******************************************************************************
*  COPYRIGHT (C) 2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*  DESCRIPTION :
*     DIGI host message local definitions.
*
*  NOTES:
*     None.
*******************************************************************************/

#ifndef _HOSTMSG_LOC_H
#define _HOSTMSG_LOC_H

#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_handle.h"
#include "pmc_log.h"
#include "pmc_err.h"
#include "cpuif.h"
#include "hostmsg.h"
/* #include "digi_loc.h" */
/* don't include digi_loc.h in hostmsg_loc.h because it breaks the compile of lineotn_unit_test */

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/* COMMON MACRO DEFINITIONS                                                   */
/******************************************************************************/

/* #define DISABLE_Q_FOR_FW_MESSAGES */

/* Magic number for hostmsg_handle_t type objects. */
#define HOSTMSG_T_CLASS_MAGIC   (0x77BE0011)

/* No longer necessary since hostmsg_handle moved to pmc_handle.
* #define HOSTMSG_HOSTMSG_PTR_GET(hostmsg_handle_t_p, pmc_sys_handle_t_base_p) do {                   \
*         PMC_ASSERT((NULL != pmc_sys_handle_t_base_p), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);        \
*         hostmsg_handle_t_p = (((helper_sys_handle_t *)pmc_sys_handle_t_base_p)->hostmsg_handle );   \
* } while(0)
* 
* #define HOSTMSG_SYS_HANDLE_GET(sys_handle_t_p, digi_handle_t_p) do {                    \
*         PMC_ASSERT((NULL != digi_handle_t_p), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);    \
*         sys_handle_t_p=((pmc_handle_t *)digi_handle_t_p)->sys_handle;                   \
*         PMC_ASSERT((NULL != sys_handle_t_p), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);     \
* } while(0)
*/

/* Check if hostmsg_handle_t object contains valid magic number. */
#define IS_HOSTMSG_PTR_MAGIC(hostmsg_handle) (HOSTMSG_T_CLASS_MAGIC == hostmsg_handle->magic)

/*
   Inbound mailbox address. Assume it is offset from the chip start.
   app_spram_inbox_beg_uncached. It used to be 0x1BD3FE00.
*/
#define HOSTMSG_IBND_MAILBOX_ADDR(x)    (x->inbound_mailbox_base_addr)

/*
  Outbound mailbox address. Assume it is offset from the chip start.
   app_spram_outbox_beg_uncached. It used to be 0x1BD3FD00.
*/
#define HOSTMSG_OBND_MAILBOX_ADDR(x)    (x->outbound_mailbox_base_addr)

    
#define CONVERT_MIPS_ADDR_TO_HOST_ADDR(x) ((UINT32)x & 0x1FFFFFF)

/*
 * Protocol related definitions.
 */

/* Doorbell numbers to use. */
#define HOSTMSG_INBOUND_DOORBELL_NUM    (0)
#define HOSTMSG_OUTBOUND_DOORBELL_NUM   (0)

 /*Max number of inbound doorbells.*/
#define HOSTMSG_MAX_IBND_DOORBELL_NUM   (32)

/*Max number of outbound doorbells.*/
#define HOSTMSG_MAX_OBND_DOORBELL_NUM   (32)

#define HOSTMSG_CATEGORY_CMD_MASK       (0x00)
#define HOSTMSG_CATEGORY_RSP_MASK       (0xF0)

/* Message header word 1 */
#define HOSTMSG_MODID_OFF               (24)
#define HOSTMSG_MODID_MSK               (0xFF << HOSTMSG_MODID_OFF)
#define HOSTMSG_MSGCAT_OFF              (16)
#define HOSTMSG_MSGCAT_MSK              (0xFF << HOSTMSG_MSGCAT_OFF)
#define HOSTMSG_MSGLEN_OFF              (0)
#define HOSTMSG_MSGLEN_MSK              (0xFFFF << HOSTMSG_MSGLEN_OFF)

/* Message header word 2 */
#define HOSTMSG_MSGSUBTYP_MSK           (0xFF << 24)
#define HOSTMSG_MSGSUBTYP_OFF           (24)
#define HOSTMSG_MSGCTRL_MSK             (0xFF << 16)
#define HOSTMSG_MSGCTRL_OFF             (16)
#define HOSTMSG_MSGSEQNUM_MSK           (0xFFFF << 0)
#define HOSTMSG_MSGSEQNUM_OFF           (0)


/* Minimum and maximum length of the host message. Note that message can not be
   shorter than message header. */
#define HOSTMSG_MIN_MSG_LEN             (HOSTMSG_NUM_HEADER_WORDS)
#define HOSTMSG_MAX_MSG_LEN             (HOSTMSG_MAX_MAILBOX_SIZE)

#define HOSTMSG_5000_USEC               (5000)
#define HOSTMSG_60_MSEC                 (60)
#define HOSTMSG_500_MSEC                (500)
#define HOSTMSG_1000_MSEC               (1000)
#define HOSTMSG_10000_MSEC              (10000)
#define HOSTMSG_DOORBELL_POLL_TIME      (HOSTMSG_1000_MSEC)
#define DIGI_USEC_PER_MSEC              (1000)

#define mGET_FIELD4(word, fMsk, fOff) \
        (((word) & ((UINT32)(fMsk))) >> (fOff))

#define mFORM_FIELD4(fVal, fMsk, fOff) \
        ((((UINT32)(fVal))<< (fOff)) & ((UINT32)(fMsk)))

#define mSET_FIELD4(word, fVal, fMsk, fOff) \
        (((word) & (~(UINT32)(fMsk))) |     \
        ((((UINT32)(fVal)) << fOff)  & ((UINT32)(fMsk))))

/* firmware context offsets and size 
   these values shall the same as the ones defined in digi_fw_memmap.h
*/
#define FW_WARM_RESTART_MAGIC_ID              0xBEEB00BA
#define FW_COLD_START_MAGIC_ID                0x0

/*******************************************************************************
 * ENUMERATED TYPES
 ******************************************************************************/

/*******************************************************************************
* STRUCTURE: hostmsg_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to hold hostmsg information data.
*
* ELEMENTS:
*   base - pmc handle - DO NOT MOVE, MUST BE FIRST MEMBER
*   magic - magic number for all hostmsg_handle_t objects. This is here because,
*           there will be access by offset.
*   msg_seq_num - Message sequence number. It does not have to start from 0.
*                 Only host can set it and increment it, firmware is just
*                 returning it with reply.
*   inbound_mailbox_base_addr - Base address of the inbound mailbox. This
*                              address is relative to chip base address.
*   outbound_mailbox_base_addr - Base address of the outbound mailbox. This
*                                address is relative to chip base address.
*   inbound_mailbox_size - Size of inbound mailbox (in words).
*   outbound_mailbox_size - Size of outbound mailbox (in words).
*   inbound_mailbox_messages_max - Maximum number of inbound mailbox messages.
*   outbound_mailbox_messages_max - Maximum number of outbound mailbox messages.
*   inbound_mailbox_message_size_max - Maximum size of inbound mailbox messages
*                                      (in words).
*   outbound_mailbox_message_size_max - Maximum size of outbound mailbox messages
*                                       (in words).
*   hostmsg_data_buf - Data buffer for inbound and outbound messages. Its size
*                      must be sufficient to handle inbound and outbound
*                      messages. It has to be greater or equal to
*                      in/outbound_mailbox_message_size_max, whichever is
*                      greater.
*  cur_hostmsg_stat  - statistics data tracking
*  prev_hostmsg_stat - statistics data tracking
*******************************************************************************/
typedef struct hostmsg_handle_t
{
    pmc_handle_t base;
    UINT32 magic;
    UINT32 msg_seq_num;
    UINT32 inbound_mailbox_base_addr;
    UINT32 outbound_mailbox_base_addr;
    UINT32 inbound_mailbox_size;
    UINT32 outbound_mailbox_size;
    UINT32 inbound_mailbox_messages_max;
    UINT32 outbound_mailbox_messages_max;
    UINT32 inbound_mailbox_message_size_max;
    UINT32 outbound_mailbox_message_size_max;
    void *hostmsg_data_buf;
} hostmsg_handle_t;

/*******************************************************************************
* STRUCTURE: helper_sys_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to mimic first two members of any sys_handle structure
*   (e.g. emulation_sys_handle_t, etc.).
*
* ELEMENTS:
*   internal_sys_handle - Sys handle structure must start with this internal
*                         PMC pointer
*   hostmsg_handle - Host message specific information
*******************************************************************************/
typedef struct helper_sys_handle
{
    /* Sys handle structure must start with this internal PMC pointer */
	void *internal_sys_handle;
    hostmsg_handle_t *hostmsg_handle;
} helper_sys_handle_t;

/*******************************************************************************
* STRUCTURE: hostmsg_sys_stat_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to keep sys stat per module id / subtype 
*
* ELEMENTS:
*   modid -  module id
*   subtype - command subtype 
*   stat - statistics
*
*******************************************************************************/
typedef struct hostmsg_sys_stat_t
{
    UINT32 modid;
    UINT32 subtype;
    pmc_sys_stat_t stat;
} hostmsg_sys_stat_t;

/*******************************************************************************
 *
 * Public method prototypes.
 *
*******************************************************************************/
PUBLIC PMC_ERROR hostmsg_check_fw_ready_to_receive_inbound_message(hostmsg_handle_t *hostmsg_handle);
PUBLIC PMC_ERROR hostmsg_poll_outbound_doorbell_int(hostmsg_handle_t *hostmsg_handle,
                                                    UINT32 num_attempts);
PUBLIC BOOL hostmsg_check_outbound_doorbell_int(hostmsg_handle_t *hostmsg_handle);
PUBLIC void hostmsg_interrupt_ipi2pcie_clear(hostmsg_handle_t *hostmsg_handle);
PUBLIC void hostmsg_interrupt_ipi2pcie_enable(hostmsg_handle_t *hostmsg_handle);
PUBLIC void hostmsg_read(hostmsg_handle_t *hostmsg_handle, UINT32 vaddr,
                         UINT32 len, UINT32 *data);
PUBLIC void hostmsg_write(hostmsg_handle_t *hostmsg_handle, UINT32 vaddr,
                          UINT32 len, UINT32 *data);
PUBLIC PMC_ERROR hostmsg_inbound_doorbell_register_get(hostmsg_handle_t *hostmsg_handle);
PUBLIC void hostmsg_inbound_doorbell_register_set(hostmsg_handle_t *hostmsg_handle);
PUBLIC PMC_ERROR hostmsg_outbound_doorbell_int_register_get(hostmsg_handle_t *hostmsg_handle);
PUBLIC PMC_ERROR hostmsg_mailbox_data_init(hostmsg_handle_t *hostmsg_handle);

#ifdef __cplusplus
}
#endif

#endif /* _HOSTMSG_LOC_H */
/* end of file */

