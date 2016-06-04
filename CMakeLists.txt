cmake_minimum_required(VERSION 3.5)
project(ss)

file(GLOB SOURCE_FILES
        ./*.h
        ./*.cpp
        )

#add_executable(yourProj ${SOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#set(SOURCE_FILES main.cpp)
add_executable(ss ${SOURCE_FILES})