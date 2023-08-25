cmake_minimum_required(VERSION 3.26.3)
project(engine)

# compile in C++17
set(CMAKE_CXX_STANDARD 17)

# gtest library
add_subdirectory(googletest)
# run tests: ./tests
add_executable(tests tests/testBook.cpp orders/order.cpp orders/orderBook.cpp)
target_link_libraries(tests gtest_main)

# run main: ./engine
add_executable(${PROJECT_NAME} main.cpp orders/order.cpp orders/orderBook.cpp exchange.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE -g -Weffc++ -Wextra -Wconversion -Wsign-conversion)
