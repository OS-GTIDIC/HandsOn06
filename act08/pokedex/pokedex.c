#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "pokedex.h"


Pokemon pokemons[151];
int next_id;

int init_pokedex(){
    
    FILE* f = fopen("./pokedex/pokedex.csv", "r");
    //printf("%d\n",fileno(f));


    if(!f) {
        perror("fopen");
        exit(1);
    }

  //Mida màx d'una linia
  char *buf = malloc(100);
  char *info;
  int i = 0;

  while (fgets(buf, 100, f) != NULL) {
    if ((strlen(buf)>0) && (buf[strlen (buf) - 1] == '\n'))
        buf[strlen (buf) - 1] = '\0';

    info = strtok(buf, ",");
    int id  = atoi(info);
    info = strtok(NULL, ",");
    char * name = NULL;
    name = info;
    info = strtok(NULL, ",");
    char ** types = malloc(2*sizeof(char*));
    types[0] = info;
    info = strtok(NULL, ",");
    types[1] = info;
    info = strtok(NULL, ",");
    int total = atoi(info);
    info = strtok(NULL, ",");
    int hp = atoi(info);
    info = strtok(NULL, ",");
    int attack = atoi(info);
    info = strtok(NULL, ",");
    int defense = atoi(info);
    info = strtok(NULL, ",");
    int spAttack = atoi(info);
    info = strtok(NULL, ",");
    int spDefense = atoi(info);
    info = strtok(NULL, ",");
    int speed = atoi(info);
    info = strtok(NULL, ",");
    int generation = atoi(info);
    info = strtok(NULL, ",");
    int legendary = atoi(info);
    info = strtok(NULL, ",");


    Pokemon p = new_pokemon(id,name,types,total,hp,attack,defense,spAttack,spDefense,speed,generation,legendary,0,0);
    pokemons[i] = p;
    i++;
  }
  next_id = i + 1;
  fclose(f);
}

int mark(int position, int mode){
    Pokemon pokemon = pokemons[position];
    if (mode==SEEN) set_pokemon_seen(pokemon, pokemon_seen(pokemon)+1);
    if (mode==CAPTURED) set_pokemon_captured(pokemon, pokemon_captured(pokemon)+1);
    return EXIT_SUCCESS;
}


int show_pokemon(int position){
    Pokemon pokemon = get_pokemon(position);
    
    printf("============================= \n");
    printf("==  id: %d \n",  pokemon_id(pokemon));
    printf("==  name: %s \n",  pokemon_name(pokemon));
    printf("==  Type1: %s, Type2: %s \n",  pokemon_type(pokemon)[0],pokemon_type(pokemon)[1]);
    printf("==  Total: %d, HP:%d \n",  pokemon_total(pokemon), pokemon_hp(pokemon));
    printf("==  Attack: %d, Defense:%d \n",  pokemon_attack(pokemon), pokemon_defense(pokemon));
    printf("==  spAttack: %d, spDefense:%d, Speed: %d \n",  pokemon_spAttack(pokemon), pokemon_spDefense(pokemon), pokemon_speed(pokemon));
    printf("==  Generation: %d, Seen:%d, , Captured:%d \n",  pokemon_generation(pokemon), pokemon_seen(pokemon), pokemon_legendary(pokemon));
    printf("============================= \n");
    return EXIT_SUCCESS;
}

Pokemon get_pokemon(int position){
    Pokemon pokemon = pokemons[position];
    return pokemon;
};

int save(){

   time_t now;
   time(&now);

    char filename[100];
    sprintf(filename, "./pokedex/pokedex_%d", ctime(&now));

    FILE* f = fopen(filename, "w");
    for(int i=0; i<151; i++){
        fprintf(f,"%d,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", pokemon_id(pokemons[i]),
        pokemon_name(pokemons[i]),
        pokemon_type(pokemons[i])[0],
        pokemon_type(pokemons[i])[1],
        pokemon_total(pokemons[i]),
        pokemon_hp(pokemons[i]),
        pokemon_attack(pokemons[i]),
        pokemon_defense(pokemons[i]),
        pokemon_spAttack(pokemons[i]),
        pokemon_spDefense(pokemons[i]),
        pokemon_speed(pokemons[i]),
        pokemon_generation(pokemons[i]),
        pokemon_legendary(pokemons[i]),
        pokemon_seen(pokemons[i]),
        pokemon_captured(pokemons[i])
        );
    }
    fclose(f);

}
