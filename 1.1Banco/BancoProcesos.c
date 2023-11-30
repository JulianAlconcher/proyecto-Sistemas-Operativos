#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <errno.h>

#define COLA_MENSAJES 'A'

#define CANT_CLIENTES 76
#define BUFFER_SIZE 30
#define MINI_BUFFER_SIZE 15
#define MAX_MSG_SIZE 256

#define SEM_MESA_ENTRADA_VACIO 1
#define SEM_MESA_ENTRADA_LLENO 2

#define SEM_COLA_POLITICOS_LLENO 3
#define SEM_COLA_POLITICOS_VACIO 4

#define SEM_COLA_USUARIO_LLENO 5
#define SEM_COLA_USUARIO_VACIO 6

#define SEM_COLA_EMPRESA_LLENO 7
#define SEM_COLA_EMPRESA_VACIO 8

#define SEM_ESPERAR_ATENCION_P 9
#define SEM_ESPERAR_ATENCION_E 10
#define SEM_ESPERAR_ATENCION_U 11

#define NUEVO_CLIENTE 12


struct Mensaje {
    long tipo;
    int dato;
};

typedef struct Mensaje msg;

void empleadoEmpresa() {
	key_t key;
	key = ftok("/tmp", COLA_MENSAJES);
	int msg_id = msgget(key, 0666);
	struct Mensaje semPolLleno;
	semPolLleno.tipo = SEM_COLA_POLITICOS_LLENO;
	while(1){
		if (msgrcv(msg_id, &semPolLleno, sizeof(int), SEM_COLA_POLITICOS_LLENO, IPC_NOWAIT) != -1) {
			printf("El empleado empresa esta atendiendo a un cliente politico.\n");
			struct Mensaje semPolVacio;
			semPolVacio.tipo = SEM_COLA_POLITICOS_VACIO;
			if (msgsnd(msg_id, &semPolVacio, sizeof(int), IPC_NOWAIT) == -1) {
				perror("msgsnd: post de empleado a cola_politico");
			}
			sleep(5);
			struct Mensaje semEsperarAtencion;
			semEsperarAtencion.tipo = SEM_ESPERAR_ATENCION_P;
			if (msgsnd(msg_id, &semEsperarAtencion, sizeof(int), IPC_NOWAIT) == -1) { 
				perror("msgsnd: post de empleado a empleado_politico");
			} else {
			}
		} else {
			struct Mensaje semEmpresaLleno;
			semEmpresaLleno.tipo = SEM_COLA_EMPRESA_LLENO;
			if (msgrcv(msg_id, &semEmpresaLleno, sizeof(int), SEM_COLA_EMPRESA_LLENO, 0) != -1) { //Wait
				printf("El empleado empresa esta atendiendo a un cliente empresa.\n");
				struct Mensaje semEmpresaVacio;
				semEmpresaVacio.tipo = SEM_COLA_EMPRESA_VACIO;
				if (msgsnd(msg_id, &semEmpresaVacio, sizeof(int), IPC_NOWAIT) == -1) {
					perror("msgsnd: post de empleado a cola_empresa");
				}
				sleep(5);
				struct Mensaje semEsperarAtencion;
				semEsperarAtencion.tipo = SEM_ESPERAR_ATENCION_E;
				if (msgsnd(msg_id, &semEsperarAtencion, sizeof(int), IPC_NOWAIT) == -1) {
					perror("msgsnd: post de empleado a empleado_empresa");
				} else {
				}
				
			}
		}
	}
}

void empleadoUsuario() {
	key_t key;
	key = ftok("/tmp", COLA_MENSAJES);
	int msg_id = msgget(key, 0666);
	struct Mensaje semPolLleno;
	semPolLleno.tipo = SEM_COLA_POLITICOS_LLENO;
	while(1){
		if (msgrcv(msg_id, &semPolLleno, sizeof(int), SEM_COLA_POLITICOS_LLENO, IPC_NOWAIT) != -1) {
			printf("El empleado usuario esta atendiendo a un cliente politico.\n");
			struct Mensaje semPolVacio;
			semPolVacio.tipo = SEM_COLA_POLITICOS_VACIO;
			if (msgsnd(msg_id, &semPolVacio, sizeof(int), IPC_NOWAIT) == -1) {
				perror("msgsnd: post de empleado a cola_politico");
			}
			sleep(5);
			struct Mensaje semEsperarAtencion;
			semEsperarAtencion.tipo = SEM_ESPERAR_ATENCION_P;
			if (msgsnd(msg_id, &semEsperarAtencion, sizeof(int), IPC_NOWAIT) == -1) {
				perror("msgsnd: post de empleado a empleado_politico");
			} else {
			}
		} else {
			struct Mensaje semUsuarioLleno;
			semUsuarioLleno.tipo = SEM_COLA_USUARIO_LLENO;
			if (msgrcv(msg_id, &semUsuarioLleno, sizeof(int), SEM_COLA_USUARIO_LLENO, 0) != -1) { //Wait
			printf("El empleado usuario esta atendiendo a un cliente usuario.\n");
				struct Mensaje semUsuarioVacio;
				semUsuarioVacio.tipo = SEM_COLA_USUARIO_VACIO;
				if (msgsnd(msg_id, &semUsuarioVacio, sizeof(int), IPC_NOWAIT) == -1) {
					perror("msgsnd: post de empleado a cola_empresa");
				}
				sleep(5);
				struct Mensaje semEsperarAtencion;
				semEsperarAtencion.tipo = SEM_ESPERAR_ATENCION_U;
				if (msgsnd(msg_id, &semEsperarAtencion, sizeof(int), IPC_NOWAIT) == -1) {
					perror("msgsnd: post de empleado a empleado_empresa");
				} else {
				}
				
			}
		}
	}
}

void clientePolitico() {
	key_t key;
	key = ftok("/tmp", COLA_MENSAJES);
	int msg_id = msgget(key, 0666);
	struct Mensaje mensajeMesaVacia;
	mensajeMesaVacia.tipo = SEM_MESA_ENTRADA_VACIO;
	if (msgrcv(msg_id, &mensajeMesaVacia, sizeof(int), SEM_MESA_ENTRADA_VACIO, IPC_NOWAIT) != -1) { 
			printf("Politico entra a la mesa de entrada.\n");
			struct Mensaje semPoliticoVacio;
			semPoliticoVacio.tipo = SEM_COLA_POLITICOS_VACIO;
			if (msgrcv(msg_id, &semPoliticoVacio, sizeof(int), SEM_COLA_POLITICOS_VACIO, 0) != -1) { 
				printf("Politico entra a la cola de politicos.\n");
				struct Mensaje semPoliticoLleno;
				semPoliticoLleno.tipo = SEM_COLA_POLITICOS_LLENO;
				if(msgsnd(msg_id, &semPoliticoLleno, sizeof(int), 0) == -1) { 
					perror("msgsnd: post cola principal");
				}
				struct Mensaje semMesaVacia;
				semMesaVacia.tipo = SEM_MESA_ENTRADA_VACIO;
				if(msgsnd(msg_id, &semMesaVacia, sizeof(int), 0) == -1) { 
					perror("msgsnd: Error en SEM MESA VACIA POLITICO");
				}
				struct Mensaje semEsperarAtencion;
				semEsperarAtencion.tipo = SEM_ESPERAR_ATENCION_P;
				if (msgrcv(msg_id, &semEsperarAtencion, sizeof(int), SEM_ESPERAR_ATENCION_P, 0) != -1) {
					printf("Un cliente de tipo politico fue atendido.\n");
				} else {
					perror("msgrcv:: ERROR AL ATENDERLO");
				}
			} else {
				perror("msgrcv: wait a la cola de usuarios");
			}
	} else {
			printf("No hay lugar en mesa de entrada. Politico se va.\n");
			exit(0);
	}
}

void clienteEmpresa() {
	key_t key;
	key = ftok("/tmp", COLA_MENSAJES);
	int msg_id = msgget(key, 0666);
	struct Mensaje mensajeMesaVacia;
	mensajeMesaVacia.tipo = SEM_MESA_ENTRADA_VACIO;
	if (msgrcv(msg_id, &mensajeMesaVacia, sizeof(int), SEM_MESA_ENTRADA_VACIO, IPC_NOWAIT) != -1) { 
			printf("Empresa entra a la mesa de entrada.\n");
			struct Mensaje semEmpresaVacio;
			semEmpresaVacio.tipo = SEM_COLA_EMPRESA_VACIO;
			if (msgrcv(msg_id, &semEmpresaVacio, sizeof(int), SEM_COLA_EMPRESA_VACIO, 0) != -1) {
				printf("Empresa entra a la cola de empresas.\n");
				struct Mensaje semEmpresaLleno;
				semEmpresaLleno.tipo = SEM_COLA_EMPRESA_LLENO;
				if(msgsnd(msg_id, &semEmpresaLleno, sizeof(int), 0) == -1) { 
					perror("msgsnd: post cola usurio lleno");
				}
				struct Mensaje semMesaVacia;
				semMesaVacia.tipo = SEM_MESA_ENTRADA_VACIO;
				if(msgsnd(msg_id, &semMesaVacia, sizeof(int), 0) == -1) {
					perror("msgsnd: Error en SEM MESA VACIA Empresa");
				}
				struct Mensaje semEsperarAtencion;
				semEsperarAtencion.tipo = SEM_ESPERAR_ATENCION_E;
				if (msgrcv(msg_id, &semEsperarAtencion, sizeof(int), SEM_ESPERAR_ATENCION_E, 0) != -1) {
					printf("Un cliente de tipo empresa fue atendido.\n");
				} else {
					perror("msgrcv:: ERROR AL ATENDERLO");
				}
			} else {
				perror("msgrcv: wait a la cola de usuarios");
			}
	} else {
			printf("No hay lugar en mesa de entrada. Empresa se va.\n");
			exit(0);
	}
}

void clienteUsuario() {
	key_t key;
	key = ftok("/tmp", COLA_MENSAJES);
	int msg_id = msgget(key, 0666);
	struct Mensaje mensajeMesaVacia;
	mensajeMesaVacia.tipo = SEM_MESA_ENTRADA_VACIO;
	if (msgrcv(msg_id, &mensajeMesaVacia, sizeof(int), SEM_MESA_ENTRADA_VACIO, IPC_NOWAIT) != -1) { //Trywait a la cola principal
			printf("Usuario entra a la mesa de entrada.\n");
			struct Mensaje semUsuarioVacio;
			semUsuarioVacio.tipo = SEM_COLA_USUARIO_VACIO;
			if (msgrcv(msg_id, &semUsuarioVacio, sizeof(int), SEM_COLA_USUARIO_VACIO, 0) != -1) { //wait a la cola de usuarios
				printf("Usuario entra a la cola de usuarios.\n");
				struct Mensaje semUsuarioLleno;
				semUsuarioLleno.tipo = SEM_COLA_USUARIO_LLENO;
				if(msgsnd(msg_id, &semUsuarioLleno, sizeof(int), 0) == -1) { //Post usuario lleno
					perror("msgsnd: post cola principal");
				}
				struct Mensaje semMesaVacia;
				semMesaVacia.tipo = SEM_MESA_ENTRADA_VACIO;
				if(msgsnd(msg_id, &semMesaVacia, sizeof(int), 0) == -1) { //Post entrada vacio
					perror("msgsnd: Error en SEM MESA VACIA USUARIO");
				}
				struct Mensaje semEsperarAtencion;
				semEsperarAtencion.tipo = SEM_ESPERAR_ATENCION_U;
				if (msgrcv(msg_id, &semEsperarAtencion, sizeof(int), SEM_ESPERAR_ATENCION_U, 0) != -1) { //wait a que lo atiendan.
					printf("Un cliente de tipo usuario comun fue atendido.\n");
				} else {
					perror("msgrcv:: ERROR AL ATENDERLO");
				}
			} else {
				perror("msgrcv: wait a la cola de usuarios");
			}
	} else {
			printf("No hay lugar en mesa de entrada. Usuario se va.\n");
			exit(0);
	}
}

int main() {
	srand(time(NULL));
	key_t key;
	int msg_id;
	key = ftok("/tmp", COLA_MENSAJES);
	msg_id = msgget(key, 0666 | IPC_CREAT);
	if (msg_id == -1) {
		perror("msgget");
		exit(1);
	}
    
   if (msgctl(msg_id, IPC_RMID, NULL) == -1) {
		perror("msgctl");
		exit(1);
	}
	msg_id = msgget(key, 0666 | IPC_CREAT);
	if (msg_id == -1) {
		perror("msgget");
		exit(1);
	}
	
	//Inicializo mesa en 30, simulando semaforo contador.
	for(int i=0; i<BUFFER_SIZE; i++) {
		struct Mensaje mensajeEnviado;
		mensajeEnviado.tipo = SEM_MESA_ENTRADA_VACIO;
		if (msgsnd(msg_id, &mensajeEnviado, sizeof(int), IPC_NOWAIT) == -1) {
			perror("msgsnd:: ERROR AL INICIALIZAR MESA DE ENTRADA CON 30");
		}
	}
	//Inicializo cada cola en 15, simulando semaforo contador.
	for(int i=0; i<(MINI_BUFFER_SIZE); i++) {
		struct Mensaje mensajeUsuario;
		mensajeUsuario.tipo = SEM_COLA_USUARIO_VACIO;
		if (msgsnd(msg_id, &mensajeUsuario, sizeof(int), IPC_NOWAIT) == -1) {
			perror("msgsnd:: ERROR AL INICIALIZAR MESA DE USUARIO CON 15");
		}
		struct Mensaje mensajeEmpresa;
		mensajeEmpresa.tipo = SEM_COLA_EMPRESA_VACIO;
		if (msgsnd(msg_id, &mensajeEmpresa, sizeof(int), IPC_NOWAIT) == -1) {
			perror("msgsnd:: ERROR AL INICIALIZAR MESA DE EMPRESA CON 15");
		}
		struct Mensaje mensajePolitico;
		mensajePolitico.tipo = SEM_COLA_POLITICOS_VACIO;
		if (msgsnd(msg_id, &mensajePolitico, sizeof(int), IPC_NOWAIT) == -1) {
			perror("msgsnd:: ERROR AL INICIALIZAR MESA DE POLITICO CON 15");
		}
	}
		
	for(int i=0; i<3; i++) {
		if(i==2) {
			if(fork()==0) {
				empleadoUsuario();
				exit(0);
			}
		} else {
			if(fork()==0) {
				empleadoEmpresa();
				exit(0);
			}
		}
	}

	for(int i=0; i<CANT_CLIENTES; i++) {
		int tipoCliente = rand() % 3;
		if(fork() == 0) {
			switch(tipoCliente) {
				case 0: printf("Nuevo proceso:: Se crea un cliente de tipo político\n"); clientePolitico(); break;
				case 1: printf("Nuevo proceso:: Se crea un cliente de tipo empresa \n"); clienteEmpresa(); break;
				case 2: printf("Nuevo proceso:: Se crea un cliente de tipo usuario\n"); clienteUsuario(); break;
			}
			exit(0);
		}
	}

	for(int i=0; i<CANT_CLIENTES+3; i++) {
		wait(NULL);
	}
	
	if (msgctl(msg_id, IPC_RMID, NULL) == -1) {
		perror("msgctl");
		exit(1);
	}
	exit(0);
	

    return 0;
}
