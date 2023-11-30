#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
	mode_t PERMISOS[3] = {S_IRUSR, S_IWUSR, S_IXUSR};
	if (*(argv[0]) != '\0' && *(argv[1]) != '\0') {
		FILE *file = fopen(argv[0], "r");
		if (file) {
			int mode = strtol(argv[1], NULL, 10);
			switch (mode) {
				case 1: chmod(argv[0], PERMISOS[0]); printf("\nPermisos modificados con éxito.\n\n"); break;
				case 2: chmod(argv[0], PERMISOS[1]); printf("\nPermisos modificados con éxito.\n\n"); break;
				case 3: chmod(argv[0], PERMISOS[2]); printf("\nPermisos modificados con éxito.\n\n"); break;
				case 4: chmod(argv[0], PERMISOS[0] | PERMISOS[1]); printf("\nPermisos modificados con éxito.\n\n"); break;
				case 5: chmod(argv[0], PERMISOS[0] | PERMISOS[2]); printf("\nPermisos modificados con éxito.\n\n"); break;
				case 6: chmod(argv[0], PERMISOS[1] | PERMISOS[2]); printf("\nPermisos modificados con éxito.\n\n"); break;
				case 7: chmod(argv[0], PERMISOS[0] | PERMISOS[1] | PERMISOS[2]); printf("\nPermisos modificados con éxito.\n\n"); break;
				default: printf("\nPermiso inválido. Los posibles permisos son:\n1: leer.\n2: escribir.\n3: ejecutar.\n4: leer y escribir.\n5: leer y ejecutar.\n6: escribir y ejecutar.\n7: todos los permisos\n\n"); break;
			}
			fclose(file);
		} else {
			perror("\nError al abrir el archivo.");
			printf("\n");
		}
	} else {
		printf("\nUso incorrecto: chmod <nombre_archivo> <permisos>\n\n");
	}
	return 0;
}
