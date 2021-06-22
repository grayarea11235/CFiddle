cmake_minimum_required(VERSION 3.10)

# set the project name
project(sqlite)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)
pkg_check_modules(GLIB REQUIRED glib-2.0)

# add the executable
add_executable(sqlite_getversion sqlite_getversion.c)
add_executable(sqlite_getversion2 sqlite_getversion2.c)
add_executable(scan_audio_dir scan_audio_dir.c)

target_include_directories(sqlite_getversion PUBLIC ${SQLITE3_INCLUDE_DIRS})
target_link_libraries(sqlite_getversion ${SQLITE3_LIBRARIES})

target_include_directories(sqlite_getversion2 PUBLIC ${SQLITE3_INCLUDE_DIRS})
target_link_libraries(sqlite_getversion2 ${SQLITE3_LIBRARIES})

target_include_directories(scan_audio_dir PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(scan_audio_dir ${GLIB_LIBRARIES})
target_include_directories(scan_audio_dir PUBLIC ${SQLITE3_INCLUDE_DIRS})
target_link_libraries(scan_audio_dir ${SQLITE3_LIBRARIES})
