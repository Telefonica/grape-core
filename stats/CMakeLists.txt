project(stats VERSION 0.1 LANGUAGES CXX)

add_library(stats STATIC src/stats.cpp)
target_include_directories(stats PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)
target_link_libraries(stats
        unicode
        util
        )

install(TARGETS stats EXPORT statsConfig
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT statsConfig DESTINATION share/stats/cmake)
export(TARGETS stats FILE statsConfig.cmake)
