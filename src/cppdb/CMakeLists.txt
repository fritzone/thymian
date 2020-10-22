include_directories (${CMAKE_CURRENT_SOURCE_DIR})

set(cppdb_src 
    cppdb.cpp
    cppdb.h
)


add_library(cppdb STATIC ${cppdb_src})
