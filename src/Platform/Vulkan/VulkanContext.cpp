#include <GLFW/glfw3.h>
#include <Vortex/Platform/Vulkan/VulkanContext.hpp>
#include <Vortex/Platform.hpp>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>
#include <vector>
#include <map>

using namespace Vortex::Vulkan;

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator,
                                      VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

namespace Vortex::Vulkan {
#ifdef VT_DEBUG
bool enableValidationLayers = true;
#else
bool enableValidationLayers = false;
#endif

#ifdef VT_PLATFORM_APPLE
bool useMoltenVk = true;
#else
bool useMoltenVk = false;
#endif

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                          VkDebugUtilsMessageTypeFlagsEXT,
                                                          const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                          void*) {
    switch (messageSeverity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        spdlog::debug("Vulkan validation layer: {}", pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        spdlog::info("Vulkan validation layer: {}", pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        spdlog::warn("Vulkan validation layer: {}", pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        spdlog::error("Vulkan validation layer: {}", pCallbackData->pMessage);
        break;
    default:
        break;
    }

    return VK_FALSE;
}

namespace Utils {
const std::unordered_map<const char*, bool> CheckValidationLayerSupport(const std::vector<const char*> layers) {
    std::unordered_map<const char*, bool> result;

    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : layers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                result[layerName] = true;
                break;
            }
        }

        if (!layerFound) {
            result[layerName] = false;
        }
    }

    return result;
}

int ScorePhysicalDevice(VkPhysicalDevice device, ProjectRequirements requirements) {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    int score = 0;

    switch (deviceProperties.deviceType) {
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
        if (requirements.IntegratedGPU == -1 || requirements.NoGPU == -1)
            return -1;
        score += requirements.DiscreteGPU;
        break;
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
        if (requirements.DiscreteGPU == -1 || requirements.NoGPU == -1)
            return -1;
        score += requirements.IntegratedGPU;
        break;
    case VK_PHYSICAL_DEVICE_TYPE_CPU:
        if (requirements.DiscreteGPU == -1 || requirements.IntegratedGPU == -1)
            return -1;
        score += requirements.NoGPU;
        break;
    case VK_PHYSICAL_DEVICE_TYPE_OTHER:
        return -1;
        break;
    default:
        break;
    }

    if (requirements.GeometryShader == -1 && !deviceFeatures.geometryShader)
        return -1;

    if (requirements.TeslationShader == -1 && !deviceFeatures.tessellationShader)
        return -1;

    if (requirements.SamplerAnisotropy == -1 && !deviceFeatures.samplerAnisotropy)
        return -1;

    if (deviceFeatures.geometryShader)
        score += requirements.GeometryShader;

    if (deviceFeatures.tessellationShader)
        score += requirements.TeslationShader;

    if (deviceFeatures.samplerAnisotropy)
        score += requirements.SamplerAnisotropy;


    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);
    uint32_t heapCount = memoryProperties.memoryHeapCount;
    VkDeviceSize heapSizesMegs = 0;
    for (uint32_t i = 0; i < heapCount; ++i) {
        VkMemoryHeap heap = memoryProperties.memoryHeaps[i];
        if (heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
            heapSizesMegs += heap.size / 1024 / 1024;
        }
    }

    if (heapSizesMegs < requirements.MinHeapMegabyte)
        return -1;

    score += requirements.HeapMegabyte * heapSizesMegs;

    return score;
}

VkPhysicalDevice ChoosePhysicalDevice(VkInstance instance, ProjectRequirements requirements) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0)
        return VK_NULL_HANDLE; // No device

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    std::multimap<int, VkPhysicalDevice> deviceScores;

    for (const auto& device : devices)
        deviceScores.insert(std::make_pair(ScorePhysicalDevice(device, requirements), device));

    if (deviceScores.rbegin()->first < 0)
        return VK_NULL_HANDLE; // No suitable device

    return deviceScores.rbegin()->second;
}
} // namespace Utils
} // namespace Vortex::Vulkan

void VulkanContext::Init() {
    ZoneScoped;

    m_PhysicalDevice = VK_NULL_HANDLE;

    std::vector<const char*> extensions = {};
    std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    auto validationLayerSupport = Utils::CheckValidationLayerSupport(validationLayers);
    for (auto [layerName, available] : validationLayerSupport) {
        if (!available) {
            spdlog::warn("Vulkan Validation Layer '{}' is not available but requested", layerName);
            validationLayers.erase(std::remove(validationLayers.begin(), validationLayers.end(), layerName), validationLayers.end());
        }
    }

    if (validationLayers.size() <= 0)
        enableValidationLayers = false;

    auto windowExtensions = m_Window->GetVulkanInstanceExtensions();
    extensions.insert(extensions.end(), windowExtensions.begin(), windowExtensions.end());

    if (enableValidationLayers)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    if (useMoltenVk)
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    VkApplicationInfo appInfo {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = VK_API_VERSION_1_3;
    appInfo.pApplicationName = "Vortex Renderer";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo instanceCreateInfo {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
    if (enableValidationLayers) {
        instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();

        VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo {};
        debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugMessengerCreateInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugMessengerCreateInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugMessengerCreateInfo.pfnUserCallback = VulkanDebugCallback;

        instanceCreateInfo.pNext = &debugMessengerCreateInfo;
    }

#ifdef VT_PLATFORM_APPLE
    instanceCreateInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

    VT_ASSERT(vkCreateInstance(&instanceCreateInfo, nullptr, &m_Instance) == VK_SUCCESS, "Failed to create Vulkan instance");

    if (enableValidationLayers) {
        VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo {};
        debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugMessengerCreateInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugMessengerCreateInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugMessengerCreateInfo.pfnUserCallback = VulkanDebugCallback;

        VT_ASSERT(CreateDebugUtilsMessengerEXT(m_Instance, &debugMessengerCreateInfo, nullptr, &m_DebugMessenger) == VK_SUCCESS, "Failed to create Vulkan debug messenger");
    }

    m_PhysicalDevice = Utils::ChoosePhysicalDevice(m_Instance, m_Requirements);
    VT_ASSERT(m_PhysicalDevice != VK_NULL_HANDLE, "No suitable physical device found");

    spdlog::info("Created Vulkan context");
    spdlog::info("Using Vulkan version 1.3");
}

void VulkanContext::Destroy() {
    if (enableValidationLayers)
        DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);

    vkDestroyInstance(m_Instance, nullptr);
}
