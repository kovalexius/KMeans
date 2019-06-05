cmake_minimum_required(VERSION 2.8)

project(KMeans)

if(${UNIX})
    add_definitions( -Wall -Wl,--no-as-needed -pthread -lpthread -std=c++11 )
endif()

source_group(src)
include_directories(
                    ./
                    src/
                    src/kmeans/singleThread/
                    src/kmeans/multiThread/
                    src/kmeans/userData/
                    src/kmeans/generator/
                    src/threadsafe/
                    src/threadsafe/lockfree/
                    src/tests/
                    src/tests/test_lockfree/
                    src/tests/test_blocking/
                    src/tests/test_blocking/index_locker/
                   )

add_executable(kmeans_singleThread
                src/single.cpp
                src/kmeans/singleThread/kmeansSingleThread.h
                src/kmeans/generator/generate.h
                src/kmeans/userData/point.h
               )

#add_executable(kmeans_multithread
#                src/multi.cpp 
#                src/kmeans/multiThread/kmeansMultiThread.h
#                src/kmeans/generator/generate.h
#                src/kmeans/userData/point.h
#			   )

add_executable(test_lockfree
                src/tests/test_lockfree/main.cpp
                src/tests/test_lockfree/test_lockfree.cpp
                src/tests/test_lockfree/test_lockfree.h
                src/threadsafe/lockfree/lockfree.h
              )

add_executable(test_blocking
                src/threadsafe/locks.h
                src/tests/test_blocking/index_locker/main.cpp
                src/tests/test_blocking/index_locker/test_index_locker.h
                src/tests/test_blocking/index_locker/test_index_locker.cpp
              )
              
if(${UNIX})
	target_link_libraries( kmeans_multithread "-pthread")
endif(${UNIX})