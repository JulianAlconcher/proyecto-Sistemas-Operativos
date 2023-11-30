#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>

#define BUFFER_SIZE 30
#define MINI_BUFFER_SIZE 15
#define CANT_CLIENTES 76
#define CANT_EMPLEADOS 3

sem_t semPrincipalVacio;
sem_t semPoliticoVacio;
sem_t semPoliticoLleno;
sem_t semEmpresaVacio;
sem_t semEmpresaLleno;
sem_t semUsuarioVacio;
sem_t semUsuarioLleno;
sem_t semEsperarAtencionP;
sem_t semEsperarAtencionE;
sem_t semEsperarAtencionU;

int mesaDeEntrada() {
    int toReturn;
    if(sem_trywait(&semPrincipalVacio) == 0) { // Hay espacio en la mesa principal.
		toReturn = 1;
    } else { // No hay espacio en la mesa principal.
		toReturn = -1;
    }
    return toReturn;
}

void *empleadoEmpresa() {
    while(1) {
	if(sem_trywait(&semPoliticoLleno) == 0) {
		printf("POLITICO ATENDIDO:: El empleado empresa ha atendido a un cliente de tipo politico.\n");
		sem_post(&semPoliticoVacio);
		sleep(5); // Atender político.
		sem_post(&semEsperarAtencionP);
	} else {
		sem_wait(&semEmpresaLleno); // En caso de que no haya politicos, pierden la prioridad momentaneamente y se espera a una empresa.
		printf("EMPRESA ATENDIDO:: El empleado empresa ha atendido a un cliente de tipo empresa.\n");
		sem_post(&semEmpresaVacio);
		sleep(5); // Atender empresa.
		sem_post(&semEsperarAtencionE);
	}
    }
}

void *empleadoUsuario() {
    while(1) {
	if(sem_trywait(&semPoliticoLleno) == 0) {
		printf("POLITIO ATENDIDO:: El empleado usuario ha atendido a un cliente de tipo politico.\n");
		sem_post(&semPoliticoVacio);
		sleep(5); // Atender político.
		sem_post(&semEsperarAtencionP);
	} else {
		sem_wait(&semUsuarioLleno); // En caso de que no haya politicos, pierden la prioridad momentaneamente y se espera un usuario.
		printf("CLIENTE COMUN ATENDIDO:: El empleado usuario ha atendido a un cliente de tipo usuario comun.\n");
		sem_post(&semUsuarioVacio);
		sleep(5); // Atender usuario.
		sem_post(&semEsperarAtencionU);
	}
    }
}

void *threadPolitico(void *arg) {
    	printf("Cliente Politico:: Llega al banco y consulta si hay lugar en mesa de entrada.\n");
	if(mesaDeEntrada() == 1) {
		printf("Un político entró a la mesa principal.\n");
		sem_wait(&semPoliticoVacio);
		printf("Un político entró a la mesa de políticos.\n");
		sem_post(&semPoliticoLleno);
		sem_post(&semPrincipalVacio);
		sem_wait(&semEsperarAtencionP);
	} else {
		printf("No hay espacio en la mesa principal. El usuario se va. \n");
	}
	pthread_exit(NULL);//El cliente se va.
	return NULL;
}

void *threadEmpresa(void *arg) {
	printf("Cliente Empresa:: Llega al banco y consulta si hay lugar en mesa de entrada.\n");
	if(mesaDeEntrada() == 1) {
		printf("Una empresa entró a la mesa principal.\n");
		sem_wait(&semEmpresaVacio);
		printf("Una empresa entró a la mesa de empresas.\n");
		sem_post(&semEmpresaLleno);
		sem_post(&semPrincipalVacio);
		sem_wait(&semEsperarAtencionE);
	} else {
		printf("No hay espacio en la mesa principal. La empresa se va. \n");
	}
	pthread_exit(NULL); //El cliente se va.
	return NULL;
}

void *threadUsuario(void *arg) {
	printf("Cliente Comun:: Llega al banco y consulta si hay lugar en mesa de entrada.\n");
	if(mesaDeEntrada() == 1) {
		printf("Un usuario entró a la mesa principal.\n");
		sem_wait(&semUsuarioVacio);
		printf("Un usuario entró a la mesa de usuarios.\n");
		sem_post(&semUsuarioLleno);
		sem_post(&semPrincipalVacio);
		sem_wait(&semEsperarAtencionU);
	} else {
		printf("No hay espacio en la mesa principal. El usuario se va. \n");
	}
	pthread_exit(NULL);//El cliente se va.
	return NULL;
}

int main() {
	
    srand(time(NULL));
    sem_init(&semPrincipalVacio, 0, BUFFER_SIZE);
    sem_init(&semPoliticoVacio, 0, MINI_BUFFER_SIZE);
    sem_init(&semPoliticoLleno, 0, 0);
    sem_init(&semEmpresaVacio, 0, MINI_BUFFER_SIZE);
    sem_init(&semEmpresaLleno, 0, 0);
    sem_init(&semUsuarioVacio, 0, MINI_BUFFER_SIZE);
    sem_init(&semUsuarioLleno, 0, 0);
    sem_init(&semEsperarAtencionU,0,0);
    sem_init(&semEsperarAtencionP,0,0);
    sem_init(&semEsperarAtencionE,0,0);
    
    pthread_t empleados[CANT_EMPLEADOS];
    pthread_create(&empleados[0], NULL, empleadoEmpresa, NULL);
    pthread_create(&empleados[1], NULL, empleadoEmpresa, NULL);
    pthread_create(&empleados[2], NULL, empleadoUsuario, NULL);
    
    pthread_t clientes[CANT_CLIENTES];
    for(int i=0; i<CANT_CLIENTES; i++) {
	int tipoCliente = rand() % 3;
	switch(tipoCliente) {
		case 0:
		    pthread_create(&clientes[i], NULL, threadPolitico, NULL);
		    break;
		case 1:
		    pthread_create(&clientes[i], NULL, threadEmpresa, NULL);
		    break;
		case 2:
		    pthread_create(&clientes[i], NULL, threadUsuario, NULL);
		    break;
	}
    }

    for(int i=0; i<CANT_EMPLEADOS; i++) {
	    pthread_join(empleados[i], NULL);
    }
    
    for(int i=0; i<CANT_CLIENTES; i++) {
	    pthread_join(clientes[i], NULL);
    }


    // Liberar recursos (semáforos)
    sem_destroy(&semPrincipalVacio);
    sem_destroy(&semPoliticoVacio);
    sem_destroy(&semPoliticoLleno);
    sem_destroy(&semEmpresaVacio);
    sem_destroy(&semEmpresaLleno);
    sem_destroy(&semUsuarioVacio);
    sem_destroy(&semUsuarioLleno);
    sem_destroy(&semEsperarAtencionP);
    sem_destroy(&semEsperarAtencionE);
    sem_destroy(&semEsperarAtencionU);
    return 0;
}
