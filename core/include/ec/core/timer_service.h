#ifndef EC_CORE_TIMER_SERVICE_H_
#define EC_CORE_TIMER_SERVICE_H_

#include "ec/core/active_object.h"

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace ec::core {

// TODO(robinlinden): Use a queue or thread pool or something instead
//  throwing threads around like this.
class TimerService {
public:
    explicit TimerService(ActiveObject *ctx) : ctx_{ctx} {}
    ~TimerService() {
        running_.store(false);
        cv_.notify_all();
        for (auto &thread : threads_) { thread.join(); }
    }

    void call_after(std::chrono::milliseconds timeout, auto function) {
        threads_.push_back(std::thread([=, this] {
            std::unique_lock<std::mutex> lock{mutex_};
            cv_.wait_for(lock, timeout);

            if (running_.load()) {
                ctx_->call(function);
            }
        }));
    }

    void call_every(std::chrono::milliseconds timeout, auto function) {
        threads_.push_back(std::thread([=, this] {
            while (running_.load()) {
                std::unique_lock<std::mutex> lock{mutex_};
                cv_.wait_for(lock, timeout);

                if (running_.load()) {
                    ctx_->call(function);
                }
            }
        }));
    }

private:
    ActiveObject *const ctx_;
    std::atomic<bool> running_{true};
    std::condition_variable cv_{};
    std::mutex mutex_{};
    std::vector<std::thread> threads_{};
};

} // namespace ec::core

#endif // EC_CORE_TIMER_SERVICE_H_
