cmake_minimum_required(VERSION 3.16)
project(day33_robot_math LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(robot_math src/math.cpp)
target_include_directories(robot_math PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(robot_app app/main.cpp)
target_link_libraries(robot_app PRIVATE robot_math)

foreach(target robot_math robot_app)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -pedantic)
    endif()
endforeach()
