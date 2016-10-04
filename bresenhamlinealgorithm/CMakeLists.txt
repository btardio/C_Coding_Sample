project(bresenham)

cmake_minimum_required(VERSION 3.4)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})

link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(bresenham bresenham.c) 

target_link_libraries(bresenham ${GTK3_LIBRARIES})
target_link_libraries(bresenham -lm)

install(TARGETS bresenham RUNTIME DESTINATION bin)


