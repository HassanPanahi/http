cmake_minimum_required(VERSION 3.0)
project(protos_msg VERSION 1.0 LANGUAGES CXX)
find_package(Protobuf)

set(MODULE_NAME ${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Threads)
file(GLOB PROTO_FILES *.proto)

set(PROTOS_SRC "")
set(PPTH --proto_path=${CMAKE_CURRENT_SOURCE_DIR})
foreach(PROTO_FILE ${PROTO_FILES})
    get_filename_component(ABSOLUTE_NAME ${PROTO_FILE} ABSOLUTE)
    get_filename_component(FILE_NAME ${PROTO_FILE} NAME_WE)
    list(APPEND PROTOS_SRC "${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.pb.cc")
    list(APPEND PROTOS_HDRS "${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.pb.h")
    protobuf_generate_cpp(TARGET ${MODULE_NAME} PROTOS ${FILE_NAME}.proto)
endforeach(PROTO_FILE ${PROTO_FILES})

file(GLOB SRCS *.cc)

add_library(${MODULE_NAME} ${PROTOS_SRC} ${SRCS} ${PROTO_FILES})
target_link_libraries(${MODULE_NAME} ${PROTOBUF_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})


target_include_directories(${MODULE_NAME}
	PUBLIC
	    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..>
	    $<INSTALL_INTERFACE:include>
    )

# This makes the project importable from the build directory
export(TARGETS ${MODULE_NAME} FILE "${MODULE_NAME}.cmake")
