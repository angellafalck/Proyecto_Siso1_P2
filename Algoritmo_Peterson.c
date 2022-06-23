#include <stdio.h>
#include <pthread.h>

#define VALUE 100 //valor que desee el usuario
int flag[2];
int turn, val = 0;

//Funcion para saber que thread entro
/*
Aqui se reserva el valor para el cual cada subproceso intentara bloquear
el algoritmo, al incrementar el valor ingresado por el usuario y luego
desbloquear el subproceso.

Se ejecutara el doble de veces por que es independiente del tiempo 
y tomara algunas iteraciones para solucionar el problema.
*/
//Funcion para lock el valor de la bandera como 1
void lock_val(int other)
{
    //Esperar a que se ejecute el subproceso y cambiar el valor de la bandera
    flag[other] = 1;  //Thread quiere entrar a la seccion critica 
    turn = 1 - other; // Que otro thread tiene prioridad
    while (flag[1 - other] == 1 && turn == 1 - other);
}

//Funcion para desbloquear el valor de la bandera
//Marcar el hilo que ya no se tiene que ejecutar en la sección critica
void unlock_val(int other)
{
    flag[other] = 0; // Marking that this thread is no longer wants to enter the critical section
}


void *work(void *s)
{
    int i = 0;
    int other = (int *)s;

    printf("Thread : %d\n", other);
    lock_val(other);

    for (i = 0; i < VALUE; i++) // 
        val++;

    unlock_val(other);
}
void lock_init()
{
    // Se resetean, reseteando su preferencia a locks adquiridos.
    //esto se realiza dandoles un turno
    flag[0] = flag[1] = 0;
    turn = 0;
}


//Inicilizar valor entero en 0
//Crear los subprocesos y se unan para imprimir el valor final
void main()
{
    pthread_t t1, t2,t3;
    val = 0; // valor compartido

    lock_init();

    //Crear los threads
    pthread_create(&t1, NULL, work, (void *)0);
    pthread_create(&t2, NULL, work, (void *)1);

    //Join threads
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Printing the result
    printf("Valor final -> %d\n", val);
}



