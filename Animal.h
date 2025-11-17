#pragma once
#include <iostream>
#include <atomic>
#include "Executor.h"

class Animal : public std::enable_shared_from_this<Animal>
{
public:
	
	static int GetIncreasedID()
	{
		static std::atomic<int> ids{};
		return ids.fetch_add(1);
	}

public:
	Animal()
	{
		_id			= Animal::GetIncreasedID();
		_threadKey	= _id % Executor::Instance().GetWorkerSize();

	}

	Animal(int id) : _id(id)
	{

	}
public:
	virtual void print_sound()	{ std::cout << "Pure Animal Cannot print_sound() \n"; };
	virtual void print_name() { std::cout << "Default: print_name" << std::endl; };
	virtual void print_age() 
	{
		std::cout <<"Animal" << " " << "Thread ID " << _threadKey << " ID: " << GetAnimalID() << " Age: " << GetAge() << std::endl;
	}
	virtual void onUpdate()
	{
		_age++;
		print_age();
	}

	void Update()
	{
		onUpdate(); // virtual 

		Executor::Instance().Execute(_threadKey, [self = shared_from_this()]()
			{
				self->Update();
			}, 1000); // 1초에 한번 호출

	};
	
	virtual void updateThreadKey()
	{
		_threadKey = std::abs(_threadKey - 1); // 1이면 0, 0이면 1
	}


	void Init()
	{
		// 업데이트 수행시작
		Executor::Instance().Execute(_threadKey, [self = shared_from_this()]()
			{
				self->Update();
			});
	}

public:
	const int GetAnimalID()
	{
		return _id;
	}

	const int GetAge()
	{
		return _age;
	}

	const int GetThreadkey()
	{
		return _threadKey;
	}

protected:

	int _age{ 0 };
	int _updateCnt{ 0 };
	int _threadKey{ 0 };

private:
	int _id { 0 };
	
	
};

