cmake_minimum_required(VERSION 3.28.3)
project(Bootcamp LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB Sources CONFIGURE_DEPENDS "src/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin")
foreach(Source ${Sources})
    get_filename_component(Exec ${Source} NAME_WE)
    add_executable(${Exec} ${Source})
endforeach()