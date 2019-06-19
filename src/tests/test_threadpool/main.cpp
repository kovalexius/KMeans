#include "../../threadsafe/threadpool.h"

#include <iostream>
#include <sys/types.h>

#ifdef __GNUC__
#include <unistd.h>
#else
#include <stdio.h>
#include "stdlib.h"
#endif

#include <thread>
#include <mutex>




class MyTask : public mtp::CTask
{
public:
	MyTask(const int _sum): m_sum(_sum)
	{}
	
	void Execute() override
	{
		for(int i = 0; i<100000; i++)
		{
			inc();
		}
		
		#ifdef __GNUC__
		pthread_t threadB = pthread_self();
		#endif
		
		std::cout << "tid: " << std::this_thread::get_id() << 
		#ifdef __GNUC__
			" posix tid: " << threadB <<
		#endif
			" sum: " << m_sum << std::endl;
	}
	
	void inc()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_sum += 1;
	}
	
	int getSum()
	{
		return m_sum;
	}
	
private:
	int m_sum;
	std::mutex m_mutex;
};


struct OnWaiting
{
	void operator()()
	{
		std::cout << "On waiting" << std::endl;
	}
};


int main()
{
	
	mtp::CThreadPool pool;
	pool.Init(4);
	pool.SetOnWaiting(OnWaiting());
	
	MyTask task1(0.0);
	MyTask task2(0.0);
	MyTask task3(0.0);
	MyTask task4(0.0);
	MyTask task5(0.0);
	MyTask task6(0.0);
	MyTask task7(0.0);
	MyTask task8(0.0);
	
	
	for(int i = 0; i < 2; i++)
	{
		pool.AddTask(&task1);
		pool.AddTask(&task2);
		pool.AddTask(&task3);
		pool.AddTask(&task4);
		pool.AddTask(&task5);
		pool.AddTask(&task6);
		pool.AddTask(&task7);
		pool.AddTask(&task8);
	}
	
#ifdef __GNUC__
	sleep(3);
#else
	_sleep(3000);
#endif

	for(int i = 0; i < 2; i++)
	{
		pool.AddTask(&task1);
		pool.AddTask(&task2);
		pool.AddTask(&task3);
		pool.AddTask(&task4);
		pool.AddTask(&task5);
		pool.AddTask(&task6);
		pool.AddTask(&task7);
		pool.AddTask(&task8);
	}

#ifdef __GNUC__
	sleep(3);
#else
	_sleep(3000);
#endif
}
