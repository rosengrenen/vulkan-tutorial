add_executable(
  vulkan-tutorial
	main.cpp
	vulkan-tutorial-app.cpp
)

set_property(TARGET vulkan-tutorial PROPERTY CXX_STANDARD 17)

target_include_directories(
	vulkan-tutorial
	PRIVATE
	${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(
  vulkan-tutorial
	PRIVATE
	glfw
	glm
	${Vulkan_LIBRARIES}
)
