include_guard(GLOBAL)

set(BURIED_SRCS
    buried.cc
    buried_core.cc)

set(MODULE_NAME "Buried")

message(STATUS "libs ${LIBS}")

if(BUILD_BURIED_SHARED_LIBS)
    add_library(${MODULE_NAME} SHARED ${BURIED_SRCS})
    target_link_libraries(${MODULE_NAME} PRIVATE ${LIBS})
endif()

if(BUILD_BURIED_STATIC_LIBS)
    add_library(${MODULE_NAME}_static STATIC ${BURIED_SRCS})
    target_link_libraries(${MODULE_NAME} PRIVATE ${LIBS})
endif()
