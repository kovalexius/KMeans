#ifndef LOCKFREE_H
#define LOCKFREE_H

#include <stdint.h>
//#include <mutex>
#include <atomic>

namespace lockfree
{
    template<typename T>
    class list
    {
        struct listCell
        {
            listCell *m_next;
            T m_val;
        };

    public:
        list()
        {
            // dummy object
            listCell *cell = new listCell;
            cell->m_next = nullptr;

            m_front = cell;
            m_back = cell;
            m_size = 0;
        }

        void push_back( const T& other )
        {
            listCell *cell = new listCell;
            cell->m_val = other;
            cell->m_next = nullptr;

           //m_change_size_mutex.lock();

            m_change_size_mutex.lock();
            // Если размер == 0 то изменение m_front
            if( m_size == 0 )
            {
                m_front_mutex.lock();
                m_front = cell;
                m_front_mutex.unlock();
            }
            // Инкрементация размера
            m_size++;
            m_change_size_mutex.unlock();

            // Изменение m_back
            m_back_mutex.lock();
            if(m_back)
                m_back->m_next = cell;
            m_back = cell;
            m_back_mutex.unlock();

            //m_change_size_mutex.unlock();
        }

        bool pop_front( T &result )
        {
            if( m_size == 0 )
                return false;

            listCell *old_front = nullptr;

           //m_change_size_mutex.lock();

            // Изменение m_front
            m_front_mutex.lock();
            if( m_front )
            {
                old_front = m_front;
                m_front = m_front->m_next;
            }
            m_front_mutex.unlock();

            // Декрементация размера
            m_change_size_mutex.lock();
            if( m_size > 0 )
                m_size--;
            m_change_size_mutex.unlock();

           //m_change_size_mutex.unlock();

            if( old_front )
            {
                // Присвоение result'у
                result = old_front->m_val;

                // Удаление элемента
                delete old_front;

                return true;
            }

            return false;
        }

        bool empty()
        {
            if( m_size > 0 )
                    return false;
            return true;
        }

        uint32_t size()
        {
            return m_size;
        }

    private:
        listCell *m_front;
        listCell *m_back;
        //std::atomic<uint32_t> m_size;
        uint32_t m_size;
    };
}

#endif
