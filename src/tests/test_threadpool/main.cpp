#include "../../threadsafe/threadpool.h"

#include <iostream>
#include <thread>

class MyTask : public mtp::CTask
{
public:
	MyTask(const double _sum): m_sum(_sum)
	{}
	
	void Execute() override
	{
		for(int i = 0; i<100000; i++)
		{
			m_sum += 0.1;
		}
		std::cout << "tid: " << std::this_thread::get_id() << " sum: " << m_sum << std::endl;
	}
	
	double getSum()
	{
		return m_sum;
	}
	
private:
	double m_sum;
};

int main()
{
	mtp::CThreadPool pool;
	pool.Init(4);
	
	MyTask task1(0.0);
	MyTask task2(0.0);
	MyTask task3(0.0);
	MyTask task4(0.0);
	
	for(int i = 0; i < 5; i++)
	{
		pool.AddTask(&task1);
		pool.AddTask(&task2);
		pool.AddTask(&task3);
		pool.AddTask(&task4);
	}
}
