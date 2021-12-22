cmake_minimum_required(VERSION 3.17)
project(dbm2)

cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0069 NEW)

set(CMAKE_CXX_STANDARD 20)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif (NOT CMAKE_BUILD_TYPE)

option(DBM_BuildTests "Build the unit tests when BUILD_TESTING is enabled." ON)
option(DBM_GetDependencies "Fetch external dependencies from web." ON)

if (DBM_BuildTests)
    set(BUILD_SHARED_LIBS ON)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wpedantic -fPIC -DBUG_BOUNDS")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG -Wall -Wpedantic -fPIC")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (DBM_GetDependencies)
    # setup for external imports
    include(ExternalProject)
    set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

    file(MAKE_DIRECTORY ${EXTERNAL_INSTALL_LOCATION}/include)

    # we can now include external libraries
    include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
    link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)
endif (DBM_GetDependencies)

add_subdirectory(src)

if(BUILD_TESTING AND DBM_BuildTests)
    #testing
    add_subdirectory(test)
    enable_testing()
    add_test(NAME DBM_test            COMMAND DBM_test)
    add_test(NAME bounds_table_test   COMMAND bounds_table_test)
    add_test(NAME bound_test          COMMAND bound_test)
endif()