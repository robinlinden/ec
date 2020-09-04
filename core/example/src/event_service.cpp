#include "ec/core/event_service.h"

#include <chrono>
#include <iostream>
#include <thread>

namespace {
struct Event1 { int a; };
struct Event2 { int b; };
struct Event3 { int c; int d; };

void handle_event(const Event1 &ev) {
    std::cout << "Got event 1 with data " << ev.a << " on thread " << std::this_thread::get_id() << '\n';
}

class EventHandler {
public:
    explicit EventHandler(ec::core::EventService *srv) :
            srv_{srv},
            sub_{srv_->subscribe<Event3>(this, &EventHandler::handle)} {
    }

    ~EventHandler() {
        srv_->unsubscribe(sub_);
    }

    void handle(const Event3 &ev) const {
        std::cout << "Got event 3 with data " << ev.c << ", " << ev.d << " on thread " << std::this_thread::get_id()
                  << '\n';
    }

private:
    ec::core::EventService *const srv_;
    const ec::core::Subscription sub_;
};
} // namespace

int main() {
    using namespace std::chrono_literals;

    std::cout << "Main thread is " << std::this_thread::get_id() << '\n';
    ec::core::ActiveObject ctx{};
    ctx.call([] { std::cout << "Event thread is " << std::this_thread::get_id() << '\n'; });
    ec::core::EventService srv{&ctx};

    // Lonely functions work.
    const auto s1 = srv.subscribe<Event1>(&handle_event);
    srv.dispatch(Event1{42});
    srv.unsubscribe(s1);
    srv.dispatch(Event1{43});

    // As do lambdas.
    auto s2 = srv.subscribe<Event2>([](const auto &ev) {
        std::cout << "Got event 2 with data " << ev.b << " on thread " << std::this_thread::get_id() << '\n';
    });
    srv.dispatch(Event2{1984});
    srv.unsubscribe(s2);
    srv.dispatch(Event2{1992});

    // And class methods too.
    {
        EventHandler eh{&srv};
        srv.dispatch(Event3{9000, 9001});
    }
    srv.dispatch(Event3{9004, 9006});

    // Yield main thread to let the event service run.
    std::this_thread::sleep_for(5ms);
}
