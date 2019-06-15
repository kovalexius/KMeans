cmake_minimum_required(VERSION 2.8)

project(test_threadpool)

add_executable(test_threadpool
				main.cpp
				../../threadsafe/threadpool.cpp
				../../threadsafe/threadpool.h
				../../threadsafe/task.cpp
				../../threadsafe/task.h
			)
			
if(${UNIX})
	target_link_libraries(test_threadpool "-pthread")
endif(${UNIX})
