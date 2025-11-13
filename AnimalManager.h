#pragma once
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <functional>
#include "Animal.h"
#include <map>
#include <mutex>
#include <set>
#include "Singleton.h"
#include <concepts>

class AnimalManager : public Singleton<AnimalManager>
{

public:

	const size_t Size() const noexcept
	{
		return _repo_1.size();
	}

	template<typename T> requires std::derived_from<T, Animal>
	void CreateAnimal()
	{
		auto animal = std::make_shared<T>();
		animal->Init();
		Insert(animal);
	}

	bool Insert(std::shared_ptr<Animal> animal)
	{
		std::lock_guard<std::recursive_mutex> lock(_lock);

		_repo_1.insert({ animal->GetAnimalID(), animal });
		return true;
	}

	bool Delete(int id)
	{
		std::lock_guard<std::recursive_mutex> lock(_lock);
		auto it = _repo_1.find(id);
		if (it == _repo_1.end()) 
			return false;


		_repo_1.erase(it);
		return true;
		
	}

	bool Delete()
	{
		std::lock_guard<std::recursive_mutex> lock(_lock);
		auto it = _repo_1.begin();
		if (it == _repo_1.end()) 
			return false;

		_repo_1.erase(it);
		return true;

	}


	void Print()
	{
		std::lock_guard<std::recursive_mutex> lock(_lock);

		for (auto& [id, animal] : _repo_1)
		{
			animal->print_sound();
		}
	}

	void Clear()
	{
		std::lock_guard<std::recursive_mutex> lock(_lock);

		_repo_1.clear(); // 이걸 위해서 SharedPtr로 변경
	}

	std::shared_ptr<Animal> Find(int id)
	{
		std::lock_guard<std::recursive_mutex> lock(_lock);

		auto it = _repo_1.find(id);

		if (it == _repo_1.end())
		{
			return nullptr;
		}
			
		return it->second;
	}

	void ForeachAnimals(std::function<void(std::shared_ptr<Animal>)> f)
	{
		std::lock_guard<std::recursive_mutex> lock(_lock);
		for (auto& _data : _repo_1)
		{
			int id							= _data.first;
			std::shared_ptr<Animal> animal	= _data.second;

			f(animal);
		}
	}

private:
	std::unordered_map<int, std::shared_ptr<Animal>>	_repo_1{};
	std::recursive_mutex _lock;
	
	int _lastInputId{ 0 };
	// 1, 2, 3 

	// 1 -> 1씩 증가
	// 2 -> 1,2각각 2씩 증가
	// 3 -> 1,2,3 각각 3씩 증가

};