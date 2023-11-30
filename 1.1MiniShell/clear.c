#include <stdio.h>

int main(int argc, char* argv[]) {
    if(*(argv[0]) == '\0' && *(argv[1]) == '\0') {
        printf("\033[H\033[J");
	} else {
		printf("\nUso incorrecto: el comando 'clear' no requiere argumentos.\n\n");
	}
	return 0;
}
