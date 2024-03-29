cmake_minimum_required(VERSION 3.0)
project(hp_http_boost C CXX)
set(MODULE_NAME ${PROJECT_NAME})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -lpthread")

set(CMAKE_BUILD_TYPE Debug)
file(GLOB Srcs src/*.cc)
set(SOURCES ${SOURCES} ${Srcs} PARENT_SCOPE)

find_package(Boost REQUIRED COMPONENTS system thread regex filesystem)

add_library(${MODULE_NAME} ${Srcs})
target_link_libraries(${MODULE_NAME} PUBLIC ${Boost_LIBRARIES} path_parser crypto ssl)

include_directories(include/boost)

target_include_directories(${PROJECT_NAME}
	PUBLIC
	    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	    $<INSTALL_INTERFACE:include>
        PRIVATE
    )

# This makes the project importable from the build directory
export(TARGETS ${PROJECT_NAME} FILE "${PROJECT_NAME}.cmake")
