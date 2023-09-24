#pragma once

struct VulkanDeviceContainer {
    VkInstance instance;
    VkDevice logicalDevice;
    VkPhysicalDevice physicalDevice;
};

VkInstance _CreateVulkanInstance(uint32_t vulkanVersion, const char *vkExtensions, uint32_t extensionCount, const char *vkLayers, uint32_t layerCount)
{
    VkApplicationInfo applicationInfo = {};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.apiVersion = vulkanVersion;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &applicationInfo;
    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = &vkExtensions;
    createInfo.enabledLayerCount = layerCount;
    createInfo.ppEnabledLayerNames = &vkLayers;

    VkInstance instance;
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

    //TODO(Kay): Make sure our instance is valid 
    return instance;
}

VkPhysicalDevice _CreatePhysicalDevice(VkInstance *instance) {
    uint32_t count;
    vkEnumeratePhysicalDevices(*instance, &count, nullptr);
    assert(count > 0);

    VkPhysicalDevice *physicalDevices = (VkPhysicalDevice *)calloc(count, sizeof(VkPhysicalDevice));
    if(physicalDevices == nullptr) {
        fprintf(stderr, "ERROR: could not allocate memory for physicaldevice listing\n");
        //TODO(Kay): Error handling needs some work...
    }

    vkEnumeratePhysicalDevices(*instance, &count, physicalDevices);
    VkPhysicalDevice physicalDevice = physicalDevices[0]; //TODO(Kay): Again we need to make sure the pyhsical device is valid __AND__ is existing inside the array !

    free(physicalDevices);

    return physicalDevice;
}

VkDevice _CreateLogicalDevice(VkPhysicalDevice *physicalDevice, float priority, uint32_t queueFamilyIndex, uint32_t queueCount, const char *queueExtensions, uint32_t queueExtensionCount) {
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = 0;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &priority;

    //const char *enabledExtensions = { "VK_KHR_swapchain" };

    VkDeviceCreateInfo createInfoQueue = {};
    createInfoQueue.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfoQueue.queueCreateInfoCount = 1;
    createInfoQueue.pQueueCreateInfos = &queueCreateInfo;
    createInfoQueue.enabledExtensionCount = queueExtensionCount;
    createInfoQueue.ppEnabledExtensionNames = &queueExtensions;

    VkDevice device;
    VkResult deviceResult = vkCreateDevice(*physicalDevice, &createInfoQueue, nullptr, &device);

    return device;
}

VulkanDeviceContainer NewVulkanDeviceContainer(uint32_t vulkanVersion, const char *vkExtensions, uint32_t extensionCount, 
        const char *vkLayers, uint32_t layerCount)
{
    VulkanDeviceContainer result = {};
    result.instance = _CreateVulkanInstance(vulkanVersion, vkExtensions, extensionCount, vkLayers, layerCount);
    result.physicalDevice = _CreatePhysicalDevice(&result.instance);
    result.logicalDevice = _CreateLogicalDevice(&result.physicalDevice, 1.0f, 0, 1, { "VK_KHR_swapchain" }, 1);
    //TODO(Kay): Make sure our instance is valid 
    //           For now we just put the instance into our result what could possibly go wrong ?
    return result;
}
