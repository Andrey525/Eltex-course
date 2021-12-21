#include "calculator.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
	printf("5 + 5 = %d\n", sum(5, 5));
	printf("5 - 5 = %d\n", subtraction(5, 5));
	printf("5 * 5 = %d\n", multiplication(5, 5));
	printf("5 / 5 = %d\n", division(5, 5));
	return 0;
}