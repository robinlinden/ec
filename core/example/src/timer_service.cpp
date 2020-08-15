#include "ec/core/timer_service.h"

#include "ec/core/active_object.h"

#include <chrono>
#include <thread>

int main() {
    using namespace std::chrono_literals;

    ec::core::ActiveObject ctx{};
    ec::core::TimerService timer{&ctx};

    const auto start = std::chrono::system_clock::now();

    timer.call_every(50ms, [&] {
        const auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "Called after " << elapsed_seconds.count() << "s." << '\n';
    });

    std::atomic<bool> done{false};

    timer.call_after(400ms, [&] {
        const auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "Done after " << elapsed_seconds.count() << "s." << '\n';
        done.store(true);
    });

    while (!done.load()) {
        std::cout << "Waiting..." << '\n';
        std::this_thread::sleep_for(100ms);
    }
}
