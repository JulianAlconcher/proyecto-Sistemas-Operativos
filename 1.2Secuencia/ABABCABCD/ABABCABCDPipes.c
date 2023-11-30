#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>

#define SIZE_MSG sizeof(tMensaje)
#define CANT_PROCESOS 4
#define CANT_PIPES 4

struct message {
    char cuerpo[12];
};

typedef struct message tMensaje;

int pipeA[2];
int pipeB[2];
int pipeC[2];
int pipeD[2];


void *print_a() {
    tMensaje recibir;
    tMensaje enviar;
    close(pipeA[1]);
    close(pipeB[0]);
    close(pipeC[1]);
    close(pipeC[0]);
    close(pipeD[0]);
    close(pipeD[1]);
    while (1) {
        read(pipeA[0], &recibir,SIZE_MSG);
        printf("A");
        fflush(NULL);
        write(pipeB[1], &enviar,SIZE_MSG);
    }
    close(pipeA[0]);
    close(pipeB[1]);
}

void *print_b() {
    tMensaje recibir;
    tMensaje enviar;
    close(pipeB[1]);
    close(pipeC[0]);
    close(pipeD[1]);
    close(pipeD[0]);
    close(pipeA[0]);
    close(pipeA[1]);    
    while (1) {
        read(pipeB[0], &recibir,SIZE_MSG);
        printf("B");
        fflush(NULL);
        write(pipeC[1], &enviar,SIZE_MSG);
    }
    close(pipeB[0]);
    close(pipeC[1]);
}


void *print_c() {
    tMensaje recibir;
    tMensaje enviar;
    close(pipeC[1]);
    close(pipeA[0]);
    close(pipeB[1]);
    close(pipeB[0]);
    close(pipeD[0]);
    while (1) {
	read(pipeC[0], &recibir,SIZE_MSG);
	write(pipeA[1], &enviar,SIZE_MSG);
	read(pipeC[0], &recibir,SIZE_MSG);
	printf("C");
    	fflush(NULL);
	write(pipeA[1], &enviar,SIZE_MSG);
	read(pipeC[0], &recibir,SIZE_MSG);
	printf("C");
	fflush(NULL);
	write(pipeD[1], &enviar,SIZE_MSG);
    }
    close(pipeC[0]);
    close(pipeA[1]);
}

void *print_d() {
    tMensaje recibir;
    tMensaje enviar;
    close(pipeC[1]);
    close(pipeC[0]);
    close(pipeB[1]);
    close(pipeB[0]);
    close(pipeA[0]);
    close(pipeD[1]);
    while (1) {
        read(pipeD[0], &recibir,SIZE_MSG);
        printf("D ");
        fflush(NULL);
	sleep(1);
        write(pipeA[1], &enviar,SIZE_MSG);
    }
    close(pipeD[0]);
    close(pipeA[1]);
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
    
    int * arregloPipe[CANT_PIPES] = {pipeA, pipeB, pipeC, pipeD};
    for(int i=0; i<CANT_PIPES; i++){
	if(pipe(arregloPipe[i])==-1){
		printf("Ocurrio un error al crear los pipes");
		exit(EXIT_FAILURE);
	}
    }
	
    tMensaje enviar;
    pid_t procesos[CANT_PROCESOS];
    write(pipeA[1], &enviar, SIZE_MSG);

    for(int i=0; i<CANT_PROCESOS; i++){
	procesos[i] = fork();
	if(procesos[i] < 0){
		perror("Error en el fork \n");
		exit(EXIT_FAILURE);
	}
	else if(procesos[i] == 0){
		funcion(i);
	}
    }
	
    for(int i=0; i<CANT_PROCESOS; i++){
	    wait(NULL);
    }
}
