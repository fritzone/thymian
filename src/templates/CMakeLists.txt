cmake_minimum_required (VERSION 3.14)
set(PROJECT templater)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)

set(common_src
    templater.cpp
    dictionary.cpp
    templater.h
)

add_library(${PROJECT} STATIC ${common_src})

target_link_libraries(${PROJECT} cxxtools common tntdb ${Python3_LIBRARIES})

find_package(Boost 1.66.0 REQUIRED)
message("Boost is in:" ${Boost_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_executable(template_compiler main.cpp)
target_link_libraries(template_compiler ${PROJECT} boost_filesystem)
