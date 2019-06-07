cmake_minimum_required(VERSION 2.8)

project(test_lockfree)

add_executable(test_lockfree
                main.cpp
                test_lockfree.cpp
                test_lockfree.h
              )
              
if(${UNIX})
	target_link_libraries(test_lockfree "-pthread")
endif(${UNIX})
