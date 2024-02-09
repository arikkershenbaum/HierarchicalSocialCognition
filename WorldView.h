#pragma once

#include "World.h"
#include "Animal.h"

class World;
class Animal;

class WorldView
{
protected:
	World* world;
	Animal* animal;
	int id;

	float visRange;

	int visPredP;
	int visPreyP;
};

