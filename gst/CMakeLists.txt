cmake_minimum_required(VERSION 3.10)

# set the project name
project(gst)

set(CMAKE_BUILD_TYPE Debug)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
pkg_check_modules(GST REQUIRED gstreamer-1.0)
pkg_check_modules(APR REQUIRED apr-1)
pkg_check_modules(PBUTILS REQUIRED gstreamer-pbutils-1.0)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)
pkg_check_modules(TAGLIB_C REQUIRED taglib_c)

# add the executable
add_executable(gst_mp3_ui gst_mp3_ui.c)
add_executable(gst_simple_audio gst_simple_audio.c)
add_executable(basic-tutorial-1 basic-tutorial-1.c)
add_executable(gst_mp3_audio gst_mp3_audio.c)
add_executable(file file_util.c)
add_executable(gst_discover gst_discover.c)
add_executable(file_scan file_scan.c)

target_include_directories(gst_mp3_ui PUBLIC ${GTK_INCLUDE_DIRS})
target_include_directories(gst_mp3_ui PUBLIC ${GST_INCLUDE_DIRS})
target_link_libraries(gst_mp3_ui ${GTK_LIBRARIES})
target_link_libraries(gst_mp3_ui ${GST_LIBRARIES})

target_include_directories(gst_simple_audio PUBLIC ${GST_INCLUDE_DIRS})
target_link_libraries(gst_simple_audio ${GST_LIBRARIES})

target_include_directories(basic-tutorial-1 PUBLIC ${GST_INCLUDE_DIRS})
target_link_libraries(basic-tutorial-1 ${GST_LIBRARIES})

target_include_directories(gst_mp3_audio PUBLIC ${GST_INCLUDE_DIRS})
target_link_libraries(gst_mp3_audio ${GST_LIBRARIES})

target_include_directories(file PUBLIC ${APR_INCLUDE_DIRS})
target_link_libraries(file ${APR_LIBRARIES})

target_include_directories(gst_discover PUBLIC ${GST_INCLUDE_DIRS})
target_link_libraries(gst_discover ${GST_LIBRARIES})
target_include_directories(gst_discover PUBLIC ${PBUTILS_INCLUDE_DIRS})
target_link_libraries(gst_discover ${PBUTILS_LIBRARIES})

target_include_directories(file_scan PUBLIC ${SQLITE3_INCLUDE_DIRS})
target_link_libraries(file_scan ${SQLITE3_LIBRARIES})
target_include_directories(file_scan PUBLIC ${TAGLIB_C_INCLUDE_DIRS})
target_link_libraries(file_scan ${TAGLIB_C_LIBRARIES})
