function(add_example name file)
    add_executable(${name} ${file})

    target_link_libraries(${name}
        PRIVATE
            ec::core
            ec::warning
    )

    target_compile_features(${name}
        PRIVATE
            cxx_std_20
    )

    set_target_properties(${name}
        PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO
    )
endfunction()

add_example(ec_core_active_object src/active_object.cpp)
add_example(ec_core_event_service src/event_service.cpp)
add_example(ec_core_timer_service src/timer_service.cpp)
