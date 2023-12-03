cmake_minimum_required(VERSION 2.8)

project(http)
find_package(Boost REQUIRED COMPONENTS system thread regex filesystem)

#add_subdirectory(rest)

add_executable(${PROJECT_NAME} "main.cpp" "beasthttp.h" "beasthttp.cpp" "server.cc" "server.h" "path_address.h" "path_address.cc" "path_parser.cc" "path_parser.h")
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
