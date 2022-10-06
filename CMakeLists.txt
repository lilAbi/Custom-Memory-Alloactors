cmake_minimum_required(VERSION 3.23)
project(memoryAllocator)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(memoryAllocator ${SRC_FILES})
