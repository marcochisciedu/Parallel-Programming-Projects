cmake_minimum_required(VERSION 3.22)
project(Parallel-Programming-Projects)

set(CMAKE_CXX_STANDARD 17)

# ON to activate the thread sanitizer
#[[OPTION(CB_THREADSANITIZER "Enable ThreadSanitizer data race detector."
    ON)
INCLUDE(CheckCCompilerFlag)
INCLUDE(CheckCXXCompilerFlag)
INCLUDE(CMakePushCheckState)
set(CMAKE_CXX_STANDARD 11)

IF (CB_THREADSANITIZER)
    CMAKE_PUSH_CHECK_STATE(RESET)
    SET(CMAKE_REQUIRED_FLAGS "-fsanitize=thread") # Also needs to be a link flag for test to pass
    CHECK_C_COMPILER_FLAG("-fsanitize=thread" HAVE_FLAG_SANITIZE_THREAD_C)
    CHECK_CXX_COMPILER_FLAG("-fsanitize=thread" HAVE_FLAG_SANITIZE_THREAD_CXX)
    CMAKE_POP_CHECK_STATE()

    IF(HAVE_FLAG_SANITIZE_THREAD_C AND HAVE_FLAG_SANITIZE_THREAD_CXX)
        SET(THREAD_SANITIZER_FLAG "-fsanitize=thread")

        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${THREAD_SANITIZER_FLAG}")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${THREAD_SANITIZER_FLAG}")
        SET(CMAKE_CGO_LDFLAGS "${CMAKE_CGO_LDFLAGS} ${THREAD_SANITIZER_FLAG} -pthread ")
        MESSAGE(STATUS "ThreadSanitizer enabled.")
    ELSE()
        MESSAGE(FATAL_ERROR "CB_THREADSANITIZER enabled but compiler doesn't support ThreadSanitizer - cannot continue.")
    ENDIF()
ENDIF()]]

# in Linux just use -fopenmp, without -lomp and -Xpreprocessor
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp ")

set(SOURCE_FILES KMeansSoA.cpp
        KMeansSoA.h
        PointsSoA.cpp
        PointsSoA.h
        testKMeansSoA.cpp
        KMeans.cpp
        KMeans.h
        Cluster.cpp
        Cluster.h
        testKMeans.cpp
        LevenshteinDistance.h
        LevenshteinDistance.cpp
        checkVocabularySeq.cpp
        checkVocabularySeq.h
        checkVocabularyPar.cpp
        checkVocabularyPar.h
        testLevenshteinDistance.cpp
        generateData.cpp
        generateData.h)

add_executable(Kmeans_clustering
        testKMeans.cpp
        KMeans.cpp
        KMeans.h
        Cluster.cpp
        Cluster.h
        generateData.cpp
        generateData.h)

add_executable(Kmeans_clustering_SoA
        testKMeansSoA.cpp
        KMeansSoA.cpp
        KMeansSoA.h
        PointsSoA.cpp
        PointsSoA.h
        generateData.cpp
        generateData.h)

add_executable(Levenshtein_distance
        LevenshteinDistance.h
        LevenshteinDistance.cpp
        checkVocabularySeq.cpp
        checkVocabularySeq.h
        checkVocabularyPar.cpp
        checkVocabularyPar.h
        testLevenshteinDistance.cpp
        generateData.cpp
        generateData.h)
