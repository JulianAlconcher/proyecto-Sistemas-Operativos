#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	if (*(argv[0]) != '\0' && *(argv[1]) == '\0') {
		if (rmdir(argv[0]) == 0) {
			printf("\nDirectorio eliminado con éxito.\n\n");
		} else {
			perror("\nError al eliminar el directorio.");
			printf("\n");
		}
	} else {
		printf("\nUso incorrecto. rmdir <nombre_directorio>\n\n");
	}
	return 0;
}
