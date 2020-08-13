cmake_minimum_required(VERSION 3.17)

project(ec)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_std_20
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -fconcepts
)
