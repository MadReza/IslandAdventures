#include "PokemonGenerator.h"
#include "BSpline.h"

std::vector<OBJModel*> PokemonGenerator::GeneratePokemon(){
	std::vector<OBJModel*> pokemon;


	
	OBJModel*  Bulbasaur = new OBJModel("../Models/Bulbasaur.obj");
		

	OBJModel* Butterfree = new OBJModel("../Models/Butterfree.obj");
		
	
	OBJModel* Charmander = new OBJModel("../Models/Charmander.obj");
		
	
	OBJModel* Meowth = new OBJModel("../Models/Meowth.obj");
		
	
	OBJModel* Pikachu = new OBJModel("../Models/Pikachu.obj");
		
	
	OBJModel* Squirtle = new OBJModel("../Models/Squirtle.obj");
		
	
	OBJModel* Zubat = new OBJModel("../Models/Zubat.obj");


	//srand(20);
	//30 pokemon! 8D
	for (int i = 0; i < 30; i++){
		int r = rand() % NUM_POKE; //random selection of pokemon

		//random position between coordinates -70,70
		int r2 = (rand() % 140) - 70;
		int r3 = (rand() % 140) - 70;

		//random rotation
		int r4 = rand() % 360;
		
		int rHeight = rand() % 20 + 10;
		OBJModel* p;
		BSpline* mSpline = new BSpline();
		



		switch (r){
		case BULBASAUR:
			p = new OBJModel(*Bulbasaur);
			mSpline->SetPosition(vec3(r2, 0, r3));
			mSpline->SetRotation(vec3(1.0, 0.0, 0.0), 180);
			mSpline->AddControlPoint(vec3(rand() % 20 -10, 0, rand() % 20 - 10));
			mSpline->AddControlPoint(vec3(rand() % 20 - 10, 0, rand() % 20 - 10));
			mSpline->AddControlPoint(vec3(rand() % 20 - 10, 0, rand() % 20 - 10));
			mSpline->AddControlPoint(vec3(rand() % 20 - 10, 0, rand() % 20 - 10));
			p->setSplinePath(mSpline);
			break;
		case BUTTERFREE:
			p = new OBJModel(*Butterfree);
			mSpline->SetPosition(vec3(r2, rHeight, r3));
			mSpline->SetRotation(vec3(1.0, 0.0, 0.0), 180);
			mSpline->AddControlPoint(vec3(rand() % 10-5, rand()%10, rand() % 10-5));
			mSpline->AddControlPoint(vec3(rand() % 10 - 5, rand() % 10, rand() % 10 -5));
			mSpline->AddControlPoint(vec3(rand() % 10 -5, rand() % 10, rand() % 10 -5));
			mSpline->AddControlPoint(vec3(rand() % 10 -5, rand() % 10, rand() % 10 - 5));
			p->setSplinePath(mSpline);
			break;
		case CHARMANDER:
			p = new OBJModel(*Charmander);
			mSpline->SetPosition(vec3(r2, 0, r3));
			mSpline->SetRotation(vec3(1.0, 0.0, 0.0), 180);
			mSpline->AddControlPoint(vec3(rand() % 20 - 10, 0, rand() % 20 - 10));
			mSpline->AddControlPoint(vec3(rand() % 20 - 10, 0, rand() % 20 - 10));
			mSpline->AddControlPoint(vec3(rand() % 20 - 10, 0, rand() % 20 - 10));
			mSpline->AddControlPoint(vec3(rand() % 20 - 10, 0, rand() % 20 - 10));
			p->setSplinePath(mSpline);
			break;
		case MEOWTH:
			p = new OBJModel(*Meowth);
			mSpline->SetPosition(vec3(r2, 0, r3));
			mSpline->SetRotation(vec3(1.0, 0.0, 0.0), 180);
			mSpline->AddControlPoint(vec3(rand() % 20 - 10, 0, rand() % 20 - 10));
			mSpline->AddControlPoint(vec3(rand() % 20 - 10, 0, rand() % 20 - 10));
			mSpline->AddControlPoint(vec3(rand() % 20 - 10, 0, rand() % 20 - 10));
			mSpline->AddControlPoint(vec3(rand() % 20 - 10, 0, rand() % 20 - 10));
			p->setSplinePath(mSpline);
			break;
		case PIKACHU:
			p = new OBJModel(*Pikachu);
			mSpline->SetPosition(vec3(r2, 0, r3));
			mSpline->SetRotation(vec3(1.0, 0.0, 0.0), 180);
			mSpline->AddControlPoint(vec3(rand() % 20 - 10, 0, rand() % 20 - 10));
			mSpline->AddControlPoint(vec3(rand() % 20 - 10, 0, rand() % 20 - 10));
			mSpline->AddControlPoint(vec3(rand() % 20 - 10, 0, rand() % 20 - 10));
			mSpline->AddControlPoint(vec3(rand() % 20 - 10, 0, rand() % 20 - 10));
			p->setSplinePath(mSpline);
			break;
		case SQUIRTLE:
			p = new OBJModel(*Squirtle);
			mSpline->SetPosition(vec3(r2, 0, r3));
			mSpline->SetRotation(vec3(1.0, 0.0, 0.0), 180);
			mSpline->AddControlPoint(vec3(rand() % 20 - 10, 0, rand() % 20 - 10));
			mSpline->AddControlPoint(vec3(rand() % 20 - 10, 0, rand() % 20 - 10));
			mSpline->AddControlPoint(vec3(rand() % 20 - 10, 0, rand() % 20 - 10));
			mSpline->AddControlPoint(vec3(rand() % 20 - 10, 0, rand() % 20 - 10));
			p->setSplinePath(mSpline);
			break;
		case ZUBAT:
			p = new OBJModel(*Zubat);
			
			mSpline->SetPosition(vec3(r2, rHeight, r3));
			mSpline->SetRotation(vec3(1.0, 0.0, 0.0), 180);
			mSpline->AddControlPoint(vec3(rand() % 10 + 20, 0.0, rand() % 10 - 20));
			mSpline->AddControlPoint(vec3(rand() % 10 - 20, 10.0, rand() % 10 + 20));
			mSpline->AddControlPoint(vec3(rand() % 10 + 20, 5.0, rand() % 10 -20));
			mSpline->AddControlPoint(vec3(rand() % 10 + 20, 0.0, rand() % 10 - 20));
			p->setSplinePath(mSpline);

		

			break;
		}

		if (p->getsplinepath() == nullptr)
			p->SetPosition(vec3(r2, 0, r3));
		p->SetRotation(vec3(0, 1, 0), (float)r4);
		p->SetScaling(vec3(0.4f, 0.4f, 0.4f));
		pokemon.push_back(p);
	}

	return pokemon;
}