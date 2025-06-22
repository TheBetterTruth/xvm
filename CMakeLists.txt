cmake_minimum_required(VERSION 3.10)

project(xvm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(xvm "src/test.cpp" "src/function_manager.cpp" "src/vm_thread.cpp" "src/opcodes.cpp")