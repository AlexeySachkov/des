cmake_minimum_required (VERSION 2.6)

project(DES CXX)


SET(
    sources
    main.cpp
    ${headers}
)

add_definitions( -std=c++11 )

add_executable(
    des
    ${sources}
)
