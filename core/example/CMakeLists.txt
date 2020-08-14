add_executable(ec_core_active_object
    src/active_object.cpp
)

target_compile_features(ec_core_active_object
    PRIVATE
        cxx_std_20
)

target_link_libraries(ec_core_active_object
    PRIVATE
        ec::core
)
