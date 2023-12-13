#include "systemcalls.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
 */
bool do_system(const char *cmd)
{

    /*
     * TODO  add your code here (DONE)
     *  Call the system() function with the command set in the cmd
     *   and return a boolean true if the system() call completed with success
     *   or false() if it returned a failure
     */
    int ret = (system(cmd) == 0);

    return ret;
}

/**
 * @param count -The numbers of variables passed to the function. The variables are command to execute.
 *   followed by arguments to pass to the command
 *   Since exec() does not perform path expansion, the command to execute needs
 *   to be an absolute path.
 * @param ... - A list of 1 or more arguments after the @param count argument.
 *   The first is always the full path to the command to execute with execv()
 *   The remaining arguments are a list of arguments to pass to the command in execv()
 * @return true if the command @param ... with arguments @param arguments were executed successfully
 *   using the execv() call, false if an error occurred, either in invocation of the
 *   fork, waitpid, or execv() command, or if a non-zero return value was returned
 *   by the command issued in @param arguments with the specified arguments.
 */

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char *command[count + 1];
    int i;
    for (i = 0; i < count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

    fflush(stdout);
    /* Forking*/
    int ret = 0;
    int pid = fork();
    if (pid == -1)
    { /* Error*/
        printf("Error occured while forking\n");
        ret = 1;
    }
    else if (pid == 0)
    { /* Child process*/
        execv(command[0], (char **)command);
    }
    else
    { /* Parent process*/
         waitpid(pid, &ret, 0);
    }

    va_end(args);

    return (ret==0);
}

/**
 * @param outputfile - The full path to the file to write with command output.
 *   This file will be closed at completion of the function call.
 * All other parameters, see do_exec above
 */
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char *command[count + 1];
    int i;
    for (i = 0; i < count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

    /* Forking*/
    int ret = fork();
    if (ret == -1)
    { /* Error*/
        printf("Error occured while forking\n");
        return false;
    }
    else if (ret == 0)
    { /* Child process*/
        int fd = open(outputfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
        if (fd < 0)
        {
            printf("error while trying to open new fd\n");
        }
        if (dup2(fd, 1) < 0)
        {
            printf("error while redirecting output");
        }
        close(fd);

        if (execv(command[0], command) == -1)
            exit(1);
    }
    else
    { /* Parent process*/
        int status;
        if (waitpid(ret, &status, 0) == -1)
            return false;
        else if (WIFEXITED(status))
            return WIFEXITED(status);
        return false;
    }

    va_end(args);

    return true;
}
