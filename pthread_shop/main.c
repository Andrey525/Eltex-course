#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define COUNT_SHOPS 5
#define COUNT_CUSTOMERS 3

typedef struct {
    int Id;
    int TimeOfSleep;
    int LeftToBuy;
} Customer;

typedef struct {
    int TimeOfSleep;
    int Capacity;
} Supplier;

int shops[COUNT_SHOPS];
pthread_mutex_t MUTEX[COUNT_SHOPS];
pthread_mutex_t FLAGMUTEX = PTHREAD_MUTEX_INITIALIZER;
int flag = 0;

void *buy(void *pthreadData) {
    int status;
    Customer *customer = (Customer *)pthreadData;
    int i = 0;
    while (1) {
        int previousLeftToBuy = customer->LeftToBuy;
        if (pthread_mutex_trylock(&MUTEX[i]) == 0) {
            if (customer->LeftToBuy >= shops[i]) {
                customer->LeftToBuy -= shops[i];
                shops[i] = 0;
            } else {
                shops[i] -= customer->LeftToBuy;
                customer->LeftToBuy = 0;
            }
            status = pthread_mutex_unlock(&MUTEX[i]);
            if (status != 0) {
                perror("pthread_mutex_unlock");
                exit(-1);
            }
            if (customer->LeftToBuy == 0) {
                status = pthread_mutex_lock(&FLAGMUTEX);
                if (status != 0) {
                    perror("pthread_mutex_lock");
                    exit(-1);
                }
                flag++;
                status = pthread_mutex_unlock(&FLAGMUTEX);
                if (status != 0) {
                    perror("pthread_mutex_unlock");
                    exit(-1);
                }
                printf("Customer[%d]->LeftToBuy == %d\n", customer->Id,
                       customer->LeftToBuy);
                pthread_exit(0);
            }
            if (previousLeftToBuy != customer->LeftToBuy) {
                printf("Customer[%d]->LeftToBuy == %d\n", customer->Id,
                       customer->LeftToBuy);
                sleep(customer->TimeOfSleep);
            }
        }
        if (i == COUNT_SHOPS - 1) {
            i = 0;
        } else {
            i++;
        }
    }
}

void *deliver(void *pthreadData) {
    int status;
    Supplier *supplier = (Supplier *)pthreadData;
    int i = 0;
    while (1) {
        if (pthread_mutex_trylock(&MUTEX[i]) == 0) {
            shops[i] += supplier->Capacity;
            status = pthread_mutex_unlock(&MUTEX[i]);
            if (status != 0) {
                perror("pthread_mutex_unlock");
                exit(-1);
            }
            if (flag == COUNT_CUSTOMERS) {
                pthread_exit(0);
            }
            sleep(supplier->TimeOfSleep);
        }

        if (i == COUNT_SHOPS - 1) {
            i = 0;
        } else {
            i++;
        }
    }
}

int main() {
    Customer customers[COUNT_CUSTOMERS];
    Supplier supplier;
    pthread_t customerThreads[COUNT_CUSTOMERS];
    pthread_t supplierThread;
    int i;
    int status;

    srand(time(NULL));

    /* Start of init */
    for (i = 0; i < COUNT_SHOPS; i++) {
        status = pthread_mutex_init(&MUTEX[i], NULL);
        if (status != 0) {
            perror("pthread_mutex_init");
            exit(-1);
        }
        shops[i] = rand() % 200 + 900;
        printf("shops[%d] = %d\n", i, shops[i]);
    }
    for (i = 0; i < COUNT_CUSTOMERS; i++) {
        customers[i].Id = i;
        customers[i].TimeOfSleep = 3;
        customers[i].LeftToBuy = 10000 + 100 * (rand() % 10);
        printf("customers[%d].Id = %d, customers[%d].TimeOfSleep = %d, "
               "customers[%d].LeftToBuy = %d\n",
               i, customers[i].Id, i, customers[i].TimeOfSleep, i,
               customers[i].LeftToBuy);
    }
    supplier.TimeOfSleep = 2;
    supplier.Capacity = 1000;
    printf("supplier.TimeOfSleep = %d, supplier.Capacity = %d\n",
           supplier.TimeOfSleep, supplier.Capacity);
    /* End of init */

    for (i = 0; i < COUNT_CUSTOMERS; i++) {
        status = pthread_create(&customerThreads[i], NULL, buy, &customers[i]);
        if (status != 0) {
            perror("pthread_create");
            exit(-1);
        }
    }
    status = pthread_create(&supplierThread, NULL, deliver, &supplier);
    if (status != 0) {
        perror("pthread_create");
        exit(-1);
    }

    for (i = 0; i < COUNT_CUSTOMERS; i++) {
        status = pthread_join(customerThreads[i], NULL);
        if (status != 0) {
            perror("pthread_join");
            exit(-1);
        }
    }
    status = pthread_join(supplierThread, NULL);
    if (status != 0) {
        perror("pthread_join");
        exit(-1);
    }
    return 0;
}