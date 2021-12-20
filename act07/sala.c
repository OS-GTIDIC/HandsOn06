#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int atrapats = 0;
int jugant = 1;

char *color_blue   = "\033[01;34m";
char *color_end    = "\033[00m";
char *color_red    = "\033[01;31m";
char *color_green  = "\033[01;32m";

char *args[] = {"jugador", "jugador", NULL};

void alarm_handler(); 

void alarm_handler(){
    char s[100];
    sprintf(s, "%s**********DING DONG %s\n", color_red,color_end);
	write(1,s,strlen(s));
    atrapats=1;
    jugant=0;
}

int main(int argc, char *argv[])
{
	int fd[2];
    char s[100];
	

	if (argc < 4) {
		fprintf(stderr, "Usage: %s tempsPartida numJugadors clau \n", argv[0]);
		exit(1);
    }

    int numJugadors = atoi(argv[2]);
	int tempsPartida = atoi(argv[1]);
	char* clau = argv[3];
	int* pids = malloc(numJugadors * sizeof(int));

	signal(SIGALRM, alarm_handler);


	if (pipe(fd)<0)
	{
		perror("El procés [Banc]::Error de creació del pipe");
		exit(1);
	}
	
    sprintf(s, "Inici del Room Escape [%d] (%d segons) \n",getpid(),tempsPartida);
	if (write(1, s, strlen(s)) < 0) perror("write");

    sprintf(s, "--- Entren %d jugadors a la sala \n",numJugadors);
	if (write(1, s, strlen(s)) < 0) perror("write");

    sprintf(s, "--- La paraula a endevinar és = %s\n",clau);
	if (write(1, s, strlen(s)) < 0) perror("write");

	for(int i = 0; i<numJugadors; i=i+1){

		switch (pids[i]=fork()){

		case -1:	/* Error creación hijo */
					perror("Padre::Error creación pipe");
					exit(2);
					break;
					
		case 0:	
		
				if (close(fd[0]) < 0) perror("close");
				if (dup2(fd[1], 3) < 0) perror("dup2");
				if (close(fd[1]) < 0) perror("close");
					
				execv(args[0], args);
				exit(-1);
	
		
		default:
            sprintf(s, "%s EscapeRoom[%d] ha entrat el jugador[%d] %s\n", color_blue, getpid(), pids[i], color_end);
			if (write(1, s, strlen(s)) < 0) perror("write");
	    }
    }

  			
	/* Cerramos descriptores no utilizados */
	close(fd[1]);

    // Comença la partida quan els fills hagi

	alarm(tempsPartida);
	

	char car[4];
	int leidos;
   				
	while((jugant==1)&&(read(fd[0],&car,sizeof(car))>0))
	{
		if ( car[0] == clau[0] && car[1] == clau[1] && car[2] == clau[2] && car[3] == clau[3]){
				sprintf(s,"--- Els jugadors obren el candau amb la clau %s \n",clau);
				write(1,s,strlen(s));
				jugant=0;
                break;
			}
	}
			

    for(int i = 0; i<numJugadors; i++){
        if (atrapats==1){
            sprintf(s, "%s El jugador [%d] ha quedat atrapat i surt de EscapeRoom[%d]. %s\n", color_red, pids[i],getpid(), color_end);
            if (write(1, s, strlen(s)) < 0) perror("write");
        }
        else{
            sprintf(s, "%s El jugador [%d] ha escapat i surt de EscapeRoom[%d]. %s\n", color_green, pids[i],getpid(), color_end);
            if (write(1, s, strlen(s)) < 0) perror("write");
        }
		kill(pids[i], SIGUSR1);
		wait(0);
	}	

	if (atrapats==1){
        sprintf(s, "--- El jugadors han perdut la partida. Fins aviat. \n");
        if (write(1, s, strlen(s)) < 0) perror("write");
    }
    else {
        sprintf(s, "--- El jugadors ha guanyat la partida en %d segons. \n", tempsPartida - alarm(tempsPartida));
        if (write(1, s, strlen(s)) < 0) perror("write");
    }

	close(fd[0]);
	exit(0);	
}
