#pragma once
#include "Cognition.h"
#include "Predator.h"
#include "Animal.h"

class Predator;

class PredatorCognition :
    public Cognition
{
public:
    PredatorCognition(Predator* p) : Cognition(p) {};
};

