project(fprtn VERSION 0.1 LANGUAGES CXX)

add_library(fprtn STATIC
        src/fprtn.cpp
        src/a_fprtn.cpp
        src/ua_fprtn.cpp
        src/aw_fprtn.cpp
        src/uaw_fprtn.cpp
        src/x_fprtn.cpp
        src/xw_fprtn.cpp
        src/ux_fprtn.cpp
        src/uxw_fprtn.cpp
        )
target_include_directories(fprtn PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)
target_link_libraries(fprtn segment)

install(TARGETS fprtn EXPORT fprtnConfig
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT fprtnConfig DESTINATION share/fprtn/cmake)
export(TARGETS fprtn FILE fprtnConfig.cmake)

#Test files
add_executable(fprtn_test src/fprtn_test.cpp)
target_link_libraries(fprtn_test fprtn)
