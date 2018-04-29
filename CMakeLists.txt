cmake_minimum_required(VERSION 2.6)

cmake_policy(SET CMP0048 NEW) # Allowing VERSION option.

set(PROJECT_NAME Arch1eNgine)

project(${PROJECT_NAME} VERSION 0.0.0.1 LANGUAGES C CXX)

set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Binaries)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
set(DEPENDENCIES_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies)

# Macro for adding precompiled headers.
MACRO(add_msvc_precompiler_header PrecompiledHeader PrecompiledSource SourcesVar)
	if(WIN32)
		if(MSVC)
			get_filename_component(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
			set(PrecompiledBinary "${CMAKE_BINARY_DIR}/${PrecompiledBasename}.pch")
			set(${Sources} ${SourcesVar})
			
			set_source_files_properties(${PrecompiledSource}
				PROPERTIES 	COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
							OBJECT_OUTPUTS "${PrecompiledBinary}"
			)
			
			set_source_files_properties(${Sources}
                                PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_DEPENDS "${PrecompiledBinary}")  
			
		endif()
	endif()
	LIST(APPEND ${SourcesVar} ${PrecompiledSource})
ENDMACRO()

include_directories(${PROJECT_SOURCE_DIR})

# Add compiler flags

if(MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    set(CPP_LATEST_FLAG "/std:c++latest")
    CHECK_CXX_COMPILER_FLAG(${CPP_LATEST_FLAG} _cpp_latest_flag_supported)
    if(_cpp_latest_flag_supported)
        add_compile_options(${CPP_LATEST_FLAG})
    endif()
endif()

# Add *.cpp and *.h file paths to the SourceFiles variable.
file(GLOB_RECURSE SourceFiles ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.h)

#Include Dependencies Source Paths

include_directories(${DEPENDENCIES_SOURCE_DIR}/GLM)
include_directories(${DEPENDENCIES_SOURCE_DIR}/assimp/include)
include_directories(${DEPENDENCIES_SOURCE_DIR}/GLEW/include)
include_directories(${DEPENDENCIES_SOURCE_DIR}/SDL2_image/include)
include_directories(${DEPENDENCIES_SOURCE_DIR}/SDL2_mixer/include)
include_directories(${DEPENDENCIES_SOURCE_DIR}/DevIL/WIN/include)
include_directories(${DEPENDENCIES_SOURCE_DIR}/STB_image)

# Include Dependencies Library Paths

if(${CMAKE_SIZEOF_VOID_P} EQUAL 8) # Is machine 64 bit ?
	set(ASSIMP_LINK_DIR ${DEPENDENCIES_SOURCE_DIR}/assimp/lib/Release)		# Notice that this lib has the same path in both variants, Change that later.
	set(GLEW_LINK_DIR ${DEPENDENCIES_SOURCE_DIR}/GLEW/lib/Release/x64)
	set(SDL_LINK_DIR ${DEPENDENCIES_SOURCE_DIR}/SDL/lib/x64)
	set(SDL_IMAGE_LINK_DIR ${DEPENDENCIES_SOURCE_DIR}/SDL2_image/lib/x64)
	set(SDL_MIXER_LINK_DIR ${DEPENDENCIES_SOURCE_DIR}/SDL2_mixer/lib/x64)
	set(DEVIL_LINK_DIR ${DEPENDENCIES_SOURCE_DIR}/DevIL/WIN/lib/x64/Release)
else()
	set(ASSIMP_LINK_DIR ${DEPENDENCIES_SOURCE_DIR}/assimp/lib/Release)
	set(GLEW_LINK_DIR ${DEPENDENCIES_SOURCE_DIR}/GLEW/lib/Release/Win32)
	set(SDL_LINK_DIR ${DEPENDENCIES_SOURCE_DIR}/SDL/lib/x86)
	set(SDL_IMAGE_LINK_DIR ${DEPENDENCIES_SOURCE_DIR}/SDL2_image/lib/x86)
	set(SDL_MIXER_LINK_DIR ${DEPENDENCIES_SOURCE_DIR}/SDL2_mixer/lib/x86)
    set(DEVIL_LINK_DIR ${DEPENDENCIES_SOURCE_DIR}/DevIL/WIN/lib/x86/Release)
endif()

link_directories(${ASSIMP_LINK_DIR})
link_directories(${GLEW_LINK_DIR})
link_directories(${SDL_LINK_DIR})
link_directories(${SDL_IMAGE_LINK_DIR})
link_directories(${SDL_MIXER_LINK_DIR})
link_directories(${DEVIL_LINK_DIR})

if(WIN32)
	if(MSVC)
		add_msvc_precompiler_header("stdafx.h" "stdafx.cpp" ${SourceFiles})
	endif()
endif()

add_executable(${PROJECT_NAME} ${SourceFiles})

target_link_libraries(${PROJECT_NAME} glew32.lib)
target_link_libraries(${PROJECT_NAME} SDL2.lib)
target_link_libraries(${PROJECT_NAME} SDL2main.lib)
target_link_libraries(${PROJECT_NAME} SDL2_image.lib)
target_link_libraries(${PROJECT_NAME} SDL2_mixer.lib)
target_link_libraries(${PROJECT_NAME} assimp-vc140-mt.lib)
target_link_libraries(${PROJECT_NAME} DevIL.lib)

### -=- ADDING DEPENDENCIES -=- ###

# SDL

include(${DEPENDENCIES_SOURCE_DIR}/SDL/CMakeLists.txt)
include_directories(${DEPENDENCIES_SOURCE_DIR}/SDL/include)


if(${CMAKE_SIZEOF_VOID_P} EQUAL 8) # Is machine 64 bit ?

else()

endif()
### ~=~ ADDING DEPENDENCIES ~=~ ###

# Finding OpenGL

find_package(OpenGL REQUIRED)

if(OPENGL_FOUND)
	include_directories(${OPENGL_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
endif()	

# Finding Boost

#find_package(Boost REQUIRED)

if(BOOST_FOUND)
    include_directories(${BOOST_INCLUDE_DIRS})
endif()

# Automatically paste dll files to the binary location, near executable.

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${DEPENDENCIES_SOURCE_DIR}/GLEW/bin/Release/Win32/glew32.dll"
    "${CMAKE_CURRENT_BINARY_DIR}")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL_LINK_DIR}/SDL2.dll"
    "${CMAKE_CURRENT_BINARY_DIR}")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${DEVIL_LINK_DIR}/DevIL.dll"
    "${CMAKE_CURRENT_BINARY_DIR}")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL_IMAGE_LINK_DIR}/SDL2_image.dll"
    "${CMAKE_CURRENT_BINARY_DIR}")
	
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${ASSIMP_LINK_DIR}/assimp-vc140-mt.dll"
    "${CMAKE_CURRENT_BINARY_DIR}")

