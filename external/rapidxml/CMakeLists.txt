add_library(rapidxml INTERFACE)

file(GLOB INC_FILES ${CMAKE_CURRENT_LIST_DIR}/include/*.hpp)

target_sources(rapidxml INTERFACE ${INC_FILES})

target_include_directories(rapidxml INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

target_compile_options(rapidxml INTERFACE "/std:c++latest")