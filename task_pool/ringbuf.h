//
// Created by Junchengji on 11/10/2025.
//

#ifndef COLORLOG_RINGBUF_H
#define COLORLOG_RINGBUF_H

#include <array>
#include <atomic>

/**
 * @tparam T type
 * @tparam N capacity
 */
template<typename T, size_t N>
class SpscQueue {
public:
    bool push(const T &value) noexcept {
        // full (head+1) % N == tail
        const auto next = (head_ + 1) % N;
        if (next == tail_.load(std::memory_order_acquire)) {
            return false; // full
        }
        buffer_[head_] = value;
        head_.store(next, std::memory_order_release);
        return true;
    }

    bool pop(T &value) noexcept {
        // empty : head == tail
        auto tail = tail_.load(std::memory_order_relaxed);
        if (tail == head_.load(std::memory_order_acquire)) {
            return false; // empty
        }
        value = buffer_[tail];
        tail_.store((tail + 1) % N, std::memory_order_release);
        return true;
    }

private:
    std::array<T, N> buffer_{};
    // head_: -> the empty slot before the first element
    std::atomic<size_t> head_{0};
    // tail_: -> the last element
    std::atomic<size_t> tail_{0};
};


#endif //COLORLOG_RINGBUF_H
