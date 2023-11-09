cmake_minimum_required(VERSION 3.16)
project(BuriedTest)

add_definitions(-D_WIN32_WINNT=0x0601)
include_directories(. ../src/third_party)

set(TEST_SRC
    test_crypt.cc
    test_common_service.cc
    test_http.cc
    test_executor.cc
    test_db.cc
    test.cc)

add_executable(buried_test ${TEST_SRC})
target_link_libraries(buried_test Buried_static gtest)
