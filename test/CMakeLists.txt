find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories (${TEST_SOURCE_DIR}/src
        ${Boost_INCLUDE_DIRS}
        ${DBM_SOURCE_DIR}
        )

add_definitions (-DBOOST_TEST_DYN_LINK)

add_executable(DBM_test DBM_test.cpp)
add_executable(bounds_table_test bounds_table_test.cpp)
add_executable(bound_test bound_test.cpp)


target_link_libraries(DBM_test ${Boost_LIBRARIES} dbm2)
target_link_libraries(bounds_table_test ${Boost_LIBRARIES} dbm2)
target_link_libraries(bound_test ${Boost_LIBRARIES} dbm2)