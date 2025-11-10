//
// Created by Junchengji on 11/10/2025.
//

#ifndef COLORLOG_SHARED_PTR_H
#define COLORLOG_SHARED_PTR_H
#include <atomic>

namespace cl {
    template<typename T>
    class shared_ptr {
    public:
        shared_ptr() : ptr_(new T()), ref_count_(new std::atomic<size_t>(1)) {
        }

        explicit shared_ptr(T *ptr) : ptr_(ptr), ref_count_(new std::atomic<size_t>(1)) {
        }

        shared_ptr(const shared_ptr &other) : ptr_(other.ptr_), ref_count_(other.ref_count_) {
            ref_count_->fetch_add(1);
        }

        shared_ptr &operator=(const shared_ptr &other) {
            if (this != &other) {
                release();
                this->ptr_ = other.ptr_;
                this->ref_count_ = other.ref_count_;
            }
            return *this;
        }

        T *get() const { return ptr_; }
        T &operator*() { return *ptr_; }
        T *operator->() { return ptr_; }

    private:
        void release() const {
            if (ref_count_ && ref_count_->fetch_sub(1) == 1) {
                delete ref_count_;
                delete ptr_;
            }
        }

        T *ptr_;
        std::atomic<size_t> *ref_count_;
    };

    template<typename T, typename... Args>
    shared_ptr<T> make_shared(Args &&... args) {
        return shared_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

#endif //COLORLOG_SHARED_PTR_H
