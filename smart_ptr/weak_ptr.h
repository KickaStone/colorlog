#ifndef COLORLOG_WEAK_PTR_H
#define COLORLOG_WEAK_PTR_H
#include "ctrl_block.h"
#include "shared_ptr.h"

namespace cl {
    template<typename T>
    class weak_ptr {
    public:
        weak_ptr() = default;
        explicit weak_ptr(const shared_ptr<T> &ptr) noexcept : cb_(ptr.cb_) {
            if (cb_) cb_->weak_count.fetch_add(1);
        }
        weak_ptr(const weak_ptr<T> &ptr) noexcept : cb_(ptr.cb_) {
            if (cb_) cb_->weak_count.fetch_add(1);
        }
        ~weak_ptr() {
            if (cb_ && cb_->weak_count.fetch_sub(1) == 1 && cb_->shared_count.load() == 1)
                delete cb_;
        }

        [[nodiscard]] bool expired() const noexcept {
            return !cb_ || cb_->shared_count == 0;
        }

        shared_ptr<T> lock() const noexcept {
            if (expired()) return shared_ptr<T>();
            cb_->shared_count.fetch_add(1);
            shared_ptr<T> sp;
            sp.cb_ = cb_;
            return sp;
        }
    private:
        ControlBlock<T> *cb_ = nullptr;
    };
}

#endif //COLORLOG_WEAK_PTR_H