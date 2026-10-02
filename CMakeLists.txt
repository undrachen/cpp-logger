cmake_minimum_required(VERSION 3.20)

project(cpp-logger
    VERSION 0.1.0
    LANGUAGES CXX
)

include(FetchContent)
include(CTest)

add_subdirectory(src)
add_subdirectory(examples)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
