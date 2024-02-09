#pragma once
#include "Animal.h"
#include "World.h"
#include "Cognition.h"
#include "PreyCornered.h"
#include "Params.h"
#include "Predator.h"

const int MAX_TIME = 8000; // Maximum run time of the simulation (in steps)

double MIN_CONSPP_APPROACH = 0.5; // closer than this, prey repel each other
double FORCE_FACTOR = 0.005; 
double CONSPP_FORCE = 0.01; // force between prey(attractive or repulsive)
double PREDATOR_FORCE = 6.0; // repulsive force between preyand predators
double PREY_SPEED = 0.01; // prey speed...
double PREY_POSITION_SCALE = 10.0; // how spread out the prey are in their initial positions
double FORCE_PREDATOR_PREDATOR = 0.5; // attractive/repulsive intraspecific force for predators
double FORCE_PREDATOR_PREY = 0.2; // attractive force for predators towards prey
double D_PREDATOR = 1.5; // more parameters from Escobedo, et al (2014). doi:10.1098/rsif.2014.0204
double D_PREY = 1.0;
double PREDATOR_POSITION_SCALE = 50.0; // how spread out the predators are in their initial positions
double PREDATOR_SPEED = 0.04;
double DEFAULT_MASS = 1.0;
double DEFAULT_FRICTION = 1.0;


// Structure to hold the simulation results
struct SimResults
{
	int time;
	double teamSum;
	double teamEntropy;
};


//
// Function that runs the simulation for a particular cognition type
//
SimResults RunSimulation(Predator::SocialCognitionType sc)
{
	const int CORNERED_DURATION = 50;
	const double CORNERED_MARGIN = 5;

	SimResults results;

	World	w;
	Position p;
	Position d;

	// Create the predators
	for (int i = 0; i < NPRED; ++i)
	{
		w.addPredator(new Predator(&w, i, sc), i);
	}

	// Create the prey
	for (int i = 0; i < NPREY; ++i)
	{
		w.addPrey(new Prey(&w, i + NPRED), i);
	}

	Predator** preds = w.getPredators();
	Prey** prey = w.getPrey();

	// Create an object for checking wether the prey has been killed
	PreyCornered PC(&w, prey, preds, CORNERED_DURATION, CORNERED_MARGIN);

	// Perform the simulation for the stated number of time steps
	int t;
	for (t = 0; t < MAX_TIME; ++t)
	{
		// Have each predator choose a target and then move
		for (int i = 0; i < NPRED; ++i)
		{
			preds[i]->ChooseTarget();
			preds[i]->MoveStep();
		}

		// Have each prey move
		for (int i = 0; i < NPREY; ++i)
		{
			prey[i]->MoveStep();
		}

		// Check if the prey has been killed
		if (PC.Success())
			break;
	}

	// Summarise the results
	results.time = t;
	results.teamSum = w.sumSocialNetworkForTeam(w.getTeamForTarget(PC.getVictim()));
	results.teamEntropy = w.entropyOfWholeSocialNetwork();

	//  Clean up
	for (int i = 0; i < NPRED; ++i)
	{
		delete preds[i];
	}

	for (int i = 0; i < NPREY; ++i)
	{
		delete prey[i];
	}

	return results;

}


//
// Run the necessary number of simulations for a set of parameters and write the results to a file
//
int Try(Predator::SocialCognitionType sc, int NUM_SIMS)
{
	// Arrays to save the simulation results
	int* T = new int[NUM_SIMS]; // Time to kill
	double* S = new double[NUM_SIMS]; // Sum of team social network
	double* E = new double[NUM_SIMS]; // Entropy of team social network
	int ncatch = 0;
	SimResults results;


	// Run the simulation repeatedly
	for (int i = 0; i < NUM_SIMS; ++i)
	{
		fprintf(stdout, "%d\t%d\t%d\n", (int)sc, i, ncatch);

		results = RunSimulation(sc);

		T[i] = results.time;
		S[i] = results.teamSum;
		E[i] = results.teamEntropy;

		// Keep track of how many prey have been killed
		if (T[i] < MAX_TIME)
			++ncatch;
	}


	// Write the results to file
	FILE* fp;
	char fname[256];
	sprintf_s(fname, "timesAndEntropyW_npred%d_nprey%d_sc%d.txt", NPRED, NPREY, (int)sc);

	fopen_s(&fp, fname, "wt");
	for (int i = 0; i < NUM_SIMS; ++i)
	{
		fprintf(fp, "%d\t%f\t%f\n", T[i], S[i],E[i]);
	}
	fclose(fp);


	delete[] T;
	delete[] S;
	delete[] E;

	return ncatch;

}


//
// main
//
int main(void)
{
	// Number of simulations to run for each set of parameters
	const int NUMSIM = 10000;

	// These 
	double sconfusion = 10.0;
	double sfactor = 0.5;

	Try(Predator::SocialCognitionType::eSCnone, NUMSIM);
	Try(Predator::SocialCognitionType::eSClone, NUMSIM);
	Try(Predator::SocialCognitionType::eSCpair, NUMSIM);
	Try(Predator::SocialCognitionType::eSCall, NUMSIM);
	

	return 0;
}

