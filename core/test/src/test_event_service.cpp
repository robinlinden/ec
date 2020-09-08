#include "ec/core/event_service.h"

#include <catch2/catch.hpp>

#include <thread>

using namespace ec::core;
using namespace std::chrono_literals;

namespace {

struct Event {
    std::string msg{};
};

TEST_CASE("event_service") {
    ActiveObject ctx{};
    EventService es{&ctx};

    SECTION("subscribe") {
        bool got_event = false;
        auto s = es.subscribe<Event>([&](const Event &e) {
            if (e.msg == "amazing") {
                got_event = true;
            }
        });

        es.dispatch(Event{"amazing"});
        std::this_thread::sleep_for(5ms);

        REQUIRE(got_event);
        es.unsubscribe(s);
    }

    SECTION("unsubscribe") {
        bool got_event = false;
        auto s = es.subscribe<Event>([&](const Event &e) {
            if (e.msg == "amazing") {
                got_event = true;
            }
        });
        es.unsubscribe(s);

        es.dispatch(Event{"amazing"});
        std::this_thread::sleep_for(5ms);

        REQUIRE_FALSE(got_event);
    }
}
} // namespace
