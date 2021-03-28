cmake_minimum_required(VERSION 3.10)

# set the project name
project(glib)

set(CMAKE_BUILD_TYPE Debug)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)

add_executable(glib_glist glib_glist.c)
add_executable(glib_variant_ex glib_variant_ex.c)

target_include_directories(glib_glist PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(glib_glist ${GLIB_LIBRARIES})

target_include_directories(glib_variant_ex PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(glib_variant_ex ${GLIB_LIBRARIES})
