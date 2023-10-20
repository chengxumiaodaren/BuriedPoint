include_guard(GLOBAL)

include_directories(. third_party/spdlog/include)

add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
add_subdirectory(third_party/spdlog)

set(DB_SRCS third_party/sqlite/sqlite3.c database/database.cc)

set(BURIED_SRCS
    ${DB_SRCS}
    buried.cc
    buried_core.cc)

set(MODULE_NAME "Buried")

message(STATUS "libs ${LIBS}")

if(BUILD_BURIED_STATIC_LIBS)
    add_library(${MODULE_NAME}_static STATIC ${BURIED_SRCS})
    target_link_libraries(${MODULE_NAME}_static PRIVATE ${LIBS} spdlog)
endif()

if(BUILD_BURIED_SHARED_LIBS)
    add_library(${MODULE_NAME}_shared SHARED ${BURIED_SRCS})
    target_link_libraries(${MODULE_NAME}_shared PRIVATE ${LIBS} spdlog)
endif()
