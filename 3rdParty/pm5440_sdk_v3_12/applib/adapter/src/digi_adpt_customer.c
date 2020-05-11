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
*   DESCRIPTION: 
*     This file includes card specific modification information. Customer need
*     modify the table based on card design.
*
*     This file is drafted under Linux operation system. Customer need modify 
*     APIs according to your system requirement.
*
*   NOTES:
*     None.
*
*******************************************************************************/
/*
** Include Files
*/
#include "digi_adpt.h"

#ifdef PMC_COMEXPRESS
#include <errno.h>
#include <sys/stat.h>
#endif

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Local Macro Definitions
*/
#ifdef NIU
#undef NIU
#endif
#define NIU  (digi_serdes_intf_t)DIGI_SERDES_PIN_NOT_USED

#ifdef LINK
#undef LINK
#endif
#define LINK SIFD_ILKN_LINK_FLOW_CONTROL

#ifdef LINE
#undef LINE
#endif
#define LINE DIGI_SERDES_LINE_INTF

#ifdef SFI51
#undef SFI51
#endif
#define SFI51 DIGI_SERDES_SFI_51_INTF

/*
** Local Structures and Unions
*/

/*
** Local Variables
*/
PRIVATE digi_dev_info_t g_dev_info[] =
{
    {
        HW_CARD_6x10G_TRANSPONDER_SYS_CLIENT,                               /* card id */
        DIGI60_PROD_APP_TRANSMUXPONDER_SYS_CLIENT_CARD,                     /* card type */
        "digi",                                                             /* file name for data storage */
#ifdef PMC_CUSTOMER_OS
        DIGI_WARM_CTXT_IN_PLACE,                                            /* warm restart type */
#else
        DIGI_WARM_CTXT_COPY,                                                /* warm restart type */
#endif
        256,                                                                /* packet size in OPSA */
        4,                                                                  /* head size in OPSA */
        DIGI_OTN_MUX_STAGES_TWO,                                            /* maximum multiplex stages */
        DIGI_TRIGGER_MANUAL,                                                /* PMON trigger mode */
        0x7,                                                                /* manual trigger mask */
        1000,                                                               /* trigger period: 250 ~ 1000(ms) */
        0x7D07,                                                             /* PMON work order */
        TRUE,                                                               /* Ethernet PMON enable/disable in Tx */
        CPUIF_INT_DEVICE_PIN,                                               /* interrupt process type */
        FALSE,                                                              /* static SerDes configuration */
        NULL,                                                               /* FW path and file name */
        6,                                                                  /* number of line serdes port */
        { LINE, LINE, LINE, LINE, LINE, LINE, NIU, NIU, NIU, NIU, NIU, NIU }, /* serdes type */
        { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, NIU, NIU, NIU, NIU, NIU, NIU },     /* line cage id */
        {                                                                   /* line tx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* line rx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },                                                
        { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, NIU, NIU, NIU, NIU, NIU, NIU },     /* line port LOS pin */
        FALSE,                                                              /* line port LOS inverse status */
        6,                                                                  /* number of system serdes port */
        { 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, NIU, NIU, NIU, NIU, NIU, NIU },     /* system cage id */
        {                                                                   /* system tx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* system rx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, NIU, NIU, NIU, NIU, NIU, NIU },     /* system port LOS pin */
        FALSE,                                                              /* system port LOS inverse status */
        ILKN_SIFD_NONE,                                                     /* Interlaken SIFD type */
    },
    {
        HW_CARD_6x10G_TRANSPONDER,                                          /* card id */
        DIGI60_PROD_APP_TRANSMUXPONDER_CARD,                                /* card type */
        "digi",                                                             /* file name for data storage */
#ifdef PMC_CUSTOMER_OS
        DIGI_WARM_CTXT_IN_PLACE,                                            /* warm restart type */
#else
        DIGI_WARM_CTXT_COPY,                                                /* warm restart type */
#endif
        256,                                                                /* packet size in OPSA */
        4,                                                                  /* head size in OPSA */
        DIGI_OTN_MUX_STAGES_TWO,                                            /* maximum multiplex stages */
        DIGI_TRIGGER_MANUAL,                                                /* PMON trigger mode */
        0x7,                                                                /* manual trigger mask */
        1000,                                                               /* trigger period: 250 ~ 1000(ms) */
        0x7D07,                                                             /* PMON work order */
        TRUE,                                                               /* Ethernet PMON enable/disable in Tx */
        CPUIF_INT_DEVICE_PIN,                                               /* interrupt process type */
        FALSE,                                                              /* static SerDes configuration */
        NULL,                                                               /* FW path and file name */
        12,                                                                 /* number of line serdes port */
        { LINE, LINE, LINE, LINE, LINE, LINE, LINE, LINE, LINE, LINE, LINE, LINE }, /* serdes type */
        { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb },     /* line cage id */
        {                                                                   /* line tx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0 } 
        },
        {                                                                   /* line rx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0 } 
        },                                                
        { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb },     /* line port LOS pin */
        FALSE,                                                              /* line port LOS inverse status */
        0,                                                                  /* number of system serdes port */
        { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* system cage id */
        {                                                                   /* system tx port lane information */
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* system rx port lane information */
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },                                                
        { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* system port LOS pin */
        FALSE,                                                              /* system port LOS inverse status */
        ILKN_SIFD_NONE,                                                     /* Interlaken SIFD type */
    },

    {
        HW_CARD_10x10G_ANY_OTN_TRIBUTARY,                                   /* card id */
        DIGI_PROD_APP_LINE_CARD,                                            /* card type */
        "digi",                                                             /* file name for data storage */
#ifdef PMC_CUSTOMER_OS
        DIGI_WARM_CTXT_IN_PLACE,                                            /* warm restart type */
#else
        DIGI_WARM_CTXT_COPY,                                                /* warm restart type */
#endif
        256,                                                                /* packet size in OPSA */
        4,                                                                  /* head size in OPSA */
        DIGI_OTN_MUX_STAGES_TWO,                                            /* maximum multiplex stages */
        DIGI_TRIGGER_MANUAL,                                                /* PMON trigger mode */
        0x7,                                                                /* manual trigger mask */
        1000,                                                               /* trigger period: 250 ~ 1000(ms) */
        0x7D07,                                                             /* PMON work order */
        TRUE,                                                               /* Ethernet PMON enable/disable in Tx */
        CPUIF_INT_DEVICE_PIN,                                               /* interrupt process type */
        FALSE,                                                              /* static SerDes configuration */
        NULL,                                                               /* FW path and file name */
        10,                                                                 /* number of line serdes port */
        { LINE, LINE, LINE, LINE, LINE, LINE, LINE, LINE, LINE, LINE, NIU, NIU }, /* serdes type */
        { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },     /* line cage id */
        {                                                                   /* line tx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* line rx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },                                                
        { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },     /* line port LOS pin */
        FALSE,                                                              /* line port LOS inverse status */
        0,                                                                  /* number of system serdes port */
        { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* system cage id */
        {                                                                   /* system tx port lane information */
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* system rx port lane information */
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },                                                
        { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* system port LOS pin */
        FALSE,                                                              /* system port LOS inverse status */
        ILKN_SIFD_1,                                                        /* Interlaken SIFD type */
        {                                                                   /* Interlaken 1 information */
            { 
                DIGI_ILKN_SERDES_RATE_10G3125, 0xFFFF,
                {
                    {
                        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
                        13, 14, 15, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU
                    }
                },
                {
                    {
                        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
                        13, 14, 15, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU
                    }
                },
                DIGI_SIFD_BURST_128, DIGI_SIFD_BURST_MIN_DISABLE, 
                DIGI_SIFD_BURST_SHORT_32, DIGI_SIFD_ILKN_MF_2048, { 0, 1024, 4 },
                TRUE, DIGI_SIFD_XFER_MODE_SEGMENT,
                DIGI_SIFD_ODUKSW_ILKN_HEADER_NONE, TRUE, TRUE,
                DIGI_SIFD_XFER_MODE_SEGMENT, DIGI_SIFD_XFER_MODE_SEGMENT,
                DIGI_SIFD_FC_MODE_NONE, 300,
            },
            { 0, 0xFFFF, 0}
        },
        {
            {
            /* ICW/BCW field In-Band Flow Control bits 55:40: */
            /*  55, 54, 53,  52,  51,  50,  49,  48,  47,  46,  45,  44,  43,  42,  41,  40, */
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
            }
        },
        {
            {
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
            }    
        },
    },

    {
        HW_CARD_2x40G_OTN_TRIBUTARY,                                        /* device id */
        DIGI_PROD_APP_LINE_CARD,                                            /* card type */
        "digi",                                                             /* file name for data storage */
#ifdef PMC_CUSTOMER_OS
        DIGI_WARM_CTXT_IN_PLACE,                                            /* warm restart type */
#else
        DIGI_WARM_CTXT_COPY,                                                /* warm restart type */
#endif
        256,                                                                /* packet size in OPSA */
        4,                                                                  /* head size in OPSA */
        DIGI_OTN_MUX_STAGES_TWO,                                            /* maximum multiplex stages */
        DIGI_TRIGGER_MANUAL,                                                /* PMON trigger mode */
        0x7,                                                                /* manual trigger mask */
        1000,                                                               /* trigger period: 250 ~ 1000(ms) */
        0x7D07,                                                             /* PMON work order */
        TRUE,                                                               /* Ethernet PMON enable/disable in Tx */
        CPUIF_INT_DEVICE_PIN,                                               /* interrupt process type */
        FALSE,                                                              /* static SerDes configuration */
        NULL,                                                               /* FW path and file name */
        2,                                                                  /* number of line serdes port */
        { LINE, LINE, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },   /* serdes type */
        { 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x1, NIU, NIU, NIU, NIU },     /* line cage id */
        {                                                                   /* line tx port lane information */
            { 0x0, 0x1, 0x2, 0x3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, 0x1, 0x2, 0x3, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* line rx port lane information */
            { 0x0, 0x1, 0x2, 0x3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, 0x1, 0x2, 0x3, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        { 0x0, 0x4, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* line port LOS pin */
        FALSE,                                                              /* line port LOS inverse status */
        0,                                                                  /* number of system serdes port */
        { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* system cage id */
        {                                                                   /* system tx port lane information */
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* system rx port lane information */
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },                                                
        { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* system port LOS pin */
        FALSE,                                                              /* system port LOS inverse status */
        ILKN_SIFD_1,                                                        /* Interlaken SIFD type */
        {                                                                   /* Interlaken 1 information */
            { 
                DIGI_ILKN_SERDES_RATE_10G3125, 0xFFFF,
                {
                    {
                        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
                        13, 14, 15, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU
                    }
                },
                {
                    {
                        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
                        13, 14, 15, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU
                    }
                },
                DIGI_SIFD_BURST_128, DIGI_SIFD_BURST_MIN_DISABLE, 
                DIGI_SIFD_BURST_SHORT_32, DIGI_SIFD_ILKN_MF_2048, { 0, 1024, 4 },
                TRUE, DIGI_SIFD_XFER_MODE_SEGMENT,
                DIGI_SIFD_ODUKSW_ILKN_HEADER_NONE, TRUE, TRUE,
                DIGI_SIFD_XFER_MODE_SEGMENT, DIGI_SIFD_XFER_MODE_SEGMENT,
                DIGI_SIFD_FC_MODE_NONE, 300,
            },
            { 0, 0xFFFF, 0}
        },
        {
            {
            /* ICW/BCW field In-Band Flow Control bits 55:40: */
            /*  55, 54, 53,  52,  51,  50,  49,  48,  47,  46,  45,  44,  43,  42,  41,  40, */
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
            }
        },
        {
            {
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
            }    
        },
    },

    {
        HW_CARD_1x100GE_OTN_TRIBUTARY,                                      /* device id */
        DIGI_PROD_APP_LINE_CARD,                                            /* card type */
        "digi",                                                             /* file name for data storage */
#ifdef PMC_CUSTOMER_OS
        DIGI_WARM_CTXT_IN_PLACE,                                            /* warm restart type */
#else
        DIGI_WARM_CTXT_COPY,                                                /* warm restart type */
#endif
        256,                                                                /* packet size in OPSA */
        4,                                                                  /* head size in OPSA */
        DIGI_OTN_MUX_STAGES_TWO,                                            /* maximum multiplex stages */
        DIGI_TRIGGER_MANUAL,                                                /* PMON trigger mode */
        0x7,                                                                /* manual trigger mask */
        1000,                                                               /* trigger period: 250 ~ 1000(ms) */
        0x7D07,                                                             /* PMON work order */
        TRUE,                                                               /* Ethernet PMON enable/disable in Tx */
        CPUIF_INT_DEVICE_PIN,                                               /* interrupt process type */
        FALSE,                                                              /* static SerDes configuration */
        NULL,                                                               /* FW path and file name */
        1,                                                                  /* number of line serdes port */
        { LINE, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },    /* serdes type */
        /*****************************************************************
         * Modification on CXP laser:
         * 1. cage id/port lane start from index 1, instead of index 0.
         * 2. LOS inverse status should be TRUE.
         * 3. LOS pin should be set 1 at index 0.
         * 4. SerDes adaptation starts from bit 1, instead of bit 0.
         *****************************************************************/
        { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, NIU, NIU },     /* line cage id */
        {                                                                   /* line tx port lane information */
            { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* line rx port lane information */
            { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* line port LOS pin */
        FALSE,                                                              /* line port LOS inverse status */
        0,                                                                  /* number of system serdes port */
        { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* system cage id */
        {                                                                   /* system tx port lane information */
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* system rx port lane information */
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },                                                
        { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* system port LOS pin */
        FALSE,                                                              /* system port LOS inverse status */
        ILKN_SIFD_1,                                                        /* Interlaken SIFD type */
        {                                                                   /* Interlaken 1 information */
            { 
                DIGI_ILKN_SERDES_RATE_10G3125, 0xFFFF,
                {
                    {
                        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
                        13, 14, 15, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU
                    }
                },
                {
                    {
                        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
                        13, 14, 15, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU
                    }
                },
                DIGI_SIFD_BURST_128, DIGI_SIFD_BURST_MIN_DISABLE, 
                DIGI_SIFD_BURST_SHORT_32, DIGI_SIFD_ILKN_MF_2048, { 0, 1024, 4 },
                TRUE, DIGI_SIFD_XFER_MODE_SEGMENT,
                DIGI_SIFD_ODUKSW_ILKN_HEADER_NONE, TRUE, TRUE,
                DIGI_SIFD_XFER_MODE_SEGMENT, DIGI_SIFD_XFER_MODE_SEGMENT,
                DIGI_SIFD_FC_MODE_NONE, 300,
            },
            { 0, 0xFFFF, 0}
        },
        {
            {
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
            }
        },
        {
            {
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
            }    
        },
    },

    {
        HW_CARD_1x100G_TRANSPONDER,                                         /* device id */
        DIGI_PROD_APP_TRANSMUXPONDER_CARD,                                  /* card type */
        "digi",                                                             /* file name for data storage */
#ifdef PMC_CUSTOMER_OS
        DIGI_WARM_CTXT_IN_PLACE,                                            /* warm restart type */
#else
        DIGI_WARM_CTXT_COPY,                                                /* warm restart type */
#endif
        256,                                                                /* packet size in OPSA */
        4,                                                                  /* head size in OPSA */
        DIGI_OTN_MUX_STAGES_ONE,                                            /* maximum multiplex stages */
        DIGI_TRIGGER_MANUAL,                                                /* PMON trigger mode */
        0x7,                                                                /* manual trigger mask */
        1000,                                                               /* trigger period: 250 ~ 1000(ms) */
        0x7D07,                                                             /* PMON work order */
        TRUE,                                                               /* Ethernet PMON enable/disable in Tx */
        CPUIF_INT_DEVICE_PIN,                                               /* interrupt process type */
        FALSE,                                                              /* static SerDes configuration */
        NULL,                                                               /* FW path and file name */
        1,                                                                  /* number of line serdes port */
        { LINE, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },    /* serdes type */
        { NIU, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, NIU },     /* line cage id */
        {                                                                   /* line tx port lane information */
            { NIU, 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* line rx port lane information */
            { NIU, 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        { 0x1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* line port LOS pin */
        TRUE,                                                               /* line port LOS inverse status */
        1,                                                                  /* number of system serdes port */
        { NIU, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, NIU },     /* system cage id */
        {                                                                   /* system tx port lane information */
            { NIU, 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* system rx port lane information */
            { NIU, 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },                                                
        { 0x1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* system port LOS pin */
        TRUE,                                                               /* system port LOS inverse status */
        ILKN_SIFD_NONE,                                                     /* Interlaken SIFD type */
    },

    {
        HW_CARD_10x10G_ANY_MUXPONDER_TO_OTU4,                               /* device id */
        DIGI_PROD_APP_SYSOTN_CARD,                                          /* card type */
        "digi",                                                             /* file name for data storage */
#ifdef PMC_CUSTOMER_OS
        DIGI_WARM_CTXT_IN_PLACE,                                            /* warm restart type */
#else
        DIGI_WARM_CTXT_COPY,                                                /* warm restart type */
#endif
        256,                                                                /* packet size in OPSA */
        4,                                                                  /* head size in OPSA */
        DIGI_OTN_MUX_STAGES_ONE,                                            /* maximum multiplex stages */
        DIGI_TRIGGER_MANUAL,                                                /* PMON trigger mode */
        0x7,                                                                /* manual trigger mask */
        1000,                                                               /* trigger period: 250 ~ 1000(ms) */
        0x7D07,                                                             /* PMON work order */
        TRUE,                                                               /* Ethernet PMON enable/disable in Tx */
        CPUIF_INT_DEVICE_PIN,                                               /* interrupt process type */
        FALSE,                                                              /* static SerDes configuration */
        NULL,                                                               /* FW path and file name */
        10,                                                                 /* number of line serdes port */
        { LINE, LINE, LINE, LINE, LINE, LINE, LINE, LINE, LINE, LINE, NIU, NIU },   /* serdes type */
        { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },     /* line cage id */
        {                                                                   /* line tx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* line rx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },         
        { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },     /* line port LOS pin */
        FALSE,                                                              /* line port LOS inverse status */
        1,                                                                  /* number of system serdes port */
        { 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, 0xa, NIU, NIU },     /* system cage id */
        {                                                                   /* system tx port lane information */
            { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* system rx port lane information */
            { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },                                             
        { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* system port LOS pin */
        FALSE,                                                              /* system port LOS inverse status */
        ILKN_SIFD_NONE,                                                        /* Interlaken SIFD type */
    },

    {
        HW_CARD_10x10G_CLIENT_MUXPONDER_TO_OTU4,                            /* device id */
        DIGI_PROD_APP_TRANSMUXPONDER_CARD,                                  /* card type */
        "digi",                                                             /* file name for data storage */
#ifdef PMC_CUSTOMER_OS
        DIGI_WARM_CTXT_IN_PLACE,                                            /* warm restart type */
#else
        DIGI_WARM_CTXT_COPY,                                                /* warm restart type */
#endif
        256,                                                                /* packet size in OPSA */
        4,                                                                  /* head size in OPSA */
        DIGI_OTN_MUX_STAGES_TWO,                                            /* maximum multiplex stages */
        DIGI_TRIGGER_MANUAL,                                                /* PMON trigger mode */
        0x7,                                                                /* manual trigger mask */
        1000,                                                               /* trigger period: 250 ~ 1000(ms) */
        0x7D07,                                                             /* PMON work order */
        TRUE,                                                               /* Ethernet PMON enable/disable in Tx */
        CPUIF_INT_DEVICE_PIN,                                               /* interrupt process type */
        FALSE,                                                              /* static SerDes configuration */
        NULL,                                                               /* FW path and file name */
        1,                                                                  /* number of line serdes port */
        { LINE, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },    /* serdes type */
        { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, NIU, NIU },     /* line cage id */
        {                                                                   /* line tx port lane information */
            { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* line rx port lane information */
            { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },    
        { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* line port LOS pin */
        FALSE,                                                              /* line port LOS inverse status */
        10,
        { 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, NIU, NIU },     /* system cage id */
        {                                                                   /* system tx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* system rx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },         
        { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },     /* system port LOS pin */
        FALSE,                                                              /* system port LOS inverse status */
        ILKN_SIFD_NONE,                                                     /* Interlaken SIFD type */
    },

    {
        HW_CARD_10x10G_TRANSPONDER,                                         /* device id */
        DIGI_PROD_APP_TRANSMUXPONDER_CARD,                                  /* card type */
        "digi",                                                             /* file name for data storage */
#ifdef PMC_CUSTOMER_OS
        DIGI_WARM_CTXT_IN_PLACE,                                            /* warm restart type */
#else
        DIGI_WARM_CTXT_COPY,                                                /* warm restart type */
#endif
        256,                                                                /* packet size in OPSA */
        4,                                                                  /* head size in OPSA */
        DIGI_OTN_MUX_STAGES_ONE,                                            /* maximum multiplex stages */
        DIGI_TRIGGER_MANUAL,                                                /* PMON trigger mode */
        0x7,                                                                /* manual trigger mask */
        1000,                                                               /* trigger period: 250 ~ 1000(ms) */
        0x7D07,                                                             /* PMON work order */
        TRUE,                                                               /* Ethernet PMON enable/disable in Tx */
        CPUIF_INT_DEVICE_PIN,                                               /* interrupt process type */
        FALSE,                                                              /* static SerDes configuration */
        NULL,                                                               /* FW path and file name */
        10,                                                                 /* number of line serdes port */
        { LINE, LINE, LINE, LINE, LINE, LINE, LINE, LINE, LINE, LINE, NIU, NIU },   /* serdes type */
        { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },     /* line cage id */
        {                                                                   /* line tx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* line rx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },    
        { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },     /* line port LOS pin */
        FALSE,                                                              /* line port LOS inverse status */
        10,
        { 0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0x10, 0x11, 0x12, 0x13, NIU, NIU }, /* system cage id */
        {                                                                   /* system tx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* system rx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },         
        { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },     /* system port LOS pin */
        FALSE,                                                              /* system port LOS inverse status */
        ILKN_SIFD_NONE,                                                     /* Interlaken SIFD type */
    },

    {
        HW_CARD_3x40G_CFP,                                                  /* device id */
        DIGI_PROD_APP_100G_3_X_40G_CFP_CARD,                                /* card type */
        "digi",                                                             /* file name for data storage */
#ifdef PMC_CUSTOMER_OS
        DIGI_WARM_CTXT_IN_PLACE,                                            /* warm restart type */
#else
        DIGI_WARM_CTXT_COPY,                                                /* warm restart type */
#endif
        256,                                                                /* packet size in OPSA */
        4,                                                                  /* head size in OPSA */
        DIGI_OTN_MUX_STAGES_TWO,                                            /* maximum multiplex stages */
        DIGI_TRIGGER_MANUAL,                                                /* PMON trigger mode */
        0x7,                                                                /* manual trigger mask */
        1000,                                                               /* trigger period: 250 ~ 1000(ms) */
        0x7D07,                                                             /* PMON work order */
        TRUE,                                                               /* Ethernet PMON enable/disable in Tx */
        CPUIF_INT_DEVICE_PIN,                                               /* interrupt process type */
        FALSE,                                                              /* static SerDes configuration */
        NULL,                                                               /* FW path and file name */
        1,                                                                  /* number of line serdes port */
        { LINE, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },    /* serdes type */
        { 0x0, 0x0, 0x0, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* line cage id */
        {                                                                   /* line tx port lane information */
            { 0x0, 0x1, 0x2, 0x3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* line rx port lane information */
            { 0x0, 0x1, 0x2, 0x3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* line port LOS pin */
        FALSE,                                                              /* line port LOS inverse status */
        2,                                                                  /* number of system serdes port */
        { 0x1, 0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x2, NIU, NIU, NIU, NIU },     /* system cage id */
        {                                                                   /* system tx port lane information */
            { 0x0, 0x1, 0x2, 0x3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, 0x1, 0x2, 0x3, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* system rx port lane information */
            { 0x0, 0x1, 0x2, 0x3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, 0x1, 0x2, 0x3, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },                                                
        { 0x0, 0x4, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* system port LOS pin */
        FALSE,                                                              /* system port LOS inverse status */
        ILKN_SIFD_1,                                                        /* Interlaken SIFD type */
        {                                                                   /* Interlaken 1 information */
            { 
                DIGI_ILKN_SERDES_RATE_12G5, 0xFF00,
                {
                    {
                        NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 8, 9, 10, 11, 
                        12, 13, 14, 15, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU
                    }
                },
                {
                    {
                        NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 8, 9, 10, 11, 
                        12, 13, 14, 15, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU
                    }
                },
                DIGI_SIFD_BURST_128, DIGI_SIFD_BURST_MIN_DISABLE, 
                DIGI_SIFD_BURST_SHORT_32, DIGI_SIFD_ILKN_MF_2048, { 0, 1024, 4 },
                TRUE, DIGI_SIFD_XFER_MODE_SEGMENT,
                DIGI_SIFD_ODUKSW_ILKN_HEADER_NONE, TRUE, TRUE,
                DIGI_SIFD_XFER_MODE_SEGMENT, DIGI_SIFD_XFER_MODE_SEGMENT,
                DIGI_SIFD_FC_MODE_NONE, 300,
            },
            { 0, 0xFF00, 0}
        },
        {
            {
            /* ICW/BCW field In-Band Flow Control bits 55:40: */
            /*  55, 54, 53,  52,  51,  50,  49,  48,  47,  46,  45,  44,  43,  42,  41,  40, */
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
            }
        },
        {
            {
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
            }    
        },
    },

    {
        HW_CARD_100G_TRANSPONDER_TO_OTU4,                                   /* device id */
        DIGI_PROD_APP_SYSOTN_CARD,                                          /* card type */
        "digi",                                                             /* file name for data storage */
#ifdef PMC_CUSTOMER_OS
        DIGI_WARM_CTXT_IN_PLACE,                                            /* warm restart type */
#else
        DIGI_WARM_CTXT_COPY,                                                /* warm restart type */
#endif
        256,                                                                /* packet size in OPSA */
        4,                                                                  /* head size in OPSA */
        DIGI_OTN_MUX_STAGES_ONE,                                            /* maximum multiplex stages */
        DIGI_TRIGGER_MANUAL,                                                /* PMON trigger mode */
        0x7,                                                                /* manual trigger mask */
        1000,                                                               /* trigger period: 250 ~ 1000(ms) */
        0x7D07,                                                             /* PMON work order */
        TRUE,                                                               /* Ethernet PMON enable/disable in Tx */
        CPUIF_INT_DEVICE_PIN,                                               /* interrupt process type */
        FALSE,                                                              /* static SerDes configuration */
        NULL,                                                               /* FW path and file name */
        1,                                                                  /* number of line serdes port */
        { LINE, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },    /* serdes type */
        { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, NIU, NIU },     /* line cage id */
        {                                                                   /* line tx port lane information */
            { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* line rx port lane information */
            { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },     /* line port LOS pin */
        FALSE,                                                              /* line port LOS inverse status */
        1,                                                                  /* number of system serdes port */
        { 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, NIU, NIU },     /* system cage id */
        {                                                                   /* system tx port lane information */
            { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* system rx port lane information */
            { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },                                             
        { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* system port LOS pin */
        FALSE,                                                              /* system port LOS inverse status */
        ILKN_SIFD_NONE,                                                     /* Interlaken SIFD type */
    },

    {
        HW_CARD_2x40G_TO_OTU4,                                              /* device id */
        DIGI_PROD_APP_SYSOTN_CARD,                                          /* card type */
        "digi",                                                             /* file name for data storage */
#ifdef PMC_CUSTOMER_OS
        DIGI_WARM_CTXT_IN_PLACE,                                            /* warm restart type */
#else
        DIGI_WARM_CTXT_COPY,                                                /* warm restart type */
#endif
        256,                                                                /* packet size in OPSA */
        4,                                                                  /* head size in OPSA */
        DIGI_OTN_MUX_STAGES_ONE,                                            /* maximum multiplex stages */
        DIGI_TRIGGER_MANUAL,                                                /* PMON trigger mode */
        0x7,                                                                /* manual trigger mask */
        1000,                                                               /* trigger period: 250 ~ 1000(ms) */
        0x7D07,                                                             /* PMON work order */
        TRUE,                                                               /* Ethernet PMON enable/disable in Tx */
        CPUIF_INT_DEVICE_PIN,                                               /* interrupt process type */
        FALSE,                                                              /* static SerDes configuration */
        NULL,                                                               /* FW path and file name */
        2,                                                                  /* number of line serdes port */
        { LINE, LINE, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },   /* serdes type */
        { 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x1, NIU, NIU, NIU, NIU },     /* line cage id */
        {                                                                   /* line tx port lane information */
            { 0x0, 0x1, 0x2, 0x3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, 0x1, 0x2, 0x3, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* line rx port lane information */
            { 0x0, 0x1, 0x2, 0x3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, 0x1, 0x2, 0x3, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },         
        { 0x0, 0x4, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* line port LOS pin */
        FALSE,                                                              /* line port LOS inverse status */
        1,                                                                  /* number of system serdes port */
        { 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, NIU, NIU },     /* system cage id */
        {                                                                   /* system tx port lane information */
            { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* system rx port lane information */
            { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },                                             
        { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* system port LOS pin */
        FALSE,                                                              /* system port LOS inverse status */
        ILKN_SIFD_NONE,                                                     /* Interlaken SIFD type */
    },

    {
        HW_CARD_SFI_2x40G_TO_OTU4,                                          /* device id */
        DIGI_PROD_APP_SYSOTN_CARD,                                          /* card type */
        "digi",                                                             /* file name for data storage */
#ifdef PMC_CUSTOMER_OS
        DIGI_WARM_CTXT_IN_PLACE,                                            /* warm restart type */
#else
        DIGI_WARM_CTXT_COPY,                                                /* warm restart type */
#endif
        256,                                                                /* packet size in OPSA */
        4,                                                                  /* head size in OPSA */
        DIGI_OTN_MUX_STAGES_ONE,                                            /* maximum multiplex stages */
        DIGI_TRIGGER_MANUAL,                                                /* PMON trigger mode */
        0x7,                                                                /* manual trigger mask */
        1000,                                                               /* trigger period: 250 ~ 1000(ms) */
        0x7D07,                                                             /* PMON work order */
        TRUE,                                                               /* Ethernet PMON enable/disable in Tx */
        CPUIF_INT_DEVICE_PIN,                                               /* interrupt process type */
        FALSE,                                                              /* static SerDes configuration */
        NULL,                                                               /* FW path and file name */
        2,                                                                  /* number of line serdes port */
        { SFI51, LINE, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },  /* serdes type */
        { 0x0, 0x1, 0x1, 0x1, 0x1, 0x1, NIU, NIU, NIU, NIU, NIU, NIU },     /* line cage id */
        {                                                                   /* line tx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, 0x1, 0x2, 0x3, 0x4, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* line rx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, 0x1, 0x2, 0x3, 0x4, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },         
        { 0x0, 0x1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* line port LOS pin */
        FALSE,                                                              /* line port LOS inverse status */
        1,                                                                  /* number of system serdes port */
        { 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, NIU },     /* system cage id */
        {                                                                   /* system tx port lane information */
            { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* system rx port lane information */
            { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },                                             
        { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* system port LOS pin */
        FALSE,                                                              /* system port LOS inverse status */
        ILKN_SIFD_NONE,                                                     /* Interlaken SIFD type */
    },

    {
        HW_CARD_12x10G_ANY_OTN_TRIBUTARY,                                   /* card id */
        DIGI_PROD_APP_LINE_CARD,                                            /* card type */
        "digi",                                                             /* file name for data storage */
#ifdef PMC_CUSTOMER_OS
        DIGI_WARM_CTXT_IN_PLACE,                                            /* warm restart type */
#else
        DIGI_WARM_CTXT_COPY,                                                /* warm restart type */
#endif
        256,                                                                /* packet size in OPSA */
        4,                                                                  /* head size in OPSA */
        DIGI_OTN_MUX_STAGES_TWO,                                            /* maximum multiplex stages */
        DIGI_TRIGGER_MANUAL,                                                /* PMON trigger mode */
        0x7,                                                                /* manual trigger mask */
        1000,                                                               /* trigger period: 250 ~ 1000(ms) */
        0x7D07,                                                             /* PMON work order */
        TRUE,                                                               /* Ethernet PMON enable/disable in Tx */
        CPUIF_INT_DEVICE_PIN,                                               /* interrupt process type */
        FALSE,                                                              /* static SerDes configuration */
        NULL,                                                               /* FW path and file name */
        12,                                                                 /* number of line serdes port */
        { LINE, LINE, LINE, LINE, LINE, LINE, LINE, LINE, LINE, LINE, LINE, LINE }, /* serdes type */
        { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb },     /* line cage id */
        {                                                                   /* line tx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0 } 
        },
        {                                                                   /* line rx port lane information */
            { 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0 } 
        },                                                
        { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb },     /* line port LOS pin */
        FALSE,                                                              /* line port LOS inverse status */
        0,                                                                  /* number of system serdes port */
        { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* system cage id */
        {                                                                   /* system tx port lane information */
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* system rx port lane information */
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },                                                
        { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* system port LOS pin */
        FALSE,                                                              /* system port LOS inverse status */
        ILKN_SIFD_1_2,                                                      /* Interlaken SIFD type */
        {                                                                   /* Interlaken 1 information */
            { 
                DIGI_ILKN_SERDES_RATE_10G3125, 0xFFFF,
                {
                    {
                        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
                        13, 14, 15, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU
                    }
                },
                {
                    {
                        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
                        13, 14, 15, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU
                    }
                },
                DIGI_SIFD_BURST_128, DIGI_SIFD_BURST_MIN_DISABLE, 
                DIGI_SIFD_BURST_SHORT_32, DIGI_SIFD_ILKN_MF_2048, { 0, 1024, 4 },
                TRUE, DIGI_SIFD_XFER_MODE_SEGMENT,
                DIGI_SIFD_ODUKSW_ILKN_HEADER_NONE, TRUE, TRUE,
                DIGI_SIFD_XFER_MODE_SEGMENT, DIGI_SIFD_XFER_MODE_SEGMENT,
                DIGI_SIFD_FC_MODE_NONE, 300,
            },
            { 0, 0xFFFF, 0}
        },
        {
            {
            /* ICW/BCW field In-Band Flow Control bits 55:40: */
            /*  55, 54, 53,  52,  51,  50,  49,  48,  47,  46,  45,  44,  43,  42,  41,  40, */
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
            }
        },
        {
            {
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
            }
        },
        {                                                                   /* Interlaken 2 information for T8 Serdes */
            { 
                DIGI_ILKN_SERDES_RATE_6G25, 0xFFFFFF,
                {
                    {
                        0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11,
                        12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23
                    }
                },
                {
                    {
                        0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11,
                        12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23
                    }
                },
                DIGI_SIFD_BURST_128, DIGI_SIFD_BURST_MIN_DISABLE, 
                DIGI_SIFD_BURST_SHORT_32, DIGI_SIFD_ILKN_MF_2048, { 0, 1024, 4 },
                TRUE, DIGI_SIFD_XFER_MODE_SEGMENT,
                DIGI_SIFD_ODUKSW_ILKN_HEADER_NONE, TRUE, FALSE,
                DIGI_SIFD_XFER_MODE_SEGMENT, DIGI_SIFD_XFER_MODE_SEGMENT,
                DIGI_SIFD_FC_MODE_NONE, 300,
            },
            { 0xFFFFFF, 0, 0}
        },
        {
            {
                /* ICW/BCW field In-Band Flow Control bits 55:40: */
                /*  55, 54, 53,  52,  51,  50,  49,  48,  47,  46,  45,  44,  43,  42,  41,  40, */
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
            }
        },
        {
            {
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
            }
        }
    },

    {
        HW_CARD_3x40G_OTN_TRIBUTARY,                                        /* device id */
        DIGI_PROD_APP_LINE_CARD,                                            /* card type */
        "digi",                                                             /* file name for data storage */
#ifdef PMC_CUSTOMER_OS
        DIGI_WARM_CTXT_IN_PLACE,                                            /* warm restart type */
#else
        DIGI_WARM_CTXT_COPY,                                                /* warm restart type */
#endif
        256,                                                                /* packet size in OPSA */
        4,                                                                  /* head size in OPSA */
        DIGI_OTN_MUX_STAGES_TWO,                                            /* maximum multiplex stages */
        DIGI_TRIGGER_MANUAL,                                                /* PMON trigger mode */
        0x7,                                                                /* manual trigger mask */
        1000,                                                               /* trigger period: 250 ~ 1000(ms) */
        0x7D07,                                                             /* PMON work order */
        TRUE,                                                               /* Ethernet PMON enable/disable in Tx */
        CPUIF_INT_DEVICE_PIN,                                               /* interrupt process type */
        FALSE,                                                              /* static SerDes configuration */
        NULL,                                                               /* FW path and file name */
        3,                                                                  /* number of line serdes port */
        { LINE, LINE, LINE, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },  /* serdes type */
        { 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x2 },     /* line cage id */
        {                                                                   /* line tx port lane information */
            { 0x0, 0x1, 0x2, 0x3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, 0x1, 0x2, 0x3, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, 0x1, 0x2, 0x3 },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* line rx port lane information */
            { 0x0, 0x1, 0x2, 0x3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, 0x0, 0x1, 0x2, 0x3, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 0x0, 0x1, 0x2, 0x3 },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        { 0x0, 0x4, 0x8, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* line port LOS pin */
        FALSE,                                                              /* line port LOS inverse status */
        0,                                                                  /* number of system serdes port */
        { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* system cage id */
        {                                                                   /* system tx port lane information */
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },
        {                                                                   /* system rx port lane information */
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },
            { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU } 
        },                                                
        { NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU },     /* system port LOS pin */
        FALSE,                                                              /* system port LOS inverse status */
        ILKN_SIFD_1,                                                        /* Interlaken SIFD type */
        {                                                                   /* Interlaken 1 information */
            {
                DIGI_ILKN_SERDES_RATE_10G3125, 0xFFFF,
                {
                    {
                        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
                        13, 14, 15, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU
                    }
                },
                {
                    {
                        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
                        13, 14, 15, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU
                    }
                },
                DIGI_SIFD_BURST_128, DIGI_SIFD_BURST_MIN_DISABLE, 
                DIGI_SIFD_BURST_SHORT_32, DIGI_SIFD_ILKN_MF_2048, { 0, 1024, 4 },
                TRUE, DIGI_SIFD_XFER_MODE_SEGMENT,
                DIGI_SIFD_ODUKSW_ILKN_HEADER_NONE, TRUE, TRUE,
                DIGI_SIFD_XFER_MODE_SEGMENT, DIGI_SIFD_XFER_MODE_SEGMENT,
                DIGI_SIFD_FC_MODE_NONE, 300,
            },
            { 0, 0xFFFF, 0}
        },
        {
            {
            /* ICW/BCW field In-Band Flow Control bits 55:40: */
            /*  55, 54, 53,  52,  51,  50,  49,  48,  47,  46,  45,  44,  43,  42,  41,  40, */
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
            }
        },
        {
            {
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
                LINK, 3, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 0, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 1, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, 
                LINK, 2, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU, NIU,
            }
        },
    },
};

PRIVATE UINT32 g_dev_num = sizeof(g_dev_info) / sizeof(g_dev_info[0]);

/*
** Public Functions
*/
/*******************************************************************************
* adpt_dev_info_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve specific card infomation according to different device application.
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   **dev_info_ptr     - pointer to card handle instance, which includes each 
*                        digi device information.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dev_info_get(UINT32 dev_id,
                                digi_dev_info_t **dev_info_ptr)
{
    UINT32 id = 0;
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    for (id = 0; id < g_dev_num; id++)
    {
        if (DIGI_CARD_ID_GET(dev_id) == g_dev_info[id].card_id)
        {
            *dev_info_ptr = &g_dev_info[id];
            break;
        }
    }

    /* can't find a proper card information */
    if (id >= g_dev_num)
    {
        rc = PMC_ERR_FAIL;
    }
    
    PMC_RETURN(rc);
} /* adpt_dev_info_get */


/*******************************************************************************
* adpt_handle_allocate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Allocate adpt_handle resource.
*
* INPUTS:
*   dev_id             - device identity.
*   data_size          - data size.
*
* OUTPUTS:
*   **data_ptr         - Adapter/DIGI handle.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_handle_allocate(UINT32 dev_id, UINT32 data_size, void **data_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    *data_ptr = (void*) PMC_CALLOC(data_size);

    PMC_RETURN(rc);
} /* adpt_handle_allocate */


/*******************************************************************************
* adpt_handle_free
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Free adpt_handle resource.
*
* INPUTS:
*   dev_id             - device identity.
*   **data_ptr         - Adapter/DIGI handle.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_handle_free(UINT32 dev_id, void **data_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_FREE(data_ptr);

    PMC_RETURN(rc);
} /* adpt_handle_free */


/*******************************************************************************
* adpt_database_save
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Save database to the flash or non volatile memory.
*
* INPUTS:
*   *file_name         - file name.
*   data_size          - data size.
*   *data_ptr          - pointer to data.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_database_save(char *file_name, UINT32 data_size, void *data_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
#ifdef PMC_COMEXPRESS
    int fd = 0;
    struct stat buf;
    PMC_ENTRY();

    if ((fd = open((const char *)file_name, O_RDWR | O_CREAT | O_SYNC, 0777)) < 0)
    {
        ADPT_ERROR("The file [%s] can't be opened: %s. \r\n", file_name, strerror(errno));
        PMC_RETURN(PMC_ERR_FAIL);
    }

    if (fstat(fd, &buf) < 0) 
    {
        ADPT_ERROR("The file [%s] is failed: %s. \r\n", file_name, strerror(errno));
        rc = PMC_ERR_FAIL;
        goto EXIT;
    }

    if ((UINT32)buf.st_size != data_size)
    {
        if (ftruncate(fd, data_size) != 0) 
        {
            ADPT_ERROR("The file [%s] is failed: %s. \r\n", file_name, strerror(errno));
            rc = PMC_ERR_FAIL;
            goto EXIT;
        }
    }

    if (lseek(fd, 0, SEEK_SET) < 0) 
    {
        ADPT_ERROR("The file [%s] is failed: %s. \r\n", file_name, strerror(errno));
        rc = PMC_ERR_FAIL;
        goto EXIT;
    }

    if (write(fd, data_ptr, (ssize_t)data_size) != (ssize_t)(data_size)) 
    {
        ADPT_ERROR("Fail to write file [%s]: %s. \r\n", file_name, strerror(errno));
        rc = PMC_ERR_FAIL;
    }

EXIT:
    close(fd);
#else
    PMC_ENTRY();
#endif

    PMC_RETURN(rc);
} /* adpt_database_save */


/*******************************************************************************
* adpt_database_restore
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Restore database from the flash or non volatile memory.
*
* INPUTS:
*   *file_name         - file name.
*   data_size          - data size.
*
* OUTPUTS:
*   **data_ptr         - pointer to data.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_database_restore(char *file_name, UINT32 data_size, void *data_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
#ifdef PMC_COMEXPRESS
    int fd = 0;
    struct stat buf;
    void *tmp_ptr = NULL;
    PMC_ENTRY();

    if ((fd = open((const char *)file_name, O_RDWR | O_SYNC, 0777)) < 0)
    {
        ADPT_ERROR("The file [%s] can't be opened: %s. \r\n", file_name, strerror(errno));
        PMC_RETURN(PMC_ERR_FAIL);
    }

    if (fstat(fd, &buf) < 0) 
    {
        ADPT_ERROR("The file [%s] is failed: %s. \r\n", file_name, strerror(errno));
        rc = PMC_ERR_FAIL;
        goto EXIT;
    }

    if ((UINT32)buf.st_size != data_size)
    {
        ADPT_ERROR("st_size[%d] != data_size[%d]. \r\n", (UINT32)buf.st_size, data_size);
        rc = PMC_ERR_FAIL;
        goto EXIT;
    }

    if (lseek(fd, 0, SEEK_SET) < 0) 
    {
        ADPT_ERROR("The file [%s] is failed: %s. \r\n", file_name, strerror(errno));
        rc = PMC_ERR_FAIL;
        goto EXIT;
    }

    /* allocate temporary data */
    tmp_ptr = (void*)PMC_CALLOC(data_size);
    /* read the context */
    if (read(fd, tmp_ptr, (ssize_t)data_size) < 0) 
    {
        ADPT_ERROR("Fail to read file [%s]: %s. \r\n", file_name, strerror(errno));
        rc = PMC_ERR_FAIL;
    }
    /* copy data from temporary memory to target */
    PMC_MEMCPY((UINT8*)data_ptr, (UINT8*)tmp_ptr, data_size);
    /* free temporary data */
    PMC_FREE(&tmp_ptr);

EXIT:
    close(fd);
#else
    PMC_ENTRY();
#endif

    PMC_RETURN(rc);
} /* adpt_database_restore */


/*******************************************************************************
* adpt_opsa_pkt_size_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve OPSA packet size and header size according to ODUk stream.
*
* INPUTS:
*   dev_id             - device identity.
*   oduk_stream        - ODUk stream type.
*
* OUTPUTS:
*   *pkt_size          - packet size.
*   *header_size       - header size.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_opsa_pkt_size_get(UINT32 dev_id,
                                     digi_oduksw_stream_t oduk_stream,
                                     UINT32 *pkt_size,
                                     UINT32 *header_size)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(pkt_size);
    ADPT_PARA_VERIFY(header_size);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* default value */
    *pkt_size = adpt_handle->dev_info->pkt_size;
    *header_size = adpt_handle->dev_info->header_size;

    /* modification by customers requirement */
    switch (oduk_stream)
    {
        case DIGI_ODUKSW_STREAM_CBR_ODU0:
            break;

        case DIGI_ODUKSW_STREAM_CBR_ODU1:
            break;

        case DIGI_ODUKSW_STREAM_CBR_ODU2:
        case DIGI_ODUKSW_STREAM_CBR_ODU2E:
        case DIGI_ODUKSW_STREAM_CBR_ODU2F:
        case DIGI_ODUKSW_STREAM_CBR_ODU1E:
        case DIGI_ODUKSW_STREAM_CBR_ODU1F:
            break;

        case DIGI_ODUKSW_STREAM_CBR_ODU3:
        case DIGI_ODUKSW_STREAM_CBR_ODU3E2:
        case DIGI_ODUKSW_STREAM_CBR_ODU3E1:
            break;

        case DIGI_ODUKSW_STREAM_CBR_ODU4:
            break;

        case DIGI_ODUKSW_STREAM_CBR_ODUFLEX:
        case DIGI_ODUKSW_STREAM_PKT_GFPF_ODUFLEX:
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_opsa_pkt_size_get */


/*******************************************************************************
* adpt_opsa_oduksw_fifo_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve OPSA FIFO and latency according to ODUk stream.
*
* INPUTS:
*   dev_id             - device identity.
*   src_chnl_id        - source channel identity.
*   oduk_stream        - ODUk stream type.
*   ts_number          - tributary slot number used for ODUflex(GFP/CBR).
*
* OUTPUTS:
*   *halt_buffer       - ODUKSW FIFO buffer room to capture in flight data in
*                        the event that ODUKSW FIFO generates an upstream flow
*                        control signal.  Used in cases where ENET utilizes
*                        pause flow control and where a channel is fed from
*                        SIFD and asserts XOFF flow control.  halt_buffer is
*                        specified in units of blocks.
*                        The block size in bytes per unit is: 768 bytes.
*                        Applicable to client_data containing "DIGI_ODUKSW_STREAM_PKT_"
*   *latency_s         - Maximum system latency outside the DIGI device.  
*                        Specified in units of 3.125 ns ticks.  The maximum 
*                        supported value is 38880 representing a maximum system 
*                        latency of 125 useconds.  This parameter is used to 
*                        configure the MPMO hold off delay following MPMOs
*                        reception of the first packet. Otherwise, set to 0.
*                        *latency_s = us x 311.04Mbps
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_opsa_oduksw_fifo_get(UINT32 dev_id,
                                        UINT32 src_chnl_id,
                                        digi_oduksw_stream_t oduk_stream,
                                        UINT32 ts_number,
                                        UINT32 *halt_buffer,
                                        UINT32 *latency_s)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 src_traffic_type = 0;
    UINT32 latency_us = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(halt_buffer);
    ADPT_PARA_VERIFY(latency_s);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc |= adpt_chnl_index_get(adpt_handle, src_chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get traffic type */
    src_traffic_type = DIGI_TRAFFIC_TYPE_GET(adpt_handle->traffic_info[index].traffic_mode);
    if (src_traffic_type >= DIGI_TYPE_MAX)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* default value */
    *halt_buffer = 0;

    /* source cross connection point is from ILKN */
    if ((DIGI_TYPE_INLK == src_traffic_type)
        && (NULL == adpt_handle->traffic_info[index].map_handle))
    {
        /* modification by customers requirement */
        /* latency_us: microsecond*/
        switch (oduk_stream)
        {
            case DIGI_ODUKSW_STREAM_CBR_ODU0:
                break;

            case DIGI_ODUKSW_STREAM_CBR_ODU1:
                break;

            case DIGI_ODUKSW_STREAM_CBR_ODU2:
            case DIGI_ODUKSW_STREAM_CBR_ODU2E:
            case DIGI_ODUKSW_STREAM_CBR_ODU2F:
            case DIGI_ODUKSW_STREAM_CBR_ODU1E:
            case DIGI_ODUKSW_STREAM_CBR_ODU1F:
                break;

            case DIGI_ODUKSW_STREAM_CBR_ODU3:
            case DIGI_ODUKSW_STREAM_CBR_ODU3E2:
            case DIGI_ODUKSW_STREAM_CBR_ODU3E1:
                break;

            case DIGI_ODUKSW_STREAM_CBR_ODU4:
                break;

            case DIGI_ODUKSW_STREAM_CBR_ODUFLEX:
            case DIGI_ODUKSW_STREAM_PKT_GFPF_ODUFLEX:
                break;

            default:
                rc = PMC_ERR_INVALID_PARAMETERS;
                break;
        }
    }
    /* source cross connection point is from COREOTN/MAPOTN */
    else
    {
        /* modification by customers requirement */
    }

    /* parameter conversion */
    *latency_s = latency_us * 311;
    if (38880 < (*latency_s))
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_opsa_oduksw_fifo_get */


/*******************************************************************************
* adpt_cpb_fifo_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves CPB buffers. 
*
* INPUTS:
*   cpb_stream         - CPB stream.
*   source             - CPB switch source.
*   ts_number          - tributary slot number used for ODUflex(GFP/CBR).
*
* OUTPUTS:
*   *halt_buffer_ptr   - pointer to halt buffer block. The block size in 
*                        bytes per unit is: 12288 bytes
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cpb_fifo_get(digi_cpb_stream_t cpb_stream,
                                digi_cpb_src_t source,
                                UINT32 ts_number,
                                UINT32 *halt_buffer_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(halt_buffer_ptr);

    /* source cross connection point is from ILKN */
    if (DIGI_CPB_SRC_ILKN == source)
    {
        switch (cpb_stream)
        {
            case DIGI_CPB_STREAM_1G25:
                *halt_buffer_ptr = 0;
                break;

            case DIGI_CPB_STREAM_2G5:
                *halt_buffer_ptr = 0;
                break;

            case DIGI_CPB_STREAM_10G:
                *halt_buffer_ptr = 0;
                break;

            case DIGI_CPB_STREAM_40G:
                *halt_buffer_ptr = 0;
                break;

            case DIGI_CPB_STREAM_FLEX:
                *halt_buffer_ptr = 0;
                break;

            case DIGI_CPB_STREAM_100G:
                *halt_buffer_ptr = 2;
                break;

            default:
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                break;
        }
    }
    /* source cross connection point is from ENET/CBR/MAPOTN */
    else
    {
    }

    PMC_RETURN(rc);
} /* adpt_cpb_fifo_get */


/*******************************************************************************
* adpt_serdes_csv_name_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the file name of static SerDes configuration. 
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   The file name string.
*
* NOTES:
*
*******************************************************************************/
PUBLIC const char* adpt_serdes_csv_name_get(UINT32 dev_id)
{
    return "serdes.csv";
} /* adpt_serdes_csv_name_get */


/*******************************************************************************
* adpt_serdes_port_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the Tx SERDES port for Tx ENET PMON according to the Rx SERDES port.
*   This API is only available for the DIGI60_PROD_APP_TRANSMUXPONDER_CARD 
*   application.
*
* INPUTS:
*   dev_id             - device identity.
*   rx_serdes_port     - Rx SERDES port.
*
* OUTPUTS:
*   *tx_serdes_port    - Tx SERDES port.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_port_get(UINT32 dev_id, UINT32 rx_serdes_port, UINT32 *tx_serdes_port)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(tx_serdes_port);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* default value */
    *tx_serdes_port = (rx_serdes_port + DIGI_SERDES_XFI_LANES_MAX / 2) % DIGI_SERDES_XFI_LANES_MAX;

    PMC_RETURN(rc);
} /* adpt_serdes_port_get */


/*******************************************************************************
* adpt_defect_opt_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the option of defect and interrupt optimization to save time when 
*   calling the adpt_defect_trigger(). Once it needs to be optimized, only those 
*   necessary defects and interrupts are retrived. And those debug purposed defects
*   interrupts will not be updated accordingly.
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *opt_en            - TRUE: Those debug purposed defects and interrupts will
*                        not be retrieved through the adpt_defect_trigger().
*                        FALSE: All defects and interrupts will be retrieved.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_defect_opt_en_get(UINT32 dev_id, BOOL *opt_en)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(opt_en);

    *opt_en = FALSE;

    PMC_RETURN(rc);
} /* adpt_defect_opt_en_get */


/*******************************************************************************
* adpt_fw_filename_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function can be optionally called to specify the application firmware 
*   path and file name before device initialization by calling the adpt_init(). 
*   Otherwise, the default name defined as DIGI_FW_DOWNLOAD_DEFAULT_FILE_NAME or
*   DIGI_FW_DOWNLOAD_REVB_FILE_NAME would be used. 
*
* INPUTS:
*   dev_id             - device identity.
*   *file_name         - firmware path and file name.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   Please note that the fw_filename argument in the g_dev_info[] table should be 
*   configured as NULL by default if this API is called. Otherwise, this API 
*   shouldn't be called for this dev_id. The fw_filename path in the g_dev_info[] 
*   table will be configured accordingly.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fw_filename_set(UINT32 dev_id, char const *file_name)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_dev_info_t *dev_info = NULL;
    UINT32 name_len = 0;
    PMC_ENTRY();

    /* get card information */
    rc = adpt_dev_info_get(dev_id, &dev_info);
    ADPT_RETURN_VERIFY(rc);

    if (NULL != dev_info->fw_filename) 
    {
        PMC_FREE(&dev_info->fw_filename);
    }

    name_len = PMC_STRLEN(file_name);

    dev_info->fw_filename = (char*)PMC_CALLOC(sizeof(char) * (name_len + 1));

    PMC_MEMCPY(dev_info->fw_filename, file_name, name_len);

    PMC_RETURN(rc);
} /* adpt_fw_filename_set */


/*
** End of file
*/
