#include<string.h>
#include<errno.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "pokedex/pokemon.h"
#include "pokedex/pokedex.h"
#include "helpers/helpers.h"

#define START_ATTACKING 4
#define START_DEFENDING 5

enum Fight {NotReady=0, Winner=2, Looser=3, Attack=4,Defense=5};
int currentStatus;

char * rivalColour;
char * myColour;
int fighting = TRUE;


void getColour(int colour){
    if (colour == 1){
            myColour = KMAG;
            rivalColour = KCYN;
    }else{
            myColour = KCYN;
            rivalColour = KMAG;
    }
}

void changeFightMode(){
    if (currentStatus==Attack){
        currentStatus=Defense;
    }else{
        currentStatus=Attack;
    }
}

void endFight(){
    currentStatus=Winner;
    exit(currentStatus);
}

void sendAttack(int damage, int fd){
    char attack_send[250]; 
    sprintf(attack_send,"%d",damage);
    if (write(fd, attack_send, strlen(attack_send)) < 0) perror("Error write 'Creació fill'"); 
}

int receiveAttack(int fd){
   char attack_received[250];
   if (read(fd, &attack_received, sizeof(char)) < 0) perror("Error read pipe");
   return atoi(attack_received); 
}

int main(int argc, char *argv[])
{
    char msg[300];
    srand(getpid());
    init_pokedex();

    // Signals
    signal(SIGUSR1,changeFightMode);
    signal(SIGUSR2,endFight);

    /**
     * @param argv1 Defines the fighting mode. One pokemon must start attacking (4) 
     *             and the other defending (5).
     * @param argv2 Defines the stdout color. The are two colors predefined. KMAG (0) and KCYN (1). 
     * */

    if ( argc != 3 || 
       (atoi(argv[1]) != START_ATTACKING && atoi(argv[1]) != START_DEFENDING ) || 
       (atoi(argv[2]) != 0 && atoi(argv[2]) != 1 )) {
        sprintf(msg,"[%d] Wrong arguments... ending! \n", getpid());
        logger("ERROR", msg);   
        exit(1);
    } 

    int initialFightMode=atoi(argv[1]); 
    getColour(atoi(argv[2])); 

    currentStatus=NotReady;
    Pokemon p = get_pokemon(getRandom(151));

    int round=0;
    int life = pokemon_hp(p);
    int currentLife = life;

    currentStatus=initialFightMode;
    kill(getppid(),SIGUSR1);

    sprintf(msg,"[%d] %s is ready to fight, sends SIGUSR1 to ash process [%d]! \n", getpid(),pokemon_name(p),getppid());
    logger("INFO", msg);

    /**
     * @warning: This point the pokemon process is ready to fight, but needs information about the rival and the 
     *           inner file descriptors that it is going to use to attack and defend from its rival. Ash process 
     *           is going to send to standard input (gamePipe) an struct with (fdW, fdR). Where fdW 
     *           represents the file descriptor to attack and fdR the file descriptor to receive damage.
     * */
    struct info i;

    if (read(0, &i, sizeof(struct info)) < 0) perror("Error read pipe - reciving pid rival");

    int pid = getpid();
    int pidRival=99999;
    //char ** types = malloc(2*sizeof(char*));
    //Pokemon pokemonRival = new_pokemon(-1, "DFSAFDSFAF", types,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1);
    char nameRival[100];
    int fdR = i.fdR;
    int fdW = i.fdW;



    while(fighting==TRUE){
        printf("%s[%d] ########%s[%d/%d]\n", myColour, getpid(), pokemon_name(p), currentLife,life);
        if (currentStatus==Attack){

            /**
             * @note: It is important, before figthing be polite and say hello! ^^ 
             **/
            if(round==0){
   
              if (write(fdW, pokemon_name(p), strlen(pokemon_name(p))) < 0) perror("Error writting pokemon rival");
              if (read(fdR, &nameRival, 100) < 0) perror("Error reading pokemonRival");
              if (write(fdW, &pid, sizeof(int)) < 0) perror("Error writting pid  to rival");
              if (read(fdR, &pidRival, sizeof(int) ) < 0) perror("Error reading pidRival");

     

              round = round +1;
            } 
            

            int attack_damage = getRandom(20);
            printf("%s[%d] ########%s %s---(Send attack with damage(%d))---> %s%s[%d]%s\n", myColour, getpid(),
                    pokemon_name(p), KGRN ,attack_damage,rivalColour,nameRival,pidRival,myColour);fflush(stdout);
            sendAttack(attack_damage, fdW);
            raise(SIGUSR1);

        }else if(currentStatus==Defense){

            /**
             * @note: It is important, before figthing be polite and say hello! ^^ 
             **/
            if(round==0){
              if (read(fdR, &nameRival, 100 ) < 0) perror("Error reading pokemonRival");
              if (write(fdW, pokemon_name(p), strlen(pokemon_name(p))) < 0) perror("Error writting pokemon rival");
              
              
              if (read(fdR, &pidRival, sizeof(int) ) < 0) perror("Error reading pidRival");
              if (write(fdW, &pid, sizeof(int)) < 0) perror("Error writting pid to rival");
              round = round +1;
              
            }

            int received_damage = receiveAttack(fdR);
            printf("%s[%d] ########%s %s<---(Receives attack with damage(%d))--- %s%s[%d]%s\n", myColour, getpid(),
             pokemon_name(p), KRED, received_damage,rivalColour,nameRival,pidRival,myColour);fflush(stdout);
            currentLife = currentLife - received_damage;
            
            if(currentLife<=0) {
                currentLife=0;
                currentStatus=Looser;
                printf("%s El combat ha acabat. %s%s[%d] a derrotat %s%s[%d]%s\n",KNRM,
                rivalColour,nameRival,pidRival, myColour,pokemon_name(p),getpid(),KNRM);fflush(stdout);
                kill(pidRival,SIGUSR2);
                exit(currentStatus);
            }else{
            raise(SIGUSR1);
            }
        }
    }
}