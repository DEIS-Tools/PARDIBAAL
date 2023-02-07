set(HEADER_FILES
        pardibaal/Federation.h
        pardibaal/DBM.h
        pardibaal/bounds_table_t.h
        pardibaal/bound_t.h
        pardibaal/difference_bound_t.h)

add_library(pardibaal
        ${HEADER_FILES}
        pardibaal/Federation.cpp
        pardibaal/DBM.cpp
        pardibaal/bounds_table_t.cpp
        pardibaal/bound_t.cpp
        pardibaal/difference_bound_t.cpp)

target_include_directories (pardibaal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS pardibaal
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(FILES ${HEADER_FILES} DESTINATION include/pardibaal)
