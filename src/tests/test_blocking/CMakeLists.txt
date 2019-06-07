cmake_minimum_required(VERSION 2.8)

project(test_blocking)

include_directories(
		../../catchTest/include/
)

add_executable(test_list
				list/main.cpp
				)

add_executable(index_locker
                ./index_locker/main.cpp
                ./index_locker/test_index_locker.h
                ./index_locker/test_index_locker.cpp
              )
              
if(${UNIX})
	target_link_libraries(test_list "-pthread")
	target_link_libraries(index_locker "-pthread")
endif(${UNIX})
