cmake_minimum_required(VERSION 3.16)
project(httputil VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

enable_testing()

add_subdirectory(httputil)
add_subdirectory(tests)
