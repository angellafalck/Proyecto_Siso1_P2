#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>

sem_t a;
sem_t b;
sem_t c;

int buffer = 0; //variable global contador del buffer
int buffersize = 7; //tamaño del buffer

void * productor()
{
	int countdown = 10;

	while(countdown > 0)
	{
		countdown--;
		
		// Producir
		printf("Producir -> Buffer en %d\n", buffer);

		sem_wait(&c);//lock semaphore buffer (produzco)
		sem_wait(&a);//lock semaphore 
		
		// Agregar
		buffer++; //sumo al buffer
		printf("Agregar ->  Buffer en %d\n", buffer);
		sem_post(&a);;//unlock semaphore
		sem_post(&b);;//unlock semaphore

		sleep(3);
	}
}

void * consumidor()
{
    int m;
	int countdown = 10;

	while(countdown > 0)
	{
		sem_wait(&b);//lock semaphore 
		sem_wait(&a);//lock semaphore 

		// Agarrar
		buffer--;
		printf("Agarrar -> Buffer en %d\n",buffer);
		sem_post(&a);;//unlock semaphore
		sem_post(&c);;//unlock semaphore
		sleep(4);
		
		// Consumir
		printf("Consumir -> Buffer en %d\n",buffer);
		sleep(3);
	}

}

int main()
{
    //Inicializar ambos threads
	pthread_t thread1;
	pthread_t thread2;

    //Solución realizada con semaforos
    //0 por que es compartido entre procesos
    /*
        previendo tener un conflicto debido a tener multiples productores,
        de esta manera evitamos que los 2 intenten escribir al mismo tiempo o insertar el objeto 
        en el mismo espacio.
    */
	sem_init(&a,0,1);//binary semaphore
	sem_init(&b,0,0);//binary semaphore 
	sem_init(&c,0,buffersize); /*counting semaphore avisa cuando pueden sumar o restar y validar
     que no se agreguen en un mismo espacio*/

    //Crear los threads de productor y consumidor
	pthread_create(&thread1,NULL,productor,NULL);
	pthread_create(&thread2,NULL,consumidor,NULL);

	sleep(60);

}