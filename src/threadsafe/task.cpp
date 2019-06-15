#include "task.h"

namespace mtp
{
	CTask::CTask()
	{
		m_future = m_promise.get_future();
	}
	
	void CTask::Wait()
	{
		m_future.wait();
	}
}
