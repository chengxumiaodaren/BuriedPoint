cmake_minimum_required(VERSION 3.20)
project(BuriedTest)

set(TEST_SRC
    test_db.cc
    test.cc)

add_executable(buried_test ${TEST_SRC})
target_link_libraries(buried_test Buried_static gtest)
