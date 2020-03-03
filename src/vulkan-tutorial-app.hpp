#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>
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
  VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;

  struct QueueFamilyIndices
  {
    std::optional<uint32_t> graphics_family;

    auto is_complete() -> bool
    {
      return graphics_family.has_value();
    }
  };

  auto init_window() -> void;
  auto create_instance() -> void;
  auto check_validation_layer_support() -> bool;
  auto get_required_extensions() -> std::vector<const char *>;
  auto setup_debug_messenger() -> void;
  auto populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT &create_info) -> void;
  auto pick_physical_device() -> void;
  auto rate_device_suitability(VkPhysicalDevice device) -> int;
  auto find_queue_families(VkPhysicalDevice device) -> QueueFamilyIndices;

  static auto s_debug_callback(
      VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
      VkDebugUtilsMessageTypeFlagsEXT message_type,
      const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
      void *user_data) -> VKAPI_ATTR VkBool32 VKAPI_CALL;
};
