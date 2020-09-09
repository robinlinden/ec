project(test_ec_core)

include(Catch)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/test_active_object.cpp
    src/test_event_service.cpp
    src/test_timer_service.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ec::core
        ec::warning
        Catch2::Catch2
)

catch_discover_tests(${PROJECT_NAME})
