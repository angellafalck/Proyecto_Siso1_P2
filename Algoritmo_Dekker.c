#include "string.h"
#include "pthread.h"
#include "stdio.h"

//las flags son coordenadas, turn indica el turno
int turn=0, flag[2]={0,0}, balance=0;


void test(int i){

	int j,k,m,p,q,r,c;
	j=1-i;

	for(k=0;k<3;k++){
		
		flag[i]=1;

		while(flag[j]){
			if (turn==j){
				flag[i]=0;
					printf(" Esperando: %d",i);
				while(turn==j);
				flag[i]=1;
			}

		}

		printf("\nEntrando a sección critica %d",i);
		c=balance;
		/*critical section*/
		printf("\n proceso: %d",i);
		printf(" -> flag[%d]=%d, flag[%d]=%d",i,flag[i],j,flag[j]);
		printf(" turno de= %d\n",turn);
		balance=c+1000; //tiempo
		printf(" balance=  %d",balance);
		turn=j;
		flag[i]=0;
		printf("\nSaliendo.. %d",i);
	}

}

int main(){

	pthread_t t1,t2;
	pthread_create(&t1,NULL,(void*)&test,(void*)0);
	printf("Crear el primer thread..\n");
	pthread_create(&t2,NULL,(void*)&test,(void*)1);
	printf("Crear el segundo thread..\n");
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	printf("\nTerminado: %d",balance);

	return(0);

}