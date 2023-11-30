#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define CANT_HILOS 3

sem_t sem_a, sem_b, sem_c;


void *print_a(void *arg) {
    while (1) {
        sem_wait(&sem_a);
        printf("A");
        sem_post(&sem_b);
    }
    return NULL;
}

void *print_b(void *arg) {
    while (1) {
        sem_wait(&sem_b);
        printf("B");
        sem_post(&sem_c);       
    }
    return NULL;
}


void *print_c(void *arg) {
    while (1) {
        sem_wait(&sem_c);
        sem_post(&sem_a);
        sem_wait(&sem_c);
        printf("C ");
        sem_post(&sem_a);
    }
    return NULL;
}


int main(){
	pthread_t hilos[CANT_HILOS];
    
    // Inicialización de semáforos
    sem_init(&sem_a, 0, 1);
    sem_init(&sem_b, 0, 0);
    sem_init(&sem_c, 0, 0);
    
    // Creación de hilos
    pthread_create(&hilos[0], NULL, print_a, NULL);
    pthread_create(&hilos[1], NULL, print_b, NULL);
    pthread_create(&hilos[2], NULL, print_c, NULL);
    
    // Espera indefinida
    for(int i=0; i<CANT_HILOS; i++){
        pthread_join(hilos[i], NULL);
    }

    return 0;
}
