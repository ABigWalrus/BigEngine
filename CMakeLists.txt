cmake_minimum_required (VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project (BigEngine)
set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)
set(VULKAN_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include")

add_subdirectory(external)
add_subdirectory(src)
add_subdirectory(test)


find_program(GLSL_VALIDATOR glslangValidator HINTS 
	${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} 
	/usr/bin 
	/usr/local/bin 
	${VULKAN_SDK_PATH}/Bin
	${VULKAN_SDK_PATH}/Bin32
	$ENV{VULKAN_SDK}/Bin/ 
	$ENV{VULKAN_SDK}/Bin32/
)

set(SHADERS_SOURCE_DIR "${PROJECT_SOURCE_DIR}/assets/shaders")

file(GLOB_RECURSE VERTEX_SHADERS
	"${SHADERS_SOURCE_DIR}/*.vert"
)

file(GLOB_RECURSE FRAGMENT_SHADERS
	"${SHADERS_SOURCE_DIR}/*.frag"
)

foreach(SHADER ${VERTEX_SHADERS})
	set(SPIRV "${SHADERS_SOURCE_DIR}/vert.spv")
	add_custom_command(
		OUTPUT ${SPIRV}
		COMMAND ${GLSL_VALIDATOR} -V ${SHADER} -o ${SPIRV}
		DEPENDS ${SHADER}
	)
	list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(SHADER)

foreach(SHADER ${FRAGMENT_SHADERS})
	set(SPIRV "${SHADERS_SOURCE_DIR}/frag.spv")
	add_custom_command(
		OUTPUT ${SPIRV}
		COMMAND ${GLSL_VALIDATOR} -V ${SHADER} -o ${SPIRV}
		DEPENDS ${SHADER}
	)
	list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(SHADER)

add_custom_target(
    default ALL
    DEPENDS ${SPIRV_BINARY_FILES}
)


