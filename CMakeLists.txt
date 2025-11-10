cmake_minimum_required(VERSION 4.0)
project(colorlog)

set(CMAKE_CXX_STANDARD 20)

add_executable(colorlog main.cpp
        smart_ptr/shared_ptr.h
        smart_ptr/unique_ptr.h
        all.h)
