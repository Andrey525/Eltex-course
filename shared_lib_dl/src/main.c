#include "calculator.h"
#include <stdio.h>
#include <stdio_ext.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGITS 5 + 1

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

int main(int argc, char *argv[]) {
	void *handle;
	int (*arithmetic_operation)(int, int);
	char *error;

    int num1, num2, rezult;
    int flag;
    int operation;

	handle = dlopen("./lib/libcalc.so", RTLD_LAZY);
	if (!handle) {
		fputs(dlerror(), stderr);
        exit(1);
	}

    while(1) {
        printf("1 - Продолжить\n"
            "2 - Закончить\n");

        flag = 1;
        while (flag != 0) {
            printf("Введите номер действия: ");
            flag = enter_number(&operation);
            if (flag == 0 && (operation < 1 || operation > 2)) {
                printf("Введен неверное действие!\n");
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

        printf("Выберите операцию:\n"
            "1 - Сложение\n"
            "2 - Вычитание\n"
            "3 - Умножение\n"
            "4 - Деление\n");

        flag = 1;
        while (flag != 0) {
            printf("Введите номер операции: ");
            flag = enter_number(&operation);
            if (flag == 0 && (operation < 1 || operation > 4)) {
                printf("Введен неверный номер операции!\n");
                flag = 1;
            }    
        }

        switch(operation) {
        case 1:
            arithmetic_operation = dlsym(handle, "sum");
            break;
        case 2:
            arithmetic_operation = dlsym(handle, "subtraction");
            break;
        case 3:
            arithmetic_operation = dlsym(handle, "multiplication");
            break;
        case 4:
            arithmetic_operation = dlsym(handle, "division");
            break;
        default:
            break;
        }
        if ((error = dlerror()) != NULL)  {
            fprintf (stderr, "%s\n", error);
            exit(1);
        }
        rezult = arithmetic_operation(num1, num2);

        printf("Результат = %d\n", rezult);
    }
    dlclose(handle);
	return 0;
}