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

add_executable(day02_example examples/day02/main.cpp)
target_link_libraries(day02_example PRIVATE course_options)

add_executable(day02_solution solutions/day02/sensor_snapshot.cpp)
target_link_libraries(day02_solution PRIVATE course_options)

add_executable(day03_example examples/day03/main.cpp)
target_link_libraries(day03_example PRIVATE course_options)

add_executable(day03_solution solutions/day03/sensor_filter.cpp)
target_link_libraries(day03_solution PRIVATE course_options)

add_executable(day04_example examples/day04/main.cpp)
target_link_libraries(day04_example PRIVATE course_options)

add_executable(day04_solution solutions/day04/mission_score.cpp)
target_link_libraries(day04_solution PRIVATE course_options)

add_executable(day05_example examples/day05/main.cpp)
target_link_libraries(day05_example PRIVATE course_options)

add_executable(day05_solution solutions/day05/task_id.cpp)
target_link_libraries(day05_solution PRIVATE course_options)

add_executable(day06_example examples/day06/main.cpp)
target_link_libraries(day06_example PRIVATE course_options)

add_executable(day06_solution solutions/day06/calibration.cpp)
target_link_libraries(day06_solution PRIVATE course_options)

enable_testing()

add_test(NAME day01_example_runs COMMAND day01_example)
set_tests_properties(day01_example_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "sensor status: 7"
)

add_test(NAME day01_solution_runs COMMAND day01_solution)
set_tests_properties(day01_solution_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "mission code: 17"
)

add_test(NAME day02_example_runs COMMAND day02_example)
set_tests_properties(day02_example_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "sample 1: 22.125 C"
)

add_test(NAME day02_solution_runs COMMAND day02_solution)
set_tests_properties(day02_solution_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "sample 1 position: 1.5 m"
)

add_test(NAME day03_example_runs COMMAND day03_example)
set_tests_properties(day03_example_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "accepted samples: 3"
)

add_test(NAME day03_solution_runs COMMAND day03_solution)
set_tests_properties(day03_solution_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "processed samples: 4"
)

add_test(NAME day04_example_runs COMMAND day04_example)
set_tests_properties(day04_example_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "sum to 4: 10"
)

add_test(NAME day04_solution_runs COMMAND day04_solution)
set_tests_properties(day04_solution_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "recursive sum: 6"
)

add_test(NAME day05_example_runs COMMAND day05_example)
set_tests_properties(day05_example_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "file limit: 80"
)

add_test(NAME day05_solution_runs COMMAND day05_solution)
set_tests_properties(day05_solution_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "501 502 503"
)

add_test(NAME day06_example_runs COMMAND day06_example)
set_tests_properties(day06_example_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "empty update: false"
)

add_test(NAME day06_solution_runs COMMAND day06_solution)
set_tests_properties(day06_solution_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "empty write: false"
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

add_test(
  NAME day02_narrowing_error_is_intentional
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/expect_failure.py"
    --compiler "${CMAKE_CXX_COMPILER}"
    --stage compile
    --source "${CMAKE_SOURCE_DIR}/exercises/day02/broken/narrowing.cpp"
)

add_test(
  NAME day02_const_assignment_error_is_intentional
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/expect_failure.py"
    --compiler "${CMAKE_CXX_COMPILER}"
    --stage compile
    --source "${CMAKE_SOURCE_DIR}/exercises/day02/broken/const_assignment.cpp"
)

add_test(
  NAME day03_duplicate_case_error_is_intentional
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/expect_failure.py"
    --compiler "${CMAKE_CXX_COMPILER}"
    --stage compile
    --source "${CMAKE_SOURCE_DIR}/exercises/day03/broken/duplicate_case.cpp"
)

add_test(
  NAME day04_ambiguous_overload_error_is_intentional
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/expect_failure.py"
    --compiler "${CMAKE_CXX_COMPILER}"
    --stage compile
    --source "${CMAKE_SOURCE_DIR}/exercises/day04/broken/ambiguous_overload.cpp"
)

add_test(
  NAME day05_missing_static_definition_error_is_intentional
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/expect_failure.py"
    --compiler "${CMAKE_CXX_COMPILER}"
    --stage link
    --source "${CMAKE_SOURCE_DIR}/exercises/day05/broken/missing_static_definition.cpp"
)

add_test(
  NAME day06_uninitialized_reference_error_is_intentional
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/expect_failure.py"
    --compiler "${CMAKE_CXX_COMPILER}"
    --stage compile
    --source "${CMAKE_SOURCE_DIR}/exercises/day06/broken/uninitialized_reference.cpp"
)
