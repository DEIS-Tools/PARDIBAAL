set(HEADER_FILES
        dbm/DBM.h)
add_library(dbm
        ${HEADER_FILES}
        dbm/DBM.cpp)

install(TARGETS dbm
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(FILES ${HEADER_FILES} DESTINATION include/dbm)