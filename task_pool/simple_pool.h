//
// Created by Junchengji on 11/11/2025.
//

#ifndef COLORLOG_SIMPLE_POOL_H
#define COLORLOG_SIMPLE_POOL_H
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>


namespace cl {
    using Task = std::function<void()>;

    class SimpleTaskPool {
    public:
        explicit SimpleTaskPool(const size_t n = std::thread::hardware_concurrency()) : workers_(n) , stop_(false) {
            for (size_t i = 0; i < workers_; i++) {
                threads_.emplace_back([this]() {
                    while (true) {
                        Task task;
                        std::unique_lock<std::mutex> lock(mutex_);
                        cv_.wait(lock, [this](){return stop_.load() || !tasks_.empty();});
                        if (stop_) break;
                        if (!tasks_.empty()) {
                            task = std::move(tasks_.front());
                            tasks_.pop();
                        }
                        if (task) {
                            task();
                        }
                    }
                });
            }
        };
        ~SimpleTaskPool() noexcept {
            stop_.store(true);
            cv_.notify_all();
            for (auto& thread : threads_) {
                thread.join();
            }
        };

        SimpleTaskPool(const SimpleTaskPool&) = delete;
        SimpleTaskPool& operator=(const SimpleTaskPool&) = delete;

        SimpleTaskPool(SimpleTaskPool&&) = delete;
        SimpleTaskPool& operator=(SimpleTaskPool&&) = delete;

        template <typename F, typename... Args>
        [[nodiscard]] bool Enqueue(F&& f, Args&&... args) {
            if (stop_.load()) { return false; }
            {
                std::lock_guard<std::mutex> lock(mutex_);
                tasks_.emplace([func = std::forward<F>(f), ...args = std::forward<Args>(args)]() mutable {
                    func(std::forward<Args>(args)...);
                });
            }
            cv_.notify_one();
            return true;
        }

    private:
        size_t workers_;
        std::vector< std::thread> threads_;
        std::queue<Task> tasks_;
        std::mutex mutex_;
        std::condition_variable cv_;
        std::atomic<bool> stop_;
    };
}

#endif //COLORLOG_SIMPLE_POOL_H