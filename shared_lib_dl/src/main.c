#include "calculator.h"
#include <stdio.h>
#include <stdio_ext.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
/*
    ########
    #Костыли:
    ########

    Для определения названия функции
    в строках смещаюсь 
    с начала на 3 элемента (пропускаю lib)
    с конца на 3 элемента (убираю .so)
    ______

    Из-за этого смещения не могу нормально высвободить память с помощью free()
    ______

    Также считываются все файлы и папки, находящиеся в директории ./lib. 
    То есть, если там окажется мусор,
    то он появится и в массиве, что приведет потом к ошибке в программме
    ______

    Директория указана относительно папки проекта, запуск из другого места не получится
    ______

    Максимальное количество подключаемых плагинов - задано статически,
    лучше бы динамически выделять память

    А так вроде норм...
    :)
*/

#define MAX_DIGITS 5 + 1
#define MAX_COUNT_PLUGINS 10

int enter_number(int *rezult) {
    char str_number[MAX_DIGITS];
    int i;
    fgets(str_number, MAX_DIGITS, stdin);
    __fpurge(stdin);
    i = 0;
    while (str_number[i] != '\0' && str_number[i] != '\n') {
        if (str_number[i] >= 0x30 && str_number[i] <= 0x39) {
            i++;
        } else {
            printf("Ошибка ввода!\n");
            return 1;
        }
    }
    *rezult = atoi(str_number);
    return 0;
}

int read_plugins_dir(char **plugins) { // считывает названия библиотек в массив plugins, возвращает в случае успеха их количество.
    int i;
    struct dirent *entry;
    for (i = 0; i < MAX_COUNT_PLUGINS; i++) {
        plugins[i] = malloc(sizeof(char) * 256);
    }
    DIR *dir;
    dir = opendir("./lib");
    if (dir == NULL) {
        return -1;
    }
    errno = 0;
    i = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(".", entry->d_name) != 0 &&
            strcmp("..", entry->d_name) != 0) {
            plugins[i] = strcpy(plugins[i], entry->d_name);
            i++;
        }
    }
    if (errno && !entry) {
        perror("Print dir error");
    }
    closedir(dir);
    return i;
}

int main(int argc, char *argv[]) {
	void *handle;
	int (*arithmetic_operation)(int, int);
	char *error;

    int num1, num2, rezult;
    int operation;

    int flag;
    
    char *plugins[MAX_COUNT_PLUGINS];
    int count;

    count = read_plugins_dir(plugins);
    if (count == 0) {
        printf("В библиотеке отсутствуют плагины!\n");
        exit(1);
    } else if (count == -1) {
        printf("Библиотека не найдена!\n");
        exit(2);
    }

    while(1) {
        char *string_path;
        char *string_name_func;

        printf("Выберите операцию:\n");
        for (int i = 0; i < count; i++) {
            printf("%d - %s\n", i, plugins[i]);
        }
        do {
            printf("Введите номер операции: ");
            flag = enter_number(&operation);
            if (flag == 0 && (operation < 0 || operation >= count)) {
                printf("Введен неверный номер операции!\n");
                flag = 1;
            }
        } while (flag != 0);

        do {
            printf("Введите первое число: ");
            flag = enter_number(&num1);
        } while (flag != 0);

        do {
            printf("Введите второе число: ");
            flag = enter_number(&num2);
        } while (flag != 0);

        printf("Operation = %d Num1 = %d Num2 = %d\n", operation, num1, num2);

        string_path = malloc(sizeof(char) * 256);
        string_path = strcpy(string_path, "./lib/");
        string_path = strcat(string_path, plugins[operation]);
        handle = dlopen(string_path, RTLD_LAZY);
        if (!handle) {
            fputs(dlerror(), stderr);
            exit(1);
        }

        string_name_func = malloc(sizeof(char) * 256);
        string_name_func = strcpy(string_name_func, plugins[operation]);
        string_name_func = string_name_func + 3;
        string_name_func[strlen(string_name_func) - 3] = '\0';
        arithmetic_operation = dlsym(handle, string_name_func);
        if ((error = dlerror()) != NULL)  {
            fprintf (stderr, "%s\n", error);
            exit(1);
        }
        rezult = arithmetic_operation(num1, num2);
        printf("Результат = %d\n", rezult);
        dlclose(handle);

        printf("1 - Продолжить\n"
            "2 - Закончить\n");
        do {
            printf("Введите номер действия: ");
            flag = enter_number(&operation);
            if (flag == 0 && (operation < 1 || operation > 2)) {
                printf("Введено неверное действие!\n");
                flag = 1;
            }    
        } while (flag != 0);

        if (operation == 2) {
            return 0;
        } else {
            system("clear");
        }
        // free(string_path);
        //free(string_name_func); // здесь ошибка free (из-за того, что провожу манипуляции с указателем)
    }
	return 0;
}