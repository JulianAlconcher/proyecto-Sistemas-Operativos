#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

int main(int argc, char* argv[]) {
	if (*(argv[0]) != '\0' && *(argv[1]) == '\0') {
		struct dirent *entry;
		DIR *dir = opendir(argv[0]);
		if (dir) {
			int isEmpty = 1;
			printf("\nContenido de %s:\n", argv[0]);
			while ((entry = readdir(dir))) {
				if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
					printf("%s\n", entry->d_name);
					isEmpty = 0;
				}
			}
			printf("\n");
			closedir(dir);
			if (isEmpty) {
				printf("\nEl directorio está vacío.\n\n");
			}
		} else {
			perror("\nError al abrir el directorio.");
			printf("\n");
		}
	} else {
		printf("\nUso incorrecto. ls <nombre_directorio>\n\n");
	}
	return 0;
}
