//
// Created by Junchengji on 11/10/2025.
//

#ifndef COLORLOG_UNIQUE_PTR_H
#define COLORLOG_UNIQUE_PTR_H
#include <utility>

namespace cl {
    template<typename T>
    class unique_ptr {
    public:
        unique_ptr() = delete;
        unique_ptr(const unique_ptr<T> &) = delete;

        explicit unique_ptr(T *p) : ptr_(p) {}
        unique_ptr(unique_ptr<T> &&p)  noexcept {
            ptr_ = p.ptr_;
            p.ptr_ = nullptr;
        }

        unique_ptr& operator=(unique_ptr<T> &&p) noexcept {
            if (this != &p) {
                delete ptr_;
                ptr_ = p.ptr_;
                p.ptr_ = nullptr;
            }
            return *this;
        }

        ~unique_ptr() {
        delete ptr_;
        }

        // 放弃所有权但不释放内存
        T* release() noexcept {
            T* tmp = ptr_;
            ptr_ = nullptr;
            return tmp;
        }

        // 放弃所有权并释放内存
        void reset(T* ptr = nullptr) noexcept {
            if (ptr != ptr_) {
                delete ptr_;
                ptr_ = ptr;
            }
        }

        T* operator->() const noexcept { return ptr_; }
        T& operator*() const noexcept { return *ptr_; }
        T* get() const noexcept { return ptr_; }


    private:
        T *ptr_;
    };


    template<typename T, typename... Args>
    unique_ptr<T> make_unique(Args&& ...args) {
        return unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}
#endif //COLORLOG_UNIQUE_PTR_H