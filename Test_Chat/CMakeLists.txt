cmake_minimum_required(VERSION 3.10)
project(Test_Chat LANGUAGES CXX)

# Базовые настройки компилятора
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Флаги компиляции для GCC/Clang
add_compile_options(-Wall -Wextra -pedantic)

# Исполняемый файл 
add_executable(Test_Chat 
    src/main.cpp 
    src/chat.cpp
)

# Директории с заголовками
target_include_directories(Test_Chat PRIVATE src)

# Обязательные зависимости для Linux
find_package(Threads REQUIRED)
target_link_libraries(Test_Chat PRIVATE 
    Threads::Threads
)