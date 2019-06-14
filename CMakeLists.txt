cmake_minimum_required(VERSION 2.8)

#project(KMeans)

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
                    src/tests/
                    src/tests/test_lockfree/
                    src/tests/test_blocking/
                    src/tests/test_blocking/index_locker/
                   )


add_subdirectory(src/kmeans ${PROJECT_BINARY_DIR}/kmeans)
add_subdirectory(src/tests/test_blocking/ ${PROJECT_BINARY_DIR}/tests/test_blocking)
add_subdirectory(src/tests/test_lockfree/ ${PROJECT_BINARY_DIR}/tests/test_lockfree)
add_subdirectory(src/tests/test_threadpool/ ${PROJECT_BINARY_DIR}/tests/test_threadpool)

              

