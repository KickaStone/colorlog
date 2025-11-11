//
// Created by Junchengji on 11/10/2025.
//

#ifndef COLORLOG_MPMC_H
#define COLORLOG_MPMC_H

#include <atomic>
#include <stdexcept>
namespace cl {
    template<typename T>
    class MPMCQueue {
        struct Node {
            std::atomic<Node *> next;
            T value;
            explicit Node(T v) : next(nullptr), value(std::move(v)) {
            }
        };

        std::atomic<Node *> head{};
        std::atomic<Node *> tail{};

    public:
        MPMCQueue() {
            Node *dummy = new Node(T{});
            head.store(dummy);
            tail.store(dummy);
        }

        void enqueue(T v) {
            Node *node = new Node(std::move(v));
            Node *oldTail;
            while (true) {
                oldTail = tail.load();
                Node *next = oldTail->next.load();
                if (next == nullptr) {
                    if (oldTail->next.compare_exchange_weak(next, node)) break;
                } else {
                    tail.compare_exchange_weak(oldTail, next);
                }
            }
            tail.compare_exchange_weak(oldTail, node);
        }

        bool dequeue(T &result) {
            while (true) {
                Node *oldHead = head.load();
                Node *next = oldHead->next.load();
                if (next == nullptr) return false; // empty
                if (head.compare_exchange_weak(oldHead, next)) {
                    result = std::move(next->value);
                    delete oldHead;
                    return true;
                }
            }
        }
    };
}

#endif //COLORLOG_MPMC_H
