cmake_minimum_required(VERSION 3.27)
project(itmo_realtime_computer_graphics LANGUAGES CXX)

if (MSVC)
    set(CMAKE_VS_GLOBALS
            "UseMultiToolTask=true"
            "EnforceProcessCountAcrossBuilds=true")
endif ()

add_subdirectory(src)
add_subdirectory(apps)
