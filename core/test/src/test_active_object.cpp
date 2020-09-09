#include "ec/core/active_object.h"

#include <catch2/catch.hpp>

#include <chrono>
#include <thread>

using namespace ec::core;
using namespace std::chrono_literals;

namespace {

TEST_CASE("active_object") {
    ActiveObject ctx{};

    SECTION("calls from its own thread") {
        const auto main_thread = std::this_thread::get_id();
        auto ctx_thread = main_thread;
        ctx.call([&] {
            ctx_thread = std::this_thread::get_id();
        });

        std::this_thread::sleep_for(5ms);
        REQUIRE(ctx_thread != main_thread);
    }

    SECTION("makes calls in the right order") {
        int i = 0;
        ctx.call([&] { i += 1; });
        ctx.call([&] { i *= 8; });
        std::this_thread::sleep_for(5ms);
        REQUIRE(i == 8);

        i = 0;
        ctx.call([&] { i *= 8; });
        ctx.call([&] { i += 1; });
        std::this_thread::sleep_for(5ms);
        REQUIRE(i == 1);
    }
}

}
