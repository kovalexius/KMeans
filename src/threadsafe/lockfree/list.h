#ifndef __LIST_LOCKFREE_H
#define __LIST_LOCKFREE_H

#include <stdint.h>
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
           
        }

        bool pop_front( T &result )
        {
            return false;
        }

        bool empty()
        {
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
