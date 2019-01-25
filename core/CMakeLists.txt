add_library(WebSearcherCore STATIC)

file(GLOB INC_FILES ${CMAKE_CURRENT_LIST_DIR}/inc/*.hpp)
file(GLOB SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

target_sources (
    WebSearcherCore
    PUBLIC ${SRC_FILES} ${INC_FILES}
)

target_include_directories (
    WebSearcherCore
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc ${CMAKE_CURRENT_LIST_DIR}/inl
)

target_compile_options(WebSearcherCore PUBLIC "/std:c++latest")