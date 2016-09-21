#include <pthread.h>
#include <cstdlib>
#include <cstdio>

struct node {
    int data;
    struct node* next;
} * head = NULL;

enum state {
    SLEEP,
    AWAKEN
};

const int MEMBER_OP = 0;
const int INSERT_OP = 1;
const int REMOVE_OP = 2;


enum state * states;
pthread_t * threads;

pthread_rwlock_t mutex;
pthread_rwlock_t statem;

int initialSize;
int NTHREADS;
int NTASKS;
int completedTasks;
int availableTasks;
int availableThread;
int i;

#define enum thread_state thread_state


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
    printf("hola state[i] %d\n", states[id]);
    srand(time(NULL));
    node ** p;
    int op = rand() % 3;
    switch(op)
    {
        case 0:
        {
            pthread_rwlock_wrlock(&mutex);
            insert(rand() % 100);
            pthread_rwlock_unlock(&mutex);
            break;
        }
        case 1:
        {
            pthread_rwlock_rdlock(&mutex);
            member(rand() % 100, p);
            pthread_rwlock_unlock(&mutex);
            break;
        }
        case 2:
        {
            pthread_rwlock_wrlock(&mutex);
            remove(rand() % 100);
            pthread_rwlock_unlock(&mutex);
            break;
        }
    }
    printf("Going to sleep %d\n", id);
    pthread_rwlock_wrlock(&statem);
    states[id] = SLEEP;
    pthread_rwlock_unlock(&statem);
    return NULL;
}

void executetask() {
    pthread_create(&threads[availableThread], NULL, operate, (void *) &availableThread);
    states[availableThread] = AWAKEN;
    availableThread = -1;
}

bool threads_available() {
    printf("availableThread %d\n", availableThread);
    if (availableThread != -1) return 1;

    for (i = 0; i < NTHREADS; i++)
    {
        printf("%d \t", states[i]);
    }
    printf("\n");

    for (i = 0; i < NTHREADS; i++) {
            printf("cuernos\n");
        pthread_rwlock_rdlock(&statem);
        if (states[i] == SLEEP) 
        {
            availableThread = i;
            return 1;
        }
        pthread_rwlock_unlock(&statem);
    }
    return 0;
}

void try_generate_tasks()
{
    if (completedTasks >= NTASKS || availableTasks + completedTasks >= NTASKS) return;
    //srand(time(NULL));
    int tasks;
    int rest = NTASKS - completedTasks;
    availableTasks = rand() % (rest + 1);
    //if ((tasks = rand() % 100) < 80) availableTasks += tasks % (NTASKS - completedTasks - 1) + 1;
    //printf("availableTasks %d\n", availableTasks);
    //printf("Tasks completed %i\n", completedTasks);
}

bool catchInput(int argc, char **argv)
{
    if (argc < 4) return 0;
    initialSize = atoi(argv[1]);
    NTHREADS = atoi(argv[2]);
    NTASKS = atoi(argv[3]);
    completedTasks = 0;
    availableTasks = 0;
    availableThread = -1;
    return 1;
}



int main(int argc, char** argv)
{


    if (catchInput(argc, argv))
    {
        pthread_rwlock_init(&mutex, NULL);
        pthread_rwlock_init(&statem, NULL);


        states = (state*) malloc(NTHREADS * sizeof(state));
        for (i = 0; i < NTHREADS; i++)
            states[i] = SLEEP;

        threads = (pthread_t*) malloc(NTHREADS * sizeof(pthread_t));
        int cont = 0, vv = 0;
        while (completedTasks < NTASKS && vv < 20) {
            printf("availableTasks %d , availableThread %d \n",availableTasks,availableThread );
            while ((availableTasks == 0 || !threads_available())  && cont < 20) {
                try_generate_tasks();
                printf("availableTasks %d , availableThread %d \n",availableTasks,availableThread );
                cont ++;
                printf("after\n");
            }
            for (; availableTasks > 0 && threads_available(); availableTasks--, completedTasks++) {
                printf("before\n");
                pthread_create(&threads[availableThread], NULL, operate, (void *) &availableThread);
                //executetask();
            }
            printf("completedTasks %d\n", completedTasks);
            vv++;
        }

        pthread_rwlock_destroy(&mutex);
        pthread_rwlock_destroy(&statem);
                
        free(threads);
        free(states);  
    }
    else
    {
        printf("initialSize NTHREADS NTASKS\n");
    }
    return 0;
}