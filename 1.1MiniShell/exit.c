#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if(*(argv[0]) == '\0' && *(argv[1]) == '\0') {
       exit(2);
	} else {
		printf("\nUso incorrecto: el comando 'exit' no requiere argumentos.\n\n");
	}
	return 0;
}
