#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define CANT_HILOS 4


sem_t sem_a, sem_b, sem_c, sem_c2, sem_d;

void *print_a() {
    while (1) {
        sem_wait(&sem_a);
        printf("A");
        sem_post(&sem_b);
    }
    return NULL;
}

void *print_b() {
    while (1) {
        sem_wait(&sem_b);
        printf("B");
        sem_post(&sem_c);
    }
    return NULL;
}

void *print_c() {
    while (1) {
        if(sem_trywait(&sem_c2) == 0) {
            sem_wait(&sem_c);
            printf("C");
            sem_post(&sem_d);
        } else {
            sem_wait(&sem_c);
            sem_post(&sem_a);
            sem_wait(&sem_c);
            printf("C");
            sem_post(&sem_a);
            sem_post(&sem_c2);
        }
    }
    return NULL;
}

void *print_d() {
    while (1) {
        sem_wait(&sem_d);
        printf("D ");
        sem_post(&sem_a);
    }
    return NULL;
}

void funcion(int op){
	switch(op) {
	    case 0: print_a(); break;
	    case 1: print_b(); break;
	    case 2: print_c(); break;
		case 3: print_d(); break;
	}
}

int main(){
    pthread_t hilos[CANT_HILOS];
    
    // Inicialización de semáforos
    sem_init(&sem_a, 0, 1);
    sem_init(&sem_b, 0, 0);
    sem_init(&sem_c, 0, 0);
    sem_init(&sem_c2, 0, 0);
    sem_init(&sem_d, 0, 0);
    
    // Creación de hilos
    pthread_create(&hilos[0], NULL, print_a, NULL);
    pthread_create(&hilos[1], NULL, print_b, NULL);
    pthread_create(&hilos[2], NULL, print_c, NULL);
    pthread_create(&hilos[3], NULL, print_d, NULL);
    
    // Espera indefinida
    for(int i=0; i<CANT_HILOS; i++){
        pthread_join(hilos[i], NULL);
    }

    return 0;
}
