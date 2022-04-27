#pragma once

//libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//std
#include <stdexcept>
#include <vector>
#include <array>
#include <set>
#include <iostream>
#include <algorithm>
#include <optional>
#include <fstream>
#include <chrono>

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

#define MAX_FRAMES_IN_FLIGHT 2

namespace HT
{

  struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

     VkVertexInputBindingDescription static getBindingDescription(){
      
      VkVertexInputBindingDescription bindingDescription{};
      bindingDescription.binding = 0;
      bindingDescription.stride = sizeof(Vertex);
      bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
      return bindingDescription;
    }

     std::array<VkVertexInputAttributeDescription, 2> static getAttributeDescriptions(){
      
      std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
      attributeDescriptions[0].binding = 0;
      attributeDescriptions[0].location = 0;
      attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
      attributeDescriptions[0].offset = offsetof(Vertex, pos);

      attributeDescriptions[1].binding = 0;
      attributeDescriptions[1].location = 1;
      attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
      attributeDescriptions[1].offset = offsetof(Vertex, color);

      return attributeDescriptions;
    }
  };

  struct QueueFamilyIndices{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete(){
      return graphicsFamily.has_value() && presentFamily.has_value();
    }
  };

  struct SwapChainSupportDetails{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

  };

  struct UniformBufferObject{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
  };

  class HelloTriangleApp
  {

  public:
     void run();

    HelloTriangleApp() = default;
    HelloTriangleApp(const HelloTriangleApp&) = delete;
    HelloTriangleApp(const HelloTriangleApp&&) = delete;

    HelloTriangleApp& operator=(const HelloTriangleApp&)  = delete;
  private:
     void initWindow();
     void initVulkan();
     void mainLoop();
     void cleanup();

     void createInstance();
     bool checkValidationLayerSupport();

     void setupDebugMessenger();
     VkResult CreateDebugUtilsMessengerEXT(
      VkInstance instance, 
      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
      const VkAllocationCallbacks* pAllocator,
      VkDebugUtilsMessengerEXT* pDebugMessenger);
     void DestroyDebugUtilsMessengerEXT(
      VkInstance instance, 
      const VkDebugUtilsMessengerEXT DebugMessenger,
      const VkAllocationCallbacks* pAllocator);
     void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);


     std::vector<const char*> getRequiredExtensions();
     static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(  
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
      VkDebugUtilsMessageTypeFlagsEXT messageType,
      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
      void* pUserData);

     void createSurface();

     void pickPhysicalDevice();

     bool isDeviceSuitable(VkPhysicalDevice device);
     QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
     bool checkDeviceExtensionSupport(VkPhysicalDevice device);
     SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

     void createLogicalDevice();

     void createSwapChain();
     VkSurfaceFormatKHR chooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& availbaleFormats);
     VkPresentModeKHR chooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
     VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

     void createImageViews();

     void createRenderPass();

     void createGraphicsPipeline();

     std::vector<char> readFile(const std::string& filename);
     VkShaderModule createShaderModule(const std::vector<char>& code);

     void createFrameBuffers();

     void createCommandPool();
     void createCommandBuffers();
     void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

     void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
     void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    
     void createUniformBuffers();
     void createDescriptorPool();
     void createDescriptorSets();
     void updateUniformBuffer(uint32_t currentImage);

     void createVertexBuffer();
     void createIndexBuffer();

     void drawFrame();

     void createSyncObjects();

     void recreateSwapChain();
     void cleanupSwapChain();

     void createDescriptorSetLayout();

     uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

  public:
     const uint32_t width = 800;
     const uint32_t height = 600;
  
    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
    const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    const std::vector<Vertex> vertices = {
      {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
      {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
      0, 1, 2, 2, 3, 0
    };

  private:
    GLFWwindow* window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSurfaceKHR surface;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkExtent2D swapChainExtent;
    VkFormat swapChainImageFormat;

    std::vector<VkImageView> swapChainImageViews;

    VkRenderPass renderPass;

    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    std::vector<VkFramebuffer> swapChainFrameBuffers;

    VkCommandPool commandPool;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightfences;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    uint32_t currentFrame = 0;

  };

} // namespace HT