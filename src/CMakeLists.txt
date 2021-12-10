set(HEADER_FILES
        dbm/DBM.h
        dbm/bounds_table_t.h
        dbm/bound_t.h)

add_library(dbm
        ${HEADER_FILES}
        dbm/DBM.cpp
        dbm/bounds_table_t.cpp
        dbm/bound_t.cpp)

target_include_directories (dbm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS dbm
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(FILES ${HEADER_FILES} DESTINATION include/dbm)