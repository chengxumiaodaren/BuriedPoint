cmake_minimum_required(VERSION 3.20)
project(BuriedExample)

add_executable(buried_example buried_example.cc)
target_link_libraries(buried_example Buried_static)

add_executable(buried_example_shared buried_example.cc)
target_link_libraries(buried_example_shared Buried_shared)
