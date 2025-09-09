cmake_minimum_required(VERSION 3.10)
project(trjadjust)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD          17)

include_directories(${PROJECT_SOURCE_DIR})

include(CheckCXXCompilerFlag)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
find_package(Boost REQUIRED)
message(STATUS "Boost include path : ${Boost_INCLUDE_DIR}")
message(STATUS "Boost library path : ${Boost_LIBRARY}")

set(TOML11_PRECOMPILE ON)

set(TRJADJUST_WARNING_FLAGS "")
check_cxx_compiler_flag("-Wall" TRJADJUST_COMPILER_HAS_WALL)
if(TRJADJUST_COMPILER_HAS_WALL)
	set(TRJADJUST_WARNING_FLAGS "${TRJADJUST_WARNING_FLAGS} -Wall")
endif()

message(STATUS "warning flags are ${TRJADJUST_WARNING_FLAGS}")

set(TRJADJUST_OPTIMIZATION_FLAGS "")
option(DEBUG "compile in debug mode" OFF)
if(DEBUG)
	check_cxx_compiler_flag("-O0" TRJADJUST_COMPILER_HAS_O0)
	if(TRJADJUST_COMPILER_HAS_O0)
		set(TRJADJUST_OPTIMIZATION_FLAGS "${TRJADJUST_OPTIMIZATION_FLAGS} -O0")
	endif()
	check_cxx_compiler_flag("-g" TRJADJUST_COMPILER_HAS_G)
	if(TRJADJUST_COMPILER_HAS_G)
		set(TRJADJUST_OPTIMIZATION_FLAGS "${TRJADJUST_OPTIMIZATION_FLAGS} -g")
	endif()
else()
	check_cxx_compiler_flag("-Ofast" TRJADJUST_COMPILER_HAS_OFAST)
	if(TRJADJUST_COMPILER_HAS_OFAST)
		set(TRJADJUST_OPTIMIZATION_FLAGS "${TRJADJUST_OPTIMIZATION_FLAGS} -Ofast")
	endif()
endif()

message(STATUS "optimization flags are ${TRJADJUST_OPTIMIZATION_FLAGS}")

set(TRJADJUST_DEBUGGER_FLAGS "")
option(GDB "add flag for gdb" OFF)
if(GDB)
	check_cxx_compiler_flag("-ggdb" TRJADJUST_COMPILER_HAS_GGDB)
	if(TRJADJUST_COMPILER_HAS_GGDB)
		set(TRJADJUST_DEBUGGER_FLAGS "${TRJADJUST_DEBUGGER_FLAGS} -ggdb")
	endif()
endif()

message(STATUS "debugger flags are ${TRJADJUST_DEBUGGER_FLAGS}")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 8.0 AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
		link_libraries(stdc++fs)
	endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "NVHPC")
	link_libraries(stdc++fs)
endif()

add_subdirectory(toml11)
add_subdirectory(src)
