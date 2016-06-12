cmake_minimum_required(VERSION 2.8)
project(ss)

include_directories(src/common/h)
include_directories(src/assembler/h)
include_directories(src/emulator/h)

file(GLOB SOURCE_FILES_COMMON
        src/common/h/*.h
        src/common/src/*.cpp
        )

file(GLOB SOURCE_FILES_COMPILER
        src/assembler/h/*.h
        src/assembler/src/*.cpp
        )

file(GLOB SOURCE_FILES_EMULATOR
        src/emulator/h/*.h
        src/emulator/src/*.cpp
        )

#add_executable(yourProj ${SOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

#target_link_libraries(ss pthread)

#set(SOURCE_FILES main.cpp)
add_executable(bin/compiler ${SOURCE_FILES_COMPILER} ${SOURCE_FILES_COMMON})
add_executable(bin/emulator ${SOURCE_FILES_EMULATOR} ${SOURCE_FILES_COMMON})
