cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME "DES")
set(APP_TARGET_NAME "${PROJECT_NAME}_app")
set(DES_LIB_TARGET_NAME "${PROJECT_NAME}_lib")

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)
add_definitions("-Wall")

include_directories(include/)

set(APP_SOURCE_FILES src/main.cpp)
set(DES_LIB_SOURCE_FILES lib/des.cpp)

add_library(${DES_LIB_TARGET_NAME} STATIC ${DES_LIB_SOURCE_FILES})

add_executable(${APP_TARGET_NAME} ${APP_SOURCE_FILES})

add_dependencies(${APP_TARGET_NAME} ${DES_LIB_TARGET_NAME})
target_link_libraries(${APP_TARGET_NAME} ${DES_LIB_TARGET_NAME})

install(TARGETS ${APP_TARGET_NAME} ${DES_LIB_TARGET_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
