cmake_minimum_required(VERSION 2.8)

project(example)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME}_boost_server_client "boost_server_client.cpp")
target_link_libraries(${PROJECT_NAME}_boost_server_client ${Boost_LIBRARIES} hp_http_boost)

set(CMAKE_BUILD_TYPE Debug)
add_executable(${PROJECT_NAME}_cpp_rest_server_clinet "cpp_rest_server_client.cpp")
target_link_libraries(${PROJECT_NAME}_cpp_rest_server_clinet hp_http_cpp_rest)
