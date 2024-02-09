#pragma once
#include "Predator.h"
#include "Animal.h"
#include "World.h"
#include "Params.h"

#include <vector>

double SOCIAL_CONFUSION = 10.0;
double SOCIAL_FACTOR = 0.5;

//
// Initialise the predator instance
//
void Predator::Initialise(SocialCognitionType sc)
{
	set_mass(0.1);
	set_coeff_of_friction(1.0);
	targetid = -1;
	type = eAnimalPredator;

	currentscore = 0.0;

	// create all the forces (there are one for each predator, plus one for the chosen prey)
	Position nopos;
	for (int i = 0; i < NPRED + 1; ++i)
	{
		forces.push_back(nopos);
	}

	position.setPos(position[0] * PREDATOR_POSITION_SCALE, 0);
	position.setPos(position[1] * PREDATOR_POSITION_SCALE, 1);

	speed = PREDATOR_SPEED;


	cognition = sc;
	// Get the social network from the world, and prune it according to my cognition
	world->getSocialNetwork(&socialNetwork);
	memcpy(actualSocialNetwork, socialNetwork, sizeof(double) * NPRED * NPRED);

	switch (cognition)
	{
	case eSCnone:
		for (int i = 0; i < NPRED; ++i)
			for (int j = 0; j < NPRED; ++j)
				socialNetwork[i][j] = 0.0;
		break;
	case eSClone:
		for (int i = 0; i < NPRED; ++i)
			if (i != id)
				for (int j = 0; j < NPRED; ++j)
					socialNetwork[i][j] = 0.0;
		break;
	case eSCpair:
		for (int i = 0; i < NPRED; ++i)
			for (int j = 0; j < NPRED; ++j)
				if (i != id && j != id)
					socialNetwork[i][j] = 0.0;
		break;
	case eSCall:
		break;
	}

}

//
// For calculating the dynamic physical forces (for movement)
//
void Predator::CalculateForces(void)
{
	Prey** prey = world->getPrey();
	Predator** pred = world->getPredators();

	Position dp;
	double norm2;

	// clear all the forces (there are one for each predator, plus one for the chosen prey)
	Position nopos;
	for (int i = 0; i < NPRED + 1; ++i)
	{
		forces[i] = nopos;
	}

	// Predator-Prey forces
	if (targetid != -1)
	{
		dp = position - prey[targetid]->getPosition();
		norm2 = dp[0] * dp[0] + dp[1] * dp[1];
		forces[0].setPos(FORCE_PREDATOR_PREY * (-dp[0] / norm2 + dp[0] * D_PREY * D_PREY / norm2 / norm2), 0);
		forces[0].setPos(FORCE_PREDATOR_PREY * (-dp[1] / norm2 + dp[1] * D_PREY * D_PREY / norm2 / norm2), 1);
	}

	// Inter-predator forces


	// interpredator forces only apply to predators targeting the same animal.
	double dist_j, phi_ij;
	Position delta_ij;
	Position f;


	if (targetid != -1)
	{
		double dist_i = position.getDistance(prey[targetid]->getPosition());

		// Go through each of the predators...
		for (int i = 0; i < NPRED; ++i)
		{
			// except ourselves...
			if (i != id)
			{
				// if they are chasing the same prey:
				if (world->getPredators()[i]->getTarget() == targetid)
				{
					dist_j = pred[i]->getConfusedPosition(actualSocialNetwork[id][i]).getDistance(prey[targetid]->getPosition());
					delta_ij = position - pred[i]->getConfusedPosition(actualSocialNetwork[id][i]);

					phi_ij = exp(-FORCE_PREDATOR_PREDATOR * ((dist_i - D_PREDATOR) * (dist_i - D_PREDATOR) + (dist_j - D_PREDATOR) * (dist_j - D_PREDATOR)));

					norm2 = delta_ij[0] * delta_ij[0] + delta_ij[1] * delta_ij[1];
					f.setPos(FORCE_PREDATOR_PREDATOR * phi_ij * -(-delta_ij[0] / norm2) + actualSocialNetwork[id][i] * SOCIAL_PUSH_FACTOR, 0);
					f.setPos(FORCE_PREDATOR_PREDATOR * phi_ij * -(-delta_ij[1] / norm2) + actualSocialNetwork[id][i] * SOCIAL_PUSH_FACTOR, 1);
				}
				else
				{
					f.setPos(0.0, 0);
					f.setPos(0.0, 1);
				}

				forces[i + 1] = f;
			}
		}
	}
}


//
// Choose a prey animal to target
//
void Predator::ChooseTarget(void)
{
	Prey** prey = world->getPrey();
	Predator** pred = world->getPredators();

	double scoresDist[NPREY];
	double scoresNet[NPREY];
	double mydist;

	// Find the score by distance to the prey

	for (int i = 0; i < NPREY; ++i)
	{
		mydist = position.getDistance(prey[i]->getPosition());
		scoresDist[i] = 1.0 / mydist;
	}

	// Find the score by consideration of the social network
	for (int i = 0; i < NPREY; ++i)
	{
		// Find the team of predators chasing this prey (including me, hypothetically)
		std::vector<int> team = world->getTeamForTarget(i);
		if (targetid != i)
		{
			team.push_back(id);
		}

		// sum the SN for that team
		scoresNet[i] = sumSocialNetworkForTeam(team) * team.size();
	}

	// balance distance and network
	double totalScore[NPREY];
	double bestscore = 0.0;
	int bestprey = -1;
	for (int i = 0; i < NPREY; ++i)
	{
		totalScore[i] = (1 - SOCIAL_FACTOR) * scoresDist[i] + SOCIAL_FACTOR * scoresNet[i];
		if (totalScore[i] > bestscore)
		{
			bestscore = totalScore[i];
			bestprey = i;
		}
	}

	if (bestscore > currentscore * 1.1) // hysteresis
	{
		targetid = bestprey;
	}
	currentscore = totalScore[targetid];
}

//
// Return the social network for the current team of which this predator is a part
//
double Predator::sumSocialNetworkForTeam(std::vector<int> team)
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
// Returns the position of the predator with added error, depending on the social network
//
Position Predator::getConfusedPosition(double confusion)
{
	// A positive social network should give little error. Transform first with a sigmoid
	confusion = (1.0 - 1.0 / (1 + exp(-confusion * SOCIAL_CONFUSION))) * SOCIAL_CONFUSION;
	Position pos = position;
	pos.setPos(pos[0] + confusion, 0);
	pos.setPos(pos[1] + confusion, 1);

	return pos;
}