#ifndef COLORLOG_SHARED_PTR_H
#define COLORLOG_SHARED_PTR_H
#include <atomic>
#include "ctrl_block.h"

namespace cl {
    template<typename T>
    class shared_ptr {
        template<typename U>
        friend class weak_ptr;
    public:
        shared_ptr() {
            cb_ = new ControlBlock<T>(new T());
        }

        explicit shared_ptr(T *ptr) {
            if (ptr) cb_ = new ControlBlock<T>(ptr);
        }

        shared_ptr(const shared_ptr &other) noexcept : cb_(other.cb_) {
            if (cb_) cb_->shared_count.fetch_add(1);
        }

        shared_ptr(shared_ptr &&other) noexcept : cb_(other.cb_) {
            other->cb_=nullptr;
        }

        shared_ptr &operator=(const shared_ptr &other) {
            if (this != &other) {
                release();
                if (other.cb_) {
                    other.cb_->shared_count.fetch_add(1);
                }
            }
            return *this;
        }

        shared_ptr &operator=(shared_ptr &&other) noexcept {
            if (this != &other) {
                this->cb_ = other.cb_;
                other.cb_ = nullptr;
            }
            return *this;
        }

        T* get() const { return cb_->ptr; }
        T &operator*() { return *cb_->ptr; }
        T *operator->() { return cb_->ptr; }

    private:
        void release() const {
            if (cb_ && cb_->shared_count.fetch_sub(1) == 1) {
                delete cb_->ptr;
                cb_->ptr = nullptr;
                if (cb_->weak_count.fetch_sub(1) == 1) {
                    delete cb_;
                }
            }
        }
        ControlBlock<T> *cb_ = nullptr;

    };

    template<typename T, typename... Args>
    shared_ptr<T> make_shared(Args &&... args) {
        return shared_ptr<T>(new T(std::forward<Args>(args)...));
    }

}

#endif //COLORLOG_SHARED_PTR_H
