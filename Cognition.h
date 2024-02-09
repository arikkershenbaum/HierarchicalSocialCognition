#pragma once

#include "Animal.h"

class Animal;

class Cognition
{
public:
	Cognition(Animal* a)
	{
		animal = a;
	}

	void ChooseTarget(void);

protected:
	Animal* animal;
};

