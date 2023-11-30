#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>

#define CANT_ALUMNOS 25
#define CANT_HORAS 12
#define RESERVADO 1
#define LIBRE 0


struct hora{
	int reservado;
	int numAlumno;
};


struct hora buffer[CANT_HORAS];

pthread_mutex_t mutexCons = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexResCan = PTHREAD_MUTEX_INITIALIZER;
sem_t semCantCons;

int horaRandom(){
	int hora = rand() % 12;
	return hora;
}

int getPrimeraHoraDeMisRevervados(int idA){
	int aRetornar = -1;
	for(int i=0; i<CANT_HORAS; i++){
		if(buffer[i].numAlumno == idA){
			aRetornar = i;
		}
	}
	return aRetornar;	
}

int obtenerHora(int hora){
	return hora + 9;
}

int reservar(int idA){
	int h = horaRandom();
	pthread_mutex_lock(&mutexResCan);
	printf("El alumno %d quiere reservar.\n", idA);
	if(buffer[h].reservado == RESERVADO){
		printf("No es posible reservar a las %d horas por el alumno %d, no se encuentra disponible. \n", obtenerHora(h), idA);
	}else{
		buffer[h].reservado = RESERVADO;
		buffer[h].numAlumno = (int) idA;
		printf("Se ha reservado con exito a las %d hs, por el alumno %d\n", obtenerHora(h), idA);
		sleep(1);
	}
	pthread_mutex_unlock(&mutexResCan);
	return h;
}

void cancelar(int idA){
	pthread_mutex_lock(&mutexResCan);
	int miPrimeraReserva = getPrimeraHoraDeMisRevervados(idA);
	printf("El alumno %d quiere cancelar.\n", idA);
	if(miPrimeraReserva != -1){
		buffer[miPrimeraReserva].reservado = LIBRE;
		buffer[miPrimeraReserva].numAlumno = -1;
		printf("Se ha cancelado la reserva de las %dhs por el alumno %d\n", obtenerHora(miPrimeraReserva), idA);
		sleep(1);
	} else {
		printf("No es posible cancelar ya que el alumno %d no contaba con reservas.\n",idA);
	}
	pthread_mutex_unlock(&mutexResCan);
}

void consultar(int idA){
	pthread_mutex_lock(&mutexCons);
	if(sem_trywait(&semCantCons)!=0) {
		pthread_mutex_lock(&mutexResCan);
	} else {
		sem_post(&semCantCons);
	}
	pthread_mutex_unlock(&mutexCons);
	sem_post(&semCantCons);
	printf("Alumno %d quiere consultar.\n", idA);
	int h = horaRandom();
	if(buffer[h].reservado == RESERVADO){
		printf("La hora %dhs consultada por el alumno %d se encuentra reservada.\n", obtenerHora(h), idA);
	} else {
		printf("La hora %dhs consultada por el alumno %d no se encuentra reservada.\n", obtenerHora(h), idA);
	}
	sem_wait(&semCantCons);
	pthread_mutex_lock(&mutexCons);
	if(sem_trywait(&semCantCons) != 0) {
		pthread_mutex_unlock(&mutexResCan);
	} else{
		sem_post(&semCantCons);
	}
	pthread_mutex_unlock(&mutexCons);
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

void * operacion(void* idA){
	int *alumnoID = (int *)idA;
	while(1){
		for(int i=0; i<4; i++){
			operacionAleatoria(*alumnoID);
		}
		sleep(5);
	}
}

void inicializarBuffer(){
	for(int i=0; i<CANT_HORAS; i++){
		buffer[i].numAlumno = -1;     // Nadie reservo
		buffer[i].reservado = LIBRE;  // LIBRE por defecto
	}
}

int main(){
	srand(time(NULL));
	pthread_t alumnos[CANT_ALUMNOS];
	inicializarBuffer();
	sem_init(&semCantCons, 0, 0);
	int aux[CANT_ALUMNOS];
	
	for(int i=0; i<CANT_ALUMNOS; i++) {
		aux[i] = i;
		pthread_create(&alumnos[i], NULL, operacion, &aux[i]);
	}
	
	for(int i=0; i<CANT_ALUMNOS; i++) {
		pthread_join(alumnos[i], NULL);
    }
    sem_destroy(&semCantCons);
    pthread_mutex_destroy(&mutexCons);
    pthread_mutex_destroy(&mutexResCan);
}
