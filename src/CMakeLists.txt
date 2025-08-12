set(SOURCE_FILES
  main.cpp
  entity.cpp
)

add_executable(
	${PROJECT_NAME}
	${SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${VULKAN_INCLUDE_DIRS}
  ${EXTERNAL_INCLUDES}
	./
)



target_link_libraries(${PROJECT_NAME} glfw ${Vulkan_LIBRARIES} glm::glm Catch2::Catch2)
