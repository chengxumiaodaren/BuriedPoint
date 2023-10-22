cmake_minimum_required(VERSION 3.20)
project(BuriedTest)

include_directories(. ../src/third_party)

set(TEST_SRC
    test_http.cc
    test_db.cc
    test.cc)

add_executable(buried_test ${TEST_SRC})
target_link_libraries(buried_test Buried_static gtest)
