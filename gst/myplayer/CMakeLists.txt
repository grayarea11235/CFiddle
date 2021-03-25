cmake_minimum_required(VERSION 3.16)

# set the project name
project(myplayer)

set(CMAKE_BUILD_TYPE Debug)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
pkg_check_modules(GST REQUIRED gstreamer-1.0)
# pkg_check_modules(APR REQUIRED apr-1)
# pkg_check_modules(PBUTILS REQUIRED gstreamer-pbutils-1.0)

# add the executable
add_executable(myplayer 
  main.c 
  gui.c 
  gst.c)

target_include_directories(myplayer PUBLIC ${GTK_INCLUDE_DIRS})
target_include_directories(myplayer PUBLIC ${GST_INCLUDE_DIRS})
target_include_directories(myplayer PRIVATE ".")
target_link_libraries(myplayer ${GTK_LIBRARIES})
target_link_libraries(myplayer ${GST_LIBRARIES})
