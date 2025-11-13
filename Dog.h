#pragma once
#include "Animal.h"
#include <iostream>

class Dog : public Animal
{
public:
	virtual void print_sound() override
	{
		std::cout << "Dog Sound  \n";
	}
};
