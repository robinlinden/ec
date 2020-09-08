project(ec_core)

find_package(Threads REQUIRED)

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

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        Threads::Threads
)

add_subdirectory(example)
add_subdirectory(test)
