#include "calculator.h"
#include <stdio.h>
#include <stdio_ext.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

#define MAX_DIGITS 5 + 1
#define MAX_COUNT_PLUGINS 10

int enter_number(int *rezult) {
    char str[MAX_DIGITS];
    int number;
    int i;
    fgets(str, MAX_DIGITS, stdin);
    __fpurge(stdin);
    i = 0;
    while (str[i] != '\0' && str[i] != '\n') {
        if (str[i] >= 0x30 && str[i] <= 0x39) {
            i++;
        } else {
            printf("Ошибка ввода!\n");
            return 1;
        }
    }
    number = atoi(str);
    *rezult = number;
    return 0;
}

int read_plugins_dir(char **plugins) {
    int count = 0;
    int i;
    for (i = 0; i < MAX_COUNT_PLUGINS; i++) {
        plugins[i] = malloc(sizeof(char) * 256);
    }
    struct dirent *entry;
    DIR *dir;
    dir = opendir("./lib");
    if (dir == NULL) {
        perror("Read dir error");
        return -1;
    }
    errno = 0;
    i = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(".", entry->d_name) != 0 &&
            strcmp("..", entry->d_name) != 0) {
            plugins[i] = strcpy(plugins[i], entry->d_name);
            count++;
            i++;
        }
    }
    if (errno && !entry) {
        perror("Print dir error");
    }
    closedir(dir);
    return count;
}

int main(int argc, char *argv[]) {
	void *handle;
	int (*arithmetic_operation)(int, int);
	char *error;

    int num1, num2, rezult;
    int flag;
    int operation;
    char *plugins[MAX_COUNT_PLUGINS];
    int count;

    while(1) {
        printf("1 - Продолжить\n"
            "2 - Закончить\n");

        flag = 1;
        while (flag != 0) {
            printf("Введите номер действия: ");
            flag = enter_number(&operation);
            if (flag == 0 && (operation < 1 || operation > 2)) {
                printf("Введено неверное действие!\n");
                flag = 1;
            }    
        }

        if (operation == 2) {
            return 0;
        } else {
            system("clear");
        }

        flag = 1;
        while (flag != 0) {
            printf("Введите первое число: ");
            flag = enter_number(&num1);
        }

        flag = 1;
        while (flag != 0) {
            printf("Введите второе число: ");
            flag = enter_number(&num2);
        }

        printf("Выберите операцию:\n");
        count = read_plugins_dir(plugins);
        for (int i = 0; i < count; i++) {
            printf("%d - %s\n", i, plugins[i]);
        }

        flag = 1;
        while (flag != 0) {
            printf("Введите номер операции: ");
            flag = enter_number(&operation);
            if (flag == 0 && (operation < 0 || operation > count)) {
                printf("Введен неверный номер операции!\n");
                flag = 1;
            }    
        }

        char *string = malloc(sizeof(char) * 256);
        string = strcpy(string, "./lib/");
        string = strcat(string, plugins[operation]);
        handle = dlopen(string, RTLD_LAZY);
        if (!handle) {
            fputs(dlerror(), stderr);
            exit(1);
        }
        char *string2 = malloc(sizeof(char) * 256);
        string2 = strcpy(string2, plugins[operation]);
        string2 = string2 + 3;
        string2[strlen(string2) - 3] = '\0';
        arithmetic_operation = dlsym(handle, string2);
        if ((error = dlerror()) != NULL)  {
            fprintf (stderr, "%s\n", error);
            exit(1);
        }
        rezult = arithmetic_operation(num1, num2);
        printf("Результат = %d\n", rezult);
        dlclose(handle);
    }
	return 0;
}