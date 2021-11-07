cmake_minimum_required(VERSION 3.20)
project(PCenter)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-static-libgcc -static-libstdc++ ${CMAKE_CXX_FLAGS}")

#include_directories(/home/edward/C++/Boost/boost_1_77_0/boost)
include_directories(D:/Boost/boost_1_77_0/)
include_directories(class)
include_directories(utils)
include_directories(src)
include_directories(test)

add_executable(PCenter src/main.cpp src/data.cpp src/work.cpp src/work.h test/test.cpp test/test.h utils/Random.cpp utils/Random.h utils/TimeChecker.cpp utils/TimeChecker.h class/Node.cpp class/Node.h class/Solution.cpp class/Solution.h)
