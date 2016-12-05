cmake_minimum_required(VERSION 3.6)

set(PROJECT_NAME "DES")
set(APP_TARGET_NAME "${PROJECT_NAME}_app")
set(DES_LIB_TARGET_NAME "${PROJECT_NAME}_lib")
set(TESTS_TARGET_NAME "${PROJECT_NAME}_tests")

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)
add_definitions("-Wall")

#enable_testing()
#find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

set(APP_SOURCE_FILES main.cpp)
set(DES_LIB_SOURCE_FILES des.cpp)
set(TESTS_SOURCE_FILES tests.cpp)

add_library(${DES_LIB_TARGET_NAME} STATIC ${DES_LIB_SOURCE_FILES})

add_executable(${APP_TARGET_NAME} ${APP_SOURCE_FILES})
add_executable(${TESTS_TARGET_NAME} ${TESTS_SOURCE_FILES})

add_dependencies(${APP_TARGET_NAME} ${DES_LIB_TARGET_NAME})
target_link_libraries(${APP_TARGET_NAME} ${DES_LIB_TARGET_NAME})

add_dependencies(${TESTS_TARGET_NAME} ${DES_LIB_TARGET_NAME})
target_link_libraries(${TESTS_TARGET_NAME} ${DES_LIB_TARGET_NAME})

#add_dependencies(${TESTS_TARGET_NAME} ${GTEST_BOTH_LIBRARIES})
#target_link_libraries(${TESTS_TARGET_NAME} ${GTEST_BOTH_LIBRARIES})
