#pragma once
#include <thread>
#include <vector>
#include "Worker.h"
#include "TimerWorker.h"
#include "Singleton.h"
#include <memory>
#include <chrono>

class Animal;

using namespace std::chrono;

class Executor : public Singleton<Executor>
{
public:
	inline static JobTimerQueue globalJobTimerQueue {};
	inline static void LaunchJobTimerQueue()
	{
		_JobTimerQueueThread = std::thread([]()
			{
				std::cout << "JobTimerQueue thread started" << "\n";

				try
				{
					while (true)
					{
						while (!globalJobTimerQueue.Empty())
						{
							auto jobTimer = globalJobTimerQueue.Pop(); // 이미 시간이 된경우
							if (jobTimer)
								Executor::Handle_JobTimer(jobTimer);
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

	inline static void Handle_JobTimer(JobTimerSharedPtr jobTimer)
	{
		Executor::Instance().Execute(jobTimer->GetAnimalThreadKey(), jobTimer->GetFunc()); // JobTimer -> Job으로 연결
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
	void Execute(uint32_t threadId, Func&& func)
	{
		auto worker = _workers[threadId % _workers.size()];
		auto job = std::make_shared<Job>(std::forward<Func>(func));
		worker->PushJob(job);
	}

	template <typename Func>
	void ExecuteTimer(uint32_t threadId, Func&& func, int t)
	{
		auto now		= steady_clock::now();
		auto ms			= duration_cast<milliseconds>(now.time_since_epoch()).count();
		auto jobTimer	= std::make_shared<JobTimer>(std::forward<Func>(func), threadId, ms + static_cast<int64_t>(t));

		Executor::globalJobTimerQueue.Push(jobTimer);
	}


public:
	const size_t GetWorkerSize() noexcept
	{
		return _workers.size();
	}

private:
	std::vector<std::shared_ptr<Worker>>		_workers;

};

