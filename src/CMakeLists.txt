include_guard(GLOBAL)

include_directories(. third_party third_party/spdlog/include third_party/boost third_party/mbedtls/include)

add_definitions(-D_WIN32_WINNT=0x0601)
add_definitions(-DBOOST_JSON_NO_LIB)
add_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
add_subdirectory(third_party/spdlog)
add_subdirectory(third_party/mbedtls/library)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/buried_config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/buried_config.h)

set(DB_SRCS third_party/sqlite/sqlite3.c database/database.cc)

set(BURIED_SRCS
    ${DB_SRCS}
    crypt/crypt.cc
    report/buried_report.cc
    report/http_report.cc
    common/common_service.cc
    context/context.cc
    buried.cc
    buried_core.cc
)

set(MODULE_NAME "Buried")

message(STATUS "libs ${LIBS}")

if(BUILD_BURIED_STATIC_LIBS)
    add_library(${MODULE_NAME}_static STATIC ${BURIED_SRCS})
    target_link_libraries(${MODULE_NAME}_static PRIVATE ${LIBS} spdlog mbedtls_static)
endif()

if(BUILD_BURIED_SHARED_LIBS)
    add_library(${MODULE_NAME}_shared SHARED ${BURIED_SRCS})
    target_link_libraries(${MODULE_NAME}_shared PRIVATE ${LIBS} spdlog mbedtls_static)
endif()
