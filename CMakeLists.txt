cmake_minimum_required(VERSION 3.25)
project(boost_test)

set(CMAKE_CXX_STANDARD 17)

add_executable(server_target server_side.cpp)
target_include_directories(server_target PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(client_target client_side.cpp)
target_include_directories(client_target PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
