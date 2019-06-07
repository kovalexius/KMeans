#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <thread>

#include "../../threadsafe/blocking/list.h"

void PushBackOnly(blocking::list<int> &_list)
{
	for(int i=0; i<50000; i++)
	{
		_list.push_back(5);
	}
	
	for(int i=0; i<50000; i++)
	{
		_list.push_back(7);
	}
}

void PopFrontOnly(blocking::list<int> &_list)
{
	for(int i = 0; i < 99990; i++)
	{
		int tmp;
		while(!_list.pop_front(tmp))
		{
			std::cout << "elements ran out" << std::endl;
		}
	}
}

SCENARIO("Test blocking list", "[Basic]")
{
	GIVEN("blocking::list")
	{
		blocking::list<int> list;
		WHEN("push and pop in many threads")
		{
			std::thread th0(PushBackOnly, std::ref(list));
			std::thread th1(PushBackOnly, std::ref(list));
			std::thread th2(PopFrontOnly, std::ref(list));
			std::thread th3(PopFrontOnly, std::ref(list));
			
			th0.join();
			th1.join();
			th2.join();
			th3.join();
			
			THEN("checking")
			{
				REQUIRE(list.size()==20);
				
				int tmp;
				while(list.pop_front(tmp))
				{
					REQUIRE(tmp==7);
				}
				std::cout << std::endl;
				
				REQUIRE(list.empty());
			}
		}
	}
}
