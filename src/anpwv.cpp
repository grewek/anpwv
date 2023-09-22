#define GLFW_INCLUDE_VULKAN
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <GLFW/glfw3.h>

void glfw_error_callback(int error, const char *description) {
    fprintf(stderr, "GLFW ERROR: %s\n", description);
}

void check_vulkan_result(VkResult *result, const char *msg) {
    if(*result != VK_SUCCESS) {
         fprintf(stderr, "VULKAN_ERROR: %s\n", msg);
         assert(*result == VK_SUCCESS);   
    }

    return;
}

int main(int argc, char *argv[]) {
    //Create and Setup a Vulkan Instance with validation layers enabled !
    VkApplicationInfo applicationInfo = {};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.apiVersion = VK_API_VERSION_1_3;

    const char *instanceExtensions[3] = { "VK_KHR_surface", "VK_KHR_xcb_surface", "VK_EXT_debug_utils" };
    const char *enabledLayers[1] = { "VK_LAYER_KHRONOS_validation" };

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &applicationInfo;
    createInfo.enabledExtensionCount = 3;
    createInfo.ppEnabledExtensionNames = instanceExtensions;
    createInfo.enabledLayerCount = 1;
    createInfo.ppEnabledLayerNames = enabledLayers;

    VkInstance instance;
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    check_vulkan_result(&result, "Cannot initialize a VkInstance");
    //------

    //Setup a debug utils
        //TODO: We need a working allocator for the Debug Messegner !
    //List all PhysicalDevices and select the first one
    uint32_t count;
    vkEnumeratePhysicalDevices(instance, &count, nullptr);
    assert(count > 0);

    VkPhysicalDevice *physicalDevices = (VkPhysicalDevice *)calloc(count, sizeof(VkPhysicalDevice));
    if(physicalDevices == nullptr) {
        fprintf(stderr, "ERROR: could not allocate memory for physicaldevice listing\n");
        return -1;
    }

    vkEnumeratePhysicalDevices(instance, &count, physicalDevices);
    VkPhysicalDevice physicalDevice = physicalDevices[0];
    //-----

    //Create a Logical Device
    float priority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = 0;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &priority;

    const char *enabledExtensions = { "VK_KHR_swapchain" };

    VkDeviceCreateInfo createInfoQueue = {};
    createInfoQueue.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfoQueue.queueCreateInfoCount = 1;
    createInfoQueue.pQueueCreateInfos = &queueCreateInfo;
    createInfoQueue.enabledExtensionCount = 1;
    createInfoQueue.ppEnabledExtensionNames = &enabledExtensions;

    VkDevice device;
    VkResult deviceResult = vkCreateDevice(physicalDevice, &createInfoQueue, nullptr, &device);
    check_vulkan_result(&deviceResult, "Could not create logical device");
    
    //Create a surface via GLFW we need to set GLFW_NO_API in order to do this
    GLFWwindow *window;

    if(!glfwInit()) return -1;
    glfwSetErrorCallback(glfw_error_callback);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);

    if(!window) {
        glfwTerminate();
        return -1;
    }

    VkSurfaceKHR surface;
    VkResult surfaceResult = glfwCreateWindowSurface(instance, window, NULL, &surface);

    //Setup a Swapchain with four images !
    VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.surface = surface;
    swapchainCreateInfo.minImageCount = 4;
    swapchainCreateInfo.imageFormat = VK_FORMAT_R8G8B8A8_SRGB;
    swapchainCreateInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    swapchainCreateInfo.imageExtent = VkExtent2D { 1920, 1080 };
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCreateInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;

    VkSwapchainKHR swapchain;
    vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain); 

    glfwMakeContextCurrent(window);

    while(!glfwWindowShouldClose(window)) {
        fprintf(stdout, "Acquire Image\n");
        fprintf(stdout, "Draw Image\n");
        fprintf(stdout, "Present Image\n");
    }

    glfwTerminate();

    free(physicalDevices);
    //TODO: Right now the validation layer is not very happy about the way i am ignoring certain fields
    //  this must be fixed!
    return 0;
}

//NOTE: glfw_main will be our main later.


int glfw_main() {
    GLFWwindow *window;

    if(!glfwInit()) return -1;
    glfwSetErrorCallback(glfw_error_callback);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    if(!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
