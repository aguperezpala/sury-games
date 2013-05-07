
cmake_minimum_required(VERSION 2.6)
 
project(fieshes)

# Build in debug by default
add_definitions(-DDEBUG)
 
if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()
 
set(CMAKE_DEBUG_POSTFIX "_d")
 
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")

# Use the source path (from the environment)
set (DEV_ROOT_PATH $ENV{SURY_FISHES_DEV_PATH})
 
# Common headers
set(HDRS
	${DEV_ROOT_PATH}/common/debug/DebugUtil.h
)
 
# Common sources
set(SRCS
	./testUI.cpp
	
)

# Common includes path 
include_directories(
	# SFML local extlib
	${DEV_ROOT_PATH}/extlib/sfml2.0/include
	
	# Root dev path
	${DEV_ROOT_PATH}
	
	# common
	${DEV_ROOT_PATH}/common
	
	# core
	${DEV_ROOT_PATH}/core
)

# include all the AutoGen.cmake
include(${DEV_ROOT_PATH}/core/math/AutoGen.cmake)
include(${DEV_ROOT_PATH}/core/utils/AutoGen.cmake)
include(${DEV_ROOT_PATH}/core/ui/AutoGen.cmake)
include(${DEV_ROOT_PATH}/core/system/AutoGen.cmake)
include(${DEV_ROOT_PATH}/core/resource_manager/AutoGen.cmake)
include(${DEV_ROOT_PATH}/core/scene/AutoGen.cmake)
include(${DEV_ROOT_PATH}/core/space_partition/AutoGen.cmake)
include(${DEV_ROOT_PATH}/common/configuration/AutoGen.cmake)


# Set all the libraries here
# Set the default flags to the build
link_directories(${DEV_ROOT_PATH}/extlib/sfml2.0/lib)
add_definitions(-DDEBUG)
add_definitions(-std=c++0x)  # C++11 standard
add_definitions(-Wall)       # compile with all the warnings
set(COMMON_LIBRARIES boost_signals boost_system boost_filesystem
                    #sfml 
                    sfml-graphics 
                    sfml-window 
                    sfml-system)


add_executable(fishes WIN32 ${HDRS} ${SRCS}) 
set_target_properties(fishes PROPERTIES DEBUG_POSTFIX _d)
target_link_libraries(fishes ${COMMON_LIBRARIES})
 
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/bin)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/media)
 
# post-build copy for win32
if(WIN32 AND NOT MINGW)
	add_custom_command( TARGET fishes PRE_BUILD
		COMMAND if not exist .\\dist\\bin mkdir .\\dist\\bin )
	add_custom_command( TARGET fishes POST_BUILD
		COMMAND copy \"$(TargetPath)\" .\\dist\\bin )
endif(WIN32 AND NOT MINGW)
if(MINGW)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW)
 
if(WIN32)
 
#	install(TARGETS fishes
#		RUNTIME DESTINATION bin
#		CONFIGURATIONS All)
 
#	install(DIRECTORY ${CMAKE_SOURCE_DIR}/dist/Media
#		DESTINATION ./
#		CONFIGURATIONS Release RelWithDebInfo Debug
#	)
 
#	install(FILES ${CMAKE_SOURCE_DIR}/dist/bin/plugins.cfg
#		${CMAKE_SOURCE_DIR}/dist/bin/resources.cfg
#		DESTINATION bin
#		CONFIGURATIONS Release RelWithDebInfo
#	)
 
#	install(FILES ${CMAKE_SOURCE_DIR}/dist/bin/plugins_d.cfg
#		${CMAKE_SOURCE_DIR}/dist/bin/resources_d.cfg
#		DESTINATION bin
#		CONFIGURATIONS Debug
#	)
 

endif(WIN32)
