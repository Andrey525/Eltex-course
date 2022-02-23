#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) { // доча
        printf("Доча: pid = %d, parent_pid = %d\n", getpid(), getppid());
        exit(EXIT_SUCCESS);
    } else { // папа
        printf("Папа: pid = %d, parent_pid = %d\n", getpid(), getppid());
        wait(&status);
    }
    return 0;
}