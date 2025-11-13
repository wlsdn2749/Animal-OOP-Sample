#pragma once
#include "Animal.h"
#include <iostream>

class Duck : public Animal
{
public:
	virtual void print_sound() override
	{
		std::cout << "Duck Sound  \n";
	}
};

