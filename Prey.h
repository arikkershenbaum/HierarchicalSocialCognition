#pragma once
#include "Animal.h"

class Animal;
class World;

class Prey :
	public Animal
{
public:
	Prey(World* w, int i, Position p, double s, Position d, double f)
		: Animal(w, i, p, s, d, f)
	{
		Initialise();
	}

	Prey(World* w, int i)
		: Animal(w, i)
	{
		Initialise();
	}

	void CalculateForces(void);

protected:

	void Initialise(void);
};

