add_subdirectory(gtest)

include_directories(
    ../templates
    ./gtest/include
    ../web
    ${CMAKE_CURRENT_SOURCE_DIR}
    ../script/inc 
    ../script/vm
    ../script/runtime 
    ../script/compiler ./gtest/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(SCRIPTED_TEMPLATES)

    add_executable(vmtest
        ${CMAKE_CURRENT_SOURCE_DIR}/code_exec.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/scripttest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/var_def.cpp
        ${CMAKE_BINARY_DIR}/tests/gen_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gtest/src/gtest_main.cc
    )

    add_dependencies(vmtest testgen)

    if(WIN32)
        add_definitions("/D_VARIADIC_MAX=10")
    endif()

    if(WIN32)
      add_custom_command(
        MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/generated.cpp
        OUTPUT ${CMAKE_BINARY_DIR}/tests/gen_test.cpp
        COMMAND testgen
        COMMENT "Running TESTGEN"
        VERBATIM
      )
    else()
      add_custom_command(
        MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/generated.cpp
        OUTPUT ${CMAKE_BINARY_DIR}/tests/gen_test.cpp
        COMMAND ${CMAKE_BINARY_DIR}/bin/testgen
        COMMENT "Running TESTGEN: ${CMAKE_BINARY_DIR}/bin/testgen"
        VERBATIM
      )
    endif()


    include_directories(${CMAKE_BINARY_DIR}/compiler
        ${CMAKE_BINARY_DIR}/runtime
    )

    target_link_libraries(vmtest nap_rt nap_bci_s nap_cpl_s gtest)

endif()

add_executable(templ_test
    ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gtest/src/gtest_main.cc
)


target_link_libraries(templ_test   ${CONAN_LIBS} common gtest templater cppdb pthread  )
