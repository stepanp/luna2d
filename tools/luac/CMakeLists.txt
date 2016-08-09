project(luac)
cmake_minimum_required(VERSION 2.8)

set(LUA_PATH ${PROJECT_SOURCE_DIR}/../../thirdparty/lua)

option(BUILD_64BIT "Build 64-version" OFF)

include_directories(${LUA_PATH})
aux_source_directory(${LUA_PATH} SOURCES)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

if(BUILD_64BIT)
	set(CMAKE_C_FLAGS "-m64")
	add_executable("luac64" ${SOURCES})
else()
	set(CMAKE_C_FLAGS "-m32")
	add_executable("luac" ${SOURCES})
endif()