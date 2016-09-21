#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BILLION 1E9

int i;
int initialSize;
int NTHREADS;
int operations;
int opPerThread;
int inserts;
int members;
int removes;

pthread_rwlock_t  mutex = PTHREAD_RWLOCK_INITIALIZER;

struct node
{
	int data;
	struct node * next;
	pthread_mutex_t mutex;
} * head;

int catchInput(int argc, char ** argv)
{
	if( argc == 7)
	{
		initialSize = atoi(argv[1]);
		NTHREADS = atoi(argv[2]);
		operations = atoi(argv[3]);
		opPerThread = operations / NTHREADS;
		inserts = atoi(argv[4]) * opPerThread / 100;
		members = atoi(argv[5]) * opPerThread / 100;
		removes = atoi(argv[6]) * opPerThread / 100;
		return 1;
	}
	else
	{
		return 0;
	}
}

int member(int value, struct node ** &p)
{
	for (p = &head; *p && (*p)->data < value; p = &((*p)->next));
	return *p && ((*p)->data) == value;
}
int insert(int value)
{
	node ** p;
	if(member(value, p)) return 0;
	node * temp = *p;
	*p = new node;
	(*p)->data = value;
	(*p)->next = temp;
	return 1;
}
int remove(int value)
{
	node ** p;
	if (!member(value, p)) return 0;
	node * temp = *p;
	(*p) = (*p)->next;
	free(temp);
	return 1;	
}
void * operate(void * rank)
{
	int id  = *(int *) rank;
	srand(time(NULL));
	node ** p;
	for(i = 0; i < opPerThread; i++)
	{
		if(i < inserts)
		{
            pthread_rwlock_wrlock(&mutex);
            insert(rand() % 100);
            pthread_rwlock_unlock(&mutex);
		}
		else if (i < inserts + members)
		{
			pthread_rwlock_rdlock(&mutex);
			member(rand() % 100, p);
			pthread_rwlock_unlock(&mutex);
		}
		else
		{
			pthread_rwlock_wrlock(&mutex);
			remove(rand() % 100);
			pthread_rwlock_unlock(&mutex);
		}
	}
	return NULL;
}
int main(int argc, char ** argv)
{
	if (catchInput(argc, argv))
	{
		struct timespec t1, t2;
		srand(time(NULL));
		for (i = 0; i < initialSize; i++)
		{
			insert(rand() % 100);
		}
		pthread_t * threads = (pthread_t *)malloc(sizeof(pthread_t) * NTHREADS);

    	clock_gettime(CLOCK_REALTIME, &t1);
		for (i = 0; i < NTHREADS; i++)
		{
			pthread_create(&threads[i], NULL, operate, (void *) &i);
		}
		for (i = 0; i < NTHREADS; i++)
		{
			pthread_join(threads[i], NULL);
		}
		clock_gettime(CLOCK_REALTIME, &t2);
		double time = ( t2.tv_sec - t1.tv_sec ) + ( t2.tv_nsec - t1.tv_nsec ) / BILLION;
		printf( "%lf\n", (time / NTHREADS) * 1000);
		
		free(threads);	
	}
	else
	{
		printf("initialSize NTHREADS operations insert members removes\n");
	}
	return 0;
}