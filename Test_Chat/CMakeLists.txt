cmake_minimum_required(VERSION 3.10)
project(Test_Chat)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Test_Chat src/main.cpp src/chat.cpp src/chat.h)

target_include_directories(Test_Chat PRIVATE src)