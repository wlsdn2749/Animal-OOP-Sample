#pragma once
#include "Animal.h"
#include <iostream>

class Pig : public Animal
{
public:
	virtual void print_sound() override
	{
		std::cout << "Pig Sound \n";
	}
};

