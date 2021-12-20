#ifndef _POKEDEX_H_
#define _POKEDEX_H_

#include "pokemon.h"

#define SEEN 0
#define CAPTURED 1

Pokemon get_pokemon(int position);
int show_pokemon(int position);
int mark(int position, int mode);
int init_pokedex();
int save();

#endif //  _POKEDEX_H_