set(HEADER_FILES
        dbm2/DBM.h
        dbm2/bounds_table_t.h
        dbm2/bound_t.h
        dbm2/Federation.h)

add_library(dbm2
        ${HEADER_FILES}
        dbm2/DBM.cpp
        dbm2/bounds_table_t.cpp
        dbm2/bound_t.cpp
        dbm2/Federation.cpp)

target_include_directories (dbm2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS dbm2
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(FILES ${HEADER_FILES} DESTINATION include/dbm)