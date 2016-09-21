#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREAD 2

/*int initialSize;
int NTHREADS;
int operations;
int opPerThread;
int inserts;
int members;
int deletes;
*/
pthread_mutex_t headMutex = PTHREAD_MUTEX_INITIALIZER;

struct node
{
	int data;
	struct node * next;
	pthread_mutex_t mutex;
} * head;

/*int catchInput(int argc, char ** argv)
{
	if( argc == 7)
	{
		initialSize = atoi(argv[1]);
		NTHREADS = atoi(argv[2]);
		operations = atoi(argv[3]);
		opPerThread = operations / NTHREADS;
		inserts = atoi(argv[4]) * opPerThread / 100;
		members = atoi(argv[5]) * opPerThread / 100;
		deletes = atoi(argv[6]) * opPerThread / 100;
		return 1;
	}
	else
	{
		return 0;
	}
}
*/
int member(int value, node *&prev, node ** &p)
{
	while(*p && (*p)->data < value)
	{
		pthread_mutex_lock(&(*p)->mutex);
		if(prev)
		{
			if (prev == head) pthread_mutex_unlock(&headMutex);
			pthread_mutex_unlock(&prev->mutex);
		}
		prev = *p;
		p = &((*p)->next);
	}
	return (*p) && ((*p)->data) == value;
}


void * insert(void * v)
{
	pthread_mutex_lock(&headMutex);
	bool headBlocked = 0;
	int value = *(int *) v;
	node ** p = &head, * prev = NULL;
	if (!member(value, prev, p))
	{
		if ((*p) == head || prev == head) headBlocked = 1;//return 0;
		node * temp = new node;
		temp->data = value;
		temp->next = *p;
		*p = temp;
		printf("%d\n", value);
		node *q = head;
		while(q){
			printf("%d ", q->data);
			q=q->next;	
		}
		printf("\n" );
		pthread_mutex_unlock(&(*p)->mutex);
		if(headBlocked)
		{
			pthread_mutex_unlock(&headMutex);
			if(prev) pthread_mutex_unlock(&prev->mutex);
		}
	}

	//return 1;
}
/*int delet(int value)
{
	struct node ** p;
	if (!member(value, p)) return 0;
	struct node * temp = (*p);
	(*p) = (*p)->next;
	pthread_mutex_unlock(&(*p)->mutex);
	free(temp);
	return 1;	
}*/
int main(int argc, char ** argv)
{
	/*if (catchInput(argc, argv) == 1)
	{
		
	}*/
	int i;
	pthread_t threads[NTHREAD];

	for(i = 0; i < NTHREAD; i++)
	{
		pthread_create(&threads[i], NULL, insert, (void *) &i);
	}
	for(i = 0; i < NTHREAD; i++)
	{
		pthread_join(threads[i], NULL);
	}

	/*node *p = head;
	for (int i = 0; i < 8; i++)
	{
		printf("%d\n", p->data);
		p = p->next;
	}*/
	return 0;
}