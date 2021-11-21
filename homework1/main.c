#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>

void clear_stdin() {
	while (getchar() != '\n');
}

void first() {
	int i, n;
	while (1) {
		printf("Enter n: ");
		if (scanf("%d", &n) != 1) {
			printf("You entered not integer type!\n");
			clear_stdin();
			continue;
		} else {
			break;
		}
	}
	for (i = 1; i <= n * n; i++) {
		printf("%5d ", i);
		if (i % n == 0) {
			printf("\n");
		}
	}
}

void second() {
	int i, n;
	int *array;
	while (1) {
		printf("Enter n: ");
		if (scanf("%d", &n) != 1) {
			printf("You entered not integer type!\n");
			clear_stdin();
			continue;
		} else {
			break;
		}
	}
	array = malloc(sizeof(int) * n);
	printf("Enter array's elements:\n");
	for (i = 0; i < n; i++) {
		while (1) {
			printf("array[%d] = ", i);
			if (scanf("%d", &array[i]) != 1) {
				printf("You entered not integer type!\n");
				clear_stdin();
				continue;
			} else {
				break;
			}	
		}
		
	}
	printf("Source array: ");
	for (i = 0; i < n; i++) {
		printf("%5d", array[i]);
	}
	printf("\nReverse array: ");
	for (i = n - 1; i >= 0; i--) {
		printf("%5d", array[i]);
	}
	printf("\n");
}

void third() {
	int i, j, n;
	int *matrix;
	while (1) {
		printf("Enter n: ");
		if (scanf("%d", &n) != 1) {
			printf("You entered not integer type!\n");
			clear_stdin();
			continue;
		} else {
			break;
		}
	}
	
	matrix = malloc(sizeof(int) * n * n);
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (j < n - i - 1) {
				*(matrix + i * n + j) = 0;
			} else {
				*(matrix + i * n + j) = 1;
			}
			
		}
	}
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			printf("%5d", matrix[i * n + j]);
		}
		printf("\n");
	}
}

void fourth() {
	int i, j, n;
	int counter_of_spiral;
	int number;
	int *matrix;
	while (1) {
		printf("Enter uneven n: ");
		if ((scanf("%d", &n) != 1) || (n % 2 == 0)) {
			printf("Try again!\n");
			clear_stdin();
			continue;
		} else {
			break;
		}
	}
	
	matrix = malloc(sizeof(int) * n * n);
	counter_of_spiral = 0;
	i = 0;
	number = 1;
	while (1) {
		for (j = counter_of_spiral; j < n - counter_of_spiral; j++) {
			*(matrix + i * n + j) = number;
			number++;
		}
		j--;
		if ((i == j) && (i == counter_of_spiral)) {
			break;
		}
		for (i = counter_of_spiral + 1; i < n - counter_of_spiral; i++) {
			*(matrix + i * n + j) = number;
			number++;
		}
		i--;
		for (j = n - 2 - counter_of_spiral; j >= counter_of_spiral; j--) {
			*(matrix + i * n + j) = number;
			number++;
		}
		j++;
		counter_of_spiral++;
		for (i = n - 1 - counter_of_spiral; i >= counter_of_spiral; i--) {
			*(matrix + i * n + j) = number;
			number++;
		}
		i++;
	}
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			printf("%5d", matrix[i * n + j]);
		}
		printf("\n");
	}

}

int main() {
	int action_num;
	system("clear");
	while (1) {
		printf("*****************************************\n"
			"Action number:\n"
			"1 - inference of a matrix in a loop\n"
			"2 - outputting the array in reverse order\n"
			"3 - triangles of 1 and 0\n"
			"4 - filling the matrix in a spiral\n"
			"5 - exit\n"
			"*****************************************\n");
		printf("Please, enter action number: ");
		if (scanf("%d", &action_num) != 1) {
			printf("You entered not integer type! Try again!\n");
			clear_stdin();
			printf("Wait 2 sec...\n");
			sleep(2);
			system("clear");
			continue;
		}
		switch(action_num) {
		case 1:
			first();
			clear_stdin();
			printf("*********\n"
				"Press enter\n");
			getchar();
			system("clear");
			break;
		case 2:
			second();
			clear_stdin();
			printf("*********\n"
				"Press enter\n");
			getchar();
			system("clear");
			break;
		case 3:
			third();
			clear_stdin();
			printf("*********\n"
				"Press enter\n");
			getchar();
			system("clear");
			break;
		case 4:
			fourth();
			clear_stdin();
			printf("*********\n"
				"Press enter\n");
			getchar();
			system("clear");
			break;
		case 5:
			system("clear");
			return 0;
		default:
			printf("You entered incorrect action number. Try again!\n");
			printf("Wait 2 sec...\n");
			sleep(2);
			system("clear");
			break;
		}
	}
	
	return 0;
}