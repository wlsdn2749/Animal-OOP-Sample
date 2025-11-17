#pragma once
#include <thread>
#include <vector>
#include "Worker.h"
#include "TimerWorker.h"
#include "Singleton.h"
#include <memory>
#include <chrono>

using namespace std::chrono;

class Executor : public Singleton<Executor>
{
public:
	void initialize(size_t count)
	{
		_workers.reserve(count);
		for (int i = 0; i < count; ++i)
		{
			_workers.emplace_back(std::make_shared<Worker>());
		}

		for (int i = 0; i < count; ++i)
		{
			_timerWorkers.emplace_back(std::make_shared<TimerWorker>());
		}
	}

	void StartAll()
	{
		for (auto worker : _workers)
		{
			worker->Start();
		}

		for (auto timerWorker : _timerWorkers)
		{
			timerWorker->Start();
		}
	}

public:
	template <typename Func>
	void Execute(uint32_t id, Func&& func)
	{
		auto worker = _workers[id % _workers.size()];
		auto job = std::make_shared<Job>(std::forward<Func>(func));
		worker->PushJob(job);
	}

	template <typename Func>
	void Execute(uint32_t id, Func&& func, int t)
	{
		auto timerrWorker = _timerWorkers[id % _workers.size()];
		auto now = steady_clock::now();
		auto ms = duration_cast<milliseconds>(now.time_since_epoch()).count();
		auto jobTimer = std::make_shared<JobTimer>(std::forward<Func>(func), ms + static_cast<int64_t>(t));
		timerrWorker->PushJob(jobTimer);
	}
public:
	const size_t GetWorkerSize() noexcept
	{
		return _workers.size();
	}

	const size_t GetTimerWorkerSize() noexcept
	{
		return _timerWorkers.size();
	}

private:
	std::vector<std::shared_ptr<Worker>>		_workers;
	std::vector<std::shared_ptr<TimerWorker>>	_timerWorkers;
};

