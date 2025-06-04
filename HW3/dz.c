#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int *arr;          
int N;            
int sum = 0;        
pthread_mutex_t lock; 


typedef struct {
    int start;
    int end;
} ThreadData;

void *worker(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int partial_sum = 0;
    for (int i = data->start; i < data->end; i++) {
        partial_sum += arr[i];
    }

    pthread_mutex_lock(&lock);
    sum += partial_sum;
    pthread_mutex_unlock(&lock);

    free(data);
    return NULL;
}

int main() {
    printf("Enter N (>=16): ");
    scanf("%d", &N);
    if (N < 16) {
        printf("N must be at least 16.\n");
        return 1;
    }

    arr = (int *)malloc(N * sizeof(int));
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 100; 
    }

    printf("Generated array:\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    pthread_t threads[4];
    pthread_mutex_init(&lock, NULL);

    int part_size = N / 4;
    for (int i = 0; i < 4; i++) {
        ThreadData *data = (ThreadData *)malloc(sizeof(ThreadData));
        data->start = i * part_size;
        if (i == 3)
            data->end = N;  
        else
            data->end = (i + 1) * part_size;

        pthread_create(&threads[i], NULL, worker, data);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    free(arr);

    printf("Total sum: %d\n", sum);
    return 0;
}
