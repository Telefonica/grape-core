project(output_fprtn_top VERSION 0.1 LANGUAGES CXX)

add_library(output_fprtn_top STATIC
        src/output_fprtn_top.cpp
        src/aw_output_fprtn_top.cpp
        src/uaw_output_fprtn_top.cpp
        src/luaw_output_fprtn_top.cpp
        src/xw_output_fprtn_top.cpp
        src/uxw_output_fprtn_top.cpp
        src/luxw_output_fprtn_top.cpp
        )

target_include_directories(output_fprtn_top PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

target_link_libraries(output_fprtn_top
        machine
        output_fprtn
        )

install(TARGETS output_fprtn_top EXPORT output_fprtn_topConfig
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT output_fprtn_topConfig DESTINATION share/output_fprtn_top/cmake)
export(TARGETS output_fprtn_top FILE output_fprtn_topConfig.cmake)
