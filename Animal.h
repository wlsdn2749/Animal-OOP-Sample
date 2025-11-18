#pragma once
#include <iostream>
#include <atomic>
#include "Executor.h"
#include "GlobalTime.h"

class AnimalJob;

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
		std::cout <<"CurrentTime: " << TimeUtil::GetCurrentTime() << "Animal" << " " << "Thread ID " << std::this_thread::get_id() << " ID: " << _id << " Age: " << GetAge() << std::endl;
	}
	virtual void onUpdate()
	{
		_age++;
		print_age();
	}

	void Update()
	{
		onUpdate(); // virtual 

		PostDelay<AnimalJob>(shared_from_this(), 1000, [self = shared_from_this()]()
			{
				self->Update();
			});

	};
	
	virtual void updateThreadKey()
	{
		_threadKey = std::abs(_threadKey - 1); // 1이면 0, 0이면 1
	}


	void Init()
	{
		// 업데이트 수행시작
		Executor::Instance().Post(_threadKey, [self = shared_from_this()]()
			{
				self->Update();
			});
	}

	template <typename T, typename... Args>
	void Post(Args&&... args)
	{
		/*auto job = std::make_shared<T>(std::forward<Args>(args)...);

		Executor::Instance().Post<T>(std::forward<Args>(args)...);*/
		;
	}

	template<typename T, typename... Args>
	void PostDelay(std::shared_ptr<Animal> animal, int64_t t, Args&&... args)
	{
		auto job = std::make_shared<T>(animal, std::forward<Args>(args)...);

		Executor::Instance().PostDelay(job->GetThreadId(), t, job);
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

	const int GetThreadKey()
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

