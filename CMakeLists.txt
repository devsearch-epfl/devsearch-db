cmake_minimum_required(VERSION 3.5)
project(devsearch_db)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp StringDict.cpp StringDict.h JsonImporter.cpp JsonImporter.h FeatureStore.cpp FeatureStore.h)
add_executable(devsearch_db ${SOURCE_FILES})