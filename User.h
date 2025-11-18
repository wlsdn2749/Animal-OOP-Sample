#pragma once
#include "Animal.h"
#include <string>

class User : public Animal
{
public:
	inline static std::atomic<int> _ctorCnt{ 0 };
	inline static std::atomic<int> _dtorCnt{ 0 };

	static int GetCtorCnt()
	{
		return _ctorCnt.load();
	}

	static int GetDtorCnt()
	{
		return _dtorCnt.load();
	}
	
	static void ClearStatistics()
	{
		_ctorCnt.exchange(0);
		_dtorCnt.exchange(0);
	}
public:
	User()
	{
		_ctorCnt++;
		//std::cout << "생성" << GetAnimalID() << std::endl;
	}
	User(const std::string& name)
		: _name(name)
	{
		
	}

	virtual ~User()
	{
		_dtorCnt++;
		//std::cout << "소멸" << GetAnimalID() << std::endl;
	}


public:
	virtual void print_sound() override { std::cout << "Hi! \n"; } 
	virtual void print_name() override { std::cout << "My name is " << _name << std::endl; }
	virtual void print_age() override
	{
		std::cout << "CurrentTime: " << TimeUtil::GetCurrentTime() % 1000000 << " User" << " " << "Thread ID " << std::this_thread::get_id() << " ThreadKey " << _threadKey << " ID: " << GetAnimalID() << " Age: " << GetAge() << std::endl;
	}
	
	virtual void onUpdate() override
	{
		_age += 2;
		print_age();
	}
	const std::string& GetName()
	{
		return _name;
	}

public:
	bool LevelUp()
	{
		int pLevel = _level;
		++_level;
		std::cout << "레벨 증가: " << pLevel << " --> " << _level << "\n";
		return true;
	}

	bool AddExp(int32_t amount)
	{
		int pExp = _exp;
		_exp += amount;
		std::cout << "경험치 증가: " << pExp << " --> " << _exp << "\n";
		return true;
	}

	bool AddMoney(int32_t amount)
	{
		int pMoney = _money;
		_money += amount;
		std::cout << "돈 증가: " << pMoney << " --> " << _money << "\n";
		return true;
	}
private:
	std::string _name{};
	int32_t		_level{};
	int32_t		_exp{};
	int32_t		_money{};
	 
};