#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <execinfo.h>
#include "Alarm.h"
#include "Counter.h"
#include "tcl.h"
#include "HAL.h"
#include "SFM.h"
#include "Snmp.h"
#include "Web.h"    // put this header file at last

#define CRASH_STACK_SIZE 100
void crashHandlerInit();
static void crashHandler(int signo, siginfo_t *sigInfo, void *ptr);

#define RegisterMenu(name, func)       Tcl_CreateCommand( interp, name, func, (ClientData) 0, (Tcl_CmdDeleteProc *) NULL);
char gTclInitCmd[] = "puts stdout \"\nWelcome to DanOS Application\n\"; set tclprompt \"DOS>\";";

extern "C" int TclCommand_app (ClientData, Tcl_Interp*, int argc, char **argv)
{
    return TCL_OK;
}

extern "C" int TclCommand_sfm (ClientData, Tcl_Interp*, int argc, char **argv)
{
    SFMMenu(argc--, argv++);
    return TCL_OK;
}

extern "C" int TclCommand_hal (ClientData, Tcl_Interp*, int argc, char **argv)
{
    HALMenu(argc, argv);
    return TCL_OK;
}

extern "C" int TclCommand_hw (ClientData, Tcl_Interp*, int argc, char **argv)
{
    HwMenu(argc, argv);
    return TCL_OK;
}

extern "C" int TclCommand_am (ClientData, Tcl_Interp*, int argc, char **argv)
{
    ActiveAlarm::GetInst().PrintAlarms();
    return TCL_OK;
}

extern "C" int TclCommand_pm (ClientData, Tcl_Interp*, int argc, char **argv)
{
    ActivePM::GetInst().PrintPM();
    return TCL_OK;
}

extern "C" int TclCommand_sh (ClientData, Tcl_Interp*, int argc, char **argv)
{
    if (argc >= 2)
    {
        char cmdline[128] = {0};
        for (int i = 1; i < argc; i++)
        {
            strcat(cmdline, argv[i]);
            strcat(cmdline, " ");
        }
        system(cmdline);
    }

    return TCL_OK;
}

extern "C" int TclCommand_swversion (ClientData, Tcl_Interp*, int argc, char **argv)
{
#if 0
    char swversion[1024] = {0};
    const char *ver_file = "/usr/local/etc/swversion";
    FILE *fp = fopen(ver_file, "r");
    if (fp != NULL)
    {
        size_t len = fread (swversion, sizeof(char), 1024 - 1, fp);
        if (len > 0)
        {
            APP_LOG("%s", swversion);
        }
        else
        {
            APP_LOG("no version info");
        }
        fclose(fp);
    }
    else
    {
        APP_LOG("open %s failed", ver_file);
    }
#endif
    return TCL_OK;
}

extern "C" int TclCommand_shutdown (ClientData, Tcl_Interp*, int argc, char **argv)
{
#if 0
    AppTermPara appTermParam;
    appTermParam.termReason = APP_RESTARTREASON_MANNUAL;

    if (argc == 2 && strcmp(argv[1], "warm") == 0)
    {
        appTermParam.termType = APP_RESTARTTYPE_WARMRESETSHELF;
    }
    else
    {
        appTermParam.termType = APP_RESTARTTYPE_COLDRESETSHELF;
    }

    Terminate(&appTermParam);

    exit(0);
#endif
    exit(0);

    return TCL_OK;
}

extern "C" void AppTclInit (Tcl_Interp *interp)
{
    int result;

    Tcl_SetVar(interp, (char*)"tcleditmode", "yes", TCL_LEAVE_ERR_MSG);

    RegisterMenu("app", TclCommand_app);
    RegisterMenu("sfm", TclCommand_sfm);
    RegisterMenu("hal", TclCommand_hal);
    RegisterMenu("hw", TclCommand_hw);
    RegisterMenu("am", TclCommand_am);
    RegisterMenu("pm", TclCommand_pm);
    RegisterMenu("sh", TclCommand_sh);       
    //REGISTER_MERU("swversion", TclCommand_swversion);

    RegisterMenu("shutdown", TclCommand_shutdown);
    RegisterMenu("quit", TclCommand_shutdown);
    RegisterMenu("exit", TclCommand_shutdown);

    /* Run TCL command above to set up tcl env, print msg. */
    result = Tcl_Eval(interp, gTclInitCmd, 0, (char **) NULL);
    if (result != TCL_OK)
    {
        printf("Tcl_Eval fail errno = %s\n", interp->result);
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    //setvbuf(stdout, NULL, _IONBF, 0);
    crashHandlerInit();
    //terminateSet(terminateHandler);
    
    //DBFInit();
    bool hwOK = HALInit();

    if (!IsFileExist("noinit") && hwOK)
    {
        SFMInit();
        WebInit();  // after service restore, we enable the northband interface
        SnmpAgentInit();
    }

    setenv("gAppDebugMode", "true", 1);
    tcl_main();

    return 0;
}

void crashHandlerInit()
{
    struct sigaction act;
    //stack_t          ss;
    int              signo;

    memset(&act, 0, sizeof(act));
    act.sa_flags = SA_SIGINFO | SA_ONSTACK;
    act.sa_sigaction = crashHandler;
    sigaction(SIGBUS,  &act, NULL);
    sigaction(SIGILL,  &act, NULL);
    sigaction(SIGSEGV, &act, NULL);
    sigaction(SIGFPE,  &act, NULL);
    sigaction(SIGTRAP, &act, NULL);

    // Catch additional abnormal signals.
    sigaction(SIGSYS,  &act, NULL);
    sigaction(SIGXCPU, &act, NULL);
    sigaction(SIGXFSZ, &act, NULL);
    sigaction(SIGSTKFLT, &act, NULL);

    // If we ever want to generate core dumps via calls to abort(), comment out the line below
    sigaction(SIGABRT, &act, NULL);

    // Set default handler for async and user signals.
    sigaction(SIGALRM, &act, NULL);
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);
    sigaction(SIGIO,   &act, NULL);
    sigaction(SIGPROF, &act, NULL);
    sigaction(SIGPWR, &act, NULL);
    sigaction(SIGVTALRM, &act, NULL);

    // Set default handler for realtime signals.
    for (signo = SIGRTMIN; signo <= SIGRTMAX; signo++)
    {
        sigaction(signo, &act, NULL);
    }
#if 0
    // Give the crashHandler() its own stack to use when processing fatal
    // exceptions caused by this process (or any of its children). This will
    // allow the crashHandler to process stack overflows.
    // NOTE: threads will not inherit this alternate stack.  Each thread must
    // create its own alternate stack.  Threads created via the taskLib will
    // have an alternate stack (courtesy of code in task_wrapper() ).
    if ((ss.ss_sp = malloc(CRASH_STACK_SIZE)) != NULL)
    {
        ss.ss_size = CRASH_STACK_SIZE;
        ss.ss_flags = 0;
        sigaltstack(&ss, NULL);
    }
#endif
}

static void crashHandler(int signo, siginfo_t *sigInfo, void *ptr)
{
    int nptrs;
    void *buffer[CRASH_STACK_SIZE];
    char **strings;
    
    nptrs = backtrace(buffer, CRASH_STACK_SIZE);
    printf("backtrace() returned %d addresses, signo = %d\n", nptrs, signo);
    /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
    *  would produce similar output to the following: */

    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nptrs; i++)
        printf("%s\n", strings[i]);
    free(strings);
    exit(-1);
}

