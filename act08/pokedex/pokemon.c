#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pokemon.h"


struct pokemon {
    int          pokemon_id;
    char         *name;
    char         **type;
    int          total;
    int          hp;
    int          attack;
    int          defense;
    int          spAttack;
    int          spDefense;
    int          speed;
    int          generation;
    int          legendary;
    int          seen;
    int          captured;
};


Pokemon new_pokemon(int pokemon_id, char *name, char** type,
    int total, int hp, int attack, int defense, int spAttack, int spDefense, 
    int speed, int generation, int legendary, int seen, int captured) {

    Pokemon new_pokemon = malloc(sizeof(struct pokemon));
    assert(new_pokemon != NULL);
    
    new_pokemon->name = strdup(name);
    assert(new_pokemon->name != NULL);

    new_pokemon->type = malloc(2 * sizeof(char *));
    assert(new_pokemon->type != NULL);

    new_pokemon->type[0] = strdup(type[0]);
    assert(new_pokemon->type[0] != NULL);

    new_pokemon->type[1] = strdup(type[1]);
    assert(new_pokemon->type[1] != NULL);
    
    new_pokemon->pokemon_id = pokemon_id;
    new_pokemon->total = total;
    new_pokemon->hp = hp;
    new_pokemon->attack = attack;
    new_pokemon->defense = defense;
    new_pokemon->spAttack = spAttack;
    new_pokemon->spDefense = spDefense;
    new_pokemon->speed = speed;
    new_pokemon->generation = generation;
    new_pokemon->legendary = legendary;
    new_pokemon->seen = seen;
    new_pokemon->captured = captured;

    return new_pokemon;
}

// Return the pokemon_id of the specified `pokemon`.
int pokemon_id(Pokemon pokemon) {
    return pokemon->pokemon_id;
}

// Return the pokemon_name of the specified `pokemon`.
char *pokemon_name(Pokemon pokemon) {
    return pokemon->name;
}

char **pokemon_type(Pokemon pokemon) {
    return pokemon->type;
}

int pokemon_total(Pokemon pokemon){
    return pokemon->total;
}

int pokemon_hp(Pokemon pokemon){
    return pokemon->hp;
}

int pokemon_attack(Pokemon pokemon){
    return pokemon->attack;
}

int pokemon_defense(Pokemon pokemon){
    return pokemon->defense;
}

int pokemon_spAttack(Pokemon pokemon){
    return pokemon->spAttack;
}

int pokemon_spDefense(Pokemon pokemon){
    return pokemon->spDefense;
}

int pokemon_speed(Pokemon pokemon){
    return pokemon->speed;
}

int pokemon_generation(Pokemon pokemon){
    return pokemon->generation;
}

int pokemon_legendary(Pokemon pokemon){
    return pokemon->legendary;
}

int pokemon_seen(Pokemon pokemon){
    return pokemon->seen;
}

int pokemon_captured(Pokemon pokemon){
    return pokemon->captured;
}

void set_pokemon_seen(Pokemon pokemon, int value){
    pokemon->seen=value;
};
void set_pokemon_captured(Pokemon pokemon, int value){
    pokemon->captured=value;
};

// Destroy the specified `pokemon`.
void destroy_pokemon(Pokemon pokemon) {
    free(pokemon->name);
    free(pokemon->type[0]);
    free(pokemon->type[1]);
    free(pokemon->type);
    free(pokemon);
}