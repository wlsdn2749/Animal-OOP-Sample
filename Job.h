#pragma once
#include <functional>
#include <memory>
#include <cstdint>
#include <utility>

class Animal;

class Job
{
public:
	template<typename Func = std::function<void(void)>>
	Job(Func&& func)
		: _func(std::move(func))
	{
		;
	}

public:
	virtual void Execute()
	{
		_func();
	}

	std::function<void(void)> GetFunc()
	{
		return _func;
	}
	
	virtual int32_t GetThreadId()
	{
		return -1;
	}

protected:
	std::function<void(void)> _func;
};

class AnimalJob : public Job
{
public:
	template<typename Func = std::function<void(void)>>
	AnimalJob(std::shared_ptr<Animal> animal, Func&& func)
		: _animal(animal)
		, Job(std::forward<Func>(func)) // Job의 생성자 그대로 사용
	{

	}
	virtual ~AnimalJob()
	{

	}

public:
	virtual void Execute() override
	{
		_func();
	}

	virtual int32_t GetThreadId() override;

	std::shared_ptr<Animal> GetAnimal() { return _animal; }

private:
	std::shared_ptr<Animal> _animal;

	
};