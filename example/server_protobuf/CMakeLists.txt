cmake_minimum_required(VERSION 2.8)

project(protobuf-msg)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(protos)
include_directories(${CMAKE_CURRENT_BINARY_DIR})


add_executable(${PROJECT_NAME} "main.cpp" "message_validator.cpp" "message_validator.h")
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} protos_msg hp_http_boost hp_http_cpp_rest)
