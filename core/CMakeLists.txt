add_library(WebSearcherCore STATIC)

set(CORE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/inc PARENT_SCOPE)


file(GLOB INC_FILES ${CMAKE_CURRENT_LIST_DIR}/inc/*.hpp)
file(GLOB SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

target_sources (
    WebSearcherCore
    PRIVATE ${SRC_FILES} ${INC_FILES}
)

target_include_directories (
    WebSearcherCore
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/inc
)

target_link_libraries(WebSearcherCore PRIVATE libcurl rapidxml)

target_compile_options(WebSearcherCore PRIVATE "/std:c++latest")