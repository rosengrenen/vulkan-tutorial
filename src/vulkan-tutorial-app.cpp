#include "vulkan-tutorial-app.hpp"

#include <cstring>
#include <iostream>
#include <map>
#include <stdexcept>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

const std::vector<const char *> validation_layers = {
    "VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
constexpr bool enable_validation_layers = false;
#else
constexpr bool enable_validation_layers = true;
#endif

auto CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *create_info,
    const VkAllocationCallbacks *allocator,
    VkDebugUtilsMessengerEXT *debug_messenger) -> VkResult;
auto DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debug_messenger,
    const VkAllocationCallbacks *allocator) -> void;

auto VulkanTutorialApp::run() -> void
{
  init_window();
  init_vulkan();
  main_loop();
  cleanup();
}

auto VulkanTutorialApp::init_vulkan() -> void
{
  create_instance();
  setup_debug_messenger();
  pick_physical_device();
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
  if (enable_validation_layers)
  {
    DestroyDebugUtilsMessengerEXT(m_instance, m_debug_messenger, nullptr);
  }

  vkDestroyInstance(m_instance, nullptr);

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

auto VulkanTutorialApp::create_instance() -> void
{
  if (enable_validation_layers && !check_validation_layer_support())
  {
    throw std::runtime_error("Validation layers requested, but not available!");
  }

  VkApplicationInfo app_info = {};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "Vulkan Tutorial";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "No Engine";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;

  auto extensions = get_required_extensions();
  create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  create_info.ppEnabledExtensionNames = extensions.data();

  VkDebugUtilsMessengerCreateInfoEXT debug_create_info;
  if (enable_validation_layers)
  {
    create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
    create_info.ppEnabledLayerNames = validation_layers.data();

    populate_debug_messenger_create_info(debug_create_info);
    create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debug_create_info;
  }
  else
  {
    create_info.enabledLayerCount = 0;

    create_info.pNext = nullptr;
  }

  if (vkCreateInstance(&create_info, nullptr, &m_instance) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create vulkan instance!");
  }

  uint32_t extension_count = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

  std::vector<VkExtensionProperties> available_extensions(extension_count);
  vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, available_extensions.data());

  std::cout << "Available extensions:\n";
  for (const auto &extension : available_extensions)
  {
    std::cout << "\t" << extension.extensionName << "\n";
  }
}

auto VulkanTutorialApp::check_validation_layer_support() -> bool
{
  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

  std::cout << "Available layers:\n";
  for (const auto &layer : available_layers)
  {
    std::cout << "\t" << layer.layerName << "\n";
  }

  for (const char *layer_name : validation_layers)
  {
    bool layer_found = false;

    for (const auto &layer_properties : available_layers)
    {
      if (strcmp(layer_name, layer_properties.layerName) == 0)
      {
        layer_found = true;
        break;
      }
    }

    if (!layer_found)
    {
      return false;
    }
  }

  return true;
}

auto VulkanTutorialApp::get_required_extensions() -> std::vector<const char *>
{
  uint32_t glfw_extension_count = 0;
  const char **glfw_extensions;
  glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  std::vector<const char *> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

  if (enable_validation_layers)
  {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}

auto VulkanTutorialApp::setup_debug_messenger() -> void
{
  if (!enable_validation_layers)
  {
    return;
  }

  VkDebugUtilsMessengerCreateInfoEXT create_info;
  populate_debug_messenger_create_info(create_info);

  if (CreateDebugUtilsMessengerEXT(m_instance, &create_info, nullptr, &m_debug_messenger) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to set up debug messenger!");
  }
}

auto VulkanTutorialApp::populate_debug_messenger_create_info(
    VkDebugUtilsMessengerCreateInfoEXT &create_info) -> void
{
  create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  create_info.pfnUserCallback = s_debug_callback;
}

auto VulkanTutorialApp::pick_physical_device() -> void
{
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr);

  if (device_count == 0)
  {
    throw std::runtime_error("Failed to find any GPU with Vulkan support!");
  }

  std::vector<VkPhysicalDevice> devices(device_count);
  vkEnumeratePhysicalDevices(m_instance, &device_count, devices.data());

  std::cout << "Available GPUs:\n";
  for (const auto &device : devices)
  {
    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(device, &device_properties);
    std::cout << "\t" << device_properties.deviceName << "\n";
  }

  // Use an ordered map to automatically sort candidates by increasing score
  std::multimap<int, VkPhysicalDevice> candidates;

  for (const auto &device : devices)
  {
    int score = rate_device_suitability(device);
    candidates.insert(std::make_pair(score, device));
  }

  // Check if the best candidate is suitable at all
  if (candidates.rbegin()->first > 0)
  {
    m_physical_device = candidates.rbegin()->second;
  }
  else
  {
    throw std::runtime_error("Failed to find a suitable GPU!");
  }
}

auto VulkanTutorialApp::rate_device_suitability(VkPhysicalDevice device) -> int
{
  QueueFamilyIndices indices = find_queue_families(device);

  if (!indices.is_complete())
  {
    return 0;
  }

  VkPhysicalDeviceProperties device_properties;
  VkPhysicalDeviceFeatures device_features;
  vkGetPhysicalDeviceProperties(device, &device_properties);
  vkGetPhysicalDeviceFeatures(device, &device_features);

  int score = 0;

  // Discrete GPUs have a significant performance advantage
  if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
  {
    score += 1000;
  }

  // Maximum possible size of textures affects graphics quality
  score += device_properties.limits.maxImageDimension2D;

  // Application can't function without geometry shaders
  if (!device_features.geometryShader)
  {
    return 0;
  }

  return score;
}

auto VulkanTutorialApp::find_queue_families(VkPhysicalDevice device) -> QueueFamilyIndices
{
  QueueFamilyIndices indices;

  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

  std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

  int i = 0;
  for (const auto &queue_family : queue_families)
  {
    if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      indices.graphics_family = i;
    }

    if (indices.is_complete())
    {
      break;
    }

    i++;
  }

  return indices;
}

auto VulkanTutorialApp::s_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
    void *user_data) -> VKAPI_ATTR VkBool32 VKAPI_CALL
{
  std::cerr << "Validation layer: " << callback_data->pMessage << std::endl;

  return VK_FALSE;
}

auto CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *create_info,
    const VkAllocationCallbacks *allocator,
    VkDebugUtilsMessengerEXT *debug_messenger) -> VkResult
{
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr)
  {
    return func(instance, create_info, allocator, debug_messenger);
  }
  else
  {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

auto DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debug_messenger,
    const VkAllocationCallbacks *allocator) -> void
{
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr)
  {
    func(instance, debug_messenger, allocator);
  }
}
