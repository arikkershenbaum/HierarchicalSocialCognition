#pragma once
#include "World.h"
#include "Prey.h"
#include "Predator.h"
#include <cmath>

//
// A class for deciding whether any of the prey agents are in a status of being cornered, or "caught"
// To be cornered, the prey should have been surrounded by a certain number of predators for a certain number of turns
//
class PreyCornered
{
public:
	PreyCornered(World* w, Prey** py, Predator** pd, int d, double m)
	{
		world = w;
		prey = py;
		preds = pd;
		minduration = d;
		margin = m;
		victim = -1;

		//minpredators = std::min(3, NPRED);
		minpredators = std::ceil(NPRED / 2.0 + 1e-6);

		for (int i = 0; i < NPREY; ++i)
			dur[i] = 0;
	}

	bool Success(void);
	int getVictim(void) { return victim; }

protected:
	World* world;
	Prey** prey;
	Predator** preds;

	int minpredators;
	int minduration;
	int turns;
	double margin;
	int victim;

	int dur[NPREY];

};

