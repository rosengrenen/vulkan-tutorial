#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class VulkanTutorialApp
{
public:
  auto run() -> void;
  auto init_vulkan() -> void;
  auto main_loop() -> void;
  auto cleanup() -> void;

private:
  GLFWwindow *m_window;

  auto init_window() -> void;
};
