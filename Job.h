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
	void Execute()
	{
		_func();
	}

	std::function<void(void)> GetFunc()
	{
		return _func;
	}

protected:
	std::function<void(void)> _func;
};


class JobTimer : public Job
{
public:
	template<typename Func = std::function<void(void)>>
	JobTimer(Func&& func, int32_t threadKey, std::shared_ptr<Animal> animal, int64_t t)
		: Job(std::forward<Func>(func)) // 기반 생성자를 명시적으로 호출
		, _threadKey(threadKey)
		, _animal(animal)
		, _t(t)
	{

	}

	bool operator<(const JobTimer& other) const
	{
		return _t < other._t;
	}

public:
	int32_t GetSavedThreadKey()
	{
		return _threadKey;
	}
	int32_t GetAnimalThreadKey();
	int64_t GetTime()
	{
		return _t;
	}

private:
	std::shared_ptr<Animal> _animal;
	int32_t					_threadKey;
	int64_t					_t{};
};
