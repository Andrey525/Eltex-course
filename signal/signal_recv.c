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
    sigset_t set, old_set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);
    sigprocmask(SIG_BLOCK, &set, &old_set);
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

    while (1) {
        // Sigsuspend временно изменяет значение маски блокировки сигналов
        // процесса на указанное old_set
        // и то есть SIGUSR1 и SIGUSR2 сейчас разблокированы
        sigsuspend(&old_set);
        // при sigsuspend возврате он сбрасывает маску сигнала процесса до
        // исходного значения, в этом случае
        // SIGUSR1 и SIGUSR2 сигналы снова блокируются.

        //  sigprocmas явно разблокирует сигналы из множества
        sigprocmask(SIG_UNBLOCK, &set, NULL);
    }
    return 0;
}