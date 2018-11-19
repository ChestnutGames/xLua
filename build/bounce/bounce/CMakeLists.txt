# cmake_minimum_required(VERSION 3.12)

project(libbounce)

include_directories(../fixmath/include)

if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_C_FLAGS "-std=gnu99 -Wall -Wextra -fPIC")
    set (CMAKE_C_FLAGS_DEBUG "-g -O0")
    set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -fPIC")
	set (CMAKE_CXX_FLAGS_DEBUG "-g -O0")
	# message (${CMAKE_C_FLAGS})
	# message (${CMAKE_CXX_FLAGS})
endif ()

set (BOUNCE_CORE ./Collision/b3BroadPhase.cpp ./Collision/b3DynamicAABBTree.cpp ./Collision/b3SAT.cpp 
	./Collision/Shapes/b3Hull.cpp ./Collision/Shapes/b3Polyhedron.cpp ./Collision/Shapes/b3Shape.cpp
	./Common/b3Time.cpp ./Common/b3Settings.cpp
	./Common/Memory/b3BlockAllocator.cpp ./Common/Memory/b3StackAllocator.cpp
	./Dynamics/b3World.cpp ./Dynamics/b3Island.cpp ./Dynamics/b3Body.cpp
	./Dynamics/Joints/b3JointGraph.cpp ./Dynamics/Joints/b3JointSolver.cpp ./Dynamics/Joints/b3MouseJoint.cpp
	./Dynamics/Joints/b3RevoluteJoint.cpp ./Dynamics/Joints/b3SphericalJoint.cpp
	./Dynamics/Contacts/b3ContactGraph.cpp ./Dynamics/Contacts/b3ContactSolver.cpp
)

add_library(libbounce STATIC ${BOUNCE_CORE})
#target_compile_definitions(libbounce PRIVATE FIXEDPT)
set_target_properties(libbounce
    PROPERTIES
    PREFIX ""
	RUNTIME_OUTPUT_DIRECTORY ../..
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ../..
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ../..
	ARCHIVE_OUTPUT_DIRECTORY ../..
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ../..
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ../..
    LIBRARY_OUTPUT_DIRECTORY_DEBUG ../..
)
