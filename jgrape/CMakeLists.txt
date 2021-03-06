project(jgrape VERSION $ENV{PRODUCT_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -DSIMPLIFIED_OUTPUT -DSWIG_STD_MODERN_STL -DSWIG_EXPORT_ITERATOR_METHODS -DDISABLE_TEXT_DICO -DDISABLE_LUA_GRAMMAR -DDISABLE_LUAW_GRAMMAR -DDISABLE_LUT_GRAMMAR -DDISABLE_LUX_GRAMMAR -DDISABLE_DEPTH_FIRST_PARSER -DDISABLE_BREADTH_FIRST_PARSER -DDISABLE_EARLEY_PARSER -DDISABLE_TO_FPRTN_PARSER -DDISABLE_TO_FPRTN_TOP_PARSER -DDISABLE_TO_FPRTN_ZPPS_PARSER -DDISABLE_TO_FPRTN_PARSER_AND_BREADTH_FIRST_EXPANDER -DDISABLE_TO_FPRTN_PARSER_AND_BLACKBOARD_SET_EXPANDER -DDISABLE_STD_SES -DDISABLE_LRB_TREE_3W_SES -DDISABLE_LRB_TREE_BS -DDISABLE_LRB_TREE_3W_BS")

set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} "-package" "com.grapenlp.core")
set(CMAKE_SWIG_OUTDIR "jgrape")
set_property(SOURCE swig/grape/jgrape.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE swig/grape/jgrape.i PROPERTY SWIG_FLAGS "-includeall")
swig_add_library(jgrape TYPE SHARED LANGUAGE java SOURCES swig/grape/jgrape.i)
swig_link_libraries(jgrape
        manager
        )

install(TARGETS jgrape EXPORT jgrapeConfig
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libjgrape)
install(EXPORT jgrapeConfig DESTINATION share/grape/jgrape/cmake COMPONENT libjgrape)
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/jgrape"
        DESTINATION "src/grape"
        COMPONENT libgrapedev
        FILES_MATCHING PATTERN "*.java")
export(TARGETS jgrape FILE jgrapeConfig.cmake)
