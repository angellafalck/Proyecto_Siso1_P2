    #include "pthread.h" //Libreria Thread
    #include "stdio.h"
    #include "string.h"
    
    #define Q_THREADS 10 //cantidad de threads, dependiendo el valor a considerar 1,2 o n threads
    //std::thread thread_object(callable);
    int num[Q_THREADS];
    int selecting[Q_THREADS];
    int resource;

    void lock_thread(int threads)
    {
        //Antes de agarrar el ticket, selecting es true (1)
        selecting[threads] = 1;
        int tkt_max = 0;
        // Encontrar el valor maximo de tickets en los threads
        for (int i = 0; i < Q_THREADS; i++)
        {
            int ticket = num[i];
            tkt_max = ticket > tkt_max ? ticket : tkt_max;
        }
        // Asignando nuevos valores como maximo+1 (valor maximo encontrado)
        num[threads] = tkt_max + 1;
        selecting[threads] = 0;

        //Comienza la sección de entrada
        for (int j = 0; j < Q_THREADS; j++)
        {
            // Aplicar las condiciones del algoritmo
            while (selecting[j])
            {
            }
            while (num[j] != 0 && (num[j] < num[threads] || (num[j] == num[threads] && j < threads)))
            {
            }
        }
    }
    //Termina la sección
    void unlock_thread(int threads)
    {
        //printf("El recurso lo tiene %d, pero esta en uso por: %d!\n", threads, resource);
        num[threads] = 0; //setear a 0 falso
    }
    // Sección critica (validando los recursos habilitados) que una persona no colisione tienendo el mismo ticket
    void use_resource(int threads)
    {

        if (resource != 0)
        {
            printf("El recurso lo tiene %d, pero esta en uso por: %d!\n", threads, resource);

        }
        resource = threads;
        printf("El thread: %d está running\n", threads);

        resource = 0;
    }

    //Simplified function to show the implementation
    void *thread_body(void *arg)
    {
        //Pasos, se bloquea, se usa el recurso y se desbloquea el thread
        long thread = (long)arg;
        lock_thread(thread);
        use_resource(thread);
        unlock_thread(thread);
        return NULL;
    }

    int main(int argc, char **argv)
    {
        //Setear memoria a 0
        memset((void *)num, 0, sizeof(num));
        memset((void *)selecting, 0, sizeof(selecting));
        resource = 0;

        // Variable thread
        pthread_t threads[Q_THREADS];

        for (int i = 0; i < Q_THREADS; ++i)
        {
            // Se crea un nuevo thread para utilizar la funcion
            pthread_create(&threads[i], NULL, &thread_body, (void *)((long)i));
            //"thread_body" como la rutina del thread (los pasos que se realizaran)
        }
        for (int i = 0; i < Q_THREADS; ++i)
        {
            // los threads se unen una vez el task haya terminado
            pthread_join(threads[i], NULL);
        }
            return 0;
    }