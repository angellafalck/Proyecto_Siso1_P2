#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

#define THREADS 1

sem_t OKToBuyMilk;
int milkAvailable;

void* buyer(void *arg)
{
    
    sem_wait(&OKToBuyMilk);//lock thread
    
    if(milkAvailable>1){
        printf("There is too much milk\n");
    }
    else if(milkAvailable==1) //enough milk
    {
        printf("There is already enough milk\n");
    }else if(milkAvailable<1)
    {
        printf("There is not enough milk, buy milk\n");
    }else if(milkAvailable<2)
    {
        // Buy some milk
        milkAvailable++;
    } 
   
    sem_post(&OKToBuyMilk); //unlock thread

    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t threads[THREADS];

    milkAvailable = 3;

    /*Inicializar el semaforo en 1, semaforo between threads
    y no procesos*/
    if(sem_init(&OKToBuyMilk, 0, 1))
    {
        //printf("Could not initialize a semaphore\n");
        return -1;
    }

    //Crear threads
    for(int i = 0; i < THREADS; ++i)
    {
        if(pthread_create(&threads[i], NULL, &buyer, NULL))
        {
            //printf("Could not create thread %d\n", i);
            return -1;
        }
    }

    //Join threads
    for(int i = 0; i < THREADS; ++i)
    {
        if(pthread_join(threads[i], NULL))
        {
            //printf("Could not join thread %d\n", i);
            return -1;
        }
    }

    sem_destroy(&OKToBuyMilk);

    // Make sure we don't have too much milk.
    printf("Total milk: %d\n", milkAvailable);

    return 0;
}
