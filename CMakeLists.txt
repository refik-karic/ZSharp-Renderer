cmake_minimum_required(VERSION 3.23)

project(SoftwareRendererV3 VERSION 1.0)

add_subdirectory(ZSharp)
add_subdirectory(SoftwareRenderer)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
	message("Generated default build configuration using ${CMAKE_GENERATOR}.")
else()
	message("Generated ${CMAKE_BUILD_TYPE} build configuration using ${CMAKE_GENERATOR}.")
endif()
