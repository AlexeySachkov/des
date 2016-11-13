cmake_minimum_required (VERSION 2.6)

project(DES CXX)

SET(
	headers
	des.hpp
)

SET(
	sources
	main.cpp
	des.cpp
	${headers}
)

add_definitions( std=c++11 )

add_executable(
	des
	${sources}
)