cmake_minimum_required(VERSION 3.15)
project(eduCpp)


find_package(fmt CONFIG REQUIRED)


# Добавьте исполняемый файл и свяжите ег``о с библиотекой fmt
add_executable(eduCpp main.cpp)
target_link_libraries(eduCpp PRIVATE fmt::fmt)