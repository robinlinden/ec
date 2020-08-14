#include "ec/core/active_object.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

namespace {

void free_function() {
    std::cout << "Free function" << '\n';
}

struct CallableStruct {
    void operator()() {
        std::cout << "Callable struct" << '\n';
    }
};

} // namespace

int main() {
    ec::core::ActiveObject ctx{};

    ctx.call([] { std::cout << "Lambda" << '\n'; });
    ctx.call(free_function);
    ctx.call(CallableStruct{});

    std::atomic<bool> done{false};
    ctx.call([&] {
        std::cout << "Done!" << '\n';
        done.store(true);
    });

    while (!done.load()) {
        using namespace std::chrono_literals;
        std::cout << "Waiting..." << '\n';
        std::this_thread::sleep_for(100ms);
    }
}
