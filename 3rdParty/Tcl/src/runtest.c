
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "tcl.h"

int testrun_Command (ClientData clientData, Tcl_Interp *interp, 
               int argc, char **argv)
{
    /* do something interesting here */

    /* Send back interesting result */
    Tcl_SetResult(interp, "test passed!", TCL_VOLATILE);

    return TCL_OK;
}

/* Register my command with the TCL command line */
int testrun_Init (Tcl_Interp *interp)
{
    Tcl_CreateCommand (interp, "testrun", testrun_Command,
		       (ClientData) 0, (Tcl_CmdDeleteProc *) NULL);

    return TCL_OK;
}
