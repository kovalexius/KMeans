#include <iostream>
#include <list>
#include "test_lockfree.h"


lockfree::list<int> test::m_list;

void test::work_thread_push()
{
    for( int i = 0; i < 10000; i++ )
        m_list.push_back( i );
}

void test::work_thread_pop()
{
    int res;
    for( int i = 0; i < 1000;)
        if( m_list.pop_front( res ) )
            i++;
}

void test::test_lockfree()
{
    time_t t0 = time(0);
    std::vector<std::thread> m_threads;
    for( int i = 0; i < 100; i++ )
    {
        m_threads.push_back( std::thread( work_thread_push ) );
        m_threads.push_back( std::thread( work_thread_pop ) );
    }

    for( unsigned int i = 0; i < m_threads.size(); i++ )
        m_threads[i].join();
    time_t t1 = time(0);

    std::cout << "size = " << m_list.size() << " period = " << t1 - t0 << std::endl;
    std::cin.get();

    std::list<int> list1;
    t0 = time(0);
    for( int j = 0; j < 100; j++ )
    {
        for( int i = 0; i < 10000; i++ )
            list1.push_back( i );
        for( int i = 0; i < 1000; i++ )
            list1.pop_front();
    }
    t1 = time(0);
    std::cout << "size = " << list1.size() << " period = " << t1 - t0 << std::endl;
    std::cin.get();

    int chislo = 0;
    int amount = 0;
    while( m_list.pop_front( chislo ) )
    {
        amount ++;
        std::cout << " amount:" << amount <<" chislo = " << chislo << std::endl;
    }
    
    std::cout << "ending" << std::endl;
    std::cin.get();
}
