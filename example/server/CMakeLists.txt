cmake_minimum_required(VERSION 2.8)

project(boost_server)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} rest_boost)
