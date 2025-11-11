#include "../../task_pool/simple_pool.h"

#include <gtest/gtest.h>

void foo(int i) {
    std::cout << i << std::endl;
}

TEST(taskpool, test1) {
    std::cout << "Test test1" << std::endl;
    cl::SimpleTaskPool pool(10);
    for (int i = 0; i < 100; i++) {
        const auto ok = pool.Enqueue(foo, i);
        if (!ok) {
            std::cout << "Enqueue failed" << std::endl;
            exit(-1);
        }
    }
}