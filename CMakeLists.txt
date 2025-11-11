cmake_minimum_required(VERSION 4.0)
project(colorlog)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(colorlog
        all.h
        smart_ptr/shared_ptr.h
        smart_ptr/unique_ptr.h
        task_pool/mpmc.h
        task_pool/ringbuf.h
        task_pool/simple_pool.h
        task_pool/simple_pool.cpp
        smart_ptr/ctrl_block.h
        smart_ptr/weak_ptr.h
)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/52eb8108c5bdec04579160ae17225d66034bd723.zip
)
# 对于 Windows 系统：防止覆盖父项目的编译器/链接器设置
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# 使得 GoogleTest 可用
FetchContent_MakeAvailable(googletest)
add_subdirectory(test)