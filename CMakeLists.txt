cmake_minimum_required(VERSION 3.22)
project(Parallel-Programming-Projects)

set(CMAKE_CXX_STANDARD 17)

# in Linux just use -fopenmp, without -lomp and -Xpreprocessor
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp ")

set(SOURCE_FILES KMeansSeq.cpp
        KMeansSeq.h
        Cluster.cpp
        Cluster.h
        testKMeans.cpp)
add_executable(Kmeans_clustering
        testKMeans.cpp
        KMeansSeq.cpp
        KMeansSeq.h
        Cluster.cpp
        Cluster.h)
