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
		std::cout << "User Type " << " ID: " << GetAnimalID() << " Age: " << GetAge() << std::endl;
	}
	
	virtual void onUpdate() override
	{
		++_updateCnt;
		if (_updateCnt % 20 == 0) // 20fps 업데이트, 1초당 1회 동작, 1살 증가
		{
			_age += 1;
			print_age();
		}
	}
	const std::string& GetName()
	{
		return _name;
	}

private:
	std::string _name{};
};