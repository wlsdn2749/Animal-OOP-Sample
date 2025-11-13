#pragma once
#include <functional>


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

private:
	std::function<void(void)> _func;
};


//class JobTimer
//{
//public:
//	template<typename Func = std::function<void(void)>>
//	JobTimer(Func&& func, int t)
//		: _func(std::move(func)), _t(t)
//	{
//
//	}
//
//public:
//	void ExecuteJobTimer()
//	{
//		
//	}
//
//private:
//	std::function<void(void)> _func;
//	int _t{};
//};
