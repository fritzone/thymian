cmake_minimum_required (VERSION 3.14)

project(thymian)

# CMake Settings
SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_VERBOSE_MAKEFILE OFF)
set(THYMIAN_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions("-DDEBUG_BUILD")
    message("Debug build for development purposes only")
ENDIF()

include(cmake/TargetArch.cmake)
target_architecture(TARGET_CPU)
message("Targeting: ${TARGET_CPU}")

# Options
option(BUILD_WEB "Build the webserver component" ON)

OPTION(ENABLE_TESTS "Enable some unit tests" ON)

OPTION(BUILD_MAILER "Build the mail sender application" OFF)

OPTION(CLANG_SANITIZE "Use specific clang sanitizer compilation, development only" OFF)

# Python
find_package(PythonInterp)

set(COMMON_COMPILER_FLAGS "-std=c++1y -ggdb -pedantic -Wno-old-style-cast")
  
  # C++11 for linux common for both gcc and clang
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET(BUILD_CLIENT OFF CACHE INTERNAL "Building the client component is not possible with clang" FORCE)

    if(CLANG_SANITIZE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILER_FLAGS} -fsanitize=address -fno-omit-frame-pointer -O1 -fno-optimize-sibling-calls")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILER_FLAGS} -fPIC -fsanitize=address -fno-omit-frame-pointer -O1 -fno-optimize-sibling-calls")
    endif()

else()
    if(COVERAGE_BUILD)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILER_FLAGS} -ftest-coverage -fprofile-arcs -fpermissive -finline-functions -Wno-long-long -fvisibility-inlines-hidden")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILER_FLAGS} -fpermissive -finline-functions -Wno-long-long -fvisibility-inlines-hidden")
    endif()
  endif()

MESSAGE("CMAKE FLAGS: ${CMAKE_CXX_FLAGS}")
add_definitions("-D_GLIBCXX_USE_CXX11_ABI")

message("Generator: ${CMAKE_GENERATOR}")

## Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
    # Optimize the stuff if building RELEASE
    if(CMAKE_BUILD_TYPE MATCHES RELEASE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Os")        ## Optimize
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")  ## Strip binary
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")        ## All warnings, please
    endif()

    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        message("GCC version: ${CMAKE_CXX_COMPILER_VERSION}")

        if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7.1)
            add_definitions(-Wimplicit-fallthrough=0)
        endif()
    endif()

endif()

# Generate the header files for version info
set(WEB_VERSION_MAJOR 1)
set(WEB_VERSION_MINOR 0)
set(WEB_VERSION_PATCH 0)

configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.tpl
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
)

# Include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
  SYSTEM
  ${CONAN_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/templates
  ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/compressor
  ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/tinyxml
  ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/config4cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/common
  ${CMAKE_CURRENT_SOURCE_DIR}/cppdb
)

add_subdirectory(cppdb)
add_subdirectory(common)
add_subdirectory(3rdparty/config4cpp)
add_subdirectory(3rdparty/tinyxml)
add_subdirectory(3rdparty/compressor)
add_subdirectory(mailer)
add_subdirectory(templates)

if(ENABLE_TESTS)
    add_subdirectory(tests)
    enable_testing()
    add_test(NAME templater_test COMMAND templ_test)
endif()
