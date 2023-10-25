cmake_minimum_required(VERSION 3.20)
project(BuriedExample)

add_definitions(-D_WIN32_WINNT=0x0601)
include_directories(. .. ../src ../src/third_party ../src/third_party/spdlog/include ../src/third_party/boost)

add_executable(buried_example buried_example.cc)
target_link_libraries(buried_example Buried_static)

add_executable(buried_example_shared buried_example.cc)
target_link_libraries(buried_example_shared Buried_shared)

add_executable(context_example context_example.cc)
target_link_libraries(context_example Buried_static)

add_executable(reporter_example reporter_example.cc)
target_link_libraries(reporter_example Buried_static)
