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
    file(MAKE_DIRECTORY ${EXTERNAL_INSTALL_LOCATION}/lib)
    if (UNIX AND NOT APPLE)
        set(BOOST_CONFIG_JAM "using gcc : : ${CMAKE_CXX_COMPILER}")
        set(BOOST_B2_OPTS "")
    elseif (APPLE)
        set(BOOST_CONFIG_JAM "using gcc : : ${CMAKE_CXX_COMPILER}")
        set(BOOST_B2_OPTS "")
    else ()
        set(BOOST_CONFIG_JAM "using gcc : : ${CMAKE_CXX_COMPILER}")
        set(BOOST_B2_OPTS target-os=windows binary-format=pe abi=ms threading=multi threadapi=win32)
    endif ()

    if (ARCH_TYPE MATCHES "win64")
        set(BOOST_CONFIG_JAM "using gcc : : ${CMAKE_CXX_COMPILER}")
    endif ()

    if(BUILD_TESTING AND DBM_BuildTests)
        ExternalProject_Add(
            boost-ext
            URL https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.zip
            URL_HASH SHA256=f22143b5528e081123c3c5ed437e92f648fe69748e95fa6e2bd41484e2986cc3
            PATCH_COMMAND echo "${BOOST_CONFIG_JAM}" $<SEMICOLON>  > config.jam
            CONFIGURE_COMMAND ./bootstrap.sh --prefix=${CMAKE_BINARY_DIR}/external cxxflags="-arch x86_64" --without-icu
            BUILD_COMMAND ./b2 --user-config=config.jam --prefix=${CMAKE_BINARY_DIR}/external --with-test cxxstd=17 cflags="-fPIC" cxxflags="-fPIC" address-model=64 ${BOOST_B2_OPTS} variant=release link=shared runtime-link=shared install
            BUILD_IN_SOURCE 1
            INSTALL_COMMAND ""
        )
    endif()

    # we can now include external libraries
    set(CMAKE_PREFIX_PATH ${EXTERNAL_INSTALL_LOCATION};${CMAKE_PREFIX_PATH})
    include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
    link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)
endif (DBM_GetDependencies)

add_subdirectory(src)
if(BUILD_TESTING AND DBM_BuildTests)
    if (CMAKE_BUILD_TYPE MATCHES Prebuild)
        # dummy for pre-build
        add_custom_target( prebuild )
        add_dependencies(prebuild boost-ext)
    else ( )

        #testing
        if (DBM_GetDependencies)
        endif (DBM_GetDependencies)

        enable_testing()
        add_subdirectory(test)
    endif()
endif()
