#pragma once
#include "Worker.h"	
#include "JobTimerQueue.h"
#include <queue>
class TimerWorker : public Worker
{
public:
	TimerWorker() {};
	virtual ~TimerWorker() // 부모 객체를 통해 소멸자가 호출되더라도, Virtual로 실제 객체의 소멸자가 호출
	{
		std::cout << "TimerWorker Dtor" << std::endl;
		if (_thread.joinable()) _thread.join();
	}

public:
	virtual void ExecuteJob() override
	{
		std::lock_guard<std::mutex> lock(_lock);
		if (!_jobTimerQueue.Empty())
		{
			auto job = PopJob();
			if(job)
				job->Execute();
		}
	}

	void PushJob(std::shared_ptr<JobTimer> job)
	{
		std::lock_guard<std::mutex> lock(_lock);
		_jobTimerQueue.Push(job);
	}

	std::shared_ptr<JobTimer> PopJob()
	{
		std::lock_guard<std::mutex> lock(_lock);
		return _jobTimerQueue.Pop();
	}

private:
	JobTimerQueue _jobTimerQueue{};
};

