#pragma once
#include <queue>
#include "Job.h"
#include <mutex>
#include <chrono> 
#include <memory>
#include <iostream>

using JobTimerSharedPtr = std::shared_ptr<JobTimer>;
using namespace std::chrono;


// compare(a,b) - 더 빠른 시간이 최상위로 정렬
struct compare
{
	bool operator()(JobTimerSharedPtr a_, JobTimerSharedPtr b_)
	{
		return a_->GetTime() > b_->GetTime();
	}
};
class JobTimerQueue
{
public:
	JobTimerSharedPtr Pop()
	{
		std::lock_guard<std::mutex> lock(_lock);

		auto now = steady_clock::now();
		auto nowMs = duration_cast<milliseconds>(now.time_since_epoch()).count();
		auto jobTimer = Top();
		//std::cout << "nowMs: " << nowMs + 1000 << "jobTimer ms:" << jobTimer->GetTime() << std::endl;
		if (jobTimer && jobTimer->GetTime() <= nowMs)
		{
			_jobTimerQueue.pop();
			return jobTimer;
		}
		return nullptr; // 가장빠른 시간이없는경우

	}

	void Push(JobTimerSharedPtr jobtimer)
	{
		std::lock_guard<std::mutex> lock(_lock);
		_jobTimerQueue.emplace(jobtimer);
	}

	bool Empty() const
	{
		return _jobTimerQueue.empty();
	}

	JobTimerSharedPtr Top()
	{
		return _jobTimerQueue.top();
	}


private:
	std::priority_queue<JobTimerSharedPtr, std::vector<JobTimerSharedPtr>, compare> _jobTimerQueue;
	std::mutex _lock;

};

