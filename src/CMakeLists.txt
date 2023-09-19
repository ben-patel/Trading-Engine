cmake_minimum_required(VERSION 3.26.3)
project(engine)

# compile in C++17
set(CMAKE_CXX_STANDARD 17)

# # gtest library
# add_subdirectory(tests/googletest)
# # run tests: ./tests
# add_executable(tests trader.cpp misc/util.cpp tests/testBook.cpp orders/order.cpp orders/orderBook.cpp)
# target_link_libraries(tests gtest_main)

# run main: ./engine
add_executable(${PROJECT_NAME} main.cpp exchange/trader.cpp server/server.cpp misc/util.cpp orders/order.cpp orders/orderBook.cpp exchange/exchange.cpp misc/exchangeFactory.cpp)
find_package(Crow)
target_link_libraries(${PROJECT_NAME} PUBLIC Crow::Crow)
target_compile_options(${PROJECT_NAME} PRIVATE -g -Wall)
