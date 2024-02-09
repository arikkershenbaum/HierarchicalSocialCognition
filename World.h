#pragma once
#include "Predator.h"
#include "Prey.h"
#include <map>
#include <string>
#include <vector>
#include <math.h>
#include "Position.h"
#include "Params.h"

class Animal;
class Predator;
class Prey;

//
// Class that describes the simulation world, with all the animals in it
//
class World
{
protected:
	// world objects can be agents or objects
	Predator* predators[NPRED];
	Prey* prey[NPREY];
	int cornered;

	// Distance matrices for the agents
	double distPred[NPRED][NPRED];
	double distPrey[NPREY][NPREY];
	double distPredPrey[NPRED][NPREY];


	int	losPred;
	int losHandlesPoint;
	int losHandlesCircle;

	int bounds; // Extent over which objects are currently distributed
	int ranges; // How far between fartherest objects

	int	timestep; // Note current time step of world : 1, 2, 3, 4, ...

	double socialNetwork[NPRED][NPRED];

public:
		World();
		~World(void);

		void	addPredator(Predator* p, int i);
		void	addPrey(Prey* p, int i);
		inline Predator** getPredators(void) { return &predators[0]; };
		inline Prey** getPrey(void) { return &prey[0]; };

		inline double		getDistancesPred(int a, int b) { return distPred[a][b]; };
		inline double		getDistancesPrey(int a, int b) { return distPrey[a][b]; };
		inline double		getDistancesPredPrey(int a, int b) { return distPredPrey[a][b]; };

		void	updateDistances(void);
		void		getSocialNetwork(void* sn);

		std::vector<int> getTeamForTarget(int i);
		double sumSocialNetworkForTeam(std::vector<int> team);
		double entropyOfSocialNetworkForTeam(std::vector<int> team);
		double entropyOfWholeSocialNetwork(void);
};