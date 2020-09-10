#ifndef EC_CORE_TIMER_SERVICE_H_
#define EC_CORE_TIMER_SERVICE_H_

#include "ec/core/active_object.h"

#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <queue>

namespace ec::core {

class TimerService {
public:
    explicit TimerService(ActiveObject *ctx) : ctx_{ctx} {}

    ~TimerService() {
        running_.store(false);
        cv_.notify_one();
        thread_.join();
    }

    template<typename T>
    void call_after(std::chrono::milliseconds timeout, T function) {
        std::unique_lock<std::mutex> lock{mutex_};
        jobs_.push(TimerJob{clock::now() + timeout, function});
        cv_.notify_one();
    }

    template<typename T>
    void call_every(std::chrono::milliseconds timeout, T function) {
        std::unique_lock<std::mutex> lock{mutex_};
        jobs_.push(TimerJob{clock::now() + timeout, function, timeout});
        cv_.notify_one();
    }

private:
    using clock = std::chrono::steady_clock;

    struct TimerJob {
        std::chrono::time_point<std::chrono::steady_clock> deadline;
        std::function<void()> function;
        std::chrono::milliseconds period{0};

        bool operator>(const TimerJob &other) const {
            return deadline > other.deadline;
        }
    };

    void run() {
        while(running_.load()) {
            std::unique_lock<std::mutex> lock{mutex_};
            if (jobs_.empty()) {
                cv_.wait(lock, [&] { return !jobs_.empty() || !running_.load(); });
                continue;
            }

            cv_.wait_until(lock, jobs_.top().deadline, [&] {
                return jobs_.top().deadline < clock::now() || !running_.load();
            });

            if (running_.load()) {
                auto job = jobs_.top();
                ctx_->call(job.function);
                jobs_.pop();
                if (job.period.count() != 0) {
                    job.deadline = clock::now() + job.period;
                    jobs_.push(job);
                }
            }
        }
    }

    ActiveObject *const ctx_;
    std::atomic<bool> running_{true};
    std::condition_variable cv_{};
    std::mutex mutex_{};
    std::priority_queue<TimerJob, std::vector<TimerJob>, std::greater<>> jobs_{};
    std::thread thread_{&TimerService::run, this};
};

} // namespace ec::core

#endif // EC_CORE_TIMER_SERVICE_H_
