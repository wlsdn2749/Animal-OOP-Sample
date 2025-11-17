#pragma once
#include <thread>
#include <vector>
#include "Worker.h"
#include "Singleton.h"
#include <memory>
#include <chrono>
#include "Job.h"

class Animal;

using namespace std::chrono;

class Executor : public Singleton<Executor>
{
public:
	inline static JobTimerQueue globalJobTimerQueue{};
	inline static void LaunchJobTimerQueue()
	{
		_JobTimerQueueThread = std::thread([]()
			{
				std::cout << "JobTimerQueue thread started" << "\n";

				try
				{
					while (true)
					{
						
						auto job_vec = globalJobTimerQueue.PopAll(); // 이미 시간이 된경우
						for (auto job : job_vec)
						{
							if (job)
								Executor::Handle_JobTimer(job);
						}		
					
						std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 20fps
					}
				}
				catch (const std::exception& e)
				{
					std::cerr << "Worker Exception: " << e.what() << "\n";
				}
			});
	}

	inline static void Handle_JobTimer(JobSharedPtr job)
	{
		Executor::Instance().Post(job->GetThreadId(), job->GetFunc()); // JobTimer -> Job으로 연결
	}

	inline static std::thread	_JobTimerQueueThread{};

public:
	void initialize(size_t count)
	{
		_workers.reserve(count);
		for (int i = 0; i < count; ++i)
		{
			_workers.emplace_back(std::make_shared<Worker>());
		}

	}

	void StartAll()
	{
		for (auto worker : _workers)
		{
			worker->Start();
		}
	}

public:
	template <typename Func>
	void Post(uint32_t threadKey, Func&& func, int64_t t = 0)
	{
		auto worker = _workers[threadKey % _workers.size()];
		auto job = std::make_shared<Job>(std::forward<Func>(func));


		if (t <= 0) // 즉시 시작
		{
			worker->PushJob(job);
		}
		else
		{
			Executor::globalJobTimerQueue.Push(job, t);
		}
	}

public:
	const size_t GetWorkerSize() noexcept
	{
		return _workers.size();
	}

private:
	std::vector<std::shared_ptr<Worker>>		_workers;

};

