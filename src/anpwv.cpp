#define GLFW_INCLUDE_VULKAN
#include <stdio.h>
#include <assert.h>
#include <GLFW/glfw3.h>


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

    const char *instanceExtensions[1] = { "VK_KHR_surface" };
    const char *enabledLayers[1] = { "VK_LAYER_KHRONOS_validation" };

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &applicationInfo;
    createInfo.enabledExtensionCount = 1;
    createInfo.ppEnabledExtensionNames = instanceExtensions;
    createInfo.enabledLayerCount = 1;
    createInfo.ppEnabledLayerNames = enabledLayers;

    VkInstance instance;
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    check_vulkan_result(&result, "Cannot initialize a VkInstance");

    return 0;
}

//NOTE: glfw_main will be our main later.
void glfw_error_callback(int error, const char *description) {
    fprintf(stderr, "GLFW ERROR: %s\n", description);
}

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
