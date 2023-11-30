#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
	if (*(argv[0]) != '\0' && *(argv[1]) == '\0') {
		FILE *file = fopen(argv[0], "w");
		if (file) {
			fclose(file);
			printf("\nArchivo creado con éxito.\n\n");
		} else {
			perror("\nError al crear el archivo.");
			printf("\n");
		}
	} else {
		printf("\nUso incorrecto. touch <nombre_archivo>\n\n");
	}
	return 0;
}
