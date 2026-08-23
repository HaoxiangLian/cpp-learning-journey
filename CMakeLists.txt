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

add_executable(day07_example examples/day07/main.cpp)
target_link_libraries(day07_example PRIVATE course_options)

add_executable(day07_solution solutions/day07/sample_buffer.cpp)
target_link_libraries(day07_solution PRIVATE course_options)

add_executable(day08_example examples/day08/main.cpp)
target_link_libraries(day08_example PRIVATE course_options)

add_executable(day08_solution solutions/day08/battery_level.cpp)
target_link_libraries(day08_solution PRIVATE course_options)

add_executable(day09_example examples/day09/main.cpp)
target_link_libraries(day09_example PRIVATE course_options)

add_executable(day09_solution solutions/day09/task_session.cpp)
target_link_libraries(day09_solution PRIVATE course_options)

add_executable(day10_example examples/day10/main.cpp)
target_link_libraries(day10_example PRIVATE course_options)

add_executable(day10_solution solutions/day10/owned_samples.cpp)
target_link_libraries(day10_solution PRIVATE course_options)

add_executable(day11_example examples/day11/main.cpp)
target_link_libraries(day11_example PRIVATE course_options)

add_executable(day11_solution solutions/day11/move_only_batch.cpp)
target_link_libraries(day11_solution PRIVATE course_options)

add_executable(day12_example examples/day12/main.cpp)
target_link_libraries(day12_example PRIVATE course_options)

add_executable(day12_solution solutions/day12/sensor_hierarchy.cpp)
target_link_libraries(day12_solution PRIVATE course_options)

add_executable(day13_example examples/day13/main.cpp)
target_link_libraries(day13_example PRIVATE course_options)

add_executable(day13_solution solutions/day13/polymorphic_sensors.cpp)
target_link_libraries(day13_solution PRIVATE course_options)

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

add_test(NAME day07_example_runs COMMAND day07_example)
set_tests_properties(day07_example_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "released pointer reset: true"
)

add_test(NAME day07_solution_runs COMMAND day07_solution)
set_tests_properties(day07_solution_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "released: true"
)

add_test(NAME day08_example_runs COMMAND day08_example)
set_tests_properties(day08_example_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "invariant holds: true"
)

add_test(NAME day08_solution_runs COMMAND day08_solution)
set_tests_properties(day08_solution_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "battery: 25"
)

add_test(NAME day09_example_runs COMMAND day09_example)
set_tests_properties(day09_example_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "destroy sensor"
)

add_test(NAME day09_solution_runs COMMAND day09_solution)
set_tests_properties(day09_solution_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "session 42 closed"
)

add_test(NAME day10_example_runs COMMAND day10_example)
set_tests_properties(day10_example_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "assigned\\[1\\]: 77"
)

add_test(NAME day10_solution_runs COMMAND day10_solution)
set_tests_properties(day10_solution_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "assigned: 5 100"
)

add_test(NAME day11_example_runs COMMAND day11_example)
set_tests_properties(day11_example_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "target empty: true"
)

add_test(NAME day11_solution_runs COMMAND day11_solution)
set_tests_properties(day11_solution_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "moved: 4 9"
)

add_test(NAME day12_example_runs COMMAND day12_example)
set_tests_properties(day12_example_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "destroy scan task"
)

add_test(NAME day12_solution_runs COMMAND day12_solution)
set_tests_properties(day12_solution_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "robot lidar: 42 32"
)

add_test(NAME day13_example_runs COMMAND day13_example)
set_tests_properties(day13_example_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "virtual kind: lidar"
)

add_test(NAME day13_solution_runs COMMAND day13_solution)
set_tests_properties(day13_solution_runs PROPERTIES
  PASS_REGULAR_EXPRESSION "force: 12.5"
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

add_test(
  NAME day07_negative_array_bound_error_is_intentional
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/expect_failure.py"
    --compiler "${CMAKE_CXX_COMPILER}"
    --stage compile
    --source "${CMAKE_SOURCE_DIR}/exercises/day07/broken/negative_array_bound.cpp"
)

add_test(
  NAME day08_private_access_error_is_intentional
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/expect_failure.py"
    --compiler "${CMAKE_CXX_COMPILER}"
    --stage compile
    --source "${CMAKE_SOURCE_DIR}/exercises/day08/broken/private_access.cpp"
)

add_test(
  NAME day09_implicit_conversion_error_is_intentional
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/expect_failure.py"
    --compiler "${CMAKE_CXX_COMPILER}"
    --stage compile
    --source "${CMAKE_SOURCE_DIR}/exercises/day09/broken/implicit_conversion.cpp"
)

add_test(
  NAME day10_copy_disabled_error_is_intentional
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/expect_failure.py"
    --compiler "${CMAKE_CXX_COMPILER}"
    --stage compile
    --source "${CMAKE_SOURCE_DIR}/exercises/day10/broken/copy_disabled.cpp"
)

add_test(
  NAME day11_copy_move_only_error_is_intentional
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/expect_failure.py"
    --compiler "${CMAKE_CXX_COMPILER}"
    --stage compile
    --source "${CMAKE_SOURCE_DIR}/exercises/day11/broken/copy_move_only.cpp"
)

add_test(
  NAME day12_private_inheritance_conversion_error_is_intentional
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/expect_failure.py"
    --compiler "${CMAKE_CXX_COMPILER}"
    --stage compile
    --source "${CMAKE_SOURCE_DIR}/exercises/day12/broken/private_inheritance_conversion.cpp"
)

add_test(
  NAME day13_mismatched_override_error_is_intentional
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/expect_failure.py"
    --compiler "${CMAKE_CXX_COMPILER}"
    --stage compile
    --source "${CMAKE_SOURCE_DIR}/exercises/day13/broken/mismatched_override.cpp"
)

add_test(
  NAME day13_abstract_instantiation_error_is_intentional
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/expect_failure.py"
    --compiler "${CMAKE_CXX_COMPILER}"
    --stage compile
    --source "${CMAKE_SOURCE_DIR}/exercises/day13/broken/instantiate_abstract.cpp"
)
