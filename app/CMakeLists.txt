add_executable (
    WebSearcher
)

file(GLOB SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

target_sources (
    WebSearcher
    PUBLIC ${SRC_FILES}
)

find_package(Qt5Widgets CONFIG REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

target_include_directories(WebSearcher PUBLIC ${Qt5Widgets_INCLUDES})

target_compile_definitions(WebSearcher PUBLIC ${Qt5Widgets_DEFINITIONS})

target_link_libraries(WebSearcher ${Qt5Widgets_LIBRARIES})
