#include <string>
#include <vector>
#include <map>
#include <thread>
#include <iostream>

#include <test_index_locker.h>

namespace test
{
//#define INDEX_LOCKER

    int indexes[NUMBER] = { 1, 5, 2, 9, 3, 8, 4, 7, 6, 0, 1, 5, 2, 9, 3, 8, 4, 7, 6, 0 };
    int n_rep = 100000;
    int thr_num = 3;
    std::string result[NUMBER];

#ifdef INDEX_LOCKER
    locks::index_locker<int> m_lock;
#else
    std::mutex m_lock;
#endif
    std::string phrase1("Hello, How are you?");
    std::string phrase2("Good bye, nice to meet me!");
    std::string phrase3("Fuck you, Spilberg!");
    std::string phrase4("I'm fine, thank you.");

    void print_hello( const int index )
    {
        size_t n = phrase1.length();

    #ifdef INDEX_LOCKER
        m_lock.lock(index);
    #else
        m_lock.lock();
    #endif
        result[index] += ";";
        for (size_t i = 0; i < n; i++)
            result[index] += phrase1[i];
    #ifdef INDEX_LOCKER
        m_lock.unlock(index);
    #else
        m_lock.unlock();
    #endif
    }

    void print_goodbye(const int index)
    {
        size_t n = phrase2.length();

    #ifdef INDEX_LOCKER
        m_lock.lock(index);
    #else
        m_lock.lock();
    #endif
        result[index] += ";";
        for (size_t i = 0; i < n; i++)
            result[index] += phrase2[i];
    #ifdef INDEX_LOCKER
        m_lock.unlock(index);
    #else
        m_lock.unlock();
    #endif
    }

    void print_fuck(const int index)
    {
        size_t n = phrase3.length();

    #ifdef INDEX_LOCKER
        m_lock.lock(index);
    #else
        m_lock.lock();
    #endif
        result[index] += ";";
        for (size_t i = 0; i < n; i++)
            result[index] += phrase3[i];
    #ifdef INDEX_LOCKER
        m_lock.unlock(index);
    #else
        m_lock.unlock();
    #endif
    }
    
    void print_fine(const int index)
    {
        size_t n = phrase4.length();

    #ifdef INDEX_LOCKER
        m_lock.lock(index);
    #else
        m_lock.lock();
    #endif
        result[index] += ";";
        for (size_t i = 0; i < n; i++)
            result[index] += phrase4[i];
    #ifdef INDEX_LOCKER
        m_lock.unlock(index);
    #else
        m_lock.unlock();
    #endif
    }

    void hello_thread(int n)
    {
        for (int i = 0; i < n; i++)
            print_hello(indexes[i % NUMBER]);
    }

    void goodbye_thread(int n)
    {
        for (int i = n - 1; i >= 0; i--)
            print_goodbye(indexes[i % NUMBER]);
    }

    void fuck_thread(int n)
    {
        for (int i = 0; i < n; i++)
            print_fuck(indexes[i % NUMBER]);
    }

    void fine_thread(int n)
    {
        for (int i = n-1; i >= 0; i--)
            print_fine(indexes[i % NUMBER]);
    }

    void test_index_locker()
    {
        time_t t0 = time(0);
        std::vector<std::thread> m_threads;
        for (int i = 0; i < thr_num; i++)
        {
            m_threads.push_back(std::thread(hello_thread, n_rep));
            m_threads.push_back(std::thread(goodbye_thread, n_rep));
            m_threads.push_back(std::thread(fuck_thread, n_rep));
            m_threads.push_back(std::thread(fine_thread, n_rep));
        }

        for (unsigned int i = 0; i < m_threads.size(); i++)
            m_threads[i].join();
        time_t t1 = time(0);

        //std::cout << "Number of locked indexes = " << m_lock.size() << " period = " << t1 - t0 << std::endl;
        std::cout << "Period = " << t1 - t0 << std::endl;
        std::cin.get();

        std::map<std::string, int> res_strings;
        for (int i = 0; i < NUMBER; i++)
        {
            std::vector<std::string> split_strings;
            split_str(result[i], split_strings);
            for (size_t j = 0; j < split_strings.size(); j++)
            {
                auto it = res_strings.find(split_strings[j]);
                if (it != res_strings.end())
                    it->second++;
                else
                    res_strings.insert( std::make_pair(split_strings[j], 1) );
            }
        }

        for (auto it = res_strings.begin(); it != res_strings.end(); it++)
            std::cout << "String = '" << it->first << "' Number = '" << it->second << "'" << std::endl;

        std::cin.get();
    }

    void split_str(const std::string in, std::vector<std::string> & out)
    {
        std::string delim = ";";
        size_t begin = 0;
        size_t end = in.find(delim, 0);
        while (end != std::string::npos)
        {
            out.push_back(in.substr(begin, end - begin));
            begin = end + delim.length();
            end = in.find(";", begin);
        }
        out.push_back(in.substr(begin));
    }
}