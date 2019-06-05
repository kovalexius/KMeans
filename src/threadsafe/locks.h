#ifndef __LOCKS_H
#define __LOCKS_H

#include <unordered_map>
#include <atomic>
#include <mutex>

namespace locks
{
    template<typename T>
    class index_locker
    {
        struct sync_object
        {
            sync_object() : catch_cnt(0)
            {}
            std::mutex mtx;
            int catch_cnt;
        };
    public:
        ~index_locker()
        {}

        size_t size()
        {
            std::lock_guard<std::mutex> lck(m_global);
            return m_lockers.size();
        }

        void lock(const T & index)
        {
            m_global.lock();
            sync_object &cur_object = m_lockers[index];
            cur_object.catch_cnt++;
            if (!cur_object.mtx.try_lock())
            {
                m_global.unlock();
                cur_object.mtx.lock();
            }
            else
                m_global.unlock();
        }

        void unlock(const T & index)
        {
            auto cur_lock = m_lockers.find(index);
            if (cur_lock != m_lockers.end())
            {
                
                cur_lock->second.mtx.unlock();
                m_global.lock();
                cur_lock->second.catch_cnt--;
                if (cur_lock->second.catch_cnt == 0)
                    m_lockers.erase(cur_lock);
                m_global.unlock();
            }
        }

    private:
        std::mutex m_global;
        std::unordered_map<T, sync_object > m_lockers;
    };
}
#endif