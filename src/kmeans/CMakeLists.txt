cmake_minimum_required(VERSION 2.8)

project(KMeans)

add_executable(kmeans_singleThread
                ./single.cpp
                ./singleThread/kmeansSingleThread.h
                ./generator/generate.h
                ./userData/point.h
               )

add_executable(kmeans_multithread
                ./multi.cpp 
                ../threadsafe/threadpool.cpp
                ../threadsafe/threadpool.h
                ../threadsafe/task.cpp
                ../threadsafe/task.h
                ./multiThread/kmeansMultiThread.h
                ./generator/generate.h
                ./userData/point.h
			   )
			   
if(${UNIX})
	target_link_libraries(kmeans_multithread "-pthread")
endif(${UNIX})
