#include "threadpool.h"

namespace mtp
{
	CThreadPool::CThreadPool() : m_isRunning(true)
	{
	}
	
	CThreadPool::~CThreadPool()
	{
		m_isRunning = false;
		WaitAll();
	}
	
	void CThreadPool::Init(int _numThreads)
	{
		m_numThreads = _numThreads;

		for (int i = 0; i < m_numThreads; i++)
		{
			std::thread th(workThread, std::ref(*this));
			m_threads.push_back(std::move(th));
		}
	}
	
	bool CThreadPool::AddTask(CTask* _task)
	{
		m_tasks.push_back(_task);
		
		m_waiting.notify_one();

		return true;
	}
	
	void CThreadPool::SetOnWaiting(std::function<void()> _onWaiting)
	{
		m_onWaiting = _onWaiting;
	}
	
	void CThreadPool::WaitAll()
	{
		m_waiting.notify_all();
		for(auto &item : m_threads)
		{
			item.join();
		}
	}
	
	void CThreadPool::workThread(CThreadPool &ths)
	{
		auto &task_list = ths.m_tasks;
		while (ths.m_isRunning)
		{
			
			while (!task_list.empty())
			{
				CTask *tsk;
				if(task_list.pop_front(tsk))
				{
					tsk->Execute();
					tsk->m_promise.set_value();
				}
			}
				
			// Ожидание новых тасков
			std::unique_lock<std::mutex> lk(ths.m_waiting_mutex);
			ths.m_waiting.wait(lk);
		}
	}
}
