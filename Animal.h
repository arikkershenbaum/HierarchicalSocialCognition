#pragma once
#include <vector>
#include "Position.h"
#include "Params.h"

class World;
class WorldView;

//
// The Animal class includes both predators and prey. The common functions are to do with movement

class Animal
{
public:
	enum AnimalType { eAnimalAnimal, eAnimalPredator, eAnimalPrey };

	Animal(World* w, int i, Position p, double s, Position d, double f);
	Animal(World* w, int i);

	void MoveStep(void);

	virtual void CalculateForces(void) {};

	inline void set_mass(double m) { mass = m; }
	inline void set_coeff_of_friction(double f) { coeff_friction = f; }
	inline void set_max_velocity(double v) { v_max = v; }
	inline Position getPosition(void) { return position; };
	inline double getPosition(int i) { return position[i]; };

protected:
	World* world;
	WorldView* perceived;
	int id;
	AnimalType	type;

	Position position;
	int path;
	double speed;
	Position  direction;
	double fitness;
	Position  prevpos;
	int timeidx = 0;
	double mass = DEFAULT_MASS;
	double coeff_friction = DEFAULT_FRICTION;
	double v_max;

	std::vector<Position> forces;
};