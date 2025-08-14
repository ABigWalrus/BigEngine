add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(Catch2)


set(
  EXTERNAL_INCLUDES
  glfw/include
  glm
  stb
  tinyobjloader
) 
# target_include_directories(${PROJECT_NAME} PUBLIC
# 	glfw/include
# 	glm
# 	stb
# 	tinyobjloader
# )
