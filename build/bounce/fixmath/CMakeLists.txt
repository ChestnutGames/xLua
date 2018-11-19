# cmake_minimum_required(VERSION 3.12)

project(libfixedptmath)

include_directories(include)

if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_C_FLAGS "-std=gnu99 -Wall -Wextra -fPIC")
    set (CMAKE_C_FLAGS_DEBUG "-g -O0")
    set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -fPIC")
	set (CMAKE_CXX_FLAGS_DEBUG "-g -O0")
	# message (${CMAKE_C_FLAGS})
	# message (${CMAKE_CXX_FLAGS})
endif ()

set (LIBFIXEDPTMATH_CORE ./src/fixmath.c ./src/fixmath_impl.c ./src/fixmath_str.c ./src/fixmath_ver.c)

add_library(libfixedptmath STATIC ${LIBFIXEDPTMATH_CORE})
target_compile_definitions(libfixedptmath PRIVATE FIXEDPT)
set_target_properties(libfixedptmath
    PROPERTIES
    PREFIX ""
	RUNTIME_OUTPUT_DIRECTORY ../..
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ../..
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ../..
	ARCHIVE_OUTPUT_DIRECTORY ../../..
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ../..
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ../..
    LIBRARY_OUTPUT_DIRECTORY_DEBUG ../..
)
