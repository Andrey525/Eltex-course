#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4
#define MAX 1000000

int A = 0;

pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int thread_num;
} PthreadData;

void *increment(void *inputData) {
    PthreadData *data = (PthreadData *)inputData;
    int i, tmp;
    for (i = 0; i < MAX / NTHREADS; i++) {
        pthread_mutex_lock(&MUTEX);
        tmp = A;
        tmp++;
        A = tmp;
        pthread_mutex_unlock(&MUTEX);
    }
    printf("Thread num %d calculate data = %d\n", data->thread_num, A);
    return 0;
}

int main() {
    pthread_t *thread;
    PthreadData *data;
    int status;
    int i;

    thread = malloc(sizeof(*thread) * NTHREADS);
    data = malloc(sizeof(*data) * NTHREADS);

    for (i = 0; i < NTHREADS; i++) {
        data[i].thread_num = i + 1;
        status = pthread_create(&thread[i], NULL, increment, &data[i]);
        if (status != 0) {
            printf("main error: can't create thread, status = %d\n", status);
            exit(-1);
        }
    }

    for (i = 0; i < NTHREADS; i++) {
        status = pthread_join(thread[i], NULL);
        if (status != 0) {
            printf("main error: can't join thread, status = %d\n", status);
            exit(-2);
        }
    }
    printf("Rezult A = %d\n", A);

    return 0;
}