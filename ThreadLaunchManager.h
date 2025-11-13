#pragma once
#include <unordered_map>
#include <thread>
#include <functional>
#include <atomic>
#include <mutex>
#include <utility>

class ThreadLaunchManager
{
public:
	static ThreadLaunchManager& Instance()
	{
		static ThreadLaunchManager _threadManager;
		return _threadManager;
	}

public:
	void InsertFunc(std::function<void(void)> func)
	{
		int funcId = _funcId.fetch_add(1);

		std::lock_guard<std::mutex> lock(_lock);
		_repo_1.emplace(funcId, func);
	}

	void InsertFunc(std::function<void(int)> func, int count)
	{
		int funcId = _funcId.fetch_add(1);
		auto bound_func = [f = std::move(func), count]() { f(count); };

		std::lock_guard<std::mutex> lock(_lock);
		_repo_1.emplace(funcId, bound_func);
	}

	void LaunchAll()
	{
		std::lock_guard<std::mutex> lock(_lock);
		for (const auto& _data : _repo_1)
		{
			int id			= _data.first;
			auto& func		= _data.second;
			
			std::thread* t = new std::thread(func);
			_threads.push_back(t);
		}
	}

	void JoinAll()
	{
		for (std::thread* thread : _threads)
		{
			thread->join();
		}
	}

	void Clear()
	{
		{
			std::lock_guard<std::mutex> lock(_lock);
			_repo_1.clear();
			_threads.clear();
		}
		_funcId.exchange(0);
	}

	void Start()
	{
		LaunchAll();
		JoinAll();
		Clear();
	}

private:
	std::unordered_map<int, std::function<void(void)>>		_repo_1;
	std::vector<std::thread*>							_threads;

	std::atomic<int>									_funcId{};
	std::mutex											_lock;
};

