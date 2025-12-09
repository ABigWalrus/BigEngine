set(SOURCE_FILES
  core.cpp
)

set(PROJECT_NAME_LIB "${PROJECT_NAME}lib")

add_library(
  "${PROJECT_NAME}lib"
  ${SOURCE_FILES}
)


target_include_directories("${PROJECT_NAME}lib" PUBLIC
	${VULKAN_INCLUDE_DIRS}
  ${EXTERNAL_INCLUDES}
  "${CMAKE_SOURCE_DIR}/include"
)



target_link_libraries("${PROJECT_NAME}lib" glfw ${Vulkan_LIBRARIES} glm::glm)


add_executable(
    ${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} "${PROJECT_NAME}lib")
