cmake_minimum_required(VERSION 3.17)

project(ec)

enable_testing()

add_subdirectory(third_party/catch2)

add_subdirectory(core)
add_subdirectory(warning)
