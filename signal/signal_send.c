#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define MAX_DIGITS 5

void clear_stdin() {
    while (getchar() != '\n')
        ;
}

int enter_number(int *rezult) {
    char str_number[MAX_DIGITS +
                    1]; // массив из шести символов, последний под конец строки
    int i;
    fgets(str_number, MAX_DIGITS + 1, stdin); // считываем 5 символов
    if (str_number[strlen(str_number) - 1] == '\n') {
        str_number[strlen(str_number) - 1] = '\0';
    } else {
        clear_stdin();
    }
    i = 0;
    while (str_number[i] != '\0' && str_number[i] != '\n') {
        if (str_number[i] >= 0x30 && str_number[i] <= 0x39) {
            i++;
        } else {
            printf("Ошибка ввода!\n");
            return 1;
        }
    }
    if (strlen(str_number) == 0) {
        printf("Ошибка ввода!\n");
        return 1;
    }
    *rezult = atoi(str_number);
    return 0;
}

int main() {
    pid_t pid;
    int number;
    char name_of_signal[7 + 1];
    while (1) {
        do {
            printf(
                "Введите pid процесса, сигналом которому вы хотите передать: ");
            fflush(stdout);
        } while (enter_number(&number));
        pid = (pid_t)number;
        do {
            printf("Введите название сигнала: ");
            fflush(stdout);
            fgets(name_of_signal, 7 + 1, stdin);
            if (name_of_signal[strlen(name_of_signal) - 1] == '\n') {
                name_of_signal[strlen(name_of_signal) - 1] = '\0';
            } else {
                clear_stdin();
            }
        } while ((strcmp(name_of_signal, "SIGUSR1") != 0 &&
                  strcmp(name_of_signal, "SIGUSR2") != 0));
        if (strcmp(name_of_signal, "SIGUSR1") == 0) {
            kill(pid, SIGUSR1);
        } else {
            kill(pid, SIGUSR2);
        }
    }

    return 0;
}