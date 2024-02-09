#include "Animal.h"

// 
// Create an animal object with a particular position and direction
//
Animal::Animal(World* w, int i, Position p, double s, Position d, double f)
{
	world = w;
	//	perceived = WorldView(w);
	id = i;
	position = p;
	prevpos = p;
	speed = s;
	direction = d;
	fitness = f;
	type = eAnimalAnimal;

	speed = speed * (1.0 / (1.0 + exp(-8 * (0.8 - 2 * 0.15))));
}

//
// Create an animal object with random position and direction
//
Animal::Animal(World* w, int i)
{
	world = w;
	//	perceived = WorldView(w);
	id = i;

	position.setPos(double(rand()) / RAND_MAX - 0.5, 0);
	position.setPos(double(rand()) / RAND_MAX - 0.5, 1);
	prevpos = position;
	speed = double(rand()) / RAND_MAX;
	direction.setPos(double(rand()) / RAND_MAX - 0.5, 0);
	direction.setPos(double(rand()) / RAND_MAX - 0.5, 1);
	fitness = 1;
	type = eAnimalAnimal;

	speed = speed * (1.0 / (1.0 + exp(-8 * (0.8 - 2 * 0.15))));
}

//
// Move the animal for one simulation time step
//
void Animal::MoveStep(void)
{
	++timeidx;

	Position f,p;
	CalculateForces();

	for (std::vector<Position>::iterator it = forces.begin(); it != forces.end(); it++)
	{
		p = *it;
		f.setPos(f[0] + p[0], 0);
		f.setPos(f[1] + p[1], 1);
	}

	Position d;
	d.setPos(direction[0] * speed + f[0], 0);
	d.setPos(direction[1] * speed + f[1], 1);

	double q = sqrt(d[0] * d[0] + d[1] * d[1]);

	direction.setPos(d[0] / q, 0);
	direction.setPos(d[1] / q, 1);

	prevpos = position;

	position.setPos(position[0] + direction[0] * speed, 0);
	position.setPos(position[1] + direction[1] * speed, 1);
}