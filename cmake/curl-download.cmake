cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(curl-download NONE)
include(ExternalProject)

set(CURL_SUBMODULES "") 

ExternalProject_Add (
    curl
    SOURCE_DIR "@CURL_DOWNLOAD_ROOT@/curl-src"
    BINARY_DIR "@CURL_DOWNLOAD_ROOT@/curl-build"
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG "master"
    GIT_SUBMODULES "${CURL_SUBMODULES}"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    TEST_COMMAND ""
)

unset(CURL_SUBMODULES)