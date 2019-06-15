#ifndef __THREADPOOL__H
#define __THREADPOOL__H

#include <thread>
#include <list>
#include <vector>
#include <condition_variable>
#include <atomic>
#include <functional>

#include <future>

#include <iostream>

#include "blocking/list.h"
#include "task.h"


namespace mtp
{
	class CThreadPool
	{
	public:
		CThreadPool();
		
		~CThreadPool();
		
		void Init(int _numThreads);

		bool AddTask(CTask* _task);
		
		void SetOnWaiting(std::function<void()> _onWaiting);
		
	private:
		
		void WaitAll();

	private:
		static void workThread(CThreadPool &ths);
		
	private:
		int m_numThreads;
		blocking::list<CTask*> m_tasks;
		std::vector<std::thread> m_threads;
		
		std::condition_variable m_waiting;
		std::mutex m_waiting_mutex;
		std::atomic<bool> m_isRunning;
		
		std::function<void()> m_onWaiting;	// Функтор должен вызываться когда все задачи выполнены и пул ожидает новых задач
	};

}

#endif // THREADPOOL

