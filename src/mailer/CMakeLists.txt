cmake_minimum_required (VERSION 3.14)
set(PROJECT mailer)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)

set(${PROJECT}_src
    main.cpp
)

add_executable(${PROJECT} ${${PROJECT}_src})

target_link_libraries(${PROJECT} vmime templater)
