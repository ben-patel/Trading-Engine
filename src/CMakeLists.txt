cmake_minimum_required(VERSION 3.26.3) # as of 14.05.23
project(TE)

# compile in C++17
set(CMAKE_CXX_STANDARD 17)

# warning flags
set(CMAKE_CXX_FLAGS -Wall)
set(CMAKE_CXX_FLAGS -Weffc++)
set(CMAKE_CXX_FLAGS -Wextra)
set(CMAKE_CXX_FLAGS -Wconversion)
set(CMAKE_CXX_FLAGS -Wsign-conversion)

# ignore warning for 'delete' memory function replacement
set(CMAKE_CXX_FLAGS -Wno-implicit-exception-spec-mismatch)

add_executable(${PROJECT_NAME} main.cpp order.cpp orderBook.cpp)
