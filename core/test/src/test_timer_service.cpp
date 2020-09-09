#include "ec/core/timer_service.h"

#include <catch2/catch.hpp>

#include <chrono>
#include <thread>

using namespace ec::core;
using namespace std::chrono_literals;

namespace {

TEST_CASE("timer_service") {
    ActiveObject ctx{};
    TimerService ts{&ctx};

    SECTION("call_after works") {
        bool called = false;
        ts.call_after(50ms, [&] { called = true; });
        std::this_thread::sleep_for(30ms);
        REQUIRE_FALSE(called);
        std::this_thread::sleep_for(40ms);
        REQUIRE(called);
    }

    SECTION("call_every works") {
        int calls = 0;
        ts.call_every(50ms, [&] { ++calls; });
        std::this_thread::sleep_for(30ms);
        REQUIRE(calls == 0);
        std::this_thread::sleep_for(40ms);
        REQUIRE(calls == 1);
        std::this_thread::sleep_for(120ms);
        REQUIRE(calls == 3);
    }
}

}
