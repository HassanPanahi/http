cmake_minimum_required(VERSION 2.8)

project(http)

add_subdirectory(protos)
find_package(Boost REQUIRED COMPONENTS system thread regex filesystem)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} "main.cpp" "http_connection.cpp" "http_connection.h" "server.cc" "server.h" "path_address.h" "path_address.cc" "path_parser.cc" "path_parser.h")
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} protos_msg)
