#include "Dog.h"
#include "Owl.h"
#include "Cat.h"
#include "Duck.h"
#include "Pig.h"
#include "Animal.h"
#include "AnimalManager.h"
#include "User.h"
#include "ThreadLaunchManager.h"
#include <functional>
#include <chrono>

namespace Test
{
	/*---------------------------------------------------------------------
		Test 1. OOP Basic + Thread Basic
	---------------------------------------------------------------------*/
	std::function<void(int)> Insert_N_Users = [](int n)
		{
			int processCount = 0;

			while (processCount < n)
			{
				bool success = AnimalManager::Instance().Insert(std::make_shared<User>());
				if (success) processCount++;
			};
		};

	std::function<void(int)> Delete_N_Users = [](int n)
		{
			int processCount = 0;
			while (processCount < n)
			{
				bool success = AnimalManager::Instance().Delete();
				if (success) processCount++;
			};
		};

	void Test_Insert(int n = 10000)
	{
		ThreadLaunchManager::Instance().InsertFunc(Insert_N_Users, n);
		ThreadLaunchManager::Instance().InsertFunc(Insert_N_Users, n);
		ThreadLaunchManager::Instance().InsertFunc(Insert_N_Users, n);
		ThreadLaunchManager::Instance().LaunchAll();
		ThreadLaunchManager::Instance().JoinAll();
		ThreadLaunchManager::Instance().Clear();
		std::cout << "----------------------------------------------------------------" << std::endl;
		std::cout << "Phase 1 - 현재 User(Count) : " << AnimalManager::Instance().Size() << std::endl;
		std::cout << "Phase 1 - 실제 생성자 호출 : " << User::GetCtorCnt() << std::endl;

		User::ClearStatistics();
	}

	void Test_Delete(int n = 10000)
	{
		ThreadLaunchManager::Instance().InsertFunc(Delete_N_Users, n);
		ThreadLaunchManager::Instance().InsertFunc(Delete_N_Users, n);
		ThreadLaunchManager::Instance().InsertFunc(Delete_N_Users, n);
		ThreadLaunchManager::Instance().LaunchAll();
		ThreadLaunchManager::Instance().JoinAll();
		ThreadLaunchManager::Instance().Clear();
		std::cout << "----------------------------------------------------------------" << std::endl;
		std::cout << "Phase 2 - 현재 User(Count) : " << AnimalManager::Instance().Size() << std::endl;
		std::cout << "Phase 2 - 실제 소멸자 호출 : " << User::GetDtorCnt() << std::endl;
		User::ClearStatistics();
	}

	void Test_InsertDelete(int n = 10000)
	{

		ThreadLaunchManager::Instance().InsertFunc(Insert_N_Users, n);
		ThreadLaunchManager::Instance().InsertFunc(Delete_N_Users, n);
		ThreadLaunchManager::Instance().InsertFunc(Insert_N_Users, n);
		ThreadLaunchManager::Instance().LaunchAll();
		ThreadLaunchManager::Instance().JoinAll();
		ThreadLaunchManager::Instance().Clear();
		std::cout << "----------------------------------------------------------------" << std::endl;
		std::cout << "Phase 3 - 현재 User(Count) : " << AnimalManager::Instance().Size() << std::endl;
		std::cout << "Phase 3 - 실제 생성자 호출 : " << User::GetCtorCnt() << std::endl;
		std::cout << "Phase 3 - 실제 소멸자 호출 : " << User::GetDtorCnt() << std::endl;

		User::ClearStatistics();
	}


	/*---------------------------------------------------------------------
		Test 2. OOP Basic + Thread Load-Balancing Basic
	---------------------------------------------------------------------*/
	//std::function<void(void)> Update_Animals = []()
	//	{
	//		int funcId = AnimalManager::Instance().GetUpdateFuncId();
	//		AnimalManager::Instance().InsertFuncIdToOrder(funcId);

	//		int fpsCnt = 0;
	//		int lastCheckFpsCnt = 0;

	//		auto start = std::chrono::system_clock::now();
	//		while (true)
	//		{
	//			auto end = std::chrono::system_clock::now();
	//			
	//			if (end - start >= std::chrono::seconds(1)) { // 1초 지났는데
	//				start = std::chrono::system_clock::now();
	//				lastCheckFpsCnt = fpsCnt;

	//				auto crt = fpsCnt - lastCheckFpsCnt;
	//				if (crt < 0) // 20번보다 덜 실행된 경우
	//				{
	//					while (crt++)
	//					{
	//						AnimalManager::Instance().SlicedForeachAnimals([](std::shared_ptr<Animal> animal)
	//							{
	//								animal->Update();
	//							},
	//							funcId);
	//						std::cout << "Less Then 20 : 보정" << std::endl;
	//					}
	//				}
	//				else if(0 < crt) // 20번보다 더 실행된 경우
	//				{
	//					while (crt--)
	//					{
	//						std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 20fps
	//						std::cout << "More Than 20 :  보정" << std::endl;
	//					}
	//				}
	//			}


	//			std::cout << ++fpsCnt << std::endl;
	//			AnimalManager::Instance().SlicedForeachAnimals([](std::shared_ptr<Animal> animal)
	//				{
	//					animal->Update();
	//				}, 
	//				funcId);

	//			std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 20fps
	//		}
	//	};

	//void Test_InsertMixed_UpdateAges()
	//{
	//	for (int i = 0; i < 20; ++i) // 동물 100마리
	//	{
	//		AnimalManager::Instance().Insert(std::make_shared<Dog>());
	//		AnimalManager::Instance().Insert(std::make_shared<Cat>());
	//		AnimalManager::Instance().Insert(std::make_shared<Owl>());
	//		AnimalManager::Instance().Insert(std::make_shared<Duck>());
	//		AnimalManager::Instance().Insert(std::make_shared<Pig>()); 

	//		for (int j = 0; j < 5; ++j) // 사람 100명
	//		{
	//			AnimalManager::Instance().Insert(std::make_shared<User>());
	//		}
	//	}
	//	std::cout << "총 몇: " << AnimalManager::Instance().Size() << std::endl;
	//	ThreadLaunchManager::Instance().InsertFunc(Update_Animals);
	//	ThreadLaunchManager::Instance().InsertFunc(Update_Animals);
	//	ThreadLaunchManager::Instance().Start(); // LaunchAll, JoinAll, Clear

	//}

	/*---------------------------------------------------------------------
		Test 3. Thread-LoadBalacing, But Animals Can move any other Thread.
	---------------------------------------------------------------------*/
	void Test_UserMoveThread()
	{
		for (int i = 0; i < 20; ++i) // 동물 100마리
		{
			AnimalManager::Instance().CreateAnimal<Dog>();
			AnimalManager::Instance().CreateAnimal<Cat>();
			AnimalManager::Instance().CreateAnimal<Owl>();
			AnimalManager::Instance().CreateAnimal<Duck>();
			AnimalManager::Instance().CreateAnimal<Pig>();

			for (int j = 0; j < 5; ++j) // 사람 100명
			{
				AnimalManager::Instance().CreateAnimal<User>();
			}
		}
	}


}

int main()
{
	Executor::Instance().initialize(2); // Worker Thread 2
	Executor::Instance().StartAll();
	Test::Test_UserMoveThread();
	
	while (true)
	{

	}
}
