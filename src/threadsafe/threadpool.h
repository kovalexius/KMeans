#ifndef THREADPOOL
#define THREADPOOL

#include <thread>
#include <list>
#include <condition_variable>
#include "lockfree.h"

using namespace std;

namespace mtp
{

	class CTask
	{
	public:
		virtual void Execute() = 0;
	};


	class CThreadPool
	{

	public:
		void Init(int _numThreads)
		{
			m_numThreads = _numThreads;
			for (int i = 0; i < m_numThreads; i++)
				m_threads.push_back( std::thread(workThread) );
		}

		bool AddTask(CTask* _task)
		{
			// lock
			m_tasks.push_back(_task);
			// unlock

			return true;
		}

	private:
		static void workThread( CThreadPool *ths )
		{
			lockfree::list<CTask*> &task_list = ths->m_tasks;
			while (true)
			{
				while (!task_list.empty())
				{
					
					CTask *tsk;
					if( task_list.pop_front(tsk) )
						// lock 
						tsk->Execute();
						// unlock
				}
				std::condition_variable blk;
				blk.wait;
			}
		}
	private:
		int m_numThreads;
		lockfree::list<CTask*> m_tasks;
		std::list<std::thread&> m_threads;
	};

}

#endif // THREADPOOL

