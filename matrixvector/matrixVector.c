#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>

#define BILLION 1E9
#define NUM_ROWS 16
#define NUM_COLUMNS 16

int NTHREADS;
int i, j;
int matrix[NUM_ROWS][NUM_COLUMNS]; 
int vector[NUM_COLUMNS]; 
int matResult[NUM_ROWS];

void fillMatrixVector()
{
    srand(time(NULL));
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }
    for (i = 0; i < NUM_COLUMNS; i++)
    {
        vector[i] = rand() % 10;
    }
}
void showMatrixVector()
{
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            printf("%i ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    for (i = 0; i < NUM_COLUMNS; i++)
    {
        printf("%i ", vector[i]);
    }   
    printf("\n");
}
void showResult()
{
    for (i = 0; i < NUM_COLUMNS; i++)
    {
        printf("%i ", matResult[i]);
    }   
    printf("\n");
}
void * matrixVector(void * id)
{
    int rank = *(int *) id;
    int i, j;
    int portion = NUM_ROWS/NTHREADS;
    int lowBound = portion * rank;
    int upperBound = lowBound + portion;
    //printf("My lb is %i  and my up is %i \n", lowBound, upperBound-1);
    for (i = lowBound; i < upperBound; i++)
    {
        matResult[i] = 10;
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            matResult[i] += matrix[i][j] * vector[j]; 
        }
    }
    return NULL;
}

int main(int argc, char ** argv)
{
    if (argc < 2) printf("Arguments needed: threads_count.");
    NTHREADS = atoi(argv[1]);

    pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t) * NTHREADS);
    
    fillMatrixVector();
    //showMatrixVector();
    struct timespec t1, t2;
    clock_gettime(CLOCK_REALTIME, &t1);
    for(i = 0; i < NTHREADS; i++)
    {
        pthread_create(&threads[i], NULL, matrixVector, (void *) &i);
    }
    for(i = 0; i < NTHREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_REALTIME, &t2);
    double time = ( t2.tv_sec - t1.tv_sec ) + ( t2.tv_nsec - t1.tv_nsec ) / BILLION;
    printf( "%lf\n", (time / NTHREADS) * 1000);
    //showResult();
    free(threads);
    return 0;
}
