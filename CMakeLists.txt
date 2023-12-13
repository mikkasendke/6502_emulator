cmake_minimum_required(VERSION 3.12)
project(6502_emulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(6502_emulator ${SOURCES})

