#pragma once
#include "Job.h"
#include <thread>
#include <queue>
#include <mutex>
#include <utility>
#include <chrono>
#include <exception>
#include <iostream>

class Worker : public std::enable_shared_from_this<Worker>
{
public:
	Worker() {};
	virtual ~Worker() 
	{
		std::cout << "Worker Dtor" << std::endl;
		if (_thread.joinable()) _thread.join();
	};
	
public:
	void Start()
	{
		std::cout << "Worker Start called\n";

		_thread = std::thread([self = shared_from_this()]()
			{
				std::cout << "Worker thread started\n";

				try
				{
					while (true)
					{
						self->ExecuteJob();
						//std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 20fps
					}
				}
				catch (const std::exception& e)
				{
					std::cerr << "Worker exception: " << e.what() << "\n";
				}
			});
	}

	virtual void ExecuteJob()
	{
		if (!_jobQueue.empty())
		{
			auto job = PopJob();
			job->Execute();
		}		
	}

	void PushJob(std::shared_ptr<Job> job)
	{
		std::lock_guard<std::recursive_mutex> lock(_lock);
		_jobQueue.push(job);
	}

	std::shared_ptr<Job> PopJob()
	{
		std::lock_guard<std::recursive_mutex> lock(_lock);
		auto job = _jobQueue.front();
		_jobQueue.pop();
		return job;
	}
public:
	const size_t Size() noexcept
	{
		return _jobQueue.size();
	}

protected:	
	std::recursive_mutex _lock{};
	std::thread _thread;

private:
	std::queue<std::shared_ptr<Job>> _jobQueue{};
};

