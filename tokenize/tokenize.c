#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>
#include <string.h>

int NTHREADS;
sem_t * sems;
const int MAX = 1000;

void * tokenize(void * id)
{
	int rank = *(int *) id;
	int count;
	int next = (rank + 1 ) % NTHREADS;
	char * fgrv;
	char myLine[MAX];
	char * myString;
	sem_wait(&sems[rank]);
	fgrv = fgets(myLine, MAX, stdin);
	sem_post(&sems[next]);
	char * p;
	while(fgrv != NULL)
	{
		printf("Thread %d > my line = %s\n" , rank, myLine);

		count = 0;
		myString = strtok_r(myLine, " \t\n", &p);
		while(myString != NULL)
		{
			count++;
			printf("Thread %d > string %d = %s\n", rank, count, myString);
			myString = strtok_r(NULL, " \t\n", &p);
		}
		sem_wait(&sems[rank]);
		fgrv = fgets(myLine, MAX, stdin);
		sem_post(&sems[next]);
	}
	return NULL;
}
int main(int argc, char ** argv) 
{
	if (argc < 2) printf("Arguments needed: threads_count.");
	NTHREADS = atoi(argv[1]);

	pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * NTHREADS);
	sems = (sem_t *) malloc(sizeof(sem_t) * NTHREADS);
	
	int i;

	sem_init(&sems[0], 0, 1);
	for (i = 1; i < NTHREADS; i++)
	{
		sem_init(&sems[i], 0, 0);
	}
	for (i = 0; i < NTHREADS; i++)
	{
		pthread_create(&threads[i], NULL, tokenize, (void *) &i);
	}
	for (i = 0; i < NTHREADS; i++)
	{
		pthread_join(threads[i], NULL);
	}
	free(threads);
	return 0;
}
