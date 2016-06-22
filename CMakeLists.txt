cmake_minimum_required(VERSION 2.8)

project(KMeans)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions( -Wall -Wl,--no-as-needed -pthread -lpthread -std=gnu++11 )
endif()

source_group(src)
add_executable(kmeans
		src/main.cpp
		src/kmeans/singleThread/kmeansSingleThread.h
		src/generator/generate.h
		src/userData/point.h
		#src/kmeans/multiThread/test_lockfree.cpp
	      )

include_directories(
			./ 
			./src/generator/
		   )

target_link_libraries( kmeans "-pthread")
