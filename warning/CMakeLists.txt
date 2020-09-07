project(ec_warning)

add_library(${PROJECT_NAME} INTERFACE)
add_library(ec::warning ALIAS ${PROJECT_NAME})

target_compile_options(${PROJECT_NAME} INTERFACE
    $<$<CXX_COMPILER_ID:Clang,GNU>:
        -pedantic-errors;
        -Wall;
        -Werror;
        -Wextra;
    >
)
