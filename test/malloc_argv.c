#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int	main(void)
{
	char **argv = malloc(4 * sizeof(char *));
	char *env[] = {NULL};

	pid_t pid = fork();
	if (pid)
	{
		free(argv);
		return (0);
	}
	for (int i = 0; i < 3; i++)
		argv[i] = strdup("ls");
	argv[3] = NULL;
	int ret = execve("/usr/bin/ls", argv, env);
	printf("failed.\n");
	return (1);
}
