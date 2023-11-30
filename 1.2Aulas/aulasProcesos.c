#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>

#define CANT_ALUMNOS 25
#define CANT_HORAS 12
#define RESERVADO 1
#define LIBRE 0

struct hora{
	int reservado;
	int numAlumno;
};

struct memComp{
	struct hora buffer[CANT_HORAS];
	sem_t semResCan;
	pthread_mutex_t mutexCons;
	sem_t semCantCons;
};

#define KEY ((key_t)(11223344))
#define MEM_SIZE sizeof(struct memComp)

struct memComp *obtenerMemoriaCompartida() {
    struct memComp *mem;
    // Obtener identificador de memoria compartida.
    int id = shmget(KEY, MEM_SIZE, 0);
    if (id < 0) {
        printf("Error en el shmget\n");
        exit(1);
    }
    // Mapear memoria compartida.
    mem = (struct memComp *)shmat(id, 0, 0);
    if (mem == (void *)-1) {
        printf("Error en el shmat\n");
        exit(2);
    }
    return mem;
}

int getPrimeraHoraDeMisRevervados(int idA){
	struct memComp *mem = obtenerMemoriaCompartida();
	int aRetornar = -1;
	for(int i=0; i<CANT_HORAS; i++){
		if(mem->buffer[i].numAlumno == idA){
			aRetornar = i;
		}
	}
	return aRetornar;	
}

int horaRandom(){
	int hora = rand() % 12;
	return hora;
}

int obtenerHora(int hora){
	return hora + 9;
}

int reservar(int idA){
	struct memComp *mem = obtenerMemoriaCompartida();
	int h = horaRandom();
	sem_wait(&mem->semResCan);
	printf("EL alumno %d quiere reservar.\n", idA);
	if(mem->buffer[h].reservado == RESERVADO){
		printf("No es posible reservar a las %d horas por el alumno %d, no se encuentra disponible. \n", obtenerHora(h), idA);
	}else{
		mem->buffer[h].reservado = RESERVADO;
		mem->buffer[h].numAlumno = (int) idA;
		printf("Se ha reservado con exito a las %d hs, por el alumno %d\n", obtenerHora(h), idA);
		sleep(1);
	}
	sem_post(&mem->semResCan);
	
	return h;
}

void cancelar(int idA){
	struct memComp *mem = obtenerMemoriaCompartida();
	sem_wait(&mem->semResCan);
	int miPrimeraReserva = -1;
	for(int i=0; i<CANT_HORAS; i++){
		if(mem->buffer[i].numAlumno == idA){
			miPrimeraReserva = i;
		}
	}
	printf("El alumno %d quiere cancelar.\n", idA);
	if(miPrimeraReserva != -1){
		mem->buffer[miPrimeraReserva].reservado = LIBRE;
		mem->buffer[miPrimeraReserva].numAlumno = -1;
		printf("Se ha cancelado la reserva de las %dhs por el alumno %d\n", obtenerHora(miPrimeraReserva), idA);
		sleep(1);
	} else {
		printf("No es posible cancelar ya que el alumno %d no contaba con reservas.\n",idA);
	}
	sem_post(&mem->semResCan);
}

void consultar(int idA){
	struct memComp *mem = obtenerMemoriaCompartida();
	pthread_mutex_lock(&mem->mutexCons);
	if(sem_trywait(&mem->semCantCons)!=0) {
		sem_wait(&mem->semResCan);
	} else {
		sem_post(&mem->semCantCons);
	}
	sem_post(&mem->semCantCons);
	pthread_mutex_unlock(&mem->mutexCons);
	printf("Alumno %d quiere consultar.\n", idA);
	int h = horaRandom();
	if(mem->buffer[h].reservado == RESERVADO){
		printf("La hora %dhs consultada por el alumno %d se encuentra reservada.\n", obtenerHora(h), idA);
	} else {
		printf("La hora %dhs consultada por el alumno %d no se encuentra reservada.\n", obtenerHora(h), idA);
	}
	sem_wait(&mem->semCantCons);
	pthread_mutex_lock(&mem->mutexCons);
	if(sem_trywait(&mem->semCantCons) != 0) {
		sem_post(&mem->semResCan);
	}else{
		sem_post(&mem->semCantCons);
	}
	pthread_mutex_unlock(&mem->mutexCons);
}

void operacionAleatoria(int idA){
	int operacion = rand() % 4;
	switch(operacion) {
	    case 0: reservar(idA); break;
	    case 1: reservar(idA); break;
	    case 2: cancelar(idA); break;
		case 3: consultar(idA);break;
	}
}

void * operacion(int idA){
	while(1){
		for(int i=0; i<4; i++){
			operacionAleatoria(idA);
		}
		sleep(5);
	}
}

int main(){
	
	// CREO EL SEGMENTO DE MEMORIA COMPARTIDA DE LOS SEMAFOROS
	int id_mem;
	//Crear u obtener un nuevo segmento de memoria, como no existe, lo creamos. 
	id_mem = shmget (KEY, MEM_SIZE, IPC_CREAT | 0666); 
	if (id_mem == -1){
		printf("No pude obtener el segmento de memoria \n");
		exit (1);
	}

	struct memComp* memoriaCompartida = NULL;
	// Mapeamos un segmento de memoria.
	memoriaCompartida = (struct memComp*) shmat (id_mem, 0, 0); 

	if (memoriaCompartida == NULL){
		printf("No se pudo asignar la memoria compartida \n");
		exit (1);
	}
	
	srand(time(NULL));
	
	//Inicializacion del buffer.
	for(int i=0; i<CANT_HORAS; i++){
		memoriaCompartida->buffer[i].numAlumno = -1; // No hay reserva
		memoriaCompartida->buffer[i].reservado = LIBRE;
	}
	//Inicializacion de los semaforos y mutex.
	sem_init(&memoriaCompartida->semCantCons, 1, 0);
	sem_init(&memoriaCompartida->semResCan, 1, 1);
	pthread_mutex_init(&memoriaCompartida->mutexCons,NULL);
		    	
	//Creacion de los procesos
	pid_t id;
	for (int i = 0; i < CANT_ALUMNOS; i++) {
	    id = fork();
	    if (id == 0) {
			operacion(i);
	    } else if (id < 0) {
			perror("Error en fork");
			exit(1);
	    }
	}

	//Wait de los procesos
	for(int i=0; i<CANT_ALUMNOS; i++){
		wait(NULL);
	}
	
	//Limpio semaforos
	sem_destroy(&memoriaCompartida->semCantCons);
    pthread_mutex_destroy(&memoriaCompartida->mutexCons);
	sem_destroy(&memoriaCompartida->semResCan);
	
	
	//Limpio memoria compartida.
	shmdt(memoriaCompartida);
	shmctl(id_mem, IPC_RMID, NULL);

}
