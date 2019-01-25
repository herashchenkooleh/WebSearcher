cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(WebSearcher CXX)


set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(app)
add_subdirectory(core)