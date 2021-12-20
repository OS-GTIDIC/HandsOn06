#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "helpers/helpers.h"

#define MAX_BERRIES 3

char msg [100];
int berries = 0;

void throw_pokeball_action(){
    sprintf(msg,"Pokemon [%d] ha rebut el senyal SIGUSR1...\n", getpid());
    logger("DEBUG", msg);

    int option = getRandom(10);
    sprintf(msg,"Pokemon [%d] ha generat el valor %d...\n", getpid(), option);
    logger("DEBUG", msg);

    for (int i=0; i<=berries; i++){
        if (option == 2 + (i*2)){
            sprintf(msg,"Pokemon [%d] ha estat capturat, per tant acaba amb exit(%d) i ho notifica al seu pare...\n", getpid(), PokemonCaught);
            logger("DEBUG", msg);
            exit(PokemonCaught);
        }
    }

    if(option==7){
        sprintf(msg,"Pokemon [%d] ha escapat, per tant acaba amb exit(%d) i ho notifica al seu pare...\n", getpid(), PokemonEscaped);
        logger("DEBUG", msg);
        exit(PokemonEscaped);
    } else{
        sprintf(msg,"Pokemon [%d] no capturat... s'atura i ho notifica al seu pare...\n", getpid());
        logger("DEBUG", msg);
        raise(SIGSTOP);
    }   
}

void run_action(){
    sprintf(msg,"Pokemon [%d] run action... acabant el proces... exit(%d)... enviant SIGCHLD al seu pare...\n", getpid(), AshEscaped);
    logger("DEBUG", msg);
    exit(AshEscaped);
}

void throw_berry_action(){

    
    if (berries < MAX_BERRIES){
        berries = berries + 1;
        sprintf(msg,"Pokemon [%d] receives a berry... [%d/%d]\n", getpid(),berries,MAX_BERRIES);
        logger("DEBUG", msg);
    } else {
        sprintf(msg,"Pokemon [%d] is stuffed... [%d/%d]\n", getpid(),berries,MAX_BERRIES);
        logger("DEBUG", msg);
    }

    raise(SIGSTOP);

}


int main(){

    // Generem una seed pseudoaleatòria amb base al pid del procés.
    srand(getpid());

    // Quan el procés reb el senyal SIGUSR1, executarà la funció action
    //signal(SIGUSR1,action);

    if (signal(SIGUSR1, throw_pokeball_action) == (void (*)(int))-1) perror ("Error. Pokemon - SIGUSR1");
    if (signal(SIGUSR2, throw_berry_action) == (void (*)(int))-1) perror ("Error. Pokemon - SIGUSR2");
    if (signal(SIGINT, run_action) == (void (*)(int))-1) perror ("Error. Pokemon - SIGINT");


    //Bucle infinit per assegurar que el procés no acabi
    while(1){

    }


}