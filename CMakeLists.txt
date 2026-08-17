cmake_minimum_required(VERSION 3.16)

project(cpp_learning_journey LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CPP_LEARNING_ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(CPP_LEARNING_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)

add_library(course_options INTERFACE)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(course_options INTERFACE -Wall -Wextra -Wpedantic)
  if(CPP_LEARNING_WARNINGS_AS_ERRORS)
    target_compile_options(course_options INTERFACE -Werror)
  endif()

  if(CPP_LEARNING_ENABLE_SANITIZERS)
    target_compile_options(course_options INTERFACE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(course_options INTERFACE -fsanitize=address,undefined -fno-omit-frame-pointer)
  endif()
elseif(MSVC)
  target_compile_options(course_options INTERFACE /W4 /permissive-)
  if(CPP_LEARNING_WARNINGS_AS_ERRORS)
    target_compile_options(course_options INTERFACE /WX)
  endif()
endif()

add_executable(day01_example
  examples/day01/main.cpp
  examples/day01/status.cpp
)
target_include_directories(day01_example PRIVATE examples/day01)
target_link_libraries(day01_example PRIVATE course_options)

add_executable(day01_solution solutions/day01/mission.cpp)
target_link_libraries(day01_solution PRIVATE course_options)

enable_testing()

add_test(NAME day01_example_runs COMMAND day01_example)
set_tests_properties(day01_example_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "sensor status: 7"
)

add_test(NAME day01_solution_runs COMMAND day01_solution)
set_tests_properties(day01_solution_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "mission code: 17"
)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

add_test(
  NAME day01_compile_error_is_intentional
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/expect_failure.py"
    --compiler "${CMAKE_CXX_COMPILER}"
    --stage compile
    --source "${CMAKE_SOURCE_DIR}/exercises/day01/broken/syntax_error.cpp"
)

add_test(
  NAME day01_link_error_is_intentional
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/expect_failure.py"
    --compiler "${CMAKE_CXX_COMPILER}"
    --stage link
    --source "${CMAKE_SOURCE_DIR}/exercises/day01/broken/missing_definition_main.cpp"
)
