#pragma once
#include "OBJModel.h"
#include <vector>

enum pokemon{
	BULBASAUR,
	BUTTERFREE,
	CHARMANDER,
	MEOWTH,
	PIKACHU,
	SQUIRTLE,
	ZUBAT,
	NUM_POKE
};

class PokemonGenerator{
public:
	static std::vector<OBJModel*> GeneratePokemon();
};