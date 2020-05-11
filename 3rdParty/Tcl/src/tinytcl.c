/* 
 * tinytcl.c --
 *
 *	Test driver for TCL.
 *
 * Copyright 1987-1991 Regents of the University of California
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appears in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 * $Id: tinytcl.c,v 1.1.1.1 2001/04/20 15:03:05 karl Exp $
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>

#include "tclInt.h"
#include "tclExtdInt.h"
#include "linenoise.h"

extern void TclX_InitGeneral (Tcl_Interp *interp);
extern void TclX_InitFmath (Tcl_Interp *interp);
extern void TclX_InitUnix (Tcl_Interp *interp);
extern void Tcl_InitReaddir (Tcl_Interp *interp);

/* The "magic" hook into user application. You must */
/* provide this function to register your own commands. */
extern void AppTclInit (Tcl_Interp *interp);

Tcl_Interp *interp;
Tcl_CmdBuf buffer;
char dumpFile[100];
int quitFlag = 0;

char initCmd[] = "";

/* Default prompt */
char defaultprompt[] = "%";

/* Command line history */
#define CMD_HIST_LEN 100
char gCmdline[CMD_HIST_LEN][1000];
int gCmdhistPtr = 0;

#define ASCII_BACKSPACE 8
#define ASCII_SPACE 32
#define ASCII_ESCAPE 0x1B
#define ASCII_TILDE 126
#define ASCII_DELETE 127

#define ESCAPE_KEYCODE_UPARROW 0x5B41
#define ESCAPE_KEYCODE_DOWNARROW 0x5B42
#define ESCAPE_KEYCODE_LEFTARROW 0x5B44

int
cmdHelpBasic(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    (void) clientData;
    (void) argc;
    (void) argv;
    register Interp *iPtr = (Interp *) interp;
    Tcl_HashEntry *hPtr;
    Tcl_HashSearch search;

    printf ("See /vobs/ots/OngCoreServices/tcl/Tcl.pdf for command reference.\n\n");

    for (hPtr = Tcl_FirstHashEntry(&iPtr->commandTable, &search);
         hPtr != NULL; hPtr = Tcl_NextHashEntry(&search)) {
      char *name = Tcl_GetHashKey(&iPtr->commandTable, hPtr);
      Tcl_AppendElement(interp, name, 0);
    }

    return TCL_OK;
}

int
cmdHistory(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    (void) clientData;
    (void) argc;
    (void) argv;
    (void) interp;
    int cmdhistIter = gCmdhistPtr;

    if (cmdhistIter == (CMD_HIST_LEN-1))
    {
        cmdhistIter = 0;
    }
    else
    {
        cmdhistIter++;
    }
    while (cmdhistIter != gCmdhistPtr)
    {
        if (gCmdline[cmdhistIter][0] != 0)
        {
            printf("%s\n", gCmdline[cmdhistIter]);
            fflush(stdout);
        }
        if (cmdhistIter == (CMD_HIST_LEN-1))
        {
            cmdhistIter = 0;
        }
        else
        {
            cmdhistIter++;
        }
    }

    return TCL_OK;
}

	/* ARGSUSED */
int
cmdCheckmem(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    (void) clientData;
    if (argc != 2) {
	Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
		" fileName\"", (char *) NULL);
	return TCL_ERROR;
    }
    strcpy(dumpFile, argv[1]);
    quitFlag = 1;
    return TCL_OK;
}

	/* ARGSUSED */
int
cmdEcho(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    int i;
    (void) clientData;

    for (i = 1; ; i++) {
	if (argv[i] == NULL) {
	    if (i != argc) {
		echoError:
		sprintf(interp->result,
		    "argument list wasn't properly NULL-terminated in \"%s\" command",
		    argv[0]);
	    }
	    break;
	}
	if (i >= argc) {
	    goto echoError;
	}
	fputs(argv[i], stdout);
	if (i < (argc-1)) {
	    printf(" ");
	}
    }
    printf("\n");
    return TCL_OK;
}

/**/
/* fgets() reads in at most one less than size characters from stream and stores  */
/* them into the buffer pointed to by s. Reading stops after an EOF or a newline. */
/* If a newline is read, it is stored into the buffer. A terminating null byte    */
/* (aq\0aq) is stored after the last character in the buffer.                     */
/**/
char *my_fgets(char *s, int size, FILE *stream)
{
    int c = 0;
    int escape_seq = 0;
    int escape_chars = 0;
    int cmdhistIter = gCmdhistPtr;
    int cnt = 0;

    while (cnt < (size-1))
    {
        s[cnt] = '\0';
        c = fgetc(stream);
    
        /* EOF / error */
        if (c < 0)
        {
            return(NULL);
        }
        
        if (c == ASCII_ESCAPE)
        {
            /* Arm to capture next 2 chars */
            escape_seq = 2;
            continue;
        }

        if (c == '\n')
        {
            putchar(c);
            fflush(stdout);
            escape_seq = 0; /* stop the escape seq */
            escape_chars = 0;
            break;
        }

        if (escape_seq != 0)
        {
            /* Handle delete, up/down/left/right */
            escape_chars = ((escape_chars << 8) | ((unsigned int) c));
            escape_seq--;
            if (escape_seq == 0)
            {
                
                switch (escape_chars)
                {
                    case ESCAPE_KEYCODE_UPARROW :
                        /* <UP ARROW> Replace current line with previous command in history. */
                        /* Erase the current line. */
                        while (cnt > 0)
                        {
                            putchar(ASCII_BACKSPACE); /* backspace */
                            putchar(ASCII_SPACE); /* space */
                            putchar(ASCII_BACKSPACE); /* backspace */
                            fflush(stdout);
                            cnt--;
                        }

                        /* Copy the previous line from the command history to the current line, and to stdout */
                        if (cmdhistIter == 0)
                        {
                            cmdhistIter = (CMD_HIST_LEN-1);
                        }
                        else
                        {
                            cmdhistIter--;
                        }
                        s[cnt] = 0;
                        while ((gCmdline[cmdhistIter][cnt] != 0) && (cnt < (size-1)))
                        {
                            s[cnt] = gCmdline[cmdhistIter][cnt];
                            putchar(gCmdline[cmdhistIter][cnt]);
                            fflush(stdout);
                            cnt++;
                        }

                        break;
                    case ESCAPE_KEYCODE_DOWNARROW :
                        /* <DOWN ARROW> Replace current line with next command in history. */
                        /* Erase the current line. */
                        while (cnt > 0)
                        {
                            putchar(ASCII_BACKSPACE); /* backspace */
                            putchar(ASCII_SPACE); /* space */
                            putchar(ASCII_BACKSPACE); /* backspace */
                            fflush(stdout);
                            cnt--;
                        }

                        /* Copy the next line from the command history to the current line, and to stdout */
                        if (cmdhistIter == (CMD_HIST_LEN-1))
                        {
                            cmdhistIter = 0;
                        }
                        else
                        {
                            cmdhistIter++;
                        }
                        s[cnt] = 0;
                        while ((gCmdline[cmdhistIter][cnt] != 0) && (cnt < (size-1)))
                        {
                            s[cnt] = gCmdline[cmdhistIter][cnt];
                            putchar(gCmdline[cmdhistIter][cnt]);
                            fflush(stdout);
                            cnt++;
                        }
                        break;
                    case ESCAPE_KEYCODE_LEFTARROW :
                        putchar(ASCII_BACKSPACE); /* left arrow is backspace */
                        putchar(ASCII_SPACE); /* left arrow is backspace */
                        putchar(ASCII_BACKSPACE); /* left arrow is backspace */
                        fflush(stdout);
                        cnt--;
                        break;
                    default : 
                        break;
                        /* printf ("0x%04X", ((unsigned int) c)); */
                }
                fflush(stdout);

                escape_chars = 0;
            }
        }
        else
        {
            if ((c >= ASCII_SPACE) && (c <= ASCII_TILDE))
            {
                /* Store printable characters in buffer and keep reading */
                putchar(c);
                fflush(stdout);
                s[cnt] = ((char) c);
                cnt++;
            }
            else if ((c == ASCII_BACKSPACE) || (c == ASCII_DELETE))
            {
                /* backspace */
                if (cnt > 0)
                {
                    putchar(ASCII_BACKSPACE); /* backspace */
                    putchar(ASCII_SPACE); /* space */
                    putchar(ASCII_BACKSPACE); /* backspace */
                    fflush(stdout);
                    cnt--;
                }
            }
            else
            {
                /* non-printable, non-special chars are ignored */
                /* printf ("0x%04X", ((unsigned int) c)); */
                /* fflush(stdout); */
            }
        }
    }

    /* Add null termination because its fun to do. */
    s[cnt+1] = '\0';
    return(s);
}

int
tcl_main()
{
    char eoln[] = "\n";
    char *cmd;
    int result, gotPartial;
    struct termios termio;
    struct termios termio_saved;
    char *tempStr;
    char promptString[64];

    // Terminal - make sure backspace works.
    tcgetattr(fileno(stdin), &termio);
    if (!strcmp(getenv("gAppDebugMode"), "true"))
    {
        termio.c_lflag |= (ISIG);  /* generate signal to App */
    }
    else
    {
        termio.c_lflag &= ~(ISIG); /* Impervious to Ctrl-C */
    }
    termio.c_cc[VERASE] = ASCII_BACKSPACE; /* Backspace instead of DEL */
    tcsetattr(fileno(stdin), TCSANOW, &termio);

    interp = Tcl_CreateInterp();
#ifdef TCL_MEM_DEBUG
    Tcl_InitMemory(interp);
#endif
    Tcl_InitDebug (interp);
    TclX_InitGeneral (interp);
    TclX_InitFmath (interp);
    TclX_InitUnix (interp);
    Tcl_InitReaddir (interp);


    Tcl_CreateCommand(interp, "echo", cmdEcho, (ClientData) "echo",
	    (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "checkmem", cmdCheckmem, (ClientData) 0,
	    (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "help", cmdHelpBasic, (ClientData) 0,
	    (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "history", cmdHistory, (ClientData) 0,
	    (Tcl_CmdDeleteProc *) NULL);

    /* Call my function to register my commands. */
    AppTclInit (interp);

    buffer = Tcl_CreateCmdBuf();
#ifndef TCL_GENERIC_ONLY
    result = Tcl_Eval(interp, initCmd, 0, (char **) NULL);
    if (result != TCL_OK) {
	printf("%s\n", interp->result);
	return(1);
    }
#endif
    
    /* Clear the command history buffer. */
    for (gCmdhistPtr=0; gCmdhistPtr < 100; gCmdhistPtr++)
    {
        gCmdline[gCmdhistPtr][0] = '\0';
    }
    gCmdhistPtr=0;

    gotPartial = 0;
    tcgetattr(fileno(stdin), &termio_saved);
    while (1) {
	clearerr(stdin);

        tcgetattr(fileno(stdin), &termio);
        termio.c_lflag &= ~(ICANON | ECHO); /* Disable Echo and Canon */
        tcsetattr(fileno(stdin), TCSANOW, &termio);

        if (!gotPartial) {   
            tempStr = Tcl_GetVar(interp, "tclprompt", TCL_GLOBAL_ONLY);
            if (tempStr == NULL) {
                sprintf (promptString, "%s ", defaultprompt);
            }
            else {
                sprintf (promptString, "%s ", tempStr);
            }
            fputs(promptString, stdout);   
            fflush(stdout);   
        }
        else
        {
            fputs(">", stdout);
            fflush(stdout);                 
        }

        if (my_fgets(gCmdline[gCmdhistPtr], 1000, stdin) == NULL) {   
            if (!gotPartial)
            {
                return(0);   
            }   
            gCmdline[gCmdhistPtr][0] = 0;
        }   

        cmd = Tcl_AssembleCmd(buffer, gCmdline[gCmdhistPtr]);   
        cmd = Tcl_AssembleCmd(buffer, eoln);

        /* If user didn't just hit enter, store the command,  */
        /* and advance the command history pointer. */
        if (gCmdline[gCmdhistPtr][0] != 0)
        {
            if (gCmdhistPtr == (CMD_HIST_LEN-1))
            { 
                gCmdhistPtr = 0;
            }
            else
            {
                gCmdhistPtr++;
            }
        }

	if (cmd == NULL) {
	    gotPartial = 1;
	    continue;
	}

	gotPartial = 0;
        tcsetattr(fileno(stdin), TCSANOW, &termio_saved);
	result = Tcl_Eval(interp, cmd, 0, (char **)NULL);
	if (result == TCL_OK) {
	    if (*interp->result != 0) {
		printf("%s\n", interp->result);
                fflush(stdout);
	    }
	    if (quitFlag) {
		Tcl_DeleteInterp(interp);
		Tcl_DeleteCmdBuf(buffer);
#ifdef TCL_MEM_DEBUG
		Tcl_DumpActiveMemory(dumpFile);
#endif
		return(0);
	    }
	} else {
	    if (result == TCL_ERROR) {
		printf("Error");
	    } else {
		printf("Error %d", result);
	    }
            fflush(stdout);
	    if (*interp->result != 0) {
		printf(": %s\n", interp->result);
	    } else {
		printf("\n");
	    }
            fflush(stdout);
	}
    }
}

int tcl_single_command(char *cmdLine)
{
    char eoln[] = "\n";
    char *cmd;
    int result;

    interp = Tcl_CreateInterp();

#ifdef TCL_MEM_DEBUG
    Tcl_InitMemory(interp);
#endif
    Tcl_InitDebug (interp);
    TclX_InitGeneral (interp);
    TclX_InitFmath (interp);
    TclX_InitUnix (interp);
    Tcl_InitReaddir (interp);

    Tcl_CreateCommand(interp, "echo", cmdEcho, (ClientData) "echo",
	    (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "checkmem", cmdCheckmem, (ClientData) 0,
	    (Tcl_CmdDeleteProc *) NULL);

    /* Call my function to register my commands. */
    AppTclInit (interp);

    buffer = Tcl_CreateCmdBuf();

    cmd = Tcl_AssembleCmd(buffer, cmdLine);
    cmd = Tcl_AssembleCmd(buffer, eoln);

    result = Tcl_Eval(interp, cmd, 0, (char **)NULL);
    if (result == TCL_OK) {
        if (*interp->result != 0) {
            printf("%s\n", interp->result);
            fflush(stdout);
        }
    }
    else {
        if (result == TCL_ERROR) {
            printf("Error");
        } else {
            printf("Error %d", result);
        }
        fflush(stdout);
        if (*interp->result != 0) {
            printf(": %s\n", interp->result);
        } else {
            printf("\n");
        }
        fflush(stdout);
    }

    Tcl_DeleteInterp(interp);
    Tcl_DeleteCmdBuf(buffer);

    return(0);
}
