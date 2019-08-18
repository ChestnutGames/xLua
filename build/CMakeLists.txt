# Tencent is pleased to support the open source community by making xLua available.
# Copyright (C) 2016 THL A29 Limited, a Tencent company. All rights reserved.
# Licensed under the MIT License (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at
# http://opensource.org/licenses/MIT
# Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

cmake_minimum_required(VERSION 2.8)

if ( WIN32 AND NOT CYGWIN AND NOT ( CMAKE_SYSTEM_NAME STREQUAL "WindowsStore" ) AND NOT ANDROID)
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT" CACHE STRING "")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd" CACHE STRING "")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT" CACHE STRING "")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd" CACHE STRING "")
endif ()

project(XLua)

if ( IOS )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fembed-bitcode")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fembed-bitcode")
endif ()

option ( UINT_ESPECIALLY "using custom ulong" OFF )
option ( USING_LUAJIT "using luajit" OFF )
option ( GC64 "using gc64" OFF )
option ( LUAC_COMPATIBLE_FORMAT "compatible format" OFF )

find_path(XLUA_PROJECT_DIR NAMES SConstruct
    PATHS 
    ${CMAKE_SOURCE_DIR}
    NO_DEFAULT_PATH
    )

MARK_AS_ADVANCED(XLUA_PROJECT_DIR)
set(LUA_SRC_PATH lua-5.3.5/src)
set(LUAJIT_SRC_PATH luajit-2.1.0b3/src)

set ( LUA_RAPIDJSON
    rapidjson/Document.cpp
    rapidjson/rapidjson.cpp
    rapidjson/Schema.cpp
    rapidjson/values.cpp
)
set_property (
    SOURCE ${LUA_RAPIDJSON}
    APPEND
    PROPERTY COMPILE_DEFINITIONS
    LUA_LIB
	XLUA
)
list ( APPEND THIRDPART_INC rapidjson/include )
list ( APPEND THIRDPART_SRC ${LUA_RAPIDJSON} )

# thirdpart
set ( LUA_SPROTO
    sproto/lsproto.c
    sproto/sproto.c
)
set ( LUA_LPEG
    lpeg/lpcap.c
    lpeg/lpcode.c
    lpeg/lpprint.c
    lpeg/lptree.c
    lpeg/lpvm.c
)
set ( LUA_XLUASOCKET
	xluasocket/posix/unistd.c
	xluasocket/Win32_Interop/Win32_APIs.c
	xluasocket/Win32_Interop/win32_cpoll.c
	xluasocket/Win32_Interop/Win32_dlfcn.c
	xluasocket/Win32_Interop/Win32_Error.c
	xluasocket/Win32_Interop/Win32_Extras.c
	xluasocket/Win32_Interop/Win32_PThread.c
	xluasocket/Win32_Interop/Win32_ThreadControl.c
	xluasocket/Win32_Interop/Win32_Time.c
	xluasocket/dict.c
	xluasocket/lxluasocket.c 
	xluasocket/message_queue.c
	xluasocket/ringbuf.c
	xluasocket/skynet_timer.c
	xluasocket/socket_server.c
	xluasocket/util.c
	xluasocket/xxhash.c
	xluasocket/zmalloc.c
)
set ( LUA_SKYNET
    skynet/lsha1.c
    skynet/lua-crypt.c
)
set ( LUA_MD5 md5/md5.c)
set ( LUA_RANDOM random/random.c)
set ( LUA_TIMER timer/timer.c)
set ( LUA_CHESTNUT
    chestnut/lua-array.c
	chestnut/lua-queue.c
	chestnut/lua-stack.c
    chestnut/lua-vector.c
)
set ( LUA_RUDP
	rudp/lua-rudp.c
	rudp/rudp.c
	rudp/rudp.h
)
set ( LUA_MTRAND
	mt_rand/lua-mtrand.c
	mt_rand/mt_rand.c
	mt_rand/mt_rand.h
)
set ( LUA_KCP
	kcp/ikcp.c
	kcp/ikcp.h
	kcp/lkcp.c
	kcp/lutil.c
)
set ( LUA_FIXMATH 
    #./fixmath/bounce/Common/Math/b3r32.h ./fixmath/bounce/Common/Math/b3r32.cpp
    #./fixmath/bounce/Collision/b3BroadPhase.h ./fixmath/bounce/Collision/b3DynamicAABBTree.h ./fixmath/bounce/Collision/b3SAT.h
    #./fixmath/bounce/Collision/b3BroadPhase.cpp ./fixmath/bounce/Collision/b3DynamicAABBTree.cpp ./fixmath/bounce/Collision/b3SAT.cpp
    #./fixmath/bounce/Collision/Shapes/b3Hull.h ./fixmath/bounce/Collision/Shapes/b3Polyhedron.h ./fixmath/bounce/Collision/Shapes/b3Shape.h
    #./fixmath/bounce/Collision/Shapes/b3Hull.cpp ./fixmath/bounce/Collision/Shapes/b3Polyhedron.cpp ./fixmath/bounce/Collision/Shapes/b3Shape.cpp
    #./fixmath/bounce/Common/b3Time.h ./fixmath/bounce/Common/b3Settings.h
    #./fixmath/bounce/Common/b3Time.cpp ./fixmath/bounce/Common/b3Settings.cpp
    #./fixmath/bounce/Common/Math/b3Vec3.h
    #./fixmath/bounce/Common/Memory/b3BlockAllocator.h ./fixmath/bounce/Common/Memory/b3StackAllocator.h
    #./fixmath/bounce/Common/Memory/b3BlockAllocator.cpp ./fixmath/bounce/Common/Memory/b3StackAllocator.cpp
    #./fixmath/bounce/Dynamics/b3World.h ./fixmath/bounce/Dynamics/b3Island.h ./fixmath/bounce/Dynamics/b3Body.h
    #./fixmath/bounce/Dynamics/b3World.cpp ./fixmath/bounce/Dynamics/b3Island.cpp ./fixmath/bounce/Dynamics/b3Body.cpp
    #./fixmath/bounce/Dynamics/Joints/b3JointGraph.cpp ./fixmath/bounce/Dynamics/Joints/b3JointSolver.cpp ./fixmath/bounce/Dynamics/Joints/b3MouseJoint.cpp
    #./fixmath/bounce/Dynamics/Joints/b3RevoluteJoint.cpp ./fixmath/bounce/Dynamics/Joints/b3SphericalJoint.cpp
    #./fixmath/bounce/Dynamics/Contacts/b3ContactGraph.cpp ./fixmath/bounce/Dynamics/Contacts/b3ContactSolver.cpp
    #./fixmath/bounce/lua-bounce.cpp
	./fixmath/reactphysics3d/body/Body.cpp
	./fixmath/reactphysics3d/body/Body.h
	./fixmath/reactphysics3d/body/CollisionBody.cpp
	./fixmath/reactphysics3d/body/CollisionBody.h
	./fixmath/reactphysics3d/body/RigidBody.cpp
	./fixmath/reactphysics3d/body/RigidBody.h
	./fixmath/reactphysics3d/collision/broadphase/BroadPhaseAlgorithm.cpp
	./fixmath/reactphysics3d/collision/broadphase/BroadPhaseAlgorithm.h
	./fixmath/reactphysics3d/collision/broadphase/DynamicAABBTree.cpp
	./fixmath/reactphysics3d/collision/broadphase/DynamicAABBTree.h
	./fixmath/reactphysics3d/collision/narrowphase/GJK/GJKAlgorithm.cpp
	./fixmath/reactphysics3d/collision/narrowphase/GJK/GJKAlgorithm.h
	./fixmath/reactphysics3d/collision/narrowphase/GJK/VoronoiSimplex.cpp
	./fixmath/reactphysics3d/collision/narrowphase/GJK/VoronoiSimplex.h
	./fixmath/reactphysics3d/collision/narrowphase/SAT/SATAlgorithm.cpp
	./fixmath/reactphysics3d/collision/narrowphase/SAT/SATAlgorithm.h
	./fixmath/reactphysics3d/collision/narrowphase/CapsuleVsCapsuleAlgorithm.cpp
	./fixmath/reactphysics3d/collision/narrowphase/CapsuleVsCapsuleAlgorithm.h
	./fixmath/reactphysics3d/collision/narrowphase/CapsuleVsConvexPolyhedronAlgorithm.cpp
	./fixmath/reactphysics3d/collision/narrowphase/CapsuleVsConvexPolyhedronAlgorithm.h
	./fixmath/reactphysics3d/collision/narrowphase/CollisionDispatch.h
	./fixmath/reactphysics3d/collision/narrowphase/ConvexPolyhedronVsConvexPolyhedronAlgorithm.cpp
	./fixmath/reactphysics3d/collision/narrowphase/ConvexPolyhedronVsConvexPolyhedronAlgorithm.h
	./fixmath/reactphysics3d/collision/narrowphase/DefaultCollisionDispatch.cpp
	./fixmath/reactphysics3d/collision/narrowphase/DefaultCollisionDispatch.h
	./fixmath/reactphysics3d/collision/narrowphase/NarrowPhaseAlgorithm.h
	./fixmath/reactphysics3d/collision/narrowphase/SphereVsCapsuleAlgorithm.cpp
	./fixmath/reactphysics3d/collision/narrowphase/SphereVsCapsuleAlgorithm.h
	./fixmath/reactphysics3d/collision/narrowphase/SphereVsConvexPolyhedronAlgorithm.cpp
	./fixmath/reactphysics3d/collision/narrowphase/SphereVsConvexPolyhedronAlgorithm.h
	./fixmath/reactphysics3d/collision/narrowphase/SphereVsSphereAlgorithm.cpp
	./fixmath/reactphysics3d/collision/narrowphase/SphereVsSphereAlgorithm.h
	./fixmath/reactphysics3d/collision/shapes/AABB.cpp
	./fixmath/reactphysics3d/collision/shapes/AABB.h
	./fixmath/reactphysics3d/collision/shapes/BoxShape.cpp
	./fixmath/reactphysics3d/collision/shapes/BoxShape.h
	./fixmath/reactphysics3d/collision/shapes/CapsuleShape.cpp
	./fixmath/reactphysics3d/collision/shapes/CapsuleShape.h
	./fixmath/reactphysics3d/collision/shapes/CollisionShape.cpp
	./fixmath/reactphysics3d/collision/shapes/CollisionShape.h
	./fixmath/reactphysics3d/collision/shapes/ConcaveMeshShape.cpp
	./fixmath/reactphysics3d/collision/shapes/ConcaveMeshShape.h
	./fixmath/reactphysics3d/collision/shapes/ConcaveShape.cpp
	./fixmath/reactphysics3d/collision/shapes/ConcaveShape.h
	./fixmath/reactphysics3d/collision/shapes/ConvexMeshShape.cpp
	./fixmath/reactphysics3d/collision/shapes/ConvexMeshShape.h
	./fixmath/reactphysics3d/collision/shapes/ConvexPolyhedronShape.cpp
	./fixmath/reactphysics3d/collision/shapes/ConvexPolyhedronShape.h
	./fixmath/reactphysics3d/collision/shapes/ConvexShape.cpp
	./fixmath/reactphysics3d/collision/shapes/ConvexShape.h
	./fixmath/reactphysics3d/collision/shapes/HeightFieldShape.cpp
	./fixmath/reactphysics3d/collision/shapes/HeightFieldShape.h
	./fixmath/reactphysics3d/collision/shapes/SphereShape.cpp
	./fixmath/reactphysics3d/collision/shapes/SphereShape.h
	./fixmath/reactphysics3d/collision/shapes/TriangleShape.cpp
	./fixmath/reactphysics3d/collision/shapes/TriangleShape.h
	./fixmath/reactphysics3d/collision/CollisionCallback.cpp
	./fixmath/reactphysics3d/collision/CollisionCallback.h
	./fixmath/reactphysics3d/collision/CollisionDetection.cpp
	./fixmath/reactphysics3d/collision/CollisionDetection.h
	./fixmath/reactphysics3d/collision/ContactManifold.cpp
	./fixmath/reactphysics3d/collision/ContactManifold.h
	./fixmath/reactphysics3d/collision/ContactManifoldInfo.cpp
	./fixmath/reactphysics3d/collision/ContactManifoldInfo.h
	./fixmath/reactphysics3d/collision/ContactManifoldSet.cpp
	./fixmath/reactphysics3d/collision/ContactManifoldSet.h
	./fixmath/reactphysics3d/collision/ContactPointInfo.h
	./fixmath/reactphysics3d/collision/HalfEdgeStructure.cpp
	./fixmath/reactphysics3d/collision/HalfEdgeStructure.h
	./fixmath/reactphysics3d/collision/MiddlePhaseTriangleCallback.cpp
	./fixmath/reactphysics3d/collision/MiddlePhaseTriangleCallback.h
	./fixmath/reactphysics3d/collision/NarrowPhaseInfo.cpp
	./fixmath/reactphysics3d/collision/NarrowPhaseInfo.h
	./fixmath/reactphysics3d/collision/OverlapCallback.h
	./fixmath/reactphysics3d/collision/PolygonVertexArray.cpp
	./fixmath/reactphysics3d/collision/PolygonVertexArray.h
	./fixmath/reactphysics3d/collision/PolyhedronMesh.cpp
	./fixmath/reactphysics3d/collision/PolyhedronMesh.h
	./fixmath/reactphysics3d/collision/ProxyShape.cpp
	./fixmath/reactphysics3d/collision/ProxyShape.h
	./fixmath/reactphysics3d/collision/RaycastInfo.cpp
	./fixmath/reactphysics3d/collision/RaycastInfo.h
	./fixmath/reactphysics3d/collision/TriangleMesh.cpp
	./fixmath/reactphysics3d/collision/TriangleMesh.h
	./fixmath/reactphysics3d/collision/TriangleVertexArray.cpp
	./fixmath/reactphysics3d/collision/TriangleVertexArray.h
	./fixmath/reactphysics3d/constraint/BallAndSocketJoint.cpp
	./fixmath/reactphysics3d/constraint/BallAndSocketJoint.h
	./fixmath/reactphysics3d/constraint/ContactPoint.cpp
	./fixmath/reactphysics3d/constraint/ContactPoint.h
	./fixmath/reactphysics3d/constraint/FixedJoint.cpp
	./fixmath/reactphysics3d/constraint/FixedJoint.h
	./fixmath/reactphysics3d/constraint/HingeJoint.cpp
	./fixmath/reactphysics3d/constraint/HingeJoint.h
	./fixmath/reactphysics3d/constraint/Joint.cpp
	./fixmath/reactphysics3d/constraint/Joint.h
	./fixmath/reactphysics3d/constraint/SliderJoint.cpp
	./fixmath/reactphysics3d/constraint/SliderJoint.h
	./fixmath/reactphysics3d/engine/CollisionWorld.cpp
	./fixmath/reactphysics3d/engine/CollisionWorld.h
	./fixmath/reactphysics3d/engine/ConstraintSolver.cpp
	./fixmath/reactphysics3d/engine/ConstraintSolver.h
	./fixmath/reactphysics3d/engine/ContactSolver.cpp
	./fixmath/reactphysics3d/engine/ContactSolver.h
	./fixmath/reactphysics3d/engine/DynamicsWorld.cpp
	./fixmath/reactphysics3d/engine/DynamicsWorld.h
	./fixmath/reactphysics3d/engine/EventListener.h
	./fixmath/reactphysics3d/engine/Island.cpp
	./fixmath/reactphysics3d/engine/Material.cpp
	./fixmath/reactphysics3d/engine/OverlappingPair.cpp
	./fixmath/reactphysics3d/engine/Timer.cpp
	./fixmath/reactphysics3d/mathematics/mathematics_functions.cpp
	./fixmath/reactphysics3d/mathematics/Matrix2x2.cpp
	./fixmath/reactphysics3d/mathematics/Matrix3x3.cpp
	./fixmath/reactphysics3d/mathematics/Matrix3x3.h
	./fixmath/reactphysics3d/mathematics/Quaternion.cpp
	./fixmath/reactphysics3d/mathematics/Transform.cpp
	./fixmath/reactphysics3d/mathematics/Transform.h
	./fixmath/reactphysics3d/mathematics/Vector2.cpp
	./fixmath/reactphysics3d/mathematics/Vector2.h
	./fixmath/reactphysics3d/mathematics/Vector3.cpp
	./fixmath/reactphysics3d/mathematics/Vector3.h
	./fixmath/reactphysics3d/memory/DefaultPoolAllocator.cpp
	./fixmath/reactphysics3d/memory/DefaultSingleFrameAllocator.cpp
	./fixmath/reactphysics3d/memory/MemoryManager.cpp
	./fixmath/reactphysics3d/utils/Logger.cpp
	./fixmath/reactphysics3d/utils/Logger.h
	./fixmath/reactphysics3d/utils/Profiler.cpp
	./fixmath/reactphysics3d/b3r32.cpp
	./fixmath/reactphysics3d/b3r32.h
	./fixmath/reactphysics3d/decimal.h
	./fixmath/reactphysics3d/lua-reactphysics3d.cpp
    ./fixmath/libfixmath/fix16.h
    ./fixmath/libfixmath/fix16.c 
	./fixmath/libfixmath/fix16_exp.c 
	./fixmath/libfixmath/fix16_sqrt.c 
	./fixmath/libfixmath/fix16_str.c 
    ./fixmath/libfixmath/fix16_trig.c 
	./fixmath/libfixmath/fract32.c 
	./fixmath/libfixmath/uint32.c
)
set ( LUA_PLIST 
	plist/libcnary/iterator.c
	plist/libcnary/list.c
	plist/libcnary/node.c
	plist/libcnary/node_iterator.c
	plist/libcnary/node_list.c
	plist/src/base64.c
	plist/src/bplist.c
	plist/src/bytearray.c
	plist/src/hashtable.c
	plist/src/plist.c
	plist/src/ptrarray.c
	plist/src/time64.c
	plist/src/xplist.c
)
set_property(
    SOURCE ${LUA_MD5} ${LUA_RANDOM} ${LUA_TIMER} ${LUA_CHESTNUT} ${LUA_RUDP} ${LUA_MTRAND} ${LUA_KCP} ${LUA_SKYNET} ${LUA_FIXMATH} ${LUA_PLIST} ${LUA_XLUASOCKET} ${LUA_LPEG} ${LUA_SPROTO}
    APPEND
    PROPERTY COMPILE_DEFINITIONS
    LUA_LIB
)
set_property(
    SOURCE ${LUA_XLUASOCKET}
    APPEND
    PROPERTY COMPILE_DEFINITIONS
	XLUASOCKET
)
list ( APPEND THIRDPART_SRC ${LUA_MD5} ${LUA_RANDOM} ${LUA_TIMER} ${LUA_CHESTNUT} ${LUA_RUDP} ${LUA_MTRAND} ${LUA_KCP} ${LUA_SKYNET} ${LUA_XLUASOCKET} ${LUA_LPEG} ${LUA_SPROTO} ${LUA_FIXMATH} )
list ( APPEND THIRDPART_SRC ${LUA_PLIST})
list ( APPEND THIRDPART_INC plist/include )
list ( APPEND THIRDPART_INC plist/libcnary/include )
list ( APPEND THIRDPART_INC LuaBridge fixmath/libfixmath fixmath/reactphysics3d )
list ( APPEND THIRDPART_INC xluasocket/posix )

if (USING_LUAJIT)
	
    if (APPLE AND NOT IOS AND NOT ANDROID)
	    include_directories(
		  ${CMAKE_SOURCE_DIR}
		  lua-5.1.5/src
		  lua-5.1.5/etc
		  ${CMAKE_CURRENT_BINARY_DIR}
		  ${THIRDPART_INC}
	    )
	    set ( LUA_CORE lua-5.1.5/src/lapi.c lua-5.1.5/src/lcode.c lua-5.1.5/src/ldebug.c lua-5.1.5/src/ldo.c lua-5.1.5/src/ldump.c lua-5.1.5/src/lfunc.c 
          lua-5.1.5/src/lgc.c lua-5.1.5/src/llex.c lua-5.1.5/src/lmem.c lua-5.1.5/src/lobject.c lua-5.1.5/src/lopcodes.c lua-5.1.5/src/lparser.c lua-5.1.5/src/lstate.c 
          lua-5.1.5/src/lstring.c lua-5.1.5/src/ltable.c lua-5.1.5/src/ltm.c lua-5.1.5/src/lundump.c lua-5.1.5/src/lvm.c lua-5.1.5/src/lzio.c lua-5.1.5/src/lauxlib.c 
          lua-5.1.5/src/lbaselib.c lua-5.1.5/src/ldblib.c lua-5.1.5/src/liolib.c lua-5.1.5/src/lmathlib.c lua-5.1.5/src/loslib.c lua-5.1.5/src/ltablib.c 
          lua-5.1.5/src/lstrlib.c lua-5.1.5/src/linit.c lua-5.1.5/src/loadlib.c)
	else ()
	    include_directories(
		  ${CMAKE_SOURCE_DIR}
		  ${LUAJIT_SRC_PATH}
		  ${CMAKE_CURRENT_BINARY_DIR}
		  ${THIRDPART_INC}
	    )

	    set ( LUA_CORE )
	    set_property( SOURCE xlua.c APPEND PROPERTY COMPILE_DEFINITIONS USING_LUAJIT )
    endif ()
	set ( LUA_LIB )
else ()
	set ( LUA_IDSIZE 120 CACHE NUMBER "gives the maximum size for the description of the source." )

	configure_file ( ${LUA_SRC_PATH}/luaconf.h.in ${CMAKE_CURRENT_BINARY_DIR}/luaconf.h )

	include_directories(
	    ${CMAKE_SOURCE_DIR}
		${LUA_SRC_PATH}
		${CMAKE_CURRENT_BINARY_DIR}
		${THIRDPART_INC}
	)

	set ( LUA_CORE ${LUA_SRC_PATH}/lapi.c ${LUA_SRC_PATH}/lcode.c ${LUA_SRC_PATH}/lctype.c ${LUA_SRC_PATH}/ldebug.c ${LUA_SRC_PATH}/ldo.c ${LUA_SRC_PATH}/ldump.c
	  ${LUA_SRC_PATH}/lfunc.c ${LUA_SRC_PATH}/lgc.c ${LUA_SRC_PATH}/llex.c ${LUA_SRC_PATH}/lmem.c ${LUA_SRC_PATH}/lobject.c ${LUA_SRC_PATH}/lopcodes.c ${LUA_SRC_PATH}/lparser.c
	  ${LUA_SRC_PATH}/lstate.c ${LUA_SRC_PATH}/lstring.c ${LUA_SRC_PATH}/ltable.c ${LUA_SRC_PATH}/ltm.c ${LUA_SRC_PATH}/lundump.c ${LUA_SRC_PATH}/lvm.c ${LUA_SRC_PATH}/lzio.c )
	set ( LUA_LIB ${LUA_SRC_PATH}/lauxlib.c ${LUA_SRC_PATH}/lbaselib.c ${LUA_SRC_PATH}/lbitlib.c ${LUA_SRC_PATH}/lcorolib.c ${LUA_SRC_PATH}/ldblib.c
	  ${LUA_SRC_PATH}/liolib.c ${LUA_SRC_PATH}/lmathlib.c ${LUA_SRC_PATH}/loslib.c ${LUA_SRC_PATH}/lstrlib.c ${LUA_SRC_PATH}/ltablib.c ${LUA_SRC_PATH}/linit.c
	  ${LUA_SRC_PATH}/lutf8lib.c ${LUA_SRC_PATH}/loadlib.c )
endif ()

set ( LUA_SOCKET
    luasocket/auxiliar.c
    luasocket/buffer.c
    luasocket/except.c
    luasocket/inet.c
    luasocket/io.c
    luasocket/luasocket.c
    luasocket/mime.c
    luasocket/options.c
    luasocket/select.c
    luasocket/tcp.c
    luasocket/timeout.c
    luasocket/udp.c 
)

if ( WIN32 AND NOT CYGWIN )
    list (APPEND LUA_SOCKET luasocket/wsocket.c)
else ( )
    list (APPEND LUA_SOCKET luasocket/usocket.c)
endif ( )

set ( XLUA_CORE
    i64lib.c
    xlua.c
)

if (NOT USING_LUAJIT)
    list (APPEND XLUA_CORE memory_leak_checker.c)
endif ( )

macro(source_group_by_dir proj_dir source_files)
    if(MSVC OR APPLE)
        get_filename_component(sgbd_cur_dir ${proj_dir} ABSOLUTE)
        foreach(sgbd_file ${${source_files}})
			get_filename_component(sgbd_abs_file ${sgbd_file} ABSOLUTE)
            file(RELATIVE_PATH sgbd_fpath ${sgbd_cur_dir} ${sgbd_abs_file})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
            if(sgbd_nogroup)
                set(sgbd_group_name "\\")
            endif(sgbd_nogroup)
            source_group(${sgbd_group_name} FILES ${sgbd_file})
        endforeach(sgbd_file)
    endif(MSVC OR APPLE)
endmacro(source_group_by_dir)

source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} LUA_SOCKET)
source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} LUA_CORE)
source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} LUA_LIB)
source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} XLUA_CORE)
source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} LUA_RAPIDJSON)
source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} LUA_CHESTNUT)
source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} LUA_SPROTO)
source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} LUA_LPEG)
source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} LUA_SKYNET)
source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} LUA_XLUASOCKET)
source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} LUA_MD5)
source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} LUA_RANDOM)
source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} LUA_TIMER)
source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} LUA_RUDP)
source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} LUA_MTRAND)
source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} LUA_KCP)
source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} LUA_FIXMATH)
source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} LUA_PLIST)

if (APPLE)
    if (IOS)
        set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)")
        add_library(xlua STATIC
           ${LUA_CORE}
           ${LUA_LIB}
           ${LUA_SOCKET}
           ${XLUA_CORE}
           ${THIRDPART_SRC}
        )
		set_xcode_property (xlua IPHONEOS_DEPLOYMENT_TARGET "7.0" "all")
    else ()
        set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD_64_BIT)")
        add_library(xlua MODULE
            ${LUA_CORE}
            ${LUA_LIB}
            ${LUA_SOCKET}
            ${XLUA_CORE}
            ${THIRDPART_SRC}
        )
        set_target_properties ( xlua PROPERTIES BUNDLE TRUE )
        #set_target_properties ( xlua PROPERTIES FRAMEWORK TRUE )
        #set_target_properties ( xlua PROPERTIES MACOSX_RPATH TRUE )
    endif ()
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Switch")
    add_library(xlua STATIC
        ${LUA_CORE}
        ${LUA_LIB}
        ${XLUA_CORE}
        ${THIRDPART_SRC}
    )
    target_compile_options(xlua PRIVATE -m64 -mcpu=cortex-a57+fp+simd+crypto+crc -fno-common -fno-short-enums -ffunction-sections -fdata-sections -fPIC -fms-extensions)
else ( )
    add_library(xlua SHARED
        ${LUA_CORE}
        ${LUA_LIB}
        ${LUA_SOCKET}
        ${XLUA_CORE}
        ${THIRDPART_SRC}
    )
endif ( )

if ( WIN32 AND NOT CYGWIN )
    target_compile_definitions (xlua PRIVATE LUA_BUILD_AS_DLL)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
endif ( )

if (GC64 AND USING_LUAJIT)
    target_compile_definitions (xlua PRIVATE LUAJIT_ENABLE_GC64)
endif ()

if (LUAC_COMPATIBLE_FORMAT AND NOT USING_LUAJIT)
    target_compile_definitions (xlua PRIVATE LUAC_COMPATIBLE_FORMAT)
endif ()

set_property(
	SOURCE ${LUA_SOCKET}
	APPEND
	PROPERTY COMPILE_DEFINITIONS
	LUA_COMPAT_APIINTCASTS LUA_LIB _CRT_SECURE_NO_WARNINGS _WINSOCK_DEPRECATED_NO_WARNINGS
)

if(UINT_ESPECIALLY)
    ADD_DEFINITIONS(-DUINT_ESPECIALLY)
endif()
	
if ( WIN32 AND NOT CYGWIN )
    if (USING_LUAJIT)
        target_link_libraries(xlua
            ${CMAKE_SOURCE_DIR}/${LUAJIT_SRC_PATH}/lua51.lib
            ws2_32
            ${THIRDPART_LIB} 
        )
    else ()
        target_link_libraries(xlua
            ws2_32
            ${THIRDPART_LIB}
            #${CMAKE_SOURCE_DIR}/chestnut/clib/libplist.lib
        )
    endif()
else ( )
    if (USING_LUAJIT AND NOT APPLE)
		target_link_libraries(xlua
			${CMAKE_SOURCE_DIR}/${LUAJIT_SRC_PATH}/libluajit.a
			m
            ${THIRDPART_LIB} 
		)
    else ()
        target_link_libraries(xlua
            m
            ${THIRDPART_LIB} 
        )
    endif()
endif ( )
