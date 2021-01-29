cmake_minimum_required(VERSION 3.10)

# set the project name
project(glibx)

set(CMAKE_BUILD_TYPE Debug)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)

add_library(libglibx g_path.c)

target_include_directories(libglibx PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(libglibx ${GLIB_LIBRARIES})
