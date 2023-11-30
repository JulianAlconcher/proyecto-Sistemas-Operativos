#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main() {
	char input[256];
	char command[128];
	char arg1[128];
	char arg2[128];
	pid_t pid;
	printf("Ingrese el comando 'help' para ver ayuda.\n");
	while(1) {
		memset(command, 0, sizeof(command));
		memset(arg1, 0, sizeof(arg1));
		memset(arg2, 0, sizeof(arg2));
		printf("CustomShell> ");
		fgets(input, sizeof(input), stdin);
		sscanf(input, "%s %s %s", command, arg1, arg2);
		char *args[] = {arg1, arg2, NULL};
		pid = fork();
		if (pid == 0) {
			execv(command, args);
			printf("\nError: el comando no existe.\n\n");
			exit(EXIT_FAILURE);
		} else {
			int status;
			wait(&status);
			if(WEXITSTATUS(status) == 2) {
				exit(0);
			}
		}
	}
	return 0;
}
