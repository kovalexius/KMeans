#include <thread>
#include <vector>
#include "lockfree.h"

class test
{
public:
    static lockfree::list<int> m_list;

    static void work_thread_push();

    static void work_thread_pop();

    static void test_lockfree();
};
