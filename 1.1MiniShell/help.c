#include <stdio.h>

int main(int argc, char* argv[]) {
    if(*(argv[0]) == '\0' && *(argv[1]) == '\0') {
        printf("\nComandos disponibles:\n");
        printf("a) Mostrar ayuda: help\n");
        printf("b) Crear directorio: mkdir <nombre_directorio>\n");
        printf("c) Eliminar directorio: rmdir <nombre_directorio>\n");
        printf("d) Crear archivo: touch <nombre_archivo>\n");
        printf("e) Listar contenido de directorio: ls <nombre_directorio>\n");
        printf("f) Mostrar contenido de archivo: cat <nombre_archivo>\n");
        printf("g) Modificar permisos de archivo: chmod <nombre_archivo> <permisos>\n");
        printf("h) Limpiar la shell: clear\n");
        printf("i) Salir de la shell: exit\n\n");
    } else {
        printf("\nUso incorrecto. El comando 'help' no requiere argumentos.\n\n");
    }
	return 0;
}
