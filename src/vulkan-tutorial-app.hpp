#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class VulkanTutorialApp
{
public:
  auto run() -> void;
  auto init_vulkan() -> void;
  auto main_loop() -> void;
  auto cleanup() -> void;

private:
  GLFWwindow *m_window;
  VkInstance m_instance;
  VkDebugUtilsMessengerEXT m_debug_messenger;

  auto init_window() -> void;
  auto create_instance() -> void;
  auto check_validation_layer_support() -> bool;
  auto get_required_extensions() -> std::vector<const char *>;
  auto setup_debug_messenger() -> void;
  auto populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT &create_info) -> void;

  static auto s_debug_callback(
      VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
      VkDebugUtilsMessageTypeFlagsEXT message_type,
      const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
      void *user_data) -> VKAPI_ATTR VkBool32 VKAPI_CALL;
};
