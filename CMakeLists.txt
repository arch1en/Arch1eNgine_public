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

set(SourceFiles 
	${PROJECT_SOURCE_DIR}/Main.cpp
	${PROJECT_SOURCE_DIR}/AppMain.cpp
	${PROJECT_SOURCE_DIR}/Helpers.cpp
	${PROJECT_SOURCE_DIR}/Actors/PrimitiveActors/ACube.cpp
	${PROJECT_SOURCE_DIR}/Actors/PrimitiveActors/ATriangle.cpp
	${PROJECT_SOURCE_DIR}/Actors/AActor.cpp
	${PROJECT_SOURCE_DIR}/Actors/ACamera.cpp
	${PROJECT_SOURCE_DIR}/Allocators/AllocatorGPU.cpp
	${PROJECT_SOURCE_DIR}/Allocators/AllocatorMesh.cpp
	${PROJECT_SOURCE_DIR}/Components/ActorComponent.cpp
	${PROJECT_SOURCE_DIR}/Components/BaseComponent.cpp
	${PROJECT_SOURCE_DIR}/Components/CameraComponent.cpp
	${PROJECT_SOURCE_DIR}/Components/InputComponent.cpp
	${PROJECT_SOURCE_DIR}/Components/MeshComponent.cpp
	${PROJECT_SOURCE_DIR}/Components/MovementComponent.cpp
	${PROJECT_SOURCE_DIR}/Core/Class.cpp
	${PROJECT_SOURCE_DIR}/Core/ModuleHandler.cpp
	${PROJECT_SOURCE_DIR}/Debuggers/Debugger.cpp
	${PROJECT_SOURCE_DIR}/Debuggers/DebugTimer.cpp
	${PROJECT_SOURCE_DIR}/Factories/FactoryActor.cpp
	${PROJECT_SOURCE_DIR}/Factories/FactoryMesh.cpp
	${PROJECT_SOURCE_DIR}/Factories/FactoryTexture.cpp
	${PROJECT_SOURCE_DIR}/IO/ConfigLoader.cpp
	${PROJECT_SOURCE_DIR}/IO/InputLayer.cpp
	${PROJECT_SOURCE_DIR}/IO/Paths.cpp
    ${PROJECT_SOURCE_DIR}/IO/Import/MeshImporter.cpp
	${PROJECT_SOURCE_DIR}/Math/Math.cpp
	${PROJECT_SOURCE_DIR}/Mesh/MeshBase.cpp
	${PROJECT_SOURCE_DIR}/Mesh/MeshCube.cpp
	${PROJECT_SOURCE_DIR}/Mesh/MeshTriangle.cpp
	${PROJECT_SOURCE_DIR}/Modules/InputModule.cpp
	${PROJECT_SOURCE_DIR}/Rendering/Textures/Texture.cpp
	${PROJECT_SOURCE_DIR}/Rendering/Renderer.cpp
	${PROJECT_SOURCE_DIR}/Rendering/ShaderProgram.cpp
	)

set(HeaderFiles 
	${PROJECT_SOURCE_DIR}/Main.h
	${PROJECT_SOURCE_DIR}/AppMain.h
	${PROJECT_SOURCE_DIR}/Helpers.h
	${PROJECT_SOURCE_DIR}/Actors/PrimitiveActors/ACube.h
	${PROJECT_SOURCE_DIR}/Actors/PrimitiveActors/ATriangle.h
	${PROJECT_SOURCE_DIR}/Actors/AActor.h
	${PROJECT_SOURCE_DIR}/Actors/ACamera.h
	${PROJECT_SOURCE_DIR}/Allocators/AllocatorGPU.h
	${PROJECT_SOURCE_DIR}/Allocators/AllocatorMesh.h
	${PROJECT_SOURCE_DIR}/Common/RenderingCommons.h
	${PROJECT_SOURCE_DIR}/Components/ActorComponent.h
	${PROJECT_SOURCE_DIR}/Components/BaseComponent.h
	${PROJECT_SOURCE_DIR}/Components/CameraComponent.h
	${PROJECT_SOURCE_DIR}/Components/InputComponent.h
	${PROJECT_SOURCE_DIR}/Components/MeshComponent.h
	${PROJECT_SOURCE_DIR}/Components/MovementComponent.h
	${PROJECT_SOURCE_DIR}/Core/Class.h
	${PROJECT_SOURCE_DIR}/Core/ModuleHandler.h
	${PROJECT_SOURCE_DIR}/Debuggers/Debugger.h
	${PROJECT_SOURCE_DIR}/Debuggers/DebugTimer.h
	${PROJECT_SOURCE_DIR}/Factories/FactoryActor.h
	${PROJECT_SOURCE_DIR}/Factories/FactoryMesh.h
	${PROJECT_SOURCE_DIR}/Factories/FactoryTexture.h
	${PROJECT_SOURCE_DIR}/IO/ConfigLoader.h
	${PROJECT_SOURCE_DIR}/IO/InputLayer.h
	${PROJECT_SOURCE_DIR}/IO/Paths.h
    ${PROJECT_SOURCE_DIR}/IO/Import/MeshImporter.h
	${PROJECT_SOURCE_DIR}/Math/Math.h
	${PROJECT_SOURCE_DIR}/Mesh/MeshBase.h
	${PROJECT_SOURCE_DIR}/Mesh/MeshCube.h
	${PROJECT_SOURCE_DIR}/Mesh/MeshTriangle.h
	${PROJECT_SOURCE_DIR}/Modules/InputModule.h
	${PROJECT_SOURCE_DIR}/Rendering/Textures/Texture.h
	${PROJECT_SOURCE_DIR}/Rendering/Renderer.h
	${PROJECT_SOURCE_DIR}/Rendering/ShaderProgram.h
	)

#Include Dependencies Source Paths

include_directories(${DEPENDENCIES_SOURCE_DIR}/GLM)
include_directories(${DEPENDENCIES_SOURCE_DIR}/assimp/include)
include_directories(${DEPENDENCIES_SOURCE_DIR}/GLEW/include)
include_directories(${DEPENDENCIES_SOURCE_DIR}/SDL/include)
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

