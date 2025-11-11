#ifndef COLORLOG_CTRL_BLOCK_H
#define COLORLOG_CTRL_BLOCK_H
#include <atomic>

namespace cl {
    template <typename T>
    class ControlBlock {
    public:
        std::atomic<size_t> shared_count{0};
        std::atomic<size_t> weak_count{0};
        T *ptr;
        explicit ControlBlock(T *p):ptr(p) {}
        ~ControlBlock() {
            delete ptr;
        }
    };
}

#endif //COLORLOG_CTRL_BLOCK_H