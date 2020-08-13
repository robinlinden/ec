#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>

namespace ec {

class ActiveObject {
public:
    ~ActiveObject() {
        running_.store(false);
        cv_.notify_one();
        thread_.join();
    }

    void call(auto function) requires std::is_invocable_v<decltype(function)> {
        std::scoped_lock<std::mutex> lock{mutex_};
        queued_callables_.emplace(function);
        cv_.notify_one();
    }

private:
    void run() {
        while (running_.load()) {
            std::unique_lock<std::mutex> lock{mutex_};
            cv_.wait(lock, [&]() { return !queued_callables_.empty() || !running_.load(); });
            decltype(queued_callables_) to_call{};
            queued_callables_.swap(to_call);
            while (!to_call.empty()) {
                to_call.front()();
                to_call.pop();
            }
        }
    }

    std::atomic<bool> running_{true};
    std::queue<std::function<void(void)>> queued_callables_{};
    std::mutex mutex_{};
    std::condition_variable cv_{};
    std::thread thread_{&ActiveObject::run, this};
};

} // namespace ec

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
    ec::ActiveObject ctx{};

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
