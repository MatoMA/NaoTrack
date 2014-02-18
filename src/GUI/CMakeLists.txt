cmake_minimum_required(VERSION 2.6)
project(NaoGUI)

# C++11 support
add_definitions(-std=c++11 -O3)

### GTK Lib
# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

### BOOST Lib
find_package(Boost REQUIRED COMPONENTS system thread)

# Executables
add_executable(position_viewer position-viewer.cc)

# Link the target to the GTK+ libraries
target_link_libraries(position_viewer ${GTK3_LIBRARIES} ${Boost_LIBRARIES} pthread)

# CloudReader
include_directories(include)
add_executable(Analyser Test.cpp Point.hpp)
target_link_libraries(Analyser ${Boost_LIBRARIES} pthread)