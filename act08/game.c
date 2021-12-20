#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "helpers/helpers.h"
#include "pokedex/pokedex.h"

char *args[] = {"pokemon", "pokemon", NULL};
int pokemonsReady;
int current_state;
/**
 * @brief This function is the handler of signal SIGUSR1. 
 * When ash-px receives this signals it means that the process must change 
 * it status and make progress. First time, ash process will receives SIGUSR1 
 * when pokedex is ready. It let ash to check information in the Pokedex. 
 * The state ready_mode indicates if ash makes an infinite loop (challenge1 requirement)
 * or not get block and just use the Pokedex when it needs. 
 * The other states are needed to control and sync the pokemon battles (challenge 3).
 * */
void changeStatus(){

    if(current_state==WaitingPokemon && pokemonsReady<2){
         pokemonsReady=pokemonsReady + 1;
    }
    if(current_state==WaitingPokemon && pokemonsReady==2){
         current_state=Fighting;
    }
    if(current_state==Fighting){
        current_state=EndFight;
    }
}
 
int main(int arc, char *arv[])
{

signal(SIGUSR1, changeStatus);

char s[100];
init_pokedex();
srand(getpid());

int endFlag=1;
while (endFlag == 1) {
int st;
char choice;
sprintf(s, "################\n# E. Explore\n# F. Fight  \n# Q. Quit\n################\n");
if (write(1, s, strlen(s)) < 0) perror("Error writting the menu");
scanf(" %c", &choice);
 

switch (choice) {
    case 'Q':
    endFlag=0;
    break;
    case 'F': ;// Codi per simular els combats
        pokemonsReady=0;

        int pokemon1PipeWR[2];
        int pokemon2PipeWR[2];
        int gamePipeWR[2];
        int pidPokemon[2];
     
        current_state=WaitingPokemon;
    
        
        if (pipe(pokemon1PipeWR) < 0) perror("Error. Opening pokemon1PipeWR");
        if (pipe(pokemon2PipeWR) < 0) perror("Error. Opening pokemon2PipeWR");
        if (pipe(gamePipeWR) < 0) perror("Error. Opening pokemonPipeWR");

        for(int p=1; p<=2; p++){
            switch (pidPokemon[p-1] = fork()){
                case -1:	perror("Error. Forking a new pokemon to battle.");
                case 0:	
                    // gamePipeWR: Ash -> Pokemon (Pipe)
                    if (close(gamePipeWR[1]) < 0) perror("Error. Closing gamePipeWR[1]");
                    if (dup2(gamePipeWR[0], 0) < 0) perror("Error. Dup2 gamePipeWR <- 0");
                    if (close(gamePipeWR[0]) < 0) perror("Error. Closing gamePipeWR[0]");

                    // pokemon1PipeWR: Pokemon -> Pokemon (Pipe) 
                    if (dup2(pokemon1PipeWR[0], 10) < 0) perror("Error. Dup2 pokemon1PipeWR <- 5");
                    if (close(pokemon1PipeWR[0]) < 0) perror("Error. Close pokemon1PipeWR [0]");

                    if (dup2(pokemon1PipeWR[1], 11) < 0) perror("Error. Dup2 pokemon1PipeWR <- 6");
                    if (close(pokemon1PipeWR[1]) < 0) perror("Error. Close pokemon1PipeWR [1]");

                    // pokemon2PipeWR: Pokemon -> Pokemon (Pipe) 
                    if (dup2(pokemon2PipeWR[0], 12) < 0) perror("Error. Dup2 pokemon2PipeWR <- 7");
                    if (close(pokemon2PipeWR[0]) < 0) perror("Error. Close pokemon2PipeWR [0]");

                    if (dup2(pokemon2PipeWR[1], 13) < 0) perror("Error. Dup2 pokemon2PipeWR <-8");
                    if (close(pokemon2PipeWR[1]) < 0) perror("Error. Close pokemon2PipeWR [1]");

                    // arguments
                    char initialFightMode[32];
                    char color[32];
                    sprintf(initialFightMode,"%d", 4+p-1);
                    sprintf(color,"%d", p-1);

                    char *pokemon_fight[] = {"pokemon-fight",initialFightMode,color, NULL};
                  
                    execv(pokemon_fight[0], pokemon_fight);
                    exit(-1);
            }
            while(pokemonsReady<p){}
        }


        printf("%s[%d] The pokemons are ready to fight...\n",KNRM, getpid());
    
    
        struct info i;
        
        /**
        * @warning: Be careful, we can not know beforehand which process (pokemon) 
        * is going to get the CPU first. Thus, we do not know who will read first from 
        * the shared gamePipe.
        * @note: It does not matter which process reads first, because it do not matter
        * which file descriptors we assign to each process. If we are consistent. 
        * @definition: Pipes are unidirectionals.
        * @example: fd=10 reprsents reading from pipe1, and fd=13 represents writting from pipe2.
        * fd=11 reprsents writting from pipe1, and fd=12 represents reading from pipe2.
        **/

      
        i.fdR=10; i.fdW=13;
        if (write(gamePipeWR[1],&i, sizeof(struct info)) < 0) perror("Error writting in the pipe info1");

        i.fdR=12;i.fdW=11;
        if (write(gamePipeWR[1],&i, sizeof(struct info)) < 0) perror("Error writting in the pipe info1"); 

        for(int p=0; p<2; p++){
            int st;
            waitpid(pidPokemon[p],&st,0);
            sprintf(s,"%s[%d] Received the signal SIGCHLD... pokemon [%d] ends properly %d\n",KNRM, getpid(),pidPokemon[p],WEXITSTATUS(st));
            logger("INFO", s);
        }

        printf("%s[%d] The fight ends...\n", KNRM, getpid());

        close(pokemon1PipeWR[1]);
        close(pokemon1PipeWR[0]);
        close(pokemon2PipeWR[1]);
        close(pokemon2PipeWR[0]); 
        close(gamePipeWR[1]);
        close(gamePipeWR[0]);
        break;
        
    case 'E':
    sprintf(s, "%s", KMAG, KNRM);
    if (write(1, s, strlen(s)) < 0) perror("Error writting colour KMAG");
    int pos = getRandom(151);
    show_pokemon(pos);
    sprintf(s, "%s", KNRM);
    mark(pos, SEEN);
    if (write(1, s, strlen(s)) < 0) perror("Error writting colour KNRM");

    int pokemon_pid = fork();
    int endExplore=1;
    while (endExplore == 1){
        int validTurn = 1;
        if (pokemon_pid == 0){
            execv("./pokemon", args);
            perror("Error exec");
        }
        else{
            sprintf(s, "################\n# P. Throw pokeball \n# B. Throw berry \n# R. Run\n################\n");
            if (write(1, s, strlen(s)) < 0) perror("Error writting the menu");
            scanf(" %c", &choice);
            switch (choice) {
                case 'P':
                    kill(pokemon_pid, SIGUSR1);
                    break;
                case 'B':
                    kill(pokemon_pid, SIGUSR2);
                    break;
                case 'R':
                    kill(pokemon_pid, SIGINT);
                    break;
                default:
                    sprintf(s, "%s!!!!Invalid option. Try again. %s\n", KRED, KNRM);
                    if (write(1, s, strlen(s)) < 0) perror("Error writting invalid option");
                    validTurn = 0;
            }

            if (validTurn == 1){
                waitpid(-1, &st, WUNTRACED);
                int status = WEXITSTATUS(st);
                if(status==PokemonEscaped){
                    sprintf(s, "%sThe pokemon escaped already%s\n", KYEL,KNRM);
                    if (write(1, s, strlen(s)) < 0) perror("Error writting the menu");
                    endExplore=0;
                } else if (status==PokemonCaught) {
                    sprintf(s, "%sGotcha!The pokemon was caught.%s\n", KGRN,KNRM);
                    if (write(1, s, strlen(s)) < 0) perror("Error writting the menu");
                    endExplore=0;
                    mark(pos, CAPTURED);
                } else if (status==AshEscaped) {
                    sprintf(s, "%sYou have escaped safely.%s\n", KRED,KNRM);
                    if (write(1, s, strlen(s)) < 0) perror("Error writting the menu");
                    endExplore=0;
                } else{
                    sprintf(s, "%sOh no!The pokemon broke free.%s\n", KBLU,KNRM);
                    if (write(1, s, strlen(s)) < 0) perror("Error writting the menu");
                    kill(pokemon_pid, SIGCONT);
                }
            }
        }
    }
    break;
    default:
        sprintf(s, "%s!!!!Invalid option. Try again. %s\n", KRED, KNRM);
        if (write(1, s, strlen(s)) < 0) perror("Error writting invalid option");
    }
}
 
sprintf(s, "%s!!!!I'm tired from all the fun... %s\n", KMAG, KNRM);
if (write(1, s, strlen(s)) < 0) perror("Error writting the ending msg");
save();
exit(0);
 
}