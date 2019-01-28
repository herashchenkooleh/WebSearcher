macro(fetch_curl _download_module_path _download_root)

    set(CURL_DOWNLOAD_ROOT ${_download_root})

    configure_file (
        ${_download_module_path}/curl-download.cmake
        ${_download_root}/CMakeLists.txt
        @ONLY
    )

    unset(CURL_DOWNLOAD_ROOT)

    execute_process (
        COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY ${_download_root}
    )

    execute_process (
        COMMAND "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY ${_download_root}
    )

    add_subdirectory (
        ${_download_root}/curl-src
        ${_download_root}/curl-build
    )
endmacro()