#pragma once
#include "World.h"
#include "Animal.h"

#include <vector>

class Animal;


// Predator is derived from class Animal
class Predator :
    public Animal
{
public:
    enum SocialCognitionType { eSCnone, eSClone, eSCpair, eSCall }; // The types of social cognition we are investigating

    // Constructors
    Predator(World* w, int i, Position p, double s, Position d, double f, SocialCognitionType sc)
        : Animal(w, i, p, s, d, f)
    {
        Initialise(sc);
    }

    Predator(World* w, int i, SocialCognitionType sc)
        : Animal(w, i)
    {
        Initialise(sc);
    }

    void CalculateForces(void); // For calculating the dynamic physical forces (for movement)
    void ChooseTarget(void); // Function to choose a prey animal to target

    inline int getTarget(void) { return targetid; }

    Position getConfusedPosition(double confusion); // Returns the position of the predator with added error, depending on the social network

protected:
    int targetid; // Prey currently being targetted
    std::vector<int> target_vector;


    void Initialise(SocialCognitionType sc);
    SocialCognitionType cognition;

    double socialNetwork[NPRED][NPRED];
    double actualSocialNetwork[NPRED][NPRED];

    double sumSocialNetworkForTeam(std::vector<int> team);

    double SOCIAL_PUSH_FACTOR = 0.0;// 0.05;
    double currentscore;

};

