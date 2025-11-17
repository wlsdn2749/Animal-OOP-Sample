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
		return 0;
	}

protected:
	std::function<void(void)> _func;
};

class AnimalJob : public Job
{
public:
	template<typename Func = std::function<void(void)>>
	AnimalJob(Func&& func, std::shared_ptr<Animal> animal)
		: Job(std::forward<Func>(func)) // Job의 생성자 그대로 사용
		, _animal(animal)
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

private:
	std::shared_ptr<Animal> _animal;
};