cmake_minimum_required(VERSION 3.10)

# set the project name
project(pthreads)

set(CMAKE_BUILD_TYPE Debug)

find_package(PkgConfig REQUIRED)
find_package(Threads)

# add the executable
add_executable(pthread_ex1 pthread_ex1.c)
target_link_libraries (pthread_ex1 ${CMAKE_THREAD_LIBS_INIT})
