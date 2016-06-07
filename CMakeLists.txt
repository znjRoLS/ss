cmake_minimum_required(VERSION 3.5)
project(ss)

file(GLOB SOURCE_FILES_COMPILER
        compiler/*.h
        compiler/*.cpp
        )

file(GLOB SOURCE_FILES_LINKER
        linker/*.h
        linker/*.cpp
        )

file(GLOB SOURCE_FILES_EMULATOR
        emulator/*.h
        emulator/*.cpp
        )

#add_executable(yourProj ${SOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#set(SOURCE_FILES main.cpp)
add_executable(compiler ${SOURCE_FILES_COMPILER})
add_executable(linker ${SOURCE_FILES_LINKER})
add_executable(emulator ${SOURCE_FILES_EMULATOR})