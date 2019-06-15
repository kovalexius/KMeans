#ifndef __TASK__H
#define __TASK__H

#include <future>

namespace mtp
{
	class CTask
	{
	public:
		friend class CThreadPool;
		
		CTask();
		
		virtual void Execute() = 0;
		
		void Wait();
		
	private:
		std::promise<void> m_promise;
		std::future<void> m_future;
	};
}

#endif
