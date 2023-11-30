#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
	if (*(argv[0]) != '\0' && *(argv[1]) == '\0') {
		FILE *file = fopen(argv[0], "r");
		if (file) {
			char c;
			printf("\n");
			while ((c = fgetc(file)) != EOF) {
				putchar(c);
			}
			printf("\n");
			fclose(file);
		} else {
			perror("\nError al abrir el archivo.");
			printf("\n");
		}
	} else {
		printf("\nUso incorrecto: cat <nombre_archivo>\n\n");
	}
	return 0;
}
