cmake_minimum_required (VERSION 2.6)

project(DES CXX)

SET(
    app_sources
    main.cpp
)

SET(
	lib_sources
	des.cpp
)

add_definitions( -std=c++11 )

add_library(
	des
	STATIC
	${lib_sources}
)

add_executable(
    des_app
    ${app_sources}
)

target_link_libraries(
	des_app
	des
)

add_dependencies(
	des_app
	des
)
