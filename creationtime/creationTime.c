#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define BILLION  1E9
int NTHREADS;

void * foo(void* rank)
{
    return NULL;
}

int main(int argc, char ** argv)
{
    
    if (argc < 2) printf("Arguments needed: threads_count.");
    NTHREADS = atoi(argv[1]);

    pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t) * NTHREADS);

    struct timespec t1, t2;
    clock_gettime(CLOCK_REALTIME, &t1);
    int i;
    printf("%d\n", NTHREADS);
    for (i = 0; i < NTHREADS; i++)
    {
        pthread_create(&threads[i], NULL, foo, (void *) &i);
    }

    free(threads);

    clock_gettime(CLOCK_REALTIME, &t2);

    double time = ( t2.tv_sec - t1.tv_sec ) + ( t2.tv_nsec - t1.tv_nsec ) / BILLION;
    printf( "%lf\n", (time / NTHREADS) * 1000);

    return 0;
}