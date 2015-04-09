#include "PokemonGenerator.h"

std::vector<OBJModel*> PokemonGenerator::GeneratePokemon(){
	std::vector<OBJModel*> pokemon;


	//30 pokemon! 8D
	for (int i = 0; i < 30; i++){
		int r = rand() % NUM_POKE; //random selection of pokemon

		//random position between coordinates -70,70
		int r2 = (rand() % 140) - 70;
		int r3 = (rand() % 140) - 70;

		//random rotation
		int r4 = rand() % 360;
		
		OBJModel* p;

		


		switch (r){
		case BULBASAUR:
			p = new OBJModel("../Models/Bulbasaur.obj");
			break;
		case BUTTERFREE:
			p = new OBJModel("../Models/Butterfree.obj");
			break;
		case CHARMANDER:
			p = new OBJModel("../Models/Charmander.obj");
			break;
		case MEOWTH:
			p = new OBJModel("../Models/Meowth.obj");
			break;
		case PIKACHU:
			p = new OBJModel("../Models/Pikachu.obj");
			break;
		case SQUIRTLE:
			p = new OBJModel("../Models/Squirtle.obj");
			break;
		case ZUBAT:
			p = new OBJModel("../Models/Zubat.obj");
			break;
		}

		p->SetPosition(vec3(r2, 0, r3));
		p->SetRotation(vec3(0, 1, 0), (float)r4);
		p->SetScaling(vec3(0.4f, 0.4f, 0.4f));
		pokemon.push_back(p);
	}

	return pokemon;
}