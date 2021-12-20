#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	int fd[2];
    char s[100];
    char *p1[] = {"ls", NULL};
    char *p2[] = {"wc", "-l", NULL};
	
	if (pipe(fd)<0)
	{
		perror("Error de creació del pipe fd[]");
		exit(-1);
	}

    int pid;

    switch (pid = fork()){
        case -1:	
					perror("Error fork()");
					exit(-2);
					break;
					
		case 0:	
				if (close(fd[0]) < 0) perror("close fd[0]");
				if (dup2(fd[1], 1) < 0) perror("dup2 fd[1] -> 1");
				if (close(fd[1]) < 0) perror("close fd[1]");

                execvp(p1[0], p1);
				exit(-3);
	
		default:

				if (close(fd[1]) < 0) perror("close fd[1]");
				if (dup2(fd[0], 0) < 0) perror("dup2 fd[0] -> 0");
				if (close(fd[0]) < 0) perror("close fd[0]");
					
				execvp(p2[0], p2);
				exit(-3);
	    }
    

}	
