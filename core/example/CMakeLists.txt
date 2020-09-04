function(add_example name file)
    add_executable(${name} ${file})
    target_compile_features(${name} PRIVATE cxx_std_20)
    target_link_libraries(${name} PRIVATE ec::core)
endfunction()

add_example(ec_core_active_object src/active_object.cpp)
add_example(ec_core_event_service src/event_service.cpp)
add_example(ec_core_timer_service src/timer_service.cpp)
