project(ec_core)

add_library(${PROJECT_NAME} INTERFACE)
add_library(ec::core ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
)

target_compile_features(${PROJECT_NAME}
    INTERFACE
        cxx_std_20
)

target_compile_options(${PROJECT_NAME}
    INTERFACE
        -fconcepts
)

add_subdirectory(example)
