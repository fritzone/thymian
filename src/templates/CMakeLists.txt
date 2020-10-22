cmake_minimum_required (VERSION 3.14)
set(PROJECT templater)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)

if(SCRIPTED_TEMPLATES)
    include_directories(${DRV2WEB_ROOT_DIR}/script/inc)
endif()

set(common_src
    templater.cpp
    templater.h
)

add_library(${PROJECT} STATIC ${common_src})

if(SCRIPTED_TEMPLATES)
    target_link_libraries(${PROJECT} cxxtools common)
else()
    target_link_libraries(${PROJECT} cxxtools common)
endif()

find_package(Boost 1.66.0 REQUIRED)
message("Boost is in:" ${Boost_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_executable(template_compiler main.cpp)
target_link_libraries(template_compiler ${PROJECT} boost_filesystem)
