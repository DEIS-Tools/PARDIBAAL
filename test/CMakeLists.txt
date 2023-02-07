find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories (${TEST_SOURCE_DIR}/src
        ${Boost_INCLUDE_DIRS}
        ${DBM_SOURCE_DIR}
        )

add_definitions (-DBOOST_TEST_DYN_LINK)

add_executable(Federation_test       Federation_test.cpp)
add_executable(DBM_test              DBM_test.cpp)
add_executable(bounds_table_test     bounds_table_test.cpp)
add_executable(bound_test            bound_test.cpp)
add_executable(difference_bound_test difference_bound_test.cpp)

target_link_libraries(Federation_test       ${Boost_LIBRARIES} pardibaal)
target_link_libraries(DBM_test              ${Boost_LIBRARIES} pardibaal)
target_link_libraries(bounds_table_test     ${Boost_LIBRARIES} pardibaal)
target_link_libraries(bound_test            ${Boost_LIBRARIES} pardibaal)
target_link_libraries(difference_bound_test ${Boost_LIBRARIES} pardibaal)

add_test(NAME Federation_test       COMMAND Federation_test)
add_test(NAME DBM_test              COMMAND DBM_test)
add_test(NAME bounds_table_test     COMMAND bounds_table_test)
add_test(NAME bound_test            COMMAND bound_test)
add_test(NAME difference_bound_test COMMAND difference_bound_test)
