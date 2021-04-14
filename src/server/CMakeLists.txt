cmake_minimum_required (VERSION 3.0)
set (PROJECT thymian)

set (ECPPC /usr/local/bin/ecppc)

set (WEB_COMPONENTS
     root
     filer
     main_root_sender
     category_root_sender
)

include_directories(
  "/usr/local/include/"
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(${PROJECT}_SOURCES
  ""
  CACHE INTERNAL ${PROJECT}_SOURCES
)

OPTION(BUILD_EXECUTABLE_WEB_COMPONENT "Build a standalone executable of the web component. If you don't select this, a library will be built, you will need to run it with tntnet." ON)

foreach(comp ${WEB_COMPONENTS})
  list(APPEND ${PROJECT}_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/${comp}.cpp )
  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${comp}.cpp
                     COMMAND ${ECPPC} -o ${CMAKE_CURRENT_BINARY_DIR}/${comp}.cpp -v -n ${comp} ${CMAKE_CURRENT_SOURCE_DIR}/${comp}.ecpp
                     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${comp}.ecpp)
  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${comp}.cpp)
endforeach()

set(CPP_SOURCES
    file_sender.cpp
    category_sender.cpp
    main_sender.cpp
    web_component.cpp
    flood_check.cpp
    config.cpp
    web_logmachine.cpp
)

set(CPP_HEADERS
    category_sender.h
    config.h
    fake_locations.h
    file_sender.h
    flood_check.h
    main_sender.h
    web_component.h
    web_logmachine.h
)

if(BUILD_EXECUTABLE_WEB_COMPONENT)

  set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++14 -Wno-deprecated -Wall -Wextra -Wstrict-aliasing -pedantic -fmax-errors=5 -Wno-error=strict-overflow -Wunreachable-code -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Wno-old-style-cast -Woverloaded-virtual -Wredundant-decls -Wno-shadow -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option --param max-gcse-memory=200000000 --param=max-vartrack-size=600000000")

  add_library(${PROJECT}_lib ${CPP_HEADERS} ${CPP_SOURCES})
  target_link_libraries(${PROJECT}_lib ${CONAN_LIBS} cxxtools dl tntnet tntdb vmime fpaq0 templater common pthread  )

  add_executable(${PROJECT}_exe main.cpp ${${PROJECT}_SOURCES})
  target_link_libraries(${PROJECT}_exe ${PROJECT}_lib)

else()
  add_library(${PROJECT} SHARED ${${PROJECT}_SOURCES})
  set_target_properties(${PROJECT} PROPERTIES PREFIX "")
  target_link_libraries(${PROJECT} ${CONAN_LIBS} cxxtools dl tntnet tntdb vmime fpaq0 templater common pthread )
endif()

