#ifndef __TASK__H
#define __TASK__H

#include <future>
#include <mutex>
#include <atomic>

namespace mtp
{
	class CTask
	{
	public:
		friend class CThreadPool;
		
		CTask();
		
		virtual void Execute() = 0;
		
		void Wait();
		void Notify();
		
	private:
		std::promise<void> m_promise;
		std::future<void> m_future;
		
		std::condition_variable m_condVar;
		std::mutex m_waiting_mutex;
		std::atomic<bool> m_waited;
	};
}

#endif
