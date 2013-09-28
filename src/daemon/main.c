// Based off: http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html

#define EXE "/Users/neil/Desktop/beeps"

#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(void) {

    pid_t pid, sid;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then
     we can exit the parent process. */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    /* Change the file mode mask */
    umask(0);

    /* Open any logs here */

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        /* Log the failure */
        exit(EXIT_FAILURE);
    }

    /* Change the current working directory */
    if ((chdir("/")) < 0) {
        /* Log the failure */
        exit(EXIT_FAILURE);
    }

    /* Close out the standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    /* Daemon-specific initialization goes here */
    while (1) {
        pid_t cid;

        /* Create a child process */
        cid = fork();

        if (cid == 0) {
            /* Replace child process with other program */
            execv(EXE, NULL);
            exit(EXIT_FAILURE); // this will never be reached
        }
        else if(cid < 0) {
            /* Handle child create error */
            exit(EXIT_FAILURE);
        }

        /* Wait for child process to die */
        waitpid(cid, NULL, 0);
        sleep(2);
    }

    exit(EXIT_SUCCESS);
}
