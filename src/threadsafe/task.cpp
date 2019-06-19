#include "task.h"

#include <iostream>

namespace mtp
{
	CTask::CTask()
	{
		m_future = m_promise.get_future();
		//m_waited = false;
	}
	
	void CTask::Wait()
	{
		//std::cout << __FUNCTION__ << std::endl;
		m_future.wait();
		
		/*
		std::unique_lock<std::mutex> lk(m_waiting_mutex);
		while (m_waited.load())
		{
			m_condVar.wait(lk);
		}
		*/
	}
	
	void CTask::Notify()
	{
		//std::cout << __FUNCTION__ << std::endl;
		m_promise.set_value();
		
		/*
		m_waited.store(false);
		m_condVar.notify_one();
		*/
	}
}
