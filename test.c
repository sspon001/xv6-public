#include "test.h"

int
main(int argc, char *argv[]){

    int i = 2 ;
    int temp = 0 ;
    int* status = &i ;

    /* Test 1 : using an existing PID with waitpid system call */

    int pid = fork() ;
    if(pid > 0) {
        printf(1, "\nParent Process: created child (PID %d)\n", pid);
        pid = waitpid(status, pid);
        printf(1, "\nchild (PID %d) exited with status %d\n", pid, *status);
    }
    else if(pid == 0) {
        printf(1, "\nchild: exiting\n");
        exit(0);
    }
    else{
        printf(1, "\nfork error\n") ;
    }

   /* Test 2 : using a non existent PID with waitpid system call */

    pid = fork() ;
    if(pid > 0) {
        printf(1, "\nParent Process: created child (PID %d)\n", pid);
        temp = pid ;
        pid = waitpid(status, 22);
        if(pid == -1) printf(1,"\nPID %d not found, waitpid returned %d\n", 22, *status) ;
        else printf(1, "\nchild (PID %d) exited with status %d\n", temp, *status);
    }
    else if(pid == 0) {
        printf(1, "\nchild: exiting\n");
        exit(0);
    }
    else{
        printf(1, "\nfork error\n") ;
    }
    pid = waitpid(status, temp) ;
    if(pid == -1) printf(1,"\n PID %d not found, waitpid returned exit status %d\n", temp, *status) ;
    else printf(1, "\nchild (PID %d) exited with status %d\n", temp, *status);
    exit(0) ;
}