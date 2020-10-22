cmake_minimum_required (VERSION 3.14)

set(project common)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(${project}_src
    common.cpp
    logger.cpp
    words.cpp    
    logstream.cpp
    logstreamhelper.h
    url_breaker.cpp
)

add_library(${project} STATIC ${${project}_src})


message("Common links with: ${project_libs}")
target_link_libraries(${project} ${CONAN_LIBS} tinyxml fpaq0 cxxtools pthread m stdc++)
