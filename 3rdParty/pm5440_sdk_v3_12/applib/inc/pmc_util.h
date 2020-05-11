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
*   DESCRIPTION :
*     This file provides utility functions for examples operation etc.
*     Currently it contains the operations that would typicially be in a
*     C file, but by being in a header file it can be directly included
*     keeping the example compact.
*
*   NOTES: None.
*
*******************************************************************************/

/* ----- Includes ------------------------------------------------------------*/


/* ----- Global Variables-----------------------------------------------------*/
int main_argc; /*!< Set this value to the total argument count. Remember to include the command name in the count */
const char **main_argv_pptr; /*!< Set this array of char pointers to point to argv. Remember the first entry is the command line name */
BOOL8 main_prompts_on; /*!< Set this value to TRUE if prompting is on. False if running from argc/argv */

/*******************************************************************************
* input_is_yes()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This fucntion either reads in a character or reads the next character from
*   the global main_argv_pptr depending on whether main_prompts_on (global) is
*   set to TRUE or FALSE. This fucntion returns TRUE if it is a Y/y, FALSE
*   if N/n and halts execution otherwise.
* 
*   Since it tracks the location within the argc list it can only read the next
*   item in the list when in non-prompting mode.
*   
*   Globals used: main_argv_pptr, main_argc, main_prompts_on
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE - Y/y entered or found in argv
*   FALSE - N/n entered or found in argv
*
* NOTES:
*
*******************************************************************************/
BOOL8 input_is_yes()
{
    static UINT32 argc_count = 1; /* Set initial point where parameters begin to command line */
    BOOL8 rc;
    char c;

    /* Check if the global variable that controls whether prompting is being used */
    if (TRUE == main_prompts_on) 
    {
        /* Prompts are on so get a character - question should have been output
           by calling function */
        do 
        {
            c = getchar();
        } while(c == '\n');
    }
    else
    {
        /* No prompts so get input from global pointer to argc/argv */
        c = *main_argv_pptr[argc_count];
        argc_count++;
        PMC_LOG_NOTRACE("%c", c);
    }

    /* Check the inputs and set output */
    switch (c)
    {
        case 'Y':
        case 'y':
            rc = TRUE;
        break;
        case 'N':
        case 'n':
            rc = FALSE;
        break;
        default:
            /* Unexpected input so exit */
            PMC_LOG_NOTRACE("ERROR - example expected Y/y/N/n for parameter %d\n", argc_count - 2);
            exit(PMC_ERR_INVALID_PARAMETERS);
        break;
    }

    return(rc);
} /* input_is_yes() */


