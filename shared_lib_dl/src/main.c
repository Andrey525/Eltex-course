#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "calculator.h"
/*
    ########
    #Костыли:
    ########
    ______

    Директория указана относительно папки проекта, запуск из другого места не
   получится
    ______

    А так вроде норм...
    :)
*/

#define MAX_DIGITS 5 + 1
int capacity_plugins = 10;
char **plugins;

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

// считывает названия библиотек в массив plugins,
// возвращает в случае успеха их количество.
int read_plugins_dir() {
  int i;
  struct dirent *entry;
  DIR *dir;
  dir = opendir("./lib");
  if (dir == NULL) {
    return -1;
  }
  errno = 0;
  plugins = malloc(sizeof(char *) * capacity_plugins);
  for (i = 0; i < capacity_plugins; i++) {
    plugins[i] = malloc(sizeof(char) * 256);
  }
  i = 0;
  while ((entry = readdir(dir)) != NULL) {
    if (i == capacity_plugins) {
      capacity_plugins *= 2;
      plugins = realloc(plugins, sizeof(char *) * capacity_plugins);
      for (int j = i; j < capacity_plugins; j++) {
        plugins[j] = malloc(sizeof(char) * 256);
      }
    }
    //если в названии есть .so и начинается с lib
    //тогда добавляем в массив плагинов эту строку
    if (strstr(entry->d_name, ".so") != NULL &&
        (entry->d_name - strstr(entry->d_name, "lib")) == 0) {
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
  char *string_path;

  int num1, num2, rezult;
  int operation;

  int flag;

  int count;
  int i;

  count = read_plugins_dir();
  if (count == 0) {
    printf("В библиотеке отсутствуют плагины!\n");
    exit(1);
  } else if (count == -1) {
    printf("Библиотека не найдена!\n");
    exit(2);
  }

  while (1) {
    printf("Выберите операцию:\n");
    for (i = 0; i < count; i++) {
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

    arithmetic_operation = dlsym(handle, "calculator");
    if ((error = dlerror()) != NULL) {
      fprintf(stderr, "%s\n", error);
      exit(1);
    }
    rezult = arithmetic_operation(num1, num2);
    printf("Результат = %d\n", rezult);
    dlclose(handle);

    printf(
        "1 - Продолжить\n"
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
      free(string_path);
      for (i = 0; i < capacity_plugins; i++) {
        free(plugins[i]);
      }
      free(plugins);
      return 0;
    } else {
      system("clear");
    }
    free(string_path);
  }
  return 0;
}