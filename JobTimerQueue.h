#pragma once
#include <queue>
#include "Job.h"
#include <mutex>
#include <chrono> 
#include <memory>
#include <iostream>
#include "GlobalTime.h"

using JobSharedPtr = std::shared_ptr<Job>;
using JobTimePair = std::pair<JobSharedPtr, int64_t>; // (JobSharedPtr, Time)


// compare(a,b) - 더 빠른 시간이 최상위로 정렬
struct compare
{
	bool operator()(JobTimePair a_, JobTimePair b_)
	{
		return a_.second > b_.second;
	}
};
class JobTimerQueue
{
public:
	JobSharedPtr Pop()
	{
		std::lock_guard<std::mutex> lock(_lock);

		auto nowMs = TimeUtil::GetCurrentTime();
		auto jobTime = Top(); // pair
		//std::cout << "nowMs: " << nowMs + 1000 << "jobTimer ms:" << jobTimer->GetTime() << std::endl;
		if (jobTime.first && jobTime.second <= nowMs)
		{
			_job_pq.pop();
			return jobTime.first;
		}
		return nullptr; // 가장빠른 시간이없는경우
	}

	std::vector<JobSharedPtr> PopAll()
	{
		auto nowMs = TimeUtil::GetCurrentTime();
		std::vector<JobSharedPtr> job_vec;

		std::lock_guard<std::mutex> lock(_lock);
		while (!_job_pq.empty())
		{
			auto jobTime = Top(); // pair
			if (jobTime.first && jobTime.second <= nowMs)
			{
				_job_pq.pop();
				job_vec.emplace_back(jobTime.first);
			}
			else
			{
				break;
			}
		}
		return job_vec;
	}

	void Push(JobSharedPtr job, int64_t t)
	{
		std::lock_guard<std::mutex> lock(_lock);
		_job_pq.emplace(JobTimePair{job, TimeUtil::GetCurrentTime() + t});
	}

	bool Empty() const
	{
		return _job_pq.empty();
	}

	JobTimePair Top()
	{
		return _job_pq.top();
	}


private:
	std::priority_queue<JobTimePair, std::vector<JobTimePair>, compare> _job_pq;
	std::mutex _lock;

};

