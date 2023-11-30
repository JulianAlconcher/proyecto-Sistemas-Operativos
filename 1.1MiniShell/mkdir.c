#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
	if (*(argv[0]) != '\0' && *(argv[1]) == '\0') {
		if (mkdir(argv[0], 0755) == 0) {
			printf("\nDirectorio creado con éxito.\n\n");
		} else {
			perror("\nError al crear el directorio.");
			printf("\n");
		}
	} else {
		printf("\nUso incorrecto. mkdir <nombre_directorio>\n\n");
	}
	return 0;
}
