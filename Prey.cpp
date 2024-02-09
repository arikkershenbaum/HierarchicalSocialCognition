#include "Prey.h"
#include "Animal.h"
#include "World.h"

//
// Create a prey object
//
void Prey::Initialise(void)
{
	set_mass(1.0);
	set_coeff_of_friction(2.0);
	type = eAnimalPrey;

	// create all the forces (there are one for each predator, plus one for the each prey)
	Position nopos;
	for (int i = 0; i < NPRED+NPREY; ++i)
	{
		forces.push_back(nopos);
	}

	position.setPos(position[0] * PREY_POSITION_SCALE, 0);
	position.setPos(position[1] * PREY_POSITION_SCALE, 1);

	speed = PREY_SPEED;
}

//
// Calculate the dynamic forces for movement
//
void Prey::CalculateForces(void)
{
	Prey** prey = world->getPrey();
	Predator** pred = world->getPredators();

	double d;
	Position v;
	// Go through each of the prey agents and calculate the attractive and repulsive forces
	for (int i = 0; i < NPREY; ++i)
	{
		if (prey[i] != this)
		{
			d = position.getDistance(prey[i]->getPosition());
			v = prey[i]->getPosition() - position;

			if (d < MIN_CONSPP_APPROACH)
			{
				// if too close, repel
				forces[i].setPos(-v.getPos(0) / d * CONSPP_FORCE, 0);
				forces[i].setPos(-v.getPos(1) / d * CONSPP_FORCE, 1);
			}
			else
			{
				// if not too close, attract
				forces[i].setPos(v.getPos(0) / d * CONSPP_FORCE, 0);
				forces[i].setPos(v.getPos(1) / d * CONSPP_FORCE, 1);
			}
		}
	}

	// Go through all the predator agents and calculate the repulsive forces
	for (int i = 0; i < NPRED; ++i)
	{
		d = position.getDistance(pred[i]->getPosition());
		v = pred[i]->getPosition() - position;
		
		forces[NPREY + i].setPos(FORCE_FACTOR * -v.getPos(0) / d * PREDATOR_FORCE, 0);
		forces[NPREY + i].setPos(FORCE_FACTOR * -v.getPos(1) / d * PREDATOR_FORCE, 1);
	}
}