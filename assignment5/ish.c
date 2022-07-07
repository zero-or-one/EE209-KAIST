/*--------------------------------------------------------------------*/
/* Assignment 5   */
/* Sabina A       */
/* 20180824       */
/* ish.c          */
/*                */
/*                                                  */
/* Original Author: Bob Dondero                                       */
/* Modified by : Park Ilwoo                                           */
/* Illustrate lexical analysis using a deterministic finite state     */
/* automaton (DFA)                                                    */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#include "lexsyn.h"
#include "dynarray.h"
#include "token.h"
#include "util.h"
// #include "cmd.h"


enum { MAX_LENGTH = 1024 };
time_t ptime = 0;
int num_jobs = 0;
//-----------------------------------------
void run_command(enum BuiltinType btype, DynArray_T oTokens) {
    size_t tlen = DynArray_getLength(oTokens);
    struct Token* token;
    if (btype == B_CD) {
        // change directory
        if (tlen == 0 + 1) {
            char* hdir = getenv("HOME");
            if (!hdir) {
                errorPrint("No home directory exists", FPRINTF);
            }
            if (chdir(hdir) == -1) {
                errorPrint("Cannot change directory, sorry ((((", FPRINTF);
            }
        }
        else if (tlen == 1 + 1) {
            token = DynArray_get(oTokens, 1);
            if (chdir(token->pcValue) == -1) {
                errorPrint("No such file or directory", FPRINTF);
            }
        }
        else {
            errorPrint("Too many parameters for chanding directory", FPRINTF);
        }
    }
    /*
    else if (btype == B_FG) {
      if(num_lobs==0){
        errorPrint("Too many parameters for chanding directory", FPRINTF);
        return;
      }
    } */
    else if (btype == B_EXIT) {
        // free everything
        for (size_t i = 0; i < tlen; i++) {
            token = DynArray_get(oTokens, i);
            freeToken(token, NULL);
        }
        DynArray_free(oTokens);
        exit(0);
    }
    else if (btype == B_SETENV) {
        // set environment
        token = DynArray_get(oTokens, 1);
        if (tlen == 2) {
            setenv(token->pcValue, "", 1);
        }
        else if (tlen == 3) {
            struct Token* second_token = DynArray_get(oTokens, 2);
            setenv(token->pcValue, second_token->pcValue, 1);
        }
        else {
            errorPrint("Too few or too many parameters for setting environment", FPRINTF);
        }
    }
    else if (btype == B_USETENV) {
        // unset environment
        if (tlen != 2) {
            errorPrint("Too few parameters for unsetting environment", FPRINTF);
        }
        else {
            token = DynArray_get(oTokens, 1);
            unsetenv(token->pcValue);
        }
    }
    //else if (btype == B_ALIAS) {
    //
    //}
    else {
        // other commands
        token = DynArray_get(oTokens, 0);
        char* cmd = token->pcValue;
        if (cmd == NULL) {
            errorPrint("Null argument", PERROR);
            exit(EXIT_FAILURE);
        }
        if (btype == B_FG) {
            if (num_jobs == 0) {
                errorPrint("No current processes", FPRINTF);
                return;
            }
        }
        int pid = fork();
        if (pid != 0) {
            wait(NULL);
            //if (waitpid(pid, NULL, 0) == -1){
            //  errorPrint("Parent waiting error.", PERROR);
            //  return;
            //  }
        }
        else {
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            char** args = malloc(sizeof(char*) * (tlen + 3));
            for (int i = 0; i < tlen; i++) {
                token = DynArray_get(oTokens, i);
                /*
                if (token -> pcValue[0] == '<') {
                  struct Token *name = DynArray_get(oTokens, i+1);
                  FILE *fn = fopen(name->pcValue,"r");
                  if (!fn) {
                  errorPrint("FIle does not exists.", PERROR);
                  return;
                  }
              }
              if (token -> pcValue[0] == '>') {
                struct Token *name = DynArray_get(oTokens, i+1);
                FILE *fn = fopen(name->pcValue,"w");
                if (!fn) {
                  errorPrint("Cannot open the file.", PERROR);
                  return;
                  }
                dup2(fn, 1);
                close(fn);
              }
              */
                args[i] = token->pcValue;
            }
            printf("%% ");
            execvp(cmd, args);
            // if here, error
            errorPrint("Child process error", PERROR);
            exit(-1);
        }
    }
    //num_jobs = 1;
}

//-----------------------------------------
static void
shellHelper(const char* inLine) {
    DynArray_T oTokens;

    enum LexResult lexcheck;
    enum SyntaxResult syncheck;
    enum BuiltinType btype;

    oTokens = DynArray_new(0);
    if (oTokens == NULL) {
        errorPrint("Cannot allocate memory", FPRINTF);
        exit(EXIT_FAILURE);
    }
    char acLine[MAX_LINE_SIZE];
    //checkAlias(inLine, acLine);

    lexcheck = lexLine(inLine, oTokens);
    switch (lexcheck) {
    case LEX_SUCCESS:
        if (DynArray_getLength(oTokens) == 0)
            return;

        /* dump lex result when DEBUG is set */
        dumpLex(oTokens);

        syncheck = syntaxCheck(oTokens);
        if (syncheck == SYN_SUCCESS) {
            btype = checkBuiltin(DynArray_get(oTokens, 0));
            /* TODO */
            run_command(btype, oTokens);
        }

        /* syntax error cases */
        else if (syncheck == SYN_FAIL_NOCMD)
            errorPrint("Missing command name", FPRINTF);
        else if (syncheck == SYN_FAIL_MULTREDOUT)
            errorPrint("Multiple redirection of standard out", FPRINTF);
        else if (syncheck == SYN_FAIL_NODESTOUT)
            errorPrint("Standard output redirection without file name", FPRINTF);
        else if (syncheck == SYN_FAIL_MULTREDIN)
            errorPrint("Multiple redirection of standard input", FPRINTF);
        else if (syncheck == SYN_FAIL_NODESTIN)
            errorPrint("Standard input redirection without file name", FPRINTF);
        else if (syncheck == SYN_FAIL_INVALIDBG)
            errorPrint("Invalid use of background", FPRINTF);
        break;

    case LEX_QERROR:
        errorPrint("Unmatched quote", FPRINTF);
        break;

    case LEX_NOMEM:
        errorPrint("Cannot allocate memory", FPRINTF);
        break;

    case LEX_LONG:
        errorPrint("Command is too large", FPRINTF);
        break;

    default:
        errorPrint("lexLine needs to be fixed", FPRINTF);
        exit(EXIT_FAILURE);
    }
}

//-----------------------------------------
void handler_sigint(int sign) {
    return;
}

void handler_sigquit(int sign) {

    time_t ctime = time(NULL);

    if (ctime - ptime >= 5) {
        ptime = ctime;
        printf("\nPress Ctrl- again within 5 seconds to exit please\n");
    }
    else {
        exit(0);
    }
}


//-----------------------------------------
int main(int argc, char* argv[]) {
    char line[MAX_LENGTH];

    //errorPrint("./ish", SETUP);


    // unblock all signals

    sigset_t sig_set;
    sigemptyset(&sig_set);
    sigaddset(&sig_set, SIGINT);
    sigaddset(&sig_set, SIGALRM);
    sigaddset(&sig_set, SIGQUIT);
    sigaddset(&sig_set, SIGCHLD);
    sigprocmask(SIG_UNBLOCK, &sig_set, NULL); // ?

    // put the necessary handlers
    signal(SIGINT, handler_sigint);
    signal(SIGQUIT, handler_sigquit);

    errorPrint(argv[0], SETUP);

    // handle initial loc
    chdir(getenv("HOME"));

    // another file
    char home_dir[136];
    //home_dir = getenv("HOME");
    strcpy(home_dir, getenv("HOME"));
    strcat(home_dir, "/.ishrc");

    // read from file if exists
    FILE* file = fopen(home_dir, "r");
    if (file) {
        while (fgets(line, MAX_LENGTH, file)) {
            printf("%% ");
            shellHelper(line);
        }
        fclose(file);
    }
    // read from std
    //else {
    printf("%% ");
    chdir(getenv("PWD"));

    while (1) {
        //char inp = fgets(line, sizeof(line), stdin);
        if (!fgets(line, sizeof(line), stdin)) {
            exit(0);
        }
        printf("%% ");
        shellHelper(line);
    }
    //}
    return 0;
}
