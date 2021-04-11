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
option(ENABLE_TESTS "Enable some unit tests" ON)
option(CLANG_SANITIZE "Use specific clang sanitizer compilation, development only" OFF)
option(coverage-build "gcov/lcov test coverage analysis (make coverage_test)" OFF)

# Python
find_package (Python3
    COMPONENTS Interpreter Development Compiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_COMPILER_FLAGS "-pedantic")
  
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    if(CLANG_SANITIZE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILER_FLAGS} -fsanitize=address -fno-omit-frame-pointer -O1 -fno-optimize-sibling-calls")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILER_FLAGS} -fPIC -fsanitize=address -fno-omit-frame-pointer -O1 -fno-optimize-sibling-calls")
    endif()
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILER_FLAGS} -fpermissive -finline-functions -Wno-long-long -fvisibility-inlines-hidden")
endif()

# Coverage build?
if(coverage-build)
     set(CMAKE_BUILD_TYPE Debug)
     message("building for coverage ...")
     SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage -rdynamic -ftest-coverage -fprofile-arcs -fpermissive -finline-functions -Wno-long-long -fvisibility-inlines-hidden -fno-omit-frame-pointer -fsanitize=address")
     SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage -rdynamic -ftest-coverage -fprofile-arcs -fpermissive -finline-functions -Wno-long-long -fvisibility-inlines-hidden -fno-omit-frame-pointer -fsanitize=address")
     include(cmake/CodeCoverage.cmake)
     SETUP_TARGET_FOR_COVERAGE(templater_test ctest coverage)
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
  ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/maddy
  ${CMAKE_CURRENT_SOURCE_DIR}/common
  ${CMAKE_CURRENT_SOURCE_DIR}/cppdb
  ${Python3_INCLUDE_DIRS}
)

add_definitions("-DPYTHON_SCRIPTING")

add_subdirectory(cppdb)
add_subdirectory(common)
add_subdirectory(3rdparty/tinyxml)
add_subdirectory(3rdparty/compressor)
add_subdirectory(mailer)
add_subdirectory(templates)
add_subdirectory(server)

if(ENABLE_TESTS)
    add_subdirectory(tests)
    enable_testing()
    add_test(NAME templater_test COMMAND templ_test)
endif()
