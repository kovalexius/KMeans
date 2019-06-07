#ifndef __LIST_LOCKFREE_H
#define __LIST_LOCKFREE_H

#include <cstdint>
#include <mutex>

namespace blocking
{
	template<typename T>
    class list
    {
        struct listCell
        {
			listCell(listCell *_next, const T &_val) : m_next(_next), m_val(_val)
			{}
			
            listCell *m_next;
            T m_val;
        };

    public:
        list(): m_front(nullptr), 
				m_back(nullptr),
				m_size(0)
        {
        }

        ~list()
		{
			T tmpVal;
			while(pop_front(tmpVal))
			{}
		}
        
        void push_back(const T& _other)
        {
			std::unique_lock<std::mutex> lock(m_mutex);
			
			m_size++;
			
			auto cell = new listCell(nullptr, _other);
			if(m_front == nullptr)			// Пусто
			{
				m_front = m_back = cell;
			}
			else
			{
				m_back->m_next = cell;
				m_back = cell;				// Передвинуть m_back на последний элемент
			}
        }

        bool pop_front(T &_result)
        {
			std::unique_lock<std::mutex> lock(m_mutex);
			
			if(m_front == nullptr)
				return false;
			
			m_size--;
			_result = m_front->m_val;
			if(m_front == m_back)		// Остался единственный элемент
			{
				delete m_front;   // Это удаление и m_back также, т.к. они указывают на один и тот же элемент
				m_front = m_back = nullptr;
				return true;
			}
			
			auto old_front = m_front;
			m_front = m_front->m_next;		// Передвигаем m_front дальше
			delete old_front;
			
			return true;
        }

        bool empty()
        {
			std::unique_lock<std::mutex> lock(m_mutex);
            return !(m_size > 0);
        }

        uint32_t size()
        {
			std::unique_lock<std::mutex> lock(m_mutex);
            return m_size;
        }

    private:
        listCell *m_front;
        listCell *m_back;
        uint32_t m_size;
		std::mutex m_mutex;
    };
}

#endif
