//
// Created by Junchengji on 11/11/2025.
//
#include "../../smart_ptr/shared_ptr.h"

#include <gtest/gtest.h>

#include "../../smart_ptr/weak_ptr.h"

TEST(SMT_PTR, test1) {
    auto t = cl::shared_ptr<int>(new int(10));
    std::cout << *t << std::endl;
}

TEST(SMT_PTR, test2) {
    auto ptr = cl::weak_ptr<int>(new int(20));
}