#include <stdio.h>

#include "calculator.h"

int calculator(int a, int b) {
  if (b != 0) {
    return a / b;
  }
  printf(
      "На ноль делить нельзя!\n"
      "Пусть результат будет равен 0, чтобы прога не ломалась)))\n");
  return 0;
}