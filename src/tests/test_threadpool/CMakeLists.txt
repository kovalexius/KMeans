cmake_minimum_required(VERSION 2.8)

project(test_threadpool)

add_executable(test_threadpool
				main.cpp
			)
			
if(${UNIX})
	target_link_libraries(test_threadpool "-pthread")
endif(${UNIX})
