#ifndef THREADPOOL
#define THREADPOOL

#include <thread>
#include <list>
#include <vector>
#include <condition_variable>
#include <atomic>
#include <functional>

#include "blocking/list.h"

using namespace std;

namespace mtp
{

	class CTask
	{
	public:
		friend class CThreadPool;
		
		virtual void Execute() = 0;
	};


	class CThreadPool
	{
	public:
		CThreadPool() : m_isRunning(true)
		{
		}
		
		~CThreadPool()
		{
			m_isRunning = false;
			WaitAll();
		}
		
		void Init(int _numThreads)
		{
			m_numThreads = _numThreads;
			for (int i = 0; i < m_numThreads; i++)
			{
				std::thread th(workThread, std::ref(*this));
				m_threads.push_back(std::move(th));
			}
		}

		bool AddTask(CTask* _task)
		{
			m_tasks.push_back(_task);
			
			m_waiting.notify_one();

			return true;
		}
		
		void SetOnWaiting(std::function<void()> _onWaiting)
		{
			m_onWaiting = _onWaiting;
		}
		
	private:
		
		void WaitAll()
		{
			m_waiting.notify_all();
			for(auto &item : m_threads)
			{
				item.join();
			}
		}

	private:
		static void workThread(CThreadPool &ths)
		{
			auto &task_list = ths.m_tasks;
			while (ths.m_isRunning)
			{
				ths.m_waited.store(true);
				
				while (!task_list.empty())
				{
					CTask *tsk;
					if(task_list.pop_front(tsk))
					{
						tsk->Execute();
					}
				}
				
				
				std::unique_lock<std::mutex> lk(ths.m_waiting_mutex);
				if(ths.m_onWaiting && ths.m_waited.exchange(false))
					ths.m_onWaiting();
				ths.m_waiting.wait(lk);
			}
		}
	private:
		int m_numThreads;
		blocking::list<CTask*> m_tasks;
		std::vector<std::thread> m_threads;
		
		std::condition_variable m_waiting;
		std::mutex m_waiting_mutex;
		std::atomic<bool> m_isRunning;
		
		std::atomic<bool> m_waited;
		std::function<void()> m_onWaiting;
	};

}

#endif // THREADPOOL

