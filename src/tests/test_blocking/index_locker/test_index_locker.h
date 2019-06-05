#ifndef __TEST_INDEX_LOCKER_H
#define __TEST_INDEX_LOCKER_H

#include<vector>
#include<string>
#include <locks.h>

namespace test
{
    #define NUMBER 20
    extern int m_indexes[NUMBER];
    extern std::string result[NUMBER];
    

    void test_index_locker();
    void split_str(const std::string in, std::vector<std::string> & out);
};

#endif