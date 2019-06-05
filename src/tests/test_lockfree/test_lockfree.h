#include <thread>
#include <vector>
#include "lockfree.h"

namespace test
{
    extern lockfree::list<int> m_list;

    void work_thread_push();

    void work_thread_pop();

    void test_lockfree();
};
