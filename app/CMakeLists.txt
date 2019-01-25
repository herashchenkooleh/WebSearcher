add_executable (
    WebSearcher
)

file(GLOB SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

target_sources (
    WebSearcher
    PUBLIC ${SRC_FILES}
)