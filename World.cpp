#include "World.h"

//
// Create the world
//
World::World()
{
	// Set up the predator social network
	for (int i = 0; i < NPRED; ++i)
	{
		for (int j = 0; j < NPRED; ++j)
		{
			if (i != j)
			{
				if (i < j)
				{
					socialNetwork[i][j] = double(rand()) / RAND_MAX - 0.5;
				}
				else
				{
					// this keeps the network symmetrical - we will want to vary that
					socialNetwork[i][j] = socialNetwork[j][i];
				}
			}
			else
			{
				socialNetwork[i][j] = 0.0;
			}
		}
	}
}

//
//
World::~World(void)
{
}

//
//
void World::addPredator(Predator* p, int i)
{
	predators[i] = p;
}

//
//
void World::addPrey(Prey* p, int i)
{
	prey[i] = p;
}

//
// Calculate the inter-agent distances
//
void World::updateDistances(void)
{
	// Distances between predators
	for (int i = 0; i < NPRED; ++i)
	{
		for (int j = 0; j < NPRED; ++j)
		{
			distPred[i][j] = predators[i]->getPosition().getDistance(predators[j]->getPosition());
		}
	}

	// Distances between prey
	for (int i = 0; i < NPREY; ++i)
	{
		for (int j = 0; j < NPREY; ++j)
		{
			distPrey[i][j] = prey[i]->getPosition().getDistance(prey[j]->getPosition());
		}
	}

	// Distances between predators and prey
	for (int i = 0; i < NPRED; ++i)
	{
		for (int j = 0; j < NPREY; ++j)
		{
			distPredPrey[i][j] = predators[i]->getPosition().getDistance(prey[j]->getPosition());
		}
	}


}

//
//
void		World::getSocialNetwork(void* sn) 
{ 
	memcpy(sn, socialNetwork, sizeof(double) * NPRED * NPRED); 
};


//
// Return a list of predators who are targetting a particular prey agent
//
std::vector<int> World::getTeamForTarget(int i)
{
	std::vector<int> team;
	for (int j = 0; j < NPRED; ++j)
	{
		if (predators[j]->getTarget() == i)
		{
			team.push_back(j);
		}
	}

	return team;
}

//
// Return the sum of a social network for a team
//
double World::sumSocialNetworkForTeam(std::vector<int> team)
{
	double sum = 0.0;
	std::vector<int>::iterator it1, it2;
	for (it1 = team.begin(); it1 != team.end(); ++it1)
	{
		for (it2 = team.begin(); it2 != team.end(); ++it2)
		{
			sum += socialNetwork[*it1][*it2];
		}
	}

	return sum;
}

//
// Calculate the entropy of a social network for a team (not used)
double World::entropyOfSocialNetworkForTeam(std::vector<int> team)
{
	double entropy = 0.0;
	double sum = sumSocialNetworkForTeam(team);
	double x;

	std::vector<int>::iterator it1, it2;
	for (it1 = team.begin(); it1 != team.end(); ++it1)
	{
		for (it2 = team.begin(); it2 != team.end(); ++it2)
		{
			x = socialNetwork[*it1][*it2];
			if (x > 0.0)
				entropy += x * log(x);
		}
	}

	entropy = -entropy / log(team.size());

	return entropy;
}

//
// Calculate the entropy of the whole social network (not used)
//
double World::entropyOfWholeSocialNetwork(void)
{
	double entropy = 0.0;
	double x;
	int count = 0;

	for (int i = 0; i < NPRED; ++i)
	{
		for (int j = 0; j < NPREY; ++j)
		{
			x = socialNetwork[i][j];
			if (x > 0.0)
			{
				entropy -= x * log(x);
				++count;
			}
		}
	}

	entropy = entropy / log(count);

	return entropy;
}
