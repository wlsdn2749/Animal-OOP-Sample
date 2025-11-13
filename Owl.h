#pragma once
#include "Animal.h"
#include <iostream>

class Owl : public Animal
{
public:
	virtual void print_sound() override
	{
		std::cout << "Owl Sound \n";
	}
};

