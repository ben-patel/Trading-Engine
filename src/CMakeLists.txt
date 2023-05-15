cmake_minimum_required(VERSION 3.26.3) # latest
project(TE)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp order.cpp orderBook.cpp)
