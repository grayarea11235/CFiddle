cmake_minimum_required(VERSION 3.10)

# set the project name
project(myplayer)

set(CMAKE_BUILD_TYPE Debug)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
pkg_check_modules(GST REQUIRED gstreamer-1.0)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)
pkg_check_modules(APR REQUIRED apr-1)
# pkg_check_modules(PBUTILS REQUIRED gstreamer-pbutils-1.0)

# add the executable
add_executable(myplayer 
  main.c 
  gui.c 
  gst.c
  datastore.c
  config.c
  logging.c)

target_include_directories(myplayer PUBLIC ${GTK_INCLUDE_DIRS})
target_include_directories(myplayer PUBLIC ${GST_INCLUDE_DIRS})
target_include_directories(myplayer PUBLIC ${APR_INCLUDE_DIRS})
target_include_directories(myplayer PRIVATE ".")

target_link_libraries(myplayer ${GTK_LIBRARIES})
target_link_libraries(myplayer ${GST_LIBRARIES})
target_link_libraries(myplayer ${SQLITE3_LIBRARIES})
target_link_libraries(myplayer ${APR_LIBRARIES})
target_link_libraries(myplayer m)
