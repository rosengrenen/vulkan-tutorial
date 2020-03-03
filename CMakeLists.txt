cmake_minimum_required(VERSION 3.15)

project(vulkan-tutorial)

include(FetchContent)
include(FindVulkan)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG        3.3.2
)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm
  GIT_TAG        0.9.9.7
)

FetchContent_MakeAvailable(glm)

add_subdirectory(src)
