#include "vulkan-tutorial-app.hpp"

#include <stdexcept>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

auto VulkanTutorialApp::run() -> void
{
  init_window();
  init_vulkan();
  main_loop();
  cleanup();
}

auto VulkanTutorialApp::init_vulkan() -> void
{
}

auto VulkanTutorialApp::main_loop() -> void
{
  while (!glfwWindowShouldClose(m_window))
  {
    glfwPollEvents();
  }
}

auto VulkanTutorialApp::cleanup() -> void
{
  glfwDestroyWindow(m_window);

  glfwTerminate();
}

auto VulkanTutorialApp::init_window() -> void
{
  if (!glfwInit())
  {
    throw std::runtime_error("Could not initialize glfw");
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan tutorial", nullptr, nullptr);
}
