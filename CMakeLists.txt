cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)
project(libGottaCatchEmAll C CXX)

find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)

SET(CMAKE_CXX_STANDARD_REQUIRED ON)
if(CMAKE_BUILD_TYPE MATCHES Release)
    message("Release Build")
    add_compile_options("-O3")
    add_compile_options("-DNDEBUG")
elseif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    message("Release Build With Debug and Profiling Info")
    add_compile_options("-O3")
    add_compile_options("-pg")
    add_compile_options("-DNDEBUG")
else() # default to Debug
    message("Debug Build")
    add_compile_options("-g")
    if (CMAKE_COMPILER_IS_GNUCXX)
        add_compile_options("-Og")
    endif()
    set(CMAKE_BUILD_TYPE Debug)
endif ()

# Until target_link_options comes out, this is necessary.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

# I think globbing is fine in small projects. For large projects, this is dangerous.
file(GLOB_RECURSE EXAMPLE_SOURCES ${PROJECT_SOURCE_DIR}/include/*.hpp
                                  ${PROJECT_SOURCE_DIR}/include/*.h
                                  ${PROJECT_SOURCE_DIR}/src/*.c
                                  ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Filter out emacs swapfiles if possible.
if (CMAKE_VERSION VERSION_GREATER "3.6.0")
  list(FILTER EXAMPLE_SOURCES EXCLUDE REGEX ^\\.)
  list(FILTER EXAMPLE_SOURCES EXCLUDE REGEX \#)
endif()

add_library(Example ${EXAMPLE_SOURCES})
target_include_directories(Example PRIVATE ${PROJECT_SOURCE_DIR}/include/ExampleDir)
target_compile_definitions(Example PRIVATE Wall lpthread)
add_executable(Exemplar ${PROJECT_SOURCE_DIR}/main.cpp)
target_include_directories(Exemplar PRIVATE ${PROJECT_SOURCE_DIR}/include/ExampleDir)
target_compile_definitions(Exemplar PRIVATE Wall)
target_link_libraries(Exemplar PUBLIC Example Threads::Threads -lpthread)
set_target_properties(Example Exemplar PROPERTIES CXX_STANDARD 11
                                                  ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
                                                  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/exe"
                                                  LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

if (BUILD_TESTING)
  file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/reports)
  message(STATUS "Testing enabled.")
  enable_testing()
  SET(UNIT_TEST_LIST ExampleDir/Example)
  foreach(NAME IN LISTS UNIT_TEST_LIST)
    string(LENGTH ${NAME} UNIT_TEST_ELEMENT_LENGTH)
    string(FIND ${NAME} "/" LAST_SLASH_POS REVERSE)
    string(SUBSTRING ${NAME} 0 ${LAST_SLASH_POS} UNIT_TEST_PATH)
    MATH(EXPR LAST_SLASH_POS_PLUS_ONE "${LAST_SLASH_POS}+1")
    string(SUBSTRING ${NAME} ${LAST_SLASH_POS_PLUS_ONE} ${UNIT_TEST_ELEMENT_LENGTH} UNIT_TEST_NAME)
    add_executable(${UNIT_TEST_NAME}_test ${PROJECT_SOURCE_DIR}/tests/${NAME}.test.cpp)
    target_link_libraries(${UNIT_TEST_NAME}_test PUBLIC ${UNIT_TEST_NAME} Threads::Threads)
    target_include_directories(${UNIT_TEST_NAME}_test PRIVATE ${PROJECT_SOURCE_DIR}/include/3rdParty/Catch/
                                                              ${PROJECT_SOURCE_DIR}/include/${UNIT_TEST_PATH})
    set_target_properties(${UNIT_TEST_NAME}_test PROPERTIES CXX_STANDARD 11
                                                            ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
                                                            RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/test"
                                                            LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
    if (TEST_TAGS)
      string(REPLACE ", " "_" CLEAN_TEST_TAGS ${TEST_TAGS})
    endif()
    add_test(NAME ${UNIT_TEST_NAME}_test COMMAND ${UNIT_TEST_NAME}_test -s -o
                                                 ${PROJECT_BINARY_DIR}/reports/${UNIT_TEST_NAME}_report_${CLEAN_TEST_TAGS}.xml
                                                 -r junit "${TEST_TAGS}")
  endforeach()
endif()
