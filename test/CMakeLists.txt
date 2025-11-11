enable_testing()

add_executable(test_pool task_pool/mpmc_test.cpp task_pool/simple_pool_test.cpp)
add_executable(test_ptr smart_ptr/shared_ptr_test.cpp)
include(googletest)
target_link_libraries(
    test_pool
    GTest::gtest_main
)
target_link_libraries(
        test_ptr
        GTest::gtest_main
)
gtest_discover_tests(test_pool)
gtest_discover_tests(test_ptr)
