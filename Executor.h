#pragma once
#include <thread>
#include <vector>
#include "Worker.h"
#include "Singleton.h"
#include <memory>

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
	}

	void StartAll()
	{
		for (auto& worker : _workers)
		{
			worker->Start();
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
		auto worker = _workers[id % _workers.size()];
		auto job = std::make_shared<Job>(std::forward<Func>(func));
		worker->PushJob(job);
	}
public:
	const size_t GetWorkerSize() noexcept
	{
		return _workers.size();
	}

private:
	std::vector<std::shared_ptr<Worker>> _workers;
};

