set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

find_package(Boost REQUIRED)
message(STATUS "Boost include path : ${Boost_INCLUDE_DIR}")
message(STATUS "Boost library path : ${Boost_LIBRARY}")

set(CMAKE_CXX_FLAGS "${TRJADJUST_WARNING_FLAGS} ${TRJADJUST_OPTIMIZATION_FLAGS} ${TRJADJUST_DEBUGGER_FLAGS}")
message(STATUS "CXX compiler flags are ${CMAKE_CXX_FLAGS}")

add_library(trjadjustlib STATIC
	input.cpp
	coordinate.cpp
	topology.cpp
	adjuster.cpp
	)
target_link_libraries(trjadjustlib PRIVATE
#	Boost::program_options)
	toml11::toml11
	boost_program_options)
add_executable(trjadjust main.cpp)
set_target_properties(trjadjust PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
#target_include_directories(trjadjust PRIVATE Boost::program_options)
target_include_directories(trjadjust PRIVATE boost_program_options)
target_link_libraries(trjadjust PRIVATE
#	Boost::program_options
	boost_program_options
	toml11::toml11
	trjadjustlib
	)

add_subdirectory(io)
add_subdirectory(util)
