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
    int status;
    char s[100];
    char *p1[] = {"ls", NULL};
    char *p2[] = {"wc", "-l", NULL};
	
	if (pipe(fd)<0)
	{
		perror("Error de creació del pipe fd[]");
		exit(-1);
	}

    int pid_p1;
    int pid_p2;

    switch (pid_p1 = fork()){
        case -1:	
					perror("Error fork()");
					exit(-2);
					break;
					
		case 0:	
				if (close(fd[1]) < 0) perror("close fd[1]");
				if (dup2(fd[0], 0) < 0) perror("dup2 fd[0] -> 0");
				if (close(fd[0]) < 0) perror("close fd[0]");
					
				execvp(p2[0], p2);
				exit(-3);
	
		default:

                 switch (pid_p2 = fork()){
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

                        close(fd[1]);
                        waitpid(pid_p1,&status,0);
                        waitpid(pid_p2,&status,0);
                    }

	    }
    

}	
