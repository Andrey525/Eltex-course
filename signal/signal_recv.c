#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#define FLAG 1

void handler(int signum) {
    if (signum == 10) {
        printf("SIGUSR1\n");
    } else if (signum == 12) {
        printf("SIGUSR2\n");
    }
}

int main() {
    printf("pid = %d\n", getpid());

#if FLAG == 0
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
#endif

#if FLAG == 1

    struct sigaction action;
    action.sa_handler = handler;
    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action, NULL);

#endif

    while (1)
        ;
    return 0;
}